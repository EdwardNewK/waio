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
#include <ntcon/ntcon.h>
#include <winapi/winapi.h>
#include <waio/waio.h>
#include <waio/waio__llapi.h>
#include "waio_impl.h"
#include "waio_cx.h"
#include "waio_test.h"

int __cdecl waio_test_alloc_free(void)
{
	int	ret;
	waio_cx cx_read;
	waio_cx cx_write;
	void *	hread;
	void *	hwrite;

	ret = __winapi->create_pipe(&hread,&hwrite,(nt_sa *)0,0);
	if (ret == 0) return NT_STATUS_INVALID_PIPE_STATE;

	cx_read = waio_alloc(hread, 0, (void *)0, &ret);
	if (!cx_read) return ret;

	cx_write = waio_alloc(hwrite, 0, (void *)0, &ret);
	if (!cx_write) return ret;

	ret = waio_free(cx_read);
	if (ret) return -ret;

	ret = waio_free(cx_write);
	if (ret) return -ret;

	return 0;
}
