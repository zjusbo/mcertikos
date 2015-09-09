#ifndef _SYS_PREINIT_DEV_CONSOLE_H_
#define _SYS_PREINIT_DEV_CONSOLE_H_

#ifdef _KERN_

#define CONSOLE_BUFFER_SIZE	512

void cons_init(void);
void cons_enable_kbd(void);
void cons_putc(char);
void cons_intr(int (*proc)(void));

#endif /* _KERN_ */

#endif /* !_SYS_PREINIT_DEV_CONSOLE_H_ */
