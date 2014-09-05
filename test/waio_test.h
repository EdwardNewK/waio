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

/* Wild & Asynchronous I/O Library: test unit */

/* testing options */
#define waio_POOL_SIZE		256
#define WAIO_READ_BUFFER_SIZE	4096

/* forward declarations */
int __cdecl waio_tu_entry_point(void);
int __cdecl waio_main_utf8(int argc, char ** argv, char ** envp);
int __cdecl waio_test_pipe(waio *);
int __cdecl waio_test_pipes(unsigned int, uint32_t, void *);
waio_hook waio_test_default_hook;

uint32_t __cdecl waio_test_output(
	_in_	void *	hconsole,
	_in_	char *	buffer,
	_in_	size_t	count);

/* wine testing */
int __cdecl waio_test_wine_behavior(void);
