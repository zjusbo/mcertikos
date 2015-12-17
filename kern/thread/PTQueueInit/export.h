#ifndef _KERN_THREAD_PTQueueINIT_H_
#define _KERN_THREAD_PTQueueINIT_H_

void tqueue_init(unsigned int mbi_addr);
void tqueue_enqueue(unsigned int chid, unsigned int pid);
unsigned int tqueue_dequeue(unsigned int chid);
void tqueue_remove(unsigned int chid, unsigned int pid);

#endif

