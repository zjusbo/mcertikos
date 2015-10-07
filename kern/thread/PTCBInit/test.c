#include <lib/x86.h>
#include <lib/debug.h>
#include <lib/thread.h>
#include <thread/PTCBIntro/export.h>
#include "export.h"

int PTCBInit_test1()
{
  unsigned int i;
  for (i = 1; i < NUM_IDS; i ++) {
    if (tcb_get_state(i) != TSTATE_DEAD || tcb_get_prev(i) != NUM_IDS || tcb_get_next(i) != NUM_IDS) {
      dprintf("test 1 failed.\n");
      return 1;
    }
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
int PTCBInit_test_own()
{
  // TODO (optional)
  // dprintf("own test passed.\n");
  return 0;
}

int test_PTCBInit()
{
  return PTCBInit_test1() + PTCBInit_test_own();
}
