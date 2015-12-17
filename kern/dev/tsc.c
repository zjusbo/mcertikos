#include <lib/types.h>

#include <lib/debug.h>
#include <lib/x86.h>

#include "timer.h"
#include "tsc.h"

volatile uint64_t tsc_per_ms;

/*
 * XXX: From Linux 3.2.6: arch/x86/kernel/tsc.c: pit_calibrate_tsc()
 */
static uint64_t
tsc_calibrate(uint32_t latch, uint32_t ms, int loopmin)
{
	uint64_t tsc, t1, t2, delta, tscmin, tscmax;;
	int pitcnt;

	/* Set the Gate high, disable speaker */
	outb(0x61, (inb(0x61) & ~0x02) | 0x01);

	/*
	 * Setup CTC channel 2 for mode 0, (interrupt on terminal
	 * count mode), binary count. Set the latch register to 50ms
	 * (LSB then MSB) to begin countdown.
	 */
	outb(0x43, 0xb0);
	outb(0x42, latch & 0xff);
	outb(0x42, latch >> 8);

	tsc = t1 = t2 = rdtsc();

	pitcnt = 0;
	tscmax = 0;
	tscmin = ~(uint64_t) 0x0;
	while ((inb(0x61) & 0x20) == 0) {
		t2 = rdtsc();
		delta = t2 - tsc;
		tsc = t2;
		if (delta < tscmin)
			tscmin = delta;
		if (delta > tscmax)
			tscmax = delta;
		pitcnt++;
	}

	/*
	 * Sanity checks:
	 *
	 * If we were not able to read the PIT more than loopmin
	 * times, then we have been hit by a massive SMI
	 *
	 * If the maximum is 10 times larger than the minimum,
	 * then we got hit by an SMI as well.
	 */
	KERN_DEBUG("pitcnt=%u, tscmin=%llu, tscmax=%llu\n",
		   pitcnt, tscmin, tscmax);
	if (pitcnt < loopmin || tscmax > 10 * tscmin)
		return ~(uint64_t) 0x0;

	/* Calculate the PIT value */
	delta = t2 - t1;
	return delta / ms;
}

int
tsc_init(void)
{
	uint64_t ret;
	int i;

	timer_hw_init();

	tsc_per_ms = 0;

	/*
	 * XXX: If TSC calibration fails frequently, try to increase the
	 *      upperbound of loop condition, e.g. alternating 3 to 10.
	 */
	for (i = 0; i < 10; i++) {
		ret = tsc_calibrate(CAL_LATCH, CAL_MS, CAL_PIT_LOOPS);
		if (ret != ~(uint64_t) 0x0)
			break;
		KERN_DEBUG("[%d] Retry to calibrate TSC.\n", i+1);
	}

	if (ret == ~(uint64_t) 0x0) {
		KERN_DEBUG("TSC calibration failed.\n");
		KERN_DEBUG("Assume TSC freq = 1 GHz.\n");
		tsc_per_ms = 1000000;

		timer_hw_init();
		return 1;
	} else {
		tsc_per_ms = ret;
		KERN_DEBUG("TSC freq = %llu Hz.\n", tsc_per_ms*1000);

		timer_hw_init();
		return 0;
	}
}

/*
 * Wait for ms millisecond.
 */
void
delay(uint32_t ms)
{
	volatile uint64_t ticks = tsc_per_ms * ms;
	volatile uint64_t start = rdtsc();
	while (rdtsc() < start + ticks);
}

/*
 * Wait for us microsecond.
 */
void
udelay(uint32_t us)
{
    volatile uint64_t ticks = tsc_per_ms / 1000 * us;
    volatile uint64_t start = rdtsc();
    while (rdtsc() < start + ticks);
}
