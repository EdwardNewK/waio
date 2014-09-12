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

	paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x33330000,paio->status_io);

	if (paio->status_io) return paio->status_io;

	/* notify the app that the io thread is ready */
	paio->status_io = __ntapi->zw_set_event(
		paio->hevent_io_ready,
		&state);

	paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x33330001,paio->status_io);

	if (paio->status_io) return paio->status_io;

	/* events of interest */
	hwait[0]    = paio->hevent_io_request;
	hwait[1]    = paio->hevent_abort_request;
	hpending[1] = paio->hevent_abort_request;

	/* io routines */
	io_routine[0] = __ntapi->zw_read_file;
	io_routine[1] = __ntapi->zw_write_file;
	io_routine[2] = __io_nop;

	paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x12340000,0);

	do {
		paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x1234000E,paio->status_io);
		/* wait for an abort request to arrive;	*/
		/* or for io op to be requested;	*/
		if (paio->io_request_timeout.quad)
			timeout = &paio->io_request_timeout;
		else
			timeout = (nt_timeout *)0;

		paio->status_io = __ntapi->zw_wait_for_multiple_objects(
			2,
			hwait,
			__ntapi->wait_type_any,
			NT_SYNC_NON_ALERTABLE,
			timeout);

		paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x1234000F,paio->status_io);

		if (paio->status_io == NT_STATUS_TIMEOUT) {
			/* hook: on query */
			paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x88888888,paio->status_io);

			paio->hooks[WAIO_HOOK_ON_TIMEOUT](paio,WAIO_HOOK_ON_TIMEOUT,0);
		}

		/* hook: on query */
		paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x12340001,paio->status_io);

		/* abort request? */
		if (paio->abort_counter) {
			__ntapi->zw_close(hpending[0]);
			waio_thread_shutdown_response(paio);
		}

		if ((uint32_t)paio->status_io >= NT_STATUS_WAIT_CAP)
			return paio->status_io;

		/* hook: on query */
		paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x12340003,paio->status_io);

		/* hook: on query */
		paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x12340004,paio->status_io);

		/* hook: after io request */
		paio->hooks[WAIO_HOOK_AFTER_IO_REQUEST](paio,WAIO_HOOK_AFTER_IO_REQUEST,paio->status_io);

		/* hook: on query */
		paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x12340005,paio->status_io);

		/* hook: before io */
		paio->hooks[WAIO_HOOK_BEFORE_IO](paio,WAIO_HOOK_BEFORE_IO,0);

		/* hook: on query */
		paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x12340006,paio->status_io);

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

		/* hook: on query */
		paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x12340007,paio->status_io);

		/* hfile may be non-blocking */
		if (paio->status_io == NT_STATUS_PENDING) {
			/* hook: on query */
			paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x12340008,paio->status_io);

			paio->status_io = __ntapi->zw_wait_for_multiple_objects(
				2,
				hpending,
				__ntapi->wait_type_any,
				NT_SYNC_NON_ALERTABLE,
				(nt_timeout *)0);

			/* hook: on query */
			paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x12340009,paio->status_io);

			/* abort request? */
			if (paio->abort_counter) {
				__ntapi->zw_close(hpending[0]);
				waio_thread_shutdown_response(paio);
			}
		}

		/* hook: on query */
		paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x1234000A,paio->status_io);

		/* hook: after io */
		paio->hooks[WAIO_HOOK_AFTER_IO](paio,WAIO_HOOK_AFTER_IO,0);

		/* hook: on query */
		paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x1234000B,paio->status_io);

		/* advance the data counter and notify the loop thread */
		at_locked_inc(&paio->data_counter);

		/* hook: on query */
		paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x1234000C,paio->status_io);

		/* hook: before io complete */
		paio->hooks[WAIO_HOOK_BEFORE_IO_COMPLETE](paio,WAIO_HOOK_BEFORE_IO_COMPLETE,paio->status_io);

		do {
			paio->status_io = __ntapi->zw_set_event(
				paio->hevent_queue_request,
				&state);
			paio->hooks[WAIO_HOOK_ON_QUERY](paio,0xAAAA0000,paio->status_io);
			paio->hooks[WAIO_HOOK_ON_QUERY](paio,0xAAAA1111,state);
		} while (!paio->status_io && state);

		paio->hooks[WAIO_HOOK_ON_QUERY](paio,0xAAAA2222,state);
		paio->status_io = __ntapi->zw_wait_for_single_object(
			paio->hevent_io_complete,
			NT_SYNC_NON_ALERTABLE,
			(nt_timeout *)0);

		paio->hooks[WAIO_HOOK_ON_QUERY](paio,0xAAAA3333,state);
		if (paio->status_io) return paio->status_io;

		__ntapi->zw_reset_event(
			paio->hevent_io_complete,
			(int32_t *)0);

		/* hook: on query */
		paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x1234000D,paio->status_io);

		paio->status_io = __ntapi->zw_set_event(
						paio->hevent_queue_request,
						&state);
		paio->hooks[WAIO_HOOK_ON_QUERY](paio,0x123400DD,state);
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
