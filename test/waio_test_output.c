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
#include "waio_impl.h"


static int32_t __stdcall __ntcon_write_console_ansi(
	_in_		void *		hconsole,
	_in_		void * const	buffer,
	_in_		uint32_t	bytes_sent,
	_out_		uint32_t *	bytes_written,
	_in_out_	void *		overlapped	_optional_)
{
	uint32_t		mode;
	nt_io_status_block	iosb;
	int32_t			status;

	/* bytes_written: fallback address */
	if (!bytes_written)
		bytes_written = (uint32_t *)&iosb.info;

	/* write method */
	if (__ntcon->get_console_mode(hconsole, &mode))
		return __ntcon->write_console_ansi(
			hconsole,
			buffer,
			bytes_sent,
			bytes_written,
			overlapped);
	else {
		status = __ntapi->zw_write_file(
			hconsole,
			(void *)0,
			(void *)0,
			(void *)0,
			&iosb,
			buffer,
			bytes_sent,
			(nt_large_integer *)0,
			(uint32_t *)0);

		/* bytes_written: either the user's, or the local fallback */
		*bytes_written = (uint32_t)iosb.info;

		/* console commands: non-zero on success, zero on failure */
		if ((iosb.status == 0) && (status == NT_STATUS_SUCCESS))
			return 1;
		else
			return 0;
	}
}


uint32_t __cdecl waio_test_output(void * hconsole, char * buffer, size_t count)
{
	uint32_t 	chars_written = 0;

	__ntcon_write_console_ansi(hconsole,
			buffer,
			(uint32_t)count,
			&chars_written,
			(void *)0);

	return chars_written;
}
