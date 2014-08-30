ARCHBITS=32
ARCH=nt$(ARCHBITS)
CC=i686-nt32-midipix-gcc
LD=i686-nt32-midipix-ld
AR=i686-nt32-midipix-ar
OBJDUMP=i686-nt32-midipix-objdump
STRIP=i686-nt32-midipix-strip
CFLAGS+=-UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT$(ARCHBITS)
CFLAGS+=-DUNICODE
DEFAULT_UNDERSCORE=_
LDFLAGS=-lkernel32 --subsystem=console
INCLUDEDIR=-Isrc/internal -Isrc/internal/headers -Iinclude
BINDIR=bin$(ARCHBITS)
LIBDIR=lib$(ARCHBITS)
