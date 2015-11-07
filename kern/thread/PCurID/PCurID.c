#include <lib/x86.h>
#include <pcpu/PCPUIntro/export.h>
#include <lib/debug.h>

unsigned int CURID[NUM_CPUS];
unsigned int PREVID[NUM_CPUS];

unsigned int get_curid(void)
{
	return CURID[get_pcpu_idx()];
}

void set_curid(unsigned int curid)
{
        PREVID[get_pcpu_idx()] = CURID[get_pcpu_idx()];
	CURID[get_pcpu_idx()] = curid;
}

unsigned int get_previd(void)
{
        return PREVID[get_pcpu_idx()];
}
