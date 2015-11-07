// define the lock structure

#ifndef _KERN_LIB_LOCK_H
#define _KERN_LIB_LOCK_H

#include "spinlock.h"
#include "TQueue.h"
#define FREE 0
#define BUSY 1
struct lock_t{
    int value; 
    spinlock_t spinlock;
    TQueue waiting; // waiting queue 
};
typedef struct lock_t lock;
void lock_init(lock* lk);
void lock_acquire(lock* lk);
void lock_release(lock* lk);

#endif
