#ifndef _KERN_TRAP_TDISPATCH_H_
#define _KERN_TRAP_TDISPATCH_H_

#ifdef _KERN_

unsigned int syscall_get_arg1(void);
void sys_puts(void);
void sys_spawn(void);
void sys_yield(void);

#endif /* _KERN_ */

#endif /* !_KERN_TRAP_TDISPATCH_H_ */
