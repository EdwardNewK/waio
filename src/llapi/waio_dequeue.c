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
#include "waio_cx.h"


waio_api
int32_t __stdcall waio_dequeue(waio * paio)
{
	waio_request *	req;
	int32_t		state;

	/* check for non-empty queue and no pending (blocking) io */
	if (paio->queue && !paio->packet) {
		req          = paio->queue;
		paio->packet = &req->rpacket;

		/* update the queue */
		paio->queue  = req->next;
		req->next    = paio->qfree;
		paio->qfree  = req;

		/* empty queue? */
		if (!paio->queue)
			paio->qtail = (waio_request *)0;

		/* submit the next io request */
		paio->status_loop = __ntapi->zw_reset_event(
			paio->hevent_io_request,
			&state);

		if (paio->status_loop)
			waio_thread_shutdown_request(paio);

		/* hook: before io request */
		paio->hooks[WAIO_HOOK_BEFORE_IO_REQUEST](paio,WAIO_HOOK_BEFORE_IO_REQUEST,0);

		paio->status_loop = __ntapi->zw_set_event(
			paio->hevent_io_request,
			&state);

		if (paio->status_loop)
			waio_thread_shutdown_request(paio);
	}

	return NT_STATUS_SUCCESS;
}
