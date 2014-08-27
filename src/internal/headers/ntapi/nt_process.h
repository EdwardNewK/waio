#ifndef _NT_PROCESS_H_
#define _NT_PROCESS_H_

#include <psxtypes/psxtypes.h>

/* special handles */
#define NT_CURRENT_PROCESS_HANDLE (void *)(uintptr_t)-1

/* process-related function signatures */
typedef int32_t	__stdcall ntapi_zw_terminate_process(
	_in_	void *		hprocess	_optional_,
	_in_	int32_t		status);

#endif /* _NT_PROCESS_H_ */
