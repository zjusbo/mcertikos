#ifndef _KERN_X86_H_
#define _KERN_X86_H_

#ifdef _KERN_

#include <lib/types.h>

/* CR0 */
#define CR0_PE		0x00000001	/* Protection Enable */
#define CR0_MP		0x00000002	/* Monitor coProcessor */
#define CR0_EM		0x00000004	/* Emulation */
#define CR0_TS		0x00000008	/* Task Switched */
#define CR0_NE		0x00000020	/* Numeric Errror */
#define CR0_WP		0x00010000	/* Write Protect */
#define CR0_AM		0x00040000	/* Alignment Mask */
#define CR0_PG		0x80000000	/* Paging */

/* CR4 */
#define CR4_PGE		0x00000080	/* Page Global Enable */
#define CR4_OSFXSR	0x00000200	/* SSE and FXSAVE/FXRSTOR enable */
#define CR4_OSXMMEXCPT	0x00000400	/* Unmasked SSE FP exceptions */

/* EFER */
#define MSR_EFER	0xc0000080
# define MSR_EFER_SVME	(1<<12)		/* for AMD processors */

/* sysenter */
#define SYSENTER_CS_MSR     0x174u
#define SYSENTER_ESP_MSR    0x175u
#define SYSENTER_EIP_MSR    0x176u

/* paging */
#define PAGESIZE	4096
#define PTE_P   0x001 // Present
#define PTE_W   0x002 // Writeable
#define PTE_U   0x004 // User
#define PTE_PWT   0x008 // Write-Through
#define PTE_PCD   0x010 // Cache-Disable
#define PTE_A   0x020 // Accessed
#define PTE_D   0x040 // Dirty
#define PTE_PS    0x080 // Page Size
#define PTE_G   0x100 // Global
#define PTE_COW   0x800 // Avail for system programmer's use

/* other constants */
#define NUM_IDS 64
#define MagicNumber 1048577
#define MAX_CHILDREN 3

uint32_t read_ebp(void);
void lldt(uint16_t);
void cli(void);
void sti(void);
uint64_t rdmsr(uint32_t);
void wrmsr(uint32_t, uint64_t);
void halt(void);
uint64_t rdtsc(void);
void enable_sse(void);
void cpuid(uint32_t, uint32_t*, uint32_t*, uint32_t*, uint32_t*);
cpu_vendor vender(void);
uint32_t rcr3(void);
void outl(int, uint32_t);
uint32_t inl(int);
void smp_wmb(void);
void ltr(uint16_t sel);
void lcr0(uint32_t val);
uint32_t rcr0(void);
uint32_t rcr2(void);
void lcr3(uint32_t val);
void lcr4(uint32_t val);
uint32_t rcr4(void);
uint8_t inb(int port);
void insl(int port, void *addr, int cnt);
void outb(int port, uint8_t data);
void outsw(int port, const void *addr, int cnt);

#define FENCE() asm volatile ("mfence" ::: "memory")

#define LOW8(x)  ((x) & 0xffu)
#define HIGH8(x)  (((x) >> 8) & 0xffu)

#endif /* _KERN_ */

#endif /* !_KERN_X86_H_ */
