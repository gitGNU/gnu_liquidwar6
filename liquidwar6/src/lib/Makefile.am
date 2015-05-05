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
if MOD_GL1
AM_CFLAGS = @LW6_CFLAGS@ @GUILE_CFLAGS@ @SDL_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@ @GL1_LIBS@ @GLU_LIBS@ @SDL_LIBS@ @SDL_LIBS_SOUND@
else # MOD_GL1
if MOD_GLES2
AM_CFLAGS = @LW6_CFLAGS@ @GUILE_CFLAGS@ @SDL_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@ @GLES2_LIBS@ @SDL_LIBS@ @SDL_LIBS_SOUND@
else # MOD_GLES2
AM_CFLAGS = @LW6_CFLAGS@ @GUILE_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@ @SDL_LIBS@ @SDL_LIBS_SOUND@
endif # MOD_GLES2
endif # MOD_GL1
else # ALLINONE
AM_CFLAGS = @LW6_CFLAGS@ @GUILE_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@
endif # ALLINONE

if ALLINONE
LW6DYN_LIB = 
LW6DYN_SUBDIR =
else # ALLINONE
LW6DYN_LIB = dyn/libdyn.la 
LW6DYN_SUBDIR = dyn
endif # ALLINONE

noinst_LTLIBRARIES = libliquidwar6_s.la
if LW6_CUNIT
TEST_SRC_FILE = lw6-test.c
else # LW6_CUNIT
TEST_SRC_FILE =
endif # LW6_CUNIT
LW6_FILES = lw6-callback.c lw6-control.c lw6-fix.c lw6-funcs.c lw6-funcssys.c lw6-funcshlp.c lw6-funcscfg.c lw6-funcsgen.c lw6-funcsgui.c lw6-funcsgfx.c lw6-funcsdsp.c lw6-funcsldr.c lw6-funcsmap.c lw6-funcsker.c lw6-funcspil.c lw6-funcssnd.c lw6-funcscns.c lw6-funcstsk.c lw6-funcsimg.c lw6-funcsnet.c lw6-funcscli.c lw6-funcssrv.c lw6-funcsp2p.c lw6-funcsbot.c lw6-global.c lw6-main.c lw6-options.c lw6-smobs.c $(TEST_SRC_FILE)
LW6_LIBS = sys/libsys.la glb/libgl1b.la cns/libcns.la hlp/libhlp.la cfg/libcfg.la map/libmap.la ker/libker.la pil/libpil.la bot/libbot.la sim/libsim.la gen/libgen.la ldr/libldr.la mat/libmat.la gui/libgui.la vox/libvox.la gfx/libgfx.la dsp/libdsp.la snd/libsnd.la img/libimg.la net/libnet.la nod/libnod.la cnx/libcnx.la msg/libmsg.la cli/libcli.la srv/libsrv.la dat/libdat.la p2p/libp2p.la scm/libscm.la tsk/libtsk.la $(LW6DYN_LIB) 
libliquidwar6_s_la_SOURCES = $(LW6_FILES)
libliquidwar6_s_la_LIBADD = $(LW6_LIBS)

if !ALLINONE
lib_LTLIBRARIES = libliquidwar6.la
libliquidwar6_la_SOURCES = $(LW6_FILES)
libliquidwar6_la_LIBADD = $(LW6_LIBS)
libliquidwar6_la_LDFLAGS = -release @PACKAGE_VERSION@ @LW6_LDFLAGS@ @GUILE_LDFLAGS@
libliquidwar6dir = $(includedir)/@PACKAGE_TARNAME@
libliquidwar6_HEADERS = liquidwar6.h
endif # !ALLINONE

if LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6-test
liquidwar6_test_SOURCES = lw6-testmain.c
liquidwar6_test_LDADD = libliquidwar6_s.la $(LW6_LIBS) @LW6_LIBINTL_LDADD@
TESTS = liquidwar6-test
endif # !FULLSTATIC
endif # LW6_CUNIT

SUBDIRS = def sys glb map ker pil $(LW6DYN_SUBDIR) bot sim gen cns hlp cfg ldr tsk mat gui vox gfx snd dsp img net nod cnx msg cli srv dat p2p scm

clean-local:
	rm -f *.log *.trs

