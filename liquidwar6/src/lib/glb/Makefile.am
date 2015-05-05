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

noinst_LTLIBRARIES = libgl1b.la
if LW6_CUNIT
TEST_SRC_FILE = glb-test.c
else # LW6_CUNIT
TEST_SRC_FILE =
endif # LW6_CUNIT
libgl1b_la_SOURCES = glb-base64.c glb-sha1.c $(TEST_SRC_FILE) gnulib-base64.c gnulib-hmacsha1.c gnulib-memxor.c gnulib-sha1.c
libgl1bdir= $(includedir)/@PACKAGE_TARNAME@/glb
libgl1b_HEADERS = glb.h

if LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6glb-test
liquidwar6glb_test_SOURCES = glb-testmain.c
liquidwar6glb_test_LDADD = libgl1b.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
TESTS = liquidwar6glb-test
endif # !FULLSTATIC
endif # LW6_CUNIT

EXTRA_DIST = glb-internal.h gnulib-base64.h gnulib-hmac.h gnulib-memxor.h gnulib-sha1.h

clean-local:
	rm -f *.log *.trs

