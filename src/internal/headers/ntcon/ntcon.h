#ifndef _NTCON_H_
#define _NTCON_H_

#include <psxtypes/psxtypes.h>

/* console standard handles */
#define NT_STD_INPUT_HANDLE		(uint32_t)-10
#define NT_STD_OUTPUT_HANDLE		(uint32_t)-11
#define NT_STD_ERROR_HANDLE		(uint32_t)-12

/* console attachment */
#define NT_ATTACH_PARENT_PROCESS	(uint32_t)-1

/* console screen buffer flags */
#define NT_CONSOLE_TEXTMODE_BUFFER	(0x00000001L)

/* console control signals */
#define NT_CTRL_C_EVENT			(0x00)
#define NT_CTRL_BREAK_EVENT		(0x01)
#define NT_CTRL_CLOSE_EVENT		(0x02)
#define NT_CTRL_LOGOFF_EVENT		(0x05)
#define NT_CTRL_SHUTDOWN_EVENT		(0x06)

/* console history flags */
#define NT_HISTORY_NO_DUP_FLAG		(0x0001)

/* console event types */
#define NT_KEY_EVENT			(0x0001)
#define NT_MOUSE_EVENT			(0x0002)
#define NT_WINDOW_BUFFER_SIZE_EVENT	(0x0004)
#define NT_MENU_EVENT			(0x0008)
#define NT_FOCUS_EVENT			(0x0010)

/* control key states */
#define NT_RIGHT_ALT_PRESSED		(0x0001)
#define NT_LEFT_ALT_PRESSED		(0x0002)
#define NT_RIGHT_CTRL_PRESSED		(0x0004)
#define NT_LEFT_CTRL_PRESSED		(0x0008)
#define NT_SHIFT_PRESSED		(0x0010)
#define NT_NUMLOCK_ON			(0x0020)
#define NT_SCROLLLOCK_ON		(0x0040)
#define NT_CAPSLOCK_ON			(0x0080)
#define NT_ENHANCED_KEY			(0x0100)

/* console foreground color attributes */
#define NT_FOREGROUND_BLACK		(0x0000)
#define NT_FOREGROUND_BLUE		(0x0001)
#define NT_FOREGROUND_GREEN		(0x0002)
#define NT_FOREGROUND_CYAN		(0x0003)
#define NT_FOREGROUND_RED		(0x0004)
#define NT_FOREGROUND_MAGENTA		(0x0005)
#define NT_FOREGROUND_YELLOW		(0x0006)
#define NT_FOREGROUND_WHITE		(0x0007)
#define NT_FOREGROUND_INTENSITY		(0x0008)

/* console background color attributes */
#define NT_BACKGROUND_BLACK		(0x0000)
#define NT_BACKGROUND_BLUE		(0x0010)
#define NT_BACKGROUND_GREEN		(0x0020)
#define NT_BACKGROUND_CYAN		(0x0030)
#define NT_BACKGROUND_RED		(0x0040)
#define NT_BACKGROUND_MAGENTA		(0x0050)
#define NT_BACKGROUND_YELLOW		(0x0060)
#define NT_BACKGROUND_WHITE		(0x0070)
#define NT_BACKGROUND_INTENSITY		(0x0080)

/* console common lvb attributes */
#define NT_COMMON_LVB_LEADING_BYTE	(0x0100)
#define NT_COMMON_LVB_TRAILING_BYTE	(0x0200)
#define NT_COMMON_LVB_GRID_HORIZONTAL	(0x0400)
#define NT_COMMON_LVB_GRID_LVERTICAL	(0x0800)
#define NT_COMMON_LVB_GRID_RVERTICAL	(0x1000)
#define NT_COMMON_LVB_REVERSE_VIDEO	(0x4000)
#define NT_COMMON_LVB_UNDERSCORE	(0x8000)

/* console font name length limit */
#define NT_LF_FACESIZE			(0x20)

/* console font families */
#define NT_FF_DONTCARE		(0x0 << 4)
#define NT_FF_ROMAN		(0x1 << 4)
#define NT_FF_SWISS		(0x2 << 4)
#define NT_FF_MODERN		(0x3 << 4)
#define NT_FF_SCRIPT		(0x4 << 4)
#define NT_FF_DECORATIVE	(0x5 << 4)

/* console font weight */
#define NT_FW_DONTCARE		0
#define NT_FW_THIN		100
#define NT_FW_EXTRALIGHT	200
#define NT_FW_ULTRALIGHT	200
#define NT_FW_LIGHT		300
#define NT_FW_NORMAL		400
#define NT_FW_REGULAR		400
#define NT_FW_MEDIUM		500
#define NT_FW_SEMIBOLD		600
#define NT_FW_DEMIBOLD		600
#define NT_FW_BOLD		700
#define NT_FW_EXTRABOLD		800
#define NT_FW_ULTRABOLD		800
#define NT_FW_HEAVY		900
#define NT_FW_BLACK		900

/* console selection bits */
#define NT_CONSOLE_NO_SELECTION			(0x0000)
#define NT_CONSOLE_SELECTION_IN_PROGRESS	(0x0001)
#define NT_CONSOLE_SELECTION_NOT_EMPTY		(0x0002)
#define NT_CONSOLE_MOUSE_SELECTION		(0x0004)
#define NT_CONSOLE_MOUSE_DOWN			(0x0008)

/* console mode bits */
#define NT_ENABLE_PROCESSED_INPUT	(0x0001)
#define NT_ENABLE_LINE_INPUT		(0x0002)
#define NT_ENABLE_ECHO_INPUT		(0x0004)
#define NT_ENABLE_WINDOW_INPUT		(0x0008)
#define NT_ENABLE_MOUSE_INPUT		(0x0010)
#define NT_ENABLE_INSERT_MODE		(0x0020)
#define NT_ENABLE_QUICK_EDIT_MODE	(0x0040)

/* console display modes */
#define NT_CONSOLE_FULLSCREEN		(0x0001)
#define NT_CONSOLE_FULLSCREEN_HARDWARE	(0x0002)


/* console api: console handles */
typedef int32_t __stdcall	ntcon_alloc_console(void);


typedef int32_t __stdcall	ntcon_attach_console(uint32_t pid);


typedef int32_t __stdcall	ntcon_free_console(void);


typedef void *	__stdcall	ntcon_get_std_handle(uint32_t ntcon_std_type);


/* console api: read/write using std handles */
typedef int32_t __stdcall	ntcon_read_console_ansi(
	_in_	void *		hconsole,
	_out_	char *		buffer,
	_in_	uint32_t	chars_to_read,
	_out_	uint32_t *	chars_read,
	_in_	void *		input_control	_optional_);


typedef int32_t __stdcall	ntcon_read_console_utf16(
	_in_	void *		hconsole,
	_out_	wchar16_t *	buffer,
	_in_	uint32_t	chars_to_read,
	_out_	uint32_t *	chars_read,
	_in_	void *		input_control	_optional_);


typedef int32_t	__stdcall	ntcon_write_console_ansi(
	_in_		void *		hconsole,
	_in_		void * const	buffer,
	_in_		uint32_t	bytes_sent,
	_out_		uint32_t *	bytes_written,
	_in_out_	void *		overlapped	_optional_);

typedef int32_t	__stdcall	ntcon_write_console_utf16(
	_in_		void *		hconsole,
	_in_		void * const	buffer,
	_in_		uint32_t	bytes_sent,
	_out_		uint32_t *	bytes_written,
	_in_out_	void *		overlapped	_optional_);


/* console api: interface mode */
typedef int32_t __stdcall	ntcon_get_console_mode(
	_in_	void *		hconsole,
	_out_	uint32_t *	mode);


typedef int32_t __stdcall	ntcon_set_console_mode(
	_in_	void *		hconsole,
	_in_	uint32_t	mode);


/* console api: code page */
typedef uint32_t __stdcall	ntcon_get_console_code_page(void);


typedef int32_t __stdcall	ntcon_set_console_code_page(uint32_t code_page);

#endif /* _NTCON_H_ */
