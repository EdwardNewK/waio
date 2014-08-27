all:	tu

tu:
	sh ./makeobjs.sh $(CC) -c $(CFLAGS) -D__WAIO_INTERNAL -DMIDIPIX_WIN$(ARCHBITS) $(DEBUG) $(INCLUDEDIR) -Werror=all
	sh ./makeflag.sh $(LDFLAGS)
	sh ./makeldpath.sh $(CC)
	sh ./makeapp.sh  $(LD) -o $(BINDIR)/waio.exe \
		--entry $(DEFAULT_UNDERSCORE)waio_tu_entry_point \
		--image-base=0x1920000

lib:
	todo__BUILD_SCRIPT_FOR_LIB

clean:
	rm -rf makeobjs.mak
	rm -rf ldflags.mak
	rm -rf ldpath.mak
	rm -rf Debug
	rm -rf Release
	rm -rf obj/*
	rm -rf bin32/*
	rm -rf bin64/*
