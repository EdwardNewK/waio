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
#include <ntcon/ntcon.h>
#include <winapi/winapi.h>
#include <waio/waio__llapi.h>
#include <waio/waio.h>
#include "waio_impl.h"
#include "waio_test.h"

/* Wild & Asynchronous I/O Library: test unit */

int __cdecl waio_test_pipe(waio * paio)
{
	int		i;
	waio_hook **	hook;
	void *		hwrite;
	nt_fsi		fsi;
	int32_t		status;

	/* init the message structure */
	__ntapi->memset(paio,0,sizeof(*paio));

	/* use the internal packet and cancel_io structures */
	paio->packet    = &paio->lpacket;

	/* allocate buffer */
	paio->packet->buffer_size = WAIO_READ_BUFFER_SIZE;

	status = __ntapi->zw_allocate_virtual_memory(
		NT_CURRENT_PROCESS_HANDLE,
		(void **)&paio->packet->data,
		0,
		&paio->packet->buffer_size,
		NT_MEM_COMMIT,
		NT_PAGE_READWRITE);

	if (status) return status;

	/* kernel32: create pipe: will always be a blocking one... */
	status = __winapi->create_pipe(
		&paio->hfile,
		&hwrite,
		(nt_sa *)0,
		0x1000);

	if ((!paio->hfile) || (!hwrite))
		return NT_STATUS_PIPE_NOT_AVAILABLE;

	/* test basic file information */
	status = __ntapi->zw_query_information_file(
		paio->hfile,
		&paio->lpacket.iosb,
		&fsi,
		sizeof(fsi),
		NT_FILE_STANDARD_INFORMATION);

	if (status) return status;

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
	for (i=0; i<WAIO_POOL_SIZE; i++) {
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
	for (i=0; i<WAIO_POOL_SIZE; i++)
		at_locked_inc(&pipe_pool[i].abort_counter);

	__ntapi->zw_set_event(
		hevent_abort_request,
		(int32_t *)0);

	/* see that everyone has aborted */
	for (i=0; i<WAIO_POOL_SIZE; i++) {
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
