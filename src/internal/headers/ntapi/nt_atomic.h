#ifndef _NT_ATOMIC_H_
#define _NT_ATOMIC_H_

#include <psxtypes/psxtypes.h>

static __inline__ void at_locked_inc(
	intptr_t volatile * ptr);


static __inline__ void at_locked_dec(
	intptr_t volatile * ptr);


static __inline__ void at_locked_add(
	intptr_t volatile *	ptr,
	intptr_t		val);


static __inline__ void at_locked_sub(
	intptr_t volatile *	ptr,
	intptr_t		val);


static __inline__ intptr_t at_locked_xadd(
	intptr_t volatile *	ptr,
	intptr_t		val);

static __inline__ intptr_t at_locked_xsub(
	intptr_t volatile *	ptr,
	intptr_t		val);

static __inline__ intptr_t at_locked_cas(
	intptr_t volatile *	dst,
	intptr_t		cmp,
	intptr_t		xchg);

#include "bits/nt_atomic_inline_asm.h"

#endif /* _NT_ATOMIC_H_ */
