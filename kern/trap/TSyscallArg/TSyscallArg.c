#include <lib/trap.h>
#include <lib/x86.h>

#include "import.h"

unsigned int syscall_get_arg1(tf_t *tf)
{
	unsigned int cur_pid;
	unsigned int arg1;

	cur_pid = get_curid();
	arg1 = tf -> regs.eax;

	return arg1;
}

unsigned int syscall_get_arg2(tf_t *tf)
{
	unsigned int cur_pid;
	unsigned int arg2;

	cur_pid = get_curid();
	arg2 = tf -> regs.ebx;

	return arg2;
}

unsigned int syscall_get_arg3(tf_t *tf)
{
	unsigned int cur_pid;
	unsigned int arg3;

	cur_pid = get_curid();
	arg3 = tf -> regs.ecx;

	return arg3;
}

unsigned int syscall_get_arg4(tf_t *tf)
{
	unsigned int cur_pid;
	unsigned int arg4;

	cur_pid = get_curid();
	arg4 = tf -> regs.edx;

	return arg4;
}

unsigned int syscall_get_arg5(tf_t *tf)
{
	unsigned int cur_pid;
	unsigned int arg5;

	cur_pid = get_curid();
	arg5 = tf -> regs.esi;

	return arg5;
}

unsigned int syscall_get_arg6(tf_t *tf)
{
	unsigned int cur_pid;
	unsigned int arg6;

	cur_pid = get_curid();
	arg6 = tf -> regs.edi;

	return arg6;
}

void syscall_set_errno(tf_t *tf, unsigned int errno)
{
	unsigned int cur_pid;

	cur_pid = get_curid();
  tf -> regs.eax = errno;
}

void syscall_set_retval1(tf_t *tf, unsigned int retval)
{
	unsigned int cur_pid;

	cur_pid = get_curid();
  tf -> regs.ebx = retval;
}

void syscall_set_retval2(tf_t *tf, unsigned int retval)
{
	unsigned int cur_pid;

	cur_pid = get_curid();
  tf -> regs.ecx = retval;
}

void syscall_set_retval3(tf_t *tf, unsigned int retval)
{
	unsigned int cur_pid;

	cur_pid = get_curid();
  tf -> regs.edx = retval;
}

void syscall_set_retval4(tf_t *tf, unsigned int retval)
{
	unsigned int cur_pid;

	cur_pid = get_curid();
  tf -> regs.esi = retval;
}

void syscall_set_retval5(tf_t *tf, unsigned int retval)
{
	unsigned int cur_pid;

	cur_pid = get_curid();
  tf -> regs.edi = retval;
}
