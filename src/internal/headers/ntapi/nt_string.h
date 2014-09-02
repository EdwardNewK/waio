#ifndef _NT_STRING_H_
#define _NT_STRING_H_

#include <psxtypes/psxtypes.h>
#include "nt_object.h"

typedef void * __cdecl ntapi_memset(
	void *dest,
	int c,
	size_t count);


typedef int __cdecl ntapi_sprintf(
	char * buffer,
	const char * format,
	...);


typedef size_t __cdecl ntapi_strlen(const char * str);


typedef size_t __cdecl ntapi_wcslen(const wchar16_t * str);


typedef void ntapi_rtl_init_unicode_string(
    _out_ 	nt_unicode_string *	str_dest,
    _in_	wchar16_t *		str_src);

typedef void __fastcall ntapi_tt_uint32_to_hex_utf8(
	_in_	uint32_t	key,
	_out_	char *		key_formatted);
#endif /* _NT_STRING_H_ */
