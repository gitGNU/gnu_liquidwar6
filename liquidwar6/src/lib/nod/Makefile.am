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

noinst_LTLIBRARIES = libnod.la
if LW6_CUNIT
TEST_SRC_FILE = nod-test.c
else # LW6_CUNIT
TEST_SRC_FILE =
endif # LW6_CUNIT
libnod_la_SOURCES = nod-community.c nod-constinfo.c nod-dyninfo.c nod-info.c nod-refinfo.c $(TEST_SRC_FILE)
libnoddir= $(includedir)/@PACKAGE_TARNAME@/nod
libnod_HEADERS = nod.h

if LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6nod-test
liquidwar6nod_test_SOURCES = nod-testmain.c
liquidwar6nod_test_LDADD = libnod.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
TESTS = liquidwar6nod-test
endif # !FULLSTATIC
endif # LW6_CUNIT

EXTRA_DIST = nod-internal.h

clean-local:
	rm -f *.log *.trs

