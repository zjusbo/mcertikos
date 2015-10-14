#ifndef _KERN_MM_MALOP_H_
#define _KERN_MM_MALOP_H_

#ifdef _KERN_

unsigned int palloc(void);
void pfree(unsigned int pfree_index);

#endif /* _KERN_ */

#endif /* !_KERN_MM_MALOP_H_ */
