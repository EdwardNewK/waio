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
int waio_suspend(
	_in_	waio_cx				cx,
	_in_	const struct waio_aiocb *	const aiocb_list[],
	_in_	int				nent,
	_in_	waio_timeout *			timeout)
{
	waio_aiocb_opaque *	opaque;
	void *			hwait[WAIO_LISTIO_MAX];
	void *			hsignal;
	int			events;
	int			i;

	events  = 0;
	hsignal = (void *)0;

	for (i=0; i<nent; i++) {
		opaque = (waio_aiocb_opaque *)aiocb_list[i]->__opaque;

		switch (opaque->qstatus) {
			case NT_STATUS_WAIT_1:
			case NT_STATUS_PENDING:
				hwait[events] = opaque->hpending;
				events++;
				break;
		}

		if (aiocb_list[i]->hsignal)
			hsignal = aiocb_list[i]->hsignal;
	}

	if (hsignal) {
		hwait[events] = hsignal;
		events++;
	}

	/* conditional wait */
	if (events)
		__ntapi->zw_wait_for_multiple_objects(
			events,
			hwait,
			__ntapi->wait_type_any,
			NT_SYNC_NON_ALERTABLE,
			(nt_timeout *)timeout);

	/* check & return */
	for (i=0; i<nent; i++) {
		opaque = (waio_aiocb_opaque *)aiocb_list[i]->__opaque;

		switch (opaque->qstatus) {
			case NT_STATUS_SUCCESS:
			case NT_STATUS_CANCELLED:
			case NT_STATUS_GENERIC_COMMAND_FAILED:
				/* above i/o operation has completed */
				return 0;
				break;
		}
	}

	return -WAIO_EAGAIN;
}
