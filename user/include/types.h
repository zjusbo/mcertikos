#ifndef _USER_TYPES_H_
#define _USER_TYPES_H_

#include "gcc.h"

typedef signed char		__int8_t;
typedef unsigned char		__uint8_t;
typedef short			__int16_t;
typedef unsigned short		__uint16_t;
typedef int			__int32_t;
typedef unsigned int		__uint32_t;
typedef long long		__int64_t;
typedef unsigned long long	__uint64_t;

typedef __uint64_t		__paddr_t;	/* CertiKOS uses PAE paging */
typedef __uint32_t		__uintptr_t;
typedef __int32_t		__intptr_t;

typedef __uint32_t		__size_t;
typedef __int32_t		__ssize_t;

typedef __uint32_t		__reg_t;


typedef __uint8_t	uint8_t;
typedef __uint16_t	uint16_t;
typedef __uint32_t	uint32_t;
typedef __uint64_t	uint64_t;

typedef __int8_t	int8_t;
typedef __int16_t	int16_t;
typedef __int32_t	int32_t;
typedef __int64_t	int64_t;

typedef __paddr_t	paddr_t;	/* physical address type */
typedef __intptr_t	intptr_t;	/* signed virtual address type */
typedef __uintptr_t	uintptr_t;	/* unsigned virtual address type */

typedef __size_t	size_t;
typedef __ssize_t	ssize_t;

typedef uint8_t		bool;
#define TRUE		((bool) 1)
#define FALSE		((bool) 0)

#define NULL		((void *) 0)

typedef int32_t		pid_t;
typedef int32_t		vmid_t;
typedef int32_t		vid_t;
typedef int32_t		chid_t;

typedef enum data_sz_t {
	SZ8, 	/* 1 byte */
	SZ16, 	/* 2 byte */
	SZ32	/* 4 byte */
} data_sz_t;

#define MIN(a, b)				\
	({					\
		typeof(a) _a = (a);		\
		typeof(b) _b = (b);		\
		_a <= _b ? _a : _b;		\
	})

#define MAX(a, b)				\
	({					\
		typeof(a) _a = (a);		\
		typeof(b) _b = (b);		\
		_a >= _b ? _a : _b;		\
	})

/* Round down to the nearest multiple of n */
#define ROUNDDOWN(a, n)				\
	({					\
		typeof(a) _a = (a);		\
		typeof(n) _n = (n);		\
		(typeof(a)) (_a - _a % _n);	\
	})

/* Round up to the nearest multiple of n */
#define ROUNDUP(_a, _n)						\
	({							\
		typeof(_a) __a = (_a);				\
		typeof(_n) __n = (_n);				\
		(typeof(_a)) (ROUNDDOWN(__a + __n - 1, __n));	\
	})

/* Return the offset of 'member' relative to the beginning of a struct type */
#define offsetof(type, member)	__builtin_offsetof(type, member)

#define ffs32(n)		__builtin_ffs((n))
#define ffs64(n)		__builtin_ffsll((n))

#endif /* !_USER_TYPES_H_ */
