#include <lib/syscall.h>

#include "import.h"

void syscall_dispatch(void)
{
	unsigned int nr;

	nr = syscall_get_arg1();

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
		sys_puts();
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
		sys_spawn();
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
		sys_yield();
		break;
	default:
		syscall_set_errno(E_INVAL_CALLNR);
	}
}
