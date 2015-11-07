#ifndef _KERN_LIB_CV
#define _KERN_LIB_CV

#include <lib/lock.h>
#include <lib/TQueue.h>

typedef struct cv{
  TQueue waiting;
} cv_t;

void cv_init(cv_t* conv);
void wait(cv_t* conv, lock* lk, unsigned int pid);
void signal(cv_t* conv);

#endif
