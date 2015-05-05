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

pkgdata_DATA = rules.xml hints.xml style.xml teams.xml
pkgdatadir = $(docdir)/example
EXTRA_DIST = $(pkgdata_DATA)

if LW6_SELFDOC
$(srcdir)/rules.xml: $(top_srcdir)/src/lib/map/map.h $(top_srcdir)/src/lib/ldr/ldr.h
	$(top_builddir)/src/lib/ldr/liquidwar6-xml --example-rules-xml | grep -v "DOCTYPE liquidwar6 SYSTEM" > $@

$(srcdir)/hints.xml: $(top_srcdir)/src/lib/map/map.h $(top_srcdir)/src/lib/ldr/ldr.h
	$(top_builddir)/src/lib/ldr/liquidwar6-xml --example-hints-xml | grep -v "DOCTYPE liquidwar6 SYSTEM" > $@

$(srcdir)/style.xml: $(top_srcdir)/src/lib/map/map.h $(top_srcdir)/src/lib/ldr/ldr.h
	$(top_builddir)/src/lib/ldr/liquidwar6-xml --example-style-xml | grep -v "DOCTYPE liquidwar6 SYSTEM" > $@

$(srcdir)/teams.xml: $(top_srcdir)/src/lib/map/map.h $(top_srcdir)/src/lib/ldr/ldr.h
	$(top_builddir)/src/lib/ldr/liquidwar6-xml --example-teams-xml | grep -v "DOCTYPE liquidwar6 SYSTEM" > $@
endif # LW6_SELFDOC

