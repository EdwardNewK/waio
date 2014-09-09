#include <psxtypes/psxtypes.h>

static __inline__ void at_locked_inc(
	intptr_t volatile * ptr)
{
	__asm__(
		"lock;"
		"incl %0"
		: "=m" (*ptr)
		: "m"  (*ptr)
		: "memory");
	return;
}


static __inline__ void at_locked_dec(
	intptr_t volatile * ptr)
{
	__asm__(
		"lock;"
		"decl %0"
		: "=m" (*ptr)
		: "m"  (*ptr)
		: "memory");
	return;
}


static __inline__ void at_locked_add(
	intptr_t volatile * ptr,
	intptr_t val)
{
	/* FIXME: use addl */
	__asm__(
		"lock;"
		"xaddl %1, %0"
		: "=m" (*ptr), "=r" (val)
		: "1"  (val)
		: "memory");
	return;
}


static __inline__ void at_locked_sub(
	intptr_t volatile * ptr,
	intptr_t val)
{
	/* FIXME: use subl */
	val = -val;

	__asm__(
		"lock;"
		"xaddl %1, %0"
		: "=m" (*ptr), "=r" (val)
		: "1"  (val)
		: "memory");
	return;
}


static __inline__ intptr_t at_locked_xadd(
	intptr_t volatile * ptr,
	intptr_t val)
{
	__asm__(
		"lock;"
		"xaddl %1, %0"
		: "=m" (*ptr), "=r" (val)
		: "1"  (val)
		: "memory");
	return val;
}


static __inline__ intptr_t at_locked_xsub(
	intptr_t volatile * ptr,
	intptr_t val)
{
	val = -val;

	__asm__(
		"lock;"
		"xaddl %1, %0"
		: "=m" (*ptr), "=r" (val)
		: "1"  (val)
		: "memory");
	return val;
}


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
