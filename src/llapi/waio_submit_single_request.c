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

static int32_t __get_slot(
	_in_	waio *		paio,
	_out_	waio_slot **	slot,
	_in_	uint32_t	tid);

waio_api
int32_t waio_submit_single_request(
	_in_	waio *			paio,
	_in_	struct waio_aiocb *	aiocb,
	_in_	int			lio_opcode,
	_in_	uint32_t		pid,
	_in_	uint32_t		tid)
{
	int32_t			status;
	int32_t			state;
	waio_slot *		slot;
	waio_aiocb_opaque *	opaque;
	void *			hpending;
	void *			hlistio;

	/* lio_opcode */
	if ((lio_opcode < 0) || (lio_opcode > WAIO_NOP))
		return NT_STATUS_INVALID_PARAMETER_3;

	/* get free slot */
	status = __get_slot(paio,&slot,tid);
	if (status) return status;

	/* slot init */
	slot->aio_lio_opcode	= lio_opcode;
	slot->aio_reqprio	= aiocb->aio_reqprio;
	slot->aio_buf		= aiocb->aio_buf;
	slot->aio_nbytes		= aiocb->aio_nbytes;
	slot->aio_offset		= aiocb->aio_offset;
	slot->aiocb		= aiocb;

	/* internal notification */
	opaque   = ((waio_aiocb_opaque *)(aiocb->__opaque));
	hpending = opaque->hpending;
	hlistio  = opaque->hlistio;

	/* init opaque data */
	__ntapi->memset(aiocb->__opaque,0,sizeof(size_t) * WAIO_CX_OPAQUE_POINTERS);

	/* opaque queue status & internal notificaiton */
	opaque->qstatus  = NT_STATUS_WAIT_1;
	opaque->hpending = hpending;
	opaque->hlistio  = hlistio;

	/* mark slot for queueing */
	slot->pid = pid;

	/* submit request to the loop thread */
	do {
		at_locked_inc(&paio->queue_inc_counter);
		status = __ntapi->zw_set_event(
				paio->hevent_queue_request,
				&state);
	} while (!status && state && (slot->tid == tid));

	return status;
}


static int32_t _get_slot(
	_in_	waio *		paio,
	_out_	waio_slot **	slot,
	_in_	uint32_t	tid)
{
	int		i;
	intptr_t	key;

	key = 1;
	i   = 0;

	/* attempt to reserve a free slot */
	while (key && (i<WAIO_CX_SLOT_COUNT)) {
		*slot = &paio->slots[i];
		key   = at_locked_cas(
			(volatile intptr_t *)&((*slot)->tid),
			0,
			(intptr_t)tid);
		i++;
	}

	if (key) return
		NT_STATUS_DEVICE_BUSY;
	else
		return NT_STATUS_SUCCESS;
}


static int32_t __get_slot(
	_in_	waio *		paio,
	_out_	waio_slot **	slot,
	_in_	uint32_t	tid)
{
	int		status;
	void *		hwait;
	nt_timeout	timeout;
	int		i;

	#define WAIO_GET_SLOT_TRIES 128

	/* get slot: first (non-contended) attempt */
	status = _get_slot(paio,slot,tid);

	if (status == NT_STATUS_SUCCESS)
		return NT_STATUS_SUCCESS;

	/* the contended case */
	status = __ntapi->tt_create_private_event(
		&hwait,
		NT_NOTIFICATION_EVENT,
		NT_EVENT_NOT_SIGNALED);

	if (status) return status;

	/* try acquiring lock using ~1usec intervals */
	timeout.quad = (-1) * 10;

	for (i=0; (i<WAIO_GET_SLOT_TRIES) && status; i++) {
		__ntapi->zw_wait_for_single_object(
			hwait,
			NT_SYNC_NON_ALERTABLE,
			&timeout);

		status = _get_slot(paio,slot,tid);
	}

	/* clean-up */
	__ntapi->zw_close(hwait);

	return status;
}
