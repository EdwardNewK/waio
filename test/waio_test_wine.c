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
#include "waio_test.h"

int __cdecl waio_test_wine_behavior(void)
{
	void *		hevent;
	nt_timeout	timeout;
	int32_t		status;
	int32_t		previous_state;

	/* create event */
	status = __ntapi->tt_create_private_event(
		&hevent,
		NT_NOTIFICATION_EVENT,
		NT_EVENT_NOT_SIGNALED);

	if (status) return 1;

	/* wait for one second */
	timeout.quad = (-1) * 10 * 1000 * 1000;

	status = __ntapi->zw_wait_for_single_object(
		hevent,
		NT_SYNC_NON_ALERTABLE,
		&timeout);

	if (status != NT_STATUS_TIMEOUT) return 2;

	/* set the event */
	status = __ntapi->zw_set_event(
		hevent,
		&previous_state);

	if (status) return 3;

	/* wait again */
	status = __ntapi->zw_wait_for_single_object(
		hevent,
		NT_SYNC_NON_ALERTABLE,
		&timeout);

	if (status) return 4;

	/* all good */
	return 77;
}
