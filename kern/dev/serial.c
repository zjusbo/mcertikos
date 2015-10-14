/*
 * Serial (COM) port I/O device driver.
 *
 * Copyright (c) 2010 Yale University.
 * Copyright (c) 1993, 1994, 1995 Charles Hannum.
 * Copyright (c) 1990 The Regents of the University of California.
 * See section "BSD License" in the file LICENSES for licensing terms.
 *
 * This code is derived from the NetBSD pcons driver, and in turn derived
 * from software contributed to Berkeley by William Jolitz and Don Ahn.
 * Adapted for PIOS by Bryan Ford at Yale University.
 */

#include <lib/types.h>
#include <lib/x86.h>

#include "console.h"
#include "serial.h"

#define COM1		0x3F8
#define COM2		0x2F8
#define COM3		0x3E8
#define COM4		0x2E8

#define COM_RX		0	// In:	Receive buffer (DLAB=0)
#define COM_TX		0	// Out: Transmit buffer (DLAB=0)
#define COM_DLL		0	// Out: Divisor Latch Low (DLAB=1)
#define COM_DLM		1	// Out: Divisor Latch High (DLAB=1)
#define COM_IER		1	// Out: Interrupt Enable Register
#define COM_IER_RDI	0x01	//   Enable receiver data interrupt
#define COM_IIR		2	// In:	Interrupt ID Register
#define COM_FCR		2	// Out: FIFO Control Register
#define COM_LCR		3	// Out: Line Control Register
#define	COM_LCR_DLAB	0x80	//   Divisor latch access bit
#define	COM_LCR_WLEN8	0x03	//   Wordlength: 8 bits
#define COM_MCR		4	// Out: Modem Control Register
#define	COM_MCR_RTS	0x02	// RTS complement
#define	COM_MCR_DTR	0x01	// DTR complement
#define	COM_MCR_OUT2	0x08	// Out2 complement
#define COM_LSR		5	// In:	Line Status Register
#define COM_LSR_DATA	0x01	//   Data available
#define COM_LSR_TXRDY	0x20	//   Transmit buffer avail
#define COM_LSR_TSRE	0x40	//   Transmitter off
#define COM_MSR		6	// In: Modem Status Register
#define COM_SRR		7	// In: Shadow Receive Register

bool serial_exists;

// Stupid I/O delay routine necessitated by historical PC design flaws
static void
delay(void)
{
	inb(0x84);
	inb(0x84);
	inb(0x84);
	inb(0x84);
}

static int
serial_proc_data(void)
{
	if (!(inb(COM1+COM_LSR) & COM_LSR_DATA))
		return -1;
	return inb(COM1+COM_RX);
}

void
serial_intr(void)
{
	if (serial_exists)
		cons_intr(serial_proc_data);
}

static int
serial_reformatnewline(int c, int p)
{
	int cr = '\r';
	int nl = '\n';
	/* POSIX requires newline on the serial line to
	 * be a CR-LF pair. Without this, you get a malformed output
	 * with clients like minicom or screen
	 */
	if (c == nl) {
		outb(p, cr);
		outb(p, nl);
		return 1;
	}
	else
		return 0;
}

void
serial_putc(char c)
{
	if (!serial_exists)
		return;

	int i;
	for (i = 0;
	     !(inb(COM1 + COM_LSR) & COM_LSR_TXRDY) && i < 12800;
	     i++)
		delay();

	if (!serial_reformatnewline(c, COM1 + COM_TX))
		outb(COM1 + COM_TX, c);
}

void
serial_init(void)
{
	/* turn off interrupt */
	outb(COM1 + COM_IER, 0);

	/* set DLAB */
	outb(COM1 + COM_LCR, COM_LCR_DLAB);

	/* set baud rate */
	outb(COM1 + COM_DLL, 0x0001 & 0xff);
	outb(COM1 + COM_DLM, 0x0001 >> 8);

	/* Set the line status.  */
	outb(COM1 + COM_LCR, COM_LCR_WLEN8 & ~COM_LCR_DLAB);

	/* Enable the FIFO.  */
	outb(COM1 + COM_FCR, 0xc7);

	/* Turn on DTR, RTS, and OUT2.  */
	outb(COM1 + COM_MCR, 0x0b);

	// Clear any preexisting overrun indications and interrupts
	// Serial COM1 doesn't exist if COM_LSR returns 0xFF
	serial_exists = (inb(COM1+COM_LSR) != 0xFF);
	(void) inb(COM1+COM_IIR);
	(void) inb(COM1+COM_RX);
}

void
serial_intenable(void)
{
	if (serial_exists) {
		outb(COM1+COM_IER, 1);
		//intr_enable(IRQ_SERIAL13);
		serial_intr();
	}
}
