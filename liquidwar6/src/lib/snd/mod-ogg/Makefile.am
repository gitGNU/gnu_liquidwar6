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
AM_LDFLAGS = @LW6_LDFLAGS@ @SDL_LIBS_SOUND@
endif # ALLINONE

libdir = @libdir@/@PACKAGE_TARNAME@-@LW6_VERSION_BASE@/snd

noinst_LTLIBRARIES = libmod_ogg_s.la
MOD_OGG_FILES = mod-ogg-backend.c mod-ogg-const.c mod-ogg-fx.c mod-ogg-music.c mod-ogg-path.c mod-ogg-repr.c mod-ogg-setup.c mod-ogg-volume.c mod-ogg-water.c
libmod_ogg_s_la_SOURCES = $(MOD_OGG_FILES) 

if !ALLINONE
lib_LTLIBRARIES = libmod_ogg.la
libmod_ogg_la_SOURCES = 
libmod_ogg_la_LIBADD = libmod_ogg_s.la
libmod_ogg_la_LDFLAGS = -release @PACKAGE_VERSION@ @LW6_LDFLAGS@ @SDL_LIBS_SOUND@
libmod_oggdir = $(includedir)/@PACKAGE_TARNAME@/snd/mod-ogg
libmod_ogg_HEADERS = mod-ogg.h
endif # !ALLINONE

EXTRA_DIST = mod-ogg-internal.h

