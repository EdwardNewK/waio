#ifndef _WAIO__LLAPI_H_
#define _WAIO__LLAPI_H_

#ifdef __cplusplus
extern "C" {
#endif

/* compiler specific */
#include "waio__compiler.h"

/* os specific */
#include "waio__os.h"

/* the necessary ... */
#if   defined (__WAIO_BUILD__)
#define	waio_api	__attr_export__
#elif defined (__WAIO_SHARED__)
#define	waio_api	__attr_import__
#else
#define	waio_api
#endif
/* ... evil */

/* forward declarations */
typedef struct waio_interface		waio;
typedef struct waio_vtbl_interface	waio_vtbl;

/* pipe type */
typedef enum {
	WAIO_READ,
	WAIO_WRITE
} waio_io_type;

/* packet */
typedef struct waio_packet_interface {
	os_signed_ptr		counter;
	os_iosb			iosb;
	os_unsigned_ptr *	data;
} waio_packet;

/* hooks */
typedef enum {
	WAIO_HOOK_BEFORE_INIT,
	WAIO_HOOK_AFTER_INIT,
	WAIO_HOOK_BEFORE_SHUTDOWN_REQUEST,
	WAIO_HOOK_AFTER_SHUTDOWN_REQUEST,
	WAIO_HOOK_BEFORE_SHUTDOWN_RESPONSE,
	WAIO_HOOK_AFTER_SHUTDOWN_RESPONSE,
	WAIO_HOOK_BEFORE_READ_REQUEST,
	WAIO_HOOK_AFTER_READ_REQUEST,
	WAIO_HOOK_BEFORE_READ,
	WAIO_HOOK_AFTER_READ,
	WAIO_HOOK_BEFORE_DATA_RECEIVED,
	WAIO_HOOK_AFTER_DATA_RECEIVED,
	WAIO_HOOK_BEFORE_DATA_PROCESSED,
	WAIO_HOOK_AFTER_DATA_PROCESSED,
	WAIO_HOOK_BEFORE_WRITE_REQUEST,
	WAIO_HOOK_AFTER_WRITE_REQUEST,
	WAIO_HOOK_BEFORE_WRITE,
	WAIO_HOOK_AFTER_WRITE,
	WAIO_HOOK_BEFORE_DATA_WRITTEN,
	WAIO_HOOK_AFTER_DATA_WRITTEN,
	WAIO_HOOK_ON_TIMEOUT,
	WAIO_HOOK_ON_FAILURE,
	WAIO_HOOK_ON_QUERY,
	WAIO_HOOK_CAP
} waio_hook_type;

typedef signed int __waio_call_conv__hook waio_hook(
	waio *		paio,
	waio_hook_type	type,
	signed int		status);


/* coordination: use norification events also for data request/received */
/* notification: that is, not synchronization, following OS semantics   */
typedef struct waio_interface {
	waio_vtbl *	vtbl;
	waio *		self;
	waio_hook *	hooks[WAIO_HOOK_CAP];
	waio_io_type	type;			/* read/write */
	unsigned int	flags;			/* todo: doc  */
	void *		options;		/* future use or app-defined */
	void *		hread;			/* os handle to reading end  */
	void *		hwrite;			/* os handle to writing end  */
	void *		hevent_loop_ready;	/* notification */
	void *		hevent_io_ready;	/* notification */
	void *		hevent_data_request;	/* notification */
	void *		hevent_data_received;	/* notification		[ZwWaitFor] */
	void *		hevent_abort_request;	/* notification		[Multiple]  */
	void *		hthread_io;		/* the blocking thread	[Objects]   */
	void *		hthread_loop;		/* (app) */
	void *		hevent_info;		/* (app, optional) */
	signed int	info_type;		/* (app, optional) */
	signed int	info_tip;		/* (app, optional) */
	os_timeout	read_request_timeout;	/* (app, optional) */
	signed int	status_loop;		/* loop thread: last os status */
	signed int	status_io;		/* io thread:   last os status */
	void *		fallback_tip;		/* internal use */
	void *		context_init;		/* future use or app-defined */
	void *		context_loop;		/* future use or app-defined */
	void *		context_io;		/* future use or app-defined */
	waio_packet	packet;
	waio_packet	cancel_io;
} waio;

/* waio default function signature */
typedef waio_api signed int __waio_call_conv__api waio_fn(waio *);

/* library */
waio_fn waio_init;

/* parent */
waio_fn waio_create;
waio_fn waio_loop;
waio_fn waio_thread_shutdown_request;
waio_fn waio_thread_shutdown_fallback;

/* child */
waio_fn waio_io;
waio_fn waio_thread_shutdown_response;

/* hooks */
waio_hook waio_hook_default;

#ifdef __cplusplus
}
#endif

#endif /* _WAIO__LLAPI_H_ */
