#include <lib/debug.h>
#include <lib/types.h>
#include <lib/monitor.h>
#include <thread/PThread/export.h>

#ifdef TEST
extern bool test_PKCtxNew(void);
extern bool test_PTCBInit(void);
extern bool test_PTQueueInit(void);
extern bool test_PThread(void);
#endif

static void
kern_main (void)
{
    KERN_DEBUG("In kernel main.\n\n");

    #ifdef TEST
    dprintf("Testing the PKCtxNew layer...\n");
    if(test_PKCtxNew() == 0)
      dprintf("All tests passed.\n");
    else
      dprintf("Test failed.\n");
    dprintf("\n");

    dprintf("Testing the PTCBInit layer...\n");
    if(test_PTCBInit() == 0)
      dprintf("All tests passed.\n");
    else
      dprintf("Test failed.\n");
    dprintf("\n");

    dprintf("Testing the PTQueueInit layer...\n");
    if(test_PTQueueInit() == 0)
      dprintf("All tests passed.\n");
    else
      dprintf("Test failed.\n");
    dprintf("\n");

    dprintf("Testing the PThread layer...\n");
    if(test_PThread() == 0)
      dprintf("All tests passed.\n");
    else
      dprintf("Test failed.\n");
    dprintf("\n");
    dprintf("\nTest complete. Please Use Ctrl-a x to exit qemu.");
    #else
    monitor(NULL);
    #endif
}

void
kern_init (uintptr_t mbi_addr)
{
    thread_init(mbi_addr);

    KERN_DEBUG("Kernel initialized.\n");

    kern_main ();
}
