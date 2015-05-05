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

AM_CFLAGS = @LW6_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@

noinst_LTLIBRARIES = libmap.la
if LW6_CUNIT
TEST_SRC_FILE = map-test.c
else # LW6_CUNIT
TEST_SRC_FILE =
endif # LW6_CUNIT
libmap_la_SOURCES = map-body.c map-color.c map-coords.c map-cursortexture.c map-cursortexturelayer.c map-dup.c map-exp.c map-hexa.c map-layer.c map-level.c map-localinfo.c map-metadata.c map-metalayer.c map-rules.c map-param.c map-style.c map-teams.c $(TEST_SRC_FILE) map-texture.c map-weapon.c
libmapdir= $(includedir)/@PACKAGE_TARNAME@/map
libmap_HEADERS = map.h

if LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6map-test
liquidwar6map_test_SOURCES = map-testmain.c
liquidwar6map_test_LDADD = libmap.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
TESTS = liquidwar6map-test
endif # !FULLSTATIC
endif # LW6_CUNIT

clean-local:
	rm -f *.log *.trs

