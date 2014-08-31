#include <psxtypes/psxtypes.h>

int64_t	_InterlockedIncrement64(int64_t volatile * ptr);
int64_t	_InterlockedDecrement64(int64_t volatile * ptr);
int64_t	_InterlockedExchangeAdd64(int64_t volatile * ptr, int64_t val);
int64_t	_InterlockedCompareExchange64(int64_t volatile * dst, int64_t xchg, int64_t cmp);

static __inline__ intptr_t at_locked_cas(
	intptr_t volatile *	dst,
	intptr_t		cmp,
	intptr_t		xchg)
{
	return _InterlockedCompareExchange64(dst,xchg,cmp);
}
