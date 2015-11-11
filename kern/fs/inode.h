// Inodes.
//
// An inode describes a single unnamed file.
// The inode disk structure holds metadata: the file's type,
// its size, the number of links referring to it, and the
// list of blocks holding the file's content.
//
// The inodes are laid out sequentially on disk immediately after
// the superblock. Each inode has a number, indicating its
// position on the disk.
//
// The kernel keeps a cache of in-use inodes in memory
// to provide a place for synchronizing access
// to inodes used by multiple processes. The cached
// inodes include book-keeping information that is
// not stored on disk: ip->ref and ip->flags.
//
// An inode and its in-memory represtative go through a
// sequence of states before they can be used by the
// rest of the file system code.
//
// * Allocation: an inode is allocated if its type (on disk)
//   is non-zero. inode_alloc() allocates, inode_put() frees if
//   the link count has fallen to zero.
//
// * Referencing in cache: an entry in the inode cache
//   is free if ip->ref is zero. Otherwise ip->ref tracks
//   the number of in-memory pointers to the entry (open
//   files and current directories). inode_get() to find or
//   create a cache entry and increment its ref, inode_put()
//   to decrement ref.
//
// * Valid: the information (type, size, &c) in an inode
//   cache entry is only correct when the I_VALID bit
//   is set in ip->flags. inode_lock() reads the inode from
//   the disk and sets I_VALID, while inode_put() clears
//   I_VALID if ip->ref has fallen to zero.
//
// * Locked: file system code may only examine and modify
//   the information in an inode and its content if it
//   has first locked the inode. The I_BUSY flag indicates
//   that the inode is locked. inode_lock() sets I_BUSY,
//   while inode_unlock clears it.
//
// Thus a typical sequence is:
//   ip = inode_get(dev, inum)
//   inode_lock(ip)
//   ... examine and modify ip->xxx ...
//   inode_unlock(ip)
//   inode_put(ip)
//
// inode_lock() is separate from inode_get() so that system calls can
// get a long-term reference to an inode (as for an open file)
// and only lock it for short periods (e.g., in read()).
// The separation also helps avoid deadlock and races during
// pathname lookup. inode_get() increments ip->ref so that the inode
// stays cached and pointers to it remain valid.
//
// Many internal file system functions expect the caller to
// have locked the inodes involved; this lets callers create
// multi-step atomic operations.

#ifndef _KERN_FS_INODE_H_
#define _KERN_FS_INODE_H_

#include "params.h"
#include "stat.h"
#include "dinode.h"

// Lock the given inode.
// in-memory copy of an inode
struct inode {
  uint32_t dev;           // Device number
  uint32_t inum;          // Inode number
  int ref;                // Reference count
  int32_t flags;          // I_BUSY, I_VALID

  int16_t type;           // copy of disk inode
  int16_t major;
  int16_t minor;
  int16_t nlink;
  uint32_t size;
  uint32_t addrs[NDIRECT+1];
};

// table mapping major device number to
// device functions
struct devsw {
  int (*read)(struct inode*, char*, int);
  int (*write)(struct inode*, char*, int);
};

struct devsw *devsw;

void inode_init(void);

// Allocate a new inode with the given type on device dev.
// A free inode has a type of zero.
struct inode* inode_alloc(uint32_t dev, short type);

// Copy a modified in-memory inode to disk.
void inode_update(struct inode *ip);

// Find the inode with number inum on device dev
// and return the in-memory copy. Does not lock
// the inode and does not read it from disk.
struct inode* inode_get(uint32_t dev, uint32_t inum);

// Increment reference count for ip.
// Returns ip to enable ip = idup(ip1) idiom.
struct inode* inode_dup(struct inode *ip);

// Reads the inode from disk if necessary.
void inode_lock(struct inode *ip);

// Unlock the given inode.
void inode_unlock(struct inode *ip);

// Drop a reference to an in-memory inode.
// If that was the last reference, the inode cache entry can
// be recycled.
// If that was the last reference and the inode has no links
// to it, free the inode (and its content) on disk.
void inode_put(struct inode *ip);

// Common idiom: unlock, then put.
void inode_unlockput(struct inode *ip);

/**
 * Inode content
 *
 * The content (data) associated with each inode is stored
 * in blocks on the disk. The first NDIRECT block numbers
 * are listed in ip->addrs[].  The next NINDIRECT blocks are 
 * listed in block ip->addrs[NDIRECT].
 */

/** Copy stat information from inode. */
void inode_stat(struct inode *ip, struct file_stat *st);

/** Read data from inode. */
int inode_read(struct inode *ip, char *dst, uint32_t off, uint32_t n);

/** Write data to inode. */
int inode_write(struct inode *ip, char *src, uint32_t off, uint32_t n);

#endif /* !_KERN_FS_INODE_H_ */
