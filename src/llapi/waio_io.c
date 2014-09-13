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

static int32_t	__stdcall __io_nop(
	_in_	void *			hfile,
	_in_	void *			hevent		_optional_,
	_in_	void *			apc_routine	_optional_,
	_in_	void *			apc_context	_optional_,
	_out_	nt_io_status_block *	io_status_block,
	_out_	void *			buffer,
	_in_	uint32_t		bytes_to_read,
	_in_	nt_large_integer *	byte_offset	_optional_,
	_in_	uint32_t *		key		_optional_)
{
	return 0;
}


waio_api
int32_t __stdcall waio_io(waio * paio)
{
	int32_t			state;
	void *			hpending[2];
	void *			hwait[2];
	ntapi_zw_read_file *	io_routine[3];
	nt_timeout *		timeout;

	/* fallback tip for legacy os versions */
	paio->fallback_tip = &state;

	/* hfile may be non-blocking */
	paio->status_io = __ntapi->tt_create_private_event(
		&hpending[0],
		NT_NOTIFICATION_EVENT,
		NT_EVENT_NOT_SIGNALED);

	if (paio->status_io) return paio->status_io;

	/* notify the app that the io thread is ready */
	paio->status_io = __ntapi->zw_set_event(
		paio->hevent_io_ready,
		&state);

	if (paio->status_io) return paio->status_io;

	/* events of interest */
	hwait[0]    = paio->hevent_io_request;
	hwait[1]    = paio->hevent_abort_request;
	hpending[1] = paio->hevent_abort_request;

	/* io routines */
	io_routine[0] = __ntapi->zw_read_file;
	io_routine[1] = __ntapi->zw_write_file;
	io_routine[2] = __io_nop;

	do {
		if (paio->io_request_timeout.quad)
			timeout = &paio->io_request_timeout;
		else
			timeout = (nt_timeout *)0;

		if (!paio->io_counter)
			paio->status_io = __ntapi->zw_wait_for_multiple_objects(
				2,
				hwait,
				__ntapi->wait_type_any,
				NT_SYNC_NON_ALERTABLE,
				timeout);

		if (paio->status_io == NT_STATUS_TIMEOUT)
			paio->hooks[WAIO_HOOK_ON_TIMEOUT](paio,WAIO_HOOK_ON_TIMEOUT,0);

		/* abort request? */
		if (paio->abort_counter) {
			__ntapi->zw_close(hpending[0]);
			waio_thread_shutdown_response(paio);
		}

		if ((uint32_t)paio->status_io >= NT_STATUS_WAIT_CAP)
			return paio->status_io;

		/* hook: after io request */
		paio->hooks[WAIO_HOOK_AFTER_IO_REQUEST](paio,WAIO_HOOK_AFTER_IO_REQUEST,paio->status_io);

		/* hook: before io */
		paio->hooks[WAIO_HOOK_BEFORE_IO](paio,WAIO_HOOK_BEFORE_IO,0);

		/* io system call */
		if (paio->packet)
			paio->status_io = io_routine[paio->type](
				paio->hfile,
				hpending[0],
				(void *)0,
				(void *)0,
				&paio->packet->iosb,
				paio->packet->data,
				(uint32_t)paio->packet->buffer_size,
				&paio->packet->offset,
				(uint32_t *)0);
		else
			waio_thread_shutdown_response(paio);

		/* hfile may be non-blocking */
		if (paio->status_io == NT_STATUS_PENDING) {
			paio->status_io = __ntapi->zw_wait_for_multiple_objects(
				2,
				hpending,
				__ntapi->wait_type_any,
				NT_SYNC_NON_ALERTABLE,
				(nt_timeout *)0);

			/* abort request? */
			if (paio->abort_counter) {
				__ntapi->zw_close(hpending[0]);
				waio_thread_shutdown_response(paio);
			}
		}

		/* hook: after io */
		paio->hooks[WAIO_HOOK_AFTER_IO](paio,WAIO_HOOK_AFTER_IO,0);

		/* advance the data counter and notify the loop thread */
		at_locked_inc(&paio->data_counter);

		/* hook: before io complete */
		paio->hooks[WAIO_HOOK_BEFORE_IO_COMPLETE](paio,WAIO_HOOK_BEFORE_IO_COMPLETE,paio->status_io);

		paio->status_io = __ntapi->zw_set_event(
			paio->hevent_queue_request,
			&state);

		if (paio->status_io) return paio->status_io;

		paio->status_io = __ntapi->zw_wait_for_single_object(
			paio->hevent_io_complete,
			NT_SYNC_NON_ALERTABLE,
			(nt_timeout *)0);

		if (paio->status_io) return paio->status_io;

		paio->status_io = __ntapi->zw_reset_event(
			paio->hevent_io_complete,
			(int32_t *)0);

		if (paio->status_io) return paio->status_io;
	} while (1);

	/* should never get here */
	return NT_STATUS_INTERNAL_ERROR;
}


/* io thread: internal loop, might block, unblocked by the loop thread */
waio_internal_api
int32_t __stdcall waio_io_entry_point(waio * paio)
{
	int32_t status;

	/* enter loop */
	status = waio_io(paio);

	/* hook: on cancel / on failure */
	if (status == NT_STATUS_CANCELLED)
		paio->hooks[WAIO_HOOK_ON_CANCEL](paio,WAIO_HOOK_ON_CANCEL,status);
	else if (status)
		paio->hooks[WAIO_HOOK_ON_FAILURE](paio,WAIO_HOOK_ON_FAILURE,status);

	/* terminate thread cleanly */
	__ntapi->zw_terminate_thread(
		NT_CURRENT_THREAD_HANDLE,
		status);

	/* should never get here */
	return NT_STATUS_INTERNAL_ERROR;
}
