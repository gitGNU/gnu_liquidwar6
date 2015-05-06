#!/bin/sh

# Liquid War 6 is a unique multiplayer wargame.
# Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015  Christian Mauduit <ufoot@ufoot.org>
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
#BUILD_STAMP_TEXI=../doc/stamp.texi
VERSION_MAJOR=0
VERSION_MINOR=6

if test "$1" = "show" ; then
    # in "show" mode, do not query for complex path, readlink
    # has some compatibility issues on Mac OS X, and in a general
    # manner, if showing only, keep it simple stupid, no need
    # to set vars we won't use.
    true
else
    HERE="$(readlink -f $(dirname $0) || readlink $(dirname $0))"
    if [ -f ../configure.ac ] ; then
	CONFIGURE_AC="$(readlink -f ../configure.ac || readlink ../configure.ac)"
	if [ -f ${CONFIGURE_AC} ] ; then
            CONFIGURE_AC_EXTRA_MAPS="$(dirname $(dirname ${CONFIGURE_AC}))/liquidwar6-extra-maps/configure.ac"
	else
            echo "unable to open ${CONFIGURE_AC}"
            exit 2
	fi
    else
	echo "unable to find ../configure.ac"
	exit 1
    fi
fi

usage () {
    echo "./stamp.sh {next|auto|show}"
}

next_id () {
    stamp=`cat ${BUILD_STAMP_H} | grep STAMP | cut -d "\"" -f 2`
    stamp=$((${stamp}+1))
    echo "#define LW6_STAMP \"${stamp}\"" > ${BUILD_STAMP_H}
    #echo "@set LW6_STAMP ${stamp}" > ${BUILD_STAMP_TEXI}
    echo "patching ${CONFIGURE_AC}"
    sed -i "s/^AC_INIT.*/AC_INIT([Liquid War 6],[${VERSION_MAJOR}.${VERSION_MINOR}.${stamp}],[ufoot@ufoot.org],[liquidwar6],[http:\/\/www.gnu.org\/software\/liquidwar6\/])/g" ${CONFIGURE_AC}
    sed -i "s/^LW6_VERSION_BASE.*/LW6_VERSION_BASE=\"${VERSION_MAJOR}.${VERSION_MINOR}\"/g" ${CONFIGURE_AC}
    sed -i "s/^LW6_VERSION_MAJOR.*/LW6_VERSION_MAJOR=\"${VERSION_MAJOR}\"/g" ${CONFIGURE_AC}
    sed -i "s/^LW6_VERSION_MINOR.*/LW6_VERSION_MINOR=\"${VERSION_MINOR}\"/g" ${CONFIGURE_AC}
    sed -i "s/^LW6_MS_WINDOWS_VERSION.*/LW6_MS_WINDOWS_VERSION=\"${VERSION_MAJOR}, ${VERSION_MINOR}, ${stamp}\"/g" ${CONFIGURE_AC}
    if [ -f ${CONFIGURE_AC_EXTRA_MAPS} ] ; then
        echo "patching ${CONFIGURE_AC_EXTRA_MAPS}"
        sed -i "s/^AC_INIT.*/AC_INIT([Liquid War 6 extra maps],[${VERSION_MAJOR}.${VERSION_MINOR}.${stamp}],[ufoot@ufoot.org],[liquidwar6-extra-maps],[http:\/\/www.gnu.org\/software\/liquidwar6\/])/g" ${CONFIGURE_AC_EXTRA_MAPS}
        sed -i "s/^LW6_VERSION_BASE.*/LW6_VERSION_BASE=\"${VERSION_MAJOR}.${VERSION_MINOR}\"/g" ${CONFIGURE_AC_EXTRA_MAPS}
    else
        echo "skipping ${CONFIGURE_AC_EXTRA_MAPS} (not found)"
    fi
    if [ -x /usr/bin/debchange ] ; then
	DEBIAN_CHANGELOG="$(dirname ${CONFIGURE_AC})/debian"
	DEBIAN_EXTRA_MAPS_CHANGELOG="$(dirname $(dirname ${CONFIGURE_AC}))/liquidwar6-extra-maps/debian"
        echo "updating ${DEBIAN_CHANGELOG}/changelog"
	if cat ${DEBIAN_CHANGELOG}/changelog | head -n 3 | grep "Bump version" ; then cat ${DEBIAN_CHANGELOG}/changelog | tail -n +7 > ${DEBIAN_CHANGELOG}/changelog.old ; mv ${DEBIAN_CHANGELOG}/changelog.old ${DEBIAN_CHANGELOG}/changelog ; fi
	cd ${DEBIAN_CHANGELOG} && /usr/bin/debchange --newversion ${VERSION_MAJOR}.${VERSION_MINOR}.${stamp}-0vendor --release-heuristic log --maintmaint "Bump version"
        echo "updating ${DEBIAN_EXTRA_MAPS_CHANGELOG}/changelog"
	if cat ${DEBIAN_EXTRA_MAPS_CHANGELOG}/changelog | head -n 3 | grep "Bump version" ; then cat ${DEBIAN_EXTRA_MAPS_CHANGELOG}/changelog | tail -n +7 > ${DEBIAN_EXTRA_MAPS_CHANGELOG}/changelog.old ; mv ${DEBIAN_EXTRA_MAPS_CHANGELOG}/changelog.old ${DEBIAN_EXTRA_MAPS_CHANGELOG}/changelog ; fi
	cd ${DEBIAN_EXTRA_MAPS_CHANGELOG} && /usr/bin/debchange --newversion ${VERSION_MAJOR}.${VERSION_MINOR}.${stamp}-0vendor --release-heuristic log --maintmaint "Bump version"
	cd "${HERE}"
    fi
}

auto_id () {
    # Defining LW6_STAMP_AUTO required for stamp to increase automatically.
    # This is not the default since it causes some (lots?) of stupid git
    # conflicts when merging. The idea is to have it set for "main" developper
    # or at least he who is responsible for releasing, as well as for Jenkins
    # continuous integration builder and all kind of "official" builds.
    if [ "x${LW6_STAMP_AUTO}" != "x" ]; then
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
            else
                echo "C source files unchanged"
            fi
        else
            echo "Not in source tree"
        fi
    else
        # Not defined, not doing anything.
        true
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
