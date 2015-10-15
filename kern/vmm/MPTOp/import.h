#ifndef _KERN_MM_MPTOP_H_
#define _KERN_MM_MPTOP_H_

#ifdef _KERN_

void container_init(unsigned int);
void set_pdir_base(unsigned int);
unsigned int get_pdir_entry(unsigned int, unsigned int);
void set_pdir_entry_identity(unsigned int, unsigned int);
void rmv_pdir_entry(unsigned int, unsigned int);
void set_pdir_entry(unsigned int, unsigned int, unsigned int);
unsigned int get_ptbl_entry(unsigned int, unsigned int, unsigned int);
void set_ptbl_entry(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
void rmv_ptbl_entry(unsigned int, unsigned int, unsigned int);
void set_ptbl_entry_identity(unsigned int, unsigned int, unsigned int);

#endif /* _KERN_ */

#endif /* !_KERN_MM_MPTOP_H_ */
