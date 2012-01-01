#!/bin/sh

# Liquid War 6 is a unique multiplayer wargame.
# Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# 
# Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
# Contact author        : ufoot@ufoot.org

# Utility script to update the build ID

BUILD_STAMP_H=./lib/sys/sys-build-stamp.h
BUILD_MD5SUM_H=./lib/sys/sys-build-md5sum.h
BUILD_STAMP_TEXI=../doc/stamp.texi

usage () {
    echo "./stamp.sh {next|auto|show}"
}

next_id () {
    i=`cat ${BUILD_STAMP_H} | cut -d "\"" -f 2`
    i=$(($i+1))
    echo "#define LW6_STAMP \"$i\"" > ${BUILD_STAMP_H}
    echo "@set LW6_STAMP $i" > ${BUILD_STAMP_TEXI}
}

auto_id () {
    # defining LW6_NO_STAMP avoids recompiling the same files over and over
    if [ "x${LW6_NO_STAMP}" = "x" ]; then
	if [ -f ${BUILD_STAMP_H} ] && [ -f ${BUILD_MD5SUM_H} ]; then
	    export LANG=C
	    export LC_ALL=C # important for sort
	    new_md5=`find lib -name "*.c" -a -not -name ".*" -a -not -path "*.libs*" | sort -u | xargs cat | tr -d [:cntrl:] | tr -d [:space:] | md5sum | cut -d " " -f 1`
	    old_md5=`cat ${BUILD_MD5SUM_H} | cut -d "\"" -f 2`
	    if [ "$new_md5" != "$old_md5" ] ; then
		echo "C source files changed"
		echo "old MD5SUM: $old_md5"
		echo "new MD5SUM: $new_md5"
		next_id
		echo "#define LW6_MD5SUM \"$new_md5\"" > ${BUILD_MD5SUM_H}
	    fi
	else
	    echo "Not in source tree"
	fi
    else
	echo "LW6_NO_STAMP defined, not updating stamp"
    fi
}

show_id () {
      cat ${BUILD_STAMP_H}
      cat ${BUILD_MD5SUM_H}
}

cd `dirname $0`

case "$1" in
    next)
	next_id
	show_id
	;;
    auto)
	auto_id
	show_id
	;;
    show)
	show_id
	;;
    *)
	usage
	;;
esac

exit 0
