#ifndef _KERN_VMM_MPTINIT_H_
#define _KERN_VMM_MPTINIT_H_

#ifdef _KERN_

void pt_spinlock_init();
void pt_spinlock_acquire();
void pt_spinlock_release();
void pdir_init_kern(unsigned int);
void set_pdir_base(unsigned int);
void enable_paging(void);


#endif /* _KERN_ */

#endif /* !_KERN_VMM_MPTINIT_H_ */
