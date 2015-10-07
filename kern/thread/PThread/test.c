#include <lib/x86.h>
#include <lib/debug.h>
#include <lib/thread.h>
#include <thread/PTCBIntro/export.h>
#include <thread/PTQueueIntro/export.h>
#include "export.h"

int PThread_test1()
{
  void * dummy_addr = (void *) 0;
  unsigned int chid = thread_spawn(dummy_addr, 0, 1000);
  if (tcb_get_state(chid) != TSTATE_READY) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  if (tqueue_get_tail(NUM_IDS) != chid) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  dprintf("test 1 passed.\n");
  return 0;
}

/**
 * Write Your Own Test Script (optional)
 *
 * Come up with your own interesting test cases to challenge your classmates!
 * In addition to the provided simple tests, selected (correct and interesting) test functions
 * will be used in the actual grading of the lab!
 * Your test function itself will not be graded. So don't be afraid of submitting a wrong script.
 *
 * The test function should return 0 for passing the test and a non-zero code for failing the test.
 * Be extra careful to make sure that if you overwrite some of the kernel data, they are set back to
 * the original value. O.w., it may make the future test scripts to fail even if you implement all
 * the functions correctly.
 */
int PThread_test_own()
{
  // TODO (optional)
  // dprintf("own test passed.\n");
  return 0;
}

int test_PThread()
{
  return PThread_test1() + PThread_test_own();
}
