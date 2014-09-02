#include <psxtypes/psxtypes.h>

int64_t	_InterlockedIncrement64(int64_t volatile * ptr);
int64_t	_InterlockedDecrement64(int64_t volatile * ptr);
int64_t	_InterlockedExchangeAdd64(int64_t volatile * ptr, int64_t val);
int64_t	_InterlockedCompareExchange64(int64_t volatile * dst, int64_t xchg, int64_t cmp);

static __inline__ void at_locked_inc(
	intptr_t volatile * ptr)
{
	_InterlockedIncrement64(ptr);
	return;
}


static __inline__ void at_locked_dec(
	intptr_t volatile * ptr)
{
	_InterlockedDecrement64(ptr);
	return;
}


static __inline__ void at_locked_add(
	intptr_t volatile *	ptr,
	intptr_t		val)
{
	_InterlockedExchangeAdd64(ptr, val);
	return;
}


static __inline__ void at_locked_sub(
	intptr_t volatile *	ptr,
	intptr_t		val)
{
	_InterlockedExchangeAdd64(ptr, -val);
	return;
}


static __inline__ intptr_t at_locked_xadd(
	intptr_t volatile *	ptr,
	intptr_t		val)
{
	return _InterlockedExchangeAdd64(ptr, val);
}


static __inline__ intptr_t at_locked_xsub(
	intptr_t volatile *	ptr,
	intptr_t		val)
{
	return _InterlockedExchangeAdd64(ptr, -val);
}


static __inline__ intptr_t at_locked_cas(
	intptr_t volatile *	dst,
	intptr_t		cmp,
	intptr_t		xchg)
{
	return _InterlockedCompareExchange64(dst,xchg,cmp);
}
