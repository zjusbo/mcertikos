#include <lib/debug.h>
#include <pmm/MContainer/export.h>
#include <vmm/MPTOp/export.h>
#include "export.h"

int MPTKern_test1()
{
  unsigned int vaddr = 4096*1024*300;
  container_split(0, 100);
  if (get_ptbl_entry_by_va(1, vaddr) != 0) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  if (get_pdir_entry_by_va(1, vaddr) != 0) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  map_page(1, vaddr, 100, 7);
  if (get_ptbl_entry_by_va(1, vaddr) == 0) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  if (get_pdir_entry_by_va(1, vaddr) == 0) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  unmap_page(1, vaddr);
  if (get_ptbl_entry_by_va(1, vaddr) != 0) {
    dprintf("test 1 failed.\n");
    return 1;
  }
  dprintf("test 1 passed.\n");
  return 0;
}

int MPTKern_test2()
{
  unsigned int i;
  for (i = 256; i < 960; i ++) {
    if (get_ptbl_entry_by_va(0, i * 4096 * 1024L) != i * 4096 * 1024L + 3) {
      dprintf("test 2 failed.\n");
      return 1;
    }
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
int MPTKern_test_own()
{
  // TODO (optional)
  // dprintf("own test passed.\n");
  return 0;
}

int test_MPTKern()
{
  return MPTKern_test1() + MPTKern_test2() + MPTKern_test_own();
}
