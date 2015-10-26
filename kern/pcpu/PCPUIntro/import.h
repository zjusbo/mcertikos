#ifndef _KERN_PCPU_PCPUINTRO_H_
#define _KERN_PCPU_PCPUINTRO_H_

#ifdef _KERN_

void pcpu_init_cpu(void);
int pcpu_boot_ap(uint32_t cpu_idx, void (*f)(void), uintptr_t stack_addr);
bool pcpu_onboot(void);
uint32_t pcpu_ncpu(void);
bool pcpu_is_smp(void);
lapicid_t pcpu_cpu_lapicid(int cpu_idx);

#endif /* _KERN_ */

#endif /* !_KERN_PCPU_PCPUINTRO_H_ */

