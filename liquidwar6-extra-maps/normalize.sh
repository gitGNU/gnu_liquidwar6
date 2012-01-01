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

find map -name "Makefile*" -exec rm "{}" \;
find map -name "*.db" -exec rm "{}" \;

for D in `find map -type d -a -not -name ".arch-ids" -a -not -name "CVS"`; do
	d=`echo $D | perl -e "print lc <>;" | tr "_. " "---"`
	if [ x"$D" != x"$d" ] ; then
		echo "$D -> $d"
		mv "$D" "$d"
	fi
done

for F in `find map -type f -a -not -name "README" -a -not -path "*.arch-ids*" -a -not -path "*CVS*"`; do
	f=`echo $F | perl -e "print lc <>;" | tr "_ " "--"`
	if [ x"$F" != x"$f" ] ; then
		echo "$F -> $f"
		mv "$F" "$f"
	fi
done
	
for r in `find map -name "readme*" -o -name "README*" -a -not -path "*.arch-ids*"`; do
	R=`echo $r | tr [A-Z] [a-z] | sed -e "s/readme.*/README/g"`
	if [ x"$r" != x"$R" ] ; then
		echo "$r -> $R"
		mv "$r" "$R"
	fi
done
	
for f_jpg in `find map -name "*.jpg"`; do
	f_jpeg=`echo $f_jpg | tr [A-Z] [a-z] | sed -e "s/jpg/jpeg/g"`
	echo "$f_jpg -> $f_jpeg"
	mv "$f_jpg" "$f_jpeg"
done

