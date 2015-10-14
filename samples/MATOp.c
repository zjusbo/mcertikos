#include <lib/debug.h>
#include "import.h"

#define KERN_LOW 262144
#define NUM_PROC 64

static unsigned int last_palloc_index = 262144;

/**
 * Allocation of a physical page.
 *
 * 1. First, implement a naive page allocator that scans the allocation table (AT) 
 *    using the functions defined in import.h to find the first unallocated page
 *    with usable permission.
 *    (Q: Do you have to scan allocation table from index 0? Recall how you have
 *    initialized the table in pmem_init.)
 *    Then mark the page as allocated in the allocation table and return the page
 *    index of the page found. In the case when there is no avaiable page found,
 *    return 0.
 * 2. Optimize the code with the memorization techniques so that you do not have to
 *    scan the allocation table from scratch every time.
 */
unsigned int
palloc()
{
    unsigned int tnps;
    unsigned int palloc_index;
    unsigned int palloc_cur_at;
    unsigned int palloc_is_norm;
    unsigned int palloc_free_index;
    tnps = get_nps();
    palloc_index = last_palloc_index + 1;
    palloc_free_index = tnps;
    while( palloc_index < tnps && palloc_free_index == tnps )
    {
        palloc_is_norm = at_is_norm(palloc_index);
        if (palloc_is_norm == 1)
        {
            palloc_cur_at = at_is_allocated(palloc_index);
            if (palloc_cur_at == 0)
                palloc_free_index = palloc_index;
        }
        palloc_index ++;
    }
    if (palloc_free_index == tnps)
      palloc_free_index = 0;
    else
    {
      at_set_allocated(palloc_free_index, 1);
    }
    last_palloc_index = palloc_free_index % tnps;
    return palloc_free_index;
} 


/**
 * Free of a physical page.
 *
 * This function marks the page with given index as unallocated
 * in the allocation table.
 *
 * Hint: Simple.
 */
void
pfree(unsigned int pfree_index)
{
	at_set_allocated(pfree_index, 0);
}
