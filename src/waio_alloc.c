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
#include "waio_impl.h"
#include "waio_cx.h"

static waio_hook cx_before_io;
static waio_hook cx_after_io;
static waio_hook cx_cancel_request;
static waio_hook cx_shutdown_response;

typedef struct waio_cx_opaque_block {
	struct waio_cx_interface	cx;
	struct waio_interface		cx_waio;
	struct waio_slot_interface	cx_slots[WAIO_CX_SLOT_COUNT];
	struct waio_request_interface	cx_requests[1];
} waio_cx_block;

waio_api waio_cx waio_alloc(
	_in_	void * 		handle,
	_in_	unsigned int	flags,
	_in_	void * 		options,
	_out_	signed int *	status)
{
	int32_t		_status;
	nt_iosb		iosb;
	nt_fsi		fsi;
	waio_cx_block *	cx_block;
	size_t		cx_block_size;
	waio_hook **	hook;
	size_t		req_count;
	size_t		i;

	/* status */
	if (!status) status = &_status;

	/* xvtbl init */
	*status = waio_xvtbls_init_once();

	if (*status) return (waio_cx)0;

	/* validate handle */
	*status = __ntapi->zw_query_information_file(
		handle,
		&iosb,
		&fsi,
		sizeof(fsi),
		NT_FILE_STANDARD_INFORMATION);

	if (*status) return (waio_cx)0;

	/* cx_block alloc */
	cx_block      = (waio_cx_block *)0;
	cx_block_size = WAIO_CX_BLOCK_SIZE;

	*status = __ntapi->zw_allocate_virtual_memory(
		NT_CURRENT_PROCESS_HANDLE,
		(void **)&cx_block,
		0,
		&cx_block_size,
		NT_MEM_COMMIT,
		NT_PAGE_READWRITE);

	if (*status) return (waio_cx)0;

	/* cx_block: memory init */
	__ntapi->memset(cx_block,0,cx_block_size);

	/* cx_block->cx init */
	cx_block->cx.self	= &cx_block->cx;
	cx_block->cx.paio	= &cx_block->cx_waio;
	cx_block->cx_waio.slots	= &cx_block->cx_slots[0];
	cx_block->cx.cx_size	= cx_block_size;

	/* cx_block->cx_waio init */
	cx_block->cx_waio.hfile = handle;

	/* cx_block: free nodes init */
	req_count = (cx_block_size -  ((size_t)(&((waio_cx_block *)0)->cx_requests))) \
		/ sizeof(waio_request);

	for (i=0; i<(req_count-1); i++)
		cx_block->cx_requests[i].next = &(cx_block->cx_requests[i+1]);

	/* cx_block->cx.paio init */
	cx_block->cx.paio->qfree = cx_block->cx_requests;

	/* default hooks */
	hook = (waio_hook **)&cx_block->cx.paio->hooks;

	for (i=0; i<WAIO_HOOK_CAP; i++, hook++)
		if (!(*hook))
			*hook = waio_hook_default;

	/* hooks */
	cx_block->cx.paio->hooks[WAIO_HOOK_BEFORE_IO] = cx_before_io;
	cx_block->cx.paio->hooks[WAIO_HOOK_AFTER_IO]  = cx_after_io;
	cx_block->cx.paio->hooks[WAIO_HOOK_ON_CANCEL_REQUEST]  = cx_cancel_request;
	cx_block->cx.paio->hooks[WAIO_HOOK_BEFORE_SHUTDOWN_RESPONSE] = cx_shutdown_response;
	cx_block->cx.paio->hooks[WAIO_HOOK_AFTER_SHUTDOWN_RESPONSE]  = cx_shutdown_response;
	cx_block->cx.paio->hooks[WAIO_HOOK_AFTER_SHUTDOWN_FALLBACK]  = cx_shutdown_response;

	/* thread pair init */
	*status = waio_init(cx_block->cx.paio);
	if (*status) return (waio_cx)0;

	/* return opaque pointer */
	return &cx_block->cx;
}


static signed int __waio_call_conv__hook cx_before_io(
	waio *		paio,
	waio_hook_type	type,
	signed int	status)
{
	waio_aiocb_opaque * opaque;

	opaque = ((waio_aiocb_opaque *)(paio->packet->aiocb->__opaque));
	opaque->qstatus = NT_STATUS_PENDING;
	opaque->status_io   = paio->status_io;

	/* translation */
	paio->type = (waio_io_type)opaque->request->slot.aio_lio_opcode;

	/* account for the fallback cancellation method */
	opaque->fc_after_io = 0;

	return status;
}


static signed int __waio_call_conv__hook cx_after_io(
	waio *		paio,
	waio_hook_type	type,
	signed int	status)
{
	waio_aiocb_opaque *	opaque;
	intptr_t		fc_after_io;

	if (!paio->packet) return status;

	opaque = ((waio_aiocb_opaque *)(paio->packet->aiocb->__opaque));
	opaque->status_io   = paio->status_io;

	/* account for the fallback cancellation method */
	fc_after_io = at_locked_cas(&opaque->fc_after_io,0,1);
	if (fc_after_io) return status;

	/* queue status */
	switch (status) {
		case NT_STATUS_SUCCESS:
			opaque->qstatus = NT_STATUS_SUCCESS;
			break;

		case NT_STATUS_CANCELLED:
			opaque->qstatus = NT_STATUS_CANCELLED;
			break;

		default:
			opaque->qstatus = NT_STATUS_GENERIC_COMMAND_FAILED;
			break;
	}

	/* copy completion status */
	opaque->iosb.pointer = paio->packet->iosb.pointer;
	opaque->iosb.info    = paio->packet->iosb.info;

	/* finalize */
	paio->packet    = (waio_packet *)0;
	paio->cancel_io = (waio_packet *)0;

	/* waio_listio notification */
	if (opaque->hlistio)
		__ntapi->zw_set_event(
			opaque->hlistio,
			(int32_t *)0);

	/* waio_suspend notification */
	if (opaque->hpending) {
		__ntapi->zw_set_event(
			opaque->hpending,
			(int32_t *)0);

		__ntapi->zw_close(opaque->hpending);
	}

	return status;
}


static signed int __waio_call_conv__hook cx_cancel_request(
	waio *		paio,
	waio_hook_type	type,
	signed int	status)
{
	return waio_cancel_aiocb(paio,(struct waio_aiocb *)paio->context_loop);
}


static signed int __waio_call_conv__hook cx_shutdown_response(
	waio *		paio,
	waio_hook_type	type,
	signed int	status)
{
	waio_aiocb_opaque * opaque;

	if (paio->packet) {
		opaque = (waio_aiocb_opaque *)paio->packet->aiocb->__opaque;
		opaque->qstatus = NT_STATUS_CANCELLED;
	}

	return status;
}
