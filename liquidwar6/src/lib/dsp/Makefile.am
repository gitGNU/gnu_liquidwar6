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

if ALLINONE
if MOD_GL1
AM_CFLAGS = @LW6_CFLAGS@ @SDL_CFLAGS@
AM_OBJCFLAGS = @LW6_CFLAGS@ @SDL_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@ @GL1_LIBS@ @GLU_LIBS@ @SDL_LIBS@
else # MOD_GL1
if MOD_GLES2
AM_CFLAGS = @LW6_CFLAGS@ @SDL_CFLAGS@
AM_OBJCFLAGS = @LW6_CFLAGS@ @SDL_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@ @GLES2_LIBS@ @SDL_LIBS@
else # MOD_GLES2
AM_CFLAGS = @LW6_CFLAGS@
AM_OBJCFLAGS = @LW6_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@
endif # MOD_GLES2
endif # MOD_GL1
else # ALLINONE
AM_CFLAGS = @LW6_CFLAGS@
AM_OBJCFLAGS = @LW6_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@
endif # ALLINONE

if ALLINONE
LW6DYN_LIB = 
else # ALLINONE
LW6DYN_LIB = ../dyn/libdyn.la 
endif # ALLINONE

if LW6_MAC_OS_X
OBJC_SRC_FILES = dsp-macosx.m
OBJC_EXTRA_FILES =
else # LW6_MAC_OS_X
OBJC_SRC_FILES =
OBJC_EXTRA_FILES = dsp-macosx.m
endif # LW6_MAC_OS_X

noinst_LTLIBRARIES = libdsp.la
if LW6_CUNIT
TEST_SRC_FILE = dsp-test.c
else # LW6_CUNIT
TEST_SRC_FILE =
endif # LW6_CUNIT
libdsp_la_SOURCES = dsp-data.c dsp-display.c $(OBJC_SRC_FILES) dsp-param.c dsp-thread.c $(TEST_SRC_FILE)
libdspdir= $(includedir)/@PACKAGE_TARNAME@/dsp
libdsp_HEADERS = dsp.h

if LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6dsp-test
liquidwar6dsp_test_SOURCES = dsp-testmain.c
liquidwar6dsp_test_LDADD = libdsp.la ../gfx/libgfx.la $(LW6DYN_LIB) ../cns/libcns.la ../vox/libvox.la ../gui/libgui.la ../mat/libmat.la ../ldr/libldr.la ../pil/libpil.la ../ker/libker.la ../map/libmap.la ../cfg/libcfg.la ../hlp/libhlp.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
TESTS = liquidwar6dsp-test
endif # !FULLSTATIC
endif # LW6_CUNIT

EXTRA_DIST = dsp-internal.h $(OBJC_EXTRA_FILES)

clean-local:
	rm -f *.log *.trs

