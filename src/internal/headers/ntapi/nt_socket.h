#ifndef _NT_SOCKET_H_
#define _NT_SOCKET_H_

#include <psxtypes/psxtypes.h>
#include "nt_status.h"
#include "nt_object.h"

/* socket structures */
typedef struct _nt_socket {
	void *			hsocket;
	void *			hevent;
	int32_t			fdtype;
	uint32_t		fshare;
	uint32_t		fsync;
	uint32_t		ntflags;
	uint32_t		psxflags;
	uint32_t		usrflags;
	nt_large_integer	timeout;
	int32_t			fd_status;
	int32_t			wait_status;

	/* socket-specific, mount-specific info */
	union {
		struct {
			uint16_t	domain;
			uint16_t	type;
			uint32_t	protocol;
		} si;

		void *	mi;
		void *	vfd;
		void *	vmount;
	} __u;
} nt_socket;

#endif /* _NT_SOCKET_H_ */
