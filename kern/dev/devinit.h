#ifndef _KERN_DEV_DEVINIT_H_
#define _KERN_DEV_DEVINIT_H_

#ifdef _KERN_

#include <lib/types.h>

void devinit(uintptr_t);
void devinit_ap(void);

#endif /* _KERN_ */

#endif /* !_KERN_DEV_DEVINIT_H_ */
