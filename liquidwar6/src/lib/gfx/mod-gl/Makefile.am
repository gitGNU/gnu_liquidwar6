# Liquid War 6 is a unique multiplayer wargame.
# Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>
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

AM_CFLAGS = @LW6_CFLAGS@ @SDL_CFLAGS@
if ALLINONE
AM_LDFLAGS = @LW6_LDFLAGS@
else
AM_LDFLAGS = @LW6_LDFLAGS@ @SDL_LIBS@
endif

libdir = $(prefix)/lib/@PACKAGE_TARNAME@-@PACKAGE_VERSION@/gfx

noinst_LTLIBRARIES = libmod_gl_s.la
MOD_GL_FILES = mod-gl-backend.c mod-gl-repr.c mod-gl-setup.c
MOD_GL_LIBS = gl-utils/libgl-utils.la gl-splash/libgl-splash.la gl-background/libgl-background.la gl-hud/libgl-hud.la gl-menu/libgl-menu.la gl-view/libgl-view.la
libmod_gl_s_la_SOURCES = $(MOD_GL_FILES) 
libmod_gl_s_la_LIBADD = $(MOD_GL_LIBS)

if !ALLINONE
lib_LTLIBRARIES = libmod_gl.la
libmod_gl_la_SOURCES = $(MOD_GL_FILES) 
libmod_gl_la_LIBADD = $(MOD_GL_LIBS) 
libmod_gl_la_LDFLAGS = -release @PACKAGE_VERSION@ @LW6_LDFLAGS@ @SDL_LIBS@
libmod_gldir = $(includedir)/@PACKAGE_TARNAME@/gfx/mod-gl
libmod_gl_HEADERS = mod-gl.h
endif

EXTRA_DIST = mod-gl-internal.h

SUBDIRS = gl-utils gl-splash gl-background gl-hud gl-menu gl-view
