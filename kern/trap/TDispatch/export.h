#ifndef _KERN_TRAP_TDISPATCH_H_
#define _KERN_TRAP_TDISPATCH_H_

#ifdef _KERN_

#include <lib/trap.h>

void syscall_dispatch(tf_t);

#endif /* _KERN_ */

#endif /* !_KERN_TRAP_TDISPATCH_H_ */

