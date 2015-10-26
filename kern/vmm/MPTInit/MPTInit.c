#include "import.h"

/**
 * Initializes the page structures,
 * move to the page structure # 0 (kernel).
 * and turn on the paging.
 */
void paging_init(unsigned int mbi_addr)
{
	pt_spinlock_init();
	pdir_init_kern(mbi_addr);
	pt_spinlock_acquire();
	set_pdir_base(0);
	enable_paging();
	pt_spinlock_release();
}

void paging_init_ap(void)
{
	pt_spinlock_acquire();
	set_pdir_base(0);
	enable_paging();
	pt_spinlock_release();
}
