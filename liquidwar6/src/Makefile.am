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
AM_CFLAGS = @LW6_CFLAGS@ -I${srcdir}/lib @GUILE_CFLAGS@ @SDL_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@ @GL1_LIBS@ @GLU_LIBS@ @SDL_LIBS@ @SDL_LIBS_SOUND@
else # MOD_GL1
if MOD_GLES2
AM_CFLAGS = @LW6_CFLAGS@ -I${srcdir}/lib @GUILE_CFLAGS@ @SDL_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@ @GLES2_LIBS@ @SDL_LIBS@ @SDL_LIBS_SOUND@
else # MOD_GLES2
AM_CFLAGS = @LW6_CFLAGS@ -I${srcdir}/lib @GUILE_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@ @SDL_LIBS@ @SDL_LIBS_SOUND@
endif # MOD_GLES2
endif # MOD_GL1
else # ALLINONE
AM_CFLAGS = @LW6_CFLAGS@ -I${srcdir}/lib @GUILE_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@
endif # ALLINONE

if FULLSTATIC
# Note: you might need to edit this to fit your system
LW6_FLAGS = -all-static -lgpm
else # FULLSTATIC
# Note: libgpm seem to be required only when linking statically
LW6_FLAGS = @LW6_LDFLAGS@
endif # FULLSTATIC

if LW6_MS_WINDOWS
# Automake isn't necessarly aware of windres and its use.
mswindows.o : ${srcdir}/mswindows.rc
	windres $^ -o $@

LW6_RC_SRC = mswindows.rc
LW6_RC_OBJ = mswindows.o
else # LW6_MS_WINDOWS
LW6_RC_SRC =
LW6_RC_OBJ =
endif # LW6_MS_WINDOWS

bin_PROGRAMS = liquidwar6
liquidwar6_SOURCES = main.c $(LW6_RC_SRC)
liquidwar6_LDADD = lib/libliquidwar6_s.la @LW6_LIBINTL_LDADD@ $(LW6_RC_OBJ)

EXTRA_DIST = scriptpo.c indent.sh scriptpo-update.sh stamp.sh clean.sh dtou.sh utod.sh mswindows.rc.in

SUBDIRS = lib

install-exec-local:
	if which id && which ldconfig && test `id -u` -eq 0 && test "$(prefix)" != "/usr"; then ldconfig; fi # Run ldconfig automatically if we're root
	if test -x $(DESTDIR)$(bindir)/liquidwar6; then mv $(DESTDIR)$(bindir)/liquidwar6 $(DESTDIR)$(bindir)/@PACKAGE_TARNAME@-@LW6_VERSION_BASE@; ln -s @PACKAGE_TARNAME@-@LW6_VERSION_BASE@ $(DESTDIR)$(bindir)/liquidwar6; fi

uninstall-local:
	rm -f $(DESTDIR)$(bindir)/@PACKAGE_TARNAME@-@LW6_VERSION_BASE@

clean-local:
	rm -f liquidwar6.exe
	find . -name "*.gcda" -exec rm -f "{}" \;
	find . -name "*.gcno" -exec rm -f "{}" \;

distclean-local:
	rm -f mswindows.rc

