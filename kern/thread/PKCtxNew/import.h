#ifndef _KERN_THREAD_PKCTXNEW_H_
#define _KERN_THREAD_PKCTXNEW_H_

#ifdef _KERN_

unsigned int alloc_mem_quota(unsigned int id, unsigned int quota);
void kctx_set_esp(unsigned int pid, void *esp);
void kctx_set_eip(unsigned int pid, void *eip);

#endif /* _KERN_ */

#endif /* !_KERN_THREAD_PKCTXNEW_H_ */
