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

AM_CFLAGS = @LW6_CFLAGS@ @SDL_CFLAGS@
if ALLINONE
AM_LDFLAGS = @LW6_LDFLAGS@
else # ALLINONE
AM_LDFLAGS = @LW6_LDFLAGS@ @GLES2_LIBS@ @SDL_LIBS@
endif # ALLINONE

libdir = @libdir@/@PACKAGE_TARNAME@-@LW6_VERSION_BASE@/gfx

noinst_LTLIBRARIES = libmod_gles2_s.la
MOD_GLES2_FILES = mod-gles2-backend.c mod-gles2-const.c mod-gles2-mode.c mod-gles2-path.c mod-gles2-repr.c mod-gles2-resolution.c mod-gles2-setup.c mod-gles2-viewport.c
libmod_gles2_s_la_SOURCES = $(MOD_GLES2_FILES) 

if !ALLINONE
lib_LTLIBRARIES = libmod_gles2.la
libmod_gles2_la_SOURCES = $(MOD_GLES2_FILES) 
libmod_gles2_la_LIBADD = $(MOD_GLES2_LIBS) 
libmod_gles2_la_LDFLAGS = -release @PACKAGE_VERSION@ @LW6_LDFLAGS@ @GLES2_LIBS@ @SDL_LIBS@
libmod_gles2dir = $(includedir)/@PACKAGE_TARNAME@/gfx/mod-gles2
libmod_gles2_HEADERS = mod-gles2.h
endif # !ALLINONE

EXTRA_DIST = mod-gles2-internal.h

