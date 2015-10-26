#include <lib/x86.h>
#include <pcpu/PCPUIntro/export.h>

unsigned int CURID[NUM_CPUS];

unsigned int get_curid(void)
{
	return CURID[get_pcpu_idx()];
}

void set_curid(unsigned int curid)
{
	CURID[get_pcpu_idx()] = curid;
}
