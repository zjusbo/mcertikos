#ifndef _KERN_MM_MPTCOMM_H_
#define _KERN_MM_MPTCOMM_H_

void pdir_init(unsigned int mbi_addr);
unsigned int alloc_ptbl(unsigned int, unsigned int);
void free_ptbl(unsigned int, unsigned int);

#endif

