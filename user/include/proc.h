#ifndef _USER_PROC_H_
#define _USER_PROC_H_

#include <types.h>

pid_t spawn(uintptr_t exe, unsigned int quota);
void  yield(void);

#endif /* !_USER_PROC_H_ */
