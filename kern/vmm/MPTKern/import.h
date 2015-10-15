#ifndef _KERN_MM_MPTKERN_H_
#define _KERN_MM_MPTKERN_H_

#ifdef _KERN_

void pdir_init(unsigned int);
void set_pdir_entry_identity(unsigned int, unsigned int);
unsigned int get_pdir_entry_by_va(unsigned int, unsigned int);
unsigned int alloc_ptbl(unsigned int, unsigned int);
void set_ptbl_entry_by_va(unsigned int, unsigned int, unsigned int, unsigned int);
void rmv_ptbl_entry_by_va(unsigned int, unsigned int);
unsigned int get_ptbl_entry_by_va(unsigned int, unsigned int);

#endif /* _KERN_ */

#endif /* !_KERN_MM_MPTKERN_H_ */
