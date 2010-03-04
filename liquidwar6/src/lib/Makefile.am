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

if ALLINONE
AM_CFLAGS = @LW6_CFLAGS@ @GUILE_CFLAGS@ @SDL_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@ @GUILE_LDFLAGS@ @SDL_LIBS@ @SDL_LIBS_SOUND@
else
AM_CFLAGS = @LW6_CFLAGS@ @GUILE_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@ @GUILE_LDFLAGS@
endif

if ALLINONE
LW6DYN_LIB = 
LW6DYN_SUBDIR =
else
LW6DYN_LIB = dyn/libdyn.la 
LW6DYN_SUBDIR = dyn
endif

noinst_LTLIBRARIES = libliquidwar6_s.la
LW6_FILES = lw6-callback.c lw6-control.c lw6-funcs.c lw6-global.c lw6-main.c lw6-options.c lw6-print.c lw6-smobs.c lw6-test.c
LW6_LIBS = sys/libsys.la cns/libcns.la hlp/libhlp.la cfg/libcfg.la map/libmap.la ker/libker.la pil/libpil.la bot/libbot.la ldr/libldr.la gui/libgui.la gfx/libgfx.la dsp/libdsp.la snd/libsnd.la img/libimg.la net/libnet.la cli/libcli.la srv/libsrv.la p2p/libp2p.la tsk/libtsk.la $(LW6DYN_LIB) 
libliquidwar6_s_la_SOURCES = $(LW6_FILES)
libliquidwar6_s_la_LIBADD = $(LW6_LIBS)

if !ALLINONE
lib_LTLIBRARIES = libliquidwar6.la
libliquidwar6_la_SOURCES = $(LW6_FILES)
libliquidwar6_la_LIBADD = $(LW6_LIBS)
libliquidwar6_la_LDFLAGS = -release @PACKAGE_VERSION@ @LW6_LDFLAGS@ @GUILE_LDFLAGS@
libliquidwar6dir = $(includedir)/@PACKAGE_TARNAME@
libliquidwar6_HEADERS = liquidwar6.h
endif

if !FULLSTATIC
libexec_PROGRAMS = liquidwar6-test
liquidwar6_test_SOURCES = lw6-testmain.c
liquidwar6_test_LDADD = libliquidwar6_s.la $(LW6_LIBS) @LW6_LIBINTL_LDADD@
TESTS = liquidwar6-test
endif

SUBDIRS = def sys map ker pil $(LW6DYN_SUBDIR) bot cns hlp cfg ldr tsk gui gfx snd dsp img net cli srv p2p
