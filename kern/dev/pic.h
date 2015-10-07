/*
 * Hardware definitions for the 8259A Programmable Interrupt Controller (PIC).
 *
 * Copyright (C) 1997 Massachusetts Institute of Technology
 * See section "MIT License" in the file LICENSES for licensing terms.
 *
 * Derived from the MIT Exokernel and JOS.
 * Adapted for PIOS by Bryan Ford at Yale University.
 */

#ifndef _SYS_PREINIT_DEV_PIC_H_
#define _SYS_PREINIT_DEV_PIC_H_

#ifdef _KERN_

#ifndef __ASSEMBLER__

#include <lib/types.h>

void pic_init(void);
void pic_setmask(uint16_t mask);
void pic_enable(int irq);
void pic_eoi(void);
void pic_reset(void);

#endif /* !__ASSEMBLER__ */

#endif /* _KERN_ */

#endif /* !_SYS_PREINIT_DEV_PIC_H */
