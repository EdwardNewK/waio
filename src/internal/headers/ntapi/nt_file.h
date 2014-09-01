#ifndef _NT_FILE_H_
#define _NT_FILE_H_

#include <psxtypes/psxtypes.h>
#include "nt_object.h"

typedef enum _nt_file_info_class {
	NT_FILE_DIRECTORY_INFORMATION			= 1,
	NT_FILE_FULL_DIRECTORY_INFORMATION		= 2,
	NT_FILE_BOTH_DIRECTORY_INFORMATION		= 3,
	NT_FILE_BASIC_INFORMATION			= 4,
	NT_FILE_STANDARD_INFORMATION			= 5,
	NT_FILE_INTERNAL_INFORMATION			= 6,
	NT_FILE_EA_INFORMATION				= 7,
	NT_FILE_ACCESS_INFORMATION			= 8,
	NT_FILE_NAME_INFORMATION			= 9,
	NT_FILE_RENAME_INFORMATION			= 10,
	NT_FILE_LINK_INFORMATION			= 11,
	NT_FILE_NAMES_INFORMATION			= 12,
	NT_FILE_DISPOSITION_INFORMATION			= 13,
	NT_FILE_POSITION_INFORMATION			= 14,
	NT_FILE_FULL_EA_INFORMATION			= 15,
	NT_FILE_MODE_INFORMATION			= 16,
	NT_FILE_ALIGNMENT_INFORMATION			= 17,
	NT_FILE_ALL_INFORMATION				= 18,
	NT_FILE_ALLOCATION_INFORMATION			= 19,
	NT_FILE_END_OF_FILE_INFORMATION			= 20,
	NT_FILE_ALTERNATE_NAME_INFORMATION		= 21,
	NT_FILE_STREAM_INFORMATION			= 22,
	NT_FILE_PIPE_INFORMATION			= 23,
	NT_FILE_PIPE_LOCAL_INFORMATION			= 24,
	NT_FILE_PIPE_REMOTE_INFORMATION			= 25,
	NT_FILE_MAILSLOT_QUERY_INFORMATION		= 26,
	NT_FILE_MAILSLOT_SET_INFORMATION		= 27,
	NT_FILE_COMPRESSION_INFORMATION			= 28,
	NT_FILE_OBJECT_ID_INFORMATION			= 29,
	NT_FILE_COMPLETION_INFORMATION			= 30,
	NT_FILE_MOVE_CLUSTER_INFORMATION		= 31,
	NT_FILE_QUOTA_INFORMATION			= 32,
	NT_FILE_REPARSE_POINT_INFORMATION		= 33,
	NT_FILE_NETWORK_OPEN_INFORMATION		= 34,
	NT_FILE_ATTRIBUTE_TAG_INFORMATION		= 35,
	NT_FILE_TRACKING_INFORMATION			= 36,
	NT_FILE_ID_BOTH_DIRECTORY_INFORMATION		= 37,
	NT_FILE_ID_FULL_DIRECTORY_INFORMATION		= 38,
	NT_FILE_VALID_DATA_LENGTH_INFORMATION		= 39,
	NT_FILE_SHORT_NAME_INFORMATION			= 40,
	NT_FILE_IO_COMPLETION_NOTIFICATION_INFORMATION	= 41,
	NT_FILE_IO_STATUS_BLOCK_RANGE_INFORMATION	= 42,
	NT_FILE_IO_PRIORITY_HINT_INFORMATION		= 43,
	NT_FILE_SFIO_RESERVE_INFORMATION		= 44,
	NT_FILE_SFIO_VOLUME_INFORMATION			= 45,
	NT_FILE_HARD_LINK_INFORMATION			= 46,
	NT_FILE_PROCESS_IDS_USING_FILE_INFORMATION	= 47,
	NT_FILE_NORMALIZED_NAME_INFORMATION		= 48,
	NT_FILE_NETWORK_PHYSICAL_NAME_INFORMATION	= 49,
	NT_FILE_ID_GLOBAL_TX_DIRECTORY_INFORMATION	= 50,
	NT_FILE_IS_REMOTE_DEVICE_INFORMATION		= 51,
	NT_FILE_ATTRIBUTE_CACHE_INFORMATION		= 52,
	NT_FILE_NUMA_NODE_INFORMATION			= 53,
	NT_FILE_STANDARD_LINK_INFORMATION		= 54,
	NT_FILE_REMOTE_PROTOCOL_INFORMATION		= 55,
	NT_FILE_REPLACE_COMPLETION_INFORMATION		= 56,
	NT_FILE_MAXIMUM_INFORMATION			= 57
} nt_file_info_class;


typedef enum _nt_file_pipe_flags {
	NT_FILE_PIPE_BYTE_STREAM_MODE	= 0x00000000,
	NT_FILE_PIPE_MESSAGE_MODE	= 0x00000001,

	NT_FILE_PIPE_QUEUE_OPERATION	= 0x00000000,
	NT_FILE_PIPE_COMPLETE_OPERATION	= 0x00000001,

	NT_FILE_PIPE_BYTE_STREAM_TYPE	= 0x00000000,
	NT_FILE_PIPE_MESSAGE_TYPE	= 0x00000001,

	NT_FILE_PIPE_INBOUND		= 0x00000000,
	NT_FILE_PIPE_OUTBOUND		= 0x00000001,
	NT_FILE_PIPE_FULL_DUPLEX	= 0x00000002,

	NT_FILE_PIPE_DISCONNECTED_STATE	= 0x00000001,
	NT_FILE_PIPE_LISTENING_STATE	= 0x00000002,
	NT_FILE_PIPE_CONNECTED_STATE	= 0x00000003,
	NT_FILE_PIPE_CLOSING_STATE	= 0x00000004,

	NT_FILE_PIPE_CLIENT_END		= 0x00000000,
	NT_FILE_PIPE_SERVER_END		= 0x00000001,
} nt_file_pipe_flags;


/* file access bits */
#define	NT_FILE_ANY_ACCESS		(0x0000u)
#define	NT_FILE_READ_ACCESS		(0x0001u)
#define	NT_FILE_READ_DATA		(0x0001u)
#define	NT_FILE_LIST_DIRECTORY		(0x0001u)
#define	NT_FILE_WRITE_ACCESS		(0x0002u)
#define	NT_FILE_WRITE_DATA		(0x0002u)
#define	NT_FILE_ADD_FILE		(0x0002u)
#define	NT_FILE_APPEND_DATA		(0x0004u)
#define	NT_FILE_ADD_SUBDIRECTORY	(0x0004u)
#define	NT_FILE_CREATE_PIPE_INSTANCE	(0x0004u)
#define	NT_FILE_READ_EA			(0x0008u)
#define	NT_FILE_WRITE_EA		(0x0010u)
#define	NT_FILE_EXECUTE			(0x0020u)
#define	NT_FILE_TRAVERSE		(0x0020u)
#define	NT_FILE_DELETE_CHILD		(0x0040u)
#define	NT_FILE_READ_ATTRIBUTES		(0x0080u)
#define	NT_FILE_WRITE_ATTRIBUTES	(0x0100u)

#define NT_FILE_ALL_ACCESS		NT_FILE_ANY_ACCESS \
					| NT_FILE_READ_ACCESS \
					| NT_FILE_WRITE_ACCESS \
					| NT_FILE_APPEND_DATA \
					| NT_FILE_READ_EA \
					| NT_FILE_WRITE_EA \
					| NT_FILE_EXECUTE \
					| NT_FILE_TRAVERSE \
					| NT_FILE_DELETE_CHILD \
					| NT_FILE_READ_ATTRIBUTES \
					| NT_FILE_WRITE_ATTRIBUTES \
					| NT_SEC_SYNCHRONIZE \
					| NT_SEC_STANDARD_RIGHTS_ALL


/* structures included in nt_file_all_information */
typedef struct _nt_file_basic_information {
	nt_large_integer	creation_time;
	nt_large_integer	last_access_time;
	nt_large_integer	last_write_time;
	nt_large_integer	change_time;
	uint32_t		file_attr;
} nt_file_basic_information, nt_fbi;


typedef struct _nt_file_standard_information {
	nt_large_integer	allocation_size;
	nt_large_integer	end_of_file;
	uint32_t		number_of_links;
	unsigned char		delete_pending;
	unsigned char		directory;
	unsigned char		reserved[2];
} nt_file_standard_information, nt_fsi;


/* structures related to nt_fs_info_class */
typedef struct _nt_file_pipe_information {
	uint32_t	read_mode;
	uint32_t	completion_mode;
} nt_file_pipe_information, nt_fpi;


typedef struct _nt_file_pipe_local_information {
	uint32_t	named_pipe_type;
	uint32_t	named_pipe_configuration;
	uint32_t	maximum_instances;
	uint32_t	current_instances;
	uint32_t	inbound_quota;
	uint32_t	read_data_available;
	uint32_t	outbound_quota;
	uint32_t	write_quota_available;
	uint32_t	named_pipe_state;
	uint32_t	named_pipe_end;
} nt_file_pipe_local_information;


typedef struct _nt_file_pipe_remote_information {
	nt_large_integer	collect_data_time;
	uint32_t		maximum_collection_count;
} nt_file_pipe_remote_information;

/* file-related function signatures */
typedef int32_t	__stdcall ntapi_zw_read_file(
	_in_	void *			hfile,
	_in_	void *			hevent		_optional_,
	_in_	void *			apc_routine	_optional_,
	_in_	void *			apc_context	_optional_,
	_out_	nt_io_status_block *	io_status_block,
	_out_	void *			buffer,
	_in_	uint32_t		bytes_to_read,
	_in_	nt_large_integer *	byte_offset	_optional_,
	_in_	uint32_t *		key		_optional_);


typedef int32_t __stdcall ntapi_zw_write_file(
	_in_	void *			hfile,
	_in_ 	void *			hevent		_optional_,
	_in_	void *			apc_routine	_optional_,
	_in_	void * 			apc_context	_optional_,
	_out_	nt_io_status_block * 	io_status_block,
	_in_	void * 			buffer,
	_in_	uint32_t		bytes_sent,
	_in_	nt_large_integer *	byte_offset	_optional_,
	_in_	uint32_t *		key		_optional_);

typedef int32_t	__stdcall ntapi_zw_query_information_file(
	_in_	void *			hfile,
	_out_	nt_io_status_block *	io_status_block,
	_out_	void *			file_info,
	_in_	uint32_t		file_info_length,
	_in_	nt_file_info_class	file_info_class);


typedef int32_t	__stdcall ntapi_zw_set_information_file(
	_in_	void *			hfile,
	_out_	nt_io_status_block *	io_status_block,
	_in_	void *			file_info,
	_in_	uint32_t		file_info_length,
	_in_	nt_file_info_class	file_info_class);

#endif /* _NT_FILE_H_ */
