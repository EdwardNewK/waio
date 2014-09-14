#ifndef _WAIO__FCNTL_H_
#define _WAIO__FCNTL_H_

typedef enum waio_fcntl_opcode_enum {
	WAIO_FCNTL_OPCODE_BASE		= 0x240000,
	WAIO_FCNTL_QUERY_SET_HOOK	= WAIO_FCNTL_OPCODE_BASE,
	WAIO_FCNTL_OPCODE_CAP
} waio_fcntl_opcode;


#endif /* _WAIO__FCNTL_H_ */
