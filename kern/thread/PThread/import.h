#ifndef _KERN_THREAD_PTHREAD_H_
#define _KERN_THREAD_PTHREAD_H_

#ifdef _KERN_

unsigned int kctx_new(void *entry, unsigned int id, unsigned int quota);
void kctx_switch(unsigned int from_pid, unsigned int to_pid);

void tcb_set_state(unsigned int pid, unsigned int state);

void tqueue_init(unsigned int mbi_addr);
void tqueue_enqueue(unsigned int chid, unsigned int pid);
unsigned int tqueue_dequeue(unsigned int chid);

unsigned int get_curid(void);
void set_curid(unsigned int curid);

#endif /* _KERN_ */

#endif /* !_KERN_THREAD_PTHREAD_H_ */
