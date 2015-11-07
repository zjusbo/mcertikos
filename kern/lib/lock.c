#include "TQueue.h"
#include "scheduler.h"
#include "spinlock.h"
#include "lock.h"
#include <pcpu/PCPUIntro/export.h>

extern spinlock_t tqueue_lock[NUM_CPUS];

void lock_init(lock* lk){
    lk->value = FREE;
    spinlock_init(&(lk->spinlock));
    TQ_init(&(lk->waiting));
}

void lock_acquire(lock* lk){
    spinlock_acquire(&(lk->spinlock));
    if(lk->value != FREE){
        unsigned int pid = get_curid();
        TQ_enqueue(&(lk->waiting), pid, get_pcpu_idx());
        scheduler_suspend(&(lk->spinlock));
        // scheduler release spinlock
    }
    else{
        lk->value = BUSY;
        spinlock_release(&(lk->spinlock));
    }
}

void lock_release(lock* lk){
    spinlock_acquire(&(lk->spinlock));
    if(!TQ_isEmpty(&(lk->waiting))){
        struct entry next = TQ_dequeue(&(lk->waiting));
        scheduler_makeReady(next.pid, next.cpu_idx);
    }
    else{
        lk->value = FREE;
    }
    spinlock_release(&(lk->spinlock));
}
