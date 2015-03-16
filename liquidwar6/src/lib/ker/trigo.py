#!/usr/bin/python

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

# This is a basic Python (http://www.python.org/) program which
# generates cos & sin tables for use in Liquid War 6 algorithms.
# Indeed, we don't want to rely on floating point routines for
# core algorithms, since most of these must be 100.00000000000%
# predictable, which is not garanted with floating point routines.
#
# The script generates ker-trigo.[ch] which is in turn used as
# C code. It's not intented to be run at each compilation. Instead
# it's ran once, then stays untouched. This way it's compiled on
# one single platform for a given version of Liquid War 6, which
# it's exactly what we want: ker-trigo.h being the same everywhere.
#
# Note that one optimization would be to only keep 1/4 of the tables
# and get sin(x) PI/2<x<PI from sin(PI-x) for instance, but it might 
# be as fast to skip this test and consumme a little more memory.

import math

CONST_2PI = 0x4000
CONST_PI = 0x2000
CONST_PI2 = 0x1000
CONST_2PI_MASK = 0x3fff
CONST_PI_MASK = 0x1fff
CONST_PI2_MASK = 0x0fff
CONST_RADIUS = 0x4000

trigo_h="""/*
Liquid War 6 is a unique multiplayer wargame.
Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015  Christian Mauduit <ufoot@ufoot.org>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
Contact author        : ufoot@ufoot.org
*/

/*
 * Generated automatically by trigo.py
 */

#ifndef LIQUIDWAR6KER_TRIGO_H
#define LIQUIDWAR6KER_TRIGO_H

#define LW6KER_TRIGO_2PI %d
#define LW6KER_TRIGO_PI %d
#define LW6KER_TRIGO_PI2 %d
#define LW6KER_TRIGO_2PI_MASK %d
#define LW6KER_TRIGO_PI_MASK %d
#define LW6KER_TRIGO_PI2_MASK %d
#define LW6KER_TRIGO_RADIUS %d

#include "../sys/sys.h"

extern int32_t LW6KER_TRIGO_SIN_TABLE[%d];

#endif // LIQUIDWAR6KER_TRIGO_H

""" % (CONST_2PI,CONST_PI,CONST_PI2,CONST_2PI_MASK,CONST_PI_MASK,CONST_PI2_MASK,CONST_RADIUS,CONST_2PI)

trigo_c="""/*
Liquid War 6 is a unique multiplayer wargame.
Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015  Christian Mauduit <ufoot@ufoot.org>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
Contact author        : ufoot@ufoot.org
*/

/*
 * Generated automatically by trigo.py
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include "ker-trigo.h"

int32_t LW6KER_TRIGO_SIN_TABLE[%d]={
""" % CONST_2PI

for alpha in range(CONST_2PI):
    trigo_c=trigo_c+("  %d,\n" % (math.sin((alpha*2*math.pi)/CONST_2PI)*CONST_RADIUS))

trigo_c=trigo_c+"""
};

"""

f=open("ker-trigo.h","w")
f.write(trigo_h)
f.close
f=open("ker-trigo.c","w")
f.write(trigo_c)
f.close
