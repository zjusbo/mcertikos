#include <lib/debug.h>
#include <lib/elf.h>
#include <lib/string.h>
#include <lib/types.h>
#include <lib/x86.h>
#include <lib/pmap.h>
#include <lib/gcc.h>

#define VM_TOP		0xffffffff
#define VM_USERHI	0xf0000000
#define VM_DYNLINK	0xe0000000
#define VM_STACKHI	0xd0000000
#define VM_USERLO	0x40000000
#define VM_BOTTOM	0x00000000

/*
 * Load elf execution file exe to the virtual address space pmap.
 */
void
elf_load (void *exe_ptr, int pid)
{
	elfhdr *eh;
	proghdr *ph, *eph;
	sechdr *sh, *esh;
	char *strtab;
	uintptr_t exe = (uintptr_t) exe_ptr;

	eh = (elfhdr *) exe;

	KERN_ASSERT(eh->e_magic == ELF_MAGIC);
	KERN_ASSERT(eh->e_shstrndx != ELF_SHN_UNDEF);

	sh = (sechdr *) ((uintptr_t) eh + eh->e_shoff);
	esh = sh + eh->e_shnum;

	strtab = (char *) (exe + sh[eh->e_shstrndx].sh_offset);
	KERN_ASSERT(sh[eh->e_shstrndx].sh_type == ELF_SHT_STRTAB);

	ph = (proghdr *) ((uintptr_t) eh + eh->e_phoff);
	eph = ph + eh->e_phnum;

	for (; ph < eph; ph++)
	{
		uintptr_t fa;
		uint32_t va, zva, eva, perm;

		if (ph->p_type != ELF_PROG_LOAD)
			continue;

		fa = (uintptr_t) eh + rounddown (ph->p_offset, PAGESIZE);
		va = rounddown (ph->p_va, PAGESIZE);
		zva = ph->p_va + ph->p_filesz;
		eva = roundup (ph->p_va + ph->p_memsz, PAGESIZE);

		perm = PTE_U | PTE_P;
		if (ph->p_flags & ELF_PROG_FLAG_WRITE)
			perm |= PTE_W;

		for (; va < eva; va += PAGESIZE, fa += PAGESIZE)
		{
			alloc_page (pid, va, perm);

			if (va < rounddown (zva, PAGESIZE))
			{
				/* copy a complete page */
				pt_copyout ((void *) fa, pid, va, PAGESIZE);
			}
			else if (va < zva && ph->p_filesz)
			{
				/* copy a partial page */
				pt_memset (pid, va, 0, PAGESIZE);
				pt_copyout ((void *) fa, pid, va, zva - va);
			}
			else
			{
				/* zero a page */
				pt_memset (pid, va, 0, PAGESIZE);
			}
		}
	}

}

uintptr_t
elf_entry (void *exe_ptr)
{
	uintptr_t exe = (uintptr_t) exe_ptr;
	elfhdr *eh = (elfhdr *) exe;
	KERN_ASSERT(eh->e_magic == ELF_MAGIC);
	return (uintptr_t) eh->e_entry;
}
