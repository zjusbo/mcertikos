#include <lib/types.h>

#include <lib/debug.h>
#include <lib/seg.h>
#include <lib/x86.h>

#include <dev/pic.h>
#include <dev/intr.h>

volatile static bool intr_inited = FALSE;

/* Entries of interrupt handlers, defined in trapasm.S by TRAPHANDLE* */
extern char Xdivide, Xdebug, Xnmi, Xbrkpt, Xoflow, Xbound, Xillop, Xdevice,
		Xdblflt, Xtss, Xsegnp, Xstack, Xgpflt, Xpgflt, Xfperr, Xalign, Xmchk;
extern char Xirq_timer, Xirq_kbd, Xirq_slave, Xirq_serial2, Xirq_serial1,
		Xirq_lpt, Xirq_floppy, Xirq_spurious, Xirq_rtc, Xirq9, Xirq10, Xirq11,
		Xirq_mouse, Xirq_coproc, Xirq_ide1, Xirq_ide2;
extern char Xsyscall;
extern char Xdefault;

/* Interrupt Descriptors Table */
gatedesc_t idt[256];
pseudodesc_t idt_pd =
	{ .pd_lim = sizeof(idt) - 1, .pd_base = (uint32_t) idt };

static void
intr_init_idt(void)
{
	int i;

	/* check that T_IRQ0 is a multiple of 8 */
	KERN_ASSERT((T_IRQ0 & 7) == 0);

	/* install a default handler */
	for (i = 0; i < sizeof(idt)/sizeof(idt[0]); i++)
		SETGATE(idt[i], 0, CPU_GDT_KCODE, &Xdefault, 0);

	SETGATE(idt[T_DIVIDE],            0, CPU_GDT_KCODE, &Xdivide,       0);
	SETGATE(idt[T_DEBUG],             0, CPU_GDT_KCODE, &Xdebug,        0);
	SETGATE(idt[T_NMI],               0, CPU_GDT_KCODE, &Xnmi,          0);
	SETGATE(idt[T_BRKPT],             0, CPU_GDT_KCODE, &Xbrkpt,        3);
	SETGATE(idt[T_OFLOW],             0, CPU_GDT_KCODE, &Xoflow,        3);
	SETGATE(idt[T_BOUND],             0, CPU_GDT_KCODE, &Xbound,        0);
	SETGATE(idt[T_ILLOP],             0, CPU_GDT_KCODE, &Xillop,        0);
	SETGATE(idt[T_DEVICE],            0, CPU_GDT_KCODE, &Xdevice,       0);
	SETGATE(idt[T_DBLFLT],            0, CPU_GDT_KCODE, &Xdblflt,       0);
	SETGATE(idt[T_TSS],               0, CPU_GDT_KCODE, &Xtss,          0);
	SETGATE(idt[T_SEGNP],             0, CPU_GDT_KCODE, &Xsegnp,        0);
	SETGATE(idt[T_STACK],             0, CPU_GDT_KCODE, &Xstack,        0);
	SETGATE(idt[T_GPFLT],             0, CPU_GDT_KCODE, &Xgpflt,        0);
	SETGATE(idt[T_PGFLT],             0, CPU_GDT_KCODE, &Xpgflt,        0);
	SETGATE(idt[T_FPERR],             0, CPU_GDT_KCODE, &Xfperr,        0);
	SETGATE(idt[T_ALIGN],             0, CPU_GDT_KCODE, &Xalign,        0);
	SETGATE(idt[T_MCHK],              0, CPU_GDT_KCODE, &Xmchk,         0);

	SETGATE(idt[T_IRQ0+IRQ_TIMER],    0, CPU_GDT_KCODE, &Xirq_timer,    0);
	SETGATE(idt[T_IRQ0+IRQ_KBD],      0, CPU_GDT_KCODE, &Xirq_kbd,      0);
	SETGATE(idt[T_IRQ0+IRQ_SLAVE],    0, CPU_GDT_KCODE, &Xirq_slave,    0);
	SETGATE(idt[T_IRQ0+IRQ_SERIAL24], 0, CPU_GDT_KCODE, &Xirq_serial2,  0);
	SETGATE(idt[T_IRQ0+IRQ_SERIAL13], 0, CPU_GDT_KCODE, &Xirq_serial1,  0);
	SETGATE(idt[T_IRQ0+IRQ_LPT2],     0, CPU_GDT_KCODE, &Xirq_lpt,      0);
	SETGATE(idt[T_IRQ0+IRQ_FLOPPY],   0, CPU_GDT_KCODE, &Xirq_floppy,   0);
	SETGATE(idt[T_IRQ0+IRQ_SPURIOUS], 0, CPU_GDT_KCODE, &Xirq_spurious, 0);
	SETGATE(idt[T_IRQ0+IRQ_RTC],      0, CPU_GDT_KCODE, &Xirq_rtc,      0);
	SETGATE(idt[T_IRQ0+9],            0, CPU_GDT_KCODE, &Xirq9,         0);
	SETGATE(idt[T_IRQ0+10],           0, CPU_GDT_KCODE, &Xirq10,        0);
	SETGATE(idt[T_IRQ0+11],           0, CPU_GDT_KCODE, &Xirq11,        0);
	SETGATE(idt[T_IRQ0+IRQ_MOUSE],    0, CPU_GDT_KCODE, &Xirq_mouse,    0);
	SETGATE(idt[T_IRQ0+IRQ_COPROCESSOR], 0, CPU_GDT_KCODE, &Xirq_coproc, 0);
	SETGATE(idt[T_IRQ0+IRQ_IDE1],     0, CPU_GDT_KCODE, &Xirq_ide1,     0);
	SETGATE(idt[T_IRQ0+IRQ_IDE2],     0, CPU_GDT_KCODE, &Xirq_ide2,     0);

	// Use DPL=3 here because system calls are explicitly invoked
	// by the user process (with "int $T_SYSCALL").
	SETGATE(idt[T_SYSCALL],           0, CPU_GDT_KCODE, &Xsyscall,      3);

	/* default */
	SETGATE(idt[T_DEFAULT],           0, CPU_GDT_KCODE, &Xdefault,      0);

	asm volatile("lidt %0" : : "m" (idt_pd));
}

void
intr_init(void)
{
	if (intr_inited == TRUE)
		return;

  pic_init();
	intr_init_idt();
	intr_inited = TRUE;
}

void
intr_enable(uint8_t irq)
{
	if (irq >= 16)
		return;
	pic_enable(irq);
}

void
intr_eoi(void)
{
	pic_eoi();
}

void
intr_local_enable(void)
{
	sti();
}

void
intr_local_disable(void)
{
	cli();
}
