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
AM_LDFLAGS = @LW6_LDFLAGS@ -lsqlite3

if ALLINONE
LW6DYN_LIB = 
else # ALLINONE
LW6DYN_LIB = ../dyn/libdyn.la 
endif # ALLINONE

noinst_LTLIBRARIES = libp2p.la
if LW6_CUNIT
TEST_SRC_FILE = p2p-test.c
else # LW6_CUNIT
TEST_SRC_FILE =
endif  # LW6_CUNIT
libp2p_la_SOURCES = p2p-backends.c p2p-clioob.c p2p-connectregistered.c p2p-data.c p2p-db.c p2p-entry.c p2p-explore.c p2p-flush.c p2p-node.c p2p-peeridlist.c p2p-recv.c p2p-srvoob.c p2p-tentacle.c $(TEST_SRC_FILE)
libp2pdir= $(includedir)/@PACKAGE_TARNAME@/p2p
libp2p_HEADERS = p2p.h

if LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6p2p-test
liquidwar6p2p_test_SOURCES = p2p-testmain.c
liquidwar6p2p_test_LDADD = libp2p.la ../dat/libdat.la ../cli/libcli.la ../srv/libsrv.la ../msg/libmsg.la ../cnx/libcnx.la ../nod/libnod.la ../net/libnet.la $(LW6DYN_LIB) ../cfg/libcfg.la ../hlp/libhlp.la ../glb/libgl1b.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
TESTS = liquidwar6p2p-test
endif # !FULLSTATIC
endif # LW6_CUNIT

EXTRA_DIST = p2p-internal.h

clean-local:
	rm -f *.log *.trs

