#ifndef _KERN_VMM_MPTOP_H_
#define _KERN_VMM_MPTOP_H_

#ifdef _KERN_

unsigned int get_ptbl_entry_by_va(unsigned int, unsigned int);
unsigned int get_pdir_entry_by_va(unsigned int, unsigned int);
void rmv_ptbl_entry_by_va(unsigned int, unsigned int);
void rmv_pdir_entry_by_va(unsigned int, unsigned int);
void set_ptbl_entry_by_va(unsigned int, unsigned int, unsigned int, unsigned int);
void set_pdir_entry_by_va(unsigned int, unsigned int, unsigned int);
void idptbl_init(unsigned int);

#endif /* _KERN_ */

#endif /* !_KERN_VMM_MPTOP_H_ */

