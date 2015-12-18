#ifndef _KERN_LIB_IPC
#define _KERN_LIB_PIC
#include <kern/lib/x86.h>
#include <kern/lib/spinlock.h>
struct MsgBlock{
  int prev, next;
  int send_pid, recv_pid;
  int length;
  int buffer_addr;
  char recv_cv, send_cv; // chan for threads to sleep on, we only need the address of these two variables.
};

struct MsgQueue{
  int head, tail;
};


void ipc_init(void);
void ipc_copy(const int dest_pid, int dest_user_addr, const int source_pid, int source_user_addr, int length);
void msg_enqueue(int);
void msg_remove(int);
int msg_getBlockBySendID(int);
int msg_getBlockByRecvID(int);

#endif
