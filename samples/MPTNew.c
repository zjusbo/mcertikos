#include <lib/x86.h>
#include <lib/debug.h>

#include "import.h"

/**
 * This function will be called when there's no mapping found in the page structure
 * for the given virtual address [vaddr], e.g., by the page fault handler when
 * a page fault happened because the user process accessed a virtual address
 * that is not mapped yet.
 * The task of this function is to allocate a physical page and use it to register
 * the mapping for the virtual address with given permission.
 * It should return the physical page index registered in the page directory, i.e., the
 * return value from map_page.
 * In the case of error, it should return the MagicNumber.
 */
unsigned int alloc_page (unsigned int proc_index, unsigned int vaddr, unsigned int perm)
{
	// TODO
	// return 0;
	unsigned int pi;
	unsigned int result;
	pi = container_alloc (proc_index);

	if (pi == 0)
		result = MagicNumber;
	else
		result = map_page (proc_index, vaddr, pi, perm);

	return result;
}


/**
 * Designate some memory quota for the next child process.
 */
unsigned int alloc_mem_quota (unsigned int id, unsigned int quota)
{
	unsigned int child;
  child = container_split (id, quota);
	return child;
}

