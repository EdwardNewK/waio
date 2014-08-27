/** This file may be processed multiple times;
 * see also the architecture-specific header files.
 *
 * In addition, it is IMPERATIVE that the various
 * checks below are performed every single time
 * that this file is being included.
 *
**/


/* support use of these headers by non-native projects */
#if defined(MIDIPIX_WIN32)
#ifndef __NT32
#define __NT32
#endif
#elif defined(MIDIPIX_WIN64)
#ifndef __NT64
#define __NT64
#endif
#endif


/**
 * ensure a sane build environment:
 * avoid source and/or header mix-ups
**/
#if (defined(__WIN32) || defined(_WIN32) || defined (WIN32)) && !defined(MIDIPIX_WIN32) && !defined(MIDIPIX_WIN64)
	#error __^@^__: Possible mix-up: you have __WIN32 or _WIN32 or WIN32 defined,
	#error __^@^__: but should have defined __NT32 instead.
	#error __^@^__:
	#error __^@^__: if this is a WIN32 project (rather than native or posix), please define MIDIPIX_WIN32
	#error
#elif (defined(__WIN64) || defined(_WIN64) || defined (WIN64))  && !defined(MIDIPIX_WIN64)
	#error __^@^__: Possible mix-up: you have __WIN64 or _WIN64 or WIN64 defined,
	#error __^@^__: but should have defined __NT64 instead.
	#error __^@^__:
	#error __^@^__: if this is a WIN64 project (rather than native or posix), please define MIDIPIX_WIN64
	#error
#endif


/**
 * require either __NT32 or __NT64 or __NATIVE_POSIX to be defined...
**/
#if !defined(__NT32) && !defined(__NT64) && !defined(__NATIVE_POSIX)
	#error __^@^__: PSXTYPES: explicit definition of either __NT32 or __NT64 or __NATIVE_POSIX is required.
#endif


/**
 *  but only one...
**/
#ifdef	__NT32
#define	__NT32_MATCHES 1
#else
#define	__NT32_MATCHES 0
#endif

#ifdef	__NT64
#define	__NT64_MATCHES 1
#else
#define	__NT64_MATCHES 0
#endif

#ifdef	__NATIVE_POSIX
#define	__NATIVE_POSIX_MATCHES 1
#else
#define	__NATIVE_POSIX_MATCHES 0
#endif

#if (__NT32_MATCHES + __NT64_MATCHES + __NATIVE_POSIX_MATCHES) > 1
	#error __^@^__: PSXTYPES: please choose one platform only...
	#error __^@^__: (currently supported platforms: __NT32, __NT64, __NATIVE_POSIX)
#endif

#undef __NT32_MATCHES
#undef __NT64_MATCHES
#undef __NATIVE_POSIX_MATCHES



/**
 *  define arbitrary constants to identify supported compilers;
 *  compiler-specific headers will define __COMPILER__ to equal
 *  one of the values below.
**/
#define __GCC__		0xBE154EBB
#define __MSVC__	0x25F425ED

/*
 *  define a visually non-intrusive macro for debug-only
 *  code sections; when encountering 'if (__PSX_DEBUG == 1)'
 *  in a release configuration, a good optimizing compiler
 *  (and all supported compilers are) will leave out the
 *  the entire code section.
**/
#if defined(__DEBUG)
#define __PSX_DEBUG	1
#else
#define __PSX_DEBUG	0
#endif

#define __debug(x)	if (__PSX_DEBUG == 1) {x;}


/**
 * single point to load compiler-specific and architecture-specific definitions
**/
#if defined(__NATIVE_POSIX)
#include "__psxtypes.h"
#elif defined(__NT32) || defined(__NT64)
#include "__nttypes.h"
#endif
