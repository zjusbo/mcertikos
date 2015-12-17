#include "import.h"

/**
 * Initializes the page structures,
 * move to the page structure # 0 (kernel).
 * and turn on the paging.
 */
void paging_init(unsigned int mbi_addr)
{
	pdir_init_kern(mbi_addr);
	set_pdir_base(0);
	enable_paging();
}
