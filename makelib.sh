# get list of object files
objfiles=`ls obj/*.o` &&

# link
echo $@ $objfiles
echo && $@ $objfiles