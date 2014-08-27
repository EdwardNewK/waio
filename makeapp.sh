# get list of object files
objfiles=`ls obj/*.o` &&

# LDFLAGS
LDFLAGS="`cat ldpath.mak` `cat ldflags.mak`" &&

# link
echo && $@ $objfiles $LDFLAGS
