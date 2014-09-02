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


#ifndef _WAIO_XVTBLS_H_
#define _WAIO_XVTBLS_H_

#include <psxtypes/psxtypes.h>
#include <ntapi/ntapi.h>
#include <ntcon/ntcon.h>
#include <winapi/winapi.h>


/* win32 api accessor table */
typedef struct _winapi_vtbl {
	winapi_get_current_thread_id *		get_current_thread_id;
	winapi_get_current_process_id *		get_current_process_id;
	winapi_get_module_handle_utf16 *	get_module_handle_utf16;
	winapi_get_proc_address *		get_proc_address;
	winapi_create_thread *			create_thread;
	winapi_create_pipe *			create_pipe;
} winapi_vtbl;


/* native api accessor table */
typedef struct _ntapi_vtbl {
	/* object */
	ntapi_zw_close *			zw_close;
	/* memory */
	ntapi_zw_allocate_virtual_memory *	zw_allocate_virtual_memory;
	ntapi_zw_free_virtual_memory *		zw_free_virtual_memory;
	ntapi_zw_query_virtual_memory *		zw_query_virtual_memory;
	/* thread */
	ntapi_zw_suspend_thread *		zw_suspend_thread;
	ntapi_zw_terminate_thread *		zw_terminate_thread;
	ntapi_zw_query_information_thread *	zw_query_information_thread;
	/* process */
	ntapi_zw_terminate_process *		zw_terminate_process;
	/* sync */
	ntapi_zw_create_event *			zw_create_event;
	ntapi_zw_set_event *			zw_set_event;
	ntapi_zw_reset_event *			zw_reset_event;
	ntapi_zw_query_event *			zw_query_event;
	ntapi_zw_wait_for_single_object *	zw_wait_for_single_object;
	ntapi_zw_wait_for_multiple_objects *	zw_wait_for_multiple_objects;
	/* file */
	ntapi_zw_query_information_file *	zw_query_information_file;
	ntapi_zw_write_file *			zw_write_file;
	ntapi_zw_read_file *			zw_read_file;
	/* time */
	ntapi_zw_yield_execution *		zw_yield_execution;
	/* string */
	ntapi_memset *				memset;
	ntapi_sprintf *				sprintf;
	ntapi_strlen *				strlen;
	ntapi_wcslen *				wcslen;
	/* extensions */
	ntapi_tt_create_inheritable_event *	tt_create_inheritable_event;
	ntapi_tt_create_private_event *		tt_create_private_event;
	ntapi_tt_uint32_to_hex_utf8 *		tt_uint32_to_hex_utf8;
	/* wine-behavior (now fixed in trunk: thank you!) */
	void *					wine_get_version;
	nt_wait_type				wait_type_any;
	nt_wait_type				wait_type_all;
} ntapi_vtbl;


/* console api accessor table */
typedef struct _ntcon_vtbl {
	ntcon_get_std_handle *		get_std_handle;
	ntcon_get_console_mode *	get_console_mode;
	ntcon_set_console_mode *	set_console_mode;
	ntcon_set_console_code_page *	set_console_code_page;
	ntcon_write_console_ansi * 	write_console_ansi;
	ntcon_write_console_utf16 *	write_console_utf16;
} ntcon_vtbl;


#endif /* _WAIO_XVTBLS_H_ */
