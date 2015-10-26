#include "lib/x86.h"

#include "import.h"

/**
 * Initializes all the thread queues with
 * tqueue_init_at_id.
 */
void tqueue_init(unsigned int mbi_addr)
{
	unsigned int cpu_idx, chid;

	tcb_init(mbi_addr);

	chid = 0;
	cpu_idx = 0;
	while (cpu_idx < NUM_CPUS) {
		while (chid <= NUM_IDS) {
			tqueue_init_at_id(cpu_idx, chid);
			chid++;
		}
		chid = 0;
		cpu_idx++;
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
	unsigned int tail;

	tail = tqueue_get_tail(chid);

	if (tail == NUM_IDS) {
		tcb_set_prev(pid, NUM_IDS);
		tcb_set_next(pid, NUM_IDS);
		tqueue_set_head(chid, pid);
		tqueue_set_tail(chid, pid);
	} else {
		tcb_set_next(tail, pid);
		tcb_set_prev(pid, tail);
		tcb_set_next(pid, NUM_IDS);
		tqueue_set_tail(chid, pid);
	}
}

/**
 * Reverse action of tqueue_enqueue, i.g., pops a TCB from the head of specified queue.
 * It returns the poped thread's id, or NUM_IDS if the queue is empty.
 * Hint: there are mutiple cases in this function.
 */
unsigned int tqueue_dequeue(unsigned int chid)
{
	unsigned int head, next, pid;

	pid = NUM_IDS;
	head = tqueue_get_head(chid);

	if (head != NUM_IDS) {
		pid = head;
		next = tcb_get_next(head);

		if(next == NUM_IDS) {
			tqueue_set_head(chid, NUM_IDS);
			tqueue_set_tail(chid, NUM_IDS);
		} else {
			tcb_set_prev(next, NUM_IDS);
			tqueue_set_head(chid, next);
		}
    tcb_set_prev(pid, NUM_IDS);
    tcb_set_next(pid, NUM_IDS);
	}

	return pid;
}

/**
 * Removes the TCB #pid from the queue #chid.
 * Hint: there are many cases in this function.
 */
void tqueue_remove(unsigned int chid, unsigned int pid)
{
	unsigned int prev, next;

	prev = tcb_get_prev(pid);
	next = tcb_get_next(pid);

	if (prev == NUM_IDS) {
		if (next == NUM_IDS) {
			tqueue_set_head(chid, NUM_IDS);
			tqueue_set_tail(chid, NUM_IDS);
		} else {
			tcb_set_prev(next, NUM_IDS);
			tqueue_set_head(chid, next);
		}
	} else {
		if (next == NUM_IDS) {
			tcb_set_next(prev, NUM_IDS);
			tqueue_set_tail(chid, prev);
		} else {
			if (prev != next)
				tcb_set_next(prev, next);
			tcb_set_prev(next, prev);
		}
	}
  tcb_set_prev(pid, NUM_IDS);
  tcb_set_next(pid, NUM_IDS);
}
