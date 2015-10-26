#ifndef _KERN_VMM_MPTCOMM_H_
#define _KERN_VMM_MPTCOMM_H_

#ifdef _KERN_

void pdir_init(unsigned int mbi_addr);
unsigned int alloc_ptbl(unsigned int, unsigned int);
void free_ptbl(unsigned int, unsigned int);

#endif /* _KERN_ */

#endif /* !_KERN_VMM_MPTCOMM_H_ */

