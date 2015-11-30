//
// This file, together with dir.c and path.c, contains the low-level file system
// manipulation routines.  The (higher-level) system call implementations are in
// sysfile.c.
//

#include <kern/lib/types.h>
#include <kern/lib/debug.h>
#include <kern/lib/string.h>
#include <kern/lib/spinlock.h>
#include "bufcache.h"
#include "log.h"
#include "block.h"
#include "inode.h"

static void inode_trunc(struct inode*);

struct {
  spinlock_t lock;
  struct inode inode[NINODE];
} inode_cache;

void
inode_init(void)
{
  spinlock_init(&inode_cache.lock);
}

struct inode* inode_get(uint32_t dev, uint32_t inum);

/**
 * Allocate a new inode with the given type on device dev.
 * A free inode has a type of zero.
 */
struct inode*
inode_alloc(uint32_t dev, short type)
{
  int inum;
  struct buf *bp;
  struct dinode *dip;
  struct superblock sb;

  read_superblock(dev, &sb);

  for(inum = 1; inum < sb.ninodes; inum++){
    bp = bufcache_read(dev, IBLOCK(inum));
    dip = (struct dinode*)bp->data + inum%IPB;
    if(dip->type == 0){  // a free inode
      memset(dip, 0, sizeof(*dip));
      dip->type = type;
      log_write(bp);   // mark it allocated on the disk
      bufcache_release(bp);
      return inode_get(dev, inum);
    }
    bufcache_release(bp);
  }
  KERN_PANIC("inode_alloc: no inodes");
}

/**
 * Copy a modified in-memory inode to disk.
 */
void
inode_update(struct inode *ip)
{
  struct buf *bp;
  struct dinode *dip;

  bp = bufcache_read(ip->dev, IBLOCK(ip->inum));
  dip = (struct dinode*)bp->data + ip->inum%IPB;
  dip->type = ip->type;
  dip->major = ip->major;
  dip->minor = ip->minor;
  dip->nlink = ip->nlink;
  dip->size = ip->size;
  memmove(dip->addrs, ip->addrs, sizeof(ip->addrs));
  log_write(bp);
  bufcache_release(bp);
}

/**
 * Find the inode with number inum on device dev
 * and return the in-memory copy. Does not lock
 * the inode and does not read it from disk.
 */
struct inode*
inode_get(uint32_t dev, uint32_t inum)
{
  struct inode *ip, *empty;

  spinlock_acquire(&inode_cache.lock);

  // Is the inode already cached?
  empty = 0;
  for(ip = &inode_cache.inode[0]; ip < &inode_cache.inode[NINODE]; ip++){
    if(ip->ref > 0 && ip->dev == dev && ip->inum == inum){
      ip->ref++;
      spinlock_release(&inode_cache.lock);
      return ip;
    }
    if(empty == 0 && ip->ref == 0)    // Remember empty slot.
      empty = ip;
  }

  // Recycle an inode cache entry.
  if(empty == 0)
    KERN_PANIC("inode_get: no inodes");

  ip = empty;
  ip->dev = dev;
  ip->inum = inum;
  ip->ref = 1;
  ip->flags = 0;
  spinlock_release(&inode_cache.lock);

  return ip;
}

/**
 * Increment reference count for ip.
 * Returns ip to enable ip = inode_dup(ip1) idiom.
 */
struct inode*
inode_dup(struct inode *ip)
{
  spinlock_acquire(&inode_cache.lock);
  ip->ref++;
  spinlock_release(&inode_cache.lock);
  return ip;
}

/**
 * Lock the given inode.
 * Reads the inode from disk if necessary.
 */
void
inode_lock(struct inode *ip)
{
  struct buf *bp;
  struct dinode *dip;

  if(ip == 0 || ip->ref < 1)
    KERN_PANIC("inode_lock");

  spinlock_acquire(&inode_cache.lock);
  while(ip->flags & I_BUSY)
    thread_sleep(ip, &inode_cache.lock);
  ip->flags |= I_BUSY;
  spinlock_release(&inode_cache.lock);
  if(!(ip->flags & I_VALID)){
    bp = bufcache_read(ip->dev, IBLOCK(ip->inum));
    dip = (struct dinode*)bp->data + ip->inum % IPB;
    ip->type = dip->type;
    ip->major = dip->major;
    ip->minor = dip->minor;
    ip->nlink = dip->nlink;
    ip->size = dip->size;

    memmove(ip->addrs, dip->addrs, sizeof(ip->addrs));
    bufcache_release(bp);
    ip->flags |= I_VALID;
    if(ip->type == 0)
      KERN_PANIC("inode_lock: no type");
  }

}

/**
 * Unlock the given inode.
 */
void
inode_unlock(struct inode *ip)
{
  if(ip == 0 || !(ip->flags & I_BUSY) || ip->ref < 1)
    KERN_PANIC("inode_unlock");

  spinlock_acquire(&inode_cache.lock);
  ip->flags &= ~I_BUSY;
  thread_wakeup(ip);
  spinlock_release(&inode_cache.lock);
}

/**
 * Drop a reference to an in-memory inode.
 * If that was the last reference, the inode cache entry can
 * be recycled.
 * If that was the last reference and the inode has no links
 * to it, free the inode (and its content) on disk.
 */
void
inode_put(struct inode *ip)
{
  spinlock_acquire(&inode_cache.lock);
  if(ip->ref == 1 && (ip->flags & I_VALID) && ip->nlink == 0){
    // inode has no links: truncate and free inode.
    if(ip->flags & I_BUSY)
      KERN_PANIC("inode_put busy");
    ip->flags |= I_BUSY;
    spinlock_release(&inode_cache.lock);
    inode_trunc(ip);
    ip->type = 0;
    inode_update(ip);
    spinlock_acquire(&inode_cache.lock);
    ip->flags = 0;
    thread_wakeup(ip);
  }
  ip->ref--;
  spinlock_release(&inode_cache.lock);
}

/**
 * Common idiom: unlock, then put.
 */
void
inode_unlockput(struct inode *ip)
{
  inode_unlock(ip);
  inode_put(ip);
}

/**
 * Inode content
 *
 * The content (data) associated with each inode is stored
 * in blocks on the disk. The first NDIRECT block numbers
 * are listed in ip->addrs[].  The next NINDIRECT blocks are 
 * listed in block ip->addrs[NDIRECT].
 */

/**
 * Return the disk block address of the nth block in inode ip.
 * If there is no such block, bmap allocates one.
 */
static uint32_t
bmap(struct inode *ip, uint32_t bn)
{
  uint32_t addr, *a;
  struct buf *bp;

  if(bn < NDIRECT){
    if((addr = ip->addrs[bn]) == 0)
      ip->addrs[bn] = addr = block_alloc(ip->dev);
    return addr;
  }
  bn -= NDIRECT;

  if(bn < NINDIRECT){
    // Load indirect block, allocating if necessary.
    if((addr = ip->addrs[NDIRECT]) == 0)
      ip->addrs[NDIRECT] = addr = block_alloc(ip->dev);
    bp = bufcache_read(ip->dev, addr);
    a = (uint32_t*)bp->data;
    if((addr = a[bn]) == 0){
      a[bn] = addr = block_alloc(ip->dev);
      log_write(bp);
    }
    bufcache_release(bp);
    return addr;
  }

  KERN_PANIC("bmap: out of range");
}

/**
 * Truncate inode (discard contents).
 * Only called when the inode has no links
 * to it (no directory entries referring to it)
 * and has no in-memory reference to it (is
 * not an open file or current directory).
 */
static void
inode_trunc(struct inode *ip)
{
  int i, j;
  struct buf *bp;
  uint32_t *a;

  for(i = 0; i < NDIRECT; i++){
    if(ip->addrs[i]){
      block_free(ip->dev, ip->addrs[i]);
      ip->addrs[i] = 0;
    }
  }
  
  if(ip->addrs[NDIRECT]){
    bp = bufcache_read(ip->dev, ip->addrs[NDIRECT]);
    a = (uint32_t*)bp->data;
    for(j = 0; j < NINDIRECT; j++){
      if(a[j])
        block_free(ip->dev, a[j]);
    }
    bufcache_release(bp);
    block_free(ip->dev, ip->addrs[NDIRECT]);
    ip->addrs[NDIRECT] = 0;
  }

  ip->size = 0;
  inode_update(ip);
}

/**
 * Copy stat information from inode.
 */
void
inode_stat(struct inode *ip, struct file_stat *st)
{
  st->dev = ip->dev;
  st->ino = ip->inum;
  st->type = ip->type;
  st->nlink = ip->nlink;
  st->size = ip->size;
}

/**
 * Read data from inode.
 */
int
inode_read(struct inode *ip, char *dst, uint32_t off, uint32_t n)
{
  uint32_t tot, m;
  struct buf *bp;

  if(ip->type == T_DEV){
    if(ip->major < 0 || ip->major >= NDEV || !devsw[ip->major].read)
      return -1;
    return devsw[ip->major].read(ip, dst, n);
  }

  if(off > ip->size || off + n < off)
    return -1;
  if(off + n > ip->size)
    n = ip->size - off;

  for(tot=0; tot<n; tot+=m, off+=m, dst+=m){
    bp = bufcache_read(ip->dev, bmap(ip, off/BSIZE));
    m = min(n - tot, BSIZE - off%BSIZE);
    memmove(dst, bp->data + off%BSIZE, m);
    bufcache_release(bp);
  }
  return n;
}

/**
 * Write data to inode.
 */
int
inode_write(struct inode *ip, char *src, uint32_t off, uint32_t n)
{
  uint32_t tot, m;
  struct buf *bp;

  if(ip->type == T_DEV){
    if(ip->major < 0 || ip->major >= NDEV || !devsw[ip->major].write)
      return -1;
    return devsw[ip->major].write(ip, src, n);
  }

  if(off > ip->size || off + n < off)
    return -1;
  if(off + n > MAXFILE*BSIZE)
    return -1;

  for(tot=0; tot<n; tot+=m, off+=m, src+=m){
    bp = bufcache_read(ip->dev, bmap(ip, off/BSIZE));
    m = min(n - tot, BSIZE - off%BSIZE);
    memmove(bp->data + off%BSIZE, src, m);
    log_write(bp);
    bufcache_release(bp);
  }

  if(n > 0 && off > ip->size){
    ip->size = off;
    inode_update(ip);
  }
  return n;
}
