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

opt: liquidwar6-test-cpu.jpeg liquidwar6-test-heap.jpeg opt-extra

liquidwar6-test-cpu.jpeg: liquidwar6-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6-test-heap.jpeg: liquidwar6-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6-test-cpu.pdf: ../../src/lib/liquidwar6-test liquidwar6-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@

liquidwar6-test-heap.pdf: liquidwar6-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/liquidwar6-test liquidwar6-test-heap.dat* > $@

liquidwar6-test-cpu.dat:
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6-test-heap.dat && cd ${top_builddir}/src/lib && $(MAKE) && ./liquidwar6-test

clean-local:
	rm -f *.jpeg liquidwar6*.pdf *.dat *.dat.*

EXTRA_DIST = Makefile.main Makefile.extra blank.pdf

liquidwar6bot-test-cpu.jpeg: liquidwar6bot-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6bot-test-heap.jpeg: liquidwar6bot-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6bot-test-cpu.pdf: ../../src/lib/bot/liquidwar6bot-test liquidwar6bot-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6bot-test-heap.pdf: ../../src/lib/bot/liquidwar6bot-test liquidwar6bot-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/bot/liquidwar6bot-test liquidwar6bot-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6bot-test-cpu.dat: ../../src/lib/bot/liquidwar6bot-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6bot-test-heap.dat && cd ${top_builddir}/src/lib/bot && ./liquidwar6bot-test




liquidwar6cfg-test-cpu.jpeg: liquidwar6cfg-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6cfg-test-heap.jpeg: liquidwar6cfg-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6cfg-test-cpu.pdf: ../../src/lib/cfg/liquidwar6cfg-test liquidwar6cfg-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6cfg-test-heap.pdf: ../../src/lib/cfg/liquidwar6cfg-test liquidwar6cfg-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/cfg/liquidwar6cfg-test liquidwar6cfg-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6cfg-test-cpu.dat: ../../src/lib/cfg/liquidwar6cfg-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6cfg-test-heap.dat && cd ${top_builddir}/src/lib/cfg && ./liquidwar6cfg-test




liquidwar6cli-test-cpu.jpeg: liquidwar6cli-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6cli-test-heap.jpeg: liquidwar6cli-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6cli-test-cpu.pdf: ../../src/lib/cli/liquidwar6cli-test liquidwar6cli-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6cli-test-heap.pdf: ../../src/lib/cli/liquidwar6cli-test liquidwar6cli-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/cli/liquidwar6cli-test liquidwar6cli-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6cli-test-cpu.dat: ../../src/lib/cli/liquidwar6cli-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6cli-test-heap.dat && cd ${top_builddir}/src/lib/cli && ./liquidwar6cli-test




liquidwar6cns-test-cpu.jpeg: liquidwar6cns-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6cns-test-heap.jpeg: liquidwar6cns-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6cns-test-cpu.pdf: ../../src/lib/cns/liquidwar6cns-test liquidwar6cns-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6cns-test-heap.pdf: ../../src/lib/cns/liquidwar6cns-test liquidwar6cns-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/cns/liquidwar6cns-test liquidwar6cns-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6cns-test-cpu.dat: ../../src/lib/cns/liquidwar6cns-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6cns-test-heap.dat && cd ${top_builddir}/src/lib/cns && ./liquidwar6cns-test




liquidwar6cnx-test-cpu.jpeg: liquidwar6cnx-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6cnx-test-heap.jpeg: liquidwar6cnx-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6cnx-test-cpu.pdf: ../../src/lib/cnx/liquidwar6cnx-test liquidwar6cnx-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6cnx-test-heap.pdf: ../../src/lib/cnx/liquidwar6cnx-test liquidwar6cnx-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/cnx/liquidwar6cnx-test liquidwar6cnx-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6cnx-test-cpu.dat: ../../src/lib/cnx/liquidwar6cnx-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6cnx-test-heap.dat && cd ${top_builddir}/src/lib/cnx && ./liquidwar6cnx-test




liquidwar6dat-test-cpu.jpeg: liquidwar6dat-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6dat-test-heap.jpeg: liquidwar6dat-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6dat-test-cpu.pdf: ../../src/lib/dat/liquidwar6dat-test liquidwar6dat-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6dat-test-heap.pdf: ../../src/lib/dat/liquidwar6dat-test liquidwar6dat-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/dat/liquidwar6dat-test liquidwar6dat-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6dat-test-cpu.dat: ../../src/lib/dat/liquidwar6dat-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6dat-test-heap.dat && cd ${top_builddir}/src/lib/dat && ./liquidwar6dat-test




liquidwar6dsp-test-cpu.jpeg: liquidwar6dsp-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6dsp-test-heap.jpeg: liquidwar6dsp-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6dsp-test-cpu.pdf: ../../src/lib/dsp/liquidwar6dsp-test liquidwar6dsp-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6dsp-test-heap.pdf: ../../src/lib/dsp/liquidwar6dsp-test liquidwar6dsp-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/dsp/liquidwar6dsp-test liquidwar6dsp-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6dsp-test-cpu.dat: ../../src/lib/dsp/liquidwar6dsp-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6dsp-test-heap.dat && cd ${top_builddir}/src/lib/dsp && ./liquidwar6dsp-test




liquidwar6dyn-test-cpu.jpeg: liquidwar6dyn-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6dyn-test-heap.jpeg: liquidwar6dyn-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6dyn-test-cpu.pdf: ../../src/lib/dyn/liquidwar6dyn-test liquidwar6dyn-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6dyn-test-heap.pdf: ../../src/lib/dyn/liquidwar6dyn-test liquidwar6dyn-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/dyn/liquidwar6dyn-test liquidwar6dyn-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6dyn-test-cpu.dat: ../../src/lib/dyn/liquidwar6dyn-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6dyn-test-heap.dat && cd ${top_builddir}/src/lib/dyn && ./liquidwar6dyn-test




liquidwar6gen-test-cpu.jpeg: liquidwar6gen-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6gen-test-heap.jpeg: liquidwar6gen-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6gen-test-cpu.pdf: ../../src/lib/gen/liquidwar6gen-test liquidwar6gen-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6gen-test-heap.pdf: ../../src/lib/gen/liquidwar6gen-test liquidwar6gen-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/gen/liquidwar6gen-test liquidwar6gen-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6gen-test-cpu.dat: ../../src/lib/gen/liquidwar6gen-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6gen-test-heap.dat && cd ${top_builddir}/src/lib/gen && ./liquidwar6gen-test




liquidwar6gfx-test-cpu.jpeg: liquidwar6gfx-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6gfx-test-heap.jpeg: liquidwar6gfx-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6gfx-test-cpu.pdf: ../../src/lib/gfx/liquidwar6gfx-test liquidwar6gfx-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6gfx-test-heap.pdf: ../../src/lib/gfx/liquidwar6gfx-test liquidwar6gfx-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/gfx/liquidwar6gfx-test liquidwar6gfx-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6gfx-test-cpu.dat: ../../src/lib/gfx/liquidwar6gfx-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6gfx-test-heap.dat && cd ${top_builddir}/src/lib/gfx && ./liquidwar6gfx-test




liquidwar6glb-test-cpu.jpeg: liquidwar6glb-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6glb-test-heap.jpeg: liquidwar6glb-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6glb-test-cpu.pdf: ../../src/lib/glb/liquidwar6glb-test liquidwar6glb-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6glb-test-heap.pdf: ../../src/lib/glb/liquidwar6glb-test liquidwar6glb-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/glb/liquidwar6glb-test liquidwar6glb-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6glb-test-cpu.dat: ../../src/lib/glb/liquidwar6glb-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6glb-test-heap.dat && cd ${top_builddir}/src/lib/glb && ./liquidwar6glb-test




liquidwar6gui-test-cpu.jpeg: liquidwar6gui-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6gui-test-heap.jpeg: liquidwar6gui-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6gui-test-cpu.pdf: ../../src/lib/gui/liquidwar6gui-test liquidwar6gui-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6gui-test-heap.pdf: ../../src/lib/gui/liquidwar6gui-test liquidwar6gui-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/gui/liquidwar6gui-test liquidwar6gui-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6gui-test-cpu.dat: ../../src/lib/gui/liquidwar6gui-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6gui-test-heap.dat && cd ${top_builddir}/src/lib/gui && ./liquidwar6gui-test




liquidwar6hlp-test-cpu.jpeg: liquidwar6hlp-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6hlp-test-heap.jpeg: liquidwar6hlp-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6hlp-test-cpu.pdf: ../../src/lib/hlp/liquidwar6hlp-test liquidwar6hlp-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6hlp-test-heap.pdf: ../../src/lib/hlp/liquidwar6hlp-test liquidwar6hlp-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/hlp/liquidwar6hlp-test liquidwar6hlp-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6hlp-test-cpu.dat: ../../src/lib/hlp/liquidwar6hlp-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6hlp-test-heap.dat && cd ${top_builddir}/src/lib/hlp && ./liquidwar6hlp-test




liquidwar6img-test-cpu.jpeg: liquidwar6img-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6img-test-heap.jpeg: liquidwar6img-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6img-test-cpu.pdf: ../../src/lib/img/liquidwar6img-test liquidwar6img-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6img-test-heap.pdf: ../../src/lib/img/liquidwar6img-test liquidwar6img-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/img/liquidwar6img-test liquidwar6img-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6img-test-cpu.dat: ../../src/lib/img/liquidwar6img-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6img-test-heap.dat && cd ${top_builddir}/src/lib/img && ./liquidwar6img-test




liquidwar6ker-test-cpu.jpeg: liquidwar6ker-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6ker-test-heap.jpeg: liquidwar6ker-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6ker-test-cpu.pdf: ../../src/lib/ker/liquidwar6ker-test liquidwar6ker-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6ker-test-heap.pdf: ../../src/lib/ker/liquidwar6ker-test liquidwar6ker-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/ker/liquidwar6ker-test liquidwar6ker-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6ker-test-cpu.dat: ../../src/lib/ker/liquidwar6ker-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6ker-test-heap.dat && cd ${top_builddir}/src/lib/ker && ./liquidwar6ker-test




liquidwar6ldr-test-cpu.jpeg: liquidwar6ldr-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6ldr-test-heap.jpeg: liquidwar6ldr-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6ldr-test-cpu.pdf: ../../src/lib/ldr/liquidwar6ldr-test liquidwar6ldr-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6ldr-test-heap.pdf: ../../src/lib/ldr/liquidwar6ldr-test liquidwar6ldr-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/ldr/liquidwar6ldr-test liquidwar6ldr-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6ldr-test-cpu.dat: ../../src/lib/ldr/liquidwar6ldr-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6ldr-test-heap.dat && cd ${top_builddir}/src/lib/ldr && ./liquidwar6ldr-test




liquidwar6map-test-cpu.jpeg: liquidwar6map-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6map-test-heap.jpeg: liquidwar6map-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6map-test-cpu.pdf: ../../src/lib/map/liquidwar6map-test liquidwar6map-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6map-test-heap.pdf: ../../src/lib/map/liquidwar6map-test liquidwar6map-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/map/liquidwar6map-test liquidwar6map-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6map-test-cpu.dat: ../../src/lib/map/liquidwar6map-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6map-test-heap.dat && cd ${top_builddir}/src/lib/map && ./liquidwar6map-test




liquidwar6mat-test-cpu.jpeg: liquidwar6mat-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6mat-test-heap.jpeg: liquidwar6mat-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6mat-test-cpu.pdf: ../../src/lib/mat/liquidwar6mat-test liquidwar6mat-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6mat-test-heap.pdf: ../../src/lib/mat/liquidwar6mat-test liquidwar6mat-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/mat/liquidwar6mat-test liquidwar6mat-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6mat-test-cpu.dat: ../../src/lib/mat/liquidwar6mat-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6mat-test-heap.dat && cd ${top_builddir}/src/lib/mat && ./liquidwar6mat-test




liquidwar6msg-test-cpu.jpeg: liquidwar6msg-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6msg-test-heap.jpeg: liquidwar6msg-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6msg-test-cpu.pdf: ../../src/lib/msg/liquidwar6msg-test liquidwar6msg-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6msg-test-heap.pdf: ../../src/lib/msg/liquidwar6msg-test liquidwar6msg-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/msg/liquidwar6msg-test liquidwar6msg-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6msg-test-cpu.dat: ../../src/lib/msg/liquidwar6msg-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6msg-test-heap.dat && cd ${top_builddir}/src/lib/msg && ./liquidwar6msg-test




liquidwar6net-test-cpu.jpeg: liquidwar6net-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6net-test-heap.jpeg: liquidwar6net-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6net-test-cpu.pdf: ../../src/lib/net/liquidwar6net-test liquidwar6net-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6net-test-heap.pdf: ../../src/lib/net/liquidwar6net-test liquidwar6net-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/net/liquidwar6net-test liquidwar6net-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6net-test-cpu.dat: ../../src/lib/net/liquidwar6net-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6net-test-heap.dat && cd ${top_builddir}/src/lib/net && ./liquidwar6net-test




liquidwar6nod-test-cpu.jpeg: liquidwar6nod-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6nod-test-heap.jpeg: liquidwar6nod-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6nod-test-cpu.pdf: ../../src/lib/nod/liquidwar6nod-test liquidwar6nod-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6nod-test-heap.pdf: ../../src/lib/nod/liquidwar6nod-test liquidwar6nod-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/nod/liquidwar6nod-test liquidwar6nod-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6nod-test-cpu.dat: ../../src/lib/nod/liquidwar6nod-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6nod-test-heap.dat && cd ${top_builddir}/src/lib/nod && ./liquidwar6nod-test




liquidwar6p2p-test-cpu.jpeg: liquidwar6p2p-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6p2p-test-heap.jpeg: liquidwar6p2p-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6p2p-test-cpu.pdf: ../../src/lib/p2p/liquidwar6p2p-test liquidwar6p2p-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6p2p-test-heap.pdf: ../../src/lib/p2p/liquidwar6p2p-test liquidwar6p2p-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/p2p/liquidwar6p2p-test liquidwar6p2p-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6p2p-test-cpu.dat: ../../src/lib/p2p/liquidwar6p2p-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6p2p-test-heap.dat && cd ${top_builddir}/src/lib/p2p && ./liquidwar6p2p-test




liquidwar6pil-test-cpu.jpeg: liquidwar6pil-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6pil-test-heap.jpeg: liquidwar6pil-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6pil-test-cpu.pdf: ../../src/lib/pil/liquidwar6pil-test liquidwar6pil-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6pil-test-heap.pdf: ../../src/lib/pil/liquidwar6pil-test liquidwar6pil-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/pil/liquidwar6pil-test liquidwar6pil-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6pil-test-cpu.dat: ../../src/lib/pil/liquidwar6pil-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6pil-test-heap.dat && cd ${top_builddir}/src/lib/pil && ./liquidwar6pil-test




liquidwar6scm-test-cpu.jpeg: liquidwar6scm-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6scm-test-heap.jpeg: liquidwar6scm-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6scm-test-cpu.pdf: ../../src/lib/scm/liquidwar6scm-test liquidwar6scm-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6scm-test-heap.pdf: ../../src/lib/scm/liquidwar6scm-test liquidwar6scm-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/scm/liquidwar6scm-test liquidwar6scm-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6scm-test-cpu.dat: ../../src/lib/scm/liquidwar6scm-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6scm-test-heap.dat && cd ${top_builddir}/src/lib/scm && ./liquidwar6scm-test




liquidwar6sim-test-cpu.jpeg: liquidwar6sim-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6sim-test-heap.jpeg: liquidwar6sim-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6sim-test-cpu.pdf: ../../src/lib/sim/liquidwar6sim-test liquidwar6sim-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6sim-test-heap.pdf: ../../src/lib/sim/liquidwar6sim-test liquidwar6sim-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/sim/liquidwar6sim-test liquidwar6sim-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6sim-test-cpu.dat: ../../src/lib/sim/liquidwar6sim-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6sim-test-heap.dat && cd ${top_builddir}/src/lib/sim && ./liquidwar6sim-test




liquidwar6snd-test-cpu.jpeg: liquidwar6snd-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6snd-test-heap.jpeg: liquidwar6snd-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6snd-test-cpu.pdf: ../../src/lib/snd/liquidwar6snd-test liquidwar6snd-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6snd-test-heap.pdf: ../../src/lib/snd/liquidwar6snd-test liquidwar6snd-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/snd/liquidwar6snd-test liquidwar6snd-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6snd-test-cpu.dat: ../../src/lib/snd/liquidwar6snd-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6snd-test-heap.dat && cd ${top_builddir}/src/lib/snd && ./liquidwar6snd-test




liquidwar6srv-test-cpu.jpeg: liquidwar6srv-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6srv-test-heap.jpeg: liquidwar6srv-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6srv-test-cpu.pdf: ../../src/lib/srv/liquidwar6srv-test liquidwar6srv-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6srv-test-heap.pdf: ../../src/lib/srv/liquidwar6srv-test liquidwar6srv-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/srv/liquidwar6srv-test liquidwar6srv-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6srv-test-cpu.dat: ../../src/lib/srv/liquidwar6srv-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6srv-test-heap.dat && cd ${top_builddir}/src/lib/srv && ./liquidwar6srv-test




liquidwar6sys-test-cpu.jpeg: liquidwar6sys-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6sys-test-heap.jpeg: liquidwar6sys-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6sys-test-cpu.pdf: ../../src/lib/sys/liquidwar6sys-test liquidwar6sys-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6sys-test-heap.pdf: ../../src/lib/sys/liquidwar6sys-test liquidwar6sys-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/sys/liquidwar6sys-test liquidwar6sys-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6sys-test-cpu.dat: ../../src/lib/sys/liquidwar6sys-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6sys-test-heap.dat && cd ${top_builddir}/src/lib/sys && ./liquidwar6sys-test




liquidwar6tsk-test-cpu.jpeg: liquidwar6tsk-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6tsk-test-heap.jpeg: liquidwar6tsk-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6tsk-test-cpu.pdf: ../../src/lib/tsk/liquidwar6tsk-test liquidwar6tsk-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6tsk-test-heap.pdf: ../../src/lib/tsk/liquidwar6tsk-test liquidwar6tsk-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/tsk/liquidwar6tsk-test liquidwar6tsk-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6tsk-test-cpu.dat: ../../src/lib/tsk/liquidwar6tsk-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6tsk-test-heap.dat && cd ${top_builddir}/src/lib/tsk && ./liquidwar6tsk-test




liquidwar6vox-test-cpu.jpeg: liquidwar6vox-test-cpu.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6vox-test-heap.jpeg: liquidwar6vox-test-heap.pdf
	mogrify -format jpeg -density 600x600 $^

liquidwar6vox-test-cpu.pdf: ../../src/lib/vox/liquidwar6vox-test liquidwar6vox-test-cpu.dat
	google-pprof --pdf --focus=lw6 $^ > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6vox-test-heap.pdf: ../../src/lib/vox/liquidwar6vox-test liquidwar6vox-test-cpu.dat
	google-pprof --pdf --focus=lw6 ../../src/lib/vox/liquidwar6vox-test liquidwar6vox-test-heap.dat* > $@ || cat $(srcdir)/blank.pdf > $@

liquidwar6vox-test-cpu.dat: ../../src/lib/vox/liquidwar6vox-test
	export CPUPROFILE=$$(readlink -f $@) && export HEAPPROFILE=$$(dirname $$(readlink -f $@))/liquidwar6vox-test-heap.dat && cd ${top_builddir}/src/lib/vox && ./liquidwar6vox-test




opt-extra: liquidwar6bot-test-cpu.jpeg liquidwar6bot-test-heap.jpeg liquidwar6cfg-test-cpu.jpeg liquidwar6cfg-test-heap.jpeg liquidwar6cli-test-cpu.jpeg liquidwar6cli-test-heap.jpeg liquidwar6cns-test-cpu.jpeg liquidwar6cns-test-heap.jpeg liquidwar6cnx-test-cpu.jpeg liquidwar6cnx-test-heap.jpeg liquidwar6dat-test-cpu.jpeg liquidwar6dat-test-heap.jpeg liquidwar6dsp-test-cpu.jpeg liquidwar6dsp-test-heap.jpeg liquidwar6dyn-test-cpu.jpeg liquidwar6dyn-test-heap.jpeg liquidwar6gen-test-cpu.jpeg liquidwar6gen-test-heap.jpeg liquidwar6gfx-test-cpu.jpeg liquidwar6gfx-test-heap.jpeg liquidwar6glb-test-cpu.jpeg liquidwar6glb-test-heap.jpeg liquidwar6gui-test-cpu.jpeg liquidwar6gui-test-heap.jpeg liquidwar6hlp-test-cpu.jpeg liquidwar6hlp-test-heap.jpeg liquidwar6img-test-cpu.jpeg liquidwar6img-test-heap.jpeg liquidwar6ker-test-cpu.jpeg liquidwar6ker-test-heap.jpeg liquidwar6ldr-test-cpu.jpeg liquidwar6ldr-test-heap.jpeg liquidwar6map-test-cpu.jpeg liquidwar6map-test-heap.jpeg liquidwar6mat-test-cpu.jpeg liquidwar6mat-test-heap.jpeg liquidwar6msg-test-cpu.jpeg liquidwar6msg-test-heap.jpeg liquidwar6net-test-cpu.jpeg liquidwar6net-test-heap.jpeg liquidwar6nod-test-cpu.jpeg liquidwar6nod-test-heap.jpeg liquidwar6p2p-test-cpu.jpeg liquidwar6p2p-test-heap.jpeg liquidwar6pil-test-cpu.jpeg liquidwar6pil-test-heap.jpeg liquidwar6scm-test-cpu.jpeg liquidwar6scm-test-heap.jpeg liquidwar6sim-test-cpu.jpeg liquidwar6sim-test-heap.jpeg liquidwar6snd-test-cpu.jpeg liquidwar6snd-test-heap.jpeg liquidwar6srv-test-cpu.jpeg liquidwar6srv-test-heap.jpeg liquidwar6sys-test-cpu.jpeg liquidwar6sys-test-heap.jpeg liquidwar6tsk-test-cpu.jpeg liquidwar6tsk-test-heap.jpeg liquidwar6vox-test-cpu.jpeg liquidwar6vox-test-heap.jpeg
