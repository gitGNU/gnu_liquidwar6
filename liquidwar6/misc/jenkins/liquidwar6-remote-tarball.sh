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

# Script used by Jenkins on non-POSIX (typically, MS-Windows)
# platforms to remotely build a working tarball.

GITREV=$1

if [ "x$GITREV" = "x" ] ; then
    exit 1
fi

TMPDIR=/tmp/liquidwar6-$(date "+%Y%m%d-%H%M%S")-$GITREV

echo $TMPDIR
install -d $TMPDIR && cd $TMPDIR
git clone git://git.savannah.gnu.org/liquidwar6.git && cd liquidwar6/liquidwar6 && git checkout $GITREV && ./bootstrap && ./configure && make && make dist
FILENAME=$(ls *.gz)
if [ -f $FILENAME ] ; then
    cp $FILENAME $TMPDIR
fi

echo $TMPDIR/$FILENAME

echo "******** $0 $(date) ********"
echo "OK"
exit 0
