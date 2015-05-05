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

AM_CFLAGS = @LW6_CFLAGS@ @CACA_CFLAGS@
if ALLINONE
AM_LDFLAGS = @LW6_LDFLAGS@
else # ALLINONE
AM_LDFLAGS = @LW6_LDFLAGS@ @CACA_LIBS@
endif # ALLINONE

libdir = @libdir@/@PACKAGE_TARNAME@-@LW6_VERSION_BASE@/gfx

noinst_LTLIBRARIES = libmod_caca_s.la
MOD_CACA_FILES = mod-caca-backend.c mod-caca-const.c mod-caca-display.c mod-caca-event.c mod-caca-mode.c mod-caca-path.c mod-caca-repr.c mod-caca-resolution.c mod-caca-setup.c mod-caca-menu.c mod-caca-game.c mod-caca-utils.c mod-caca-splash.c
libmod_caca_s_la_SOURCES = $(MOD_CACA_FILES) 

if !ALLINONE
lib_LTLIBRARIES = libmod_caca.la
libmod_caca_la_SOURCES = $(MOD_CACA_FILES) 
libmod_caca_la_LIBADD = $(MOD_CACA_LIBS) 
libmod_caca_la_LDFLAGS = -release @PACKAGE_VERSION@ @LW6_LDFLAGS@ @CACA_LIBS@
libmod_cacadir = $(includedir)/@PACKAGE_TARNAME@/gfx/mod-caca
libmod_caca_HEADERS = mod-caca.h
endif # !ALLINONE

EXTRA_DIST = mod-caca-internal.h
