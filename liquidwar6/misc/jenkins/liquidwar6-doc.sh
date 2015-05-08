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

# Script used by Jenkins daemon to autobuild the program in
# continuous integration mode. http://jenkins-ci.org/

if test x$WORKSPACE = x ; then
    if test x$TMP = x ; then
	if test x$TMPDIR = x ; then
	    WORKSPACE=/tmp
	else
	    WORKSPACE=$TMPDIR
	fi
    else
	WORKSPACE=$TMP
    fi
fi	

# This will build the docs, which is quite long and has been
# separated from the other builds. The idea was to allow quick
# builds for the main and avoid it being reported as broken when
# there's only a doc generation problem. Which is of small concern
# for end users as doc ships already generated.

echo "******** $0 $(date) ********"
git clean -d -f -x
rm -rf $HOME/.liquidwar6
if cd liquidwar6 && rm -f *.gz doc/*.gz pkg/*.gz pkg/*vendor* ; then
    echo "cd liquidwar6 OK"
else
    echo "cd liquidwar6 failed"
    exit 1
fi

echo "******** $0 $(date) ********"
if ./bootstrap ; then
    echo "bootstrap OK"
else
    echo "bootstrap failed"
    exit 2
fi

echo "******** $0 $(date) ********"
if ./configure --prefix=$WORKSPACE/local ; then
    echo "./configure OK"
else
    echo "./configure failed"
    exit 3
fi

echo "******** $0 $(date) ********"
if make ; then
    echo "make OK"
else
    echo "make failed"
    exit 4
fi

echo "******** $0 $(date) ********"
if make dist ; then
    echo "make dist OK"
else
    echo "make dist failed"
    exit 5
fi

echo "******** $0 $(date) ********"
DOC_DONE=no
if [ -x /usr/bin/Xvfb ] ; then
    echo "******** $0 $(date) ********"
    killall Xvfb
    Xvfb :6 -pixdepths 1 8 16 24 32 -screen 0 640x480x24 &
    export DISPLAY=:6
    export TERM=none-when-building-with-jenkins
    echo "******** $0 $(date) ********"
    if cp liquidwar6-*.tar.gz doc/ && make -C doc pub; then
	echo "make pub OK"
	DOC_DONE=yes
    else
	echo "make pub failed"
	exit 6
    fi
else
    echo "unable to find Xvfb"
fi

if [ x$DOC_DONE = xyes ] ; then
    echo "doc OK"
else
    echo "doc failed"
    exit 7
fi

echo "******** $0 $(date) ********"
if rm -rf $WORKSPACE/pub && install -d $WORKSPACE/pub && cd .. && for i in coverage global cyclo doxygen perf manual ; do tar -xzf $WORKSPACE/liquidwar6/doc/$i.tar.gz -C $WORKSPACE/pub ; done; then
    echo "extract pub OK"
else
    echo "extract pub failed"
    exit 8
fi

echo "******** $0 $(date) ********"
if rsync --archive --delete $WORKSPACE/pub/ /var/lib/jenkins/pub/doc/ ; then
    echo "rsync OK"
else
    echo "rsync failed"
    exit 9
fi

echo "******** $0 $(date) ********"
echo "OK"
exit 0
