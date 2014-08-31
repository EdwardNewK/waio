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
#include "waio_impl.h"
#include "waio_cx.h"

typedef struct waio_cx_opaque_block {
	struct waio_cx_interface	cx;
	struct waio_interface		cx_waio;
	struct waio_slot_interface	cx_slots[WAIO_CX_SLOT_COUNT_TODO];
	struct waio_request_interface	cx_requests[1];
} waio_cx_block;



waio_api waio_cx waio_alloc(
	_in_	void * 		handle,
	_in_	unsigned int	flags,
	_in_	void * 		options,
	_out_	signed int *	status)
{
	int32_t		_status;
	nt_iosb		iosb;
	nt_fbi		fbi;
	waio_cx_block *	cx_block;
	size_t		cx_block_size;
	waio_hook **	hook;
	int		i;

	/* status */
	if (!status) status = &_status;

	/* xvtbl init */
	*status = waio_xvtbls_init_once();

	if (*status) return (waio_cx)0;

	/* validate handle */
	*status = __ntapi->zw_query_information_file(
		handle,
		&iosb,
		&fbi,
		sizeof(fbi),
		NT_FILE_BASIC_INFORMATION);

	if (*status) return (waio_cx)0;

	/* cx_block alloc */
	cx_block_size = WAIO_CX_BLOCK_SIZE;

	*status = __ntapi->zw_allocate_virtual_memory(
		NT_CURRENT_PROCESS_HANDLE,
		(void **)&cx_block,
		0,
		&cx_block_size,
		NT_MEM_COMMIT,
		NT_PAGE_READWRITE);

	if (*status) return (waio_cx)0;

	/* cx_block->cx init */
	cx_block->cx.self	= &cx_block->cx;
	cx_block->cx.paio	= &cx_block->cx_waio;
	cx_block->cx.free_nodes	= &cx_block->cx_requests[0];
	cx_block->cx.cx_size	= cx_block_size;

	/* cx_block->cx_waio init */
	cx_block->cx_waio.hfile = handle;

	/* default hooks */
	hook = (waio_hook **)&cx_block->cx.paio->hooks;

	for (i=0; i<WAIO_HOOK_CAP; i++, hook++)
		if (!(*hook))
			*hook = waio_hook_default;

	/* todo: free_nodes init */

	/* thread pair init */
	*status = waio_init(cx_block->cx.paio);
	if (*status) return (waio_cx)0;

	/* return opaque pointer */
	return &cx_block->cx;
}
