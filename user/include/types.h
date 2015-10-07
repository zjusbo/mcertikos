#ifndef _USER_TYPES_H_
#define _USER_TYPES_H_

typedef signed char		int8_t;
typedef unsigned char		uint8_t;
typedef short			int16_t;
typedef unsigned short		uint16_t;
typedef int			int32_t;
typedef unsigned int		uint32_t;
typedef long long		int64_t;
typedef unsigned long long	uint64_t;

typedef uint32_t		uintptr_t;
typedef int32_t			intptr_t;
typedef uint32_t		size_t;
typedef int32_t			ssize_t;

typedef uint8_t			bool;
#define TRUE			((bool) 1)
#define FALSE			((bool) 0)

#define NULL			((void *) 0)

typedef int32_t			pid_t;

typedef enum {
	A16, A32, A64
} addr_sz_t;

typedef enum {
	SZ8, SZ16, SZ32
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

#endif /* !_USER_TYPES_H_ */
