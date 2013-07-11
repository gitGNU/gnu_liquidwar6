#!/bin/sh

# Liquid War 6 is a unique multiplayer wargame.
# Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013  Christian Mauduit <ufoot@ufoot.org>
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

# This will build and check the program, it runs it a quite
# paranoid mode, running a check then a dist then a distcheck.
# If this passes, one can be confident program is OK.

echo "******** $0 $(date) ********"
if cd liquidwar6 ; then
    echo "******** $0 $(date) ********"
    if autoreconf ; then
	echo "******** $0 $(date) ********"
	if ./configure --prefix=$WORKSPACE/local ; then
	    echo "******** $0 $(date) ********"
	    if make ; then
		echo "******** $0 $(date) ********"
		killall Xvfb
		Xvfb :6 -pixdepths 1 8 16 24 32 -screen 0 640x480x24 &
		export DISPLAY=:6
		echo "******** $0 $(date) ********"
		# No check on --cunit output, make check will do this, we do not
		# want the whole build to fail just because of cunit, only, we
		# look at the XML output with Jenkins
		src/liquidwar6 --test
		killall Xvfb
		if [ -f $HOME/.liquidwar6/CUnit-Results.xml ] && [ -x /usr/bin/xsltproc ] ; then
		    cp $HOME/.liquidwar6/CUnit-Results.xml .
		    /usr/bin/xsltproc --output JUnit-Results.xml --path $WORKSPACE/liquidwar6/misc/cunit-to-junit/ $WORKSPACE/liquidwar6/misc/cunit-to-junit/cunit-to-junit.xsl CUnit-Results.xml
		fi
		echo "******** $0 $(date) ********"
		if make distcheck; then
		    echo "******** $0 $(date) ********"
		else
		    killall Xvfb
		    echo "make distcheck failed"
		    exit 5
		fi
	    else
		killall Xvfb
		echo "make failed"
		exit 4
	    fi
	else
	    killall Xvfb
	    echo "./configure failed"
	    exit 3
	fi
    else
	killall Xvfb
	echo "autoreconf failed"
	exit 2
    fi
else
echo "cd failed"
    exit 1
fi

echo "OK"
exit 0
