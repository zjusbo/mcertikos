#ifndef _KERN_TRAP_TTRAPHANDLER_H_
#define _KERN_TRAP_TTRAPHANDLER_H_

#ifdef _KERN_

#include <lib/trap.h>

void trap (tf_t *);
void exception_handler(tf_t *);
void interrupt_handler(tf_t *);

#endif /* _KERN_ */

#endif /* !_KERN_TRAP_TTRAPHANDLER_H_ */

