#ifndef _KERN_LIB_THREAD_H_
#define _KERN_LIB_THREAD_H_

#ifdef _KERN_

#define SCHED_SLICE 5

typedef enum {TSTATE_READY=0, TSTATE_RUN, TSTATE_SLEEP, TSTATE_DEAD} t_state;

#endif /* _KERN_ */

#endif /* !_KERN_LIB_THREAD_H_ */
