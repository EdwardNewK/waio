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


/* UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED */
/* UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED */
/* UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED */
/* UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED */
/* UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED UNTESTED */

#include <psxtypes/psxtypes.h>
#include <ntapi/nt_status.h>

#ifndef NTAPI_STATIC
int32_t __fastcall __ntapi_tt_hex_utf8_to_uint32(
	_in_	unsigned char	hex_key_utf8[8],
	_out_	uint32_t *	key)
{
	int		i;
	unsigned char	uch[8];
	unsigned char	ubytes[4];
	uint32_t *	key_ret;

	/* input validation */
	i = 0;
	do {
		if (/* [a-f],[[A-F],[0-9] */
			((hex_key_utf8[i] >= 'a') && (hex_key_utf8[i] <= 'f'))
			|| ((hex_key_utf8[i] >= 'A') && (hex_key_utf8[i] <= 'F'))
			|| ((hex_key_utf8[i] >= '0') && (hex_key_utf8[i] <= '9')))
			/* valid hex character */
			i++;
		else
			return NT_STATUS_ILLEGAL_CHARACTER;
	} while (i < 8);

	/* intermediate step: little endian byte order */
	uch[0] = (unsigned char)hex_key_utf8[6];
	uch[1] = (unsigned char)hex_key_utf8[7];
	uch[2] = (unsigned char)hex_key_utf8[4];
	uch[3] = (unsigned char)hex_key_utf8[5];
	uch[4] = (unsigned char)hex_key_utf8[2];
	uch[5] = (unsigned char)hex_key_utf8[3];
	uch[6] = (unsigned char)hex_key_utf8[0];
	uch[7] = (unsigned char)hex_key_utf8[1];

	for (i=0; i<8; i++) {
		/* 'a' > 'A' > '0' */
		if (uch[i] >= 'a')
			uch[i] -= ('a' - 0x0a);
		else if (uch[i] >= 'A')
			uch[i] -= ('A' - 0x0a);
		else
			uch[i] -= '0';
	}

	ubytes[0] = uch[0] * 0x10 + uch[1];
	ubytes[1] = uch[2] * 0x10 + uch[3];
	ubytes[2] = uch[4] * 0x10 + uch[5];
	ubytes[3] = uch[6] * 0x10 + uch[7];

	key_ret = (uint32_t *)ubytes;
	*key = *key_ret;

	return NT_STATUS_SUCCESS;
}


int32_t __fastcall __ntapi_tt_hex_utf8_to_uint64(
	_in_	unsigned char	hex_key_utf8[16],
	_out_	uint64_t *	key)
{
	int32_t		ntstatus;
	uint32_t	x64_key[2];
	uint64_t *	key_ret;

	ntstatus = __ntapi_tt_hex_utf8_to_uint32(
			&hex_key_utf8[0],
			&x64_key[1]);

	if (ntstatus != NT_STATUS_SUCCESS)
		return ntstatus;

	ntstatus = __ntapi_tt_hex_utf8_to_uint32(
			&hex_key_utf8[8],
			&x64_key[0]);

	if (ntstatus != NT_STATUS_SUCCESS)
		return ntstatus;

	key_ret = (uint64_t *)x64_key;
	*key = *key_ret;

	return NT_STATUS_SUCCESS;
}


int32_t __fastcall __ntapi_tt_hex_utf8_to_uintptr(
	_in_	unsigned char	hex_key_utf8[],
	_out_	uintptr_t *	key)
{
	#if defined (__NT32)
		return __ntapi_tt_hex_utf8_to_uint32(hex_key_utf8,key);
	#elif defined (__NT64)
		return __ntapi_tt_hex_utf8_to_uint64(hex_key_utf8,key);
	#endif
}


int32_t __fastcall __ntapi_tt_hex_utf8_to_uint16(
	_in_	unsigned char	hex_key_utf8[4],
	_out_	uint16_t *	key)
{
	int32_t		ret;
	uint32_t	dword_key;
	unsigned char	hex_buf[8] = {'0','0','0','0'};

	hex_buf[4] = hex_key_utf8[0];
	hex_buf[5] = hex_key_utf8[1];
	hex_buf[6] = hex_key_utf8[2];
	hex_buf[7] = hex_key_utf8[3];

	ret = __ntapi_tt_hex_utf8_to_uint32(hex_buf,&dword_key);

	if (ret == NT_STATUS_SUCCESS)
		*key = (uint16_t)dword_key;

	return ret;
}
#endif /* NTAPI_STATIC */