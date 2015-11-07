#include <lib/cv.h>
#include <lib/scheduler.h>
#include <lib/debug.h>

void cv_init(cv_t* conv) {
  TQ_init(&(conv->waiting));
}

void wait(cv_t* conv, lock* lk, unsigned int pid) {
  //KERN_DEBUG("CPU %d,thread %d, is waiting on CV\n", get_pcpu_idx(), pid);
  //KERN_DEBUG("CPU %d,thread %d, is being put on CV's queue\n", get_pcpu_idx(), pid);
  TQ_enqueue(&(conv->waiting), pid, get_pcpu_idx());
  //KERN_DEBUG("CPU %d,thread %d, has been put on CV's queue\n", get_pcpu_idx(), pid);
  scheduler_suspend_lock(lk);
  lock_acquire(lk);
}

void signal(cv_t* conv) {
  //KERN_DEBUG("CPU %d,thread %d, is signaling CV\n", get_pcpu_idx(), get_curid());
  if (!TQ_isEmpty(&(conv->waiting))) {
    struct entry next = TQ_dequeue(&(conv->waiting));
    scheduler_makeReady(next.pid, next.cpu_idx);
  }
}
