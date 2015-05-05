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

AM_CFLAGS = @LW6_CFLAGS@ @CSOUND_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@ @CSOUND_LIBS@

libdir = @libdir@/@PACKAGE_TARNAME@-@LW6_VERSION_BASE@/snd

noinst_LTLIBRARIES = libmod_csound_s.la
MOD_CSOUND_FILES = mod-csound-backend.c mod-csound-fx.c mod-csound-music.c mod-csound-path.c mod-csound-repr.c mod-csound-setup.c mod-csound-volume.c mod-csound-water.c 
libmod_csound_s_la_SOURCES = $(MOD_CSOUND_FILES) 

if !ALLINONE
lib_LTLIBRARIES = libmod_csound.la
libmod_csound_la_SOURCES =
libmod_csound_la_LIBADD = libmod_csound_s.la
libmod_csound_la_LDFLAGS = -release @PACKAGE_VERSION@ @LW6_LDFLAGS@ @CSOUND_LIBS@
libmod_csounddir = $(includedir)/@PACKAGE_TARNAME@/snd/mod-csound
libmod_csound_HEADERS = mod-csound.h
endif # !ALLINONE

EXTRA_DIST = mod-csound-internal.h
