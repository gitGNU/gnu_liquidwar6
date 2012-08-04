# Liquid War 6 is a unique multiplayer wargame.
# Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>
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

EXTRA_DIST = texinfo-struct.xsl

opt: html/index.html

if LW6_DOXYGEN
all: opt
endif

# dependency on doxygen.cfg.in and not doxygen.cfg to avoid too many builds
html/index.html: doxygen.cfg.in ${top_srcdir}/doc/stamp.texi ${top_srcdir}/src/lib/bot/bot.h ${top_srcdir}/src/lib/cfg/cfg.h ${top_srcdir}/src/lib/cli/cli.h ${top_srcdir}/src/lib/cns/cns.h ${top_srcdir}/src/lib/cnx/cnx.h ${top_srcdir}/src/lib/dat/dat.h ${top_srcdir}/src/lib/dsp/dsp.h ${top_srcdir}/src/lib/dyn/dyn.h ${top_srcdir}/src/lib/gfx/gfx.h ${top_srcdir}/src/lib/glb/glb.h ${top_srcdir}/src/lib/gui/gui.h ${top_srcdir}/src/lib/hlp/hlp.h ${top_srcdir}/src/lib/img/img.h ${top_srcdir}/src/lib/ker/ker.h ${top_srcdir}/src/lib/ldr/ldr.h ${top_srcdir}/src/lib/map/map.h ${top_srcdir}/src/lib/msg/msg.h ${top_srcdir}/src/lib/net/net.h ${top_srcdir}/src/lib/nod/nod.h ${top_srcdir}/src/lib/p2p/p2p.h ${top_srcdir}/src/lib/pil/pil.h ${top_srcdir}/src/lib/scm/scm.h ${top_srcdir}/src/lib/sim/sim.h ${top_srcdir}/src/lib/snd/snd.h ${top_srcdir}/src/lib/srv/srv.h ${top_srcdir}/src/lib/sys/sys.h ${top_srcdir}/src/lib/tsk/tsk.h ${top_srcdir}/src/lib/vox/vox.h
	export DOXYGEN_INPUT=${top_srcdir}/src ; if test -f doxygen.cfg ; then doxygen doxygen.cfg ; fi

clean-local:
	rm -rf html xml

