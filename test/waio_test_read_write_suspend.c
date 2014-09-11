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

char write_buf[65] = {
			'1','2','3','4','5','6','7','8','9','a','b','c','d','e','f',
			'1','2','3','4','5','6','7','8','9','a','b','c','d','e','f',
			'1','2','3','4','5','6','7','8','9','a','b','c','d','e','f',
			'1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};


int __cdecl waio_test_read_write_suspend(void)
{
	int			ret;
	waio_cx 		cx_read;
	waio_cx 		cx_write;
	void *			hread;
	void *			hwrite;
	struct waio_aiocb	cb_read;
	struct waio_aiocb	cb_write;
	const struct waio_aiocb*cb[1];
	ssize_t			bytes;
	waio_timeout		timeout;
	char			read_buf[128];

	__ntapi->memset(read_buf,0xffffffff,128);
	__ntapi->memset(&cb_read,0,sizeof(cb_read));
	__ntapi->memset(&cb_write,0,sizeof(cb_write));

	ret = __winapi->create_pipe(&hread,&hwrite,(nt_sa *)0,0);
	if (ret == 0) return NT_STATUS_INVALID_PIPE_STATE;

	cx_read = waio_alloc(hread, 0, (void *)0, &ret);
	if (!cx_read) return ret;

	cx_write = waio_alloc(hwrite, 0, (void *)0, &ret);
	if (!cx_write) return ret;

	waio_fcntl(cx_read,(struct waio_aiocb *)0,WAIO_OPCODE_QUERY_SET_HOOK,0,waio_test_query_hook,(void *)0);
	waio_fcntl(cx_read,(struct waio_aiocb *)0,WAIO_OPCODE_QUERY_SET_HOOK,0,waio_test_query_hook,(void *)0);

	cb_read.aio_buf		= read_buf;
	cb_read.aio_nbytes	= 8;

	cb_write.aio_buf	= write_buf;
	cb_write.aio_nbytes	= 64;

	ret = waio_read(cx_read,&cb_read);
	if (ret) return -ret;

	timeout.quad = 0;
	cb[0] = &cb_read;
	ret = waio_suspend(cx_read,cb,1,&timeout);
	if (ret != -WAIO_EAGAIN) return NT_STATUS_INTERNAL_ERROR;

	ret = waio_write(cx_write,&cb_write);
	if (ret) return -ret;

	cb[0] = &cb_write;
	ret = waio_suspend(cx_write,cb,1,(waio_timeout *)0);
	if (ret) return -ret;

	cb[0] = &cb_read;

	while (1) {
		ret = waio_suspend(cx_read,cb,1,(waio_timeout *)0);
		if (ret) return -ret;

		bytes = waio_return(cx_read,&cb_read);
		if (bytes < 0) return NT_STATUS_INTERNAL_ERROR;

		ret = waio_read(cx_read,&cb_read);
		if (ret) return -ret;
	}

	ret = waio_free(cx_read);
	if (ret) return -ret;

	ret = waio_free(cx_write);
	if (ret) return -ret;

	return 0;
}
