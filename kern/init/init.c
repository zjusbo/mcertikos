#include <lib/debug.h>
#include <lib/types.h>
#include <lib/monitor.h>
#include <pmm/MATInit/export.h>

#define NUM_CHAN		64
#define TD_STATE_RUN		1

#ifdef TEST
extern bool test_MATIntro(void);
extern bool test_MATInit(void);
extern bool test_MATOp(void);
#endif

static void
kern_main (void)
{
    KERN_DEBUG("In kernel main.\n\n");

    #ifdef TEST
    dprintf("Testing the MATIntro layer...\n");
    if(test_MATIntro() == 0)
      dprintf("All tests passed.\n");
    else
      dprintf("Test failed.\n");
    dprintf("\n");

    dprintf("Testing the MATInit layer...\n");
    if(test_MATInit() == 0)
      dprintf("All tests passed.\n");
    else
      dprintf("Test failed.\n");
    dprintf("\n");

    dprintf("Testing the MATOp layer...\n");
    if(test_MATOp() == 0)
      dprintf("All tests passed.\n");
    else
      dprintf("Test failed.\n");
    dprintf("\n");
    #endif

    monitor(NULL);
}

void
kern_init (uintptr_t mbi_addr)
{
    pmem_init (mbi_addr);

    KERN_DEBUG("Kernel initialized.\n");

    kern_main ();
}
