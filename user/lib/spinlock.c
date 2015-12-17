#include <spinlock.h>
#include <types.h>

static inline uint32_t
xchg(volatile uint32_t *addr, uint32_t newval)
{
	uint32_t result;

	asm volatile("lock; xchgl %0, %1" :
		     "+m" (*addr), "=a" (result) :
		     "1" (newval) :
		     "cc");
	return result;
}

void
spinlock_init(spinlock_t *lk)
{
	*lk = 0;
}

void
spinlock_acquire(spinlock_t *lk)
{
	while(xchg(lk, 1) != 0)
		asm volatile("pause");
}

// Release the lock.
void
spinlock_release(spinlock_t *lk)
{
	if (spinlock_holding(lk) == FALSE)
		return;

	xchg(lk, 0);
}

// Check whether this cpu is holding the lock.
bool
spinlock_holding(spinlock_t *lock)
{
	return *lock;
}
