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


typedef int waio_ioctl_fn (
	_in_	waio_cx			cx,
	_in_	struct waio_aiocb *	aiocb,
	_in_	int			cmd,
	_in_	size_t			flags,
	_in_	void *			pin,
	_out_	void *			pout);

static waio_ioctl_fn *	ioctl_vtbl[WAIO_IOCTL_OPCODE_CAP - WAIO_IOCTL_OPCODE_BASE + 1] = {};

waio_api
int waio_ioctl (
	_in_	waio_cx			cx,
	_in_	struct waio_aiocb *	aiocb,
	_in_	int			cmd,
	_in_	size_t			flags,
	_in_	void *			pin,
	_out_	void *			pout)
{
	/* none implemented */
	return -WAIO_EINVAL;

	if ((cmd >= WAIO_IOCTL_OPCODE_BASE) && (cmd < WAIO_IOCTL_OPCODE_CAP))
		return ioctl_vtbl[cmd - WAIO_IOCTL_OPCODE_BASE](
			cx,
			aiocb,
			cmd,
			flags,
			pin,
			pout);
	else
		return -WAIO_EINVAL;
}
