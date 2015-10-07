#ifndef _KERN_THREAD_PTCBINTRO_H_
#define _KERN_THREAD_PTCBINTRO_H_

unsigned int tcb_get_state(unsigned int pid);
void tcb_set_state(unsigned int pid, unsigned int state);
unsigned int tcb_get_prev(unsigned int pid);
void tcb_set_prev(unsigned int pid, unsigned int prev_pid);
unsigned int tcb_get_next(unsigned int pid);
void tcb_set_next(unsigned int pid, unsigned int next_pid);
void tcb_init_at_id(unsigned int pid);

#endif

