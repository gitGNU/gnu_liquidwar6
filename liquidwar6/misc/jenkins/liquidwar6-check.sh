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

if uname | grep Darwin ; then
    MAC_OS_X=yes
fi

# This will build and check the program, it runs it a quite
# paranoid mode, running a check then a dist then a distcheck.
# If this passes, one can be confident program is OK.

echo "******** $0 $(date) ********"
git clean -d -f -x
rm -rf $HOME/.liquidwar6
if cd liquidwar6 && rm -f *.gz doc/*.gz pkg/*.gz pkg/*vendor* pkg/*.zip pkg/*.exe pkg/*.dmg ; then
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
if make clean && make ; then
    echo "make OK"
else
    echo "make failed"
    exit 4
fi

echo "******** $0 $(date) ********"
if make distcheck ; then
    echo "make distcheck OK"
else
    echo "make distcheck failed"
    exit 5
fi

if [ -f /etc/debian_version ] ; then
    echo "******** $0 $(date) ********"
    if cp *.gz pkg/ && make -C pkg deb ; then
        echo "make deb OK"
    else
        echo "make deb failed"
        exit 6
    fi

    echo "******** $0 $(date) ********"
    if scp -P 9221 pkg/*.deb jenkins@10.0.2.2:/var/lib/jenkins/pub/snapshots/debian/ ; then
        echo "scp OK"
    else
        echo "scp failed"
        exit 7
    fi
fi

if [ -f /etc/redhat-release ] ; then
    echo "******** $0 $(date) ********"
    if cp *.gz pkg/ && make -C pkg rpm ; then
        echo "make rpm OK"
    else
        echo "make rpm failed"
        exit 8
    fi

    echo "******** $0 $(date) ********"
    if scp -P 9221 pkg/*.rpm jenkins@10.0.2.2:/var/lib/jenkins/pub/snapshots/redhat/ ; then
        echo "scp OK"
    else
        echo "scp failed"
        exit 9
    fi
fi

echo "******** $0 $(date) ********"
if cd $WORKSPACE/liquidwar6-extra-maps && rm -f *.gz ; then
    echo "cd liquidwar6-extra-maps OK"
else
    echo "cd liquidwar6-extra-maps failed"
    exit 10
fi

echo "******** $0 $(date) ********"
if ./bootstrap ; then
    echo "bootstrap OK"
else
    echo "bootstrap failed"
    exit 11
fi

echo "******** $0 $(date) ********"
if ./configure --prefix=$WORKSPACE/local ; then
    echo "./configure OK"
else
    echo "./configure failed"
    exit 12
fi

echo "******** $0 $(date) ********"
if make ; then
    echo "make OK"
else
    echo "make failed"
    exit 13
fi

echo "******** $0 $(date) ********"
if make dist ; then
    echo "make dist OK"
else
    echo "make dist failed"
    exit 14
fi

if [ -f /etc/debian_version ] ; then
    echo "******** $0 $(date) ********"
    if make deb ; then
        echo "make extra deb OK"
    else
        echo "make extra deb failed"
        exit 15
    fi

    echo "******** $0 $(date) ********"
    if scp -P 9221 *.deb jenkins@10.0.2.2:/var/lib/jenkins/pub/snapshots/debian/ ; then
        echo "scp extra OK"
    else
        echo "scp extra failed"
        exit 16
    fi
fi

if [ -f /etc/redhat-release ] ; then
    echo "******** $0 $(date) ********"
    if make rpm ; then
        echo "make extra rpm OK"
    else
        echo "make extra rpm failed"
        exit 17
    fi

    echo "******** $0 $(date) ********"
    if scp -P 9221 *.rpm jenkins@10.0.2.2:/var/lib/jenkins/pub/snapshots/redhat/ ; then
        echo "scp extra OK"
    else
        echo "scp extra failed"
        exit 18
    fi
fi

echo "******** $0 $(date) ********"
echo "OK"
exit 0
