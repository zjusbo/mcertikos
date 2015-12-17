/*
 * Driver code for the 8259A Programmable Interrupt Controller (PIC).
 *
 * Copyright (C) 1997 Massachusetts Institute of Technology
 * See section "MIT License" in the file LICENSES for licensing terms.
 *
 * Derived from the MIT Exokernel and JOS.
 * Adapted for PIOS by Bryan Ford at Yale University.
 */

#include <lib/trap.h>
#include <lib/types.h>
#include <lib/x86.h>

#include "intr.h"
#include "pic.h"

#define MAX_IRQS	16	// Number of IRQs

/* I/O Addresses of the two 8259A programmable interrupt controllers */
#define IO_PIC1		0x20	/* Master (IRQs 0-7) */
#define IO_PIC2		0xA0	/* Slave (IRQs 8-15) */

#define IRQ_SLAVE	2	/* IRQ at which slave connects to master */

#define IO_ELCR1	0x4d0
#define IO_ELCR2	0x4d1

/* i8259 */

// Current IRQ mask.
// Initial IRQ mask has interrupt 2 enabled (for slave 8259A).
static uint16_t irqmask = 0xFFFF & ~(1<<IRQ_SLAVE);
static bool pic_inited = FALSE;

/* Initialize the 8259A interrupt controllers. */
void
pic_init(void)
{
	if (pic_inited == TRUE)		// only do once on bootstrap CPU
		return;
	pic_inited = TRUE;

	/* mask all interrupts */
	outb(IO_PIC1+1, 0xff);
	outb(IO_PIC2+1, 0xff);

	// Set up master (8259A-1)

	// ICW1:  0001g0hi
	//    g:  0 = edge triggering, 1 = level triggering
	//    h:  0 = cascaded PICs, 1 = master only
	//    i:  0 = no ICW4, 1 = ICW4 required
	outb(IO_PIC1, 0x11);

	// ICW2:  Vector offset
	outb(IO_PIC1+1, T_IRQ0);

	// ICW3:  bit mask of IR lines connected to slave PICs (master PIC),
	//        3-bit No of IR line at which slave connects to master(slave PIC).
	outb(IO_PIC1+1, 1<<IRQ_SLAVE);

	// ICW4:  000nbmap
	//    n:  1 = special fully nested mode
	//    b:  1 = buffered mode
	//    m:  0 = slave PIC, 1 = master PIC
	//	  (ignored when b is 0, as the master/slave role
	//	  can be hardwired).
	//    a:  1 = Automatic EOI mode
	//    p:  0 = MCS-80/85 mode, 1 = intel x86 mode
	outb(IO_PIC1+1, 0x1);

	// Set up slave (8259A-2)
	outb(IO_PIC2, 0x11);			// ICW1
	outb(IO_PIC2+1, T_IRQ0 + 8);		// ICW2
	outb(IO_PIC2+1, IRQ_SLAVE);		// ICW3
	// NB Automatic EOI mode doesn't tend to work on the slave.
	// Linux source code says it's "to be investigated".
	outb(IO_PIC2+1, 0x01);			// ICW4

	// OCW3:  0ef01prs
	//   ef:  0x = NOP, 10 = clear specific mask, 11 = set specific mask
	//    p:  0 = no polling, 1 = polling mode
	//   rs:  0x = NOP, 10 = read IRR, 11 = read ISR
	outb(IO_PIC1, 0x68);             /* clear specific mask */
	outb(IO_PIC1, 0x0a);             /* read IRR by default */

	outb(IO_PIC2, 0x68);               /* OCW3 */
	outb(IO_PIC2, 0x0a);               /* OCW3 */

	// mask all interrupts
	outb(IO_PIC1+1, 0xFF);
	outb(IO_PIC2+1, 0xFF);
}

void
pic_setmask(uint16_t mask)
{
	irqmask = mask;
	outb(IO_PIC1+1, (char)mask);
	outb(IO_PIC2+1, (char)(mask >> 8));
}

void
pic_enable(int irq)
{
	pic_setmask(irqmask & ~(1 << irq));
}

void
pic_eoi(void)
{
	// OCW2: rse00xxx
	//   r: rotate
	//   s: specific
	//   e: end-of-interrupt
	// xxx: specific interrupt line
	outb(IO_PIC1, 0x20);
	outb(IO_PIC2, 0x20);
}

void
pic_reset(void)
{
	// mask all interrupts
	outb(IO_PIC1+1, 0x00);
	outb(IO_PIC2+1, 0x00);

	// Set up master (8259A-1)

	// ICW1:  0001g0hi
	//    g:  0 = edge triggering, 1 = level triggering
	//    h:  0 = cascaded PICs, 1 = master only
	//    i:  0 = no ICW4, 1 = ICW4 required
	outb(IO_PIC1, 0x11);

	// ICW2:  Vector offset
	outb(IO_PIC1+1, T_IRQ0);

	// ICW3:  bit mask of IR lines connected to slave PICs (master PIC),
	//        3-bit No of IR line at which slave connects to master(slave PIC).
	outb(IO_PIC1+1, 1<<IRQ_SLAVE);

	// ICW4:  000nbmap
	//    n:  1 = special fully nested mode
	//    b:  1 = buffered mode
	//    m:  0 = slave PIC, 1 = master PIC
	//	  (ignored when b is 0, as the master/slave role
	//	  can be hardwired).
	//    a:  1 = Automatic EOI mode
	//    p:  0 = MCS-80/85 mode, 1 = intel x86 mode
	outb(IO_PIC1+1, 0x3);

	// Set up slave (8259A-2)
	outb(IO_PIC2, 0x11);			// ICW1
	outb(IO_PIC2+1, T_IRQ0 + 8);		// ICW2
	outb(IO_PIC2+1, IRQ_SLAVE);		// ICW3
	// NB Automatic EOI mode doesn't tend to work on the slave.
	// Linux source code says it's "to be investigated".
	outb(IO_PIC2+1, 0x01);			// ICW4

	// OCW3:  0ef01prs
	//   ef:  0x = NOP, 10 = clear specific mask, 11 = set specific mask
	//    p:  0 = no polling, 1 = polling mode
	//   rs:  0x = NOP, 10 = read IRR, 11 = read ISR
	outb(IO_PIC1, 0x68);             /* clear specific mask */
	outb(IO_PIC1, 0x0a);             /* read IRR by default */

	outb(IO_PIC2, 0x68);               /* OCW3 */
	outb(IO_PIC2, 0x0a);               /* OCW3 */
}
