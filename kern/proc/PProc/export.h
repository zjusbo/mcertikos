#ifndef _KERN_THREAD_PTCBINIT_H_
#define _KERN_THREAD_PTCBINIT_H_

#ifdef _KERN_

unsigned int proc_create(void *elf_addr, unsigned int);
void proc_start_user(void);

#endif /* _KERN_ */

#endif /* !_KERN_THREAD_PICBINIT_H_ */

