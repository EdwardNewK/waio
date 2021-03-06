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
#include <waio/waio.h>
#include "waio_impl.h"
#include "waio_cx.h"


waio_api
int32_t __stdcall waio_enqueue(waio * paio)
{
	waio_slot *		slot;
	waio_request *		req;
	waio_aiocb_opaque *	opaque;
	int			i;

	/* waio_enqueue: executes in the loop thread */

	/* check slots and add requests as appropriate */
	if (!paio->qfree)
		return NT_STATUS_DEVICE_BUSY;

	for (i=0; i<WAIO_CX_SLOT_COUNT; i++) {
		slot = &paio->slots[i];

		if (slot->pid) {
			/* regress the queue request counter */
			at_locked_dec((intptr_t *)&paio->queue_counter);

			/* behead and retail */
			req		= paio->qfree;
			paio->qfree	= paio->qfree->next;
			req->next	= (waio_request *)0;

			/* copy request from slot */
			req->slot.pid            = slot->pid;
			req->slot.tid            = slot->tid;
			req->slot.aio_lio_opcode = slot->aio_lio_opcode;
			req->slot.aio_reqprio    = slot->aio_reqprio;
			req->slot.aio_buf        = slot->aio_buf;
			req->slot.aio_nbytes     = slot->aio_nbytes;
			req->slot.aio_offset     = slot->aio_offset;
			req->slot.aiocb		 = slot->aiocb;

			/* request pointer (cancellation reference) */
			opaque = ((waio_aiocb_opaque *)(slot->aiocb->__opaque));
			opaque->request = req;

			/* translation */
			req->rpacket.data        = (os_unsigned_ptr *)slot->aio_buf;
			req->rpacket.buffer_size = slot->aio_nbytes;
			req->rpacket.offset.quad = slot->aio_offset;
			req->rpacket.aiocb       = slot->aiocb;

			/* fake memset and default status */
			req->rpacket.iosb.info	  = 0;
			req->rpacket.iosb.pointer = (void *)0;
			req->rpacket.iosb.status  = NT_STATUS_PENDING;

			/* add request to queue */
			if (paio->qtail) {
				paio->qtail->next = req;
				paio->qtail       = req;
			} else {
				paio->queue = req;
				paio->qtail = req;
			}

			/* mark slot as free */
			slot->pid = 0;
			slot->tid = 0;
		}
	}

	return NT_STATUS_SUCCESS;
}
