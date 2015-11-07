#ifndef _KERN_LIB_BOUNDEDBUFF
#define _KERN_LIB_BOUNDEDBUFF

#define MAX_BUFFSIZE 10
#include <lib/lock.h>
#include <lib/cv.h>


void bbf_init(void);
int bbf_get(void);
void bbf_put(int item);

int bbf_get_front_idx(void);
int bbf_get_tail_idx(void);
int bbf_get_size(void);


#endif
