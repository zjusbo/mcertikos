#include <lib/x86.h>
#include <lib/seg.h>
#include <lib/kstack.h>
#include <lib/debug.h>
#include "import.h"

static bool pcpu_inited = FALSE;

void
pcpu_init(void)
{
    struct kstack *ks =
                (struct kstack *) ROUNDDOWN(get_stack_pointer(), KSTACK_SIZE);
	  int cpu_idx = ks->cpu_idx;
    int i;

	  if (cpu_idx == 0){
        if (pcpu_inited == TRUE)
            return;

        pcpu_set_zero();

        /*
        * Probe SMP.
        */
        pcpu_mp_init();

        for (i = 0; i < NUM_CPUS; i++) {
            pcpu_fields_init(i);
        }

        pcpu_inited = TRUE;
	  }

    set_pcpu_idx(cpu_idx, cpu_idx);
    set_pcpu_kstack_pointer(cpu_idx, (uintptr_t *) ks);
    set_pcpu_boot_info(cpu_idx, TRUE);
    pcpu_init_cpu();
}

