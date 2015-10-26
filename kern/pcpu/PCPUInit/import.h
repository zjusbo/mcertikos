#ifndef _KERN_PCPU_PCPUINIT_H_
#define _KERN_PCPU_PCPUINIT_H_

#ifdef _KERN_

#include <dev/pcpu_mp.h>

/* defined in pcpu_intro.c file */
int get_pcpu_idx(void);
void set_pcpu_idx(int index, int cpu_idx);
uintptr_t* get_pcpu_kstack_pointer(int cpu_idx);
void set_pcpu_kstack_pointer(int cpu_idx, uintptr_t* ks);
volatile bool get_pcpu_boot_info(int cpu_idx);
void set_pcpu_boot_info(int cpu_idx, volatile bool boot_info);
cpu_vendor get_pcpu_cpu_vendor(int cpu_idx);
uintptr_t* get_pcpu_arch_info_pointer(int cpu_idx);
bool get_pcpu_inited_info(int cpu_idx);

/* functions defined in preinit/dev/pcpu_mp.c */
void pcpu_init_cpu(void);


#endif /* _KERN_ */

#endif /* !_KERN_PCPU_PCPUINIT_H_ */

