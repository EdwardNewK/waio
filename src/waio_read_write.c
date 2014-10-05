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

static int waio_read_write(
	_in_	waio_cx			cx,
	_in_	struct waio_aiocb *	aiocb,
	_in_	int			lio_opcode)
{
	int32_t			status;
	waio_aiocb_opaque *	opaque;

	/* internal notification */
	opaque = (waio_aiocb_opaque *)aiocb->__opaque;
	opaque->cx = cx;
	opaque->hlistio = 0;

	status = waio_submit_single_request(
		cx->paio,
		aiocb,
		lio_opcode,
		__winapi->get_current_process_id(),
		__winapi->get_current_thread_id());

	if (status == NT_STATUS_INVALID_HANDLE)
		return -WAIO_EBADF;
	else if (status)
		return -WAIO_EAGAIN;

	return 0;
}


waio_api
int waio_read(
	_in_	waio_cx			cx,
	_in_	struct waio_aiocb *	aiocb)
{
	return waio_read_write(cx,aiocb,WAIO_READ);
}


waio_api
int waio_write(
	_in_	waio_cx			cx,
	_in_	struct waio_aiocb *	aiocb)
{
	return waio_read_write(cx,aiocb,WAIO_WRITE);
}
