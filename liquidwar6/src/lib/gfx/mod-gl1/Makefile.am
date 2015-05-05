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
AM_LDFLAGS = @LW6_LDFLAGS@ @GL1_LIBS@ @GLU_LIBS@ @SDL_LIBS@
endif # ALLINONE

libdir = @libdir@/@PACKAGE_TARNAME@-@LW6_VERSION_BASE@/gfx

noinst_LTLIBRARIES = libmod_gl1_s.la
MOD_GL1_FILES = mod-gl1-backend.c mod-gl1-repr.c mod-gl1-setup.c
MOD_GL1_LIBS = gl1-utils/libgl1-utils.la gl1-splash/libgl1-splash.la gl1-background/libgl1-background.la gl1-hud/libgl1-hud.la gl1-menu/libgl1-menu.la gl1-view/libgl1-view.la
libmod_gl1_s_la_SOURCES = $(MOD_GL1_FILES) 
libmod_gl1_s_la_LIBADD = $(MOD_GL1_LIBS)

if !ALLINONE
lib_LTLIBRARIES = libmod_gl1.la
libmod_gl1_la_SOURCES = $(MOD_GL1_FILES) 
libmod_gl1_la_LIBADD = $(MOD_GL1_LIBS) 
libmod_gl1_la_LDFLAGS = -release @PACKAGE_VERSION@ @LW6_LDFLAGS@ @GL1_LIBS@ @GLU_LIBS@ @SDL_LIBS@
libmod_gl1dir = $(includedir)/@PACKAGE_TARNAME@/gfx/mod-gl1
libmod_gl1_HEADERS = mod-gl1.h
endif # !ALLINONE

EXTRA_DIST = mod-gl1-internal.h

SUBDIRS = gl1-utils gl1-splash gl1-background gl1-hud gl1-menu gl1-view
