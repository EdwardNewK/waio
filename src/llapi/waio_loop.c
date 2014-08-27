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
	nt_event_basic_information	event_info;
	uint32_t			state;
	size_t				info_size;
	intptr_t			counter;
	void *				hwait[3];

	/* get initial counter */
	counter = paio->packet.counter;

	/* notify the init routine that the loop is ready */
	paio->status_loop = __ntapi->zw_set_event(
		paio->hevent_loop_ready,
		(uint32_t *)0);

	if (paio->status_loop) return paio->status_loop;

	/* hook: before read request */
	paio->hooks[WAIO_HOOK_BEFORE_READ_REQUEST](paio,WAIO_HOOK_BEFORE_READ_REQUEST,0);

	/* submit initial read request */
	paio->status_loop = __ntapi->zw_set_event(
		paio->hevent_data_request,
		&state);

	if (paio->status_loop) return paio->status_loop;

	/* hook: before read request */
	paio->hooks[WAIO_HOOK_BEFORE_READ_REQUEST](paio,WAIO_HOOK_BEFORE_READ_REQUEST,0);

	/* prepare for the waits */
	hwait[0] = paio->hevent_abort_request;
	hwait[1] = paio->hevent_data_received;
	hwait[2] = paio->hthread_io;

	do {
		/* wait for an abort request to arrive;	*/
		/* or for data to be received;		*/
		/* or for the io thread to die.         */
		paio->status_loop = __ntapi->zw_wait_for_multiple_objects(
			3,
			hwait,
			NT_WAIT_ANY,
			0,
			&paio->read_request_timeout);

		/* wine debug */
		paio->hooks[WAIO_HOOK_ON_TIMEOUT](paio,WAIO_HOOK_ON_TIMEOUT,paio->status_loop);

		if ((uint32_t)paio->status_loop >= NT_STATUS_WAIT_CAP)
			waio_thread_shutdown_request(paio);

		/* abort request? */
		paio->status_loop = __ntapi->zw_query_event(
			paio->hevent_abort_request,
			NT_EVENT_BASIC_INFORMATION,
			&event_info,
			sizeof(event_info),
			&info_size);

		if ((paio->status_loop) || (event_info.signal_state == NT_EVENT_SIGNALED))
			waio_thread_shutdown_request(paio);

		if (paio->packet.counter == counter)
			/* that must have been the thread crashing */
			return NT_STATUS_THREAD_NOT_IN_PROCESS;

		/* hook: after data received */
		paio->hooks[WAIO_HOOK_AFTER_DATA_RECEIVED](paio,WAIO_HOOK_AFTER_DATA_RECEIVED,0);

		/* update the local counter */
		counter = paio->packet.counter;

		/* hook: before data processed */
		paio->hooks[WAIO_HOOK_BEFORE_DATA_PROCESSED](paio,WAIO_HOOK_BEFORE_DATA_PROCESSED,0);

		/* hook: after data processed */
		paio->hooks[WAIO_HOOK_AFTER_DATA_PROCESSED](paio,WAIO_HOOK_AFTER_DATA_PROCESSED,0);

		/* submit the next data request */
		paio->status_loop = __ntapi->zw_reset_event(
			paio->hevent_data_received,
			&state);

		if (paio->status_loop)
			waio_thread_shutdown_request(paio);

		/* hook: before read request */
		paio->hooks[WAIO_HOOK_BEFORE_READ_REQUEST](paio,WAIO_HOOK_BEFORE_READ_REQUEST,0);

		paio->status_loop = __ntapi->zw_set_event(
			paio->hevent_data_request,
			&state);

		if (paio->status_loop)
			waio_thread_shutdown_request(paio);

		/* app's data processing takes place here */
	} while (paio->status_loop == NT_STATUS_SUCCESS); /* (1) */

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

	/* terminate thread cleanly */
	__ntapi->zw_terminate_thread(
		NT_CURRENT_THREAD_HANDLE,
		status);

	/* should never get here */
	return NT_STATUS_INTERNAL_ERROR;
}
