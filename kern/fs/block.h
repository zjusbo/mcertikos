// Blocks. 
//
// Block 0 is unused.
// Block 1 is super block.
// Blocks 2 through sb.ninodes/IPB hold inodes.
// Then free bitmap blocks holding sb.size bits.
// Then sb.nblocks data blocks.
// Then sb.nlog log blocks.

#ifndef _KERN_FS_BLOCKS_H_
#define _KERN_FS_BLOCKS_H_

#include "dinode.h"

// Read the super block into sb.
void read_superblock(int dev, struct superblock *sb);

// Zero a block.
void block_zero(uint32_t dev, uint32_t bno);

// Allocate a zeroed disk block.
uint32_t block_alloc(uint32_t dev);

// Free a disk block.
void block_free(uint32_t dev, uint32_t b);

#endif /* !_KERN_FS_BLOCKS_H_ */
