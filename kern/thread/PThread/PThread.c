#include <lib/x86.h>
#include <lib/thread.h>

#include "import.h"

void thread_init(unsigned int mbi_addr)
{
	tqueue_init(mbi_addr);
	set_curid(0);
	tcb_set_state(0, TSTATE_RUN);
}

/**
 * Allocates new child thread context, set the state of the new child thread
 * as ready, and pushes it to the ready queue.
 * It returns the child thread id.
 */
unsigned int thread_spawn(void *entry, unsigned int id, unsigned int quota)
{
  // TODO
  int child = kctx_new(entry, id, quota);
  tcb_set_state(child, TSTATE_READY);
  tqueue_enqueue(NUM_IDS, child);
  return child;
}

/**
 * Yield to the next thread in the ready queue.
 * You should set the currently running thread state as ready,
 * and push it back to the ready queue.
 * And set the state of the poped thread as running, set the
 * current thread id, then switches to the new kernel context.
 * Hint: if you are the only thread that is ready to run,
 * do you need to switch to yourself?
 */
void thread_yield(void)
{
  // TODO
  int curid = get_curid();
  int next_ready = tqueue_dequeue(NUM_IDS);
  if (next_ready == NUM_IDS) {
    //no ready threads then return
    return;
  }
  //set current running thread's state to ready
  tcb_set_state(curid, TSTATE_READY);
  //push the current thread to ready queue
  tqueue_enqueue(NUM_IDS, curid);
  
  //set the state of next ready thread
  tcb_set_state(next_ready, TSTATE_RUN);
  
  //set current thread id, switch context
  set_curid(next_ready);
  kctx_switch(curid, next_ready); 
}
