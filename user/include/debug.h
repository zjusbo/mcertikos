#ifndef _USER_DEBUG_H_
#define _USER_DEBUG_H_

#include <stdarg.h>

#define DEBUG(...)	debug(__FILE__, __LINE__, __VA_ARGS__)
#define WARN(...)	warn(__FILE__, __LINE__, __VA_ARGS__)
#define PANIC(...)	panic(__FILE__, __LINE__, __VA_ARGS__)

#define ASSERT(x)							\
	do {								\
		if (!(x))						\
			PANIC("User assertion failed: %s\n", #x);	\
	} while (0)

void debug(const char *file, int line, const char *fmt, ...);
void warn(const char *file, int line, const char *fmt, ...);
void panic(const char *file, int line, const char *fmt, ...);

#endif /* !_USER_DEBUG_H_*/
