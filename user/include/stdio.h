#ifndef _USER_STDIO_H_
#define _USER_STDIO_H_

#include <stdarg.h>

#define MAX_BUF 4096

#define getc()		sys_getc()
#define puts(str, len)	sys_puts((str), (len))


/*
 * standard c formatted output
 * supports the following flags:
 * 	- . * l c s d u o p x %
 */
int printf(const char *fmt, ...);
int printf2(const char *fmt, ...);
int vcprintf(const char *fmt, va_list ap);

void printfmt(void (*f)(int, void *), void *buf, const char *fmt, ...);
void vprintfmt(void (*f)(int, void *), void *buf, const char *fmt, va_list ap);

/*
 * reads up a line of up to size - 1 chars from keyboard into buf
 * and null terminates it
 */
void gets(char * buf, int size);

#endif /* _USER_STDIO_H_ */
