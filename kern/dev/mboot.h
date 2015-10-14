#ifndef _KERN_LIB_MBOOT_H_
#define _KERN_LIB_MBOOT_H_

#ifdef _KERN_

#include <lib/types.h>

typedef
struct mboot_info {
	uint32_t flags;

	/* if bit 0 of flags is set */
	uint32_t mem_lower;		/* the amount of the loer memory */
	uint32_t mem_upper;		/* the amount of the higher memory */

	/* if bit 1 of flags is set */
	struct {
		uint8_t driver;		/* BIOS disk device number */
		uint8_t part1;		/* top-level partition number */
		uint8_t part2;		/* second-level partition number */
		uint8_t part3;		/* third-level partition number */
	} boot_device;

	/* if bit 2 of flags is set */
	uint32_t cmdline;		/* the address of a zerom terminated
					   command string passed to kernel */

	/* if bit 3 of flags is set */
	uint32_t mods_count;
	uint32_t mods_addr;

	/* if bit 4/5 of flags is set */
	union {
		struct {
			uint32_t tabsize;
			uint32_t strsize;
			uint32_t addr;
			uint32_t _reserved;
		} aout;
		struct {
			uint32_t num;
			uint32_t size;
			uint32_t addr;
			uint32_t shndx;
		} elf;
	} syms;

	/* if bit 6 of flags is set */
	uint32_t mmap_length;		/* length of the buffer containing
					   the memory map */
	uint32_t mmap_addr;		/* address of the buffer containing
					   the memory map */

	/* if bit 7 of flags is set */
	uint32_t drives_length;
	uint32_t drives_addr;

	/* if bit 8 of flags is set */
	uint32_t config_table;

	/* if bit 9 of flags is set */
	uint32_t boot_loader_name;

	/* if bit 10 of flags is set */
	uint32_t apm_table;

	/* if bit 11 of flags is set */
	uint32_t vbe_control_info;
	uint32_t vbe_mode_info;
	uint32_t vbe_mode;
	uint32_t vbe_interface_seg;
	uint32_t vbe_interface_off;
	uint32_t vbe_interface_len;
} mboot_info_t;

typedef
struct mboot_mmap
{
	uint32_t size;
	uint32_t base_addr_low;
	uint32_t base_addr_high;
	uint32_t length_low;
	uint32_t length_high;
	uint32_t type;
} mboot_mmap_t;

/* memory types */
#define MEM_RAM			1
#define MEM_RESERVED		2
#define MEM_ACPI		3
#define MEM_NVS			4

void pmmap_init(uintptr_t mbi_addr);
int pmmap_entries_nr(void);
uint32_t pmmap_get_entry_start(int idx);
uint32_t pmmap_get_entry_length(int idx);
void set_cr3(char **);
void enable_paging(void);

#endif /* _KERN_ */

#endif /* !_KERN_LIB_MBOOT_H_ */
