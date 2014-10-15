#ifndef _PSXTYPES_PSX_COMPILER_GCC_H_
#define _PSXTYPES_PSX_COMPILER_GCC_H_


/**
 *  compiler identification
**/
#define __COMPILER__			__GCC__
#define __COMPILER_DEPENDENT_SUFFIX__	__gcc


/**
 *  x86 processor model
**/
#if defined(__i986__)
	#define __X86_MODEL 	986
#elif defined(__i886__)
	#define __X86_MODEL 	886
#elif defined(__i786__)
	#define __X86_MODEL 	786
#elif defined(__i686__)
	#define __X86_MODEL 	686
#elif defined(__i586__)
	#define __X86_MODEL 	586
#elif defined(__i486__)
	#define __X86_MODEL 	486
#elif defined(__i386__)
	#define __X86_MODEL 	386
#endif


/**
 *  x86_64 processor model
**/
#if defined(__amd64) && (__amd64)
	#define __X86_64_MODEL 	__amd64
#endif


/**
 *  va_list
**/
#define __CUSTOM_OR_BUILTIN_va_list__  __builtin_va_list


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


/**
 *  compiler attributes
**/
#define __attr_aligned__(x)		__attribute__((__aligned__(x)))
#define __attr_ptr_size_aligned__	__attr_aligned__(__SIZEOF_POINTER__)
#define __attr_packed__			__attribute__((__packed__))
#define __attr_export__			__attribute__((dllexport))
#define __attr_import__			__attribute__((dllimport))
#define __attr_naked__
#define __attr_hidden__			__attribute__ ((visibility ("hidden")))
#define __attr_public__			__attribute__ ((visibility ("default")))
#define __attr_protected__		__attribute__ ((visibility ("protected")))

/* compiler-dependent assertions */
#define __assert_aligned_size(s,a)	typedef char __assert##s [-(sizeof(s) % a)]
#define __assert_struct_size(s1,s2)     typedef char __assert##s1##s2 [sizeof(s2) -sizeof(s1)]


/**
 *  calling conventions
**/
#ifndef __cdecl
#define __cdecl
#endif

#ifndef __fastcall
#define __fastcall
#endif

#ifndef __stdcall
#define __stdcall
#endif

#undef APIENTRY
#undef CALLBACK
#undef NTAPI
#undef WINAPI

#define APIENTRY
#define CALLBACK
#define NTAPI
#define WINAPI

#endif /* _PSXTYPES_PSX_COMPILER_GCC_H_ */
