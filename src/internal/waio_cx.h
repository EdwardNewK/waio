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


#ifndef _WAIO_CX_H_
#define _WAIO_CX_H_

#include <psxtypes/psxtypes.h>
#include <ntapi/ntapi.h>
#include <waio/waio__llapi.h>
#include "waio_impl.h"

#define WAIO_CX_BLOCK_SIZE	4096
#define	WAIO_CX_SLOT_COUNT	4

typedef struct waio_slot_interface		waio_slot;
typedef struct waio_request_interface		waio_request;

typedef struct waio_slot_interface {
	uint32_t		pid;
	uint32_t		tid;
	int			aio_lio_opcode;
	int			aio_reqprio;
	void *			aio_hevent;
	volatile void *		aio_buf;
	size_t			aio_nbytes;
	off_t           	aio_offset;
	struct waio_aiocb *	aiocb;
} waio_slot;


typedef struct waio_request_interface {
	waio_slot		slot;
	waio_packet		rpacket;
	waio_request *		next;
} waio_request;


typedef struct waio_cx_interface {
	struct waio_cx_interface *	self;
	struct waio_interface *		paio;
	size_t				cx_size;
} waio_opaque_cx;


typedef struct waio_aiocb_opaque_interface {
	nt_iosb		iosb;
	void *          hpending;
	waio_request *	request;
	int32_t		qstatus;
	int32_t		fcancel;
	intptr_t	fc_after_io;
	intptr_t	fc_before_io_complete;
} waio_aiocb_opaque;


#endif /* _WAIO_CX_H_ */
