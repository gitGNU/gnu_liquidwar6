# Liquid War 6 is a unique multiplayer wargame.
# Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>
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
AM_CFLAGS = @LW6_CFLAGS@ @MOD_GL_ENABLE_CFLAGS@ @SDL_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@ @SDL_LIBS@
else
AM_CFLAGS = @LW6_CFLAGS@ @MOD_GL_ENABLE_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@
endif

if MOD_GL
MOD_GL_SUBDIR = mod-gl
MOD_GL_LIB_S = mod-gl/libmod_gl_s.la
else
MOD_GL_SUBDIR =
MOD_GL_LIB_S =
endif

if ALLINONE
LW6DYN_LIB = 
LW6GFX_LIB_S = $(MOD_GL_LIB_S)
else
LW6DYN_LIB = ../dyn/libdyn.la 
LW6GFX_LIB_S =
endif

noinst_LTLIBRARIES = libgfx.la
libgfx_la_LIBADD = $(LW6GFX_LIB_S) 
libgfx_la_SOURCES = gfx-api.c gfx-register.c gfx-test.c
libgfxdir= $(includedir)/@PACKAGE_TARNAME@/gfx
libgfx_HEADERS = gfx.h

if !FULLSTATIC
libexec_PROGRAMS = liquidwar6gfx-test
liquidwar6gfx_test_SOURCES = gfx-testmain.c
liquidwar6gfx_test_LDADD = libgfx.la $(LW6DYN_LIB) ../gui/libgui.la ../ldr/libldr.la ../pil/libpil.la ../ker/libker.la ../map/libmap.la ../cfg/libcfg.la ../hlp/libhlp.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
TESTS = liquidwar6gfx-test
endif

SUBDIRS = $(MOD_GL_SUBDIR)

