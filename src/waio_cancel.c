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


static int waio_cancel_return(
	_in_	waio *			paio,
	_in_	struct waio_aiocb *	aiocb,
	_in_	int			ret);

waio_api
int waio_cancel(
	_in_	waio_cx			cx,
	_in_	struct waio_aiocb *	aiocb)
{
	int32_t			status;
	waio_aiocb_opaque *	opaque;
	void *			context;
	void *			hwait;
	nt_timeout		timeout;
	struct waio_aiocb	laiocb;
	unsigned int		cx_cancel;
	int			i;

	#define WAIO_ACQUIRE_LOCK_TRIES 1024

	if (aiocb)
		cx_cancel = 0;
	else {
		aiocb = &laiocb;
		__ntapi->memset(aiocb,0,sizeof(*aiocb));
		cx_cancel = 1;
	}

	/* acquire lock */
	hwait   = (void *)0;
	context = (void *)at_locked_cas(
			(volatile intptr_t *)&cx->paio->context_loop,
			0,
			(intptr_t)aiocb);

	if (context) {
		/* the contended case */
		status = __ntapi->tt_create_private_event(
			&hwait,
			NT_NOTIFICATION_EVENT,
			NT_EVENT_NOT_SIGNALED);

		if (status) return -WAIO_EAGAIN;

		/* try again before performing the wait */
		context = (void *)at_locked_cas(
			(volatile intptr_t *)&cx->paio->context_loop,
			0,
			(intptr_t)aiocb);

		/* try using ~10usec intervals */
		timeout.quad = (-1) * 10 * 10;

		for (i=0; (i<WAIO_ACQUIRE_LOCK_TRIES) && context; i++) {
			__ntapi->zw_wait_for_single_object(
				hwait,
				NT_SYNC_NON_ALERTABLE,
				&timeout);

			context = (void *)at_locked_cas(
				(volatile intptr_t *)&cx->paio->context_loop,
				0,
				(intptr_t)aiocb);
		}

		if (context) return -WAIO_EAGAIN;
	}

	/* internal cancel notification */
	opaque = (waio_aiocb_opaque *)aiocb->__opaque;

	if (opaque->hcancel) {
		status = __ntapi->zw_reset_event(
			opaque->hcancel,
			(int32_t *)0);
	} else {
		if (hwait) {
			/* save two system calls */
			opaque->hcancel = hwait;
			hwait = (void *)0;
		} else {
			status = __ntapi->tt_create_private_event(
				&opaque->hcancel,
				NT_NOTIFICATION_EVENT,
				NT_EVENT_NOT_SIGNALED);
		}
	}

	/* hwait clean-up */
	if (hwait) __ntapi->zw_close(hwait);

	/* validation */
	if (status) return -WAIO_EAGAIN;

	/* cancellation request */
	opaque->cx_cancel = cx_cancel;
	at_locked_inc(&(cx->paio->cancel_inc_counter));

	status = __ntapi->zw_set_event(
		cx->paio->hevent_cancel_request,
		(int32_t *)0);

	if (status) return -WAIO_EAGAIN; 

	/* wait for waio_cancel_aiocb (loop thread) to complete */
	status = __ntapi->zw_wait_for_single_object(
		aiocb->hsignal,
		NT_SYNC_NON_ALERTABLE,
		(nt_timeout *)0);

	if (status) return -WAIO_EAGAIN;

	/* copy result */
	status = opaque->status_cancel;

	/* release lock */
	__ntapi->zw_reset_event(
		cx->paio->hevent_cancel_request,
		(int32_t *)0);

	cx->paio->context_loop = (void *)0;

	/* and voila */
	return status;
}


waio_internal_api
int waio_cancel_aiocb(
	_in_	waio *			paio,
	_in_	struct waio_aiocb *	aiocb)
{
	waio_aiocb_opaque *	opaque;
	waio_request *		req;
	waio_request *		next;
	unsigned char		fdone;
	unsigned char		fcanceled;
	int			ret;

	/* init flags */
	fdone     = 1;
	fcanceled = 1;

	/* cancel all pending requests? */
	opaque = (waio_aiocb_opaque *)aiocb->__opaque;

	if (opaque->cx_cancel) {
		waio_cancel_current_request(paio);

		for (req=paio->queue; req; req=next) {
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
			req->next	= paio->qfree;
			paio->qfree	= req;
		}


		if (fdone)
			ret = WAIO_ALLDONE;
		else if (fcanceled)
			ret = WAIO_CANCELED;
		else
			ret = WAIO_NOTCANCELED;

		return waio_cancel_return(paio,aiocb,ret);
	}

	/* cancel current request? */
	if (paio->packet->aiocb == aiocb)
		waio_cancel_current_request(paio);

	/* remove from queue */
	req = paio->queue;

	if (req)
		next = req->next;
	else
		next = (waio_request *)0;

	if (req->rpacket.aiocb == aiocb) {
		paio->queue	= req->next;
		req->next	= paio->qfree;
		paio->qfree	= req;
	} else {
		while (next && next->rpacket.aiocb != aiocb) {
			req =  next;
			next = next->next;
		}

		if (next->rpacket.aiocb == aiocb) {
			req->next	= next->next;
			next->next	= paio->qfree;
			paio->qfree	= next;
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
			ret = WAIO_CANCELED;
			break;

		default:
			ret = WAIO_ALLDONE;
			break;
	}

	return waio_cancel_return(paio,aiocb,ret);
}


static int waio_cancel_return(
	_in_	waio *			paio,
	_in_	struct waio_aiocb *	aiocb,
	_in_	int			ret)
{
	waio_aiocb_opaque * opaque;

	opaque = (waio_aiocb_opaque *)aiocb->__opaque;

	/* return status to caller */
	opaque->status_cancel = ret;

	/* wake caller */
	__ntapi->zw_set_event(
		opaque->hcancel,
		(int32_t *)0);

	/* return to loop */
	return ret;
}
