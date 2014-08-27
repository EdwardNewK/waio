#ifndef _NT_MEMORY_H_
#define _NT_MEMORY_H_

#include <psxtypes/psxtypes.h>
#include "nt_object.h"

typedef enum _nt_memory_info_class {
	NT_MEMORY_BASIC_INFORMATION,
	NT_MEMORY_WORKING_SET_LIST,
	NT_MEMORY_SECTION_NAME,
	NT_MEMORY_BASIC_VLM_INFORMATION
} nt_memory_info_class;

/* memory (de)allocation types */
#define NT_MEM_PAGE_GUARD       	0x00000100 /* protect */
#define NT_MEM_COMMIT			0x00001000 /* commit */
#define NT_MEM_RESERVE			0x00002000 /* reserve only */
#define NT_MEM_DECOMMIT			0x00004000 /* decommit but maintain reservavion */
#define NT_MEM_RELEASE			0x00008000 /* decommit and cancel reservation */
#define NT_MEM_RESET			0x00080000 /* make obsolete */
#define NT_MEM_TOP_DOWN			0x00100000 /* allocate at highest possible address using a slow and possibly buggy algorithm */
#define NT_MEM_WRITE_WATCH		0x00200000 /* track writes */
#define NT_MEM_PHYSICAL			0x00400000 /* physical view */
#define NT_MEM_RESET_UNDO AVOID		0x01000000 /* only after a successful NT_MEM_RESET */
#define NT_MEM_LARGE_PAGES		0x20000000 /* use large-page support */
#define NT_MEM_FREE			0x00010000 /* informational only: nt_memory_basic_information.state */
#define NT_MEM_IMAGE			0x01000000 /* informational only: nt_memory_basic_information.type */
#define NT_MEM_MAPPED			0x00040000 /* informational only: nt_memory_basic_information.type */
#define NT_MEM_PRIVATE			0x00020000 /* informational only: nt_memory_basic_information.type */


/* memory page access bits */
#define NT_PAGE_NOACCESS		(uint32_t)0x01
#define NT_PAGE_READONLY		(uint32_t)0x02
#define NT_PAGE_READWRITE		(uint32_t)0x04
#define NT_PAGE_WRITECOPY		(uint32_t)0x08
#define NT_PAGE_EXECUTE			(uint32_t)0x10
#define NT_PAGE_EXECUTE_READ		(uint32_t)0x20
#define NT_PAGE_EXECUTE_READWRITE	(uint32_t)0x40
#define NT_PAGE_EXECUTE_WRITECOPY	(uint32_t)0x80


typedef struct _nt_memory_basic_information {
	void *		base_address;
	void *		allocation_base;
	uint32_t	allocation_protect;
	size_t		region_size;
	uint32_t	state;
	uint32_t	protect;
	uint32_t	type;
} nt_memory_basic_information;


typedef int32_t __stdcall ntapi_zw_allocate_virtual_memory(
	_in_		void *		hprocess,
	_in_out_	void **		base_address,
	_in_		uint32_t	zero_bits,
	_in_out_	size_t *	allocation_size,
	_in_		uint32_t	allocation_type,
	_in_		uint32_t	protect);


typedef int32_t __stdcall ntapi_zw_free_virtual_memory(
	_in_		void *		hprocess,
	_in_out_	void **		base_address,
	_in_out_	size_t *	free_size,
	_in_		uint32_t	deallocation_type);


typedef int32_t __stdcall ntapi_zw_query_virtual_memory(
	_in_	void *			hprocess,
	_in_	void *			base_address,
	_in_	nt_memory_info_class	mem_info_class,
	_out_	void *			mem_info,
	_in_	size_t			mem_info_length,
	_out_	size_t *		returned_length			_optional_);

#endif /* _NT_MEMORY_H_ */
