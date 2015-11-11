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

#ifndef _KERN_FS_BUFCACHE_H_
#define _KERN_FS_BUFCACHE_H_

#include <kern/lib/buf.h>
#include "params.h"

void bufcache_init(void);

/**
 * Return a B_BUSY buf with the contents of the indicated disk sector.
 */
struct buf* bufcache_read(uint32_t dev, uint32_t sector);

/**
 * Write b's contents to disk.  Must be B_BUSY.
 */
void bufcache_write(struct buf *b);

/**
 * Release a B_BUSY buffer.
 * Move to the head of the MRU list.
 */
void bufcache_release(struct buf *b);

#endif /* !_KERN_FS_BUFCACHE_H_ */
