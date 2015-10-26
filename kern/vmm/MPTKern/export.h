#ifndef _KERN_VMM_MPTKERN_H_
#define _KERN_VMM_MPTKERN_H_

#ifdef _KERN_

void pdir_init_kern(unsigned int mbi_addr);
unsigned int map_page(unsigned int, unsigned int, unsigned int, unsigned int);
unsigned int unmap_page(unsigned int, unsigned int);

#endif /* _KERN_ */

#endif /* !_KERN_VMM_MPTKERN_H_ */

