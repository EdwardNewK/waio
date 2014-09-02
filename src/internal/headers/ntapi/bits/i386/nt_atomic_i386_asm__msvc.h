#include <psxtypes/psxtypes.h>

intptr_t	_InterlockedIncrement(intptr_t volatile * ptr);
intptr_t	_InterlockedDecrement(intptr_t volatile * ptr);
intptr_t	_InterlockedExchangeAdd(intptr_t volatile * ptr, intptr_t val);
intptr_t	_InterlockedCompareExchange(intptr_t volatile * dst, intptr_t xchg, intptr_t cmp);

static __inline__ void at_locked_inc(
	intptr_t volatile * ptr)
{
	_InterlockedIncrement(ptr);
	return;
}


static __inline__ void at_locked_dec(
	intptr_t volatile * ptr)
{
	_InterlockedDecrement(ptr);
	return;
}


static __inline__ void at_locked_add(
	intptr_t volatile *	ptr,
	intptr_t		val)
{
	_InterlockedExchangeAdd(ptr, val);
	return;
}


static __inline__ void at_locked_sub(
	intptr_t volatile *	ptr,
	intptr_t		val)
{
	_InterlockedExchangeAdd(ptr, -val);
	return;
}


static __inline__ intptr_t at_locked_xadd(
	intptr_t volatile *	ptr,
	intptr_t		val)
{
	return _InterlockedExchangeAdd(ptr, val);
}


static __inline__ intptr_t at_locked_xsub(
	intptr_t volatile *	ptr,
	intptr_t		val)
{
	return _InterlockedExchangeAdd(ptr, -val);
}


static __inline__ intptr_t at_locked_cas(
	intptr_t volatile *	dst,
	intptr_t		cmp,
	intptr_t		xchg)
{
	return _InterlockedCompareExchange(dst,xchg,cmp);
}
