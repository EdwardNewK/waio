ARCHBITS=32
ARCH=nt$(ARCHBITS)
CC=i686-w64-mingw32-gcc
LD=i686-w64-mingw32-ld
AR=i686-w64-mingw32-ar
OBJDUMP=i686-w64-mingw32-objdump
STRIP=i686-w64-mingw32-strip
CFLAGS+=-UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT$(ARCHBITS)
CFLAGS+=-DUNICODE
DEFAULT_UNDERSCORE=_
LDFLAGS=-lkernel32 --subsystem=console
INCLUDEDIR=-Isrc/internal -Isrc/internal/headers -Iinclude
BINDIR=bin$(ARCHBITS)
LIBDIR=lib$(ARCHBITS)
