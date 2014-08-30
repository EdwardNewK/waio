all:	tu lib

tu:
	sh ./makeobjs.sh $(CC) -c $(CFLAGS) -D__WAIO_BUILD__ -D__WAIO_INTERNAL -DMIDIPIX_WIN$(ARCHBITS) $(DEBUG) $(INCLUDEDIR) -Werror=all
	sh ./makeflag.sh $(LDFLAGS)
	sh ./makeldpath.sh $(CC)
	sh ./makeapp.sh  $(LD) -o $(BINDIR)/waio.exe \
		--entry $(DEFAULT_UNDERSCORE)waio_tu_entry_point \
		--image-base=0x1920000

lib:	lib-shared lib-static

lib-shared:
	sh ./makelibo.sh $(CC) -c $(CFLAGS) -D__WAIO_BUILD__ -D__WAIO_INTERNAL -DMIDIPIX_WIN$(ARCHBITS) $(DEBUG) $(INCLUDEDIR) -Werror=all
	sh ./makeflag.sh $(LDFLAGS)
	sh ./makeldpath.sh $(CC)
	sh ./makeapp.sh  $(LD) -o $(LIBDIR)/libwaio.dll \
		-shared \
		--entry $(DEFAULT_UNDERSCORE)waio_lib_entry_point \
		--output-def $(LIBDIR)/libwaio.def \
		--out-implib $(LIBDIR)/libwaio.dll.a


lib-static:
	sh ./makelibo.sh $(CC) -c $(CFLAGS) -D__WAIO_STATIC__ -D__WAIO_INTERNAL -DMIDIPIX_WIN$(ARCHBITS) $(DEBUG) $(INCLUDEDIR) -Werror=all
	./makelib.sh $(AR) rcs $(LIBDIR)/libwaio.a

clean:
	rm -rf makeobjs.mak
	rm -rf ldflags.mak
	rm -rf ldpath.mak
	rm -rf Debug
	rm -rf Release
	rm -rf obj/*
	rm -rf bin32/*
	rm -rf bin64/*
	rm -rf lib32/*
	rm -rf lib64/*
