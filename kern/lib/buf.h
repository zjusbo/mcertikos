#ifndef _KERN_LIB_BUF_H_
#define _KERN_LIB_BUF_H_

struct buf {
  int32_t flags;
  int dev;
  uint32_t sector;
  struct buf *prev; // LRU cache list
  struct buf *next;
  struct buf *qnext; // disk queue
  uint8_t data[512];
};

#define B_BUSY  0x1  // buffer is locked by some process
#define B_VALID 0x2  // buffer has been read from disk
#define B_DIRTY 0x4  // buffer needs to be written to disk

#endif /* !_KERN_LIB_BUF_H_ */
