#include <dev/pcpu_mp_intro.h>
#include <lib/x86.h>

#include "import.h"

struct pcpu {
    bool              inited;         /* is pcpu structure initialized? */
    volatile bool     booted;         /* does this processor boot up ? */
    uintptr_t *       kstack;         /* bootstrap kernel stack */
    struct pcpuinfo   arch_info;      /* arch-dependent information */
    int               cpu_idx;        /* cpu id */
};

struct pcpu pcpu[NUM_CPUS];

extern int get_kstack_cpu_idx(void);

void pcpu_set_zero(){
    memzero(pcpu, sizeof(struct pcpu) * NUM_CPUS);
}

void
pcpu_fields_init(int cpu_idx){
    pcpu[cpu_idx].inited = TRUE;
    pcpu[cpu_idx].cpu_idx = cpu_idx;
}

struct pcpu *
pcpu_cur(void)
{
    int cpu_idx = get_kstack_cpu_idx();
    return &pcpu[cpu_idx];
}

int
get_pcpu_idx(void)
{
    return pcpu_cur()->cpu_idx;
}

void
set_pcpu_idx(int index, int cpu_idx)
{
    pcpu[index].cpu_idx = cpu_idx;
}

uintptr_t*
get_pcpu_kstack_pointer(int cpu_idx)
{
    return pcpu[cpu_idx].kstack;
}

void
set_pcpu_kstack_pointer(int cpu_idx, uintptr_t* ks)
{
    pcpu[cpu_idx].kstack = ks;
}

volatile bool
get_pcpu_boot_info(int cpu_idx)
{
    return pcpu[cpu_idx].booted;
}

void
set_pcpu_boot_info(int cpu_idx, volatile bool boot_info)
{
    pcpu[cpu_idx].booted = boot_info;
}

cpu_vendor
get_pcpu_cpu_vendor(int cpu_idx)
{
    return (pcpu[cpu_idx].arch_info).cpu_vendor;
}

uintptr_t*
get_pcpu_arch_info_pointer(int cpu_idx)
{
    return (uintptr_t *) &(pcpu[cpu_idx].arch_info);
}

bool
get_pcpu_inited_info(int cpu_idx)
{
    return pcpu[cpu_idx].inited;
}
