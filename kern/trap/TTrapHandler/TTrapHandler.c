#include <lib/string.h>
#include <lib/trap.h>
#include <lib/syscall.h>
#include <lib/debug.h>
#include <lib/x86.h>
#include <dev/intr.h>
#include <pcpu/PCPUIntro/export.h>

#include <vmm/MPTOp/export.h>
#include <thread/PThread/export.h>

#include "import.h"

void ide_intr(void);

static void trap_dump(tf_t *tf)
{
	if (tf == NULL)
		return;

	uintptr_t base = (uintptr_t) tf;

	KERN_DEBUG("trapframe at %x\n", base);
	KERN_DEBUG("\t%08x:\tedi:   \t\t%08x\n", &tf->regs.edi, tf->regs.edi);
	KERN_DEBUG("\t%08x:\tesi:   \t\t%08x\n", &tf->regs.esi, tf->regs.esi);
	KERN_DEBUG("\t%08x:\tebp:   \t\t%08x\n", &tf->regs.ebp, tf->regs.ebp);
	KERN_DEBUG("\t%08x:\tesp:   \t\t%08x\n", &tf->regs.oesp, tf->regs.oesp);
	KERN_DEBUG("\t%08x:\tebx:   \t\t%08x\n", &tf->regs.ebx, tf->regs.ebx);
	KERN_DEBUG("\t%08x:\tedx:   \t\t%08x\n", &tf->regs.edx, tf->regs.edx);
	KERN_DEBUG("\t%08x:\tecx:   \t\t%08x\n", &tf->regs.ecx, tf->regs.ecx);
	KERN_DEBUG("\t%08x:\teax:   \t\t%08x\n", &tf->regs.eax, tf->regs.eax);
	KERN_DEBUG("\t%08x:\tes:    \t\t%08x\n", &tf->es, tf->es);
	KERN_DEBUG("\t%08x:\tds:    \t\t%08x\n", &tf->ds, tf->ds);
	KERN_DEBUG("\t%08x:\ttrapno:\t\t%08x\n", &tf->trapno, tf->trapno);
	KERN_DEBUG("\t%08x:\terr:   \t\t%08x\n", &tf->err, tf->err);
	KERN_DEBUG("\t%08x:\teip:   \t\t%08x\n", &tf->eip, tf->eip);
	KERN_DEBUG("\t%08x:\tcs:    \t\t%08x\n", &tf->cs, tf->cs);
	KERN_DEBUG("\t%08x:\teflags:\t\t%08x\n", &tf->eflags, tf->eflags);
	KERN_DEBUG("\t%08x:\tesp:   \t\t%08x\n", &tf->esp, tf->esp);
	KERN_DEBUG("\t%08x:\tss:    \t\t%08x\n", &tf->ss, tf->ss);
}

void default_exception_handler(tf_t *tf)
{
	unsigned int cur_pid;

	cur_pid = get_curid();
	trap_dump(tf);

	KERN_PANIC("Trap %d @ 0x%08x.\n", tf -> trapno, tf -> eip);
}

void pgflt_handler(tf_t *tf)
{
	unsigned int cur_pid;
	unsigned int errno;
	unsigned int fault_va;

	cur_pid = get_curid();
	errno = tf -> err;
	fault_va = rcr2();

  //Uncomment this line if you need to see the information of the sequence of page faults occured.
	//KERN_DEBUG("Page fault: VA 0x%08x, errno 0x%08x, process %d, EIP 0x%08x.\n", fault_va, errno, cur_pid, tf -> eip);

	if (errno & PFE_PR) {
		trap_dump(tf);
		KERN_PANIC("Permission denied: va = 0x%08x, errno = 0x%08x.\n", fault_va, errno);
		return;
	}

	if (alloc_page(cur_pid, fault_va, PTE_W | PTE_U | PTE_P) == MagicNumber)
    KERN_PANIC("Page allocation failed: va = 0x%08x, errno = 0x%08x.\n", fault_va, errno);

}

/**
 * We currently only handle the page fault exception.
 * All other exceptions should be routed to the default exception handler.
 */
void exception_handler(tf_t *tf)
{
	unsigned int cur_pid;
	unsigned int trapno;

	cur_pid = get_curid();
	trapno = tf -> trapno;

	if (trapno == T_PGFLT)
		pgflt_handler(tf);
	else
		default_exception_handler(tf);
}



static int spurious_intr_handler (void)
{
    return 0;
}

static int timer_intr_handler (void)
{
    intr_eoi ();
    sched_update();
    return 0;
}

static int default_intr_handler (void)
{
    intr_eoi ();
    return 0;
}

/**
 * Any interrupt request except the spurious or timer should be
 * routed to the default interrupt handler.
 */
void interrupt_handler (tf_t *tf)
{
    unsigned int cur_pid;
    unsigned int trapno;

    cur_pid = get_curid ();

    trapno = tf -> trapno;

    switch (trapno)
    {
      case T_IRQ0 + IRQ_SPURIOUS:
          spurious_intr_handler ();
          break;
      case T_IRQ0 + IRQ_TIMER:
          timer_intr_handler ();
          break;
      // TODO: handle the disk interrupts here
      case T_IRQ0 + IRQ_IDE1:
          ide_intr();
          intr_eoi();
          break;
      case T_IRQ0 + IRQ_IDE2:
          break;
      default:
          default_intr_handler ();
    }
}

void trap (tf_t *tf)
{
    unsigned int cur_pid;
    unsigned int in_kernel;

    cur_pid = get_curid ();
    set_pdir_base (0); //switch to the kernel's page table.

    trap_cb_t f;

    f = TRAP_HANDLER[get_pcpu_idx()][tf->trapno];

    if (f){
            f(tf);
    } else {
            KERN_WARN("No handler for user trap 0x%x, process %d, eip 0x%08x. \n",
                            tf->trapno, cur_pid, tf->eip);
    }

    kstack_switch(cur_pid);
    set_pdir_base(cur_pid);
	  trap_return((void *) tf);
}
