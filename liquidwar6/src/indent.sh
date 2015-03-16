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

# Automatically indent source files.

# OK, GNU standards require 80 columns, however LW6 does have very
# long variables and functions names, which causes it to be quite
# unreadable. So it's raised to 159, which is 2*80-1, which means
# lines can be spawned on two lines. On modern terminals with wide
# 16/9 screens (HD, full-HD & co) this is usually not a problem.
LINE_LENGTH=159

./scriptpo-update.sh

#../src/dtou.sh `find ../src/ -name "*.c"`
#../src/dtou.sh `find ../src/ -name "*.h"`
#../src/dtou.sh `find ../src/ -name "*.in"`

# Indent C file twice to avoid weird diffs (ping-pong effect)
find ../src/ -name "*.c" -exec indent -l$LINE_LENGTH "{}" \;
find ../src/ -name "*.c" -exec indent -l$LINE_LENGTH "{}" \;
find ../src/ -name "*.m" -exec indent -l$LINE_LENGTH "{}" \;
find ../src/ -name "*.h" -exec indent -l$LINE_LENGTH "{}" \;
find ../src/ -name "*.h" -exec indent -l$LINE_LENGTH "{}" \;
find ../src/ -name "*.h.in" -exec indent -l$LINE_LENGTH "{}" \;

./clean.sh
./stamp.sh auto

