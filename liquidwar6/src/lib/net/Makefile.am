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

noinst_LTLIBRARIES = libnet.la
if LW6_CUNIT
TEST_SRC_FILE = net-test.c
else # LW6_CUNIT
TEST_SRC_FILE =
endif # LW6_CUNIT
libnet_la_SOURCES = net-address.c  net-connectable.c net-const.c net-counters.c net-dns.c net-error.c net-if.c net-line.c net-log.c net-setup.c net-socket.c net-tcp.c $(TEST_SRC_FILE) net-udp.c
libnetdir= $(includedir)/@PACKAGE_TARNAME@/net
libnet_HEADERS = net.h

if LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6net-test
liquidwar6net_test_SOURCES = net-testmain.c
liquidwar6net_test_LDADD = libnet.la ../cfg/libcfg.la ../hlp/libhlp.la ../glb/libgl1b.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
TESTS = liquidwar6net-test
endif # !FULLSTATIC
endif # LW6_CUNIT

EXTRA_DIST = net-internal.h

clean-local:
	rm -f *.log *.trs

