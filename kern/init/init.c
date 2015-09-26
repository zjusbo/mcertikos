#include <lib/debug.h>
#include <lib/types.h>
#include <lib/monitor.h>
#include <vmm/MPTInit/export.h>
#include <vmm/MPTKern/export.h>

#define NUM_CHAN		64
#define TD_STATE_RUN		1

#ifdef TEST
extern bool test_MContainer(void);
extern bool test_MPTIntro(void);
extern bool test_MPTOp(void);
extern bool test_MPTComm(void);
extern bool test_MPTKern(void);
extern bool test_MPTNew(void);
#endif

static void
kern_main (void)
{
    KERN_DEBUG("In kernel main.\n\n");

    #ifdef TEST
    dprintf("Testing the MContainer layer...\n");
    if(test_MContainer() == 0)
      dprintf("All tests passed.\n");
    else
      dprintf("Test failed.\n");
    dprintf("\n");

    dprintf("Testing the MPTIntro layer...\n");
    if(test_MPTIntro() == 0)
      dprintf("All tests passed.\n");
    else
      dprintf("Test failed.\n");
    dprintf("\n");

    dprintf("Testing the MPTOp layer...\n");
    if(test_MPTOp() == 0)
      dprintf("All tests passed.\n");
    else
      dprintf("Test failed.\n");
    dprintf("\n");

    dprintf("Testing the MPTComm layer...\n");
    if(test_MPTComm() == 0)
      dprintf("All tests passed.\n");
    else
      dprintf("Test failed.\n");
    dprintf("\n");

    dprintf("Testing the MPTKern layer...\n");
    if(test_MPTKern() == 0)
      dprintf("All tests passed.\n");
    else
      dprintf("Test failed.\n");
    dprintf("\n");

    dprintf("Testing the MPTNew layer...\n");
    if(test_MPTNew() == 0)
      dprintf("All tests passed.\n");
    else
      dprintf("Test failed.\n");
    dprintf("\nTest complete. Please Use Ctrl-a x to exit qemu.");
    #else
    monitor(NULL);
    #endif
}

void
kern_init (uintptr_t mbi_addr)
{
    #ifdef TEST
    pdir_init_kern(mbi_addr);
    #else
    paging_init (mbi_addr);
    #endif

    KERN_DEBUG("Kernel initialized.\n");

    kern_main ();
}
