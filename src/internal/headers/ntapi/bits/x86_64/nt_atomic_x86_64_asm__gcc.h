#include <psxtypes/psxtypes.h>

static __inline__ intptr_t at_locked_cas(
	intptr_t volatile * 	dst,
	intptr_t		cmp,
	intptr_t		xchg)
{
	intptr_t ret;

	__asm__(
		"lock;"
		"cmpxchg %3, %0"
		: "=m" (*dst), "=a" (ret)
		: "a"  (cmp),  "r"  (xchg)
		: "memory");

	return ret;
}
