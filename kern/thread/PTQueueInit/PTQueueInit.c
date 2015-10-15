// CONTENTED BY BO SONG
// DOUBLE CHECKED, BO SONG
#include "lib/x86.h"

#include "import.h"

/**
 * Initializes all the thread queues with
 * tqueue_init_at_id.
 */
void tqueue_init(unsigned int mbi_addr)
{
  // TODO: define your local variables here.

	tcb_init(mbi_addr);
	unsigned int id;
  // TODO
  	for(id = 0; id <= NUM_IDS; id++){
		//NUM_IDS + 1 QUEUES IN TOTAL
		tqueue_init_at_id(id);
	}
}

/**
 * Insert the TCB #pid into the tail of the thread queue #chid.
 * Recall that the doubly linked list is index based.
 * So you only need to insert the index.
 * Hint: there are multiple cases in this function.
 */
void tqueue_enqueue(unsigned int chid, unsigned int pid)
{
  // TODO
  	// remove pid from TCB list
	unsigned int tail_pid;
	// link pid to the tail of queue
	// move tail to point to pid 
        tail_pid = tqueue_get_tail(chid);
	if(tail_pid != NUM_IDS){
		tcb_set_next(tail_pid, pid);
	}
	else{
		tqueue_set_head(chid, pid);
	}

        tcb_set_prev(pid, tail_pid); // tail_pid may be NUM_IDS, but it does't matter
	tqueue_set_tail(chid, pid);
}

/**
 * Reverse action of tqueue_enqueue, i.g., pops a TCB from the head of specified queue.
 * It returns the poped thread's id, or NUM_IDS if the queue is empty.
 * Hint: there are mutiple cases in this function.
 */
unsigned int tqueue_dequeue(unsigned int chid)
{
  // TODO
  	unsigned int head_id, head_next_id;
	head_id = tqueue_get_head(chid);
	if(head_id == NUM_IDS) return NUM_IDS;
	
	// head_id is valid below this line
	head_next_id = tcb_get_next(head_id);
	if(head_next_id != NUM_IDS){
		// head_next_id exists
		tcb_set_prev(head_next_id, NUM_IDS);
	}
	else{
		tqueue_set_tail(chid, NUM_IDS);
	}
	tcb_set_next(head_id, NUM_IDS);
	tqueue_set_head(chid, head_next_id);
  	return head_id;
}

/**
 * Removes the TCB #pid from the queue #chid.
 * Hint: there are many cases in this function.
 */
void tqueue_remove(unsigned int chid, unsigned int pid)
{
  // TODO
   	unsigned int head_pid, tail_pid;
	unsigned int prev_pid, next_pid;
	head_pid = tqueue_get_head(chid);
	tail_pid = tqueue_get_tail(chid);
	prev_pid = tcb_get_prev(pid);
	next_pid = tcb_get_next(pid);
	if(head_pid == pid){
		// we are deleting head
		// move head to head->next
		tqueue_set_head(chid, next_pid);
	}
	if(tail_pid == pid){
		// we are deleting tail
		// move tail to tail->prev
		tqueue_set_tail(chid, prev_pid);
	}
	if(next_pid != NUM_IDS){
		// pid is not tail
		tcb_set_prev(next_pid, prev_pid);
	}
	if(prev_pid != NUM_IDS){
		// pid is not head
		tcb_set_next(prev_pid, next_pid);
	}
	
	tcb_set_next(pid, NUM_IDS);
	tcb_set_prev(pid, NUM_IDS);
}
