#include <lib/x86.h>
#include <lib/seg.h>

#include "kstack.h"

uintptr_t*
get_kstack_pointer(void)
{
	struct kstack *ks =
                (struct kstack *) ROUNDDOWN(get_stack_pointer(), KSTACK_SIZE);
	
	return (uintptr_t *) ks;
}


int 
get_kstack_cpu_idx(void)
{

	struct kstack *ks = (struct kstack *) get_kstack_pointer();
	
	return ks->cpu_idx;
}
