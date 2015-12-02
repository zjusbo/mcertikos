// Simple PIO-based (non-DMA) IDE driver code.

#include <kern/lib/types.h>
#include <kern/lib/debug.h>
#include <kern/lib/x86.h>
#include <kern/thread/PThread/export.h>
#include <kern/lib/spinlock.h>
#include <kern/lib/buf.h>
#include <kern/dev/intr.h>
#include "ide.h"

// idequeue points to the buf now being read/written to the disk.
// idequeue->qnext points to the next buf to be processed.
// You must hold ide_lk while manipulating queue.

static spinlock_t ide_lk;
spinlock_t buf_lock;
static struct buf *idequeue;

static int havedisk1;
static void ide_start(struct buf*);

/**
 * Wait for IDE disk to become ready.
 */
static int
ide_wait(int checkerr)
{
  int r;

  while(((r = inb(0x1f7)) & (IDE_BSY|IDE_DRDY)) != IDE_DRDY) 
    ;
  if(checkerr && (r & (IDE_DF|IDE_ERR)) != 0)
    return -1;
  return 0;
}

void
ide_init(void)
{
  int i;

  spinlock_init(&ide_lk);
  spinlock_init(&buf_lock);
  picenable(IRQ_IDE1);
  ioapicenable(IRQ_IDE1, pcpu_ncpu() - 1);
  ide_wait(0);
  
  // Check if disk 1 is present
  outb(0x1f6, 0xe0 | (1<<4));
  for(i=0; i<1000; i++){
    if(inb(0x1f7) != 0){
      havedisk1 = 1;
      break;
    }
  }
  
  // Switch back to disk 0.
  outb(0x1f6, 0xe0 | (0<<4));
}

/**
 * Start the request for b.  Caller must hold ide_lk.
 */
static void
ide_start(struct buf *b)
{
  if(b == 0)
    KERN_PANIC("ide_start");

  ide_wait(0);
  outb(0x3f6, 0);  // generate interrupt
  outb(0x1f2, 1);  // number of sectors
  outb(0x1f3, b->sector & 0xff);
  outb(0x1f4, (b->sector >> 8) & 0xff);
  outb(0x1f5, (b->sector >> 16) & 0xff);
  outb(0x1f6, 0xe0 | ((b->dev&1)<<4) | ((b->sector>>24)&0x0f));
  if(b->flags & B_DIRTY){
    outb(0x1f7, IDE_CMD_WRITE);
    outsl(0x1f0, b->data, 512/4);
  } else {
    outb(0x1f7, IDE_CMD_READ);
  }
}

/**
 * Interrupt handler.
 */
void
ide_intr(void)
{
  struct buf *b;

  // First queued buffer is the active request.
  spinlock_acquire(&ide_lk);
  if((b = idequeue) == 0){
    spinlock_release(&ide_lk);
    KERN_INFO("spurious IDE interrupt\n");
    return;
  }
  idequeue = b->qnext;

  // Read data if needed.
  if(!(b->flags & B_DIRTY) && ide_wait(1) >= 0)
    insl(0x1f0, b->data, 512/4);
  
  // Wake process waiting for this buf.
  b->flags |= B_VALID;
  b->flags &= ~B_DIRTY;
  thread_wakeup(b);
  
  // Start disk on next buf in queue.
  if(idequeue != 0)
    ide_start(idequeue);

  spinlock_release(&ide_lk);
}

/**
 * Sync buf with disk. 
 * If B_DIRTY is set, write buf to disk, clear B_DIRTY, set B_VALID.
 * Else if B_VALID is not set, read buf from disk, set B_VALID.
 */
void
ide_rw(struct buf *b)
{
  struct buf **pp;
  //KERN_INFO("  !! ide_rw\n");

  if(!(b->flags & B_BUSY))
    KERN_PANIC("ide_rw: buf not busy");
  if((b->flags & (B_VALID|B_DIRTY)) == B_VALID)
    KERN_PANIC("ide_rw: nothing to do");
  if(b->dev != 0 && !havedisk1)
    KERN_PANIC("ide_rw: ide disk 1 not present");

  spinlock_acquire(&ide_lk);  //DOC:acquire-lock

  // Append b to idequeue.
  b->qnext = 0;
  for(pp=&idequeue; *pp; pp=&(*pp)->qnext)  //DOC:insert-queue
    ;
  *pp = b;
  // Start disk if necessary.
  if(idequeue == b)
    ide_start(b);
  
  // Wait for request to finish.
  while((b->flags & (B_VALID|B_DIRTY)) != B_VALID){
    thread_sleep(b, &ide_lk);
    
  }

  spinlock_release(&ide_lk);
}
