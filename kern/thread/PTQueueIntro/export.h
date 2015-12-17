#ifndef _KERN_THREAD_PTCBINTRO_H_
#define _KERN_THREAD_PTCBINTRO_H_

unsigned int tqueue_get_head(unsigned int chid);
void tqueue_set_head(unsigned int chid, unsigned int head);
unsigned int tqueue_get_tail(unsigned int chid);
void tqueue_set_tail(unsigned int chid, unsigned int tail);
void tqueue_init_at_id(unsigned int chid);

#endif

