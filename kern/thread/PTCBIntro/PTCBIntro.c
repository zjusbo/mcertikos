#include <lib/x86.h>
#include <lib/thread.h>

/**
 * The structure for the thread control block (TCB).
 * We are storing the set of TCBs in doubly linked lists.
 * To this purpose, in addition to the thread state, you also
 * need to save the thread (process) id of the previous and
 * the next TCB.
 * Since the value 0 is reserved for thread id 0, we use NUM_IDS
 * to represent the NULL index.
 */
struct TCB {
	t_state state;
	unsigned int prev;
	unsigned int next;
};

struct TCB TCBPool[NUM_IDS];


unsigned int tcb_get_state(unsigned int pid)
{
	return TCBPool[pid].state;
}

void tcb_set_state(unsigned int pid, unsigned int state)
{
	TCBPool[pid].state = state;
}

unsigned int tcb_get_prev(unsigned int pid)
{
	return TCBPool[pid].prev;
}

void tcb_set_prev(unsigned int pid, unsigned int prev_pid)
{
	TCBPool[pid].prev = prev_pid;
}

unsigned int tcb_get_next(unsigned int pid)
{
	return TCBPool[pid].next;
}

void tcb_set_next(unsigned int pid, unsigned int next_pid)
{
	TCBPool[pid].next = next_pid;
}

void tcb_init_at_id(unsigned int pid)
{
	TCBPool[pid].state = TSTATE_DEAD;
	TCBPool[pid].prev = NUM_IDS;
	TCBPool[pid].next = NUM_IDS;
}
