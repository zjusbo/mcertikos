#ifndef _KERN_X86_H_
#define _KERN_X86_H_

#ifdef _KERN_

#include <lib/types.h>


/* CPUID */
/* 0x0000_0001 ECX */
#define CPUID_FEATURE_RDRAND            (1<<21)
#define CPUID_FEATURE_F16C              (1<<29)
#define CPUID_FEATURE_AVX               (1<<28)
#define CPUID_FEATURE_OSXSAVE           (1<<27)
#define CPUID_FEATURE_XSAVE             (1<<26)
#define CPUID_FEATURE_AES               (1<<25)
#define CPUID_FEATURE_TSC_DEADLINE      (1<<24)
#define CPUID_FEATURE_POPCNT            (1<<23)
#define CPUID_FEATURE_MOVBE             (1<<22)
#define CPUID_FEATURE_X2APIC            (1<<21)
#define CPUID_FEATURE_SSE42             (1<<20)
#define CPUID_FEATURE_SSE41             (1<<19)
#define CPUID_FEATURE_DCA               (1<<18)
#define CPUID_FEATURE_PCID              (1<<17)
#define CPUID_FEATURE_PDCM              (1<<15)
#define CPUID_FEATURE_XTPR              (1<<14)
#define CPUID_FEATURE_CMPXCHG16B        (1<<13)
#define CPUID_FEATURE_FMA               (1<<12)
#define CPUID_FEATURE_CNXT_ID           (1<<10)
#define CPUID_FEATURE_SSSE3             (1<<9)
#define CPUID_FEATURE_TM2               (1<<8)
#define CPUID_FEATURE_EIST              (1<<7)
#define CPUID_FEATURE_SMX               (1<<6)
#define CPUID_FEATURE_VMX               (1<<5)
#define CPUID_FEATURE_DS_CPL            (1<<4)
#define CPUID_FEATURE_MONITOR           (1<<3)
#define CPUID_FEATURE_DTES64            (1<<2)
#define CPUID_FEATURE_PCLMULQDQ         (1<<1)
#define CPUID_FEATURE_SSE3              (1<<0)
/* 0x0000_0001 EDX */
#define CPUID_FEATURE_PBE               (1<<31)
#define CPUID_FEATURE_TM                (1<<29)
#define CPUID_FEATURE_HTT               (1<<28)
#define CPUID_FEATURE_SS                (1<<27)
#define CPUID_FEATURE_SSE2              (1<<26)
#define CPUID_FEATURE_SSE               (1<<25)
#define CPUID_FEATURE_FXSR              (1<<24)
#define CPUID_FEATURE_MMX               (1<<23)
#define CPUID_FEATURE_ACPI              (1<<22)
#define CPUID_FEATURE_DS                (1<<21)
#define CPUID_FEATURE_CLFUSH            (1<<19)
#define CPUID_FEATURE_PSE36             (1<<17)
#define CPUID_FEATURE_PAT               (1<<16)
#define CPUID_FEATURE_CMOV              (1<<15)
#define CPUID_FEATURE_MCA               (1<<14)
#define CPUID_FEATURE_PGE               (1<<13)
#define CPUID_FEATURE_MTRR              (1<<12)
#define CPUID_FEATURE_SYSENTREXIT       (1<<11)
#define CPUID_FEATURE_APIC              (1<<9)
#define CPUID_FEATURE_CMPXCHG8B         (1<<8)
#define CPUID_FEATURE_MCE               (1<<7)
#define CPUID_FEATURE_PAE               (1<<6)
#define CPUID_FEATURE_MSR               (1<<5)
#define CPUID_FEATURE_TSC               (1<<4)
#define CPUID_FEATURE_PSE               (1<<3)
#define CPUID_FEATURE_DE                (1<<2)
#define CPUID_FEATURE_VME               (1<<1)
#define CPUID_FEATURE_FPU               (1<<0)



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
#define NUM_CPUS 8
#define NUM_IDS 64
#define MagicNumber 1048577
#define MAX_CHILDREN 3

uintptr_t get_stack_base(void);
uintptr_t get_stack_pointer(void);
uint32_t read_ebp(void);
void lldt(uint16_t);
void cli(void);
void sti(void);
uint64_t rdmsr(uint32_t);
void wrmsr(uint32_t, uint64_t);
void pause(void);
void halt(void);
uint32_t xchg(volatile uint32_t *addr, uint32_t newval);
uint64_t rdtsc(void);
void enable_sse(void);
void cpuid(uint32_t, uint32_t*, uint32_t*, uint32_t*, uint32_t*);
void cpuid_subleaf(uint32_t leaf, uint32_t subleaf, uint32_t *eaxp, uint32_t *ebxp, uint32_t *ecxp, uint32_t *edxp);
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
void outsl(int port, const void *addr, int cnt);

#define FENCE() asm volatile ("mfence" ::: "memory")

#define LOW8(x)  ((x) & 0xffu)
#define HIGH8(x)  (((x) >> 8) & 0xffu)

#endif /* _KERN_ */

#endif /* !_KERN_X86_H_ */
