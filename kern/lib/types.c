#include "types.h"

uint32_t
max(uint32_t a, uint32_t b)
{
	return (a > b) ? a : b;
}

uint32_t
min(uint32_t a, uint32_t b)
{
	return (a < b) ? a : b;
}

uint32_t
rounddown(uint32_t a, uint32_t n)
{
	return a - a % n;
}

uint32_t
roundup(uint32_t a, uint32_t n)
{
	return rounddown(a+n-1, n);
}
