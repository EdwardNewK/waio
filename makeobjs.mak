objs:
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/waio_alloc.c -o obj/waio_alloc.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/waio_cancel.c -o obj/waio_cancel.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/waio_error.c -o obj/waio_error.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/waio_fcntl.c -o obj/waio_fcntl.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/waio_free.c -o obj/waio_free.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/waio_fsync.c -o obj/waio_fsync.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/waio_ioctl.c -o obj/waio_ioctl.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/waio_listio.c -o obj/waio_listio.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/waio_read_write.c -o obj/waio_read_write.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/waio_return.c -o obj/waio_return.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/waio_suspend.c -o obj/waio_suspend.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/internal/waio_xvtbls.c -o obj/waio_xvtbls.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/ntapi/ntapi_tt_create_event.c -o obj/ntapi_tt_create_event.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/ntapi/ntapi_tt_hex_utf8_to_uintptr.c -o obj/ntapi_tt_hex_utf8_to_uintptr.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/ntapi/ntapi_tt_uintptr_to_hex_utf8.c -o obj/ntapi_tt_uintptr_to_hex_utf8.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/llapi/waio_init.c -o obj/waio_init.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/llapi/waio_io.c -o obj/waio_io.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/llapi/waio_loop.c -o obj/waio_loop.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/llapi/waio_thread_shutdown.c -o obj/waio_thread_shutdown.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/llapi/waio_thread_shutdown_fallback.c -o obj/waio_thread_shutdown_fallback.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/../test/waio_test.c -o obj/waio_test.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/../test/waio_test_hooks.c -o obj/waio_test_hooks.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/../test/waio_test_output.c -o obj/waio_test_output.o
	echo && x86_64-w64-mingw32-gcc -c -UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT64 -DUNICODE -D__WAIO_INTERNAL -DMIDIPIX_WIN64 -Isrc/internal -Isrc/internal/headers -Iinclude -Werror=all src/../test/waio_test_wine.c -o obj/waio_test_wine.o
