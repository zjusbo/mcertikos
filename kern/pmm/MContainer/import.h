#ifndef _KERN_MM_MCONTAINER_H_
#define _KERN_MM_MCONTAINER_H_

#ifdef _KERN_

unsigned int at_is_norm(unsigned int);
unsigned int at_is_allocated(unsigned int);
void pmem_init(unsigned int);
unsigned int palloc(void);

#endif /* _KERN_ */

#endif /* !_KERN_MM_MCONTAINER_H_ */
