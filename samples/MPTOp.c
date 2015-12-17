#include <lib/x86.h>

#include "import.h"

/**
 * Returns the page table entry corresponding to the virtual address,
 * according to the page structure of process # [proc_index].
 * Returns 0 if the mapping does not exist.
 */
unsigned int get_ptbl_entry_by_va(unsigned int proc_index, unsigned int vaddr)
{
    // TODO
    // return 0;
    unsigned int pde_index;
    unsigned int pte_index;
    unsigned int ptbl_entry;
    unsigned int pde;
    pde_index = vaddr / (4096 * 1024);
    pde = get_pdir_entry(proc_index, pde_index);
    if (pde == 0)
      return 0;
    pte_index = (vaddr / 4096) % 1024;
    ptbl_entry = get_ptbl_entry(proc_index, pde_index, pte_index);
    return ptbl_entry;
}         

// returns the page directory entry corresponding to the given virtual address
unsigned int get_pdir_entry_by_va(unsigned int proc_index, unsigned int vaddr)
{
    // TODO
    // return 0;
    unsigned int pde_index;
    unsigned int page_index;
    pde_index = vaddr / (4096 * 1024);
    page_index = get_pdir_entry(proc_index, pde_index);
    return page_index;
}

// removes the page table entry for the given virtual address
void rmv_ptbl_entry_by_va(unsigned int proc_index, unsigned int vaddr)
{
    // TODO
    unsigned int pde_index;
    unsigned int pte_index;
    pde_index = vaddr / (4096 * 1024);
    pte_index = (vaddr / 4096) % 1024;
    rmv_ptbl_entry(proc_index, pde_index, pte_index);
}

// removes the page directory entry for the given virtual address
void rmv_pdir_entry_by_va(unsigned int proc_index, unsigned int vaddr)
{
    // TODO
    unsigned int pde_index;
    pde_index = vaddr / (4096 * 1024);
    rmv_pdir_entry(proc_index, pde_index);
}

// maps the virtual address [vaddr] to the physical page # [page_index] with permission [perm]
// you do not need to worry about the page directory entry. just map the page table entry.
void set_ptbl_entry_by_va(unsigned int proc_index, unsigned int vaddr, unsigned int page_index, unsigned int perm)
{
    // TODO
    unsigned int pde_index;
    unsigned int pte_index;
    pde_index = vaddr / (4096 * 1024);
    pte_index = (vaddr / 4096) % 1024;
    set_ptbl_entry(proc_index, pde_index, pte_index, page_index, perm);
}

// registers the mapping from [vaddr] to physical page # [page_index] in the page directory
void set_pdir_entry_by_va(unsigned int proc_index, unsigned int vaddr, unsigned int page_index)
{
    // TODO
    unsigned int pde_index;
    pde_index = vaddr / (4096 * 1024);
    set_pdir_entry(proc_index, pde_index, page_index);
}   

// initializes the identity page table
// the permission for the kernel memory should be PTE_P, PTE_W, and PTE_G,
// while the permission for the rest should be PTE_P and PTE_W.
void idptbl_init(unsigned int mbi_adr)
{
    unsigned int i, j;
    unsigned int perm;
    container_init(mbi_adr);

    // TODO
    i = 0;
    while(i < 1024)
    {
      if (i < 256 || i >= 960)
        perm = PTE_P | PTE_W | PTE_G;
      else
        perm = PTE_P | PTE_W;
      j = 0;
      while(j < 1024)
      {
        set_ptbl_entry_identity(i, j, perm);
        j ++;
      }
      i ++;
    }
}
