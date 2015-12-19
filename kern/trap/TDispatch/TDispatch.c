#include <lib/syscall.h>
#include <lib/x86.h>
#include <lib/trap.h>
#include <lib/debug.h>
#include <dev/intr.h>
#include <pcpu/PCPUIntro/export.h>

#include "import.h"
#include <kern/fs/sysfile.h>

void syscall_dispatch(tf_t *tf)
{
  unsigned int nr;

  nr = syscall_get_arg1(tf);
  switch (nr) {
  case SYS_sync_send:
    sys_sync_send(tf);
    break;
  case SYS_sync_recv:
    sys_sync_recv(tf);
    break;
   //shell related commands
  case SYS_is_dir:
    sys_is_dir(tf);
    break;
  case SYS_ls:
    sys_ls(tf);
    break;
  case SYS_pwd:
    sys_pwd(tf);
    break;
  case SYS_cp:
    sys_cp(tf);
    break;
  case SYS_mv:
    sys_mv(tf);
    break;
  case SYS_rm:
    sys_rm(tf);
    break;
  case SYS_cat:
    sys_cat(tf);
    break;
  case SYS_touch:
    sys_touch(tf);
  case SYS_readline:
    sys_readline(tf);
    break;
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

  /** Filesystem calls **/
  case SYS_open:
    sys_open(tf);
    break;
  case SYS_close:
    sys_close(tf);
    break;
  case SYS_read:
    sys_read(tf);
    break;
  case SYS_write:
    sys_write(tf);
    break;
  case SYS_mkdir:
    sys_mkdir(tf);
    break;
  case SYS_chdir:
    sys_chdir(tf);
    break;
  case SYS_link:
    sys_link(tf);
    break;
  case SYS_unlink:
    sys_unlink(tf);
    break;
  case SYS_stat:
    sys_fstat(tf);
    break;
  default:
    syscall_set_errno(E_INVAL_CALLNR);
  }
}
