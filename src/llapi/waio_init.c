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


#include <psxtypes/psxtypes.h>
#include <ntapi/ntapi.h>
#include <waio/waio__llapi.h>
#include "waio_impl.h"


waio_api
int32_t __stdcall waio_init(waio * paio)
{
	int		i;
	waio_hook **	hook;
	int32_t		status;

	/* default hooks */
	hook = (waio_hook **)&paio->hooks;

	for (i=0; i<WAIO_HOOK_CAP; i++, hook++)
		if (!(*hook))
			*hook = waio_hook_default;

	/* hook: before pipe init */
	paio->hooks[WAIO_HOOK_BEFORE_INIT](paio,WAIO_HOOK_BEFORE_INIT,0);

	/* notification events (app might have created some of them for us) */
	/* hevent_abort_request */
	if (!paio->hevent_abort_request)
		paio->status_loop = __ntapi->tt_create_private_event(
			&paio->hevent_abort_request,
			NT_NOTIFICATION_EVENT,
			NT_EVENT_NOT_SIGNALED);

	if (paio->status_loop) return paio->status_loop;

	/* hevent_cancel_request */
	if (!paio->hevent_cancel_request)
		paio->status_loop = __ntapi->tt_create_private_event(
			&paio->hevent_cancel_request,
			NT_NOTIFICATION_EVENT,
			NT_EVENT_NOT_SIGNALED);

	if (paio->status_loop) return paio->status_loop;

	/* hevent_queue_request */
	if (!paio->hevent_queue_request)
		paio->status_loop = __ntapi->tt_create_private_event(
			&paio->hevent_queue_request,
			NT_NOTIFICATION_EVENT,
			NT_EVENT_NOT_SIGNALED);

	if (paio->status_loop) return paio->status_loop;

	/* hevent_loop_ready */
	if (!paio->hevent_loop_ready)
		paio->status_loop = __ntapi->tt_create_private_event(
			&paio->hevent_loop_ready,
			NT_NOTIFICATION_EVENT,
			NT_EVENT_NOT_SIGNALED);

	if (paio->status_loop) return paio->status_loop;

	/* hevent_io_ready */
	if (!paio->hevent_io_ready)
		paio->status_loop = __ntapi->tt_create_private_event(
			&paio->hevent_io_ready,
			NT_NOTIFICATION_EVENT,
			NT_EVENT_NOT_SIGNALED);

	if (paio->status_loop) return paio->status_loop;

	/* hevent_io_request */
	if (!paio->hevent_io_request)
		paio->status_loop = __ntapi->tt_create_private_event(
			&paio->hevent_io_request,
			NT_NOTIFICATION_EVENT,
			NT_EVENT_NOT_SIGNALED);

	if (paio->status_loop) return paio->status_loop;

	/* hevent_io_complete */
	if (!paio->hevent_io_complete)
		paio->status_loop = __ntapi->tt_create_private_event(
			&paio->hevent_io_complete,
			NT_NOTIFICATION_EVENT,
			NT_EVENT_NOT_SIGNALED);

	if (paio->status_loop) return paio->status_loop;

	/* (custom thread creation routine not included with this project) */
	/* create io thread */
	paio->status_io = NT_STATUS_THREAD_NOT_IN_PROCESS;

	paio->hthread_io = __winapi->create_thread(
		(nt_sa *)0,
		0x1000,
		(nt_thread_start_routine *)waio_io_entry_point,
		paio,
		0,
		(uint32_t *)0);

	if (!paio->hthread_io) return NT_STATUS_INVALID_THREAD;

	/* wait for the io thread to be ready */
	status = __ntapi->zw_wait_for_single_object(
		paio->hevent_io_ready,
		NT_SYNC_NON_ALERTABLE,
		(nt_large_integer *)0);

	if (status) return status;

	/* create loop thread */
	paio->status_loop = NT_STATUS_THREAD_NOT_IN_PROCESS;

	paio->hthread_loop = __winapi->create_thread(
		(nt_sa *)0,
		0x1000,
		(nt_thread_start_routine *)waio_loop_entry_point,
		paio,
		0,
		(uint32_t *)0);

	if (!paio->hthread_loop) return NT_STATUS_INVALID_THREAD;

	/* wait for the loop thread to be ready */
	status = __ntapi->zw_wait_for_single_object(
		paio->hevent_loop_ready,
		NT_SYNC_NON_ALERTABLE,
		(nt_large_integer *)0);

	if (status) return status;

	/* hook: after pipe init */
	paio->hooks[WAIO_HOOK_AFTER_INIT](paio,WAIO_HOOK_AFTER_INIT,0);


	/* done */
	return NT_STATUS_SUCCESS;
}


int32_t __stdcall waio_hook_default(
	_in_		waio *		paio,
	_in_		signed int	type,
	_in_		int32_t		status)
{
	return status;
}
