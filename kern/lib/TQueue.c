#include <thread/PTQueueInit/export.h>
#include <thread/PTCBIntro/export.h>
#include <lib/x86.h>
#include "TQueue.h"
#include <lib/debug.h>

void TQ_init(TQueue* q){
    q->head = q->tail = 0;
}
void TQ_enqueue(TQueue* q, unsigned int pid, unsigned int cpu_idx){
  if (q->tail - q->head > MAX_SIZE) {
    //queue is full
    return;
  }
  //KERN_DEBUG("QUEUE : %d, %d", q->head, q->tail);
  q->queue[q->tail % MAX_SIZE].pid = pid;
  q->queue[q->tail % MAX_SIZE].cpu_idx = cpu_idx;
  q->tail++;
}


struct entry TQ_dequeue(TQueue* q){
  if (q->head == q->tail) {
    //queue is empty
    return;
  }
  //KERN_DEBUG("QUEUE : %d, %d", q->head, q->tail);
  struct entry front = q->queue[q->head % MAX_SIZE];
  q->head++;
  return front;
}

int TQ_isEmpty(TQueue* q){
    return q->head == q->tail;
}
