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

SUBDIRS = coverage global cyclo doxygen perf manual example dtd

info_TEXINFOS = liquidwar6.texi
gdoc_TEXINFOS = liquidwar6-gdoc.texi bot-gdoc.texi cfg-gdoc.texi cli-gdoc.texi cns-gdoc.texi cnx-gdoc.texi dat-gdoc.texi dsp-gdoc.texi dyn-gdoc.texi fdl.texi gen-gdoc.texi gfx-gdoc.texi glb-gdoc.texi gpl.texi gui-gdoc.texi hlp-gdoc.texi img-gdoc.texi ker-gdoc.texi ldr-gdoc.texi liquidwar6-gdoc.texi map-gdoc.texi mat-gdoc.texi mod-brute-gdoc.texi mod-caca-gdoc.texi mod-csound-gdoc.texi mod-follow-gdoc.texi mod-gl1-gdoc.texi mod-gles2-gdoc.texi mod-httpd-gdoc.texi mod-http-gdoc.texi mod-idiot-gdoc.texi mod-ogg-gdoc.texi mod-random-gdoc.texi mod-soft-gdoc.texi mod-tcpd-gdoc.texi mod-tcp-gdoc.texi mod-udpd-gdoc.texi mod-udp-gdoc.texi msg-gdoc.texi net-gdoc.texi nod-gdoc.texi p2p-gdoc.texi pil-gdoc.texi scm-gdoc.texi shared-sdl-gdoc.texi sim-gdoc.texi snd-gdoc.texi srv-gdoc.texi sys-gdoc.texi tsk-gdoc.texi vox-gdoc.texi
doxygen_TEXINFOS = liquidwar6-doxygen.texi bot-doxygen.texi cfg-doxygen.texi cli-doxygen.texi cns-doxygen.texi cnx-doxygen.texi dat-doxygen.texi dsp-doxygen.texi dyn-doxygen.texi fdl.texi gen-doxygen.texi gfx-doxygen.texi glb-doxygen.texi gpl.texi gui-doxygen.texi hlp-doxygen.texi img-doxygen.texi ker-doxygen.texi ldr-doxygen.texi liquidwar6-doxygen.texi map-doxygen.texi mat-doxygen.texi mod-brute-doxygen.texi mod-caca-doxygen.texi mod-csound-doxygen.texi mod-follow-doxygen.texi mod-gl1-doxygen.texi mod-gles2-doxygen.texi mod-httpd-doxygen.texi mod-http-doxygen.texi mod-idiot-doxygen.texi mod-ogg-doxygen.texi mod-random-doxygen.texi mod-soft-doxygen.texi mod-tcpd-doxygen.texi mod-tcp-doxygen.texi mod-udpd-doxygen.texi mod-udp-doxygen.texi msg-doxygen.texi net-doxygen.texi nod-doxygen.texi p2p-doxygen.texi pil-doxygen.texi scm-doxygen.texi shared-sdl-doxygen.texi sim-doxygen.texi snd-doxygen.texi srv-doxygen.texi sys-doxygen.texi tsk-doxygen.texi vox-doxygen.texi
selfdoc_TEXINFOS = quick-selfdoc.texi doc-selfdoc.texi show-selfdoc.texi path-selfdoc.texi players-selfdoc.texi input-selfdoc.texi graphics-selfdoc.texi sound-selfdoc.texi network-selfdoc.texi map-selfdoc.texi map-rules-selfdoc.texi map-hints-selfdoc.texi map-style-selfdoc.texi map-teams-selfdoc.texi funcs-selfdoc.texi hooks-selfdoc.texi advanced-selfdoc.texi aliases-selfdoc.texi team-colors-selfdoc.texi weapons-selfdoc.texi
liquidwar6_TEXINFOS = gpl.texi fdl.texi version.texi const.texi $(gdoc_TEXINFOS) $(doxygen_TEXINFOS) $(selfdoc_TEXINFOS)

man6_MANS = liquidwar6.man

EXTRA_DIST = COPYING gdoc.pl gdoc-update.sh selfdoc.pl selfdoc-update.sh doxygen-update.sh perf-update.sh deps.dot deps.eps deps.txt mod-gl1-layout.xcf mod-gl1-layout.ods

deps.eps: deps.dot
	if which dot ; then dot -Kdot -Teps -o$@ $< ; fi ; if which rsync ; then rsync $@ ${srcdir} ; fi

deps.txt: deps.dot
	if which dot ; then dot -Tcanon -o$@ $< ; fi ; if which rsync ; then rsync $@ ${srcdir} ; fi

deps.png: deps.dot
	if which dot ; then dot -Kdot -Tpng -o$@ $< ; fi

deps.pdf: deps.dot
	if which dot ; then dot -Kdot -Tpdf -o$@ $< ; fi

# To generate const.texi we need to first generate const.texi.in
# in builddir then copy it to srcdir for using a texi file located
# in builddir causes an error when running make distcheck
# (complains about write permissions, can't create temp dir).
all-local: deps.eps deps.txt
	if ! diff ${builddir}/const.texi.in ${srcdir}/const.texi ; then cp ${builddir}/const.texi.in ${srcdir}/const.texi ; fi

html-local:
	if test -d ${builddir}/liquidwar6.html ; then cp -f ${builddir}/*.png ${builddir}/liquidwar6.html/ ; fi

liquidwar6.html: deps.png

liquidwar6.pdf: deps.pdf

targz: @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz

@PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz:
	echo "Run make dist in parent directory and/or copy @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz here."

pub: coverage-pub global-pub cyclo-pub doxygen-pub perf-pub manual-pub
	echo "Generated pub doc (coverage.tar.gz global.tar.gz cyclo.tar.gz doxygen.tar.gz perf.tar.gz manual.tar.gz)"

coverage-pub: coverage.tar.gz

coverage.tar.gz: @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz
	rm -rf coverage-tmp && mkdir coverage-tmp && cd coverage-tmp && tar -xzf ../@PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz && cd @PACKAGE_TARNAME@-@PACKAGE_VERSION@ && pwd > pwd.txt && mkdir build && cd build && $$(cat ../pwd.txt)/configure --enable-gcov --enable-allinone && rm -f ../pwd.txt && $(MAKE) -C src && cd doc/coverage && $(MAKE) opt && rm -f Makefile && cd .. && tar -czf coverage.tar.gz coverage && cd ../../../.. && mv coverage-tmp/@PACKAGE_TARNAME@-@PACKAGE_VERSION@/build/doc/coverage.tar.gz . && rm -rf coverage-tmp

global-pub: global.tar.gz

global.tar.gz: @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz
	rm -rf global-tmp && mkdir global-tmp && cd global-tmp && tar -xzf ../@PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz && cd @PACKAGE_TARNAME@-@PACKAGE_VERSION@ && ./configure && cd doc/global && $(MAKE) opt && mv HTML global && tar -czf global.tar.gz global && cd ../../../.. && mv global-tmp/@PACKAGE_TARNAME@-@PACKAGE_VERSION@/doc/global/global.tar.gz . && rm -rf global-tmp

cyclo-pub: cyclo.tar.gz

cyclo.tar.gz: @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz
	rm -rf cyclo-tmp && mkdir cyclo-tmp && cd cyclo-tmp && tar -xzf ../@PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz && cd @PACKAGE_TARNAME@-@PACKAGE_VERSION@ && ./configure && cd doc/cyclo && $(MAKE) opt && mkdir cyclo && mv index.html cyclo && tar -czf cyclo.tar.gz cyclo && cd ../../../.. && mv cyclo-tmp/@PACKAGE_TARNAME@-@PACKAGE_VERSION@/doc/cyclo/cyclo.tar.gz . && rm -rf cyclo-tmp

doxygen-pub: doxygen.tar.gz

doxygen.tar.gz: @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz
	rm -rf doxygen-tmp && mkdir doxygen-tmp && cd doxygen-tmp && tar -xzf ../@PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz && cd @PACKAGE_TARNAME@-@PACKAGE_VERSION@ && ./configure && cd doc/doxygen && $(MAKE) opt && mv html doxygen && tar -czf doxygen.tar.gz doxygen && cd ../../../.. && mv doxygen-tmp/@PACKAGE_TARNAME@-@PACKAGE_VERSION@/doc/doxygen/doxygen.tar.gz . && rm -rf doxygen-tmp

perf-pub: perf.tar.gz

perf.tar.gz: @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz
	rm -rf perf-tmp && mkdir perf-tmp && cd perf-tmp && tar -xzf ../@PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz && cd @PACKAGE_TARNAME@-@PACKAGE_VERSION@ && ./configure --enable-profiler && $(MAKE) -C src && cd doc/perf && $(MAKE) opt && install -d perf && cp *.pdf *.jpeg perf && tar -czf perf.tar.gz perf && cd ../../../.. && mv perf-tmp/@PACKAGE_TARNAME@-@PACKAGE_VERSION@/doc/perf/perf.tar.gz . && rm -rf perf-tmp

manual-pub: manual.tar.gz

manual.tar.gz: @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz
	rm -rf manual-tmp && mkdir manual-tmp && cd manual-tmp && tar -xzf ../@PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz && cd @PACKAGE_TARNAME@-@PACKAGE_VERSION@ && ./configure && cd doc/manual && $(MAKE) opt && tar -czf manual.tar.gz manual && cd ../../../.. && mv manual-tmp/@PACKAGE_TARNAME@-@PACKAGE_VERSION@/doc/manual/manual.tar.gz . && rm -rf manual-tmp

@PACKAGE_TARNAME@-savannah-cvs.tar.gz: @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz
	rm -rf @PACKAGE_TARNAME@-@PACKAGE_VERSION@ && tar -xzf @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz && cd @PACKAGE_TARNAME@-@PACKAGE_VERSION@.tar.gz && ./configure && cd doc/

clean-local:
	rm -f *-doxygen.texi
	rm -f deps.png deps.pdf
	rm -f coverage.tar.gz global.tar.gz cyclo.tar.gz doxygen.tar.gz perf.tar.gz manual.tar.gz
	rm -rf coverage-tmp global-tmp cyclo-tmp doxygen-tmp perf-tmp manual-tmp
	rm -rf liquidwar6.t??

install-data-local:
	if test -d liquidwar6.html ; then install -d $(DESTDIR)$(docdir)/html ; cp -f liquidwar6.html/* $(DESTDIR)$(docdir)/html/ ; fi
	if test -f liquidwar6.pdf ; then install -d $(DESTDIR)$(docdir) ; cp -f liquidwar6.pdf $(DESTDIR)$(docdir)/ ; fi

uninstall-local: 
	rm -r -f $(DESTDIR)$(docdir)

if LW6_GDOC
include Makefile.gdoc
endif # LW6_GDOC

if LW6_SELFDOC
include Makefile.selfdoc
endif # LW6_SELFDOC

if LW6_DOXYGEN
include Makefile.doxygen
else # LW6_DOXYGEN
# Yes, % is not standard and a GNU extension but this rule is a quick by-default
# workarround to avoid breaking build when doxygen is not here. For max compat,
# just install doxygen http://www.stack.nl/~dimitri/doxygen/
%-doxygen.texi:
	touch $@
endif # LW6_DOXYGEN

