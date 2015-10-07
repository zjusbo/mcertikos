#ifndef _SYS_PREINIT_DEV_TIMER_H_
#define _SYS_PREINIT_DEV_TIMER_H_

#ifdef _KERN_

#define	TIMER_FREQ	1193182

/**
 * the frequency of timer irq
 */
#define FREQ    100

#define CAL_MS		10
#define CAL_LATCH	(TIMER_FREQ / (1000 / CAL_MS))
#define CAL_PIT_LOOPS	1000

void timer_hw_init(void);

#endif /* _KERN_ */

#endif /* !_SYS_PREINIT_DEV_TIMER_H_ */
