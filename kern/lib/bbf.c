#include <lib/bbf.h>
#include <lib/debug.h>

struct bbf_t {
  unsigned int front;
  unsigned int tail;
  
  lock lk;
  cv_t full;
  cv_t empty;
  
  int buffer[MAX_BUFFSIZE];
};

struct bbf_t BoundedBuffer;

int bbf_get_front_idx(void) {
  return BoundedBuffer.front;
}

int bbf_get_tail_idx(void) {
  return BoundedBuffer.tail;
}

int bbf_get_size(void) {
  return BoundedBuffer.tail - BoundedBuffer.front;
}

void bbf_init(void) {
  BoundedBuffer.front = 0;
  BoundedBuffer.tail = 0;
  lock_init(&(BoundedBuffer.lk));
  cv_init(&(BoundedBuffer.full));
  cv_init(&(BoundedBuffer.empty));
}

int bbf_get(void) {
  //KERN_DEBUG("Buffer front %d. buffer tail %d, at %d \n", BoundedBuffer.front, BoundedBuffer.tail, &BoundedBuffer);
  //KERN_DEBUG("CPU %d, thread %d, consumer trying to get an item, buffer size is %d\n", get_pcpu_idx(), get_curid(), BoundedBuffer.tail - BoundedBuffer.front);
  //KERN_DEBUG("CPU %d, thread %d, acquring bff_lock\n", get_pcpu_idx(), get_curid());

  lock_acquire(&(BoundedBuffer.lk));
  //KERN_DEBUG("CPU %d, thread %d, acqured bff_lock\n", get_pcpu_idx(), get_curid());

  unsigned int pid = get_curid();
  while (BoundedBuffer.front == BoundedBuffer.tail) {
    //KERN_DEBUG("CPU %d, thread %d, buffer is empty!\n", get_pcpu_idx(), get_curid());
    wait(&(BoundedBuffer.empty), &(BoundedBuffer.lk), pid);
  }
  int item = BoundedBuffer.buffer[BoundedBuffer.front % MAX_BUFFSIZE];
  BoundedBuffer.front++;
  signal(&(BoundedBuffer.full));
  lock_release(&(BoundedBuffer.lk));
  //KERN_DEBUG("CPU %d, thread %d, bbf lock is released!\n", get_pcpu_idx(), get_curid());
  return item;
}

void bbf_put(int item) {
  //KERN_DEBUG("Buffer front %d. buffer tail %d, at %d \n", BoundedBuffer.front, BoundedBuffer.tail, &BoundedBuffer);
  //KERN_DEBUG("CPU %d, thread %d, producer trying to put an item %d, buffer size is %d\n", get_pcpu_idx(), get_curid(),item, BoundedBuffer.tail - BoundedBuffer.front);
  //KERN_DEBUG("CPU %d, thread %d, acquring bff_lock\n", get_pcpu_idx(), get_curid());
  lock_acquire(&(BoundedBuffer.lk));
  //KERN_DEBUG("CPU %d, thread %d, acqured bff_lock\n", get_pcpu_idx(), get_curid());
  unsigned int pid = get_curid();
  while ((BoundedBuffer.tail - BoundedBuffer.front) == MAX_BUFFSIZE) {
    //KERN_DEBUG("CPU %d, thread %d, buffer is full!\n", get_pcpu_idx(), get_curid());
    wait(&(BoundedBuffer.full), &(BoundedBuffer.lk), pid);
  }
  BoundedBuffer.buffer[BoundedBuffer.tail % MAX_BUFFSIZE] = item;
  BoundedBuffer.tail++;
  signal(&(BoundedBuffer.empty));
  lock_release(&(BoundedBuffer.lk));
  //KERN_DEBUG("CPU %d, thread %d, bbf lock is released!\n", get_pcpu_idx(), get_curid());
}
