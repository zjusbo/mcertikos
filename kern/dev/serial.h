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

#ifndef _SYS_PREINIT_DEV_SERIAL_H_
#define _SYS_PREINIT_DEV_SERIAL_H_

#ifdef _KERN_

void serial_init(void);
void serial_putc(char c);
void serial_intenable(void);
void serial_intr(void); // irq 4

#endif /* _KERN_ */

#endif /* !_SYS_PREINIT_DEV_SERIAL_H_ */
