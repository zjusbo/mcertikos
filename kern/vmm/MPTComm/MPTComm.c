#include <lib/x86.h>

#include "import.h"



#define VM_USERLO 0X40000000
#define VM_USERHI 0xF0000000
#define VM_USERLO_PI (VM_USERLO / PAGESIZE)
#define VM_USERHI_PI (VM_USERHI / PAGESIZE)
/**
 * For each process from id 0 to NUM_IDS -1,
 * set the page directory entries sothat the kernel portion of the map as identity map,
 * and the rest of page directories are unmmaped.
 */
void pdir_init(unsigned int mbi_adr)
{
    // TODO: define your local variables here.
    int i, j;
    
    idptbl_init(mbi_adr);

    //set other processes
    for(i = 0; i < NUM_IDS; i++){
      //kernel address, set to identity
      for(j = 0; j < (VM_USERLO_PI >> 10); j++){
        set_pdir_entry_identity(i, j);
      }
      //normal address, set to unmap
      for(j = VM_USERLO_PI >> 10; j < VM_USERHI_PI >> 10; j++){
        rmv_pdir_entry(i, j);
      }
      //kernel address, set to identity
      for(j = VM_USERHI_PI >> 10; j < 1024; j++){
        set_pdir_entry_identity(i, j);
      }
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
  unsigned int addr;
  unsigned int page_index;
  page_index = container_alloc(proc_index);
  if(page_index == 0) return 0;//no physical page vailable
  
  // register it in page table directory
  set_pdir_entry_by_va(proc_index, vadr, page_index);
  
  // clears the whole page table entries
  // addr increases 4 per step, since entry is 4 bytes
  for(addr = page_index << 12; addr < (page_index + 1) << 12; addr += 4){
    *(unsigned int *)addr &= 0x00000000; 
  }
  return page_index;
}

// Reverse operation of alloc_ptbl.
// Removes corresponding page directory entry,
// and frees the page for the page table entries (with container_free).
void free_ptbl(unsigned int proc_index, unsigned int vadr)
{
  // TODO
  unsigned int pdir_entry;
  unsigned int page_index;
  pdir_entry = get_pdir_entry_by_va(proc_index, vadr);
  page_index = pdir_entry >> 12;
  // remove page directory entry
  rmv_pdir_entry_by_va(proc_index, vadr);

  //free the page for the page table entities
  container_free(proc_index, page_index);
}
