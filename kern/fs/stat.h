#ifndef _KERN_FS_STAT_H_
#define _KERN_FS_STAT_H_

#define T_DIR  1   // Directory
#define T_FILE 2   // File
#define T_DEV  3   // Device

//#include <lib/types.h>
struct file_stat {
  int16_t type;    // Type of file
  uint32_t dev;    // File system's disk device
  uint32_t ino;    // Inode number
  uint16_t nlink;  // Number of links to file
  size_t size;     // Size of file in bytes
};

#endif /* !_KERN_FS_STAT_H_ */
