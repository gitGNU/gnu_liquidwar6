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

noinst_LTLIBRARIES = libgui.la
if LW6_CUNIT
TEST_SRC_FILE = gui-test.c
else # LW6_CUNIT
TEST_SRC_FILE =
endif # LW6_CUNIT
libgui_la_SOURCES = gui-button.c gui-coord.c gui-joystick.c gui-keyboard.c gui-keypress.c gui-input.c gui-look.c gui-menu.c gui-menuitem.c gui-mouse.c gui-point.c gui-poweroftwo.c gui-quad.c gui-rect.c gui-rectarray.c gui-segment.c gui-smoother.c $(TEST_SRC_FILE) gui-triangle.c gui-videomode.c gui-viewport.c gui-zone.c
libguidir= $(includedir)/@PACKAGE_TARNAME@/gui
libgui_HEADERS = gui.h

if LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6gui-test
liquidwar6gui_test_SOURCES = gui-testmain.c
liquidwar6gui_test_LDADD = libgui.la ../map/libmap.la ../cfg/libcfg.la ../hlp/libhlp.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
TESTS = liquidwar6gui-test
endif # !FULLSTATIC
endif # LW6_CUNIT

clean-local:
	rm -f *.log *.trs

