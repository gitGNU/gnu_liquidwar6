# Liquid War 6 is a unique multiplayer wargame.
# Copyright (C)  2005, 2006, 2007, 2008, 2009  Christian Mauduit <ufoot@ufoot.org>
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

all-local:
	echo "Updating Debian control files for version @PACKAGE_VERSION@"
	if grep -q @PACKAGE_VERSION@ $(srcdir)/liquidwar6.dirs ; then echo "Debian control files already set up for version @PACKAGE_VERSION@" ; else sed -i -e "s/@PACKAGE_TARNAME@-[a-zA-Z0-9]\+\.[a-zA-Z0-9]\+\.[a-zA-Z0-9]\+/@PACKAGE_TARNAME@-@PACKAGE_VERSION@/g" $(srcdir)/*.dirs $(srcdir)/*.install ; fi

EXTRA_DIST = changelog compat control copyright liquidwar6-data.dirs liquidwar6-data.docs liquidwar6-data.install liquidwar6-devel.dirs liquidwar6-devel.docs liquidwar6-devel.install liquidwar6.dirs liquidwar6.doc-base liquidwar6.docs liquidwar6.info liquidwar6.install liquidwar6-mod-gl-data.dirs liquidwar6-mod-gl-data.docs liquidwar6-mod-gl-data.install liquidwar6-mod-gl.dirs liquidwar6-mod-gl.docs liquidwar6-mod-gl.install liquidwar6-mod-http-data.dirs liquidwar6-mod-http-data.docs liquidwar6-mod-http-data.install liquidwar6-mod-http.dirs liquidwar6-mod-http.docs liquidwar6-mod-http.install liquidwar6-mod-ogg-data.dirs liquidwar6-mod-ogg-data.docs liquidwar6-mod-ogg-data.install liquidwar6-mod-ogg.dirs liquidwar6-mod-ogg.docs liquidwar6-mod-ogg.install rules

