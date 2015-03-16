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

OPT_EXTRA="opt-extra:"
MAKEFILE_MAIN=perf/Makefile.main
MAKEFILE_EXTRA=perf/Makefile.extra
MAKEFILE_AM=perf/Makefile.am

cp $MAKEFILE_MAIN $MAKEFILE_AM
for i in bot cfg cli cns cnx dat dsp dyn gen gfx glb gui hlp img ker ldr map mat msg net nod p2p pil scm sim snd srv sys tsk vox ; do
	OPT_EXTRA="$OPT_EXTRA liquidwar6${i}-test-cpu.jpeg liquidwar6${i}-test-heap.jpeg"
	echo >> $MAKEFILE_AM
	cat $MAKEFILE_EXTRA | sed "s/xyz/${i}/g" >> $MAKEFILE_AM
done
echo >> $MAKEFILE_AM
echo "$OPT_EXTRA" >> $MAKEFILE_AM

