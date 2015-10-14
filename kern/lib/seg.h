#ifndef _SYS_PREINIT_LIB_SEG_H_
#define _SYS_PREINIT_LIB_SEG_H_

#ifdef _KERN_

#ifndef __ASSEMBLER__

#include <lib/gcc.h>
#include <lib/types.h>

/* Segment Descriptors */
typedef
struct segdesc {
	unsigned sd_lim_15_0 : 16;  /* Low bits of segment limit */
	unsigned sd_base_15_0 : 16; /* Low bits of segment base address */
	unsigned sd_base_23_16 : 8; /* Middle bits of segment base address */
	unsigned sd_type : 4;       /* Segment type (see STS_ constants) */
	unsigned sd_s : 1;          /* 0 = system, 1 = application */
	unsigned sd_dpl : 2;        /* Descriptor Privilege Level */
	unsigned sd_p : 1;          /* Present */
	unsigned sd_lim_19_16 : 4;  /* High bits of segment limit */
	unsigned sd_avl : 1;        /* Unused (available for software use) */
	unsigned sd_rsv1 : 1;       /* Reserved */
	unsigned sd_db : 1;         /* 0 = 16-bit segment, 1 = 32-bit segment */
	unsigned sd_g : 1;          /* Granularity: limit scaled by 4K when set */
	unsigned sd_base_31_24 : 8; /* High bits of segment base address */
} segdesc_t;

/* Pseudo-descriptors for GDT, LDT and IDT */
typedef
struct pseudodesc {
	uint16_t  pd_lim;	/* limit */
	uint32_t  pd_base;	/* base */
} gcc_packed pseudodesc_t;

/* Null segment */
#define SEGDESC_NULL							\
	(struct segdesc){ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
/* Segment that is loadable but faults when used */
#define SEGDESC_FAULT							\
	(struct segdesc){ 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0 }
/* Normal segment */
#define SEGDESC32(type, base, lim, dpl) (struct segdesc)		\
{ ((lim) >> 12) & 0xffff, (base) & 0xffff, ((base) >> 16) & 0xff,	\
    type, 1, dpl, 1, (unsigned) (lim) >> 28, 0, 0, 1, 1,		\
    (unsigned) (base) >> 24 }
#define SEGDESC16(type, base, lim, dpl) (struct segdesc)		\
{ (lim) & 0xffff, (base) & 0xffff, ((base) >> 16) & 0xff,		\
    type, 1, dpl, 1, (unsigned) (lim) >> 16, 0, 0, 1, 0,		\
    (unsigned) (base) >> 24 }

#endif

/* Application segment type bits ('app' bit = 1) */
#define STA_X		0x8	    /* Executable segment */
#define STA_E		0x4	    /* Expand down (non-executable segments) */
#define STA_C		0x4	    /* Conforming code segment (executable only) */
#define STA_W		0x2	    /* Writeable (non-executable segments) */
#define STA_R		0x2	    /* Readable (executable segments) */
#define STA_A		0x1	    /* Accessed */

/* System segment type bits ('app' bit = 0) */
#define STS_T16A	0x1	    /* Available 16-bit TSS */
#define STS_LDT		0x2	    /* Local Descriptor Table */
#define STS_T16B	0x3	    /* Busy 16-bit TSS */
#define STS_CG16	0x4	    /* 16-bit Call Gate */
#define STS_TG		0x5	    /* Task Gate / Coum Transmitions */
#define STS_IG16	0x6	    /* 16-bit Interrupt Gate */
#define STS_TG16	0x7	    /* 16-bit Trap Gate */
#define STS_T32A	0x9	    /* Available 32-bit TSS */
#define STS_T32B	0xB	    /* Busy 32-bit TSS */
#define STS_CG32	0xC	    /* 32-bit Call Gate */
#define STS_IG32	0xE	    /* 32-bit Interrupt Gate */
#define STS_TG32	0xF	    /* 32-bit Trap Gate */

#define CPU_GDT_NULL	0x00	    /* null descriptor */
#define CPU_GDT_KCODE	0x08	    /* kernel text */
#define CPU_GDT_KDATA	0x10	    /* kernel data */
#define CPU_GDT_UCODE	0x18	    /* user text */
#define CPU_GDT_UDATA	0x20	    /* user data */
#define CPU_GDT_TSS	0x28	    /* task state segment */
#define CPU_GDT_NDESC	6	    /* number of GDT entries used */

#ifndef __ASSEMBLER__

#include <lib/types.h>

typedef
struct taskgate {
	uint32_t	ts_link;
	uint32_t	ts_esp0;
	uint16_t	ts_ss0;
	uint16_t	ts_padding1;
	uint32_t	ts_esp1;
	uint16_t	ts_ss1;
	uint16_t	ts_padding2;
	uint32_t	ts_esp2;
	uint16_t	ts_ss2;
	uint16_t	ts_padding3;
	uint32_t	ts_cr3;
	uint32_t	ts_eip;
	uint32_t	ts_eflags;
	uint32_t	ts_eax;
	uint32_t        ts_ecx;
	uint32_t	ts_edx;
	uint32_t	ts_ebx;
	uint32_t	ts_esp;
	uint32_t	ts_ebp;
	uint32_t	ts_esi;
	uint32_t	ts_edi;
	uint16_t	ts_es;
	uint16_t	ts_padding4;
	uint16_t	ts_cs;
	uint16_t	ts_padding5;
	uint16_t	ts_ss;
	uint16_t	ts_padding6;
	uint16_t	ts_ds;
	uint16_t	ts_padding7;
	uint16_t	ts_fs;
	uint16_t	ts_padding8;
	uint16_t	ts_gs;
	uint16_t	ts_padding9;
	uint16_t	ts_ldt;
	uint16_t	ts_padding10;
	uint16_t	ts_trap;
	uint16_t	ts_iomb;
	uint8_t		ts_iopm[129];
} tss_t;

typedef
struct gatedesc {
	unsigned gd_off_15_0 : 16;   /* low 16 bits of offset in segment */
	unsigned gd_ss : 16;         /* segment selector */
	unsigned gd_args : 5;        /* # args, 0 for interrupt/trap gates */
	unsigned gd_rsv1 : 3;        /* reserved(should be zero I guess) */
	unsigned gd_type : 4;        /* type(STS_{TG,IG32,TG32}) */
	unsigned gd_s : 1;           /* must be 0 (system) */
	unsigned gd_dpl : 2;         /* descriptor(meaning new) privilege level */
	unsigned gd_p : 1;           /* Present */
	unsigned gd_off_31_16 : 16;  /* high bits of offset in segment */
} gatedesc_t;

/*
 * Set up a normal interrupt/trap gate descriptor.
 * - istrap: 1 for a trap (= exception) gate, 0 for an interrupt gate.
 * - sel: Code segment selector for interrupt/trap handler
 * - off: Offset in code segment for interrupt/trap handler
 * - dpl: Descriptor Privilege Level -
 *	  the privilege level required for software to invoke
 *	  this interrupt/trap gate explicitly using an int instruction.
*/
#define SETGATE(gate, istrap, sel, off, dpl)			\
{								\
	(gate).gd_off_15_0 = (uint32_t) (off) & 0xffff;		\
	(gate).gd_ss = (sel);					\
	(gate).gd_args = 0;					\
	(gate).gd_rsv1 = 0;					\
	(gate).gd_type = (istrap) ? STS_TG32 : STS_IG32;	\
	(gate).gd_s = 0;					\
	(gate).gd_dpl = (dpl);					\
	(gate).gd_p = 1;					\
	(gate).gd_off_31_16 = (uint32_t) (off) >> 16;		\
}

/* Set up a call gate descriptor. */
#define SETCALLGATE(gate, ss, off, dpl)				\
{								\
	(gate).gd_off_15_0 = (uint32_t) (off) & 0xffff;		\
	(gate).gd_ss = (ss);					\
	(gate).gd_args = 0;					\
	(gate).gd_rsv1 = 0;					\
	(gate).gd_type = STS_CG32;				\
	(gate).gd_s = 0;					\
	(gate).gd_dpl = (dpl);					\
	(gate).gd_p = 1;					\
	(gate).gd_off_31_16 = (uint32_t) (off) >> 16;		\
}

void seg_init(void);
void tss_switch(uint32_t pid);

#endif /* !__ASSEMBLER__ */

#endif /* _KERN_ */

#endif /* !_SYS_PREINIT_LIB_SEG_H_ */
