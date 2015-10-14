#include <lib/gcc.h>
#include <lib/x86.h>
#include <lib/debug.h>

#include "import.h"

#define PT_PERM_UP 0
#define PT_PERM_PTU (PTE_P | PTE_W | PTE_U)

/**
 * Page directory pool for NUM_IDS processes.
 * mCertiKOS maintains one page structure for each process.
 * Each PDirPool[index] represents the page directory of the page structure for the process # [index].
 * In mCertiKOS, we statically allocate page directories, and maintain the second level page tables dynamically.
 */
char * PDirPool[NUM_IDS][1024] gcc_aligned(PAGESIZE);

/**
 * In mCertiKOS, we use identity page table mappings for the kernel memory.
 * IDPTbl is a statically allocated, identity page table that will be reused for
 * all the kernel memory.
 * Every page directory entry of kernel memory links to corresponding entry in IDPTbl.
 */
unsigned int IDPTbl[1024][1024] gcc_aligned(PAGESIZE);


// sets the CR3 register with the start address of the page structure for process # [index]
void set_pdir_base(unsigned int index)
{
    // TODO
    set_cr3(PDirPool[index]);
}

// returns the page directory entry # [pde_index] of the process # [proc_index]
// this can be used to test whether the page directory entry is mapped
unsigned int get_pdir_entry(unsigned int proc_index, unsigned int pde_index)
{
    // TODO??? return pagetable base address of entire 
    // TODO TODO TODO
    return (unsigned int)PDirPool[proc_index][pde_index];
}   

// sets specified page directory entry with the start address of physical page # [page_index].
// you should also set the permissions PTE_P, PTE_W, and PTE_U
void set_pdir_entry(unsigned int proc_index, unsigned int pde_index, unsigned int page_index)
{
    // TODO
    // bit 31-12 is the base address of page table
    unsigned int value = (page_index << 12) | PT_PERM_PTU; 
    PDirPool[proc_index][pde_index] = (char *)value;
    
}   

// sets the page directory entry # [pde_index] for the process # [proc_index]
// with the initial address of page directory # [pde_index] in IDPTbl
// you should also set the permissions PTE_P, PTE_W, and PTE_U
// this will be used to map the page directory entry to identity page table.
void set_pdir_entry_identity(unsigned int proc_index, unsigned int pde_index)
{   
    // TODO
    // the address of IDPTbl[pde_index] is aligned to 4096
    // so the low 12bits are all 0
    unsigned int value = (unsigned int)IDPTbl[pde_index];
    value |= PT_PERM_PTU;
    PDirPool[proc_index][pde_index] = (char *)value;
}   

// removes specified page directory entry (set the page directory entry to 0).
// don't forget to cast the value to (char *).
void rmv_pdir_entry(unsigned int proc_index, unsigned int pde_index)
{
    // TODO
    PDirPool[proc_index][pde_index] = (char *)0x00000000;
}   

// returns the specified page table entry.
// do not forget that the permission info is also stored in the page directory entries.
unsigned int get_ptbl_entry(unsigned int proc_index, unsigned int pde_index, unsigned int pte_index)
{   // PdirPool[proc_index][pde_index] stores the bass address of a page table
    // pte_index * 4 means each page table entry is 4 byte.
    unsigned int pte_addr = (unsigned int )PDirPool[proc_index][pde_index];
    pte_addr &= 0xfffff000; //remove perm bits
    pte_addr += pte_index << 2;//
    return *(unsigned int *)pte_addr; 
}

// sets specified page table entry with the start address of physical page # [page_index]
// you should also set the given permission
void set_ptbl_entry(unsigned int proc_index, unsigned int pde_index, unsigned int pte_index, unsigned int page_index, unsigned int perm)
{   
    // TODO
    // each page table entry is 4 byte, char * is also 4 byte
    unsigned int* pte;
    unsigned int pte_addr =  (unsigned int )PDirPool[proc_index][pde_index];
    pte_addr &= 0xfffff000;//rmove perm bits
    pte_addr += pte_index << 2;

    pte = (unsigned int *)pte_addr;
    *pte &= 0x00000000;
    *pte = page_index << 12;
    *pte |= (perm & 0x00000fff);
}   

// sets the specified page table entry in IDPTbl as the identity map.
// you should also set the given permission
void set_ptbl_entry_identity(unsigned int pde_index, unsigned int pte_index, unsigned int perm)
{
    // TODO
    IDPTbl[pde_index][pte_index] = ((pde_index << 10) + pte_index) << 12;
    IDPTbl[pde_index][pte_index] |= perm;
}

// sets the specified page table entry to 0
void rmv_ptbl_entry(unsigned int proc_index, unsigned int pde_index, unsigned int pte_index)
{
    // TODO
    unsigned int * pte;
    unsigned int pte_addr = (unsigned int)PDirPool[proc_index][pde_index];
    pte_addr &= 0xfffff000;//remove perm bits
    pte_addr += pte_index << 2;
    pte = (unsigned int *)pte_addr;
    *pte &= 0x00000000;
}
