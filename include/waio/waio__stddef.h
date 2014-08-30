#ifndef _WAIO__STDDEF_H_
#define _WAIO__STDDEF_H_

#if   defined(__WAIO_INTERNAL)
#include <psxtypes/psxtypes.h>

#elif defined (__MIDIPIX_INTERNAL)
#include <psxtypes/psxtypes.h>

#elif defined (MIDIPIX_STANDALONE)
#include <psxtypes/psxtypes.h>

#else
#include <stddef.h>
#include <stdio.h>

#endif

#endif /* _WAIO__STDDEF_H_ */
