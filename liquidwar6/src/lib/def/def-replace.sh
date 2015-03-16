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

# This shell will search/replace "foo-bar" keys in C and Scheme
# sources and replace them with equivalent LW6DEF_FOO_BAR
# and lw6def-foo-bar equivalents.

for i in `cat def-list.txt`; do
	if [ ${#i} -ge 3 ]; then
		i_upper=`echo $i | tr "[a-z]-" "[A-Z]_"`
		i_lower=`echo $i | tr "[A-Z]_" "[a-z]-"`
		echo "$i_lower"
		find ../../../src -name "*.c" -exec sed -i "s/\"$i_lower\"/LW6DEF_$i_upper/g" "{}" \;
		find ../../../script -name "*.scm" -a -not -name "def.scm" -exec sed -i "s/\"$i_lower\"/lw6def-$i_lower/g" "{}" \;
	fi
done

