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

/* accessor tables */
static ntapi_vtbl	_ntapi;
static ntcon_vtbl	_ntcon;
static winapi_vtbl	_winapi;
static waio_xvtbls	_waio_xvtbls;

/* .rdata */
waio_xvtbls *		__waio_xvtbls	      = (waio_xvtbls *)0;
static intptr_t		__waio_init_started   = 0;
static intptr_t		__waio_init_completed = 0;


waio_internal_api
int32_t __stdcall winapi_init(winapi_vtbl * pvtbl)
{
	/** with virtually no exceptions, the kernel32 ansi functions
	 *  are wrappers around the utf16 functions; calling the utf16
	 *  functions is therefore the cleaner, faster, and better way
	 *  to go.
	**/
	__attr_import__ winapi_get_module_handle_utf16	GetModuleHandleW;
	__attr_import__ winapi_get_proc_address		GetProcAddress;

	wchar16_t ntdll_module_name[]    = {'n','t','d','l','l','.','d','l','l',0};
	wchar16_t kernel32_module_name[] = {'k','e','r','n','e','l','3','2','.','d','l','l',0};

	/* hntdll, hkernel32 */
	__hntdll	= GetModuleHandleW(ntdll_module_name);
	__hkernel32	= GetModuleHandleW(kernel32_module_name);

	if ((!__hntdll) || (!__hkernel32))
		return NT_STATUS_DLL_INIT_FAILED;

	/* imported functions (PE export table is always ansi/ascii) */
	pvtbl->get_current_thread_id	= (winapi_get_current_thread_id *)GetProcAddress(__hkernel32,"GetCurrentThreadId ");
	pvtbl->get_current_process_id	= (winapi_get_current_process_id *)GetProcAddress(__hkernel32,"GetCurrentProcessId");
	pvtbl->get_module_handle_utf16	= (winapi_get_module_handle_utf16 *)GetProcAddress(__hkernel32,"GetModuleHandleW");
	pvtbl->get_proc_address		= (winapi_get_proc_address *)GetProcAddress(__hkernel32,"GetProcAddress");
	pvtbl->create_thread		= (winapi_create_thread *)GetProcAddress(__hkernel32,"CreateThread");
	pvtbl->create_pipe		= (winapi_create_pipe *)GetProcAddress(__hkernel32,"CreatePipe");

	/* verification */
	if ((!__winapi->get_module_handle_utf16) \
			|| (!__winapi->get_proc_address) \
			|| (!__winapi->create_thread) \
			|| (!__winapi->create_pipe))
		return NT_STATUS_DLL_INIT_FAILED;
	/* done */
	return NT_STATUS_SUCCESS;
}


waio_internal_api
int32_t __stdcall ntapi_init(ntapi_vtbl * pvtbl)
{
	#undef  __get_proc_address
	#define __get_proc_address(vtbl,member,symbol) do { \
		vtbl->member = (ntapi_##member *)__winapi->get_proc_address(__hntdll,symbol); \
		if (!vtbl->member) return NT_STATUS_DLL_INIT_FAILED; \
	} while (0);

	/* object */
	__get_proc_address(pvtbl,zw_close,"ZwClose");
	/* memory */
	__get_proc_address(pvtbl,zw_allocate_virtual_memory,"ZwAllocateVirtualMemory");
	__get_proc_address(pvtbl,zw_free_virtual_memory,"ZwFreeVirtualMemory");
	__get_proc_address(pvtbl,zw_query_virtual_memory,"ZwQueryVirtualMemory");
	/* thread */
	__get_proc_address(pvtbl,zw_suspend_thread,"ZwSuspendThread");
	__get_proc_address(pvtbl,zw_terminate_thread,"ZwTerminateThread");
	__get_proc_address(pvtbl,zw_query_information_thread,"ZwQueryInformationThread");
	/* process */
	__get_proc_address(pvtbl,zw_terminate_process,"ZwTerminateProcess");
	/* sync */
	__get_proc_address(pvtbl,zw_create_event,"ZwCreateEvent");
	__get_proc_address(pvtbl,zw_set_event,"ZwSetEvent");
	__get_proc_address(pvtbl,zw_reset_event,"ZwResetEvent");
	__get_proc_address(pvtbl,zw_query_event,"ZwQueryEvent");
	__get_proc_address(pvtbl,zw_wait_for_single_object,"ZwWaitForSingleObject");
	__get_proc_address(pvtbl,zw_wait_for_multiple_objects,"ZwWaitForMultipleObjects");
	/* file */
	__get_proc_address(pvtbl,zw_query_information_file,"ZwQueryInformationFile");
	__get_proc_address(pvtbl,zw_write_file,"ZwWriteFile");
	__get_proc_address(pvtbl,zw_read_file,"ZwReadFile");
	/* time */
	__get_proc_address(pvtbl,zw_yield_execution,"ZwYieldExecution");
	/* string */
	__get_proc_address(pvtbl,memset,"memset");
	__get_proc_address(pvtbl,sprintf,"sprintf");
	__get_proc_address(pvtbl,strlen,"strlen");
	__get_proc_address(pvtbl,wcslen,"wcslen");

	/* zw_cancel_io_file_ex (newer os versions only) */
	pvtbl->zw_cancel_io_file_ex = (ntapi_zw_cancel_io_file_ex *)
					__winapi->get_proc_address(
						__hntdll,
						"ZwCancelIoFileEx");

	/* wine */
	pvtbl->wine_get_version = __winapi->get_proc_address(__hntdll,"wine_get_version");

	/* done */
	return NT_STATUS_SUCCESS;
}


waio_internal_api
int32_t __stdcall ntcon_init(ntcon_vtbl * pvtbl)
{
	#undef  __get_proc_address
	#define __get_proc_address(vtbl,member,symbol) do { \
		vtbl->member = (ntcon_##member *)__winapi->get_proc_address(__hkernel32,symbol); \
		if (!vtbl->member) return NT_STATUS_DLL_INIT_FAILED; \
	} while (0);

	/* tty ioctl */
	__get_proc_address(pvtbl,get_std_handle,"GetStdHandle");
	__get_proc_address(pvtbl,get_console_mode,"GetConsoleMode");
	__get_proc_address(pvtbl,set_console_mode,"SetConsoleMode");
	__get_proc_address(pvtbl,set_console_code_page,"SetConsoleCP");
	/* streams */
	__get_proc_address(pvtbl,write_console_ansi,"WriteConsoleA");
	__get_proc_address(pvtbl,write_console_utf16,"WriteConsoleW");

	/* done */
	return NT_STATUS_SUCCESS;
}


waio_internal_api
int32_t __stdcall waio_xvtbls_init(waio_xvtbls * pxvtbls)
{
	int32_t status;

	/* internal pointer to accessor tables */
	pxvtbls->ntapi	= &_ntapi;
	pxvtbls->ntcon	= &_ntcon;
	pxvtbls->winapi	= &_winapi;

	/* vtbl init */
	if (winapi_init(pxvtbls->winapi) != NT_STATUS_SUCCESS)
		return NT_STATUS_DLL_INIT_FAILED;

	if (ntapi_init(pxvtbls->ntapi) != NT_STATUS_SUCCESS)
		return NT_STATUS_DLL_INIT_FAILED;

	if (ntcon_init(pxvtbls->ntcon) != NT_STATUS_SUCCESS)
		return NT_STATUS_DLL_INIT_FAILED;

	/* extensions */
	__ntapi->tt_uint32_to_hex_utf8		= __ntapi_tt_uint32_to_hex_utf8;
	__ntapi->tt_create_inheritable_event	= __ntapi_tt_create_inheritable_event;
	__ntapi->tt_create_private_event	= __ntapi_tt_create_private_event;

	/* wine */
	if (1 || __ntapi->wine_get_version) {
		status = ntapi_detect_wine_behavior(__ntapi);
	} else {
		__ntapi->wait_type_any = NT_WAIT_ANY;
		__ntapi->wait_type_all = NT_WAIT_ALL;
		status = NT_STATUS_SUCCESS;
	}

	return status;
}


waio_internal_api
int32_t __stdcall waio_xvtbls_init_once(void)
{
	void *					hntdll;
	intptr_t				init_started;
	void *					hevent;
	ntapi_zw_create_event *			pfn_create_event;
	ntapi_zw_wait_for_single_object *	pfn_wait_for_single_object;
	ntapi_zw_close *			pfn_close;
	os_timeout				timeout;
	uintptr_t				tries;
	int32_t					status;

	__attr_import__ winapi_get_module_handle_utf16	GetModuleHandleW;
	__attr_import__ winapi_get_proc_address		GetProcAddress;

	wchar16_t ntdll_module_name[]    = {'n','t','d','l','l','.','d','l','l',0};

	/* init once completed? */
	if (__waio_init_completed)
		return NT_STATUS_SUCCESS;

	/* init once started? */
	init_started = at_locked_cas(&__waio_init_started,0,1);

	if (init_started) {
		/* private pointers */
		hntdll			   = GetModuleHandleW(ntdll_module_name);
		pfn_create_event	   = (ntapi_zw_create_event *)GetProcAddress(hntdll,"ZwCreateEvent");
		pfn_wait_for_single_object = (ntapi_zw_wait_for_single_object *)GetProcAddress(hntdll,"ZwWaitForSingleObject");
		pfn_close		   = (ntapi_zw_close *)GetProcAddress(hntdll,"ZwClose");

		if ((!hntdll) || (!pfn_create_event) || (!pfn_wait_for_single_object) || (!pfn_close))
			return NT_STATUS_DLL_INIT_FAILED;

		/* save two system calls if possible */
		if (__waio_init_completed)
			return NT_STATUS_SUCCESS;

		status = pfn_create_event(
			&hevent,
			NT_EVENT_ALL_ACCESS,
			(nt_oa *)0,
			NT_NOTIFICATION_EVENT,
			NT_EVENT_NOT_SIGNALED);

		if (status) return status;

		/* wait for init to complete (~1 minute) using 100usec intervals */
		timeout.quad = (-1) * 1000;
		tries = 0;

		while ((__waio_init_completed == 0) && (tries < 0x2000)) {
			pfn_wait_for_single_object(hevent,NT_SYNC_NON_ALERTABLE,&timeout);
			tries++;
		}

		/* clean-up */
		pfn_close(hevent);

		/* verify & return */
		return __waio_init_completed
			? NT_STATUS_SUCCESS
			: NT_STATUS_DLL_INIT_FAILED;
	}

	/* init once should be performed by this thread */
	__waio_xvtbls = &_waio_xvtbls;
	status = waio_xvtbls_init(&_waio_xvtbls);

	if (status == NT_STATUS_SUCCESS)
		__waio_init_completed = 1;

	return status;
}


waio_internal_api
int32_t __stdcall ntapi_detect_wine_behavior(ntapi_vtbl * pvtbl)
{
	int32_t		status;
	void *		hevent[2];
	nt_timeout	timeout;

	/* create two events */
	status = pvtbl->tt_create_private_event(
		&hevent[0],
		NT_NOTIFICATION_EVENT,
		NT_EVENT_NOT_SIGNALED);

	if (status) return (status);

	status = pvtbl->tt_create_private_event(
		&hevent[1],
		NT_NOTIFICATION_EVENT,
		NT_EVENT_NOT_SIGNALED);

	if (status) {
		pvtbl->zw_close(hevent[0]);
		return (status);
	}

	/* set one of them */
	status = pvtbl->zw_set_event(hevent[0],(int32_t *)0);

	/* detect behavior (reversed flags fixed in wine on 02.09.2014) */
	timeout.quad = (-1);

	status = pvtbl->zw_wait_for_multiple_objects(
		2,
		hevent,
		NT_WAIT_ANY,
		NT_SYNC_NON_ALERTABLE,
		&timeout);

	/* set flags accordingly */
	if (status == NT_STATUS_SUCCESS) {
		/* compatible flags */
		pvtbl->wait_type_any = NT_WAIT_ANY;
		pvtbl->wait_type_all = NT_WAIT_ALL;
	} else if (status == NT_STATUS_TIMEOUT) {
		/* reversed flags */
		pvtbl->wait_type_any = NT_WAIT_ALL;
		pvtbl->wait_type_all = NT_WAIT_ANY;
		status = NT_STATUS_SUCCESS;
	}

	/* clean-up & return */
	pvtbl->zw_close(hevent[0]);
	pvtbl->zw_close(hevent[1]);

	return status;
}

waio_internal_api
int __stdcall waio_lib_entry_point(void * hinstance, uint32_t reason, void * reserved)
{
	return 1;
}
