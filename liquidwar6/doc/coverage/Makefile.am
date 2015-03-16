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

opt: index.html

../../src/main.gcno:
	echo Run ./configure --enable-gcov to generate .gcno files

../../src/main.gcda:
	lcov --directory ../../src --zerocounters
	cd ../../src && ./liquidwar6 --test || true

liquidwar6.info: ../../src/main.gcno ../../src/main.gcda
	lcov --directory `readlink -m ${top_builddir}/src/` --capture --base-directory `readlink -m ${top_srcdir}/src/` --output-file liquidwar6.info

index.html: liquidwar6.info
	genhtml --prefix ${top_srcdir} liquidwar6.info

clean-local:
	find ../../src -name "*.gcda" -exec rm -f "{}" \;
	for i in *; do if echo $$i | grep -v Makefile; then rm -rf "../coverage/$$i"; fi; done

