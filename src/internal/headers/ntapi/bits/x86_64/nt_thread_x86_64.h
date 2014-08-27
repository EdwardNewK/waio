#ifndef _NT_THREAD_X86_64_H_
#define _NT_THREAD_X86_64_H_

#include <psxtypes/psxtypes.h>

/* register names */
#if defined (__X86_64_MODEL)
#define STACK_POINTER_REGISTER 		        Rsp
#define INSTRUCTION_POINTER_REGISTER	        Rip
#define FAST_CALL_ARG0				Rcx
#define FAST_CALL_ARG1				Rdx
#endif

typedef struct __attr_packed__ __nt_m128a {
	uint64_t	Low;	/* 0x000 */
	int64_t		High;	/* 0x008 */
} _nt_m128a;


typedef struct __attr_packed__ _nt_xsave_format {
	uint16_t	ControlWord;		/* 0x000 */
	uint16_t	StatusWord;		/* 0x002 */
	unsigned char	TagWord;		/* 0x004 */
	unsigned char	Reserved1;		/* 0x005 */
	uint16_t	ErrorOpcode;		/* 0x006 */
	uint32_t	ErrorOffset;		/* 0x008 */
	uint16_t	ErrorSelector;		/* 0x00c */
	uint16_t	Reserved2;		/* 0x00e */
	uint32_t	DataOffset;		/* 0x010 */
	uint16_t	DataSelector;		/* 0x014 */
	uint16_t	Reserved3;		/* 0x016 */
	uint32_t	MxCsr;			/* 0x018 */
	uint32_t	MxCsr_Mask;		/* 0x01c */
	_nt_m128a	FloatRegisters[8];	/* 0x020 */
	_nt_m128a	XmmRegisters[16];	/* 0x0a0 */
	unsigned char	Reserved4[96];		/* 0x1a0 */
} nt_xsave_format;


typedef struct __attr_packed__ _nt_thread_context_x86_64 {
	uint64_t	P1Home;			/* 0x000 */
	uint64_t	P2Home;			/* 0x008 */
	uint64_t	P3Home;			/* 0x010 */
	uint64_t	P4Home;			/* 0x018 */
	uint64_t	P5Home;			/* 0x020 */
	uint64_t	P6Home;			/* 0x028 */
	uint32_t	ContextFlags;		/* 0x030 */
	uint32_t	MxCsr;			/* 0x034 */
	uint16_t	SegCs;			/* 0x038 */
	uint16_t	SegDs;			/* 0x03a */
	uint16_t	SegEs;			/* 0x03c */
	uint16_t	SegFs;			/* 0x03e */
	uint16_t	SegGs;			/* 0x040 */
	uint16_t	SegSs;			/* 0x042 */
	uint32_t	EFlags;			/* 0x044 */
	uint64_t	Dr0;			/* 0x048 */
	uint64_t	Dr1;			/* 0x050 */
	uint64_t	Dr2;			/* 0x058 */
	uint64_t	Dr3;			/* 0x060 */
	uint64_t	Dr6;			/* 0x068 */
	uint64_t	Dr7;			/* 0x070 */
	uint64_t	Rax;			/* 0x078 */
	uint64_t	Rcx;			/* 0x080 */
	uint64_t	Rdx;			/* 0x088 */
	uint64_t	Rbx;			/* 0x090 */
	uint64_t	Rsp;			/* 0x098 */
	uint64_t	Rbp;			/* 0x0a0 */
	uint64_t	Rsi;			/* 0x0a8 */
	uint64_t	Rdi;			/* 0x0b0 */
	uint64_t	R8;			/* 0x0b8 */
	uint64_t	R9;			/* 0x0c0 */
	uint64_t	R10;			/* 0x0c8 */
	uint64_t	R11;			/* 0x0d0 */
	uint64_t	R12;			/* 0x0d8 */
	uint64_t	R13;			/* 0x0e0 */
	uint64_t	R14;			/* 0x0e8 */
	uint64_t	R15;			/* 0x0f0 */
	uint64_t	Rip;			/* 0x0f8 */

	union {					/* 0x100 */
		nt_xsave_format		FltSave;

		struct {
			_nt_m128a	Header[2];
			_nt_m128a	Legacy[8];
		} __hdr_leagacy;
	} __uflt;

	_nt_m128a	Xmm0;			/* 0x1a0 */
	_nt_m128a	Xmm1;			/* 0x1b0 */
	_nt_m128a	Xmm2;			/* 0x1c0 */
	_nt_m128a	Xmm3;			/* 0x1d0 */
	_nt_m128a	Xmm4;			/* 0x1e0 */
	_nt_m128a	Xmm5;			/* 0x1f0 */
	_nt_m128a	Xmm6;			/* 0x200 */
	_nt_m128a	Xmm7;			/* 0x210 */
	_nt_m128a	Xmm8;			/* 0x220 */
	_nt_m128a	Xmm9;			/* 0x230 */
	_nt_m128a	Xmm10;			/* 0x240 */
	_nt_m128a	Xmm11;			/* 0x250 */
	_nt_m128a	Xmm12;			/* 0x260 */
	_nt_m128a	Xmm13;			/* 0x270 */
	_nt_m128a	Xmm14;			/* 0x280 */
	_nt_m128a	Xmm15;			/* 0x290 */
	_nt_m128a	VectorRegister[26];	/* 0x300 */
	uint64_t	VectorControl;		/* 0x4a0 */
	uint64_t	DebugControl;		/* 0x4a8 */
	uint64_t	LastBranchToRip;	/* 0x4b0 */
	uint64_t	LastBranchFromRip;	/* 0x4b8 */
	uint64_t	LastExceptionToRip;	/* 0x4c0 */
	uint64_t	LastExceptionFromRip;	/* 0x4c8 */
} nt_thread_context_x86_64;

#endif /* _NT_THREAD_X86_64_H_ */
