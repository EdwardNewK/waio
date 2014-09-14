#ifndef _WAIO__COMPILER_H_
#define _WAIO__COMPILER_H_

/* gcc */
#ifdef	__GNUC__
#define __attr_export__				__attribute__((dllexport))
#define __attr_import__				__attribute__((dllimport))

#if defined(__amd64)

#define	__waio_call_conv__thread_entry_point
#define	__waio_call_conv__hook
#define	__waio_call_conv__api

#else

#define	__waio_call_conv__thread_entry_point	__attribute__((__stdcall__))
#define	__waio_call_conv__hook			__attribute__((__stdcall__))
#define	__waio_call_conv__api			__attribute__((__stdcall__))

#endif

typedef	__INTPTR_TYPE__				__waio_intptr;
typedef	__UINTPTR_TYPE__			__waio_uintptr;

/* msvc */
#elif defined (_MSC_VER)
#define	__waio_call_conv__thread_entry_point	_stdcall
#define	__waio_call_conv__hook			_stdcall
#define	__waio_call_conv__api			_stdcall
#define __attr_export__				__declspec(dllexport)
#define __attr_import__				__declspec(dllimport)

#if defined(_M_AMD64)
typedef	long long int				__waio_intptr;
typedef	unsigned long long int			__waio_uintptr;
#elif defined(_M_IX86)
typedef	signed long int				__waio_intptr;
typedef	unsigned long int			__waio_uintptr;
#endif

/* todo: clang/llvm */
#endif

#endif /* _WAIO__COMPILER_H_ */
