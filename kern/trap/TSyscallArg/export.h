#ifndef _KERN_TRAP_TSYSCALLARG_H_
#define _KERN_TRAP_TSYSCALLARG_H_

unsigned int syscall_get_arg1(void);
unsigned int syscall_get_arg2(void);
unsigned int syscall_get_arg3(void);
unsigned int syscall_get_arg4(void);
unsigned int syscall_get_arg5(void);
unsigned int syscall_get_arg6(void);

void syscall_set_errno(unsigned int errno);
void syscall_set_retval1(unsigned int retval);
void syscall_set_retval2(unsigned int retval);
void syscall_set_retval3(unsigned int retval);
void syscall_set_retval4(unsigned int retval);
void syscall_set_retval5(unsigned int retval);

#endif

