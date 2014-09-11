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

waio_api
int32_t __stdcall waio_loop(waio * paio)
{
	void *		hwait[5];
	nt_timeout *	timeout;
	int a = 0;

	/* notify the init routine that the loop is ready */
	paio->status_loop = __ntapi->zw_set_event(
		paio->hevent_loop_ready,
		(int32_t *)0);

	if (paio->status_loop) return paio->status_loop;

	/* prepare for the waits */
	hwait[0] = paio->hevent_abort_request;
	hwait[1] = paio->hevent_cancel_request;
	hwait[2] = paio->hevent_queue_request;
	hwait[3] = paio->hevent_io_complete;
	hwait[4] = paio->hthread_io;

	do {
		if ((!paio->abort_inc_counter) \
				&& (!paio->cancel_inc_counter) \
				&& (!paio->queue_counter) \
				&& (!paio->data_counter)) {
			if (paio->io_request_timeout.quad)
				timeout = &paio->io_request_timeout;
			else
				timeout = (nt_timeout *)0;

			/* wait */
			paio->status_loop = __ntapi->zw_wait_for_multiple_objects(
				4,
				hwait,
				__ntapi->wait_type_any,
				NT_SYNC_NON_ALERTABLE,
				timeout);
		}

		/* hook: on query */
		paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x66666666,(signed int)paio->queue_counter);

		/* timeout? */
		if (paio->status_loop == NT_STATUS_TIMEOUT)
			paio->hooks[WAIO_HOOK_ON_TIMEOUT](paio,WAIO_HOOK_ON_TIMEOUT,paio->status_loop);

		/* abort request? */
		if (paio->abort_inc_counter > paio->abort_req_counter)
			waio_thread_shutdown_request(paio);

		/* cancel request? */
		if (paio->cancel_inc_counter > paio->cancel_req_counter)
			paio->hooks[WAIO_HOOK_ON_CANCEL_REQUEST](paio,WAIO_HOOK_ON_CANCEL_REQUEST,paio->status_loop);

		/* io call completed? */
		if (paio->data_counter) {
			/* hook: after io complete */
			paio->hooks[WAIO_HOOK_AFTER_IO_COMPLETE](paio,WAIO_HOOK_AFTER_IO_COMPLETE,0);

			/* regress the data counter */
			at_locked_dec(&paio->data_counter);
		}

		/* queue request? */
		if (paio->queue_counter) {
			/* process current request */
			paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x55555555,++a);
			waio_enqueue(paio);
		}

		/* io thread died? */
		/* TODO: ?? */
		/* 	return NT_STATUS_THREAD_NOT_IN_PROCESS; */

		/* submit the next io request if applicable */
		waio_dequeue(paio);
	} while (1);

	/* should never get here */
	return NT_STATUS_INTERNAL_ERROR;
}


waio_internal_api
int32_t __stdcall waio_loop_entry_point(waio * paio)
{
	int32_t status;

	/* enter loop */
	status = waio_loop(paio);

	/* hook: on failure */
	if (status)
		paio->hooks[WAIO_HOOK_ON_FAILURE](paio,WAIO_HOOK_ON_FAILURE,status);

	waio_thread_shutdown_request(paio);

	/* terminate thread cleanly */
	__ntapi->zw_terminate_thread(
		NT_CURRENT_THREAD_HANDLE,
		status);

	/* should never get here */
	return NT_STATUS_INTERNAL_ERROR;
}
