#ifndef _KERN_MM_MPTOP_H_
#define _KERN_MM_MPTOP_H_

unsigned int get_pdir_entry_by_va(unsigned int, unsigned int);
void set_pdir_entry_by_va(unsigned int, unsigned int, unsigned int);
void rmv_pdir_entry_by_va(unsigned int, unsigned int);
unsigned int get_ptbl_entry_by_va(unsigned int, unsigned int);
void set_ptbl_entry_by_va(unsigned int, unsigned int, unsigned int, unsigned int);
void rmv_ptbl_entry_by_va(unsigned int, unsigned int);
void idptbl_init(unsigned int);

#endif

