ARCHBITS=64
ARCH=nt$(ARCHBITS)
CC=x86_64-nt64-midipix-gcc
LD=x86_64-nt64-midipix-ld
OBJDUMP=x86_64-nt64-midipix-objdump
STRIP=x86_64-nt64-midipix-strip
CFLAGS+=-UWIN32 -U_WIN32 -U__WIN32__ -UWIN64 -U_WIN64 -U__WIN64__ -D__NT$(ARCHBITS)
CFLAGS+=-DUNICODE
DEFAULT_UNDERSCORE=
LDFLAGS=-lkernel32 --subsystem=console
INCLUDEDIR=-Isrc/internal -Isrc/internal/headers -Iinclude
BINDIR=bin$(ARCHBITS)
