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

waio_api
ssize_t waio_return(
	_in_	waio_cx			cx,
	_in_	struct waio_aiocb *	aiocb)
{
	waio_aiocb_opaque *	opaque;

	opaque	 = ((waio_aiocb_opaque *)(aiocb->__opaque));

	switch (opaque->qstatus) {
		case NT_STATUS_WAIT_1:
			/* technically UB */
			return -WAIO_EINPROGRESS;
			break;

		case NT_STATUS_PENDING:
			/* technically UB */
			return -WAIO_EINPROGRESS;
			break;

		case NT_STATUS_CANCELLED:
			return -WAIO_ECANCELED;
			break;
	}

	/* successful i/o? */
	if (opaque->iosb.status == NT_STATUS_SUCCESS)
		return opaque->iosb.info;

	/* completed: translate error */
	switch (opaque->iosb.status) {
		case NT_STATUS_INVALID_HANDLE:
			return -WAIO_EBADF;
			break;

		case NT_STATUS_ACCESS_VIOLATION:
			return -WAIO_EACCES;
			break;

		case NT_STATUS_BAD_FILE_TYPE:
			return -WAIO_EINVAL;
			break;

		default:
			return -WAIO_ENXIO;
	}
}
