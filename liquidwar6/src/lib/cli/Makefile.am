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

AM_CFLAGS = @LW6_CFLAGS@ @MOD_HTTP_ENABLE_CFLAGS@
AM_LDFLAGS = @LW6_LDFLAGS@

if MOD_HTTP
MOD_HTTP_SUBDIR = mod-http
MOD_HTTP_LIB_S = mod-http/libmod_http_s.la
else
MOD_HTTP_SUBDIR =
MOD_HTTP_LIB_S =
endif

if ALLINONE
LW6DYN_LIB = 
LW6CLI_LIB_S = mod-tcp/libmod_tcp_s.la mod-udp/libmod_udp_s.la $(MOD_HTTP_LIB_S)
else # ALLINONE
LW6DYN_LIB = ../dyn/libdyn.la 
LW6CLI_LIB_S =
endif # ALLINONE

noinst_LTLIBRARIES = libcli.la
libcli_la_LIBADD = $(LW6CLI_LIB_S)
if LW6_CUNIT
TEST_SRC_FILE = cli-test.c
else # LW6_CUNIT
TEST_SRC_FILE =
endif # LW6_CUNIT
libcli_la_SOURCES = cli-api.c cli-oob.c cli-register.c $(TEST_SRC_FILE)
libclidir= $(includedir)/@PACKAGE_TARNAME@/cli
libcli_HEADERS = cli.h

if LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6cli-test
liquidwar6cli_test_SOURCES = cli-testmain.c
liquidwar6cli_test_LDADD = libcli.la $(LW6DYN_LIB) ../msg/libmsg.la ../cnx/libcnx.la ../nod/libnod.la ../net/libnet.la ../cfg/libcfg.la ../hlp/libhlp.la ../glb/libgl1b.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
TESTS = liquidwar6cli-test
endif # !FULLSTATIC
endif # LW6_CUNIT

SUBDIRS = mod-tcp mod-udp $(MOD_HTTP_SUBDIR)

clean-local:
	rm -f *.log *.trs

