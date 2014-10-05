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
#include <waio/waio.h>
#include <waio/waio__llapi.h>
#include <waio/waio__errno.h>
#include "waio_impl.h"
#include "waio_cx.h"

waio_hook waio_test_default_hook;

waio_api int waio_free (waio_cx cx)
{
	int32_t		status;
	int32_t		state;
	void *		hwait[2];
	nt_timeout	timeout;
	void *		region_addr;
	size_t		region_size;

	/* validate cx */
	if (!cx) return -WAIO_EINVAL;

	/* abort pending io operations */
	at_locked_inc(&cx->paio->abort_counter);
	status = __ntapi->zw_set_event(
		cx->paio->hevent_abort_request,
		&state);

	if (status) return -WAIO_EACCES;

	/* wait for loop and io threads to terminate */
	hwait[0] = cx->paio->hthread_loop;
	hwait[1] = cx->paio->hthread_io;
	timeout.quad = 60 * (-1) * 10 * 1000 * 1000;

	status = __ntapi->zw_wait_for_multiple_objects(
		2,
		hwait,
		__ntapi->wait_type_all,
		NT_SYNC_NON_ALERTABLE,
		&timeout);

	if (status) return -WAIO_EACCES;

	/* close private notification events */
	__ntapi->zw_close(cx->paio->hevent_loop_ready);
	__ntapi->zw_close(cx->paio->hevent_io_ready);
	__ntapi->zw_close(cx->paio->hevent_io_request);
	__ntapi->zw_close(cx->paio->hevent_io_complete);
	__ntapi->zw_close(cx->paio->hevent_abort_request);
	__ntapi->zw_close(cx->paio->hevent_cancel_request);
	__ntapi->zw_close(cx->paio->hevent_queue_request);

	/* free memory */
	region_addr = cx->self;
	region_size = 0;

	status = __ntapi->zw_free_virtual_memory(
		NT_CURRENT_PROCESS_HANDLE,
		&region_addr,
		&region_size,
		NT_MEM_RELEASE);

	if (status) return -WAIO_EINVAL;

	return 0;
}
