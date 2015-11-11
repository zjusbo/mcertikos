// Buffer cache.
//
// The buffer cache is a linked list of buf structures holding
// cached copies of disk block contents.  Caching disk blocks
// in memory reduces the number of disk reads and also provides
// a synchronization point for disk blocks used by multiple processes.
// 
// Interface:
// * To get a buffer for a particular disk block, call bread.
// * After changing buffer data, call bwrite to write it to disk.
// * When done with the buffer, call brelse.
// * Do not use the buffer after calling brelse.
// * Only one process at a time can use a buffer,
//     so do not keep them longer than necessary.
// 
// The implementation uses three state flags internally:
// * B_BUSY: the block has been returned from bread
//     and has not been passed back to brelse.  
// * B_VALID: the buffer data has been read from the disk.
// * B_DIRTY: the buffer data has been modified
//     and needs to be written to disk.

#include <kern/lib/types.h>
#include <kern/lib/debug.h>
#include <kern/lib/spinlock.h>
#include <kern/lib/buf.h>
#include "params.h"

struct {
  spinlock_t lock;
  struct buf buf[NBUF];

  // Linked list of all buffers, through prev/next.
  // head.next is most recently used.
  struct buf head;
} bcache;

void bufcache_init(void)
{
  struct buf *b;

  spinlock_init(&bcache.lock);
  
  // Create linked list of buffers
  bcache.head.prev = &bcache.head;
  bcache.head.next = &bcache.head;
  for (b = bcache.buf; b < bcache.buf+NBUF; b++){
    b->next = bcache.head.next;
    b->prev = &bcache.head;
    b->dev = -1;
    bcache.head.next->prev = b;
    bcache.head.next = b;
  }
}

/**
 * Look through buffer cache for sector on device dev.
 * If not found, allocate fresh block.
 * In either case, return B_BUSY buffer.
 */
static struct buf* bufcache_get(uint32_t dev, uint32_t sector)
{
  struct buf *b;

  spinlock_acquire(&bcache.lock);

 loop:
  // Is the sector already cached?
  for (b = bcache.head.next; b != &bcache.head; b = b->next){
    if (b->dev == dev && b->sector == sector){
      if (!(b->flags & B_BUSY)){
        b->flags |= B_BUSY;
        spinlock_release(&bcache.lock);
        return b;
      }
      thread_sleep(b, &bcache.lock);
      goto loop;
    }
  }
  
  // Not cached; recycle some non-busy and clean buffer.
  for (b = bcache.head.prev; b != &bcache.head; b = b->prev){
    if ((b->flags & B_BUSY) == 0 && (b->flags & B_DIRTY) == 0){
      b->dev = dev;
      b->sector = sector;
      b->flags = B_BUSY;
      spinlock_release(&bcache.lock);
      return b;
    }
  }
  KERN_PANIC("bufcache_get: no buffers");
}

/**
 * Return a B_BUSY buf with the contents of the indicated disk sector.
 */
struct buf* bufcache_read(uint32_t dev, uint32_t sector)
{
  struct buf *b;

  b = bufcache_get(dev, sector);
  if (!(b->flags & B_VALID)){
    ide_rw(b);
  }
  return b;
}

/**
 * Write b's contents to disk.  Must be B_BUSY.
 */
void bufcache_write(struct buf *b)
{
  if((b->flags & B_BUSY) == 0)
    KERN_PANIC("bwrite");

  b->flags |= B_DIRTY;
  ide_rw(b);
}

/**
 * Release a B_BUSY buffer.
 * Move to the head of the MRU list.
 */
void bufcache_release(struct buf *b)
{
  if((b->flags & B_BUSY) == 0)
    KERN_PANIC("brelse");

  spinlock_acquire(&bcache.lock);

  b->next->prev = b->prev;
  b->prev->next = b->next;
  b->next = bcache.head.next;
  b->prev = &bcache.head;
  bcache.head.next->prev = b;
  bcache.head.next = b;

  b->flags &= ~B_BUSY;
  thread_wakeup(b);

  spinlock_release(&bcache.lock);
}

