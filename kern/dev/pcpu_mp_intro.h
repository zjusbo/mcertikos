#ifndef _KERN_DEV_PCPU_MP_INTRO_H_
#define _KERN_DEV_PCPU_MP_INTRO_H_

#ifdef _KERN_

#include <lib/types.h>

#include <dev/lapic.h>

struct pcpuinfo {
	uint32_t	lapicid;
	bool		bsp;

	/* cpuid 0x0 */
	uint32_t	cpuid_high;	/* Maximum input value for basic CPUID
					   information (%eax)*/
	char		vendor[20];	/* CPU vendor string (%ebx, %ecx, %edx) */
	cpu_vendor	cpu_vendor;	/* CPU vendor */

	/* cpuid 0x1 */
	uint8_t		family;		/* CPU family (%eax[11:8]) */
	uint8_t		model;		/* CPU model (%eax[7:4]) */
	uint8_t		step;		/* CPU step (%eax[3:0]) */
	uint8_t		ext_family;	/* extended CPU family (%eax[27:20]) */
	uint8_t		ext_model;	/* extended CPU model (%eax[19:16]) */
	uint8_t		brand_idx;	/* CPU brand index (%ebx[7:0]) */
	uint8_t		clflush_size;	/* CLFLUSH line size in bits (%ebx[15:8]) */
	uint8_t		max_cpu_id;	/* maximum number of addressable IDs for
					   logical processors (%ebx[23:16]) */
	uint8_t		apic_id;	/* initial APIC id (%ebx[31:24]) */
	uint32_t	feature1;	/* CPU features (%ecx) */
	uint32_t	feature2;	/* CPU features (%edx) */

	/* L1 cache information */
	size_t		l1_cache_size;		/* L1 cache size in KBs */
	size_t		l1_cache_line_size;	/* L1 cache line size in bytes */

	/* cpuid 0x80000000 */
	uint32_t	cpuid_exthigh;	/* Maximum Input Value for Extended
					   Function CPUID Information */
};


#endif /* _KERN_ */

#endif /* !_KERN_DEV_PCPU_MP_H_ */
