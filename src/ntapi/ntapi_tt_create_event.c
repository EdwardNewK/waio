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
#include <ntapi/nt_object.h>
#include <ntapi/nt_sync.h>
#include <ntapi/ntapi.h>
#include "waio_impl.h"

#ifndef NTAPI_STATIC
static int32_t __cdecl __tt_create_event(
	_out_	void **		hevent,
	_in_ 	nt_event_type	event_type,
	_in_ 	int32_t		initial_state,
	_in_	uint32_t	obj_attr)
{
	int32_t	status;
	nt_sqos	sqos;
	nt_oa	oa;

	/* sanity */
	if (!hevent)
		return NT_STATUS_INVALID_PARAMETER;

	/* security structure */
	sqos.length 			= sizeof(sqos);
	sqos.impersonation_level	= NT_SECURITY_IMPERSONATION;
	sqos.context_tracking_mode	= NT_SECURITY_TRACKING_DYNAMIC;
	sqos.effective_only		= 1;

	/* object attributes */
	oa.len		= sizeof(nt_object_attributes);
	oa.root_dir	= (void *)0;
	oa.obj_name	= (nt_unicode_string *)0;
	oa.obj_attr	= obj_attr;
	oa.sec_desc	= (nt_security_descriptor *)0; /**/
	oa.sec_qos	= &sqos;

	status = __ntapi->zw_create_event(
		hevent,
		NT_EVENT_ALL_ACCESS,
		&oa,
		event_type,
		initial_state);

	return status;
}


waio_internal_api
int32_t __stdcall __ntapi_tt_create_inheritable_event(
	_out_	void **		hevent,
	_in_ 	nt_event_type	event_type,
	_in_ 	int32_t		initial_state)
{
	return __tt_create_event(
		hevent,
		event_type,
		initial_state,
		NT_OBJ_INHERIT);
}


waio_internal_api
int32_t __stdcall __ntapi_tt_create_private_event(
	_out_	void **		hevent,
	_in_ 	nt_event_type	event_type,
	_in_ 	int32_t		initial_state)
{
	return __tt_create_event(
		hevent,
		event_type,
		initial_state,
		0);
}
#endif /* NTAPI_STATIC */
