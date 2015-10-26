#ifndef _KERN_THREAD_PTQUEUEINTRO_H_
#define _KERN_THREAD_PTQUEUEINTRO_H_

#ifdef _KERN_

unsigned int tqueue_get_head(unsigned int chid);
void tqueue_set_head(unsigned int chid, unsigned int head);
unsigned int tqueue_get_tail(unsigned int chid);
void tqueue_set_tail(unsigned int chid, unsigned int tail);
void tqueue_init_at_id(unsigned int cpu_idx, unsigned int chid);

#endif /* _KERN_ */

#endif /* !_KERN_THREAD_PTQUEUEINTRO_H_ */
