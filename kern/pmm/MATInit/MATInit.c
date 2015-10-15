//Author: Bo Song
//9/20/2015
#include <lib/debug.h>
#include "import.h"

#define PAGESIZE  4096
#define VM_USERLO  0x40000000
#define VM_USERHI  0xF0000000
#define VM_USERLO_PI  (VM_USERLO / PAGESIZE)
#define VM_USERHI_PI  (VM_USERHI / PAGESIZE)

/**
 * The initialization function for the allocation table AT.
 * It contains two major parts:
 * 1. Calculate the actual physical memory of the machine, and sets the number of physical pages (NUM_PAGES).
 * 2. Initializes the physical allocation table (AT) implemented in MATIntro layer, based on the
 *    information available in the physical memory map table.
 *    Review import.h in the current directory for the list of avaiable getter and setter functions.
 */

void
pmem_init(unsigned int mbi_addr)
{
  // : Define your local variables here.
  unsigned int nps;
  unsigned int table_nrow;
  unsigned int start_addr;
  unsigned int length;
  unsigned int highest_addr;
  unsigned int page_idx;
  unsigned int perm;
  unsigned int i;

  //Calls the lower layer initializatin primitives.
  //The parameter mbi_addr shell not be used in the further code.
  
  devinit(mbi_addr);

  /**
   * Calculate the number of actual number of avaiable physical pages and store it into the local varaible nps.
   * Hint: Think of it as the highest address possible in the ranges of the memory map table,
   *       divided by the page size.
   */

  table_nrow = get_size();
  if(table_nrow == 0){
    nps = 0;
  }
  else{
    // Does the highest address stores in the last row of table?
    //Yes the last row of e820 stores the highest mem address

    start_addr = get_mms(table_nrow - 1);
    length = get_mml(table_nrow - 1);
    highest_addr = start_addr + length - 1;
    nps = start_addr / PAGESIZE + length / PAGESIZE + (start_addr % PAGESIZE + length % PAGESIZE) / PAGESIZE;
  }
  set_nps(nps); // Setting the value computed above to NUM_PAGES.

  /**
   * Initialization of the physical allocation table (AT).
   *
   * In CertiKOS, the entire addresses < VM_USERLO or >= VM_USERHI are reserved by the kernel.
   * That corresponds to the physical pages from 0 to VM_USERLO_PI-1, and from VM_USERHI_PI to NUM_PAGES-1.
   * The rest of pages that correspond to addresses [VM_USERLO, VM_USERHI), can be used freely ONLY IF
   * the entire page falls into one of the ranges in the memory map table with the permission marked as usable.
   *
   * Hint:
   * 1. You have to initialize AT for all the page indices from 0 to NPS - 1.
   * 2. For the pages that are reserved by the kernel, simply set its permission to 1.
   *    Recall that the setter at_set_perm also marks the page as unallocated. 
   *    Thus, you don't have to call another function to set the allocation flag.
   * 3. For the rest of the pages, explore the memory map table to set its permission accordingly.
   *    The permission should be set to 2 only if there is a range containing the entire page that
   *    is marked as available in the memory map table. O.w., it should be set to 0.
   *    Note that the ranges in the memory map are not aligned by pages.
   *    So it may be possible that for some pages, only part of the addresses are in the ranges.
   *    Currently, we do not utilize partial pages, so in that case, you should consider those pages as unavailble.
   * 4. Every page in the allocation table shold be initialized.
   *    But the ranges in the momory map table may not cover the entire available address space.
   *    That means there may be some gaps between the ranges.
   *    You should still set the permission of those pages in allocation table to 0.
   */
   
  // Reserved by the kernel
  for(i = 0; i < VM_USERLO_PI; i++){
    at_set_perm(i, 1);
  }
  for(i = VM_USERHI_PI; i < nps; i++){
    at_set_perm(i, 1);
  }
  //set all default permission to 0
  for(i = VM_USERLO_PI; i < VM_USERHI_PI; i++){
    at_set_perm(i, 0);
  }
  for(i = 0; i < table_nrow; i++){
    start_addr = get_mms(i);
    length = get_mml(i);
    perm = is_usable(i);
    perm = perm == 1? 2: 0;
    page_idx = start_addr / PAGESIZE;
    //align to the beginning of a page
    if(page_idx * PAGESIZE < start_addr){
      page_idx++;
    }
    //the whole page resides in this row
    while((page_idx + 1) * PAGESIZE <= start_addr + length){
      //the kernel reserved area
      if(page_idx < VM_USERLO_PI){
        page_idx++;
        continue;
      }
      if(page_idx >= VM_USERHI_PI){
        break;
      }
      at_set_perm(page_idx, perm);
      page_idx++;
    }
  }
}
