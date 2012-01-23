#!/bin/sh

BUILDDIR=/mnt/ramdisk/liquidwar6
SRCDIR=$HOME/Home/_/liquidwar6
INSTALLDIR=$HOME/Home/local/liquidwar6

if test -e $SRCDIR/configure.ac ; then
	cd $SRCDIR
	aclocal -I m4 --install
	automake
	autoconf
	install -d $BUILDDIR
	cd $BUILDDIR
	$SRCDIR/configure --enable-profiler --prefix=$INSTALLDIR
	make clean > /dev/null
	make -j2
fi

