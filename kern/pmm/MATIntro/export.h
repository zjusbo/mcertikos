#ifndef _KERN_MM_MALINIT_H_
#define _KERN_MM_MALINIT_H_

unsigned int get_nps(void);
void set_nps(unsigned int);

unsigned int at_is_norm(unsigned int);
void at_set_perm(unsigned int, unsigned int);

unsigned int at_is_allocated(unsigned int);
void at_set_allocated(unsigned int, unsigned int);


#endif

