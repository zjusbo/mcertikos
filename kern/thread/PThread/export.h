#ifndef _KERN_THREAD_PTCBINIT_H_
#define _KERN_THREAD_PTCBINIT_H_

void thread_init(unsigned int mbi_addr);
unsigned int thread_spawn(void *entry, unsigned int id, unsigned int quota);
void thread_yield(void);

#endif

