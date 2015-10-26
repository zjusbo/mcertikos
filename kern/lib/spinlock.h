#ifndef _KERN_LIB_SPINLOCK_H_
#define _KERN_LIB_SPINLOCK_H_

#ifdef _KERN_

#include <lib/gcc.h>
#include <lib/types.h>
#include <lib/x86.h>

typedef struct {
	uint32_t lock_holder;
	volatile uint32_t lock;
} spinlock_t;

void spinlock_init(spinlock_t *lk);

#ifdef DEBUG_LOCKHOLDING

#define spinlock_acquire(lk)	spinlock_acquire_(lk, __FILE__, __LINE__)
#define spinlock_release(lk)	spinlock_release_(lk, __FILE__, __LINE__)
#define spinlock_try_acquire(lk)	spinlock_try_acquire_(lk, __FILE__, __LINE__)

void spinlock_acquire_(spinlock_t *lk, ...);
void spinlock_release_(spinlock_t *lk, const char *file, int line);
int spinlock_try_acquire_(spinlock_t *lk, const char *file, int line);

#else

void spinlock_acquire(spinlock_t *lk);
void spinlock_release(spinlock_t *lk);
int spinlock_try_acquire(spinlock_t *lk);

#endif

bool spinlock_holding(spinlock_t *lock);

#endif /* _KERN_ */

#endif /* !_KERN_LIB_SPINLOCK_H_ */
