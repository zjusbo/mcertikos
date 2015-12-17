#ifndef _KERN_TRAP_TTRAPHANDLER_H_
#define _KERN_TRAP_TTRAPHANDLER_H_

#ifdef _KERN_

unsigned int syscall_get_arg1(void);
void set_pdir_base(unsigned int);
void proc_start_user(void);

#endif /* _KERN_ */

#endif /* !_KERN_TRAP_TTRAPHANDLER_H_ */
