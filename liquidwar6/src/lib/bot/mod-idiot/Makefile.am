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

AM_CFLAGS = @LW6_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@

libdir = @libdir@/@PACKAGE_TARNAME@-@LW6_VERSION_BASE@/bot

noinst_LTLIBRARIES = libmod_idiot_s.la
MOD_IDIOT_FILES = mod-idiot-backend.c mod-idiot-move.c mod-idiot-repr.c mod-idiot-setup.c
libmod_idiot_s_la_SOURCES = $(MOD_IDIOT_FILES) 

if !ALLINONE
lib_LTLIBRARIES = libmod_idiot.la
libmod_idiot_la_SOURCES = $(MOD_IDIOT_FILES) 
libmod_idiot_la_LDFLAGS = -release @PACKAGE_VERSION@ @LW6_LDFLAGS@
libmod_idiotdir = $(includedir)/@PACKAGE_TARNAME@/bot/mod-idiot
libmod_idiot_HEADERS = mod-idiot.h
endif # !ALLINONE

EXTRA_DIST = mod-idiot-internal.h
