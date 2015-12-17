#include <kern/lib/ipc.h>
#include <kern/lib/pmap.h>
#define MSG_BUF_SIZE 100

struct MsgBlock msgBlock[NUM_IDS];
struct MsgQueue msgQueue;

spinlock_t msg_lock;

char msg_buf[MSG_BUF_SIZE];

void ipc_init(void){
  //init locks
  int i;
  spinlock_init(&msg_lock);
  //init msgBlock;
  for(i = 0; i < NUM_IDS; i++){
    msgBlock[i].prev = NUM_IDS;
    msgBlock[i].next = NUM_IDS;
    msgBlock[i].send_pid = i;
    msgBlock[i].recv_pid = NUM_IDS;
    msgBlock[i].length = 0;
    msgBlock[i].buffer_addr = 0x0;
    msgBlock[i].recv_cv = msgBlock[i].send_cv = 0; // we only use the address of these two variables. We don't care its value
  }
  //init msgQueue
  msgQueue.head = msgQueue.tail = NUM_IDS;
}

// copy data between different processes
void ipc_copy(const int dest_pid, int dest_user_addr, const int source_pid, int source_user_addr, int length){
  int len;
  while(length > 0){
    len = length < MSG_BUF_SIZE? length: MSG_BUF_SIZE;
    length -= len;
    pt_copyin(source_pid, source_user_addr, msg_buf, len);
    pt_copyout(msg_buf, dest_pid, dest_user_addr, len);
    source_user_addr += len;
    dest_user_addr += len;
  }
}

void msg_enqueue(int send_pid){
  int tail;
  tail = msgQueue.tail;
  //first element in the queue
  if(tail == NUM_IDS){
    msgBlock[send_pid].prev = NUM_IDS;
    msgBlock[send_pid].next = NUM_IDS;
    msgQueue.tail = msgQueue.head = send_pid;
  }  
  else{
    msgBlock[tail].next = send_pid;
    msgBlock[send_pid].prev = tail;
    msgBlock[send_pid].next = NUM_IDS;
    msgQueue.tail = send_pid;
  }
}

void msg_remove(int send_pid){
  int next, prev;
  prev = msgBlock[send_pid].prev;
  next = msgBlock[send_pid].next;
  // head of the queue
  if(prev == NUM_IDS){
    // only one element in the queue
    if(next == NUM_IDS){
      msgQueue.head = msgQueue.tail = NUM_IDS;
    }else{
      msgQueue.head = next;
      msgBlock[next].prev = NUM_IDS;
    }
  }
  else{
    // tail of the queue
    if(next == NUM_IDS){
      msgQueue.tail = prev;
      msgBlock[prev].next = NUM_IDS;
    }else{
      // TODO ? why do we need to check prev != next?
      msgBlock[prev].next = next;
      msgBlock[next].prev = prev;
    }
  }
  msgBlock[send_pid].prev = msgBlock[send_pid].next = NUM_IDS;
}


int msg_getBlockBySendID(int send_pid){
    int pid = msgQueue.head;
    while(pid != NUM_IDS){
      // pid is in the queue
      if(msgBlock[pid].send_pid == send_pid){
        return send_pid;
      }
      pid = msgBlock[pid].next;
    }
    return NUM_IDS;
}


int msg_getBlockByRecvID(int recv_pid){
  int pid = msgQueue.head;
  while(pid != NUM_IDS){
    if(msgBlock[pid].recv_pid == recv_pid){
      return msgBlock[pid].send_pid;
    }
  }
  return NUM_IDS;
}
