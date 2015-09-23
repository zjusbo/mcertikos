#include <lib/string.h>
#include <lib/trap.h>
#include <lib/debug.h>
#include <lib/x86.h>
#include <dev/intr.h>

extern unsigned int CID;

static void
trap_dump (tf_t *tf)
{
	if (tf == NULL)
		return;

	uintptr_t base = (uintptr_t) tf;

	KERN_DEBUG("trapframe at %x\n", base);
	KERN_INFO("\t%08x:\tedi:   \t\t%08x\n", &tf->regs.edi, tf->regs.edi);
	KERN_INFO("\t%08x:\tesi:   \t\t%08x\n", &tf->regs.esi, tf->regs.esi);
	KERN_INFO("\t%08x:\tebp:   \t\t%08x\n", &tf->regs.ebp, tf->regs.ebp);
	KERN_INFO("\t%08x:\tesp:   \t\t%08x\n", &tf->regs.oesp, tf->regs.oesp);
	KERN_INFO("\t%08x:\tebx:   \t\t%08x\n", &tf->regs.ebx, tf->regs.ebx);
	KERN_INFO("\t%08x:\tedx:   \t\t%08x\n", &tf->regs.edx, tf->regs.edx);
	KERN_INFO("\t%08x:\tecx:   \t\t%08x\n", &tf->regs.ecx, tf->regs.ecx);
	KERN_INFO("\t%08x:\teax:   \t\t%08x\n", &tf->regs.eax, tf->regs.eax);
	KERN_INFO("\t%08x:\tes:    \t\t%08x\n", &tf->es, tf->es);
	KERN_INFO("\t%08x:\tds:    \t\t%08x\n", &tf->ds, tf->ds);
	KERN_INFO("\t%08x:\ttrapno:\t\t%08x\n", &tf->trapno, tf->trapno);
	KERN_INFO("\t%08x:\terr:   \t\t%08x\n", &tf->err, tf->err);
	KERN_INFO("\t%08x:\teip:   \t\t%08x\n", &tf->eip, tf->eip);
	KERN_INFO("\t%08x:\tcs:    \t\t%08x\n", &tf->cs, tf->cs);
	KERN_INFO("\t%08x:\teflags:\t\t%08x\n", &tf->eflags, tf->eflags);
	KERN_INFO("\t%08x:\tesp:   \t\t%08x\n", &tf->esp, tf->esp);
	KERN_INFO("\t%08x:\tss:    \t\t%08x\n", &tf->ss, tf->ss);
}

void
pgflt_handler (tf_t *tf)
{
	unsigned int errno;
	unsigned int fault_va;


	errno = tf->err;
	fault_va = rcr2 ();

	dprintf("Page fault: VA 0x%08x, errno 0x%08x, page table # %d, EIP 0x%08x.\n",
		fault_va, errno, CID, tf->eip);

	if (tf->err & PFE_PR)
	{
		KERN_PANIC("Permission denied: va = 0x%08x, errno = 0x%08x.\n",
			fault_va, errno);
		return;
	}

	alloc_page (CID, rounddown (fault_va, PAGESIZE), PTE_W | PTE_U | PTE_P);
}

void
checkpoint ()
{
	KERN_INFO("check point\n");
}

void
trap (tf_t *tf)
{
	unsigned int cur_pid;

	if (tf->trapno == T_PGFLT)
	{
		set_pdir_base (0);
		pgflt_handler (tf);
	}
	else
	{
		KERN_DEBUG("unhandled trap: %d\n", tf->trapno);
		trap_dump (tf);
		KERN_PANIC("stop!\n");
	}

	set_pdir_base (CID);
	trap_return (tf);
}
