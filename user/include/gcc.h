#ifndef _USER_GCC_H_
#define _USER_GCC_H_

#define gcc_aligned(mult)       __attribute__((aligned (mult)))

#define gcc_packed              __attribute__((packed))

#define gcc_inline              __inline __attribute__((always_inline))

#define gcc_noinline            __attribute__((noinline))

#define gcc_noreturn            __attribute__((noreturn))

#define likely(x)		__builtin_expect (!!(x), 1)
#define unlikely(x)		__builtin_expect (!!(x), 0)

#endif /* !_USER_GCC_H_ */
