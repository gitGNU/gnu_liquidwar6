# Liquid War 6 is a unique multiplayer wargame.
# Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it wil/l be useful,
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

AC_INIT([Liquid War 6 extra maps],[0.0.10beta],[ufoot@ufoot.org],[liquidwar6-extra-maps])

AC_ARG_ENABLE([liquidwar6], AC_HELP_STRING([--enable-liquidwar6=PATH-TO-LIQUIDWAR6-BINARY],[specify the liquidwar6 binary which will use the extra maps, this overrides the --prefix settings]), [LIQUIDWAR6_BIN=$enableval], [LIQUIDWAR6_BIN=no])

if test "$LIQUIDWAR6_BIN" == "yes" ; then
    AC_CHECK_PROG(LIQUIDWAR6, liquidwar6, yes, no)
    if test "$LIQUIDWAR6" == "yes" ; then 
        LIQUIDWAR6_BIN=`which liquidwar6`
    else
	LIQUIDWAR6_BIN=no
    fi
fi

LW6_MAP_DIR=${datadir}/liquidwar6-${PACKAGE_VERSION}/map
if test "$LIQUIDWAR6_BIN" != "no"  && test -x "$LIQUIDWAR6_BIN" ; then
   LW6_MAP_DIR=`liquidwar6 --show-map-dir`
fi

AC_SUBST(LW6_MAP_DIR)

AC_CONFIG_FILES([Makefile
	liquidwar6-extra-maps.spec
	debian/Makefile
	debian/liquidwar6-extra-maps.dirs
	debian/liquidwar6-extra-maps.install
])

AC_OUTPUT

AC_MSG_NOTICE([prefix is "${prefix}"])

if test "$LIQUIDWAR6_BIN" != "no"  && test -x "$LIQUIDWAR6_BIN" ; then
   AC_MSG_NOTICE([liquidwar6 binary is "$LIQUIDWAR6_BIN"])
   AC_MSG_NOTICE([mapdir is "$LW6_MAP_DIR"])
fi

