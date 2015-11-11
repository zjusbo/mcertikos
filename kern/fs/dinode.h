#ifndef _KERN_FS_DINODE_H_
#define _KERN_FS_DINODE_H_

#define NDIRECT 12
#define NINDIRECT (BSIZE / sizeof(uint32_t))
#define MAXFILE (NDIRECT + NINDIRECT)

// On-disk inode structure
struct dinode {
  int16_t type;                // File type
  int16_t major;               // Major device number (T_DEV only)
  int16_t minor;               // Minor device number (T_DEV only)
  int16_t nlink;               // Number of links to inode in file system
  uint32_t size;               // Size of file (bytes)
  uint32_t addrs[NDIRECT+1];   // Data block addresses
};

#define I_BUSY 0x1
#define I_VALID 0x2

// Inodes per block.
#define IPB           (BSIZE / sizeof(struct dinode))

// Block containing inode i
#define IBLOCK(i)     ((i) / IPB + 2)

// Bitmap bits per block
#define BPB           (BSIZE*8)

// Block containing bit for block b
#define BBLOCK(b, ninodes) (b/BPB + (ninodes)/IPB + 3)

// File system super block
struct superblock {
  size_t size;           // Size of file system image (blocks)
  uint32_t nblocks;      // Number of data blocks
  uint32_t ninodes;      // Number of inodes.
  uint32_t nlog;         // Number of log blocks
};

#endif /* !_KERN_FS_DINODE_H_ */
