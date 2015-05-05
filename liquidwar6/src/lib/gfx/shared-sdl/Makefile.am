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
AM_LDFLAGS = @LW6_LDFLAGS@ @SDL_LIBS@
endif # ALLINONE

libdir = @libdir@/@PACKAGE_TARNAME@-@LW6_VERSION_BASE@/gfx

noinst_LTLIBRARIES = libshared_sdl_s.la
SHARED_SDL_FILES = shared-sdl-backend.c shared-sdl-const.c shared-sdl-event.c shared-sdl-path.c shared-sdl-timer.c
libshared_sdl_s_la_SOURCES = $(SHARED_SDL_FILES) 

if !ALLINONE
lib_LTLIBRARIES = libshared_sdl.la
libshared_sdl_la_SOURCES = $(SHARED_SDL_FILES) 
libshared_sdl_la_LIBADD = $(SHARED_SDL_LIBS) 
libshared_sdl_la_LDFLAGS = -release @PACKAGE_VERSION@ @LW6_LDFLAGS@ @SDL_LIBS@
libshared_sdldir = $(includedir)/@PACKAGE_TARNAME@/gfx/shared-sdl
libshared_sdl_HEADERS = shared-sdl.h
endif # !ALLINONE

EXTRA_DIST = shared-sdl-internal.h
