#ifndef _NT_SYNC_H_
#define _NT_SYNC_H_

#include <psxtypes/psxtypes.h>
#include "nt_object.h"

typedef enum _nt_wait_type {
	NT_WAIT_ALL,
	NT_WAIT_ANY
} nt_wait_type;


typedef enum _nt_event_type {
	NT_NOTIFICATION_EVENT,
	NT_SYNCHRONIZATION_EVENT
} nt_event_type;


typedef enum _nt_event_states {
	NT_EVENT_NOT_SIGNALED,
	NT_EVENT_SIGNALED
} nt_event_states;


typedef enum _nt_event_info_class {
	NT_EVENT_BASIC_INFORMATION
} nt_event_info_class;


/* event access bits */
#define NT_EVENT_QUERY_STATE		0x00000001U
#define NT_EVENT_MODIFY_STATE		0x00000002U
#define NT_EVENT_ALL_ACCESS 		0x001F0003U


/* alertable threads */
#define NT_SYNC_NON_ALERTABLE		0x00000000U


typedef struct _nt_event_basic_information {
	nt_event_type		event_time;
	int32_t			signal_state;
} nt_event_basic_information;


typedef int32_t __stdcall ntapi_zw_wait_for_single_object(
	_in_	void *		handle,
	_in_ 	int32_t		alertable,
	_in_	nt_timeout *	timeout	_optional_);


typedef int32_t __stdcall ntapi_zw_wait_for_multiple_objects(
	_in_	uint32_t	handle_count,
	_in_	void **		handles,
	_in_	nt_wait_type	wait_type,
	_in_ 	int32_t		alertable,
	_in_	nt_timeout *	timeout	_optional_);


typedef int32_t __stdcall ntapi_zw_create_event(
	_out_	void **			hevent,
	_in_ 	uint32_t 		desired_access,
	_in_ 	nt_object_attributes * 	obj_attr,
	_in_ 	nt_event_type		event_type,
	_in_ 	int32_t			initial_state);


typedef int32_t __stdcall ntapi_zw_open_event(
	_out_	void **			hevent,
	_in_ 	uint32_t 		desired_access,
	_in_ 	nt_object_attributes * 	obj_attr);


typedef int32_t __stdcall ntapi_zw_set_event(
	_in_	void *		hevent,
	_out_	int32_t *	previous_state);


typedef int32_t __stdcall ntapi_zw_reset_event(
	_in_	void *		hevent,
	_out_	int32_t *	previous_state);


typedef int32_t __stdcall ntapi_zw_clear_event(
	_in_	void *		hevent);


typedef int32_t __stdcall ntapi_zw_query_event(
	_in_	void *			hevent,
	_in_	nt_event_info_class	event_info_class,
	_out_	void *			event_info,
	_in_	size_t			event_info_length,
	_out_	size_t *		returned_length		_optional_);


/* 'tedious task' extension functions */
typedef int32_t __stdcall ntapi_tt_create_inheritable_event(
	_out_	void **		hevent,
	_in_ 	nt_event_type	event_type,
	_in_ 	int32_t		initial_state);


typedef int32_t __stdcall ntapi_tt_create_private_event(
	_out_	void **		hevent,
	_in_ 	nt_event_type	event_type,
	_in_ 	int32_t		initial_state);

#endif /* _NT_SYNC_H_ */
