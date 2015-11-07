#ifndef _KERN_LIB_TQUEUE_H
#define _KERN_LIB_TQUEUE_H

#define MAX_SIZE 100

struct entry {
  unsigned int pid;
  unsigned int cpu_idx;
};

struct TQueue_t{
    unsigned int head, tail;
    struct entry queue[MAX_SIZE];
};

typedef struct TQueue_t TQueue;

void TQ_init(TQueue* q);
void TQ_enqueue(TQueue* q, unsigned int pid, unsigned int cpu_idx);
struct entry TQ_dequeue(TQueue* q);
int TQ_isEmpty(TQueue* q);

#endif
