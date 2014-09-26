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

/* the thread shutdown request executes in the context of the loop thread */
waio_api
int32_t __stdcall waio_cancel_current_request(waio * paio)
{
	int32_t		status;
	nt_iosb		cancel_iosb;

	/* cancel needed? */
	if (paio->packet->iosb.status != NT_STATUS_PENDING)
		return NT_STATUS_SUCCESS;

	/* cancel current request using the appropriate method */
	if (__ntapi->zw_cancel_io_file_ex && !__ntapi->wine_get_version) {
		/* preferred method */
		status = __ntapi->zw_cancel_io_file_ex(
			paio->hfile,
			&paio->packet->iosb,
			&cancel_iosb);
	} else {
		/* fallback method */
		waio_thread_shutdown_fallback(paio);

		/* i/o completed nonetheless? */
		if (paio->packet->iosb.status != NT_STATUS_PENDING) {
			/* hook: after io */
			paio->hooks[WAIO_HOOK_AFTER_IO](paio,WAIO_HOOK_AFTER_IO,0);

			/* hook: before io complete */
			paio->hooks[WAIO_HOOK_BEFORE_IO_COMPLETE](paio,WAIO_HOOK_BEFORE_IO_COMPLETE,0);
		}

		/* re-create io thread */
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
	}

	return status;
}
