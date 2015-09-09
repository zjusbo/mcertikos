#ifndef _KERN_MM_MALINIT_H_
#define _KERN_MM_MALINIT_H_

#ifdef _KERN_

/**
 * Premitives that are already implemented in this lab.
 */
void set_nps(unsigned int); // Sets the number of avaiable pages.
void at_set_perm(unsigned int, unsigned int); // Sets the permission of the physical page with given index.

/**
 * Getter and setter functions for the physical memory map table.
 *
 * It reports which memory address ranges are usable and which are reserved for use by the BIOS.
 * The table contains certain number of rows.
 * Each row contains the start address of the range, the length of the range, and a flag indicating
 * whether that memory address range is usable by the kernel.
 * E.g., A row (10000, 1000, 1) represents that the physical address range (10000, 11000) is avaiable for the kernel,
 * while a row (11000, 300, 0) represents that the range (11000, 11300) is reserved by the BIOS thus is not
 * available for the kernel to use.
 */
unsigned int get_size(void); // The number of rows in the table.
unsigned int get_mms(unsigned int); // The start address of the range with given row index.
unsigned int get_mml(unsigned int); // The length of the range with given row index.
unsigned int is_usable(unsigned int); // Whether the range with given row index is usable by the kernel. (0: reserved, 1: userable)

/**
 * Lower layer initialization function.
 * It initializes device drivers and interrupts.
 */
unsigned int devinit(unsigned int);

#endif /* _KERN_ */

#endif /* !_KERN_MM_MALINIT_H_ */
