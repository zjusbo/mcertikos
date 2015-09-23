#ifndef _USER_SYSCALL_H_
#define _USER_SYSCALL_H_

void yield ();
int sys_getc (void);
void sys_puts (const char *s, unsigned int len);

#endif /* !_USER_SYSCALL_H_ */

