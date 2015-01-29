#ifndef _WAIO__COMMON_H_
#define _WAIO__COMMON_H_

/* compiler specific */
#include "waio__compiler.h"

/* os specific */
#include "waio__os.h"

/* size_t and friends */
#include "waio__stddef.h"

/* the necessary ... */
#if   defined (__WAIO_BUILD__)
#define	waio_api __attr_export__
#elif defined (__WAIO_SHARED__)
#define	waio_api __attr_import__
#elif defined (__WAIO_STATIC__)
#define	waio_api
#else
#define	waio_api
#endif
/* ... evil */

#endif /* _WAIO__COMMON_H_ */
