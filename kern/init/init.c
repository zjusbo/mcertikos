#include <lib/debug.h>
#include <lib/types.h>
#include <lib/monitor.h>
#include <thread/PThread/export.h>
#include <dev/devinit.h>
#include <pcpu/PCPUIntro/export.h>
#include <lib/kstack.h>
#include <lib/thread.h>
#include <lib/x86.h>

#ifdef TEST
extern bool test_PKCtxNew(void);
extern bool test_PTCBInit(void);
extern bool test_PTQueueInit(void);
extern bool test_PThread(void);
#endif

static volatile int cpu_booted = 0;
static volatile int all_ready = FALSE;
static void kern_main_ap(void);

extern uint8_t _binary___obj_user_pingpong_ping_start[];
extern uint8_t _binary___obj_user_pingpong_pong_start[];

static void
kern_main (void)
{
    KERN_INFO("[BSP KERN] In kernel main.\n\n");
    
    KERN_INFO("[BSP KERN] Number of CPUs in this system: %d. \n", pcpu_ncpu());

    int cpu_idx = get_pcpu_idx();
    int i;
    unsigned int pid;

    all_ready = FALSE;
    for (i = 1; i < pcpu_ncpu(); i++){
        KERN_INFO("[BSP KERN] Boot CPU %d .... \n", i);

        bsp_kstack[i].cpu_idx = i;
        pcpu_boot_ap(i, kern_main_ap, (uintptr_t) &(bsp_kstack[i]));

        while (get_pcpu_boot_info(i) == FALSE);

        KERN_INFO("[BSP KERN] done.\n");

    }

    all_ready = TRUE;
    

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
    #endif
}

static void
kern_main_ap(void)
{
    int cpu_idx = get_pcpu_idx();
    unsigned int pid, pid2;

    set_pcpu_boot_info(cpu_idx, TRUE);

    while (all_ready == FALSE);

    KERN_INFO("[AP%d KERN] kernel_main_ap\n", cpu_idx);

    cpu_booted ++;

    #ifndef TEST
    if (cpu_idx == 1) {
        pid = proc_create (_binary___obj_user_pingpong_ping_start, 1000);
        KERN_INFO("CPU%d: process ping1 %d is created.\n", cpu_idx, pid);
        pid2 = proc_create (_binary___obj_user_pingpong_ping_start, 1000);
        KERN_INFO("CPU%d: process ping2 %d is created.\n", cpu_idx, pid2);
    }
    else if (cpu_idx == 2) {
        pid = proc_create (_binary___obj_user_pingpong_pong_start, 1000);
        KERN_INFO("CPU%d: process pong1 %d is created.\n", cpu_idx, pid);
        pid2 = proc_create (_binary___obj_user_pingpong_pong_start, 1000);
        KERN_INFO("CPU%d: process pong2 %d is created.\n", cpu_idx, pid2);
    }
    else
        return;
    tqueue_remove (NUM_IDS, pid);
    tcb_set_state (pid, TSTATE_RUN);
    set_curid (pid);
    kctx_switch (0, pid);

    KERN_PANIC("kern_main_ap() should never reach here.\n");
    #endif
}

void
kern_init (uintptr_t mbi_addr)
{
    thread_init(mbi_addr);

    KERN_INFO("[BSP KERN] Kernel initialized.\n");

    kern_main ();
}

void 
kern_init_ap(void (*f)(void))
{
	devinit_ap();
	f();
}
