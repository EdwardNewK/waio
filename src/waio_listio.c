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
int waio_listio(
	_in_	waio_cx			cx,
	_in_	int			mode,
	_in_	struct waio_aiocb *	const aiocb_list[],
	_in_	int			nent)
{
	int32_t			status;
	waio_aiocb_opaque *	opaque;
	void *			hwait[WAIO_LISTIO_MAX];
	int			aio_lio_opcode;
	uint32_t		pid;
	uint32_t		tid;
	int			i;

	/* validation */
	if ((mode < 0) || (mode > WAIO_NOWAIT))
		return -WAIO_EINVAL;
	else if (nent > WAIO_LISTIO_MAX)
		return -WAIO_EINVAL;

	/* submit requests */
	pid = __winapi->get_current_process_id();
	tid = __winapi->get_current_thread_id();

	for (i=0; i<nent; i++) {
		aio_lio_opcode = aiocb_list[i]->aio_lio_opcode;

		if ((aio_lio_opcode < 0) || (aio_lio_opcode > WAIO_NOP))
			return -WAIO_EINVAL;

		if (mode == WAIO_WAIT) {
			opaque = (waio_aiocb_opaque *)aiocb_list[i]->__opaque;

			status = __ntapi->tt_create_private_event(
					&opaque->hlistio,
					NT_NOTIFICATION_EVENT,
					NT_EVENT_NOT_SIGNALED);

			if (status) return -WAIO_EAGAIN;

			hwait[i] = opaque->hlistio;
		}

		status = waio_submit_single_request(
			cx->paio,
			aiocb_list[i],
			aio_lio_opcode,
			pid,
			tid);

		if (status) return -WAIO_EAGAIN;
	}

	if (mode == WAIO_NOWAIT)
		return 0;

	status = __ntapi->zw_wait_for_multiple_objects(
		nent,
		hwait,
		__ntapi->wait_type_all,
		NT_SYNC_NON_ALERTABLE,
		(nt_timeout *)0);

	if (status) return -WAIO_EINTR;

	/* check results & clean-up */
	for (i=0; i<nent; i++) {
		opaque = ((waio_aiocb_opaque *)(aiocb_list[i]->__opaque));
		__ntapi->zw_close(opaque->hlistio);

		if (opaque->qstatus)
			status = -WAIO_EIO;
	}

	return status;
}
