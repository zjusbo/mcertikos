#ifndef _KERN_MM_MCONTAINER_H_
#define _KERN_MM_MCONTAINER_H_

#ifdef _KERN_

void container_init(unsigned int mbi_addr);
unsigned int container_get_parent(unsigned int);
unsigned int container_get_nchildren(unsigned int);
unsigned int container_get_quota(unsigned int);
unsigned int container_get_usage(unsigned int);
unsigned int container_can_consume(unsigned int, unsigned int);
unsigned int container_split(unsigned int, unsigned int);
unsigned int container_alloc(unsigned int);
void container_free(unsigned int, unsigned int);

#endif /* _KERN_ */

#endif /* !_KERN_MM_MCONTAINER_H_ */
