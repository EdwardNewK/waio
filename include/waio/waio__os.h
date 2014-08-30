#ifndef _WAIO__OS_H_
#define _WAIO__OS_H_

#include "waio__compiler.h"

/* timeout structure */
#ifdef	__WAIO_INTERNAL

#include <psxtypes/psxtypes.h>
#include <ntapi/ntapi.h>

typedef	nt_timeout	os_timeout;
typedef nt_iosb		os_iosb;
typedef intptr_t	os_signed_ptr;
typedef uintptr_t	os_unsigned_ptr;

#else

typedef	__waio_intptr	os_iosb_info;
typedef __waio_intptr	os_signed_ptr;
typedef __waio_uintptr	os_unsigned_ptr;

typedef union _os_timeout {
	struct {
		unsigned int	ulow;
		signed int	ihigh;
	} __u;
	long long		quad;
} os_timeout;


typedef struct _os_iosb {
	union {
		signed int	status;
		void *		pointer;
	};
	os_iosb_info		info;
} os_iosb;


#endif /* __WAIO_INTERNAL */

#endif /* _WAIO__OS_H_ */
