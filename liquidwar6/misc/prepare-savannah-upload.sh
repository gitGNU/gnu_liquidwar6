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
# Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
# Contact author        : ufoot@ufoot.org

# Script used to prepare Savannah uploads
# Valuable info on http://savannah.gnu.org/maintenance/DownloadArea
# Typical rsync command:
# rsync release.tar.gz you@dl.sv.nongnu.org:/releases/project/

SRC="/storage/download/liquidwar/v6"
SIG="/storage/sig"
EXCLUDE="csound mingw32 snapshot"
DST="$HOME/home/tmp/liquidwar6-savannah-upload"

rm -rf $DST
install -d $DST

for i in $SRC/*; do
	ok=1
	for j in $EXCLUDE; do
		if echo $i | grep -q $j; then
			ok=0
		fi
	done
	if [ "$ok" == "1" ]; then
		echo "Copying $i"
		cp -r "$i" "$DST"
	fi
done

for f in `find $DST -type f`; do
	b=`basename $f`
	if [ -f "$SIG/$b.sig" ] ; then
		cp "$SIG/$b.sig" "$f.sig"
	else
		echo "No sig for $b!"
		rm -f "$f"
	fi
done

