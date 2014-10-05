/*****************************************************************************/
/*                                                                           */
/*  Wild & Asynchronous I/O Library: implemented for the digital humanities  */
/*                                                                           */
/*  Copyright (C) 2014 Zvi Gilboa                                            */
/*                                                                           */
/*  This program is free software: you can redistribute it and/or modify     */
/*  it under the terms of the GNU General Public License as published by     */
/*  the Free Software Foundation, either version 2 of the License, or        */
/*  (at your option) any later version.                                      */
/*                                                                           */
/*  This program is distributed in the hope that it will be useful,          */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/*  GNU General Public License for more details.                             */
/*                                                                           */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*                                                                           */
/*****************************************************************************/


#ifndef _WAIO_IMPL_H_
#define _WAIO_IMPL_H_

#include <waio/waio__llapi.h>
#include "waio_xvtbls.h"

/* annotation */
#define waio_internal_api

/* forward declarations */
typedef struct _ntapi_vtbl	ntapi_vtbl;
typedef struct _ntcon_vtbl	ntcon_vtbl;
typedef struct _winapi_vtbl	winapi_vtbl;


/* accessors */
typedef struct _waio_xvtbls {
	void *			hntdll;
	void *			hkernel32;
	ntapi_vtbl *		ntapi;
	ntcon_vtbl *		ntcon;
	winapi_vtbl *		winapi;
} waio_xvtbls;

/* internal accessors to external function */
extern waio_xvtbls *	__waio_xvtbls;

/* shortcuts to accessor pointers */
#define	__hntdll	__waio_xvtbls->hntdll
#define	__hkernel32	__waio_xvtbls->hkernel32

#define __ntapi		__waio_xvtbls->ntapi
#define __ntcon		__waio_xvtbls->ntcon
#define __winapi	__waio_xvtbls->winapi

/* library */
int __stdcall waio_lib_entry_point(void *, uint32_t, void *);

/* init */
waio_internal_api int32_t __stdcall waio_xvtbls_init_once(void);
waio_internal_api int32_t __stdcall waio_xvtbls_init(waio_xvtbls *);
waio_internal_api int32_t __stdcall ntapi_init(ntapi_vtbl *);
waio_internal_api int32_t __stdcall ntcon_init(ntcon_vtbl *);
waio_internal_api int32_t __stdcall winapi_init(winapi_vtbl *);
waio_internal_api int32_t __stdcall ntapi_detect_wine_behavior(ntapi_vtbl *);

/* loop */
waio_internal_api int32_t __stdcall waio_loop_entry_point(waio *);
waio_internal_api waio_fn waio_thread_shutdown_fallback;

/* io */
waio_internal_api int32_t __stdcall waio_io_entry_point(waio *);
waio_internal_api waio_fn waio_thread_shutdown_response;

#endif /* _WAIO_IMPL_H_ */
