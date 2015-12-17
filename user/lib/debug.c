#include <proc.h>
#include <stdarg.h>
#include <stdio.h>

void
debug(const char *file, int line, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	printf("[D] %s:%d: ", file, line);
	vcprintf(fmt, ap);
	va_end(ap);
}

void
warn(const char *file, int line, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	printf("[W] %s:%d: ", file, line);
	vcprintf(fmt, ap);
	va_end(ap);
}

void
panic(const char *file, int line, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	printf("[P] %s:%d: ", file, line);
	vcprintf(fmt, ap);
	va_end(ap);

	while (1)
		yield();
}
