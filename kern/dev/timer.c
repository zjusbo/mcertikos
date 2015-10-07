/*
 * The 8253 Programmable Interval Timer (PIT),
 * which generates interrupts on IRQ 0.
 */

#include <lib/types.h>
#include <lib/x86.h>

#include "timer.h"

/* I/O addresses of the timer */
#define	PIT_CHANNEL0	0x040		/* 8253 Timer #0 */
#define PIT_CHANNEL1    0x041       /* 8253 Timer #1 */
#define PIT_CHANNEL2    0x042       /* 8253 Timer #2 */
#define PIT_CONTROL     0x043       /* 8253 Timer control register */

/*
 * Frequency of all three count-down timers; (TIMER_FREQ/freq) is the
 * appropriate count to generate a frequency of freq hz.
 */
#define TIMER_DIV(x)	((TIMER_FREQ+(x)/2)/(x))
#define LATCH   TIMER_DIV(FREQ)


#if (LATCH > 0xFFFFu)
#error "The frequency of timer irq is too small. Should be larger than 18 Hz."
#endif

#if (LATCH == 0x0u)
#error "The frequency of timer irq is too big. Should be less than 1.18 MHz."
#endif


/*
 * Macros for specifying values to be written into a mode register.
 */
#define		TIMER_SEL0	0x00	/* select counter 0 */
#define		TIMER_RATEGEN	0x04	/* mode 2, rate generator */
#define		TIMER_16BIT	0x30	/* r/w counter 16 bits, LSB first */

// Initialize the programmable interval timer.

void
timer_hw_init(void)
{
	outb(PIT_CONTROL, TIMER_SEL0 | TIMER_RATEGEN | TIMER_16BIT);
	outb(PIT_CHANNEL0, LOW8(LATCH));
	outb(PIT_CHANNEL0, HIGH8(LATCH));
}
