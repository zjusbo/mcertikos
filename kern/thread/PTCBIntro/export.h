#ifndef _KERN_THREAD_PTCBINTRO_H_
#define _KERN_THREAD_PTCBINTRO_H_

#ifdef _KERN_

unsigned int tcb_get_state(unsigned int pid);
void tcb_set_state(unsigned int pid, unsigned int state);
unsigned int tcb_get_prev(unsigned int pid);
void tcb_set_prev(unsigned int pid, unsigned int prev_pid);
unsigned int tcb_get_next(unsigned int pid);
void tcb_set_next(unsigned int pid, unsigned int next_pid);
void tcb_init_at_id(unsigned int cpu_idx, unsigned int pid);

void* tcb_get_chan(unsigned int pid);
void tcb_set_chan(unsigned int pid, void *state);

#include <kern/fs/stat.h>
#include <kern/fs/dinode.h>
#include <kern/fs/inode.h>
#include <kern/fs/file.h>

struct file** tcb_get_openfiles(unsigned int pid);
void tcb_set_openfiles(unsigned int pid, int fd, struct file* f);
struct inode* tcb_get_cwd(unsigned int pid);
void tcb_set_cwd(unsigned int pid, struct inode* d);
  
#endif /* _KERN_ */

#endif /* !_KERN_THREAD_PTCBINTRO_H_ */

