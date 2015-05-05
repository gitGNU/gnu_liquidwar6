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

if ALLINONE
LW6DYN_LIB = 
LW6SRV_LIB_S = mod-tcpd/libmod_tcpd_s.la mod-udpd/libmod_udpd_s.la mod-httpd/libmod_httpd_s.la
else # ALLINONE
LW6DYN_LIB = ../dyn/libdyn.la 
LW6SRV_LIB_S =
endif # ALLINONE

noinst_LTLIBRARIES = libsrv.la
libsrv_la_LIBADD = $(LW6SRV_LIB_S)
if LW6_CUNIT
TEST_SRC_FILE = srv-test.c
else # LW6_CUNIT
TEST_SRC_FILE =
endif # LW6_CUNIT
libsrv_la_SOURCES = srv-api.c srv-control.c srv-oob.c srv-register.c srv-tcpaccepter.c $(TEST_SRC_FILE) srv-udpbuffer.c
libsrvdir= $(includedir)/@PACKAGE_TARNAME@/srv
libsrv_HEADERS = srv.h

if LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6srv-test
liquidwar6srv_test_SOURCES = srv-testmain.c
liquidwar6srv_test_LDADD = libsrv.la $(LW6DYN_LIB) ../msg/libmsg.la ../cnx/libcnx.la ../nod/libnod.la ../net/libnet.la ../cfg/libcfg.la ../hlp/libhlp.la ../glb/libgl1b.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
TESTS = liquidwar6srv-test
endif # !FULLSTATIC
endif # LW6_CUNIT

SUBDIRS = mod-tcpd mod-udpd mod-httpd

clean-local:
	rm -f *.log *.trs

