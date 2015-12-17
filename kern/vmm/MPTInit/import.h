#ifndef _INIT_MM_MPTINIT_H_
#define _INIT_MM_MPTINIT_H_

#ifdef _INIT_

void pdir_init_kern(unsigned int);
void set_pdir_base(unsigned int);
void enable_paging(void);

#endif /* _INIT_ */

#endif /* !_INIT_MM_MPTINIT_H_ */
