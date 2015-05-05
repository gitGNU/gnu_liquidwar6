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
AM_LDFLAGS = @LW6_LDFLAGS@ -lpng -ljpeg

noinst_LTLIBRARIES = libldr.la
if LW6_CUNIT
TEST_SRC_FILE = ldr-test.c
else # LW6_CUNIT
TEST_SRC_FILE =
endif # LW6_CUNIT
libldr_la_SOURCES = ldr-body.c ldr-bw.c ldr-color.c ldr-cursortexture.c ldr-cursortexturelayer.c ldr-dir.c ldr-exp.c ldr-metalayer.c ldr-grease.c ldr-hints.c ldr-layer.c ldr-metadata.c ldr-options.c ldr-param.c ldr-print.c ldr-read.c ldr-resampler.c ldr-rgba.c ldr-rules.c ldr-style.c ldr-teams.c $(TEST_SRC_FILE) ldr-texture.c ldr-use.c
libldrdir= $(includedir)/@PACKAGE_TARNAME@/ldr
libldr_HEADERS = ldr.h

if LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6-xml liquidwar6ldr-test
liquidwar6_xml_SOURCES = ldr-xmlmain.c
liquidwar6_xml_LDADD = libldr.la ../cfg/libcfg.la ../hlp/libhlp.la ../map/libmap.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
liquidwar6ldr_test_SOURCES = ldr-testmain.c
liquidwar6ldr_test_LDADD = libldr.la ../cfg/libcfg.la ../hlp/libhlp.la ../map/libmap.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
TESTS = liquidwar6ldr-test
endif # !FULLSTATIC
else # LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6-xml
liquidwar6_xml_SOURCES = ldr-xmlmain.c
liquidwar6_xml_LDADD = libldr.la ../cfg/libcfg.la ../hlp/libhlp.la ../map/libmap.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
endif # !FULLSTATIC
endif # LW6_CUNIT

EXTRA_DIST = ldr-internal.h

clean-local:
	rm -f *.log *.trs

