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
# generates dummy LW6DEF_FOO_BAR="foo-bar" C #defines, along
# with equivalent (define lw6def-foo-bar "foo-bar") entries.
# The idea is to avoid typesettings errors when using raw string
# keys in programs. Indeed, C compiler and Scheme interpreter
# will both raise explicit errors when using such undefined keys.

import re

defs_c=""
defs_scm=""

f=open("def-list.txt","r")
for line in f.readlines():
    if len(line.strip())>=3 :
        key_upper=line.upper().replace("-","_").strip()
        key_lower=line.lower().replace("_","-").strip()
        print key_lower
        defs_c=defs_c + ("#define LW6DEF_%s \"%s\"" % (key_upper,key_lower))+ "\n" 
        defs_scm=defs_scm + ("(define lw6def-%s \"%s\")" % (key_lower,key_lower)) + "\n"
f.close()

def_h="""/*
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
 * Generated automatically by def-update.py
 */

#ifndef LIQUIDWAR6DEF_H
#define LIQUIDWAR6DEF_H

%s

#endif // LIQUIDWAR6DEF_H

""" % (defs_c)

def_scm=""";; Liquid War 6 is a unique multiplayer wargame.
;; Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015  Christian Mauduit <ufoot@ufoot.org>
;;
;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;;
;;
;; Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
;; Contact author        : ufoot@ufoot.org

;; Generated automatically by def-update.py

%s

""" % (defs_scm)

f=open("def.h","w")
f.write(def_h)
f.close
f=open("../../../script/def.scm","w")
f.write(def_scm)
f.close
