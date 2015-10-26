#include <lib/debug.h>
#include <lib/types.h>
#include <lib/x86.h>

#include <dev/intr.h>

#include <dev/timer.h>
#include "lapic.h"

volatile lapic_t *lapic;

/*
 * Read the index'th local APIC register.
 */
static uint32_t
lapic_read(int index)
{
	return lapic[index];
}

/* Write value to the index'th local APIC register. */
static void
lapic_write(int index, int value)
{
	lapic[index] = value;
	/* wait for the finish of writing */
	lapic[LAPIC_ID];
}

/*
 * Spin for a given number of microseconds.
 * On real hardware would want to tune this dynamically.
 */
static void
microdelay(int us)
{
}

void
lapic_register(uintptr_t lapic_addr)
{
	lapic = (lapic_t *) lapic_addr;
}

static uint32_t
lapic_calibrate_timer(uint32_t latch, uint32_t ms, int loopmin)
{
	uint32_t timer, timer1, timer2, delta, timermin, timermax;;
	int pitcnt;

	lapic_write(LAPIC_TICR, ~(uint32_t) 0x0);

	/* Set the Gate high, disable speaker */
	outb(0x61, (inb(0x61) & ~0x02) | 0x01);

	/*
	 * Setup CTC channel 2* for mode 0, (interrupt on terminal
	 * count mode), binary count. Set the latch register to 50ms
	 * (LSB then MSB) to begin countdown.
	 */
	outb(0x43, 0xb0);
	outb(0x42, latch & 0xff);
	outb(0x42, latch >> 8);

	timer = timer1 = timer2 = lapic_read(LAPIC_TCCR);

	pitcnt = 0;
	timermax = 0;
	timermin = ~(uint32_t) 0x0;
	while ((inb(0x61) & 0x20) == 0) {
		timer2 = lapic_read(LAPIC_TCCR);
		delta = timer - timer2;
		timer = timer2;
		if (delta < timermin)
			timermin = delta;
		if (delta > timermax)
			timermax = delta;
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
	if (pitcnt < loopmin || timermax > 10 * timermin)
		return ~(uint32_t) 0x0;

	/* Calculate the PIT value */
	delta = timer1 - timer2;
	/* do_div(delta, ms); */
	return delta/ms;
}

/*
 * Initialize local APIC.
 */
void
lapic_init()
{
	if (!lapic)
		KERN_PANIC("NO LAPIC");

	/* debug("Use local APIC at %x\n", (uintptr_t) lapic); */

	/* enable local APIC */
	lapic_write(LAPIC_SVR, LAPIC_SVR_ENABLE | (T_IRQ0 + IRQ_SPURIOUS));

	/* enable internal timer of local APIC */
	lapic_write(LAPIC_TDCR, LAPIC_TIMER_X1);
	lapic_write(LAPIC_TIMER, LAPIC_TIMER_PERIODIC | (T_IRQ0 + IRQ_TIMER));

	/*
	 * Calibrate the internal timer of LAPIC using TSC.
	 * XXX: TSC should be already calibrated before here.
	 */
	uint32_t lapic_ticks_per_ms;
	int i;
	for (i = 0; i < 5; i++) {
		lapic_ticks_per_ms =
			lapic_calibrate_timer(CAL_LATCH, CAL_MS, CAL_PIT_LOOPS);
		if (lapic_ticks_per_ms != ~(uint32_t) 0x0)
			break;
		KERN_DEBUG("[%d] Retry to calibrate internal timer of LAPIC.\n", i);
	}
	if (lapic_ticks_per_ms == ~(uint32_t) 0x0) {
		KERN_WARN("Failed to calibrate internal timer of LAPIC.\n");
		KERN_DEBUG("Assume LAPIC timer freq = 0.5 GHz.\n");
		lapic_ticks_per_ms = 500000;
	} else
		KERN_DEBUG("LAPIC timer freq = %llu Hz.\n",
			   (uint64_t) lapic_ticks_per_ms * 1000);
	uint32_t ticr = lapic_ticks_per_ms * 1000 / LAPIC_TIMER_INTR_FREQ;
	KERN_DEBUG("Set LAPIC TICR = %x.\n", ticr);
	lapic_write(LAPIC_TICR, ticr);

	/* Disable logical interrupt lines. */
	lapic_write(LAPIC_LINT0, LAPIC_LINT_MASKED);
	lapic_write(LAPIC_LINT1, LAPIC_LINT_MASKED);

	// Disable performance counter overflow interrupts
	// on machines that provide that interrupt entry.
	if (((lapic_read(LAPIC_VER)>>16) & 0xFF) >= 4)
		lapic_write(LAPIC_PCINT, APIC_LVTT_M);

	// Set DFR to flat mode
	lapic_write(LAPIC_DFR, LAPIC_DFR_FLAT_MODE);

	// clear LDR
	lapic_write(LAPIC_LDR, 0x0);

	// Map error interrupt to IRQ_ERROR.
	lapic_write(LAPIC_ERROR, T_LERROR);

	// Clear error status register (requires back-to-back writes).
	lapic_write(LAPIC_ESR, 0);
	lapic_write(LAPIC_ESR, 0);

	// Ack any outstanding interrupts.
	lapic_write(LAPIC_EOI, 0);

	// Send an Init Level De-Assert to synchronise arbitration ID's.
	lapic_write(LAPIC_ICRHI, 0);
	lapic_write(LAPIC_ICRLO,
		    LAPIC_ICRLO_BCAST | LAPIC_ICRLO_INIT | LAPIC_ICRLO_LEVEL);
	while(lapic_read(LAPIC_ICRLO) & LAPIC_ICRLO_DELIVS)
		;

	// Enable interrupts on the APIC (but not on the processor).
	lapic_write(LAPIC_TPR, 0);
}

/*
 * Ackownledge the end of interrupts.
 */
void
lapic_eoi(void)
{
	if (lapic)
		lapic_write(LAPIC_EOI, 0);
}

#define IO_RTC  0x70

/*
 * Start additional processor running bootstrap code at addr.
 * See Appendix B of MultiProcessor Specification.
 */
void
lapic_startcpu(lapicid_t apicid, uintptr_t addr)
{
	int i;
	uint16_t *wrv;

	// "The BSP must initialize CMOS shutdown code to 0AH
	// and the warm reset vector (DWORD based at 40:67) to point at
	// the AP startup code prior to the [universal startup algorithm]."
	outb(IO_RTC, 0xF);  // offset 0xF is shutdown code
	outb(IO_RTC+1, 0x0A);
	wrv = (uint16_t*)(0x40<<4 | 0x67);  // Warm reset vector
	wrv[0] = 0;
	wrv[1] = addr >> 4;

	// "Universal startup algorithm."
	// Send INIT (level-triggered) interrupt to reset other CPU.
	lapic_write(LAPIC_ICRHI, apicid<<24);
	lapic_write(LAPIC_ICRLO,
		    LAPIC_ICRLO_INIT | LAPIC_ICRLO_LEVEL | LAPIC_ICRLO_ASSERT);
	microdelay(200);
	lapic_write(LAPIC_ICRLO, LAPIC_ICRLO_INIT | LAPIC_ICRLO_LEVEL);
	microdelay(100);    // should be 10ms, but too slow in Bochs!

	// Send startup IPI (twice!) to enter bootstrap code.
	// Regular hardware is supposed to only accept a STARTUP
	// when it is in the halted state due to an INIT.  So the second
	// should be ignored, but it is part of the official Intel algorithm.
	// Bochs complains about the second one.  Too bad for Bochs.
	for (i = 0; i < 2; i++) {
		lapic_write(LAPIC_ICRHI, apicid<<24);
		lapic_write(LAPIC_ICRLO, LAPIC_ICRLO_STARTUP | (addr>>12));
		microdelay(200);
	}
}

uint32_t
lapic_read_debug(int index)
{
	return lapic_read(index);
}

/*
 * Send an IPI.
 */
void
lapic_send_ipi(lapicid_t apicid, uint8_t vector,
	       uint32_t deliver_mode, uint32_t shorthand_mode)
{
	KERN_ASSERT(deliver_mode != LAPIC_ICRLO_INIT &&
		    deliver_mode != LAPIC_ICRLO_STARTUP);
	KERN_ASSERT(vector >= T_IPI0);

	while (lapic_read(LAPIC_ICRLO) & LAPIC_ICRLO_DELIVS)
		pause();

	if (shorthand_mode == LAPIC_ICRLO_NOBCAST)
		lapic_write(LAPIC_ICRHI, (apicid << LAPIC_ICRHI_DEST_SHIFT) &
			    LAPIC_ICRHI_DEST_MASK);

	lapic_write(LAPIC_ICRLO, shorthand_mode | /* LAPIC_ICRLO_LEVEL | */
		    deliver_mode | (vector & LAPIC_ICRLO_VECTOR));

	/* KERN_DEBUG("IPI %d has been sent to processor %d.\n", vector, apicid); */
}
