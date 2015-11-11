#include <lib/debug.h>
#include <lib/gcc.h>
#include <lib/stdarg.h>
#include <lib/x86.h>

#include <lib/types.h>
#include <lib/spinlock.h>

static spinlock_t debug_lk;
static spinlock_t serial_lk;

void
debug_init(void)
{
  spinlock_init(&debug_lk);
	spinlock_init(&serial_lk);
}


void
debug_lock(void)
{
  spinlock_acquire(&debug_lk);
}

void
debug_unlock(void)
{
  spinlock_release(&debug_lk);
}

void
serial_lock(void)
{
  spinlock_acquire(&serial_lk);
}

void
serial_unlock(void)
{
  spinlock_release(&serial_lk);
}


void
debug_info(const char *fmt, ...)
{
#ifdef DEBUG_MSG
  debug_lock();

	va_list ap;
	va_start(ap, fmt);
	vdprintf(fmt, ap);
	va_end(ap);

  debug_unlock();
#endif
}

#ifdef DEBUG_MSG

void
debug_normal(const char *file, int line, const char *fmt, ...)
{
  debug_lock();

	dprintf("[D] %s:%d: ", file, line);

	va_list ap;
	va_start(ap, fmt);
	vdprintf(fmt, ap);
	va_end(ap);
	
  debug_unlock();
}

#define DEBUG_TRACEFRAMES	10

static void
debug_trace(uintptr_t ebp, uintptr_t *eips)
{
	int i;
	uintptr_t *frame = (uintptr_t *) ebp;

	for (i = 0; i < DEBUG_TRACEFRAMES && frame; i++) {
		eips[i] = frame[1];		/* saved %eip */
		frame = (uintptr_t *) frame[0];	/* saved %ebp */
	}
	for (; i < DEBUG_TRACEFRAMES; i++)
		eips[i] = 0;
}

gcc_noinline void
debug_panic(const char *file, int line, const char *fmt,...)
{
	int i;
	uintptr_t eips[DEBUG_TRACEFRAMES];
	va_list ap;

  debug_lock();

	dprintf("[P] %s:%d: ", file, line);

	va_start(ap, fmt);
	vdprintf(fmt, ap);
	va_end(ap);

	debug_trace(read_ebp(), eips);
	for (i = 0; i < DEBUG_TRACEFRAMES && eips[i] != 0; i++)
		dprintf("\tfrom 0x%08x\n", eips[i]);

	dprintf("Kernel Panic !!!\n");

  debug_unlock();
	//intr_local_disable();
	halt();
}

void
debug_warn(const char *file, int line, const char *fmt,...)
{
  debug_lock();

	dprintf("[W] %s:%d: ", file, line);

	va_list ap;
	va_start(ap, fmt);
	vdprintf(fmt, ap);
	va_end(ap);

  debug_unlock();
}

#endif /* DEBUG_MSG */
