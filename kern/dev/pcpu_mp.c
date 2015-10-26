#include <lib/debug.h>
#include <dev/mptable.h>
#include <lib/string.h>
#include <lib/types.h>
#include <lib/x86.h>
#include <lib/gcc.h>

#include <dev/acpi.h>
#include <dev/ioapic.h>
#include <dev/lapic.h>

#include <dev/pcpu_mp_intro.h>
#include <dev/pcpu_mp.h>
#include <pcpu/PCPUIntro/export.h>

#define PAGE_SIZE	4096

static uint32_t ncpu = 0;

static bool mp_inited = FALSE;

static bool ismp = FALSE;

extern uint8_t _binary___obj_kern_init_boot_ap_start[];
extern uint8_t _binary___obj_kern_init_boot_ap_size[];


void pcpu_init_cpu(void);
uint32_t pcpu_ncpu(void);
bool pcpu_is_smp(void);
bool pcpu_onboot(void);
lapicid_t pcpu_cpu_lapicid(int cpu_idx);

static gcc_inline void
pcpu_print_cpuinfo(uint32_t cpu_idx, struct pcpuinfo *cpuinfo)
{
	KERN_INFO("CPU%d: %s, FAMILY %d(%d), MODEL %d(%d), STEP %d, "
		  "FEATURE 0x%x 0x%x,%s%s%s%s%s%s%s "
		  "L1 Cache %d KB (%d bytes) \n",
		  cpu_idx, cpuinfo->vendor,
		  cpuinfo->family, cpuinfo->ext_family,
		  cpuinfo->model, cpuinfo->ext_model, cpuinfo->step,
		  cpuinfo->feature1, cpuinfo->feature2,
		  cpuinfo->feature2 & CPUID_FEATURE_SSE ? " SSE," : "",
		  cpuinfo->feature2 & CPUID_FEATURE_SSE2 ? " SSE2," : "",
		  cpuinfo->feature1 & CPUID_FEATURE_SSE3 ? " SSE3," : "",
		  cpuinfo->feature1 & CPUID_FEATURE_SSSE3 ? " SSSE3," : "",
		  cpuinfo->feature1 & CPUID_FEATURE_SSE41 ? " SSE41," : "",
		  cpuinfo->feature1 & CPUID_FEATURE_SSE42 ? " SSE42," : "",
		  cpuinfo->feature1 & CPUID_FEATURE_POPCNT ? " POPCNT," : "",
		  cpuinfo->l1_cache_size, cpuinfo->l1_cache_line_size);
}

static void
pcpu_identify(void)
{
	int cpu_idx = get_pcpu_idx();
	struct pcpuinfo *cpuinfo = (struct pcpuinfo*) get_pcpu_arch_info_pointer(cpu_idx);
	uint32_t eax, ebx, ecx, edx;

	int i, j;
	uint8_t *desc;
	uint32_t *regs[4] = { &eax, &ebx, &ecx, &edx };

	static const int intel_cache_info[0xff][2] = {
		[0x0a] = {  8, 32 },
		[0x0c] = { 16, 32 },
		[0x0d] = { 16, 64 },
		[0x0e] = { 24, 64 },
		[0x2c] = { 32, 64 },
		[0x60] = { 16, 64 },
		[0x66] = {  8, 64 },
		[0x67] = { 16, 64 },
		[0x68] = { 32, 64 }
	};

	cpuid(0x0, &eax, &ebx, &ecx, &edx);
	cpuinfo->cpuid_high = eax;
	((uint32_t *) cpuinfo->vendor)[0] = ebx;
	((uint32_t *) cpuinfo->vendor)[1] = edx;
	((uint32_t *) cpuinfo->vendor)[2] = ecx;
	cpuinfo->vendor[12] = '\0';

	if (strncmp(cpuinfo->vendor, "GenuineIntel", 20) == 0)
		cpuinfo->cpu_vendor = INTEL;
	else if (strncmp(cpuinfo->vendor, "AuthenticAMD", 20) == 0)
		cpuinfo->cpu_vendor = AMD;
	else
		cpuinfo->cpu_vendor = UNKNOWN_CPU;

	cpuid(0x1, &eax, &ebx, &ecx, &edx);
	cpuinfo->family = (eax >> 8) & 0xf;
	cpuinfo->model = (eax >> 4) & 0xf;
	cpuinfo->step = eax & 0xf;
	cpuinfo->ext_family = (eax >> 20) & 0xff;
	cpuinfo->ext_model = (eax >> 16) & 0xff;
	cpuinfo->brand_idx = ebx & 0xff;
	cpuinfo->clflush_size = (ebx >> 8) & 0xff;
	cpuinfo->max_cpu_id = (ebx >> 16) &0xff;
	cpuinfo->apic_id = (ebx >> 24) & 0xff;
	cpuinfo->feature1 = ecx;
	cpuinfo->feature2 = edx;

	switch (cpuinfo->cpu_vendor) {
	case INTEL:
		/* try cpuid 2 first */
		cpuid(0x00000002, &eax, &ebx, &ecx, &edx);
		i = eax & 0x000000ff;
		while (i--)
			cpuid(0x00000002, &eax, &ebx, &ecx, &edx);

		for (i = 0; i < 4; i++) {
			desc = (uint8_t *) regs[i];
			for (j = 0; j < 4; j++) {
				cpuinfo->l1_cache_size =
					intel_cache_info[desc[j]][0];
				cpuinfo->l1_cache_line_size =
					intel_cache_info[desc[j]][1];
			}
		}

		/* try cpuid 4 if no cache info are got by cpuid 2 */
		if (cpuinfo->l1_cache_size && cpuinfo->l1_cache_line_size)
			break;

		for (i = 0; i < 3; i++) {
			cpuid_subleaf(0x00000004, i, &eax, &ebx, &ecx, &edx);
			if ((eax & 0xf) == 1 && ((eax & 0xe0) >> 5) == 1)
				break;
		}

		if (i == 3) {
			KERN_WARN("Cannot determine L1 cache size.\n");
			break;
		}

		cpuinfo->l1_cache_size =
			(((ebx & 0xffc00000) >> 22) + 1) *	/* ways */
			(((ebx & 0x003ff000) >> 12) + 1) *	/* partitions */
			(((ebx & 0x00000fff)) + 1) *		/* line size */
			(ecx + 1) /				/* sets */
			1024;
		cpuinfo->l1_cache_line_size = ((ebx & 0x00000fff)) + 1;

		break;
	case AMD:
		cpuid(0x80000005, &eax, &ebx, &ecx, &edx);
		cpuinfo->l1_cache_size = (ecx & 0xff000000) >> 24;
		cpuinfo->l1_cache_line_size = (ecx & 0x000000ff);
		break;
	default:
		cpuinfo->l1_cache_size = 0;
		cpuinfo->l1_cache_line_size = 0;
		break;
	}

	cpuid(0x80000000, &eax, &ebx, &ecx, &edx);
	cpuinfo->cpuid_exthigh = eax;

	pcpu_print_cpuinfo(get_pcpu_idx(), cpuinfo);
}

static void
pcpu_mp_init_cpu(uint32_t idx, uint8_t lapic_id, bool is_bsp)
{
	KERN_ASSERT((is_bsp == TRUE && idx == 0) || (is_bsp == FALSE));

	if (idx >= NUM_CPUS)
	return;

	struct pcpuinfo *info = (struct pcpuinfo *) get_pcpu_arch_info_pointer(idx);

	info->lapicid = lapic_id;
	info->bsp = is_bsp;
}

/*
 * fallback multiple processors initialization method using MP table.
 */

static uint8_t
sum(uint8_t * addr, int len)
{
	int i, sum;

	sum = 0;
	for (i = 0; i < len; i++)
		sum += addr[i];
	return sum;
}

/* Look for an MP structure in the len bytes at addr. */
static struct mp *
mpsearch1(uint8_t * addr, int len)
{
	uint8_t *e, *p;

	e = addr + len;
	for (p = addr; p < e; p += sizeof(struct mp))
		if (memcmp(p, "_MP_", 4) == 0 && sum(p, sizeof(struct mp)) == 0)
			return (struct mp *) p;
	return 0;
}

/*
 * Search for the MP Floating Pointer Structure, which according to the
 * spec is in one of the following three locations:
 * 1) in the first KB of the EBDA;
 * 2) in the last KB of system base memory;
 * 3) in the BIOS ROM between 0xE0000 and 0xFFFFF.
 */
static struct mp *
mpsearch(void)
{
	uint8_t          *bda;
	uint32_t            p;
	struct mp      *mp;

	bda = (uint8_t *) 0x400;
	if ((p = ((bda[0x0F] << 8) | bda[0x0E]) << 4)) {
		if ((mp = mpsearch1((uint8_t *) p, 1024)))
			return mp;
	} else {
		p = ((bda[0x14] << 8) | bda[0x13]) * 1024;
		if ((mp = mpsearch1((uint8_t *) p - 1024, 1024)))
			return mp;
	}
	return mpsearch1((uint8_t *) 0xF0000, 0x10000);
}

/*
 * Search for an MP configuration table.  For now,
 * don 't accept the default configurations (physaddr == 0).
 * Check for correct signature, calculate the checksum and,
 * if correct, check the version.
 *
 */
static struct mpconf *
mpconfig(struct mp **pmp) {
	struct mpconf  *conf;
	struct mp      *mp;

	if ((mp = mpsearch()) == 0 || mp->physaddr == 0)
		return 0;
	conf = (struct mpconf *) mp->physaddr;
	if (memcmp(conf, "PCMP", 4) != 0)
		return 0;
	if (conf->version != 1 && conf->version != 4)
		return 0;
	if (sum((uint8_t *) conf, conf->length) != 0)
		return 0;
	*pmp = mp;
	return conf;
}

static bool
mp_init_fallback(void)
{
	uint8_t *p, *e;
	struct mp      *mp;
	struct mpconf  *conf;
	struct mpproc  *proc;
	struct mpioapic *mpio;
	uint32_t ap_idx = 1;

	if (mp_inited == TRUE)
		return TRUE;

	if ((conf = mpconfig(&mp)) == 0) /* not SMP */
		return FALSE;

	ismp = 1;

	ncpu = 0;

	lapic_register((uintptr_t) conf->lapicaddr);

	for (p = (uint8_t *) (conf + 1), e = (uint8_t *) conf + conf->length;
	     p < e;) {
		switch (*p) {
		case MPPROC:
			proc = (struct mpproc *) p;
			p += sizeof(struct mpproc);
			if (!(proc->flags & MPENAB))
				continue;

			KERN_INFO("\tCPU%d: APIC id = %x, ",
				  ncpu, proc->apicid);

			if (proc->flags & MPBOOT) {
				KERN_INFO("BSP.\n");
				pcpu_mp_init_cpu(0, proc->apicid, TRUE);
			} else {
				KERN_INFO("AP.\n");
				pcpu_mp_init_cpu(ap_idx, proc->apicid, FALSE);
				ap_idx++;
			}
			ncpu++;
			continue;
		case MPIOAPIC:
			mpio = (struct mpioapic *) p;
			p += sizeof(struct mpioapic);

			KERN_INFO("\tIOAPIC: APIC id = %x, base = %x\n",
				  mpio->apicno, mpio->addr);

			ioapic_register((uintptr_t) mpio->addr,
					mpio->apicno, 0);
			continue;
		case MPBUS:
		case MPIOINTR:
		case MPLINTR:
			p += 8;
			continue;
		default:
			KERN_WARN("mpinit: unknown config type %x\n", *p);
		}
	}

	if (mp->imcrp) {
		outb(0x22, 0x70);
		outb(0x23, inb(0x23) | 1);
	}

	/*
	 * Copy AP boot code to 0x8000.
	 */
	memcpy((uint8_t *)0x8000,
	       _binary___obj_kern_init_boot_ap_start,
	       (size_t)_binary___obj_kern_init_boot_ap_size);

	mp_inited = TRUE;

	return TRUE;
}

/*
 * multiple processors initialization method using ACPI
 */

bool
pcpu_mp_init(void)
{

	uint8_t *p, *e;
	acpi_rsdp_t *rsdp;
	acpi_rsdt_t *rsdt;
	acpi_xsdt_t *xsdt;
	acpi_madt_t *madt;
	uint32_t ap_idx = 1;
	bool found_bsp=FALSE;

	if (mp_inited == TRUE)
		return TRUE;

	KERN_INFO("\n");

	if ((rsdp = acpi_probe_rsdp()) == NULL) {
		KERN_DEBUG("Not found RSDP.\n");
		goto fallback;
	}

	xsdt = NULL;
	if ((xsdt = acpi_probe_xsdt(rsdp)) == NULL &&
	    (rsdt = acpi_probe_rsdt(rsdp)) == NULL) {
		KERN_DEBUG("Not found either RSDT or XSDT.\n");
		goto fallback;
	}

	if ((madt =
	     (xsdt != NULL) ?
	     (acpi_madt_t *) acpi_probe_xsdt_ent(xsdt, ACPI_MADT_SIG) :
	     (acpi_madt_t *) acpi_probe_rsdt_ent(rsdt, ACPI_MADT_SIG)) == NULL) {
		KERN_DEBUG("Not found MADT.\n");
		goto fallback;
	}

	ismp = TRUE;

	lapic_register(madt->lapic_addr);

	ncpu = 0;

	p = (uint8_t *)madt->ent;
	e = (uint8_t *)madt + madt->length;

	while (p < e) {
		acpi_madt_apic_hdr_t * hdr = (acpi_madt_apic_hdr_t *) p;

		switch (hdr->type) {
		case ACPI_MADT_APIC_LAPIC:
			;
			acpi_madt_lapic_t *
				lapic_ent = (acpi_madt_lapic_t *) hdr;

			if (!(lapic_ent->flags & ACPI_APIC_ENABLED)) {
				/* KERN_DEBUG("CPU is disabled.\n"); */
				break;
			}

			KERN_INFO("\tCPU%d: APIC id = %x, ",
				  ncpu, lapic_ent->lapic_id);

			//according to acpi p.138, section 5.2.12.1,
			//"platform firmware should list the boot processor as the first processor entry in the MADT"
			if (!found_bsp) {
				found_bsp=TRUE;
				KERN_INFO("BSP\n");
				pcpu_mp_init_cpu(0, lapic_ent->lapic_id, TRUE);
			} else {
				KERN_INFO("AP\n");
				pcpu_mp_init_cpu
					(ap_idx, lapic_ent->lapic_id, FALSE);
				ap_idx++;
			}

			ncpu++;

			break;

		case ACPI_MADT_APIC_IOAPIC:
			;
			acpi_madt_ioapic_t *
				ioapic_ent = (acpi_madt_ioapic_t *)hdr;

			KERN_INFO("\tIOAPIC: APIC id = %x, base = %x\n",
				  ioapic_ent->ioapic_id, ioapic_ent->ioapic_addr);

			ioapic_register(ioapic_ent->ioapic_addr,
					ioapic_ent->ioapic_id,
					ioapic_ent->gsi);

			break;

		default:
			;
			KERN_INFO("\tUnhandled ACPI entry (type=%x)\n",
				  hdr->type);
			break;
		}

		p += hdr->length;
	}

	/*
	 * Force NMI and 8259 signals to APIC when PIC mode
	 * is not implemented.
	 *
	 */
	if ((madt->flags & APIC_MADT_PCAT_COMPAT) == 0) {
		outb(0x22, 0x70);
		outb(0x23, inb(0x23) | 1);
	}

	/*
	 * Copy AP boot code to 0x8000.
	 */
	memmove((uint8_t *)0x8000,
	       _binary___obj_kern_init_boot_ap_start,
		(size_t)_binary___obj_kern_init_boot_ap_size);

	mp_inited = TRUE;

	return TRUE;

 fallback:
	KERN_DEBUG("Use the fallback multiprocessor initialization.\n");
	if (mp_init_fallback() == FALSE) {
		ismp = 0;
		ncpu = 1;
		return FALSE;
	} else
		return TRUE;
}

int
pcpu_boot_ap(uint32_t cpu_idx, void (*f)(void), uintptr_t stack_addr)
{
	KERN_ASSERT(cpu_idx > 0 && cpu_idx < pcpu_ncpu());
	KERN_ASSERT(get_pcpu_inited_info(cpu_idx) == TRUE);
	KERN_ASSERT(f != NULL);

	/* avoid being called by AP */
	if (pcpu_onboot() == FALSE)
		return 1;

	if (get_pcpu_boot_info(cpu_idx) == TRUE)
		return 0;

	extern void kern_init_ap(void);		/* defined in sys/kern/init.c */
	uint8_t *boot = (uint8_t *) PCPU_AP_START_ADDR;
	*(uintptr_t *) (boot - 4) = stack_addr + PAGE_SIZE;
	*(uintptr_t *) (boot - 8) = (uintptr_t) f;
	*(uintptr_t *) (boot - 12) = (uintptr_t) kern_init_ap;
	lapic_startcpu(pcpu_cpu_lapicid(cpu_idx), (uintptr_t) boot);

	/* wait until the processor is intialized */
	while (get_pcpu_boot_info(cpu_idx) == FALSE)
		pause();

	KERN_ASSERT(get_pcpu_boot_info(cpu_idx) == TRUE);

	return 0;
}

void
pcpu_init_cpu(void)
{
	pcpu_identify();
}

uint32_t
pcpu_ncpu(void)
{
	return ncpu;
}

bool
pcpu_is_smp(void)
{
	return ismp;
}

bool
pcpu_onboot(void)
{
	int cpu_idx = get_pcpu_idx();
	struct pcpuinfo* arch_info = (struct pcpuinfo *) get_pcpu_arch_info_pointer(cpu_idx);
	return (mp_inited == TRUE) ?
		arch_info->bsp : (get_pcpu_idx() == 0);
}

lapicid_t
pcpu_cpu_lapicid(int cpu_idx)
{
	struct pcpuinfo* arch_info = (struct pcpuinfo *) get_pcpu_arch_info_pointer(cpu_idx);
	KERN_ASSERT(0 <= cpu_idx && cpu_idx < ncpu);
	return arch_info->lapicid;
}

