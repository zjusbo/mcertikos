#include <lib/x86.h>
#include <pcpu/PCPUIntro/export.h>

/**
 * The structure for thread queues.
 * The queue structure only needs to record
 * the head and tail index, since we've already implemented
 * the doubly linked list in the TCB structure.
 * This implementation is valid if at any given time, a thread
 * is in at most one thread queue.
 */
struct TQueue {
	unsigned int head;
	unsigned int tail;
};

/**
 * The mCertiKOS kernel needs NUM_IDS + 1 thread queues.
 * The first NUM_IDS thread queues are thread sleep queues for the NUM_IDS threads/processes.
 * A thread can sleep on other thread's sleeping queue, waiting for the other thread
 * to perform some related tasks and wake it up.
 * You may not need these sleeping queues in this lab, but they will be particularly helpful
 * when you implement the inter-process communication protocols later.
 * The last queue with id NUM_IDS is called ready queue.
 * Any threads that are ready to be sheduled are pushed to the ready queue,
 * and are scheduled in a round-robin manner.
 */
struct TQueue TQueuePool[NUM_CPUS][NUM_IDS + 1];

unsigned int tqueue_get_head(unsigned int chid)
{
	return TQueuePool[get_pcpu_idx()][chid].head;
}

void tqueue_set_head(unsigned int chid, unsigned int head)
{
	TQueuePool[get_pcpu_idx()][chid].head = head;
}

unsigned int tqueue_get_tail(unsigned int chid)
{
	return TQueuePool[get_pcpu_idx()][chid].tail;
}

void tqueue_set_tail(unsigned int chid, unsigned int tail)
{
	TQueuePool[get_pcpu_idx()][chid].tail = tail;
}

void tqueue_init_at_id(unsigned int cpu_idx, unsigned int chid)
{
	TQueuePool[cpu_idx][chid].head = NUM_IDS;
	TQueuePool[cpu_idx][chid].tail = NUM_IDS;
}

