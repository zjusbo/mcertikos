#ifndef _NEW_MM_MPTNEW_H_
#define _NEW_MM_MPTNEW_H_

#ifdef _NEW_

unsigned int container_alloc(unsigned int);
unsigned int container_split(unsigned int, unsigned int);
unsigned int map_page(unsigned int, unsigned int, unsigned int, unsigned int);

#endif /* _NEW_ */

#endif /* !_NEW_MM_MPTNEW_H_ */
