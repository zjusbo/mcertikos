#include <lib/syscall.h>
#include <lib/x86.h>
#include <lib/trap.h>
#include <lib/debug.h>
#include <dev/intr.h>
#include <pcpu/PCPUIntro/export.h>

#include "import.h"

void syscall_dispatch(tf_t *tf)
{
	unsigned int nr;

	nr = syscall_get_arg1(tf);

	switch (nr) {
	case SYS_puts:
		/*
		 * Output a string to the screen.
		 *
		 * Parameters:
		 *   a[0]: the linear address where the string is
		 *   a[1]: the length of the string
		 *
		 * Return:
		 *   None.
		 *
		 * Error:
		 *   E_MEM
		 */
		sys_puts(tf);
		break;
	case SYS_spawn:
		/*
		 * Create a new process.
		 *
		 * Parameters:
		 *   a[0]: the identifier of the ELF image
		 *
		 * Return:
		 *   the process ID of the process
		 *
		 * Error:
		 *   E_INVAL_ADDR, E_INVAL_PID
		 */
		sys_spawn(tf);
		break;
	case SYS_yield:
		/*
		 * Called by a process to abandon its CPU slice.
		 *
		 * Parameters:
		 *   None.
		 *
		 * Return:
		 *   None.
		 *
		 * Error:
		 *   None.
		 */
		sys_yield(tf);
		break;
  case SYS_produce:
    intr_local_enable();
    sys_produce(tf);
    intr_local_disable();
    break;
  case SYS_consume:
    intr_local_enable();
    sys_consume(tf);
    intr_local_disable();
    break;
	default:
		syscall_set_errno(E_INVAL_CALLNR);
	}
}
