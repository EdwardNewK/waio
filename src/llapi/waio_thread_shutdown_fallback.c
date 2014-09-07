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
#include <waio/waio__llapi.h>
#include "waio_impl.h"

/****************************************************************/
/* FALLBACK METHOD FOR LEGACY OS VERSIONS (normally pre-vista)	*/
/* -----------------------------------------------------------	*/
/* 1. suspend the reader (child) thread				*/
/* 2. query the thread's stack					*/
/* 3. terminate the thread					*/
/* 4. deallocate the thread's stack				*/
/*								*/
/****************************************************************/

/* the thread shutdown request executes in the context of the parent */
waio_internal_api
int32_t __stdcall waio_thread_shutdown_fallback(waio * paio)
{
	uint32_t			prev_suspend_count;
	nt_thread_basic_information	tinfo;
	int32_t				status;
	size_t				info_size;
	nt_memory_basic_information	mbi;

	/* hook */
	paio->hooks[WAIO_HOOK_BEFORE_SHUTDOWN_FALLBACK](paio,WAIO_HOOK_BEFORE_SHUTDOWN_FALLBACK,0);

	/* suspend the reader (child) thread */
	status = __ntapi->zw_suspend_thread(
		paio->hthread_io,
		&prev_suspend_count);

	if (status) {
		if (status == NT_STATUS_THREAD_IS_TERMINATING)
			return NT_STATUS_SUCCESS;

		status = __ntapi->zw_query_information_thread(
			paio->hthread_io,
			NT_THREAD_BASIC_INFORMATION,
			&tinfo,
			sizeof(tinfo),
			&info_size);

		if (status) {
			paio->status_loop = status;
			return status;
		} else if (tinfo.exit_status != NT_STATUS_PENDING) {
			/* the thread has already terminated */
			return NT_STATUS_SUCCESS;
		}
	}

	/* query the thread's stack */
	paio->status_loop = __ntapi->zw_query_virtual_memory(
		NT_CURRENT_PROCESS_HANDLE,
		paio->fallback_tip,
		NT_MEMORY_BASIC_INFORMATION,
		&mbi,
		sizeof(nt_memory_basic_information),
		(size_t *)0);

	if (paio->status_loop) return paio->status_loop;

	/* terminate the thread */
	paio->status_loop = __ntapi->zw_terminate_thread(
		paio->hthread_io,
		NT_STATUS_REQUEST_ABORTED);

	if (paio->status_loop) return paio->status_loop;

	/* deallocate the thread's stack */
	paio->status_loop = __ntapi->zw_free_virtual_memory(
		NT_CURRENT_PROCESS_HANDLE,
		&mbi.allocation_base,
		&mbi.region_size,
		NT_MEM_RELEASE);

	paio->hooks[WAIO_HOOK_AFTER_SHUTDOWN_FALLBACK](paio,WAIO_HOOK_AFTER_SHUTDOWN_FALLBACK,0);

	/* return control to synchronization loop */
	return paio->status_loop;
}
