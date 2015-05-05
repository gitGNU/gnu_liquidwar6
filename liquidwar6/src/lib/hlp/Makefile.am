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
AM_LDFLAGS = @LW6_LDFLAGS@ -lexpat

noinst_LTLIBRARIES = libhlp.la
if LW6_CUNIT
TEST_SRC_FILE = hlp-test.c
else # LW6_CUNIT
TEST_SRC_FILE =
endif # LW6_CUNIT
libhlp_la_SOURCES = hlp-about.c hlp-credits.c hlp-keyword.c hlp-list.c hlp-options.c hlp-print.c hlp-refadvanced.c hlp-refaliases.c hlp-refdoc.c hlp-reffuncs.c hlp-refgraphics.c hlp-refhooks.c hlp-refinput.c hlp-refmap.c hlp-refmaphints.c hlp-refmaprules.c hlp-refmapstyle.c hlp-refmapteams.c hlp-refnetwork.c hlp-refpath.c hlp-refplayers.c hlp-refquick.c hlp-refshow.c hlp-refsound.c hlp-refteamcolors.c hlp-refweapons.c hlp-ref.c $(TEST_SRC_FILE)
libhlpdir= $(includedir)/@PACKAGE_TARNAME@/hlp
libhlp_HEADERS = hlp.h

if LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6-doc liquidwar6hlp-test
liquidwar6_doc_SOURCES = hlp-docmain.c
liquidwar6_doc_LDADD = libhlp.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
liquidwar6hlp_test_SOURCES = hlp-testmain.c
liquidwar6hlp_test_LDADD = libhlp.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
TESTS = liquidwar6hlp-test
endif # !FULLSTATIC
else # LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6-doc
liquidwar6_doc_SOURCES = hlp-docmain.c
liquidwar6_doc_LDADD = libhlp.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
endif # !FULLSTATIC
endif # LW6_CUNIT

EXTRA_DIST = hlp-internal.h

clean-local:
	rm -f *.log *.trs

