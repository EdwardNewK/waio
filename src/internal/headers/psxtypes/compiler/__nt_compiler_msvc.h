#ifndef _PSXTYPES_NT_COMPILER_MSVC_H_
#define _PSXTYPES_NT_COMPILER_MSVC_H_


/**
 *  compiler identification
**/
#define __COMPILER__			__MSVC__
#define __COMPILER_DEPENDENT_SUFFIX__	__msvc


/**
 * NTVER: the bare minimum...
**/
#ifndef NTVER
	#define NTVER 0x0500
#elif (NTVER < 0x0500)
	#error __^@^__: NTVER: minimum supported NT version is 0x0500.
#endif


/**
 * require UNICODE to be defined
**/
#ifndef UNICODE
	#error __^@^__: UNICODE must be defined.
	#error __^@^__: Hint: gcc -DUNICODE
#endif


/**
 *  x86 processor model
**/
#if (_M_IX86 == 900)
	#define __X86_MODEL 	986
#elif (_M_IX86 == 800)
	#define __X86_MODEL 	886
#elif (_M_IX86 == 700)
	#define __X86_MODEL 	786
#elif (_M_IX86 == 600)
	#define __X86_MODEL 	686
#elif (_M_IX86 == 500)
	#define __X86_MODEL 	586
#elif (_M_IX86 == 400)
	#define __X86_MODEL 	486
#elif (_M_IX86 == 300)
	#define __X86_MODEL 	386
#endif


/**
 *  x86_64 processor model
**/
#if defined(_M_AMD64)
	#define __amd64 1
	#define __X86_64_MODEL 	__amd64
#endif


/**
 *  va_list
**/
#define __CUSTOM_OR_BUILTIN_va_list__  char *


/**
 *  compiler keywords
**/
#define near
#define far

#define _in_
#define _out_
#define _in_opt_
#define _out_opt_
#define _in_out_
#define _in_out_opt_
#define _reserved_
#define _optional_
#define _simplified_
#define _caller_allocated_
#define _caller_initialized_
#define _arch_specific_
#define _arch_of_image_
#define _null_placeholder_		(0x0)

/* __SIZEOF_POINTER__ */
#if defined(__NT32)
	#define __SIZEOF_POINTER__	4
#elif defined(__NT64)
	#define __SIZEOF_POINTER__	8
#endif


/**
 *  compiler attributes
 *
 *  packing: avoid the #pragma;
 *  use align instead, possibly with char[x] members and padding.
**/
#define __inline__			__inline
#define __volatile__			volatile
#define __attr_aligned__(x)		__declspec(align(x))
#define __attr_ptr_size_aligned__	__attr_aligned__(__SIZEOF_POINTER__)
#define __attr_packed__
#define __attr_export__			__declspec(dllexport)
#define __attr_import__			__declspec(dllimport)
#define __attr_naked__			__declspec(naked)
#define __attr_hidden__
#define __attr_public__
#define __attr_protected__


/* compiler-dependent assertions */
#define __assert_aligned_size(s,a)	typedef char __assert##s [1-((sizeof(s) % a))]
#define __assert_struct_size(s1,s2)     typedef char __assert##s1##s2 [1 + sizeof(s2) -sizeof(s1)]


/**
 *  calling conventions
**/
#undef __stdcall
#undef APIENTRY
#undef CALLBACK
#undef NTAPI
#undef WINAPI

#define __stdcall 	_stdcall
#define APIENTRY	__stdcall
#define CALLBACK	__stdcall
#define NTAPI		__stdcall
#define WINAPI		__stdcall

#endif /* _PSXTYPES_NT_COMPILER_MSVC_H_ */
