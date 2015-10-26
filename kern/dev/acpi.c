#include <lib/debug.h>
#include <lib/types.h>

#include "acpi.h"

static uint8_t
sum(uint8_t * addr, int len)
{
	int i, sum;

	sum = 0;
	for (i = 0; i < len; i++) {

		sum += addr[i];
	}

	return sum;
}

static acpi_rsdp_t *
acpi_probe_rsdp_aux(uint8_t *addr, int length)
{
	uint8_t *e, *p;

	/* debug("Search %08x ~ %08x for RSDP\n", addr, addr+length); */
	e = addr + length;
	for (p = addr; p < e; p += 16) {
		if (*(uint32_t *)p == ACPI_RSDP_SIG1 &&
		    *(uint32_t *)(p + 4) == ACPI_RSDP_SIG2 &&
		    sum(p, sizeof(acpi_rsdp_t)) == 0) {
			/* debug("RSDP is at %08x\n", p); */
			return (acpi_rsdp_t *)p;
		}
	}

	return NULL;
}

acpi_rsdp_t *
acpi_probe_rsdp(void)
{
	uint8_t *bda;
	uint32_t p;
	acpi_rsdp_t *rsdp;

	bda = (uint8_t *) 0x400;
	if ((p = ((bda[0x0F] << 8) | bda[0x0E]) << 4)) {
		/* debug("Search RSDP from %08x\n", p); */
		if ((rsdp = acpi_probe_rsdp_aux((uint8_t *) p, 1024)))
			return rsdp;
	}

	/* debug("Search RSDP from 0xE0000\n"); */
	return acpi_probe_rsdp_aux((uint8_t *) 0xE0000, 0x1FFFF);
}

acpi_rsdt_t *
acpi_probe_rsdt(acpi_rsdp_t *rsdp)
{
	KERN_ASSERT(rsdp != NULL);

	acpi_rsdt_t *rsdt = (acpi_rsdt_t *)(rsdp->rsdt_addr);
	/* KERN_DEBUG("rsdp->rsdt_addr = %08x\n", rsdt); */
	if (rsdt == NULL)
		return NULL;
	if (rsdt->sig == ACPI_RSDT_SIG &&
	    sum((uint8_t *)rsdt, rsdt->length) == 0) {
		/* debug("RSDT is at %08x\n", rsdt); */
		return rsdt;
	}

	return NULL;
}

acpi_sdt_hdr_t *
acpi_probe_rsdt_ent(acpi_rsdt_t *rsdt, const uint32_t sig)
{
	KERN_ASSERT(rsdt != NULL);

	uint8_t * p = (uint8_t *)(&rsdt->ent[0]),
		* e = (uint8_t *)rsdt + rsdt->length;

	/* debug("RSDT->entry is at %08x\n", rsdt->ent); */

	int i;
	for (i = 0; p < e; i++) {
		acpi_sdt_hdr_t *hdr = (acpi_sdt_hdr_t *)(rsdt->ent[i]);
		/* debug("RSDT entry (%08x): addr = %08x, sig = %08x, length = %x\n", &rsdt->ent[i], hdr, hdr->sig, hdr->length); */
		if (hdr->sig == sig &&
		    sum((uint8_t *)hdr, hdr->length) == 0) {
			return hdr;
		}
		p = (uint8_t *)(&rsdt->ent[i+1]);
	}

	return NULL;
}

acpi_xsdt_t *
acpi_probe_xsdt(acpi_rsdp_t *rsdp)
{
	KERN_ASSERT(rsdp != NULL);

	acpi_xsdt_t *xsdt  = (acpi_xsdt_t *)(uintptr_t) rsdp->xsdt_addr;
	/* debug("rsdp->xsdt_addr = %08x\n", xsdt); */
	if (xsdt == NULL)
		return NULL;
	if (xsdt->sig == ACPI_XSDT_SIG &&
	    sum((uint8_t *)xsdt, xsdt->length) == 0) {
		/* debug("XSDT is at %08x\n", xsdt); */
		return xsdt;
	}

	return NULL;
}

acpi_sdt_hdr_t *
acpi_probe_xsdt_ent(acpi_xsdt_t *xsdt, const uint32_t sig)
{
	KERN_ASSERT(xsdt != NULL);

	uint8_t * p = (uint8_t *)(&xsdt->ent[0]),
		* e = (uint8_t *)xsdt + xsdt->length;

	int i;
	for (i = 0; p < e; i++) {
		acpi_sdt_hdr_t *hdr =
			(acpi_sdt_hdr_t *)(uintptr_t) (xsdt->ent[i]);
		/* debug("probe XSDT entry %d at %08x\n", i, hdr); */
		if (hdr->sig == sig &&
		    sum((uint8_t *)hdr, hdr->length) == 0) {
			return hdr;
		}
		p = (uint8_t *)(&xsdt->ent[i+1]);
	}

	return NULL;
}
