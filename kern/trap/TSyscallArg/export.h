#ifndef _KERN_TRAP_TSYSCALLARG_H_
#define _KERN_TRAP_TSYSCALLARG_H_

#ifdef _KERN_

unsigned int syscall_get_arg1(tf_t *tf);
unsigned int syscall_get_arg2(tf_t *tf);
unsigned int syscall_get_arg3(tf_t *tf);
unsigned int syscall_get_arg4(tf_t *tf);
unsigned int syscall_get_arg5(tf_t *tf);
unsigned int syscall_get_arg6(tf_t *tf);

void syscall_set_errno(tf_t *tf, unsigned int errno);
void syscall_set_retval1(tf_t *tf, unsigned int retval);
void syscall_set_retval2(tf_t *tf, unsigned int retval);
void syscall_set_retval3(tf_t *tf, unsigned int retval);
void syscall_set_retval4(tf_t *tf, unsigned int retval);
void syscall_set_retval5(tf_t *tf, unsigned int retval);

#endif /* _KERN_ */

#endif /* !_KERN_TRAP_TSYSCALLARG_H_ */

