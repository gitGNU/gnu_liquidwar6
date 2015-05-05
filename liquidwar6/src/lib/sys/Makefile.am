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

AM_CFLAGS = @LW6_CFLAGS@ @LW6_CONSOLE_ENABLE_CFLAGS@ @LW6_GTK_ENABLE_CFLAGS@ @MOD_GL1_ENABLE_CFLAGS@ @MOD_GLES2_ENABLE_CFLAGS@ @MOD_SOFT_ENABLE_CFLAGS@ @MOD_CACA_ENABLE_CFLAGS@ @MOD_CSOUND_ENABLE_CFLAGS@ @MOD_OGG_ENABLE_CFLAGS@ @MOD_HTTP_ENABLE_CFLAGS@ @LW6_ABS_SRCDIR_CFLAGS@ -DLW6_TOP_SRCDIR=\"$(top_srcdir)\" -DLW6_PREFIX=\"$(prefix)\" -DLW6_DATADIR=\"$(datadir)\" -DLW6_LIBDIR=\"$(libdir)\" -DLW6_INCLUDEDIR=\"$(includedir)\" -DLW6_LOCALEDIR=\"$(localedir)\" -DLW6_DOCDIR=\"$(docdir)\"
AM_OBJCFLAGS = $(AM_CFLAGS)
AM_LDFLAGS = @LW6_LDFLAGS@ @LW6_GTK_ENABLE_LDFLAGS@ -lm

if LW6_X86
ASM_SRC_FILES = sys-testandset.c sys-testandsetx86.s
ASM_EXTRA_FILES =
else # LW6_X86
if LW6_AMD64
ASM_SRC_FILES = sys-testandset.c sys-testandsetamd64.s
ASM_EXTRA_FILES =
else # LW6_AMD64
ASM_SRC_FILES =
ASM_EXTRA_FILES = 
endif # LW6_AMD64
endif # LW6_X86

if LW6_MAC_OS_X
OBJC_SRC_FILES = sys-macosx.m
OBJC_EXTRA_FILES =
else # LW6_MAC_OS_X
OBJC_SRC_FILES =
OBJC_EXTRA_FILES = sys-macosx.m
endif # LW6_MAC_OS_X

noinst_LTLIBRARIES = libsys.la
if LW6_CUNIT
TEST_SRC_FILE = sys-cunit.c sys-test.c
else # LW6_CUNIT
TEST_SRC_FILE =
endif # LW6_CUNIT
libsys_la_SOURCES = sys-arg.c sys-assoc.c sys-backtrace.c sys-bazooka.c sys-build.c sys-cache.c sys-callerinfo.c sys-checksum.c sys-color.c sys-context.c sys-convert.c sys-daemon.c sys-debug.c sys-dump.c sys-env.c sys-escape.c sys-exec.c sys-file.c sys-profiler.c sys-hash.c sys-hexa.c sys-history.c sys-i18n.c sys-id.c sys-keyword.c sys-list.c sys-listr.c sys-log.c sys-math.c sys-mem.c sys-mutex.c sys-nop.c sys-openmp.c sys-options.c sys-path.c sys-print.c sys-process.c sys-progress.c sys-random.c sys-sdl.c sys-serial.c sys-shape.c sys-signal.c sys-sort.c sys-spinlock.c sys-str.c sys-stream.c $(TEST_SRC_FILE) sys-thread.c sys-time.c sys-url.c sys-version.c sys-vthread.c $(ASM_SRC_FILES) $(OBJC_SRC_FILES)
libsysdir= $(includedir)/@PACKAGE_TARNAME@/sys
libsys_HEADERS = sys.h sys-gettext.h

if LW6_CUNIT
if !FULLSTATIC
libexec_PROGRAMS = liquidwar6sys-test
liquidwar6sys_test_SOURCES = sys-testmain.c
liquidwar6sys_test_LDADD = libsys.la @LW6_LIBINTL_LDADD@
TESTS = liquidwar6sys-test
endif # !FULLSTATIC
endif # LW6_CUNIT

EXTRA_DIST = sys-internal.h sys-build-stamp.h sys-build-md5sum.h $(ASM_EXTRA_FILES) $(OBJC_EXTRA_FILES)

clean-local:
	rm -f *.log *.trs

