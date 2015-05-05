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

if ALLINONE
AM_CFLAGS = @LW6_CFLAGS@ @MOD_CSOUND_ENABLE_CFLAGS@ @MOD_OGG_ENABLE_CFLAGS@ @SDL_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@ @SDL_LIBS_SOUND@
else # ALLINONE
AM_CFLAGS = @LW6_CFLAGS@ @MOD_CSOUND_ENABLE_CFLAGS@ @MOD_OGG_ENABLE_CFLAGS@ 
AM_LDFLAGS = @LW6_LDFLAGS@
endif # ALLINONE

if MOD_CSOUND
MOD_CSOUND_SUBDIR = mod-csound
MOD_CSOUND_LIB_S = mod-csound/libmod_csound_s.la
else # MOD_CSOUND
MOD_CSOUND_SUBDIR =
MOD_CSOUND_LIB_S =
endif # MOD_CSOUND

if MOD_OGG
MOD_OGG_SUBDIR = mod-ogg
MOD_OGG_LIB_S = mod-ogg/libmod_ogg_s.la
else # MOD_OGG
MOD_OGG_SUBDIR =
MOD_OGG_LIB_S =
endif # MOD_OGG

if ALLINONE
LW6DYN_LIB = 
LW6SND_LIB_S = $(MOD_CSOUND_LIB_S) $(MOD_OGG_LIB_S)
else # ALLINONE
LW6DYN_LIB = ../dyn/libdyn.la 
LW6SND_LIB_S =
endif # ALLINONE

noinst_LTLIBRARIES = libsnd.la
libsnd_la_LIBADD = $(LW6SND_LIB_S) 
if LW6_CUNIT
TEST_SRC_FILE = snd-test.c
else # LW6_CUNIT
TEST_SRC_FILE =
endif # LW6_CUNIT
libsnd_la_SOURCES = snd-api.c snd-register.c $(TEST_SRC_FILE)
libsnddir= $(includedir)/@PACKAGE_TARNAME@/snd
libsnd_HEADERS = snd.h

if LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6snd-test
liquidwar6snd_test_SOURCES = snd-testmain.c
liquidwar6snd_test_LDADD = libsnd.la $(LW6DYN_LIB) ../cfg/libcfg.la ../hlp/libhlp.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
TESTS = liquidwar6snd-test
endif # !FULLSTATIC
endif # LW6_CUNIT

SUBDIRS = $(MOD_CSOUND_SUBDIR) $(MOD_OGG_SUBDIR)

clean-local:
	rm -f *.log *.trs

