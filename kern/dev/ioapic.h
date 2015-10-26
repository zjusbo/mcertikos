#ifndef _KERN_DEV_IOAPIC_H_
#define _KERN_DEV_IOAPIC_H_

#ifndef _KERN_
#error "This is a kernel header file; do not include it in userspace programs."
#endif

#include <lib/types.h>

#include <dev/lapic.h>

/*
 * IOAPIC
 */

#define IOAPIC_DEFAULT		0xFEC00000   // Default physical address of IO APIC

#define IOAPIC_ID		0x00         // Register index: ID
#define IOAPIC_VER		0x01         // Register index: version
#define IOAPIC_TABLE		0x10         // Redirection table base

// The redirection table starts at REG_TABLE and uses
// two registers to configure each interrupt.
// The first (low) register in a pair contains configuration bits.
// The second (high) register contains a bitmask telling which
// CPUs can serve that interrupt.
#define IOAPIC_INT_DISABLED	0x00010000  // Interrupt disabled
#define IOAPIC_INT_LEVEL	0x00008000  // Level-triggered (vs edge-)
#define IOAPIC_INT_ACTIVELOW	0x00002000  // Active low (vs high)
#define IOAPIC_INT_LOGICAL	0x00000800  // Destination is CPU id (vs APIC ID)

typedef volatile
struct ioapic_t {
	uint32_t reg;
	uint32_t pad[3];
	uint32_t data;
} ioapic_t;

void ioapic_init(void);
void ioapic_enable(uint8_t irq, lapicid_t, bool, bool);
void ioapic_register(uintptr_t base, lapicid_t, int gsi);
int ioapic_number(void);
ioapic_t *ioapic_get(uint32_t);

#endif /* !_KERN_DEV_IOAPIC_H_ */
