#include <lib/debug.h>
#include <lib/x86.h>
#include <lib/gcc.h>
#include <lib/queue.h>
#include <lib/types.h>

#include "mboot.h"

#define PAGESIZE		4096

struct pmmap {
	uintptr_t		start;
	uintptr_t		end;
	uint32_t		type;
	SLIST_ENTRY(pmmap)	next;
	SLIST_ENTRY(pmmap)	type_next;
};

static struct pmmap pmmap_slots[128];
static int pmmap_slots_next_free = 0;

static SLIST_HEAD(, pmmap) pmmap_list;	/* all memory regions */
static SLIST_HEAD(, pmmap) pmmap_sublist[4];

enum __pmmap_type { PMMAP_USABLE, PMMAP_RESV, PMMAP_ACPI, PMMAP_NVS };

#define PMMAP_SUBLIST_NR(type)				\
	(((type) == MEM_RAM) ? PMMAP_USABLE :		\
	 ((type) == MEM_RESERVED) ? PMMAP_RESV :	\
	 ((type) == MEM_ACPI) ? PMMAP_ACPI :		\
	 ((type) == MEM_NVS) ? PMMAP_NVS : -1)

static uintptr_t max_usable_memory = 0;
static int mem_npages = 0;
static int pmmap_nentries = 0;

struct pmmap *
pmmap_alloc_slot(void)
{
	if (unlikely(pmmap_slots_next_free == 128))
		return NULL;
	return &pmmap_slots[pmmap_slots_next_free++];
}

/*
 * Insert an physical memory map entry in pmmap[].
 *
 * XXX: The start fields of all entries of the physical memory map are in
 *      incremental order.
 * XXX: The memory regions of some entries maybe overlapped.
 *
 * @param start
 * @param end
 * @param type
 */
static void
pmmap_insert(uintptr_t start, uintptr_t end, uint32_t type)
{
	struct pmmap *free_slot, *slot, *last_slot;

	if ((free_slot = pmmap_alloc_slot()) == NULL)
		KERN_PANIC("More than 128 E820 entries.\n");

	free_slot->start = start;
	free_slot->end = end;
	free_slot->type = type;

	last_slot = NULL;

	SLIST_FOREACH(slot, &pmmap_list, next) {
		if (start < slot->start)
			break;
		last_slot = slot;
	}

	if (last_slot == NULL)
  {
		SLIST_INSERT_HEAD(&pmmap_list, free_slot, next);
  }
	else
  {
		SLIST_INSERT_AFTER(last_slot, free_slot, next);
  }
}

static void
pmmap_merge(void)
{
	struct pmmap *slot, *next_slot;
	struct pmmap *last_slot[4] = { NULL, NULL, NULL, NULL };
	int sublist_nr;

	/*
	 * Step 1: Merge overlaped entries in pmmap_list.
	 */
	SLIST_FOREACH(slot, &pmmap_list, next) {
		if ((next_slot = SLIST_NEXT(slot, next)) == NULL)
			break;
		if (slot->start <= next_slot->start &&
		    slot->end >= next_slot->start &&
		    slot->type == next_slot->type) {
			slot->end = max(slot->end, next_slot->end);
			SLIST_REMOVE_AFTER(slot, next);
		}
	}

	/*
	 * Step 2: Create the specfic lists: pmmap_usable, pmmap_resv,
	 *         pmmap_acpi, pmmap_nvs.
	 */
	SLIST_FOREACH(slot, &pmmap_list, next) {
		sublist_nr = PMMAP_SUBLIST_NR(slot->type);
    KERN_ASSERT(sublist_nr != -1);
		if (last_slot[sublist_nr] != NULL)
			SLIST_INSERT_AFTER(last_slot[sublist_nr], slot,
					   type_next);
		else
			SLIST_INSERT_HEAD(&pmmap_sublist[sublist_nr], slot,
					  type_next);
		last_slot[sublist_nr] = slot;
	}

	if (last_slot[PMMAP_USABLE] != NULL)
		max_usable_memory = last_slot[PMMAP_USABLE]->end;
}

static void
pmmap_dump(void)
{
	struct pmmap *slot;
	SLIST_FOREACH(slot, &pmmap_list, next) {
		KERN_INFO("BIOS-e820: 0x%08x - 0x%08x (%s)\n",
			  slot->start,
			  (slot->start == slot->end) ? slot->end :
			  (slot->end == 0xffffffff) ? slot->end : slot->end - 1,
			  (slot->type == MEM_RAM) ? "usable" :
			  (slot->type == MEM_RESERVED) ? "reserved" :
			  (slot->type == MEM_ACPI) ? "ACPI data" :
			  (slot->type == MEM_NVS) ? "ACPI NVS" :
			  "unknown");
	}
}

void
pmmap_init(uintptr_t mbi_addr)
{
	KERN_INFO("\n");

	mboot_info_t *mbi = (mboot_info_t *) mbi_addr;
	mboot_mmap_t *p = (mboot_mmap_t *) mbi->mmap_addr;

	SLIST_INIT(&pmmap_list);
	SLIST_INIT(&pmmap_sublist[PMMAP_USABLE]);
	SLIST_INIT(&pmmap_sublist[PMMAP_RESV]);
	SLIST_INIT(&pmmap_sublist[PMMAP_ACPI]);
	SLIST_INIT(&pmmap_sublist[PMMAP_NVS]);

	/*
	 * Copy memory map information from multiboot information mbi to pmmap.
	 */
	while ((uintptr_t) p - (uintptr_t) mbi->mmap_addr < mbi->mmap_length) {
		uintptr_t start,end;
		uint32_t type;

		if (p->base_addr_high != 0)	/* ignore address above 4G */
			goto next;
		else
			start = p->base_addr_low;

		if (p->length_high != 0 ||
		    p->length_low >= 0xffffffff - start)
			end = 0xffffffff;
		else
			end = start + p->length_low;

		type = p->type;

		pmmap_insert(start, end, type);
    //pmmap_dump();

	next:
		p = (mboot_mmap_t *) (((uint32_t) p) + sizeof(mboot_mmap_t)/* p->size */);
	}

	/* merge overlapped memory regions */
	pmmap_merge();
	pmmap_dump();

	/* count the number of pmmap entries */
	struct pmmap *slot;
	SLIST_FOREACH(slot, &pmmap_list, next) {
		pmmap_nentries++;
	}

	/* Calculate the maximum page number */
	mem_npages = rounddown(max_usable_memory, PAGESIZE) / PAGESIZE;
}

int
get_size(void)
{
	return pmmap_nentries;
}

uint32_t
get_mms(int idx)
{
	int i = 0;
	struct pmmap *slot = NULL;

	SLIST_FOREACH(slot, &pmmap_list, next) {
		if (i == idx)
			break;
		i++;
	}

	if (slot == NULL || i == pmmap_nentries)
		return 0;

	return slot->start;
}

uint32_t
get_mml(int idx)
{
	int i = 0;
	struct pmmap *slot = NULL;

	SLIST_FOREACH(slot, &pmmap_list, next) {
		if (i == idx)
			break;
		i++;
	}

	if (slot == NULL || i == pmmap_nentries)
		return 0;

	return slot->end - slot->start;
}

int
is_usable(int idx)
{
	int i = 0;
	struct pmmap *slot = NULL;

	SLIST_FOREACH(slot, &pmmap_list, next) {
		if (i == idx)
			break;
		i++;
	}

	if (slot == NULL || i == pmmap_nentries)
		return 0;

	return slot->type == MEM_RAM;
}

void
set_cr3(char **pdir)
{
	lcr3((uint32_t) pdir);
}

void
enable_paging(void)
{
	/* enable global pages (Sec 4.10.2.4, Intel ASDM Vol3) */
	uint32_t cr4 = rcr4();
	cr4 |= CR4_PGE;
	lcr4(cr4);

	/* turn on paging */
	uint32_t cr0 = rcr0();
	cr0 |= CR0_PE | CR0_PG | CR0_AM | CR0_WP | CR0_NE | CR0_MP;
	cr0 &= ~(CR0_EM | CR0_TS);
	lcr0(cr0);
}
