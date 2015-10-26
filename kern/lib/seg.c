#include <lib/gcc.h>
#include <lib/x86.h>
#include <lib/string.h>
#include <lib/types.h>
#include <lib/kstack.h>

#include "seg.h"

#define offsetof(type, member)	__builtin_offsetof(type, member)

void kstack_switch(uint32_t pid)
{
  int cpu_idx = get_pcpu_idx();

  struct kstack *ks = (struct kstack *) get_pcpu_kstack_pointer(cpu_idx);

  /*
   * Switch to the new TSS.
   */
  ks->tss.ts_esp0 = (uint32_t) proc_kstack[pid].kstack_hi;
  ks->tss.ts_ss0 = CPU_GDT_KDATA;
  ks->gdt[CPU_GDT_TSS >> 3] =
          SEGDESC16(STS_T32A,
                    (uint32_t) (&(proc_kstack[pid].tss)), sizeof(tss_t) - 1, 0);
  ks->gdt[CPU_GDT_TSS >> 3].sd_s = 0;
  ltr(CPU_GDT_TSS);

}

void seg_init (int cpu_idx)
{

	/* clear BSS */

	if (cpu_idx == 0){
		extern uint8_t end[], edata[];
    memzero(edata, ((uint8_t *) (&bsp_kstack[0])) - edata);
    memzero(((uint8_t *) (&bsp_kstack[0])) + 4096, end - ((uint8_t *) (&bsp_kstack[0])) - 4096);
	}

  /* setup GDT */
  bsp_kstack[cpu_idx].gdt[0] = SEGDESC_NULL;
  /* 0x08: kernel code */
  bsp_kstack[cpu_idx].gdt[CPU_GDT_KCODE >> 3] =
          SEGDESC32(STA_X | STA_R, 0x0, 0xffffffff, 0);
  /* 0x10: kernel data */
  bsp_kstack[cpu_idx].gdt[CPU_GDT_KDATA >> 3] =
          SEGDESC32(STA_W, 0x0, 0xffffffff, 0);
  /* 0x18: user code */
  bsp_kstack[cpu_idx].gdt[CPU_GDT_UCODE >> 3] =
          SEGDESC32(STA_X | STA_R, 0x00000000, 0xffffffff, 3);
  /* 0x20: user data */
  bsp_kstack[cpu_idx].gdt[CPU_GDT_UDATA >> 3] =
          SEGDESC32(STA_W, 0x00000000, 0xffffffff, 3);

  /* setup TSS */
  bsp_kstack[cpu_idx].tss.ts_esp0 = (uint32_t) bsp_kstack[cpu_idx].kstack_hi;
  bsp_kstack[cpu_idx].tss.ts_ss0 = CPU_GDT_KDATA;
  bsp_kstack[cpu_idx].gdt[CPU_GDT_TSS >> 3] =
          SEGDESC16(STS_T32A,
                    (uint32_t) (&(bsp_kstack[cpu_idx].tss)), sizeof(tss_t) - 1, 0);
  bsp_kstack[cpu_idx].gdt[CPU_GDT_TSS >> 3].sd_s = 0;

  /* other fields */
  /* Set the KSTACK_MAGIC value when we initialize the kstack */
  bsp_kstack[cpu_idx].magic = KSTACK_MAGIC;

  pseudodesc_t gdt_desc = {
          .pd_lim   = sizeof(bsp_kstack[cpu_idx].gdt) - 1,
          .pd_base  = (uint32_t) (bsp_kstack[cpu_idx].gdt)
  };
  asm volatile("lgdt %0" :: "m" (gdt_desc));
  asm volatile("movw %%ax,%%gs" :: "a" (CPU_GDT_KDATA));
  asm volatile("movw %%ax,%%fs" :: "a" (CPU_GDT_KDATA));
  asm volatile("movw %%ax,%%es" :: "a" (CPU_GDT_KDATA));
  asm volatile("movw %%ax,%%ds" :: "a" (CPU_GDT_KDATA));
  asm volatile("movw %%ax,%%ss" :: "a" (CPU_GDT_KDATA));
  /* reload %cs */
  asm volatile("ljmp %0,$1f\n 1:\n" :: "i" (CPU_GDT_KCODE));

	/*
	 * Load a null LDT.
	 */
	lldt (0);

	/*
	 * Load the bootstrap TSS.
	 */
	ltr (CPU_GDT_TSS);

	/*
	 * Load IDT.
	 */
	extern pseudodesc_t idt_pd;
	asm volatile("lidt %0" : : "m" (idt_pd));

	/*
	 * Initialize all TSS structures for processes.
	 */

	if (cpu_idx == 0){
		memzero(&(bsp_kstack[1]), sizeof(struct kstack) * 7);
		memzero(proc_kstack, sizeof(struct kstack) * 64);
	}
	
}

/* initialize the kernel stack for each process */
void
seg_init_proc(int cpu_idx, int pid){

  /* setup GDT */
  proc_kstack[pid].gdt[0] = SEGDESC_NULL;
  /* 0x08: kernel code */
  proc_kstack[pid].gdt[CPU_GDT_KCODE >> 3] =
          SEGDESC32(STA_X | STA_R, 0x0, 0xffffffff, 0);
  /* 0x10: kernel data */
  proc_kstack[pid].gdt[CPU_GDT_KDATA >> 3] =
          SEGDESC32(STA_W, 0x0, 0xffffffff, 0);
  /* 0x18: user code */
  proc_kstack[pid].gdt[CPU_GDT_UCODE >> 3] =
          SEGDESC32(STA_X | STA_R, 0x00000000, 0xffffffff, 3);
  /* 0x20: user data */
  proc_kstack[pid].gdt[CPU_GDT_UDATA >> 3] =
          SEGDESC32(STA_W, 0x00000000, 0xffffffff, 3);

  /* setup TSS */
  proc_kstack[pid].tss.ts_esp0 = (uint32_t) proc_kstack[pid].kstack_hi;
  proc_kstack[pid].tss.ts_ss0 = CPU_GDT_KDATA;
  proc_kstack[pid].tss.ts_iomb = offsetof(tss_t, ts_iopm);
  memzero (proc_kstack[pid].tss.ts_iopm, sizeof(uint8_t) * 128);
  proc_kstack[pid].tss.ts_iopm[128] = 0xff;

  proc_kstack[pid].gdt[CPU_GDT_TSS >> 3] =
          SEGDESC16(STS_T32A,
                    (uint32_t) (&(proc_kstack[pid].tss)), sizeof(tss_t) - 1, 0);
  proc_kstack[pid].gdt[CPU_GDT_TSS >> 3].sd_s = 0;

  /* other fields */
  proc_kstack[pid].magic = KSTACK_MAGIC;

  proc_kstack[pid].cpu_idx = cpu_idx;

}

