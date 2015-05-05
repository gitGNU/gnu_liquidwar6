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
AM_LDFLAGS = @LW6_LDFLAGS@ @SOFT_LIBS@ @SDL_LIBS@
endif # ALLINONE

libdir = @libdir@/@PACKAGE_TARNAME@-@LW6_VERSION_BASE@/gfx

noinst_LTLIBRARIES = libmod_soft_s.la
MOD_SOFT_FILES = mod-soft-backend.c mod-soft-const.c mod-soft-mode.c mod-soft-path.c mod-soft-repr.c mod-soft-resolution.c mod-soft-setup.c mod-soft-viewport.c
libmod_soft_s_la_SOURCES = $(MOD_SOFT_FILES) 

if !ALLINONE
lib_LTLIBRARIES = libmod_soft.la
libmod_soft_la_SOURCES = $(MOD_SOFT_FILES) 
libmod_soft_la_LIBADD = $(MOD_SOFT_LIBS) 
libmod_soft_la_LDFLAGS = -release @PACKAGE_VERSION@ @LW6_LDFLAGS@ @SOFT_LIBS@ @SDL_LIBS@
libmod_softdir = $(includedir)/@PACKAGE_TARNAME@/gfx/mod-soft
libmod_soft_HEADERS = mod-soft.h
endif # !ALLINONE

EXTRA_DIST = mod-soft-internal.h
