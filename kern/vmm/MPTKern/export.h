#ifndef _KERN_MM_MPTKERN_H_
#define _KERN_MM_MPTKERN_H_

void pdir_init_kern(unsigned int mbi_addr);
unsigned int map_page(unsigned int, unsigned int, unsigned int, unsigned int);
unsigned int unmap_page(unsigned int, unsigned int);

#endif

