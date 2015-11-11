#include <lib/x86.h>
#include <lib/debug.h>
#include <lib/thread.h>

#include <kern/fs/params.h>
#include <kern/fs/stat.h>
#include <kern/fs/dinode.h>
#include <kern/fs/inode.h>
#include <kern/fs/path.h>
#include <kern/fs/file.h>


/**
 * The structure for the thread control block (TCB).
 * We are storing the set of TCBs in doubly linked lists.
 * To this purpose, in addition to the thread state, you also
 * need to save the thread (process) id of the previous and
 * the next TCB.
 * Since the value 0 is reserved for thread id 0, we use NUM_IDS
 * to represent the NULL index.
 */
struct TCB {
  t_state state; 
  unsigned int prev;
  unsigned int next;
  void *channel;
  struct file *openfiles[NOFILE];  // Open files
  struct inode *cwd;               // Current working directory
};

struct TCB TCBPool[NUM_IDS];


unsigned int tcb_get_state(unsigned int pid)
{
	return TCBPool[pid].state;
}

void tcb_set_state(unsigned int pid, unsigned int state)
{
  //KERN_INFO("_____0_____ tcb_set_state: %d -> %d\n", pid, state);
  TCBPool[pid].state = state;
}

unsigned int tcb_get_prev(unsigned int pid)
{
	return TCBPool[pid].prev;
}

void tcb_set_prev(unsigned int pid, unsigned int prev_pid)
{
	TCBPool[pid].prev = prev_pid;
}

unsigned int tcb_get_next(unsigned int pid)
{
	return TCBPool[pid].next;
}

void tcb_set_next(unsigned int pid, unsigned int next_pid)
{
	TCBPool[pid].next = next_pid;
}

void tcb_init_at_id(unsigned int pid)
{
	TCBPool[pid].state = TSTATE_DEAD;
	TCBPool[pid].prev = NUM_IDS;
	TCBPool[pid].next = NUM_IDS;
	TCBPool[pid].channel = 0;
	memzero(TCBPool[pid].openfiles, sizeof *(TCBPool[pid].openfiles));
	//	memzero(TCBPool[pid].cwd, sizeof *(TCBPool[pid].cwd));
	TCBPool[pid].cwd = namei("/");
}

/*** NEW ***/

void* tcb_get_chan(unsigned int pid)
{
  return TCBPool[pid].channel;
}

void tcb_set_chan(unsigned int pid, void *chan)
{
  TCBPool[pid].channel = chan;
}

struct file** tcb_get_openfiles(unsigned int pid)
{
  return TCBPool[pid].openfiles;
}

void tcb_set_openfiles(unsigned int pid, int fd, struct file* f)
{
  (TCBPool[pid].openfiles)[fd] = f;
}

struct inode* tcb_get_cwd(unsigned int pid)
{
  return TCBPool[pid].cwd;
}

void tcb_set_cwd(unsigned int pid, struct inode* d)
{
  TCBPool[pid].cwd = d;
}
