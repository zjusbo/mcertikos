#ifndef _KERN_DEV_ACPI_H_
#define _KERN_DEV_ACPI_H_

#ifndef _KERN_
#error "This is a kernel header file; do not include it in userspace programs."
#endif

#include <lib/types.h>

/* data structure for ACPI Root System Description Pointer Structure */
typedef
struct acpi_rsdp {
	uint32_t sig[2];	/* signature (should be "RSD PTR ") */
	uint8_t checksum;	/* checksum of the first 20 bytes (ACPIv1) */
	uint8_t oemid[6];
	uint8_t revision;
	uint32_t rsdt_addr;	/* 32 bit physical address of RSDT */
	uint32_t length;	/* the length in bytes of the entire table */
	uint64_t xsdt_addr;	/* 64 bit physical address of XSDT */
	uint8_t xchecksum;	/* checksum of the entire table */
	uint8_t reserved[3];	/* reserved fields */
} acpi_rsdp_t;

/* data structure for ACPI System Description Table Header */
typedef
struct acpi_sdt_hdr {
	uint32_t sig;		/* signature */
	uint32_t length;	/* length of the entire table */
	uint8_t revision;	/* revision */
	uint8_t checksum;	/* checksum of the entire table */
	uint8_t oemid[6];
	uint64_t oemtid;
	uint32_t oemrev;
	uint8_t creator[4];	/* vendor ID of utility that created the table*/
	uint32_t creator_rev;	/* revision of the vendor */
} acpi_sdt_hdr_t;

/* Signatures of the ACPI SDT */
#define ACPI_RSDP_SIG1	0x20445352	/* "RSD PTR " */
#define ACPI_RSDP_SIG2	0x20525450
#define ACPI_FACP_SIG	0x50434146	/* "FACP" */
#define ACPI_RSDT_SIG	0x54445352	/* "RSDT" */
#define ACPI_XSDT_SIG	0x54445358	/* "XSDT" */
#define ACPI_DMAR_SIG   0x52414D44      /* "DMAR" */
#define ACPI_MADT_SIG   0x43495041	/* "APIC" */

/* data structure for ACPI Root System Description Table */
typedef
struct acpi_rsdt {
	uint32_t sig;		/* signature */
	uint32_t length;	/* length of the entire table */
	uint8_t revision;	/* revision */
	uint8_t checksum;	/* checksum of the entire table */
	uint8_t oemid[6];
	uint64_t oemtid;
	uint32_t oemrev;
	uint8_t creator[4];	/* vendor ID of utility that created the table*/
	uint32_t creator_rev;	/* revision of the vendor */
	uint32_t ent[1];	/* entries to other description table */
} acpi_rsdt_t;

/* data structure for ACPI eXtended System Description Table */
typedef
struct acpi_xsdt {
	uint32_t sig;		/* signature */
	uint32_t length;	/* length of the entire table */
	uint8_t revision;	/* revision */
	uint8_t checksum;	/* checksum of the entire table */
	uint8_t oemid[6];
	uint64_t oemtid;
	uint32_t oemrev;
	uint8_t creator[4];	/* vendor ID of utility that created the table*/
	uint32_t creator_rev;	/* revision of the vendor */
	uint64_t ent[1];	/* entries to other description table */
} acpi_xsdt_t;

/* data structure for ACPI Fixed ACPI Description Table */
typedef
struct acpi_fadt {
	uint32_t sig;		/* signature */
	uint32_t length;	/* length of the entire table */
	uint8_t revision;	/* revision */
	uint8_t checksum;	/* checksum of the entire table */
	uint8_t oemid[6];
	uint64_t oemtid;
	uint32_t oemrev;
	uint8_t creator[4];	/* vendor ID of utility that created the table*/
	uint32_t creator_rev;	/* revision of the vendor */
	uint32_t facs_addr;	/* 32 bit physical address of FACS */
	uint32_t dsdt_addr;	/* 32 bit physical address of DSDT */
} acpi_fadt_t;

/* data structure for ACPI Multiple APIC Description Table */
typedef
struct acpi_madt {
	uint32_t sig;		/* signature */
	uint32_t length;	/* length of the entire table */
	uint8_t revision;	/* revision */
	uint8_t checksum;	/* checksum of the entire table */
	uint8_t oemid[6];
	uint64_t oemtid;
	uint32_t oemrev;
	uint8_t creator[4];	/* vendor ID of utility that created the table*/
	uint32_t creator_rev;	/* revision of the vendor */
	uint32_t lapic_addr;	/* physical address of local APIC */
	uint32_t flags;		/* Multiple APIC flags */
	/*
	 * The last field of MADT is a list of APIC structures. Each structure
	 * maybe in different lengths.
	 */
	uint8_t ent[1];
} acpi_madt_t;

/* data structure for DMA Remapping Reporting Structure (DMAR) */
typedef
struct acpi_dmar {
	uint32_t sig;		/* signature */
	uint32_t length;	/* length of the entire table */
	uint8_t revision;	/* revision */
	uint8_t checksum;	/* checksum of the entire table */
	uint8_t oemid[6];
	uint64_t oemtid;
	uint32_t oemrev;
	uint8_t creator[4];	/* vendor ID of utility that created the table*/
	uint32_t creator_rev;	/* revision of the vendor */
	uint8_t host_addr_wid;	/* host addr width */
	uint8_t flags;		/* */
	uint8_t reserved[10];
	
} acpi_dmar_t;

/*DMAR subtable header */
struct acpi_dmar_header {
	uint16_t type;
	uint16_t length;
};

/* Values for subtable type in struct acpi_dmar_header*/

enum acpi_dmar_type {
	ACPI_DMAR_TYPE_HARDWARE_UNIT = 0,
	ACPI_DMAR_TYPE_RESERVED_MEMORY = 1,
	ACPI_DMAR_TYPE_ATSR = 2,
	ACPI_DMAR_HARDWARE_AFFINITY = 3,
	ACPI_DMAR_TYPE_RESERVED = 4     /* 4 and greater are reserved */
};

/* DMAR Device Scope structure */

struct acpi_dmar_device_scope {
	uint8_t entry_type;
	uint8_t length;
	uint16_t reserved;
	uint8_t enumeration_id;
	uint8_t bus;
};

/* Values for entry_type in struct acpi_dmar_device_scope */

enum acpi_dmar_scope_type {
	ACPI_DMAR_SCOPE_TYPE_NOT_USED = 0,
	ACPI_DMAR_SCOPE_TYPE_ENDPOINT = 1,
	ACPI_DMAR_SCOPE_TYPE_BRIDGE = 2,
	ACPI_DMAR_SCOPE_TYPE_IOAPIC = 3,
	ACPI_DMAR_SCOPE_TYPE_HPET = 4,
	ACPI_DMAR_SCOPE_TYPE_RESERVED = 5	/* 5 and greater are reserved */
};

struct acpi_dmar_pci_path {
	uint8_t dev;
	uint8_t fn;
};

/*
 *  * DMAR Sub-tables, correspond to Type in struct acpi_dmar_header
 *   */

/* 0: Hardware Unit Definition */

struct acpi_dmar_hardware_unit {
	struct acpi_dmar_header header;
	uint8_t flags;
	uint8_t reserved;
	uint16_t segment;
	uint64_t address;		/* Register Base Address */
};

/* Masks for Flags field above */

#define ACPI_DMAR_INCLUDE_ALL       (1)

/* 1: Reserved Memory Defininition */

struct acpi_dmar_reserved_memory {
	struct acpi_dmar_header header;
	uint16_t reserved;
	uint16_t segment;
	uint64_t base_address;	/* 4_k aligned base address */
	uint64_t end_address;	/* 4_k aligned limit address */
};

/* Masks for Flags field above */

#define ACPI_DMAR_ALLOW_ALL         (1)

/* MADT flags */
#define APIC_MADT_PCAT_COMPAT	0x1
#define APIC_MADT_FLAG_RESERVED	0xFFFFFFFE

/* data structure for APIC structures header */
typedef
struct acpi_madt_apic_hdr {
	uint8_t type;		/* type of the APIC structure */
	uint8_t length;		/* length of the entire APIC structure */
} acpi_madt_apic_hdr_t;

#define ACPI_MADT_APIC_LAPIC	0
#define ACPI_MADT_APIC_IOAPIC	1

/* Processor Local APIC Structure */
typedef
struct acpi_madt_lapic {
	acpi_madt_apic_hdr_t hdr;	/* type = 0, length = 8 */
	uint8_t acip_proc_id;	/* ACPI processor ID */
	uint8_t lapic_id;	/* local APIC ID */
	uint32_t flags;		/* local APIC flags */
} acpi_madt_lapic_t;

/* Local APIC Flags */
#define ACPI_APIC_ENABLED	0x1
#define ACPI_APIC_FLAG_RESERVED	0xFFFFFFFE

/* I/O APIC Structure */
typedef
struct acpi_madt_ioapic {
	acpi_madt_apic_hdr_t hdr;	/* type = 1, length = 12 */
	uint8_t ioapic_id;	/* I/O APIC ID */
	uint8_t reserved;	/* 0 */
	uint32_t ioapic_addr;	/* 32 bit physical address of I/O APIC */
	uint32_t gsi;		/* global system interrupt number where
				   I/O APIC interrupt inputs start */
} acpi_madt_ioapic_t;


acpi_rsdp_t *acpi_probe_rsdp(void);
acpi_rsdt_t *acpi_probe_rsdt(acpi_rsdp_t *);
acpi_sdt_hdr_t *acpi_probe_rsdt_ent(acpi_rsdt_t *, const uint32_t sig);

acpi_xsdt_t *acpi_probe_xsdt(acpi_rsdp_t *);
acpi_sdt_hdr_t *acpi_probe_xsdt_ent(acpi_xsdt_t *, const uint32_t sig);

#endif /* !_H_KERN_DEV_ACPI_ */
