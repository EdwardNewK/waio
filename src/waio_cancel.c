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
#include <waio/waio.h>
#include <waio/waio__llapi.h>
#include <waio/waio__errno.h>
#include "waio_impl.h"
#include "waio_cx.h"

waio_api
int waio_cancel(
	_in_	waio_cx			cx,
	_in_	struct waio_aiocb *	aiocb)
{
	waio_aiocb_opaque *	opaque;
	waio_request *		req;
	waio_request *		next;
	unsigned char		fdone;
	unsigned char		fcanceled;

	/* todo: acquire lock */

	/* init flags */
	fdone     = 1;
	fcanceled = 1;

	/* cancel all pending requests? */
	if (!aiocb) {
		waio_cancel_current_request(cx->paio);

		for (req=cx->paio->queue; req; req=next) {
			opaque = (waio_aiocb_opaque *)req->rpacket.aiocb->__opaque;

			switch (opaque->qstatus) {
				case NT_STATUS_WAIT_1:
				case NT_STATUS_PENDING:
					opaque->qstatus = NT_STATUS_CANCELLED;
					fdone = 0;
					break;

				case NT_STATUS_GENERIC_COMMAND_FAILED:
				case NT_STATUS_SUCCESS:
					fcanceled  = 0;
					break;
			}

			/* book keeping */
			next		= req->next;
			req->next	= cx->paio->qfree;
			cx->paio->qfree	= req;
		}


		if (fdone)
			return WAIO_ALLDONE;
		else if (fcanceled)
			return WAIO_CANCELED;
		else
			return WAIO_NOTCANCELED;
	}

	/* cancel current request? */
	if (cx->paio->packet->aiocb == aiocb)
		waio_cancel_current_request(cx->paio);

	/* remove from queue */
	req  = cx->paio->queue;

	if (req)
		next = req->next;
	else
		next = (waio_request *)0;

	if (req->rpacket.aiocb == aiocb) {
		cx->paio->queue	= req->next;
		req->next	= cx->paio->qfree;
		cx->paio->qfree	= req;
	} else {
		while (next && next->rpacket.aiocb != aiocb) {
			req =  next;
			next = next->next;
		}

		if (next->rpacket.aiocb == aiocb) {
			req->next	= next->next;
			next->next	= cx->paio->qfree;
			cx->paio->qfree	= next;
		}
	}

	/* check status, clean-up, and return */
	opaque = (waio_aiocb_opaque *)aiocb->__opaque;

	if (opaque->hpending)
		__ntapi->zw_close(opaque->hpending);

	switch (opaque->qstatus) {
		case NT_STATUS_WAIT_1:
		case NT_STATUS_PENDING:
			opaque->qstatus = NT_STATUS_CANCELLED;
			return WAIO_CANCELED;
			break;

		default:
			return WAIO_ALLDONE;
			break;
	}
}
