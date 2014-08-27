rm -f ldpath.mak &&
syslibdir=`$@ -print-file-name=libkernel32.a | xargs dirname` &&
cat >> ldpath.mak << EOF
-L$syslibdir
EOF
