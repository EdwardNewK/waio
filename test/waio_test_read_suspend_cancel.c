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

int __cdecl waio_test_read_suspend_cancel(void)
{
	int			ret;
	waio_cx 		cx_read;
	void *			hread;
	void *			hsignal;
	struct waio_aiocb	cb_read;
	const struct waio_aiocb*cb[1];
	ssize_t			bytes;
	char			read_buf[128];

	__ntapi->memset(read_buf,0xffffffff,128);
	__ntapi->memset(&cb_read,0,sizeof(cb_read));

	hread = __ntcon->get_std_handle(NT_STD_INPUT_HANDLE);
	cx_read = waio_alloc(hread, 0, (void *)0, &ret);
	if (!cx_read) return ret;

	__ntapi->tt_create_private_event(
		&hsignal,
		NT_NOTIFICATION_EVENT,
		NT_EVENT_NOT_SIGNALED);

	waio_fcntl(cx_read,(struct waio_aiocb *)0,WAIO_FCNTL_QUERY_SET_HOOK,0,waio_test_query_hook,(void *)0);

	cb_read.hsignal		= hsignal;
	cb_read.aio_buf		= read_buf;
	cb_read.aio_nbytes	= 8;
	cb[0] = &cb_read;

	while (1) {
		ret = waio_read(cx_read,&cb_read);
		waio_test_query_hook((waio *)0, 0x11111111,ret);
		if (ret) return -ret;

		waio_test_query_hook((waio *)0, 0x99999999,read_buf[0]);

		if (read_buf[0] == 'c') {
			waio_test_query_hook((waio *)0, 0x44444444,(signed int)bytes);
			waio_cancel(cx_read,&cb_read);
		}

		ret = waio_suspend(cx_read,cb,1,(waio_timeout *)0);
		waio_test_query_hook((waio *)0, 0x22222222,ret);
		if (ret) break;

		bytes = waio_return(cx_read,&cb_read);
		waio_test_query_hook((waio *)0, 0x33333333,(signed int)bytes);
		if (bytes < 0) return NT_STATUS_INTERNAL_ERROR;

		if (read_buf[0] == 'q')
			__ntapi->zw_set_event(
				hsignal,
				(int32_t *)0);
	}

	ret = waio_free(cx_read);
	if (ret) return -ret;

	return 0;
}
