#include <psxtypes/psxtypes.h>
#include <ntapi/nt_object.h>

typedef uint32_t __stdcall winapi_get_current_thread_id(void);

typedef uint32_t __stdcall winapi_get_current_process_id(void);

typedef void * __stdcall winapi_get_module_handle_ansi(
	_in_	char *		module_name);

typedef void * __stdcall winapi_get_module_handle_utf16(
	_in_	wchar16_t *	module_name);

typedef void * __stdcall winapi_get_proc_address(
	_in_	void *	image_base,
	_in_	char *	symbol_name);

typedef void * __stdcall winapi_create_thread(
	_in_	nt_sa *				thread_attr	_optional_,
	_in_	size_t				stack_size,
	_in_	nt_thread_start_routine *	thread_start_routine,
	_in_	void *				params,
	_in_	uint32_t			creation_flags,
	_out_	uint32_t *			tid		_optional_);

/* a pipe created by kernel32's CreatePipe is always a blop (blocking pipe) */
typedef int32_t __stdcall winapi_create_pipe(
	_out_	void *		hread,
	_out_	void *		hwrite,
	_in_	nt_sa *		pipe_attr	_optional_,
	_in_	uint32_t	advisory_size);
