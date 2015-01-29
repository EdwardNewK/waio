#ifndef _NT_THREAD_H_
#define _NT_THREAD_H_

#include <psxtypes/psxtypes.h>
#include "nt_object.h"
#include "bits/i386/nt_thread_i386.h"
#include "bits/x86_64/nt_thread_x86_64.h"

typedef enum _nt_thread_info_class {
	NT_THREAD_BASIC_INFORMATION,
	NT_THREAD_TIMES,
	NT_THREAD_PRIORITY,
	NT_THREAD_BASE_PRIORITY,
	NT_THREAD_AFFINITY_MASK,
	NT_THREAD_IMPERSONATION_TOKEN,
	NT_THREAD_DESCRIPTOR_TABLE_ENTRY,
	NT_THREAD_ENABLE_ALIGNMENT_FAULT_FIXUP,
	NT_THREAD_EVENT_PAIR,
	NT_THREAD_QUERY_SET_WIN32_START_ADDRESS,
	NT_THREAD_ZERO_TLS_CELL,
	NT_THREAD_PERFORMANCE_COUNT,
	NT_THREAD_AM_I_LASTNT_THREAD,
	NT_THREAD_IDEAL_PROCESSOR,
	NT_THREAD_PRIORITY_BOOST,
	NT_THREAD_SET_TLS_ARRAY_ADDRESS,
	NT_THREAD_IS_IO_PENDING,
	NT_THREAD_HIDE_FROM_DEBUGGER
} nt_thread_info_class;

typedef enum _nt_exception_disposition {
	NT_EXCEPTION_CONTINUE_EXECUTION,
	NT_EXCEPTION_CONTINUE_SEARCH,
	NT_EXCEPTION_NESTED_EXCEPTION,
	NT_EXCEPTION_COLLIDED_UNWIND
} nt_exception_disposition;


/* special handles */
#define NT_CURRENT_THREAD_HANDLE (void *)(uintptr_t)-2

/* thread access bits */
#define NT_THREAD_TERMINATE			0x00000001
#define NT_THREAD_SUSPEND_RESUME		0x00000002
#define NT_THREAD_ALERT				0x00000004 /* fits right in the middle... */
#define NT_THREAD_GET_CONTEXT			0x00000008
#define NT_THREAD_SET_CONTEXT			0x00000010
#define NT_THREAD_SET_INFORMATION		0x00000020
#define NT_THREAD_QUERY_INFORMATION		0x00000040
#define NT_THREAD_SET_THREAD_TOKEN		0x00000080
#define NT_THREAD_IMPERSONATE			0x00000100
#define NT_THREAD_DIRECT_IMPERSONATION		0x00000200
#define NT_THREAD_SYNCHRONIZE			0x00100000

#define NT_THREAD_ALL_ACCESS			NT_THREAD_TERMINATE \
						| NT_THREAD_SUSPEND_RESUME \
						| NT_THREAD_ALERT \
						| NT_THREAD_GET_CONTEXT \
						| NT_THREAD_SET_CONTEXT \
						| NT_THREAD_SET_INFORMATION \
						| NT_THREAD_QUERY_INFORMATION \
						| NT_THREAD_SET_THREAD_TOKEN \
						| NT_THREAD_IMPERSONATE \
						| NT_THREAD_DIRECT_IMPERSONATION \
						| NT_THREAD_SYNCHRONIZE

/* library-specific thread creation flags */
#define NT_THREAD_RUN_IMMEDIATELY		0x00000000
#define NT_CREATE_SUSPENDED			0x00000004
#define NT_CREATE_FIRST_THREAD_OF_NEW_PROCESS	0x00008000
#define NT_STACK_SIZE_PARAM_IS_A_RESERVATION	0x00800000
#define NT_CLOSE_THREAD_HANDLE			0x01000000


/* thread context */
#define NT_CONTEXT_JUST_EVERYTHING		(intptr_t)-1


/* source mark: arch-specific code: begin */
#if defined(__NT32) && defined (__X86_MODEL)
typedef struct _nt_thread_context_i386			nt_thread_context;
#elif defined(__NT64) && defined (__X86_64_MODEL)
typedef struct _nt_thread_context_x86_64		nt_thread_context;
#endif
/* source mark: arch-specific code: end */


typedef struct _nt_exception_registration_record {
	struct _nt_exception_registration_record *	next;
	nt_exception_disposition *			handler;
} nt_exception_registration_record;


typedef struct _nt_tib {
	nt_exception_registration_record * 	exception_list;
	void * 					stack_base;
	void * 					stack_limit;
	void * 					sub_system_tib;

	union {
		void * 		fiber_data;
		uint32_t 	version;
	};

	void * 					arbitrary_user_pointer;
	struct _nt_tib * 			self;
} nt_tib;


typedef struct _nt_thread_basic_information {
	int32_t		exit_status;
	nt_tib *	teb_base_address;
	nt_client_id	cid;
	intptr_t	affinity_mask;
	int32_t		priority;
	int32_t		base_priority;
} nt_thread_basic_information;


typedef int32_t __stdcall nt_thread_start_routine(void * start_routine_param);


typedef int32_t __stdcall ntapi_zw_terminate_thread(
	_in_	void *	hthread,
	_in_	int32_t	exit_status);


typedef int32_t __stdcall ntapi_zw_query_information_thread(
	_in_	void *			hthread,
	_in_	nt_thread_info_class	thread_info_class,
	_out_	void *			thread_info,
	_in_	size_t			thread_info_length,
	_out_	size_t *		returned_length		_optional_);


typedef int32_t __stdcall ntapi_zw_suspend_thread(
	_in_	void *		hthread,
	_out_	uint32_t *	prev_suspend_count _optional_);

#endif /* _NT_THREAD_H_ */
