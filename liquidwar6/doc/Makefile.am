# Liquid War 6 is a unique multiplayer wargame.
# Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>
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

SUBDIRS = coverage global cyclo example dtd

info_TEXINFOS = liquidwar6.texi
gdoc_TEXINFOS = liquidwar6-gdoc.texi bot-gdoc.texi cfg-gdoc.texi cli-gdoc.texi cns-gdoc.texi cnx-gdoc.texi dat-gdoc.texi dsp-gdoc.texi dyn-gdoc.texi fdl.texi gfx-gdoc.texi glb-gdoc.texi gpl.texi gui-gdoc.texi hlp-gdoc.texi img-gdoc.texi ker-gdoc.texi ldr-gdoc.texi liquidwar6-gdoc.texi map-gdoc.texi mod-csound-gdoc.texi mod-follow-gdoc.texi mod-gl-gdoc.texi mod-httpd-gdoc.texi mod-http-gdoc.texi mod-idiot-gdoc.texi mod-ogg-gdoc.texi mod-random-gdoc.texi mod-tcpd-gdoc.texi mod-tcp-gdoc.texi mod-udpd-gdoc.texi mod-udp-gdoc.texi msg-gdoc.texi net-gdoc.texi nod-gdoc.texi p2p-gdoc.texi pil-gdoc.texi snd-gdoc.texi srv-gdoc.texi sys-gdoc.texi tsk-gdoc.texi
selfdoc_TEXINFOS = quick-selfdoc.texi doc-selfdoc.texi show-selfdoc.texi path-selfdoc.texi players-selfdoc.texi input-selfdoc.texi graphics-selfdoc.texi sound-selfdoc.texi network-selfdoc.texi map-selfdoc.texi map-rules-selfdoc.texi map-hints-selfdoc.texi map-style-selfdoc.texi funcs-selfdoc.texi hooks-selfdoc.texi advanced-selfdoc.texi aliases-selfdoc.texi
liquidwar6_TEXINFOS = gpl.texi fdl.texi version.texi stamp.texi const.texi $(gdoc_TEXINFOS) $(selfdoc_TEXINFOS)

man6_MANS = liquidwar6.man

EXTRA_DIST = COPYING gdoc.pl gdoc-update.sh selfdoc.pl selfdoc-update.sh

# To generate const.texi we need to first generate const.texi.in
# in builddir then copy it to srcdir for using a texi file located
# in builddir causes an error when running make distcheck
# (complains about write permissions, can't create temp dir).
all-local:
	if ! diff ${builddir}/const.texi.in ${srcdir}/const.texi ; then cp ${builddir}/const.texi.in ${srcdir}/const.texi ; fi

install-data-local:
	if test -d liquidwar6.html ; then install -d $(DESTDIR)$(docdir)/html ; cp -f liquidwar6.html/* $(DESTDIR)$(docdir)/html/ ; fi
	if test -f liquidwar6.pdf ; then install -d $(DESTDIR)$(docdir) ; cp -f liquidwar6.pdf $(DESTDIR)$(docdir)/ ; fi

uninstall-local: 
	rm -r -f $(DESTDIR)$(docdir)

if LW6_GDOC
include Makefile.gdoc
endif

if LW6_SELFDOC
include Makefile.selfdoc
endif

