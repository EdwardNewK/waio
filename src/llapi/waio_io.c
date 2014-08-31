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

/**
 *  the parent waits (also) on the child; if the child
 *  terminates (normally or not) due to an error, then
 *  the parent (and app) can use the status_io member
 *  to determine the reason.
**/


waio_api
int32_t __stdcall waio_io(waio * paio)
{
	nt_event_basic_information	event_info;
	uint32_t			state;
	size_t				info_size;
	void *				hwait[2];

	/* wine debug */
	paio->hooks[WAIO_HOOK_ON_QUERY](paio,WAIO_HOOK_ON_QUERY,1);

	/* fallback tip for legacy os versions */
	paio->fallback_tip = &state;

	/* norify the app that the child is ready */
	paio->status_io = __ntapi->zw_set_event(
		paio->hevent_io_ready,
		&state);

	/* wine debug */
	paio->hooks[WAIO_HOOK_ON_QUERY](paio,WAIO_HOOK_ON_QUERY,2);

	if (paio->status_io) return paio->status_io;

	hwait[0] = paio->hevent_data_request;
	hwait[1] = paio->hevent_abort_request;

	do {
		/* wine debug */
		paio->hooks[WAIO_HOOK_ON_QUERY](paio,WAIO_HOOK_ON_QUERY,3);
		/* wait for an abort request to arrive;	*/
		/* or for data to be requested;		*/
		paio->status_io = __ntapi->zw_wait_for_multiple_objects(
			2,
			hwait,
			NT_WAIT_ANY,
			0,
			&paio->read_request_timeout);

		/* wine debug */
		paio->hooks[WAIO_HOOK_ON_QUERY](paio,WAIO_HOOK_ON_QUERY,4);

		/* when enforcing a longest time interval between requests */
		/* (here we simply keep trying)                            */
		while (paio->status_io == NT_STATUS_TIMEOUT)
			paio->status_io = __ntapi->zw_wait_for_multiple_objects(
				2,
				hwait,
				NT_WAIT_ANY,
				0,
				&paio->read_request_timeout);

		if ((uint32_t)paio->status_io >= NT_STATUS_WAIT_CAP)
			return paio->status_io;

		/* abort message? */
		paio->status_io = __ntapi->zw_query_event(
			paio->hevent_abort_request,
			NT_EVENT_BASIC_INFORMATION,
			&event_info,
			sizeof(event_info),
			&info_size);

		if (paio->status_io)
			return paio->status_io;
		else if (event_info.signal_state == NT_EVENT_SIGNALED)
			waio_thread_shutdown_response(paio);

		/* read from pipe */
		/* todo: read or write */

		/* hook: after read request */
		paio->hooks[WAIO_HOOK_AFTER_READ_REQUEST](paio,WAIO_HOOK_AFTER_READ_REQUEST,0);

		/* hook: before read */
		paio->hooks[WAIO_HOOK_BEFORE_READ](paio,WAIO_HOOK_BEFORE_READ,0);

		/* io: read */
		paio->status_io = __ntapi->zw_read_file(
			paio->hfile,
			(void *)0,
			(void *)0,
			(void *)0,
			&paio->packet->iosb,
			paio->packet->data,
			(uint32_t)paio->packet->buffer_size,
			(nt_large_integer *)0,
			(uint32_t *)0);

		if (paio->status_io) return paio->status_io;

		/* hook: after read */
		paio->hooks[WAIO_HOOK_AFTER_READ](paio,WAIO_HOOK_AFTER_READ,0);

		/* abort message? */
		paio->status_io = __ntapi->zw_query_event(
			paio->hevent_abort_request,
			NT_EVENT_BASIC_INFORMATION,
			&event_info,
			sizeof(event_info),
			&info_size);

		if (paio->status_io)
			return paio->status_io;
		else if (event_info.signal_state == NT_EVENT_SIGNALED)
			waio_thread_shutdown_response(paio);

		/* successful read; advance the counter and notify the parent */
		paio->packet->counter++;

		paio->status_io = __ntapi->zw_reset_event(
			paio->hevent_data_request,
			&state);

		if (paio->status_io) return paio->status_io;

		/* hook: before data received */
		paio->hooks[WAIO_HOOK_BEFORE_DATA_RECEIVED](paio,WAIO_HOOK_BEFORE_DATA_RECEIVED,0);

		paio->status_io = __ntapi->zw_set_event(
			paio->hevent_data_received,
			&state);

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

	/* hook: on failure */
	if (status)
		paio->hooks[WAIO_HOOK_ON_FAILURE](paio,WAIO_HOOK_ON_FAILURE,status);

	/* terminate thread cleanly */
	__ntapi->zw_terminate_thread(
		NT_CURRENT_THREAD_HANDLE,
		status);

	/* should never get here */
	return NT_STATUS_INTERNAL_ERROR;
}
