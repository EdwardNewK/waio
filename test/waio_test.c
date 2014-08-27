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
#include "waio_impl.h"
#include "waio_test.h"

/* Wild & Asynchronous I/O Library: test unit */

/* .rdata */
static waio_xvtbls	xvtbls = {0};
static waio	pipe_pool[waio_POOL_SIZE] = {{0}};
static void *		hevent_abort_request;
static void *		hstdout;


char * waio_test_hook_strings[WAIO_HOOK_CAP] = {
	"WAIO_HOOK_BEFORE_PIPE_INIT",
	"WAIO_HOOK_AFTER_PIPE_INIT",
	"WAIO_HOOK_BEFORE_SHUTDOWN_REQUEST",
	"WAIO_HOOK_AFTER_SHUTDOWN_REQUEST",
	"WAIO_HOOK_BEFORE_SHUTDOWN_RESPONSE",
	"WAIO_HOOK_AFTER_SHUTDOWN_RESPONSE",
	"WAIO_HOOK_BEFORE_READ_REQUEST",
	"WAIO_HOOK_AFTER_READ_REQUEST",
	"WAIO_HOOK_BEFORE_READ",
	"WAIO_HOOK_AFTER_READ",
	"WAIO_HOOK_BEFORE_DATA_RECEIVED",
	"WAIO_HOOK_AFTER_DATA_RECEIVED",
	"WAIO_HOOK_BEFORE_DATA_PROCESSED",
	"WAIO_HOOK_AFTER_DATA_PROCESSED",
	"WAIO_HOOK_BEFORE_WRITE_REQUEST",
	"WAIO_HOOK_AFTER_WRITE_REQUEST",
	"WAIO_HOOK_BEFORE_WRITE",
	"WAIO_HOOK_AFTER_WRITE",
	"WAIO_HOOK_BEFORE_DATA_WRITTEN",
	"WAIO_HOOK_AFTER_DATA_WRITTEN",
	"WAIO_HOOK_ON_TIMEOUT",
	"WAIO_HOOK_ON_FAILURE",
	"WAIO_HOOK_ON_QUERY",
};


int32_t __stdcall waio_test_default_hook(
	_in_		waio *		paio,
	_in_		waio_hook_type	type,
	_in_		int32_t			status)
{
	if (type == WAIO_HOOK_ON_FAILURE)
		status = status;

	waio_test_output(
		hstdout,
		waio_test_hook_strings[type],
		__ntapi->strlen(waio_test_hook_strings[type]));

	waio_test_output(hstdout,"\n",1);

	return status;
}


int __cdecl waio_test_pipe(waio * paio)
{
	int		i;
	waio_hook **	hook;
	int32_t		status;
	size_t		buffer_size = WAIO_READ_BUFFER_SIZE;

	/* init the message structure */
	__ntapi->memset(paio,0,sizeof(*paio));

	/* allocate buffer */
	status = __ntapi->zw_allocate_virtual_memory(
		NT_CURRENT_PROCESS_HANDLE,
		(void **)&paio->packet.data,
		0,
		&buffer_size,
		NT_MEM_COMMIT,
		NT_PAGE_READWRITE);

	if (status) return status;

	/* kernel32: create pipe: will always be a blocking one... */
	status = __winapi->create_pipe(
		&paio->hread,
		&paio->hwrite,
		(nt_sa *)0,
		0x1000);

	if ((!paio->hread) || (!paio->hwrite))
		return NT_STATUS_PIPE_NOT_AVAILABLE;

	/* pass options to pipe */
	paio->hevent_abort_request = hevent_abort_request;

	/* default hooks */
	hook = (waio_hook **)&paio->hooks;

	for (i=0; i<WAIO_HOOK_CAP; i++, hook++)
		if (!(*hook))
			*hook = waio_test_default_hook;

	/* create the thread pair and leave */
	return waio_init(paio);
}


/* main */
int __cdecl waio_test_pipes(unsigned int pool_size, uint32_t flags, void * options)
{
	typedef int32_t __stdcall ntapi_zw_yield_execution(void);

	int i, status;
	nt_timeout timeout;
	ntapi_zw_yield_execution * pfn_yield;

	/* pfn_yield */
	pfn_yield = (ntapi_zw_yield_execution *)__winapi->get_proc_address(
		__hntdll,
		"ZwYieldExecution");

	/* create (global) abort request event */
	status = __ntapi->tt_create_private_event(
		&hevent_abort_request,
		NT_NOTIFICATION_EVENT,
		NT_EVENT_NOT_SIGNALED);

	if (status) return status;

	/* test loop */
	for (i=0; i<waio_POOL_SIZE; i++) {
		waio_test_pipe(&pipe_pool[i]);
		pfn_yield();
	}

	/* allow everyone to block */
	timeout.quad = (long long)1 * (-1) * 10 * 1000 * 1000;
	timeout.quad = 0;

	__ntapi->zw_wait_for_single_object(
		hevent_abort_request,
		NT_SYNC_NON_ALERTABLE,
		&timeout);

	/* ask everyone to abort */
	__ntapi->zw_set_event(
		hevent_abort_request,
		(uint32_t *)0);

	/* see that everyone has aborted */
	for (i=0; i<waio_POOL_SIZE; i++) {
		status = __ntapi->zw_wait_for_single_object(
			pipe_pool[i].hthread_io,
			NT_SYNC_NON_ALERTABLE,
			(nt_timeout *)0);

		if (status)
			return status;

		status = __ntapi->zw_wait_for_single_object(
			pipe_pool[i].hthread_loop,
			NT_SYNC_NON_ALERTABLE,
			(nt_timeout *)0);

		if (status)
			return status;
	}

	/* done */
	return 0;
}



/* main */
int __cdecl waio_main_utf8(int argc, char ** argv, char ** envp)
{
	int ret;

	/* return waio_test_wine_behavior(); */

	hstdout = __ntcon->get_std_handle(NT_STD_OUTPUT_HANDLE);
	ret = waio_test_pipes(waio_POOL_SIZE,0,(void *)0);
	return ret;
}


/* _start */
int waio_tu_entry_point(void)
{
	/* xvtbl init */
	if (waio_xvtbls_init(&xvtbls) != NT_STATUS_SUCCESS)
		return NT_STATUS_DLL_INIT_FAILED;

	/* invoke waio_tu_main_utf8, then exit cleanly */
	__ntapi->zw_terminate_process(
		NT_CURRENT_PROCESS_HANDLE,
		waio_main_utf8(0,0,0));

	/* should never get here */
	return NT_STATUS_INTERNAL_ERROR;
}