#include <kern/lib/types.h>
#include <kern/lib/debug.h>
#include <kern/lib/string.h>
#include "inode.h"
#include "dir.h"

// Directories

int
dir_namecmp(const char *s, const char *t)
{
  return strncmp(s, t, DIRSIZ);
}

/**
 * Look for a directory entry in a directory.
 * If found, set *poff to byte offset of entry.
 */
struct inode*
dir_lookup(struct inode *dp, char *name, uint32_t *poff)
{
  uint32_t off, inum;
  struct dirent de;

  if(dp->type != T_DIR)
    KERN_PANIC("dir_lookup not DIR");

  //TODO

  return 0;
}

// Write a new directory entry (name, inum) into the directory dp.
int
dir_link(struct inode *dp, char *name, uint32_t inum)
{
  // TODO: Check that name is not present.

  // TODO: Look for an empty dirent.
  
  return 0;
}
