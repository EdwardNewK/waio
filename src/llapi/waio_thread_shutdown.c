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

/* the thread shutdown response executes in the context of the io thread */
waio_internal_api
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
	int32_t				status_cancel_io_ex;
	nt_timeout			timeout;

	/* hook: before shutdown_request */
	paio->hooks[WAIO_HOOK_BEFORE_SHUTDOWN_REQUEST](paio,WAIO_HOOK_BEFORE_SHUTDOWN_REQUEST,0);

	/* use fallback method if not supported */
	if (!__ntapi->zw_cancel_io_file_ex || __ntapi->wine_get_version)
		return waio_thread_shutdown_fallback(paio);

	/* hook: before shutdown_request */
	paio->hooks[WAIO_HOOK_BEFORE_SHUTDOWN_REQUEST](paio,WAIO_HOOK_BEFORE_SHUTDOWN_REQUEST,0);

	/* cancel io request */
	status = __ntapi->zw_cancel_io_file_ex(
		paio->hfile,
		&paio->packet->iosb,
		&paio->packet->iosb);

	/* hook: before shutdown_request */
	paio->hooks[WAIO_HOOK_ON_QUERY](paio,WAIO_HOOK_ON_QUERY,status);

	/* NOT_FOUND means the io thread was not blocking, slight chance of race */
	if (status == NT_STATUS_NOT_FOUND) {
		/* hook: before shutdown_request */
		paio->hooks[WAIO_HOOK_ON_QUERY](paio,WAIO_HOOK_ON_QUERY,status);

		timeout.quad = (-1) * 10 * 1000;
		do {
			/* wait for the thread for one millisecond */
			status = paio->status_loop = __ntapi->zw_wait_for_single_object(
				paio->hthread_io,
				NT_SYNC_NON_ALERTABLE,
				&timeout);

			/* cancel io request */
			status_cancel_io_ex = __ntapi->zw_cancel_io_file_ex(
				paio->hfile,
				&paio->packet->iosb,
				&paio->packet->iosb);

			paio->hooks[WAIO_HOOK_ON_QUERY](paio,WAIO_HOOK_ON_QUERY,status_cancel_io_ex);
		} while (status);
	} else if (status == 0) {
		paio->hooks[WAIO_HOOK_ON_CANCEL](paio,WAIO_HOOK_ON_CANCEL,0);
		/* the thread was blocking: now wait for it to terminate */
		paio->status_loop = __ntapi->zw_wait_for_single_object(
			paio->hthread_io,
			NT_SYNC_NON_ALERTABLE,
			(nt_timeout *)0);
	} else
		paio->hooks[WAIO_HOOK_ON_FAILURE](paio,WAIO_HOOK_ON_FAILURE,status);

	paio->hooks[WAIO_HOOK_AFTER_SHUTDOWN_RESPONSE](paio,WAIO_HOOK_AFTER_SHUTDOWN_RESPONSE,0);

	/* terminate this thread cleanly */
	__ntapi->zw_terminate_thread(
		NT_CURRENT_THREAD_HANDLE,
		NT_STATUS_REQUEST_ABORTED);

	/* should never get here */
	return NT_STATUS_INTERNAL_ERROR;
}
