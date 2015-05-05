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

noinst_LTLIBRARIES = libmat.la
if LW6_CUNIT
TEST_SRC_FILE = mat-test.c
else # LW6_CUNIT
TEST_SRC_FILE =
endif # LW6_CUNIT
libmat_la_SOURCES = mat-fmat2.c mat-fmat3.c mat-fmat4.c mat-fvec2.c mat-fvec3.c mat-fvec4.c mat-imat2.c mat-imat3.c mat-imat4.c mat-ivec2.c mat-ivec3.c mat-ivec4.c mat-dmat2.c mat-dmat3.c mat-dmat4.c mat-dvec2.c mat-dvec3.c mat-dvec4.c mat-xmat2.c mat-xmat3.c mat-xmat4.c mat-xvec2.c mat-xvec3.c mat-xvec4.c mat-similar.c $(TEST_SRC_FILE)
libmatdir= $(includedir)/@PACKAGE_TARNAME@/mat
libmat_HEADERS = mat.h

if LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6mat-test
liquidwar6mat_test_SOURCES = mat-testmain.c
liquidwar6mat_test_LDADD = libmat.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
TESTS = liquidwar6mat-test
endif # !FULLSTATIC
endif # LW6_CUNIT

clean-local:
	rm -f *.log *.trs

