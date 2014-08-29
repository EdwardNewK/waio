#ifndef _NT_THREAD_I386_H_
#define _NT_THREAD_I386_H_

#include <psxtypes/psxtypes.h>

/* register names */
#if defined(__X86_MODEL)
#define STACK_POINTER_REGISTER 		        Esp
#define INSTRUCTION_POINTER_REGISTER	        Eip
#define FAST_CALL_ARG0				Ecx
#define FAST_CALL_ARG1				Edx
#endif

typedef struct __attr_packed__ _nt_floating_save_area_i386 {
	uint32_t	ControlWord;		/* 0x000 */
	uint32_t	StatusWord;		/* 0x004 */
	uint32_t	TagWord;		/* 0x008 */
	uint32_t	ErrorOffset;		/* 0x00c */
	uint32_t	ErrorSelector;		/* 0x010 */
	uint32_t	DataOffset;		/* 0x014 */
	uint32_t	DataSelector;		/* 0x018 */
	unsigned char 	RegisterArea[80];	/* 0x01c */
	uint32_t	Cr0NpxState;		/* 0x06c */
} nt_floating_save_area_i386;


typedef struct __attr_packed__ _nt_thread_context_i386 {
	uint32_t	ContextFlags;		/* 0x000 */
	uint32_t	Dr0;			/* 0x004 */
	uint32_t	Dr1;			/* 0x008 */
	uint32_t	Dr2;			/* 0x00c */
	uint32_t	Dr3;			/* 0x010 */
	uint32_t	Dr6;			/* 0x014 */
	uint32_t	Dr7;			/* 0x018 */

	nt_floating_save_area_i386
			FloatSave;		/* 0x01c */

	uint32_t	SegGs; 			/* 0x08c */
	uint32_t	SegFs; 			/* 0x090 */
	uint32_t	SegEs; 			/* 0x094 */
	uint32_t	SegDs; 			/* 0x098 */
	uint32_t	Edi;			/* 0x09c */
	uint32_t	Esi;			/* 0x0a0 */
	uint32_t	Ebx;			/* 0x0a4 */
	uint32_t	Edx;			/* 0x0a8 */
	uint32_t	Ecx;			/* 0x0ac */
	uint32_t	Eax;			/* 0x0b0 */
	uint32_t	Ebp;			/* 0x0b4 */
	uint32_t	Eip;			/* 0x0b8 */
	uint32_t	SegCs; 			/* 0x0bc */
	uint32_t	EFlags;			/* 0x0c0 */
	uint32_t	Esp;			/* 0x0c4 */
	uint32_t	SegSs;			/* 0x0c8 */
	unsigned char	ExtendedRegisters[512]; /* 0x0cc */
} nt_thread_context_i386;


#endif /* _NT_THREAD_I386_H_ */