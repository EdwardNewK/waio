#ifndef _NT_OBJECT_H_
#define _NT_OBJECT_H_

#include <psxtypes/psxtypes.h>

typedef enum _nt_object_info_class {
	NT_OBJECT_BASIC_INFORMATION	= 0,
	NT_OBJECT_NAME_INFORMATION	= 1,
	NT_OBJECT_TYPE_INFORMATION	= 2,
	NT_OBJECT_ALL_TYPES_INFORMATION	= 3,
	NT_OBJECT_HANDLE_INFORMATION	= 4
} nt_object_info_class;


typedef enum _nt_security_impersonation_level {
	NT_SECURITY_ANONYMOUS		= 0,
	NT_SECURITY_IDENTIFICATION	= 1,
	NT_SECURITY_IMPERSONATION	= 2,
	NT_SECURITY_DELEGATION		= 3
} nt_security_impersonation_level;


typedef enum _nt_security_information {
	NT_OWNER_SECURITY_INFORMATION	= 0x01,
	NT_GROUP_SECURITY_INFORMATION	= 0x02,
	NT_DACL_SECURITY_INFORMATION	= 0x04,
	NT_SACL_SECURITY_INFORMATION	= 0x08
} nt_security_information;



/* generic access rights */
#define NT_SEC_DELETE				(0x00010000u)
#define NT_SEC_READ_CONTROL			(0x00020000u)
#define NT_SEC_WRITE_DAC			(0x00040000u)
#define NT_SEC_WRITE_OWNER			(0x00080000u)
#define NT_SEC_SYNCHRONIZE			(0x00100000u)
#define NT_SEC_STANDARD_RIGHTS_REQUIRED		(0x000F0000u)
#define NT_SEC_STANDARD_RIGHTS_READ		NT_SEC_READ_CONTROL
#define NT_SEC_STANDARD_RIGHTS_WRITE		NT_SEC_READ_CONTROL
#define NT_SEC_STANDARD_RIGHTS_EXECUTE		NT_SEC_READ_CONTROL
#define NT_SEC_STANDARD_RIGHTS_ALL		(0x001F0000u)
#define NT_SEC_SPECIFIC_RIGHTS_ALL		(0x0000FFFFu)

#define NT_GENERIC_ALL				(0x10000000u)
#define NT_GENERIC_EXECUTE			(0x20000000u)
#define NT_GENERIC_WRITE			(0x40000000u)
#define NT_GENERIC_READ				(0x80000000u)


/* zw_open_directory access rights */
#define NT_DIRECTORY_QUERY			(0x0001u)
#define NT_DIRECTORY_TRAVERSE			(0x0002u)
#define NT_DIRECTORY_CREATE_OBJECT		(0x0004u)
#define NT_DIRECTORY_CREATE_SUBDIRECTORY	(0x0008u)
#define NT_DIRECTORY_ALL_ACCESS			NT_DIRECTORY_QUERY \
							| NT_DIRECTORY_TRAVERSE \
							| NT_DIRECTORY_CREATE_OBJECT \
							| NT_DIRECTORY_CREATE_SUBDIRECTORY \
							| NT_SEC_STANDARD_RIGHTS_REQUIRED

/* zw_open_symbolic_link_object access rights */
#define NT_SYMBOLIC_LINK_QUERY			(0x0001u)
#define NT_SYMBOLIC_LINK_ALL_ACCESS		(NT_SYMBOLIC_LINK_QUERY \
							| NT_SEC_STANDARD_RIGHTS_REQUIRED

/* object handles */
#define NT_HANDLE_FLAG_INHERIT			(0x0001u)
#define NT_HANDLE_FLAG_PROTECT_FROM_CLOSE	(0x0002u)
#define NT_HANDLE_PERMANENT			(0x0010u)
#define NT_HANDLE_EXCLUSIVE			(0x0020u)
#define NT_INVALID_HANDLE_VALUE 		((void *)(intptr_t)-1)

/* object attribute bits */
#define NT_OBJ_INHERIT	 			(0x0002u)
#define NT_OBJ_PERMANENT 			(0x0010u)
#define NT_OBJ_EXCLUSIVE 			(0x0020u)
#define NT_OBJ_CASE_INSENSITIVE			(0x0040u)
#define NT_OBJ_OPENIF	 			(0x0080u)
#define NT_OBJ_OPENLINK	 			(0x0100u)
#define NT_OBJ_KERNEL_HANDLE 			(0x0200u)

/* duplicate object bits */
#define NT_DUPLICATE_CLOSE_SOURCE		(0x0001u)
#define NT_DUPLICATE_SAME_ACCESS		(0x0002u)
#define NT_DUPLICATE_SAME_ATTRIBUTES		(0x0004u)

/* nt_security_descriptor constants (IFS open specification) */
#define NT_SE_OWNER_DEFAULTED		(int16_t)0x0001
#define NT_SE_GROUP_DEFAULTED		(int16_t)0x0002
#define NT_SE_DACL_PRESENT		(int16_t)0x0004
#define NT_SE_DACL_DEFAULTED		(int16_t)0x0008
#define NT_SE_SACL_PRESENT		(int16_t)0x0010
#define NT_SE_SACL_DEFAULTED		(int16_t)0x0020
#define NT_SE_DACL_AUTO_INHERIT_REQ	(int16_t)0x0100
#define NT_SE_SACL_AUTO_INHERIT_REQ	(int16_t)0x0200
#define NT_SE_DACL_AUTO_INHERITED	(int16_t)0x0400
#define NT_SE_SACL_AUTO_INHERITED	(int16_t)0x0800
#define NT_SE_DACL_PROTECTED		(int16_t)0x1000
#define NT_SE_SACL_PROTECTED		(int16_t)0x2000
#define NT_SE_RM_CONTROL_VALID		(int16_t)0x4000
#define NT_SE_SELF_RELATIVE		(int16_t)0x8000

/* security tracking */
#define NT_SECURITY_TRACKING_STATIC	0
#define NT_SECURITY_TRACKING_DYNAMIC	1

/* predefined security authorities */
#define NT_SECURITY_NULL_SID_AUTHORITY		0
#define NT_SECURITY_WORLD_SID_AUTHORITY		1
#define NT_SECURITY_LOCAL_SID_AUTHORITY		2
#define NT_SECURITY_CREATOR_SID_AUTHORITY	3
#define NT_SECURITY_NON_UNIQUE_AUTHORITY	4
#define NT_SECURITY_NT_AUTHORITY		5

/* token source length */
#define NT_TOKEN_SOURCE_LENGTH			8


typedef struct _nt_unicode_string {
	uint16_t	strlen;
	uint16_t	maxlen;
	uint16_t *	buffer;
} nt_unicode_string;


typedef union _nt_large_integer {
	struct {
		uint32_t	ulow;
		int32_t		ihigh;
	} __u;
	long long		quad;
} nt_large_integer, nt_timeout;


typedef struct _nt_io_status_block {
	union {
		int32_t		status;
		void *		pointer;
	};
	intptr_t		info;
} nt_io_status_block, nt_iosb;


typedef struct _nt_client_id {
	uintptr_t	process_id;
	uintptr_t	thread_id;
} nt_client_id, nt_cid;


typedef struct _nt_generic_mapping {
	uint32_t	generic_read;
	uint32_t	generic_write;
	uint32_t	generic_execute;
	uint32_t	generic_all;
} nt_generic_mapping, nt_gmap;


typedef struct _nt_security_attributes {
	uint32_t	length;
	void *		security_descriptor;
	int32_t		inherit_handle;
} nt_security_attributes, nt_sa;


typedef struct _nt_guid {
	uint32_t	data1;
	uint16_t	data2;
	uint16_t	data3;
	unsigned char	data4[8];
} nt_guid, nt_uuid;


typedef struct _nt_uuid_vector {
	uint32_t	count;
	nt_uuid *	uuid[];
} nt_uuid_vector;


typedef struct _nt_acl {
	unsigned char	acl_revision;
	unsigned char	sbz_1st;
	uint16_t	acl_size;
	uint16_t	ace_count;
	uint16_t	sbz_2nd;
} nt_acl;


typedef struct _nt_security_descriptor {
	unsigned char	revision;
	unsigned char	sbz_1st;
	uint16_t	control;
	uint32_t	offset_owner;
	uint32_t	offset_group;
	uint32_t	offset_sacl;
	uint32_t	offset_dacl;
} nt_security_descriptor, nt_sd;


typedef struct _nt_security_quality_of_service {
  uint32_t				length;
  nt_security_impersonation_level	impersonation_level;
  int32_t 				context_tracking_mode;
  int32_t				effective_only;
} nt_security_quality_of_service, nt_sqos;


typedef struct _nt_object_attributes {
	uint32_t		len;
	void *			root_dir;
	nt_unicode_string *	obj_name;
	uint32_t		obj_attr;
	nt_security_descriptor *sec_desc;
	nt_sqos *		sec_qos;
} nt_object_attributes, nt_oa;

typedef int32_t __stdcall ntapi_zw_close(
	_in_	void *	handle);

#endif /* _NT_OBJECT_H_ */
