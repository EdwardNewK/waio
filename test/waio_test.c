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
#include <waio/waio__llapi.h>
#include <waio/waio.h>
#include "waio_impl.h"
#include "waio_test.h"

/* Wild & Asynchronous I/O Library: test unit */

/* .rdata */
waio_xvtbls	xvtbls = {0};
waio		pipe_pool[WAIO_POOL_SIZE] = {{0}};
void *		hevent_abort_request;
void *		hstdout;

char * waio_test_hook_strings[WAIO_HOOK_CAP] = {
	"WAIO_HOOK_BEFORE_INIT",
	"WAIO_HOOK_AFTER_INIT",
	"WAIO_HOOK_BEFORE_CANCEL",
	"WAIO_HOOK_AFTER_CANCEL",
	"WAIO_HOOK_BEFORE_SHUTDOWN_REQUEST",
	"WAIO_HOOK_AFTER_SHUTDOWN_REQUEST",
	"WAIO_HOOK_BEFORE_SHUTDOWN_FALLBACK",
	"WAIO_HOOK_AFTER_SHUTDOWN_FALLBACK",
	"WAIO_HOOK_BEFORE_SHUTDOWN_RESPONSE",
	"WAIO_HOOK_AFTER_SHUTDOWN_RESPONSE",
	"WAIO_HOOK_BEFORE_IO_REQUEST",
	"WAIO_HOOK_AFTER_IO_REQUEST",
	"WAIO_HOOK_BEFORE_IO",
	"WAIO_HOOK_AFTER_IO",
	"WAIO_HOOK_BEFORE_IO_COMPLETE",
	"WAIO_HOOK_AFTER_IO_COMPLETE",
	"WAIO_HOOK_ON_TIMEOUT",
	"WAIO_HOOK_ON_CANCEL",
	"WAIO_HOOK_ON_FAILURE",
	"WAIO_HOOK_ON_QUERY"
};


int32_t __stdcall waio_test_default_hook(
	_in_		waio *		paio,
	_in_		waio_hook_type	type,
	_in_		int32_t		status)
{
	char buffer[32];

	__ntapi->memset(buffer,0,32);

	waio_test_output(
		hstdout,
		waio_test_hook_strings[type],
		__ntapi->strlen(waio_test_hook_strings[type]));

	waio_test_output(hstdout,"\n",1);

	if ((type == WAIO_HOOK_ON_QUERY) || (type == WAIO_HOOK_ON_FAILURE)) {
		buffer[0]  = '0';
		buffer[1]  = 'x';
		buffer[10] = '\n';

		__ntapi->tt_uint32_to_hex_utf8(
			status,
			&buffer[2]);

		waio_test_output(
			hstdout,
			buffer,
			11);
	}

	return status;
}


/* main */
int __cdecl waio_main_utf8(int argc, char ** argv, char ** envp)
{
	int ret;

	/* return waio_test_wine_behavior(); */

	hstdout = __ntcon->get_std_handle(NT_STD_OUTPUT_HANDLE);
	ret = waio_test_pipes(WAIO_POOL_SIZE,0,(void *)0);
	return ret;
}


/* _start */
int waio_tu_entry_point(void)
{
	int32_t status;

	/* xvtbl init (library) */
	waio_alloc((void *)0,0,(void *)0,&status);

	/* xvtbl init (test unit) */
	if (waio_xvtbls_init(&xvtbls) != NT_STATUS_SUCCESS)
		return NT_STATUS_DLL_INIT_FAILED;

	/* invoke waio_tu_main_utf8, then exit cleanly */
	__ntapi->zw_terminate_process(
		NT_CURRENT_PROCESS_HANDLE,
		waio_main_utf8(0,0,0));

	/* should never get here */
	return NT_STATUS_INTERNAL_ERROR;
}
