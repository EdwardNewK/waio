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


#include <psxtypes/psxtypes.h>
#include <ntapi/ntapi.h>
#include <waio/waio__llapi.h>
#include "waio_impl.h"

/* cancel all io operations on a file within the current process */
typedef int32_t __stdcall ntapi_zw_cancel_io_file_ex(
	_in_	void *		hfile,
	_in_	nt_iosb *	iosb_in		_optional_,
	_out_	nt_iosb *	iosb_out);


/* the thread shutdown response executes in the context of the io thread */
waio_api
int32_t __stdcall waio_thread_shutdown_response(waio * paio)
{
	/* hook: before shutdown_response */
	paio->hooks[WAIO_HOOK_BEFORE_SHUTDOWN_RESPONSE](paio,WAIO_HOOK_BEFORE_SHUTDOWN_RESPONSE,0);

	/* terminate thread cleanly */
	__ntapi->zw_terminate_thread(
		NT_CURRENT_THREAD_HANDLE,
		NT_STATUS_REQUEST_ABORTED);

	/* should never get here */
	return NT_STATUS_INTERNAL_ERROR;
}


/* the thread shutdown request executes in the context of the loop thread */
waio_api
int32_t __stdcall waio_thread_shutdown_request(waio * paio)
{
	int32_t				status;
	ntapi_zw_cancel_io_file_ex *	pfn_zw_cancel_io_file_ex;
	nt_timeout			timeout;

	/* hook: before shutdown_request */
	paio->hooks[WAIO_HOOK_BEFORE_SHUTDOWN_REQUEST](paio,WAIO_HOOK_BEFORE_SHUTDOWN_REQUEST,0);

	/* see whether zw_cancel_io_file_ex is available (vista->, wine); */
	pfn_zw_cancel_io_file_ex = (ntapi_zw_cancel_io_file_ex *)
				__winapi->get_proc_address(
					__hntdll,
					"ZwCancelIoFileEx");

	/* use fallback method if not */
	if (!pfn_zw_cancel_io_file_ex)
		return waio_thread_shutdown_fallback(paio);

	/* cancel io request */
	status = pfn_zw_cancel_io_file_ex(
		paio->hread,
		&paio->packet.iosb,
		&paio->cancel_io.iosb);

	/* NOT_FOUND means the io thread was not blocking, slight chance of race */
	if (status == NT_STATUS_NOT_FOUND) {
		timeout.quad = (-1) * 10 * 1000;
		do {
			/* wait for the thread for one millisecond */
			status = paio->status_loop = __ntapi->zw_wait_for_single_object(
				paio->hthread_io,
				NT_SYNC_NON_ALERTABLE,
				&timeout);

			/* cancel io request */
			pfn_zw_cancel_io_file_ex(
				paio->hread,
				&paio->packet.iosb,
				&paio->cancel_io.iosb);
		} while (status);
	} else {
		/* the thread was blocking: now wait for it to terminate */
		paio->status_loop = __ntapi->zw_wait_for_single_object(
			paio->hthread_io,
			NT_SYNC_NON_ALERTABLE,
			(nt_timeout *)0);
	}

	/* terminate this thread cleanly */
	__ntapi->zw_terminate_thread(
		NT_CURRENT_THREAD_HANDLE,
		NT_STATUS_REQUEST_ABORTED);

	/* should never get here */
	return NT_STATUS_INTERNAL_ERROR;
}
