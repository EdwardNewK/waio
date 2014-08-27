/**
 * ensure that each of __NT32 and __NT64 is being used with a matching compiler
**/
#if defined(__NT32) && \
	(defined (_X86_64_) \
	|| defined (__MINGW64__) \
	|| defined (__amd64) \
	|| defined (_M_IA64) \
	|| defined (_M_X64))
    #error __^@^__: PSXTYPES: you have __NT32 defined, but are using a 64-bit compiler

#elif defined(__NT64) && \
	(defined (_X86_) \
	|| defined (_M_IX86))
    #error __^@^__: PSXTYPES: you have __NT64 defined, but are using a 32-bit compiler
#endif


/**
 * compiler directives: unified syntax
**/
#if defined(__GNUC__)
	#include "compiler/__nt_compiler_gcc.h"
#elif defined (_MSC_VER)
	#include "compiler/__nt_compiler_msvc.h"
#endif


#ifndef __ASSEMBLER__
/**
 * on NT, we also need to define all NEEDed types
**/
#if defined(__NT32)
#include "arch/nt32/ntalltypes.h"
#elif defined(__NT64)
#include "arch/nt64/ntalltypes.h"
#endif

#endif /* __ASSEMBLER__ */
