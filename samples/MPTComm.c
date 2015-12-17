#include <lib/x86.h>

#include "import.h"

/**
 * For each process from id 0 to NUM_IDS -1,
 * set the page directory entries sothat the kernel portion of the map as identity map,
 * and the rest of page directories are unmmaped.
 */
void pdir_init(unsigned int mbi_adr)
{
    unsigned int i, j;
    idptbl_init(mbi_adr);

    // TODO
    i = 0;
    while(i < NUM_IDS)
    {
        j = 0;
        while(j < 1024)    
        {
            if (j < 256 || j >= 960)    
              set_pdir_entry_identity(i, j);
            else
              rmv_pdir_entry(i, j);
            j++;
        }
        i++;
    }
}

/**
 * Allocates a page (with container_alloc) for the page table,
 * and registers it in page directory for the given virtual address,
 * and clears (set to 0) the whole page table entries for this newly mapped page table.
 * It returns the page index of the newly allocated physical page.
 * In the case when there's no physical page available, it returns 0.
 */
unsigned int alloc_ptbl(unsigned int proc_index, unsigned int vadr)
{
  // TODO
  unsigned int i;
  unsigned int pi;
  unsigned int pde_index;
  pi = container_alloc(proc_index);
  if (pi != 0)
  {
    set_pdir_entry_by_va(proc_index, vadr, pi);
    pde_index = vadr / (4096 * 1024);
    i = 0;
    while (i < 1024)        
    {
      rmv_ptbl_entry(proc_index, pde_index, i);
      i ++;
    }     
  }       
  return pi;
}

// Reverse operation of alloc_ptbl.
// Removes corresponding page directory entry,
// and frees the page for the page table entries (with container_free).
void free_ptbl(unsigned int proc_index, unsigned int vadr)
{
  // TODO
  unsigned int pde;
  pde = get_pdir_entry_by_va(proc_index, vadr);
  rmv_pdir_entry_by_va(proc_index, vadr);
  container_free(proc_index, pde / PAGESIZE);
}
