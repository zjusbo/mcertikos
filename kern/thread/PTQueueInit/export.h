#ifndef _KERN_THREAD_PTQUEUEINIT_H_
#define _KERN_THREAD_PTQUEUEINIT_H_

#ifdef _KERN_

void tqueue_init(unsigned int mbi_addr);
void tqueue_enqueue(unsigned int chid, unsigned int pid);
unsigned int tqueue_dequeue(unsigned int chid);
void tqueue_remove(unsigned int chid, unsigned int pid);

void tqueue_enqueue_pcpu_idx(unsigned int chid, unsigned int pid, unsigned int pcpu_idx);
#endif /* _KERN_ */

#endif /* !_KERN_THREAD_PTQUEUEINIT_H_ */

