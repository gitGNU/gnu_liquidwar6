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
AM_CFLAGS = @LW6_CFLAGS@ @MOD_GL1_ENABLE_CFLAGS@ @SDL_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@ @GL1_LIBS@ @GLU_LIBS@ @SDL_LIBS@
else # MOD_GL1
if MOD_GLES2
AM_CFLAGS = @LW6_CFLAGS@ @MOD_GLES2_ENABLE_CFLAGS@ @SDL_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@ @GLES2_LIBS@ @SDL_LIBS@
else # MOD_GLES2
if MOD_SOFT
AM_CFLAGS = @LW6_CFLAGS@ @MOD_SOFT_ENABLE_CFLAGS@ @SDL_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@ @SOFT_LIBS@ @SDL_LIBS@
else # MOD_SOFT
if MOD_CACA
AM_CFLAGS = @LW6_CFLAGS@ @MOD_CACA_ENABLE_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@ @CACA_LIBS@
else # MOD_CACA
AM_CFLAGS = @LW6_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@
endif # MOD_CACA
endif # MOD_SOFT
endif # MOD_GLES2
endif # MOD_GL1
else # ALLINONE
AM_CFLAGS = @LW6_CFLAGS@ @MOD_GL1_ENABLE_CFLAGS@ @MOD_GLES2_ENABLE_CFLAGS@ @MOD_SOFT_ENABLE_CFLAGS@ @MOD_CACA_ENABLE_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@
endif # ALLINONE

if MOD_GL1
MOD_GL1_SUBDIR = mod-gl1
MOD_GL1_LIB_S = mod-gl1/libmod_gl1_s.la
else # MOD_GL1
MOD_GL1_SUBDIR =
MOD_GL1_LIB_S =
endif # MOD_GL1

if MOD_GLES2
MOD_GLES2_SUBDIR = mod-gles2
MOD_GLES2_LIB_S = mod-gles2/libmod_gles2_s.la
else # MOD_GLES2
MOD_GLES2_SUBDIR =
MOD_GLES2_LIB_S =
endif # MOD_GLES2

if MOD_SOFT
MOD_SOFT_SUBDIR = mod-soft
MOD_SOFT_LIB_S = mod-soft/libmod_soft_s.la
else # MOD_SOFT
MOD_SOFT_SUBDIR =
MOD_SOFT_LIB_S =
endif # MOD_SOFT

if SHARED_SDL
SHARED_SDL_SUBDIR = shared-sdl
SHARED_SDL_LIB_S = shared-sdl/libshared_sdl_s.la
else # SHARED_SDL
SHARED_SDL_SUBDIR =
SHARED_SDL_LIB_S =
endif # SHARED_SDL

if MOD_CACA
MOD_CACA_SUBDIR = mod-caca
MOD_CACA_LIB_S = mod-caca/libmod_caca_s.la
else # MOD_CACA
MOD_CACA_SUBDIR =
MOD_CACA_LIB_S =
endif # MOD_CACA

if ALLINONE
LW6DYN_LIB = 
LW6GFX_LIB_S = $(MOD_GL1_LIB_S) $(MOD_GLES2_LIB_S) $(MOD_SOFT_LIB_S) $(SHARED_SDL_LIB_S) $(MOD_CACA_LIB_S)
else # ALLINONE
LW6DYN_LIB = ../dyn/libdyn.la 
LW6GFX_LIB_S =
endif # ALLINONE

noinst_LTLIBRARIES = libgfx.la
libgfx_la_LIBADD = $(LW6GFX_LIB_S) 
if LW6_CUNIT
TEST_SRC_FILE = gfx-test.c
else # LW6_CUNIT
TEST_SRC_FILE =
endif # LW6_CUNIT
libgfx_la_SOURCES = gfx-api.c gfx-register.c gfx-sdl.c $(TEST_SRC_FILE)
libgfxdir= $(includedir)/@PACKAGE_TARNAME@/gfx
libgfx_HEADERS = gfx.h

if LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6gfx-test
liquidwar6gfx_test_SOURCES = gfx-testmain.c
liquidwar6gfx_test_LDADD = libgfx.la $(LW6DYN_LIB) ../cns/libcns.la ../vox/libvox.la ../gui/libgui.la ../mat/libmat.la ../pil/libpil.la ../ker/libker.la ../map/libmap.la ../cfg/libcfg.la ../hlp/libhlp.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
TESTS = liquidwar6gfx-test
endif # !FULLSTATIC
endif # LW6_CUNIT

SUBDIRS = $(MOD_GL1_SUBDIR) $(MOD_GLES2_SUBDIR) $(MOD_SOFT_SUBDIR) $(SHARED_SDL_SUBDIR) $(MOD_CACA_SUBDIR)

EXTRA_DIST = gfx-internal.h

clean-local:
	rm -f *.log *.trs

