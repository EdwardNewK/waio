#ifndef _WAIO__LLAPI_H_
#define _WAIO__LLAPI_H_

/* low-level interfaces */
#if !defined(__WAIO_INTERNAL) \
	&& !defined(__MIDIPIX_INTERNAL) \
	&& !defined(MIDIPIX_STANDALONE) \
	&& !defined(MIDIPIX_WIN32) \
	&& !defined(MIDIPIX_WIN64)
	/* framework mismatch */
	#error __^@^__: incorrect framework for <waio/waio__llapi.h>: please include <waio/waio.h> instead.
#endif

/* clerical duties */
#include "waio__common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* forward declarations */
typedef struct waio_interface		waio;
typedef struct waio_vtbl_interface	waio_vtbl;
typedef struct waio_slot_interface	waio_slot;
typedef struct waio_request_interface	waio_request;
typedef struct waio_aiocb		waio_aiocbs;

/* i/o type */
typedef enum {
	WAIO_IO_READ,
	WAIO_IO_WRITE,
	WAIO_IO_NOP
} waio_io_type;

/* packet */
typedef struct waio_packet_interface {
	os_iosb			iosb;
	os_unsigned_ptr *	data;
	os_unsigned_ptr		buffer_size;
	os_large_integer	offset;
	struct waio_aiocb *	aiocb;
} waio_packet;

/* hooks */
typedef enum {
	WAIO_HOOK_BEFORE_INIT,
	WAIO_HOOK_AFTER_INIT,
	WAIO_HOOK_BEFORE_CANCEL,
	WAIO_HOOK_AFTER_CANCEL,
	WAIO_HOOK_BEFORE_SHUTDOWN_REQUEST,
	WAIO_HOOK_AFTER_SHUTDOWN_REQUEST,
	WAIO_HOOK_BEFORE_SHUTDOWN_FALLBACK,
	WAIO_HOOK_AFTER_SHUTDOWN_FALLBACK,
	WAIO_HOOK_BEFORE_SHUTDOWN_RESPONSE,
	WAIO_HOOK_AFTER_SHUTDOWN_RESPONSE,
	WAIO_HOOK_BEFORE_IO_REQUEST,
	WAIO_HOOK_AFTER_IO_REQUEST,
	WAIO_HOOK_BEFORE_IO,
	WAIO_HOOK_AFTER_IO,
	WAIO_HOOK_BEFORE_IO_COMPLETE,
	WAIO_HOOK_AFTER_IO_COMPLETE,
	WAIO_HOOK_ON_TIMEOUT,
	WAIO_HOOK_ON_CANCEL,
	WAIO_HOOK_ON_CANCEL_REQUEST,
	WAIO_HOOK_ON_FAILURE,
	WAIO_HOOK_ON_QUERY,
	WAIO_HOOK_CAP
} waio_hook_type;

typedef signed int __waio_call_conv__hook waio_hook(
	waio *		paio,
	waio_hook_type	type,
	signed int	status);


/* coordination: use notification events also for data request/received */
/* notification: that is, not synchronization, following OS semantics   */
typedef struct waio_interface {
	waio_vtbl *	vtbl;
	waio *		self;
	waio_hook *	hooks[WAIO_HOOK_CAP];
	waio_io_type	type;			/* read/write */
	unsigned int	flags;			/* todo: doc  */
	void *		options;		/* future use or app-defined */
	void *		hfile;			/* os handle    */
	void *		hevent_loop_ready;	/* notification */
	void *		hevent_io_ready;	/* notification */
	void *		hevent_io_request;	/* notification */
	void *		hevent_io_complete;	/* notification */
	void *		hevent_abort_request;	/* notification */
	void *		hevent_cancel_request;	/* notification */
	void *		hevent_queue_request;	/* notification */
	intptr_t	abort_req_counter;	/* optimization */
	intptr_t	abort_inc_counter;	/* optimization */
	intptr_t	cancel_req_counter;	/* optimization */
	intptr_t	cancel_inc_counter;	/* optimization */
	intptr_t	queue_req_counter;	/* optimization */
	intptr_t	queue_inc_counter;	/* optimization */
	intptr_t	io_counter;		/* optimization */
	void *		hthread_io;		/* the blocking thread */
	void *		hthread_loop;		/* (app) */
	void *		hevent_info;		/* (app, optional) */
	signed int	info_type;		/* (app, optional) */
	signed int	info_tip;		/* (app, optional) */
	os_timeout	io_request_timeout;	/* (app, optional) */
	signed int	status_loop;		/* loop thread: last os status */
	signed int	status_io;		/* io thread:   last os status */
	void *		fallback_tip;		/* internal use */
	void *		context_init;		/* internal use */
	void *		context_loop;		/* internal use */
	void *		context_io;		/* internal use */
	waio_packet	lpacket;		/* internal request info        */
	waio_packet	lcancel_io;		/* internal cencellation status */
	waio_packet *	packet;			/* aio single request        */
	waio_packet *	cancel_io;		/* aio cancellation request  */
	waio_request *	queue;			/* request serialization */
	waio_request *	qtail;			/* request serialization */
	waio_request *	qfree;			/* request serialization */
	waio_slot *	slots;			/* request serialization */
	signed int	cpu_count;		/* serialization helper  */
	signed int	favorite_slot_index;	/* serialization helper  */
} waio;

/* waio default function signature */
typedef signed int __waio_call_conv__api waio_fn(waio *);

/* library */
waio_api waio_fn waio_init;

/* loop thread */
waio_api waio_fn waio_create;
waio_api waio_fn waio_loop;
waio_api waio_fn waio_enqueue;
waio_api waio_fn waio_dequeue;
waio_api waio_fn waio_cancel_current_request;
waio_api waio_fn waio_thread_shutdown_request;

/* io thread */
waio_api waio_fn waio_io;

/* hooks */
waio_hook waio_hook_default;

/* client */
waio_api int32_t waio_submit_single_request(
	_in_	waio *			paio,
	_in_	struct waio_aiocb *	aiocb,
	_in_	int			lio_opcode,
	_in_	uint32_t		pid,
	_in_	uint32_t		tid);


#ifdef __cplusplus
}
#endif

#endif /* _WAIO__LLAPI_H_ */
