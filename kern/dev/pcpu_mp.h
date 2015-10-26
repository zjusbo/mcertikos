#ifndef _KERN_DEV_PCPU_MP_H_
#define _KERN_DEV_PCPU_MP_H_

#ifdef _KERN_

#include <lib/types.h>
#include "pcpu_mp_intro.h"
#include <dev/lapic.h>

/*
 * The address of the first instruction which is executed by an application
 * processor core.
 *
 * XXX: It's actually start_ap() in sys/arch/i386/i386/boot_ap.S.
 */
#define PCPU_AP_START_ADDR      0x8000

bool pcpu_mp_init();
void mp_cache_flush(void);


#endif /* _KERN_ */

#endif /* !_KERN_DEV_PCPU_MP_H_ */
