#ifndef _WAIO_H_
#define _WAIO_H_

/*****************************************************************************/
/* the below interfaces come to provide asynchronous i/o on the target       */
/* platform in a way that is independent of the mode in which a file         */
/* handle in general, and a pipe handle in particular, had been opened.      */
/*                                                                           */
/* the high-level waio interfaces follow the semantics of posix aio as       */
/* closely as possible with respect to behaviour and return values, yet      */
/* cannot adhere to them completely due to the differences between file      */
/* descriptors and file or socket handles, as well as the lack of support    */
/* of signals within the current library.  in this last respect, however,    */
/* a similar functionality is available via the aio_hevent member of the     */
/* waio_aiocb structure.                                                     */
/*                                                                           */
/* the forthcoming midipix development framework offers a full-fledged       */
/* asynchronous i/o through a standard <aio.h> header, and accordingly       */
/* does not require direct use of the waio library surrogate functions.      */
/*                                                                           */
/*****************************************************************************/

/* waio_api */
#if   defined (__WAIO_BUILD__)
#define	waio_api	__attr_export__
#elif defined (__WAIO_SHARED__)
#define	waio_api	__attr_import__
#elif defined (__WAIO_STATIC__)
#define	waio_api
#else
#define	waio_api
#endif

/* size_t and friends */
#include "waio__stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* status codes */
typedef enum waio_status_enum {
	WAIO_CANCELED,
	WAIO_NOTCANCELED,
	WAIO_ALLDONE
} waio_status;

/* op codes */
typedef enum waio_opcode_enum {
	WAIO_READ,
	WAIO_WRITE,
	WAIO_NOP
} waio_opcode;

/* modes */
typedef enum waio_wait_mode_enum {
	WAIO_WAIT,
	WAIO_NOWAIT
} waio_wait_mode;

/* context poll flags */
#define WAIO_POLLIN		0x0001
#define WAIO_POLLPRI		0x0002
#define WAIO_POLLOUT		0x0004
#define WAIO_POLLERR		0x0008
#define WAIO_POLLHUP		0x0010
#define WAIO_POLLNVAL		0x0020
#define WAIO_POLLRDNORM		0x0040
#define WAIO_POLLRDBAND		0x0080
#define WAIO_POLLWRNORM		0x0100
#define WAIO_POLLWRBAND		0x0200
#define WAIO_POLLMSG		0x0400
#define WAIO_POLLRDHUP		0x2000

/* request */
struct waio_aiocb {
	int			aio_fildes;
	int			aio_reserved;
	int			aio_lio_opcode;
	int			aio_reqprio;
	void *			aio_hevent;
	volatile void *		aio_buf;
	size_t			aio_nbytes;
	off_t           	aio_offset;
	void *			__opaque[16];
};

/* opaque context handle */
typedef struct waio_cx_interface * waio_cx;

/* context poll request */
struct waio_poll_cx {
	waio_cx		cx;
	short		events;
	short		revents;
};


/* function prototypes: library specific */
/* waio_alloc(): associate a native file handle with an opaque waio context */
waio_cx	waio_alloc(
	void * 		handle,	  /* in          */
	unsigned int	flags,	  /* in, ignored */
	void * 		options); /* in, ignores */

/* waio_free(): free a waio context after cancelling all pending i/o operations */
int	waio_free	(waio_cx);

/* waio_poll(): poll assorted file handles through their associated waio contexts */

/* function prototypes: aio-like interfaces */
int	waio_read	(waio_cx, struct waio_aiocb *);
int	waio_write	(waio_cx, struct waio_aiocb *);
int	waio_error	(waio_cx, const struct waio_aiocb *);
int	waio_cancel	(waio_cx, int, struct waio_aiocb *);
ssize_t	waio_return	(waio_cx, struct waio_aiocb *);



#ifdef __cplusplus
}
#endif

#endif /* _WAIO_H_ */