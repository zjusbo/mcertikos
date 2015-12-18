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
  uint32_t de_size;
  struct inode * p_inode;
  if(dp->type != T_DIR)
    KERN_PANIC("dir_lookup not DIR");

  //TODO
  de_size = sizeof(de);
  for(off = 0; off < dp->size; off += de_size){
    if(inode_read(dp, (char *)&de, off, de_size)!= de_size){
        // size is not legal
        KERN_PANIC("wrong in dir_lookup"); 
    }
    if(de.inum == 0){
        // free entry
        continue;
    }  
    if(!dir_namecmp(de.name, name)){
        // target directory found
        if(poff != 0){
          *poff = off;
        }
        inum = de.inum;
        p_inode = inode_get(dp->dev, inum);
        return p_inode;
    }
  }
  return 0;
}

// Write a new directory entry (name, inum) into the directory dp.
int
dir_link(struct inode *dp, char *name, uint32_t inum)
{
  uint32_t poff;
  uint32_t off;
  struct dirent de;
  struct inode* pinode;
  uint32_t entry_size;
  // TODO: Check that name is not present.
  entry_size = sizeof(de);
  pinode = dir_lookup(dp, name, &poff);
  if(pinode != 0 ){
    inode_put(pinode);
    return -1;
  }
  // TODO: Look for an empty dirent.
  //KERN_INFO("dp->size = %d, entry_size = %d\n", dp->size, entry_size);
  for(off = 0; off < dp->size; off += entry_size){
    if(inode_read(dp, (char*)&de, off, entry_size) != entry_size){
       KERN_PANIC("entry size read is not correct, dir_link");
    }
    if(de.inum == 0){
      // free entry found
      break;
    }
  }
  de.inum = inum;
  strncpy(de.name, name, DIRSIZ);
  if(inode_write(dp, (char*)&de, off, entry_size) != entry_size){
    KERN_PANIC("entry size write is not corrent, dir_link");
  }
   
  // free entry is not found
  return 0;
}
