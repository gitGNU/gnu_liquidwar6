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
LW6BOT_LIB_S = mod-brute/libmod_brute_s.la mod-follow/libmod_follow_s.la mod-idiot/libmod_idiot_s.la mod-random/libmod_random_s.la
else #ALLINONE
LW6DYN_LIB = ../dyn/libdyn.la 
LW6BOT_LIB_S =
endif #ALLINONE

noinst_LTLIBRARIES = libbot.la
libbot_la_LIBADD = $(LW6BOT_LIB_S)
if LW6_CUNIT
TEST_SRC_FILE = bot-test.c
else # LW6_CUNIT
TEST_SRC_FILE =
endif # LW6_CUNIT
libbot_la_SOURCES = bot-api.c bot-register.c $(TEST_SRC_FILE)
libbotdir= $(includedir)/@PACKAGE_TARNAME@/bot
libbot_HEADERS = bot.h

if LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6bot-test
liquidwar6bot_test_SOURCES = bot-testmain.c
liquidwar6bot_test_LDADD = libbot.la $(LW6DYN_LIB) ../pil/libpil.la ../ker/libker.la ../map/libmap.la ../sys/libsys.la @LW6_LIBINTL_LDADD@
TESTS = liquidwar6bot-test
endif # !FULLSTATIC
endif # LW6_CUNIT

SUBDIRS = mod-brute mod-follow mod-idiot mod-random

clean-local:
	rm -f *.log *.trs

