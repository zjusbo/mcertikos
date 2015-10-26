#ifndef _KERN_MM_MALINIT_H_
#define _KERN_MM_MALINIT_H_

#ifdef _KERN_

void mem_spinlock_init(void);
void mem_lock(void);
void mem_unlock(void);

unsigned int get_nps(void);
void set_nps(unsigned int);

unsigned int at_is_norm(unsigned int);
void at_set_perm(unsigned int, unsigned int);

unsigned int at_is_allocated(unsigned int);
void at_set_allocated(unsigned int, unsigned int);

#endif /* _KERN_ */

#endif /* !_KERN_MM_MALINIT_ */

