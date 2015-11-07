#include <lib/types.h>
#include <lib/debug.h>
#include <lib/x86.h>
#include <pcpu/PCPUIntro/export.h>
#include <dev/intr.h>
#include <lib/thread.h>
#include "thread/PTQueueInit/export.h"
#include "TQueue.h"
#include "spinlock.h"
#include "thread/PTCBIntro/export.h"
#include "scheduler.h"
#include "lock.h"

extern spinlock_t tqueue_lock[NUM_CPUS];

void scheduler_suspend(spinlock_t * sl){
    unsigned int pcpu_idx;
    unsigned int pid, next_pid;
    pcpu_idx = get_pcpu_idx();
    pid = get_curid();
    
    //KERN_DEBUG("in scheduler_suspend, CPU %d, thread %d, getting scheduler_spinlock:%d\n", pcpu_idx, pid, pcpu_idx);
    spinlock_acquire(&scheduler_lock[pcpu_idx]);
    //KERN_DEBUG("CPU %d, thread %d, got scheduler_spinlock:%d\n", pcpu_idx, pid, pcpu_idx);

    spinlock_release(sl);
    //KERN_DEBUG("CPU %d, thread %d, in scheduler_suspend, outside spinlock has been released\n", pcpu_idx, pid);
   

    spinlock_acquire(&(tqueue_lock[get_pcpu_idx()]));
    next_pid = tqueue_dequeue(NUM_IDS); 


    //KERN_DEBUG("in scheduler_suspend CPU %d, thread %d, next_pid = %d\n",pcpu_idx, pid, next_pid);
    // content switch
    

      tcb_set_state(next_pid, TSTATE_RUN);

      spinlock_release(&scheduler_lock[pcpu_idx]);
      spinlock_release(&(tqueue_lock[get_pcpu_idx()]));

      //KERN_DEBUG("CPU %d, thread %d, release scheduler_spinlock:%d",pcpu_idx, pid);
      set_curid(next_pid); // previously a BUG HERE, I FORGOT TO ADD THIS LINE HERE
      kctx_switch(pid, next_pid);
}

void scheduler_suspend_lock(lock * lk){
    unsigned int pcpu_idx;
    unsigned int pid, next_pid;
    unsigned int i;
    pcpu_idx = get_pcpu_idx();
    pid = get_curid();
//    KERN_DEBUG("CPU %d, thread %d, getting scheduler_spinlock:%d\n", pcpu_idx, pid, pcpu_idx);
    spinlock_acquire(&scheduler_lock[pcpu_idx]);
//    KERN_DEBUG("CPU %d, thread %d, got scheduler_spinlock:%d\n", pcpu_idx, pid, pcpu_idx);
    //KERN_DEBUG("CPU %d, thread %d, in scheduler_suspend_lock, going to release lock\n", pcpu_idx, pid);

    spinlock_release(&scheduler_lock[pcpu_idx]);
    lock_release(lk);
    spinlock_acquire(&scheduler_lock[pcpu_idx]);

    //KERN_DEBUG("CPU %d, thread %d, in scheduler_suspend_lock, lock has been released\n", pcpu_idx, pid);

    spinlock_acquire(&(tqueue_lock[get_pcpu_idx()]));
    next_pid = tqueue_dequeue(NUM_IDS); 

    //KERN_DEBUG("in scheduler_suspend_lock CPU %d, thread %d, next_pid = %d\n",pcpu_idx, pid, next_pid);
    // content switch

    tcb_set_state(next_pid, TSTATE_RUN); 

    spinlock_release(&scheduler_lock[pcpu_idx]);
    spinlock_release(&(tqueue_lock[get_pcpu_idx()]));

    //KERN_DEBUG("CPU %d, thread %d, release scheduler_spinlock:%d",pcpu_idx, pid);
    set_curid(next_pid); // previously a BUG HERE, I FORGOT TO ADD THIS LINE HERE`
    kctx_switch(pid, next_pid);
}

void scheduler_makeReady(unsigned int pid, unsigned int cpu_idx){
  //  KERN_DEBUG("CPU %d, thread %d, getting scheduler_spinlock:%d\n", pcpu_idx, get_curid(), pcpu_idx);
    spinlock_acquire(&scheduler_lock[get_pcpu_idx()]);
   // KERN_DEBUG("CPU %d, thread %d, got scheduler_spinlock:%d\n", pcpu_idx, get_curid(), pcpu_idx);

    spinlock_acquire(&(tqueue_lock[cpu_idx]));
    tqueue_enqueue_pcpu_idx(NUM_IDS, pid, cpu_idx);
    tcb_set_state(pid, TSTATE_READY);
    spinlock_release(&(tqueue_lock[cpu_idx]));

    //KERN_DEBUG("put thread %d on CPU %d\n", pid, cpu_idx);

     spinlock_release(&scheduler_lock[get_pcpu_idx()]);
}

void scheduler_init(){
    int i;
    for(i = 0; i <= NUM_CPUS; i++){
        spinlock_init(&scheduler_lock[i]);
    }
}
