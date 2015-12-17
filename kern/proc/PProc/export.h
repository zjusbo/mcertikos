#ifndef _KERN_THREAD_PTCBINIT_H_
#define _KERN_THREAD_PTCBINIT_H_

unsigned int proc_create(void *elf_addr, unsigned int);
void proc_start_user(void);

#endif

