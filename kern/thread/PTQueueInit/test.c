#include <lib/x86.h>
#include <lib/debug.h>
#include <thread/PTCBIntro/export.h>
#include <thread/PTQueueIntro/export.h>
#include "export.h"

int PTQueueInit_test1()
{
  unsigned int i;
  for (i = 0; i < NUM_IDS; i ++) {
    if (tqueue_get_head(i) != NUM_IDS || tqueue_get_tail(i) != NUM_IDS) {
      dprintf("test 1 failed.\n");
      return 1;
    }
  }
  dprintf("test 1 passed.\n");
  return 0;
}

int PTQueueInit_test2()
{
  unsigned int pid;
  tqueue_enqueue(0, 2);
  tqueue_enqueue(0, 3);
  tqueue_enqueue(0, 4);
  if (tcb_get_prev(2) != NUM_IDS || tcb_get_next(2) != 3) {
    dprintf("test 2 failed.\n");
    return 1;
  }
  if (tcb_get_prev(3) != 2 || tcb_get_next(3) != 4) {
    dprintf("test 2 failed.\n");
    return 1;
  }
  if (tcb_get_prev(4) != 3 || tcb_get_next(4) != NUM_IDS) {
    dprintf("test 2 failed.\n");
    return 1;
  }
  tqueue_remove(0, 3);
  if (tcb_get_prev(2) != NUM_IDS || tcb_get_next(2) != 4) {
    dprintf("test 2 failed.\n");
    return 1;
  }
  if (tcb_get_prev(3) != NUM_IDS || tcb_get_next(3) != NUM_IDS) {
    dprintf("test 2 failed.\n");
    return 1;
  }
  if (tcb_get_prev(4) != 2 || tcb_get_next(4) != NUM_IDS) {
    dprintf("test 2 failed.\n");
    return 1;
  }
  pid = tqueue_dequeue(0);
  if (pid != 2 || tcb_get_prev(pid) != NUM_IDS || tcb_get_next(pid) != NUM_IDS
   || tqueue_get_head(0) != 4 || tqueue_get_tail(0) != 4) {
    dprintf("test 2 failed.\n");
    return 1;
  }
  dprintf("test 2 passed.\n");
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
int PTQueueInit_test_own()
{
  // TODO (optional)
  // dprintf("own test passed.\n");
  return 0;
}

int test_PTQueueInit()
{
  return PTQueueInit_test1() + PTQueueInit_test2() + PTQueueInit_test_own();
}
