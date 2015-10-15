#include <lib/x86.h>
#include <lib/debug.h>
#include "export.h"

extern char * PDirPool[NUM_IDS][1024];
extern unsigned int IDPTbl[1024][1024];

int MPTIntro_test1()
{
  set_pdir_base(0);
  if ((unsigned int)PDirPool[0] != rcr3()) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  set_pdir_entry_identity(1, 1);
  set_pdir_entry(1, 2, 100);
  if (get_pdir_entry(1, 1) != (unsigned int)IDPTbl[1] +   7) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  if (get_pdir_entry(1, 2) != 409607) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  rmv_pdir_entry(1, 1);
  rmv_pdir_entry(1, 2);
  if (get_pdir_entry(1, 1) != 0 || get_pdir_entry(1, 2) != 0) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  dprintf("test 1 passed.\n");
  return 0;
}

int MPTIntro_test2()
{
  set_pdir_entry(1, 1, 10000);
  set_ptbl_entry(1, 1, 1, 10000, 259);
  if (get_ptbl_entry(1, 1, 1) != 40960259) {
    dprintf("test 2 failed.\n");
    return 1;
  }
  rmv_ptbl_entry(1, 1, 1);
  if (get_ptbl_entry(1, 1, 1) != 0) {
    dprintf("test 2 failed.\n");
    return 1;
  }
  rmv_pdir_entry(1, 1);
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
int MPTIntro_test_own()
{
  // TODO (optional)
  // dprintf("own test passed.\n");
  return 0;
}

int test_MPTIntro()
{
  return MPTIntro_test1() + MPTIntro_test2() + MPTIntro_test_own();
}
