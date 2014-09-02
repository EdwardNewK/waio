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

/* UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED */
/* UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED */
/* UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED */
/* UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED */
/* UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED */

#include <psxtypes/psxtypes.h>

#ifndef NTAPI_STATIC
static void __fastcall __ntapi_tt_uint_to_hex_utf8(
	_in_	uint64_t	key,
	_out_	char *		key_formatted,
	_in_	unsigned	bits)
{
	unsigned	i;
	uint32_t	hex_buf[4];
	unsigned char *	hex_chars;
	unsigned char *	uch;
	unsigned	offset;
	unsigned	bytes;

	/* avoid using .rdata for that one */
	hex_buf[0] = ('3' << 24) | ('2' << 16) | ('1' << 8) | '0';
	hex_buf[1] = ('7' << 24) | ('6' << 16) | ('5' << 8) | '4';
	hex_buf[2] = ('B' << 24) | ('A' << 16) | ('9' << 8) | '8';
	hex_buf[3] = ('F' << 24) | ('E' << 16) | ('D' << 8) | 'C';

	uch = (unsigned char *)&key;
	hex_chars = (unsigned char *)&hex_buf;

	bytes  = bits / 8;
	offset = bits / 4;

	for (i = 0; i < bytes; i++) {
		key_formatted[offset - 1 - (i*2)] =	hex_chars[uch[i] % 16];
		key_formatted[offset - 2 - (i*2)] =	hex_chars[uch[i] / 16];
	}
}


void __fastcall __ntapi_tt_uint16_to_hex_utf8(
	_in_	uint32_t	key,
	_out_	char *		key_formatted)
{
	__ntapi_tt_uint_to_hex_utf8(
		key,
		key_formatted,
		16);
}


void __fastcall __ntapi_tt_uint32_to_hex_utf8(
	_in_	uint32_t	key,
	_out_	char *		key_formatted)
{
	__ntapi_tt_uint_to_hex_utf8(
		key,
		key_formatted,
		32);
}


void __fastcall __ntapi_tt_uint64_to_hex_utf8(
	_in_	uint64_t	key,
	_out_	char *		key_formatted)
{
	__ntapi_tt_uint_to_hex_utf8(
		key,
		key_formatted,
		64);
}


void __fastcall __ntapi_tt_uintptr_to_hex_utf8(
	_in_	uintptr_t	key,
	_out_	char *		key_formatted)
{
	#if defined (__NT32)
		__ntapi_tt_uint_to_hex_utf8(
			key,
			key_formatted,
			32);
	#elif defined (__NT64)
		__ntapi_tt_uint_to_hex_utf8(
			key,
			key_formatted,
			64);
	#endif
}
#endif /* NTAPI_STATIC */