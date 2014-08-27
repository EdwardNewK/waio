#ifndef _NTAPI_H_
#define _NTAPI_H_

#include <psxtypes/psxtypes.h>
#include "nt_status.h"
#include "nt_object.h"
#include "nt_memory.h"
#include "nt_thread.h"
#include "nt_process.h"
#include "nt_sync.h"
#include "nt_file.h"
#include "nt_string.h"

/* extensions */
ntapi_tt_create_inheritable_event	__ntapi_tt_create_inheritable_event;
ntapi_tt_create_private_event		__ntapi_tt_create_private_event;

#endif /* _NTAPI_H_ */
