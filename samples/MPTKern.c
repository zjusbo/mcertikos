#include <lib/x86.h>
#include <lib/debug.h>

#include "import.h"

/**
 * Sets the entire page map for process 0 as identity map.
 * Note that part of the task is already completed by pdir_init.
 */
void pdir_init_kern(unsigned int mbi_adr)
{
    unsigned int i;
    pdir_init(mbi_adr);
    
    //TODO
    i = 256;
    while(i < 960)
    {
        set_pdir_entry_identity(0, i);
        i ++;
    }
}

/**
 * Maps the physical page # [page_index] for the given virtual address with the given permission.
 * In the case, when the page table for the page directory entry is not set up, you need to allocate the page table first.
 * In the case of error (when the allocation fails), it returns the constant MagicNumber defined in lib/x86.h,
 * and when the page table is not set up , it returns the physical page index for the newly alloacted page table,
 * otherwise, it returns 0.
 */
unsigned int map_page(unsigned int proc_index, unsigned int vadr, unsigned int page_index, unsigned int perm)
{   
  // TODO
  // return 0;
  unsigned int pdir_entry; 
  unsigned int result;
  pdir_entry = get_pdir_entry_by_va(proc_index, vadr);
  if (pdir_entry != 0)
    result = 0;
  else
  {
    result = alloc_ptbl(proc_index, vadr);
    if (result == 0)
      result = MagicNumber;
  }
  if (result != MagicNumber)
    set_ptbl_entry_by_va(proc_index, vadr, page_index, perm);
  return result;
}

/**
 * Remove the mapping for the given virtual address (with rmv_ptbl_entry_by_va).
 * You need to first make sure that the mapping is still valid,
 * e.g., by reading the page table entry for the virtual address.
 * Nothing should be done if the mapping no longer exists.
 * You do not need to unmap the page table from the page directory.
 * It should return the corresponding page table entry.
 */
unsigned int unmap_page(unsigned int proc_index, unsigned int vadr)
{
  // TODO
  // return 0;
  unsigned int ptbl_entry;
  unsigned int count;
  ptbl_entry = get_ptbl_entry_by_va(proc_index, vadr);
  if (ptbl_entry != 0)
    rmv_ptbl_entry_by_va(proc_index, vadr);
  return ptbl_entry;
}   

