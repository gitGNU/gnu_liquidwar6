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

# Script used by Jenkins daemon to autobuild the program in
# continuous integration mode. http://jenkins-ci.org/

echo "******** $0 $(date) ********"
if cd liquidwar6 ; then
    echo "******** $0 $(date) ********"
    if autoreconf ; then
	echo "******** $0 $(date) ********"
	if ./configure --prefix=$WORKSPACE/local ; then
	    echo "******** $0 $(date) ********"
	    if make ; then
#		echo "******** $0 $(date) ********"
#		if make check; then
		    echo "******** $0 $(date) ********"
		    if cd ../liquidwar6-extra-maps ; then
			echo "******** $0 $(date) ********"
			if autoreconf ; then
			    echo "******** $0 $(date) ********"
			    if ./configure --prefix=$WORKSPACE/local ; then
				echo "******** $0 $(date) ********"
				if make ; then
				    echo "******** $0 $(date) ********"
				    if make install; then
					echo "******** $0 $(date) ********"
				    else
					echo "make install failed"
					exit 10
				    fi
				else
				    echo "make failed"
				    exit 9
				fi
			    else
				echo "./configure failed"
				exit 8
			    fi
			else
			    echo "autoreconf failed"
			    exit 7
			fi
		    else
			echo "cd failed"
			exit 6
		    fi
#		else
#		    echo "make check failed"
#		    exit 5
#		fi
	    else
		echo "make failed"
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
