#include <lib/debug.h>
#include <lib/types.h>

#include <dev/intr.h>

#include "ioapic.h"

#define IRQ_START	16
#define MAX_IOAPIC	16

volatile ioapic_t	*ioapics[MAX_IOAPIC];
volatile lapicid_t	ioapicid[MAX_IOAPIC];
volatile int		gsi[MAX_IOAPIC];

volatile int		ioapic_num = 0;

static uint32_t
ioapic_read(ioapic_t *base, int reg)
{
	/* KERN_DEBUG("ioapic_read: base=%x, reg=%x\n", base, reg); */
	base->reg = reg;
	return base->data;
}

static void
ioapic_write(ioapic_t *base, int reg, uint32_t data)
{
	/* KERN_DEBUG("ioapic_write: base=%x, reg=%x, data@%x\n", base, reg, data); */
	base->reg = reg;
	base->data = data;
}

void
ioapic_register(uintptr_t addr, lapicid_t id, int g)
{
	if (ioapic_num >= MAX_IOAPIC) {
		KERN_WARN("CertiKOS cannot manipulate more than %d IOAPICs.\n",
			  MAX_IOAPIC);
		return;
	}

	ioapics[ioapic_num] = (ioapic_t *) addr;
	ioapicid[ioapic_num] = id;
	gsi[ioapic_num] = g;

	ioapic_num++;
}

void
ioapic_init(void)
{
	/* KERN_ASSERT(ioapics != NULL); */

	int i;

	for (i = 0; i < ioapic_num; i++) {
		/* debug("Initialize IOAPIC %x\n", ioapicid[i]); */

		volatile ioapic_t *ioapic = ioapics[i];

		KERN_ASSERT(ioapic != NULL);

		lapicid_t id = ioapic_read(ioapic, IOAPIC_ID) >> 24;

		if (id == 0) {
			// I/O APIC ID not initialized yet - have to do it ourselves.
			ioapic_write(ioapic, IOAPIC_ID, ioapicid[i] << 24);
			id = ioapicid[i];
		}

		if (id != ioapicid[i])
			KERN_WARN("ioapic_init: id %d != ioapicid %d\n",
				  id, ioapicid[i]);

		int maxintr = (ioapic_read(ioapic, IOAPIC_VER) >> 16) & 0xFF;

		// Mark all interrupts edge-triggered, active high, disabled,
		// and not routed to any CPUs.
		int j;
		for (j = 0; j <= maxintr; j++){
			ioapic_write(ioapic, IOAPIC_TABLE + 2 * j,
				     IOAPIC_INT_DISABLED | (T_IRQ0 + j));
			ioapic_write(ioapic, IOAPIC_TABLE + 2 * j + 1, 0);
		}

	}
}

// extern bool pcpu_is_smp(void);

void
ioapic_enable(uint8_t irq, lapicid_t apicid, bool trigger_mode, bool polarity)
{
	// KERN_ASSERT(pcpu_is_smp());
	// Mark interrupt edge-triggered, active high,
	// enabled, and routed to the given APIC ID,

	int i;

	for (i = 0; i < ioapic_num; i++) {
		ioapic_t *ioapic = ioapics[i];
		int maxintr = (ioapic_read(ioapic, IOAPIC_VER) >> 16) & 0xFF;

		if (irq >= gsi[i] && irq <= gsi[i] + maxintr) {
			ioapic_write(ioapic,
				     IOAPIC_TABLE + 2 * (irq - gsi[i]),
				     ((trigger_mode << 15) | (polarity << 13) | (T_IRQ0 + irq)));

			ioapic_write(ioapic,
				     IOAPIC_TABLE + 2 * (irq - gsi[i]) + 1,
				     apicid << 24);
			break;
		}
	}

	if (i == ioapic_num)
		KERN_PANIC("Cannot enable IRQ %d on IOAPIC.\n", irq);
}

int
ioapic_number(void)
{
	return ioapic_num;
}

ioapic_t *
ioapic_get(uint32_t idx)
{
	if (idx >= ioapic_num)
		return NULL;
	return ioapics[idx];
}
