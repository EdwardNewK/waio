/*****************************************************************************/
/*                                                                           */
/*  Wild & Asynchronous I/O Library: implemented for the digital humanities  */
/*                                                                           */
/*  Copyright (C) 2014 Zvi Gilboa                                            */
/*                                                                           */
/*  This program is free software: you can redistribute it and/or modify     */
/*  it under the terms of the GNU General Public License as published by     */
/*  the Free Software Foundation, either version 3 of the License, or        */
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
typedef struct _WAIO_xvtbls {
	void *			hntdll;
	void *			hkernel32;
	ntapi_vtbl *		ntapi;
	ntcon_vtbl *		ntcon;
	winapi_vtbl *		winapi;
} waio_xvtbls;

/* internal accessors to external function */
extern waio_xvtbls *		__WAIO_xvtbls;

/* shortcuts to accessor pointers */
#define	__hntdll	__WAIO_xvtbls->hntdll
#define	__hkernel32	__WAIO_xvtbls->hkernel32

#define __ntapi		__WAIO_xvtbls->ntapi
#define __ntcon		__WAIO_xvtbls->ntcon
#define __winapi	__WAIO_xvtbls->winapi

/* library */
/* TODO: waio_lib_entry_point(void); */

/* init */
waio_internal_api int32_t __stdcall waio_xvtbls_init(waio_xvtbls *);
waio_internal_api int32_t __stdcall ntapi_init(ntapi_vtbl *);
waio_internal_api int32_t __stdcall ntcon_init(ntcon_vtbl *);
waio_internal_api int32_t __stdcall winapi_init(winapi_vtbl *);

/* loop */
waio_internal_api int32_t __stdcall waio_loop_entry_point(waio *);

/* io */
waio_internal_api int32_t __stdcall waio_io_entry_point(waio *);

#endif /* _WAIO_IMPL_H_ */
