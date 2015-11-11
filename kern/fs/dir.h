// Directory is a file containing a sequence of dirent structures.
#define DIRSIZ 14

struct dirent {
  uint16_t inum;
  char name[DIRSIZ];
};

int dir_namecmp(const char *s, const char *t);

/**
 * Look for a directory entry in a directory.
 * If found, set *poff to byte offset of entry.
 */
struct inode* dir_lookup(struct inode *dp, char *name, uint32_t *poff);

/**
 * Write a new directory entry (name, inum) into the directory dp.
 */
int dir_link(struct inode *dp, char *name, uint32_t inum);
