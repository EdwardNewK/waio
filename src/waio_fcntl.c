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


typedef int waio_fcntl_fn (
	_in_	waio_cx			cx,
	_in_	struct waio_aiocb *	aiocb,
	_in_	int			cmd,
	_in_	size_t			flags,
	_in_	void *			pin,
	_out_	void *			pout);

static waio_fcntl_fn	waio_fcntl_query_set_hook;

static waio_fcntl_fn *	fcntl_vtbl[WAIO_FCNTL_OPCODE_CAP - WAIO_FCNTL_OPCODE_BASE + 1] = {
			waio_fcntl_query_set_hook};

waio_api
int waio_fcntl (
	_in_	waio_cx			cx,
	_in_	struct waio_aiocb *	aiocb,
	_in_	int			cmd,
	_in_	size_t			flags,
	_in_	void *			pin,
	_out_	void *			pout)
{
	if ((cmd >= WAIO_FCNTL_OPCODE_BASE) && (cmd < WAIO_FCNTL_OPCODE_CAP))
		return fcntl_vtbl[cmd - WAIO_FCNTL_OPCODE_BASE](
			cx,
			aiocb,
			cmd,
			flags,
			pin,
			pout);
	else
		return -WAIO_EINVAL;
}


static int waio_fcntl_query_set_hook (
	_in_	waio_cx			cx,
	_in_	struct waio_aiocb *	aiocb,
	_in_	int			cmd,
	_in_	size_t			flags,
	_in_	void *			pin,
	_out_	void *			pout)
{
	cx->paio->hooks[WAIO_HOOK_ON_QUERY] = (waio_hook *)pin;
	return 0;
}

