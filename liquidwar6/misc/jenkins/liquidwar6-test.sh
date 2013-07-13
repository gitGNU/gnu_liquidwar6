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

# This will test the program, test won't fail if some tests
# do not pass, it's used to collect test stats through Jenkins

echo "******** $0 $(date) ********"
if cd liquidwar6 && rm -f *.gz doc/*.gz pkg/*.gz pkg/*vendor* ; then
    echo "cd liquidwar6 OK"
else
    echo "cd liquidwar6 failed"
    exit 1
fi

echo "******** $0 $(date) ********"
if autoreconf ; then
    echo "autoreconf OK"
else
    echo "autoreconf failed"
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
TEST_DONE=no
if [ -x /usr/bin/Xvfb ] ; then
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
    if [ -f $HOME/.liquidwar6/CUnit-Results.xml ] && [ -x /usr/bin/xsltproc ] && [ -x /usr/bin/xmllint ] ; then
        rm -f *-Results.xml
        xmllint --recover $HOME/.liquidwar6/CUnit-Results.xml > CUnit-Results.xml
        /usr/bin/xsltproc --output JUnit-Results.xml --path ./misc/cunit-to-junit/ misc/cunit-to-junit/cunit-to-junit.xsl CUnit-Results.xml
        if [ -f $HOME/.liquidwar6/log.csv ] ; then
            cp -f $HOME/.liquidwar6/log.csv .
            cp -f $HOME/.liquidwar6/net/tcp-send.log .
            cp -f $HOME/.liquidwar6/net/tcp-recv.log .
            cp -f $HOME/.liquidwar6/net/udp-send.log .
            cp -f $HOME/.liquidwar6/net/udp-recv.log .
            cp -f $HOME/.liquidwar6/httpd/access_log.txt .
            TEST_DONE=yes
        else
            echo "unable to find $HOME/.liquidwar6/log.csv"
        fi
    else
        echo "unable to find $HOME/.liquidwar6/CUnit-Results.xml and XML tools"
    fi
else
    echo "unable to find Xvfb"
fi
if [ x$TEST_DONE = xyes ] ; then
    echo "test OK"
else
    echo "test failed"
    exit 5
fi

echo "******** $0 $(date) ********"
echo "OK"
exit 0
