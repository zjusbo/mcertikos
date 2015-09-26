#include <lib/debug.h>
#include <pmm/MATIntro/export.h>
#include "export.h"

int MATOp_test1()
{
  int page_index = palloc();
  if (page_index < 262144) {
    pfree(page_index);
    dprintf("test 1 failed.\n");
    return 1;
  }
  if (at_is_norm(page_index) != 1) {
    pfree(page_index);
    dprintf("test 1 failed.\n");
    return 1;
  }
  if (at_is_allocated(page_index) != 1) {
    pfree(page_index);
    dprintf("test 1 failed.\n");
    return 1;
  }
  pfree(page_index);
  if (at_is_allocated(page_index) != 0) {
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
int MATOp_test_own()
{
  // TODO (optional)
  // dprintf("own test passed.\n");
  return 0;
}

int test_MATOp()
{
  return MATOp_test1() + MATOp_test_own();
}
