# this build script is a joke and will soon be replaced.

mkdir -p obj
mkdir -p lib32
mkdir -p lib64

# create list for make
rm -f makeobjs.mak
cat > makeobjs.mak << EOF
objs:
EOF

# get list of source files
srcfiles=`cd src && ls *.c | sed -e 's/\.c//g'`

for f in $srcfiles
do
cat >> makeobjs.mak << EOF
	echo && $@ src/$f.c -o obj/$f.o
EOF
done


# get list of source files
srcfiles=`cd src && ls internal/*.c | sed -e 's/\.c//g'`

for f in $srcfiles
do
b=`basename $f`
cat >> makeobjs.mak << EOF
	echo && $@ src/$f.c -o obj/$b.o
EOF
done


# get list of source files
srcfiles=`cd src && ls ntapi/*.c | sed -e 's/\.c//g'`

for f in $srcfiles
do
b=`basename $f`
cat >> makeobjs.mak << EOF
	echo && $@ src/$f.c -o obj/$b.o
EOF
done


# get list of source files
srcfiles=`cd src && ls llapi/*.c | sed -e 's/\.c//g'`

for f in $srcfiles
do
b=`basename $f`
cat >> makeobjs.mak << EOF
	echo && $@ src/$f.c -o obj/$b.o
EOF
done


# clean obj directory
rm -rf obj/*

# make object files
echo
make -f makeobjs.mak
