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

SUBDIRS = src script po data music map doc debian pkg

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = config.rpath

all-local:
	cd $(abs_srcdir)/src && ./stamp.sh auto

clean-local:
	rm -f @PACKAGE_TARNAME@-*.tar.gz # wildcard because stamp is ever-increasing
	rm -f confdefs.h # causes problems on Jenkins (still there...)

distclean-local:
	rm -rf autom4te.cache # curiously this directory is left by autotools

uninstall-local:
	rm -rf $(DESTDIR)$(includedir)/@PACKAGE_TARNAME@
	rm -rf $(DESTDIR)$(libdir)/@PACKAGE_TARNAME@-@PACKAGE_VERSION@
	rm -rf $(DESTDIR)$(datadir)/@PACKAGE_TARNAME@-@PACKAGE_VERSION@

deb:
	echo "Copy source tarball to ./pkg then \"cd pkg\" and build package there." 

rpm:
	echo "Copy source tarball to ./pkg then \"cd pkg\" and build package there." 

indent:
	cd $(top_srcdir)/src && ./indent.sh

