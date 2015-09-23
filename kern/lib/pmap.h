#ifndef _SYS_LIB_PREINIT_PMAP_H_
#define _SYS_LIB_PREINIT_PMAP_H_

#include <lib/types.h>

size_t pt_copyin(uint32_t pmap_id, uintptr_t uva, void *kva, size_t len);
size_t pt_copyout(void *kva, uint32_t pmap_id, uintptr_t uva, size_t len);
size_t pt_memset(uint32_t pmap_id, uintptr_t va, char c, size_t len);

#endif /* !_SYS_LIB_PREINIT_PMAP_H_ */
