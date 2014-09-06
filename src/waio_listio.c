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
#include "waio_errno.h"

waio_api
int waio_listio(
	_in_	waio_cx			cx,
	_in_	int			mode,
	_in_	struct waio_aiocb *	const aiocb_list[],
	_in_	int			nent)
{
	int32_t				status;
	struct waio_aiocb_opaque *	opaque;
	void *				hwait[WAIO_LISTIO_MAX];
	int				aio_lio_opcode;
	uint32_t			pid;
	uint32_t			tid;
	int				i;

	/* validation */
	if ((mode < 0) || (mode > WAIO_NOWAIT))
		return EINVAL;
	else if (nent > WAIO_LISTIO_MAX)
		return EINVAL;

	/* submit requests */
	pid = __winapi->get_current_process_id();
	tid = __winapi->get_current_thread_id();

	for (i=0; i<nent; i++) {
		aio_lio_opcode = aiocb_list[i]->aio_lio_opcode;

		if ((aio_lio_opcode < 0) || (aio_lio_opcode > WAIO_NOP))
			return EINVAL;

		status = waio_submit_single_request(
			cx->paio,
			aiocb_list[i],
			aio_lio_opcode,
			pid,
			tid);

		if (status)
			return EAGAIN;
	}

	if (mode == WAIO_NOWAIT)
		return 0;

	/* init list of wait events */
	for (i=0; i<nent; i++) {
		opaque	 = ((struct waio_aiocb_opaque *)(aiocb_list[i]->__opaque));
		hwait[i] = opaque->hpending;
	}

	status = __ntapi->zw_wait_for_multiple_objects(
		nent,
		hwait,
		__ntapi->wait_type_all,
		NT_SYNC_NON_ALERTABLE,
		(nt_timeout *)0);

	if (status)
		return EINTR;

	/* check results */
	for (i=0; i<nent; i++) {
		opaque = ((struct waio_aiocb_opaque *)(aiocb_list[i]->__opaque));

		if (opaque->iosb.status || opaque->cancel_io.status)
			return EIO;
	}

	return 0;
}
