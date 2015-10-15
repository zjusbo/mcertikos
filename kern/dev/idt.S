/* Trap handler entry and exit code. */
/* See COPYRIGHT for copyright information. */

#include <lib/seg.h>
#include <lib/trap.h>
#include <dev/intr.h>


/* The TRAPHANDLER macro defines a globally-visible function for handling
 * a trap.  It pushes a trap number onto the stack, then jumps to _alltraps.
 * Use TRAPHANDLER for traps where the CPU automatically pushes an error code.
 */
#define TRAPHANDLER(name, num)						\
	.globl name;		/* define global symbol for 'name' */	\
	.type name, @function;	/* symbol type is function */		\
	.align 2;		/* align function definition */		\
	name:			/* function starts here */		\
	pushl $(num);  /* insert the number of the interrupt */ \
	jmp _alltraps

/* Use TRAPHANDLER_NOEC for traps where the CPU doesn't push an error code.
 * It pushes a 0 in place of the error code, so the trap frame has the same
 * format in either case.
 */
#define TRAPHANDLER_NOEC(name, num)					\
	.globl name;							\
	.type name, @function;						\
	.align 2;							\
	name:								\
	pushl $0;/* error code for interrupts that generate no error code */ \
	pushl $(num); /* interrupt number */ \
	jmp _alltraps

.text

/* exceptions  */
TRAPHANDLER_NOEC(Xdivide,	T_DIVIDE)
TRAPHANDLER_NOEC(Xdebug,	T_DEBUG)
TRAPHANDLER_NOEC(Xnmi,		T_NMI)
TRAPHANDLER_NOEC(Xbrkpt,	T_BRKPT)
TRAPHANDLER_NOEC(Xoflow,	T_OFLOW)
TRAPHANDLER_NOEC(Xbound,	T_BOUND)
TRAPHANDLER_NOEC(Xillop,	T_ILLOP)
TRAPHANDLER_NOEC(Xdevice,	T_DEVICE)
TRAPHANDLER     (Xdblflt,	T_DBLFLT)
TRAPHANDLER_NOEC(Xcoproc,	T_COPROC)
TRAPHANDLER     (Xtss,		T_TSS)
TRAPHANDLER     (Xsegnp,	T_SEGNP)
TRAPHANDLER     (Xstack,	T_STACK)
TRAPHANDLER     (Xgpflt,	T_GPFLT)
TRAPHANDLER     (Xpgflt,	T_PGFLT)
TRAPHANDLER_NOEC(Xres,		T_RES)
TRAPHANDLER_NOEC(Xfperr,	T_FPERR)
TRAPHANDLER     (Xalign,	T_ALIGN)
TRAPHANDLER_NOEC(Xmchk,		T_MCHK)

/* ISA interrupts  */
TRAPHANDLER_NOEC(Xirq_timer,	T_IRQ0 + IRQ_TIMER)
TRAPHANDLER_NOEC(Xirq_kbd,	T_IRQ0 + IRQ_KBD)
TRAPHANDLER_NOEC(Xirq_slave,	T_IRQ0 + IRQ_SLAVE)
TRAPHANDLER_NOEC(Xirq_serial2,	T_IRQ0 + IRQ_SERIAL24)
TRAPHANDLER_NOEC(Xirq_serial1,	T_IRQ0 + IRQ_SERIAL13)
TRAPHANDLER_NOEC(Xirq_lpt,	T_IRQ0 + IRQ_LPT2)
TRAPHANDLER_NOEC(Xirq_floppy,	T_IRQ0 + IRQ_FLOPPY)
TRAPHANDLER_NOEC(Xirq_spurious,	T_IRQ0 + IRQ_SPURIOUS)
TRAPHANDLER_NOEC(Xirq_rtc,	T_IRQ0 + IRQ_RTC)
TRAPHANDLER_NOEC(Xirq9,		T_IRQ0 + 9)
TRAPHANDLER_NOEC(Xirq10,	T_IRQ0 + 10)
TRAPHANDLER_NOEC(Xirq11,	T_IRQ0 + 11)
TRAPHANDLER_NOEC(Xirq_mouse,	T_IRQ0 + IRQ_MOUSE)
TRAPHANDLER_NOEC(Xirq_coproc,	T_IRQ0 + IRQ_COPROCESSOR)
TRAPHANDLER_NOEC(Xirq_ide1,	T_IRQ0 + IRQ_IDE1)
TRAPHANDLER_NOEC(Xirq_ide2,	T_IRQ0 + IRQ_IDE2)

/* syscall */
TRAPHANDLER_NOEC(Xsyscall,	T_SYSCALL)

/* default ? */
TRAPHANDLER     (Xdefault,	T_DEFAULT)

.globl	_alltraps
.type	_alltraps,@function
.p2align 4, 0x90		/* 16-byte alignment, nop filled */
_alltraps:
	cli			# make sure there is no nested trap

	cld

	pushl %ds		# build context
	pushl %es
	pushal

	movl $CPU_GDT_KDATA,%eax # load kernel's data segment
	movw %ax,%ds
	movw %ax,%es

	pushl %esp		# pass pointer to this trapframe
	call trap		# and call trap (does not return)

1:	hlt			# should never get here; just spin...

//
// Trap return code.
// C code in the kernel will call this function to return from a trap,
// providing the
// Restore the CPU state from a given trapframe struct
// and return from the trap using the processor's 'iret' instruction.
// This function does not return to the caller,
// since the new CPU state this function loads
// replaces the caller's stack pointer and other registers.
//
.globl	trap_return
.type	trap_return,@function
.p2align 4, 0x90		/* 16-byte alignment, nop filled */
trap_return:
	movl	4(%esp),%esp	// reset stack pointer to point to trap frame
	popal			// restore general-purpose registers except esp
	popl	%es		// restore data segment registers
	popl	%ds
	addl	$8,%esp		// skip tf_trapno and tf_errcode
	iret			// return from trap handler
