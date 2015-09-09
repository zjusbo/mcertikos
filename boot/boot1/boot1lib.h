#ifndef _CERTIKOS_BOOT_LIB_H_
#define _CERTIKOS_BOOT_LIB_H_

/**
 * gcc
 */
#define gcc_packed		__attribute__((packed))

/**
 * types
 */
typedef __signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;

/**
 * x86 instructions
 */
static inline void
outb(int port, uint8_t data)
{
	__asm __volatile("outb %0,%w1" : : "a" (data), "d" (port));
}

static inline void
outw(int port, uint16_t data)
{
	__asm __volatile("outw %0,%w1" : : "a" (data), "d" (port));
}

static inline uint8_t
inb(int port)
{
	uint8_t data;
	__asm __volatile("inb %w1,%0" : "=a" (data) : "d" (port));
	return data;
}

static inline void
insl(int port, void *addr, int cnt)
{
	__asm __volatile("cld\n\trepne\n\tinsl"			:
			 "=D" (addr), "=c" (cnt)		:
			 "d" (port), "0" (addr), "1" (cnt)	:
			 "memory", "cc");
}

/**
 * video
 */

#define CRT_ROWS	25
#define CRT_COLS	80

#define VGA_CLR_BLACK	0
#define VGA_CLR_BLUE	1
#define VGA_CLR_GREEN	2
#define VGA_CLR_CYAN	3
#define VGA_CLR_RED		4
#define VGA_CLR_MAGENTA	5
#define VGA_CLR_BROWN	6
#define VGA_CLR_LIGHT_GRAY	7
#define VGA_CLR_DARK_GRAY	8
#define VGA_CLR_LIGHT_BLUE	9
#define VGA_CLR_LIGHT_GREEN	10
#define VGA_CLR_LIGHT_CYAN	11
#define VGA_CLR_LIGHT_RED	12
#define VGA_CLR_LIGHT_MAGENTA	13
#define VGA_CLR_YELLOW	14
#define VGA_CLR_WHITE	15


int puts (int r, int c, int color, const char *string);
void putline(char * s);
void puti (int32_t i);
void roll(int r);
void panic(char * m);

/**
 * string
 */
int strlen (const char *s);
void reverse (char s[]);
void itoa (int n, char* s);
void itoh (int n, char* s);

/**
 * disk
 */
#define SECTOR_SIZE	512

typedef struct mbr
{
	uint8_t bootloader[436];
	uint8_t disk_sig[10];
	struct
	{
		uint8_t bootable;
#define INACTIVE_PARTITION	0x00
#define BOOTABLE_PARTITION	0x80
		uint8_t first_chs[3];
		uint8_t id;
#define EMPTY_PARTITION		0x00
#define LINUX_PARTITION		0x83
#define EXTENDED_PARTITION	0x5
		uint8_t last_chs[3];
		uint32_t first_lba;
		uint32_t sectors_count;
	}gcc_packed partition[4];
	uint8_t signature[2];
}gcc_packed mbr_t;

void readsector (void *dst, uint32_t offset);

void readsection (uint32_t va, uint32_t count, uint32_t offset, uint32_t lba);

/**
 * physical memory map
 */
typedef
struct bios_smap {
	uint32_t size;
	uint64_t base_addr;
	uint64_t length;
	uint32_t type;
} gcc_packed bios_smap_t;

/**
 * elf
 */
#define ELF_MAGIC 0x464C457FU	/* "\x7FELF" in little endian */

// ELF header
typedef struct elfhdf {
	uint32_t e_magic;	// must equal ELF_MAGIC
	uint8_t e_elf[12];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint32_t e_entry;
	uint32_t e_phoff;
	uint32_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
} elfhdr;

// ELF program header
typedef struct proghdr {
	uint32_t p_type;
	uint32_t p_offset;
	uint32_t p_va;
	uint32_t p_pa;
	uint32_t p_filesz;
	uint32_t p_memsz;
	uint32_t p_flags;
	uint32_t p_align;
} proghdr;

/**
 * mboot
 */
/* Provided by bootloader */
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

#endif
