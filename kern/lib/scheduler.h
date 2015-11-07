#ifndef _KERN_LIB_SCHEDULER
#define _KERN_LIB_SCHEDULER
#include "lock.h"
#include "spinlock.h"
struct scheduler_t{
    // TQueue readyList; we use the reday list defined in thread/PTQueueIntro/PTQueueIntro.c 
    spinlock_t spinlock;
};

spinlock_t scheduler_lock[NUM_CPUS];
void scheduler_suspend(spinlock_t * sl);
void scheduler_suspend_lock(lock * lk);
void scheduler_makeReady(unsigned int pid, unsigned int cpu_idx);
void scheduler_init(void);

#endif
