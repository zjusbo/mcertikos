#include <kern/lib/types.h>
#include <kern/lib/debug.h>
#include <kern/lib/spinlock.h>
#include "bufcache.h"
#include "dinode.h"

// Read the super block.
void read_superblock(int dev, struct superblock *sb)
{
  struct buf *bp;
  
  bp = bufcache_read(dev, 1);        // Block 1 is super block.
  memmove(sb, bp->data, sizeof(*sb));
  bufcache_release(bp);
}

// Zero a block.
void
block_zero(uint32_t dev, uint32_t bno)
{
  struct buf *bp;
  
  bp = bufcache_read(dev, bno);
  memset(bp->data, 0, BSIZE);
  log_write(bp);
  bufcache_release(bp);
}

// Allocate a zeroed disk block.
uint32_t
block_alloc(uint32_t dev)
{
  int b, bi, m;
  struct buf *bp;
  struct superblock sb;

  bp = 0;
  read_superblock(dev, &sb);
  for(b = 0; b < sb.size; b += BPB){
    bp = bufcache_read(dev, BBLOCK(b, sb.ninodes));
    for(bi = 0; bi < BPB && b + bi < sb.size; bi++){
      m = 1 << (bi % 8);
      if((bp->data[bi/8] & m) == 0){  // Is block free?
        bp->data[bi/8] |= m;  // Mark block in use.
        log_write(bp);
        bufcache_release(bp);
        block_zero(dev, b + bi);
        return b + bi;
      }
    }
    bufcache_release(bp);
  }
 ("balloc: out of blocks");
}

// Free a disk block.
void
block_free(uint32_t dev, uint32_t b)
{
  struct buf *bp;
  struct superblock sb;
  int bi, m;

  read_superblock(dev, &sb);
  bp = bufcache_read(dev, BBLOCK(b, sb.ninodes));
  bi = b % BPB;
  m = 1 << (bi % 8);
  if((bp->data[bi/8] & m) == 0)
    KERN_PANIC("freeing free block");
  bp->data[bi/8] &= ~m;
  log_write(bp);
  bufcache_release(bp);
}
