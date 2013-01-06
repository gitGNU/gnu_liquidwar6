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

echo "******** $0 $(date) ********"
if cd liquidwar6 ; then
    echo "******** $0 $(date) ********"
    if autoreconf ; then
	echo "******** $0 $(date) ********"
	if ./configure --prefix=$WORKSPACE/local ; then
            echo "******** $0 $(date) ********"
            if make dist; then
		echo "******** $0 $(date) ********"
		if cp liquidwar6-*.tar.gz doc/ && make -C doc pub; then
		    echo "******** $0 $(date) ********"
                    if install -d $WORKSPACE/pub && cd .. ; then
                        echo "******** $0 $(date) ********"
                        if for i in coverage global cyclo doxygen manual ; do tar -xzf $WORKSPACE/liquidwar6/doc/$i.tar.gz -C $WORKSPACE/pub ; done; then
                            echo "******** $0 $(date) ********"
                        else
                            echo "extract pub failed"
                            exit 8
                        fi
                    else
                        echo "install pub failed"
                        exit 7
                    fi
		else
                    echo "cd failed"
                    exit 6
		fi
            else
		echo "make pub failed"
		exit 5
            fi
        else
            echo "make dist failed"
            exit 4
        fi
    else
	echo "./configure failed"
	exit 3
    fi
else
    echo "autoreconf failed"
    exit 2
fi
else
echo "cd failed"
exit 1
fi

echo "OK"
exit 0
