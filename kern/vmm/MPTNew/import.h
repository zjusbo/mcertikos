#ifndef _KERN_VMM_MPTNEW_H_
#define _KERN_VMM_MPTNEW_H_

#ifdef _KERN_

void pt_spinlock_acquire(void);
void pt_spinlock_release(void);
unsigned int container_alloc(unsigned int);
unsigned int container_split(unsigned int, unsigned int);
unsigned int map_page(unsigned int, unsigned int, unsigned int, unsigned int);

#endif /* _KERN_ */

#endif /* !_KERN_VMM_MPTNEW_H_ */
