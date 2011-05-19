# Liquid War 6 is a unique multiplayer wargame.
# Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>
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

AC_INIT([Liquid War 6],[0.0.10beta],[ufoot@ufoot.org],[liquidwar6])
# latest versions of automake accept a 5th arg mapped to PACKAGE_URL
# AC_INIT([Liquid War 6],[0.0.8beta],[ufoot@ufoot.org],[liquidwar6],[http://www.gnu.org/software/liquidwar6/])
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE
AM_CONFIG_HEADER([config.h]) 
AC_PROG_CC
AC_PROG_OBJC
AM_PROG_AS
AC_USE_SYSTEM_EXTENSIONS
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.17])
AM_ICONV

LW6_HKEY=Liquid_War_6
LW6_MS_WINDOWS_VERSION="0, 0, 10"
AC_SUBST(LW6_HKEY)
AC_SUBST(LW6_MS_WINDOWS_VERSION)

dnl Headers, some are optional
AC_CHECK_HEADERS(arpa/inet.h ctype.h dirent.h errno.h expat.h fcntl.h GL/gl.h GL/glu.h iconv.h ifaddrs.h jpeglib.h langinfo.h libguile.h libintl.h libpng12/png.h libpng14/png.h lmcons.h locale.h ltdl.h math.h netdb.h netinet/in.h pthread.h readline/history.h readline/readline.h SDL/SDL.h SDL/SDL_image.h SDL/SDL_mixer.h SDL/SDL_ttf.h signal.h stdarg.h stdio.h stdlib.h string.h syslog.h sys/select.h sys/signal.h sys/socket.h sys/stat.h sys/time.h sys/types.h sys/utsname.h time.h unistd.h winbase.h windows.h winsock2.h)

AM_MISSING_PROG(PERL, perl, ${missing_dir})
AM_MISSING_PROG(AWK, awk, ${missing_dir})
AM_MISSING_PROG(SED, sed, ${missing_dir})

dnl For gdoc
lw6_gdoc=no
if which ${PERL}; then
   lw6_gdoc=yes
fi
AM_CONDITIONAL(LW6_GDOC, test x${lw6_gdoc} = xyes)

lw6_selfdoc=no
if test "${build_cpu}" = "${host_cpu}" && test "${build_os}" = "${host_os}" ; then
   lw6_selfdoc=yes
else
   AC_MSG_WARN(["Cross-compiling, selfdoc is disabled"])
fi
AM_CONDITIONAL(LW6_SELFDOC, test x${lw6_selfdoc} = xyes)

dnl To calculate build checksum
AC_CHECK_PROG(MD5SUM, md5sum, yes, no)
if test x${MD5SUM} != xyes; then
   AC_MSG_WARN([
*** Liquid War 6 recommends md5sum (http://www.gnu.org/software/coreutils/)
])
fi

dnl Target conditionals
lw6_ms_windows=no
if echo "${host_os}" | grep "mingw32" ; then
   lw6_ms_windows=yes
fi
AM_CONDITIONAL(LW6_MS_WINDOWS, test x${lw6_ms_windows} = xyes)

lw6_mac_os_x=no
if echo "${host_os}" | grep "arwin" ; then
   lw6_mac_os_x=yes
fi
AM_CONDITIONAL(LW6_MAC_OS_X, test x${lw6_mac_os_x} = xyes)

lw6_gp2x=no
if echo "${host_os}" | grep "arm-open2x" ; then
   lw6_gp2x=yes
fi
AM_CONDITIONAL(LW6_GP2X, test x${lw6_gp2x} = xyes)

lw6_x86=no
if test ${host_cpu} = i386 || test ${host_cpu} = i486 || test ${host_cpu} = i586 || test ${host_cpu} = i686 ; then
   lw6_x86=yes
fi
AM_CONDITIONAL(LW6_X86, test x${lw6_x86} = xyes)
LW6_X86_CFLAGS=
if test x${lw6_x86} = xyes ; then
   LW6_X86_CFLAGS="-DLW6_X86=1"
fi

dnl For debugging and profiling
AC_CHECK_PROG(GPROF, gprof, yes, no)
if test x${GPROF} != xyes; then
   AC_MSG_NOTICE([Liquid War 6 suggests gprof (http://www.gnu.org/software/binutils/)])
fi
AC_CHECK_PROG(PPROF, google-pprof, yes, no)
   if test x${PPROF} == no; then
   AC_CHECK_PROG(PPROF, pprof, yes, no)
   if test x${PPROF} == no; then
      AC_MSG_NOTICE([Liquid War 6 suggests pprof (http://code.google.com/p/google-perftools/)])
   fi
fi
AC_CHECK_PROG(GCOV, gcov, yes, no)
if test x${GCOV} != xyes; then
   AC_MSG_NOTICE([Liquid War 6 suggests gcov (http://gcc.gnu.org/onlinedocs/gcc/Gcov.html)])
fi
AC_CHECK_PROG(LCOV, lcov, yes, no)
if test x${LCOV} != xyes; then
   AC_MSG_NOTICE([Liquid War 6 suggests lcov (http://ltp.sourceforge.net/coverage/lcov.php)])
fi
AC_CHECK_PROG(VALGRIND, valgrind, yes, no)
if test x${VALGRIND} != xyes; then
   AC_MSG_NOTICE([Liquid War 6 suggests Valgrind (http://valgrind.org/)])
fi

dnl Check for libdl
AC_CHECK_LIB(dl, dlopen, HAVE_LIBDL=1, AC_MSG_NOTICE([
no libdl
]))

dnl Required libraries
AC_CHECK_HEADER(gmp.h, HAVE_GMP_H=1, AC_MSG_ERROR([
*** Liquid War 6 needs GNU MP (http://gmplib.org/)
]))

AC_CHECK_HEADER(libguile.h, HAVE_LIBGUILE_H=1, AC_MSG_ERROR([
*** Liquid War 6 needs Guile 1.8 (http://www.gnu.org/software/guile/)
]))

GUILE_CFLAGS=""
GUILE_LDFLAGS=""
if test "x${lw6_ms_windows}" = "xyes" ; then
   if test -f "/usr/local/lib/libguile.la" ; then
      AC_MSG_NOTICE([Found libguile in /usr/local/lib/libguile.la])
      GUILE_CFLAGS=`guile-config compile`
      GUILE_LDFLAGS=`guile-config link`" /usr/local/lib/libguile.la"
   else
      # http://sourceforge.net/project/shownotes.php?release_id=540763
      AC_MSG_WARN([Couldn't find libguile in /usr/local/lib/libguile.la])
   fi
fi
if test "x${lw6_ms_windows}" != "xyes" && test "x${lw6_gp2x}" != "xyes" ; then
   AC_CHECK_PROG(GUILE, guile, yes, no)
   if test x${GUILE} != xyes; then
      AC_MSG_ERROR([
*** Liquid War 6 needs Guile 1.8 (http://www.gnu.org/software/guile/)
])
   fi
   AC_CHECK_PROG(GUILE_CONFIG, guile-config, yes, no)
   if test x${GUILE_CONFIG} != xyes; then
      AC_MSG_ERROR([
*** Liquid War 6 needs Guile 1.8 (http://www.gnu.org/software/guile/)
])
   fi
   if guile-config --version ; then
      GUILE_CFLAGS=`guile-config compile`
      GUILE_LDFLAGS=`guile-config link`
      AC_CHECK_LIB(guile, scm_with_guile, HAVE_LIBGUILE=1, AC_MSG_ERROR([
*** Liquid War 6 needs Guile 1.8 (http://www.gnu.org/software/guile/)
]))
   else
      # Guile-config doesn't work (GP2X for instance), use builtin
      GUILE_LDFLAGS="-lguile -lgmp -lpthread -lltdl"
      AC_CHECK_LIB(guile, scm_with_guile, HAVE_LIBGUILE=1, AC_MSG_ERROR([
*** Liquid War 6 needs Guile 1.8 (http://www.gnu.org/software/guile/)
]), -lgmp -lpthread -lltdl)
   fi
fi
AC_SUBST(GUILE_CFLAGS)
AC_SUBST(GUILE_LDFLAGS)

AC_CHECK_HEADER(ltdl.h, HAVE_LTDL_H=1, AC_MSG_ERROR([
*** Liquid War 6 needs ltdl (http://www.gnu.org/software/libtool/)
]))
AC_CHECK_LIB(ltdl, lt_dlopen, HAVE_LIBLTDL=1, AC_MSG_ERROR([
*** Liquid War 6 needs ltdl (http://www.gnu.org/software/libtool/)
]))

AC_CHECK_HEADER(zlib.h, HAVE_ZLIB_H=1, AC_MSG_ERROR([
*** Liquid War 6 needs zlib (http://www.zlib.net/)
]))

AC_CHECK_LIB(z, gzopen, HAVE_LIBZ=1, AC_MSG_ERROR([
*** Liquid War 6 needs zlib (http://www.zlib.net/)
]))

AC_CHECK_HEADER(expat.h, HAVE_EXPAT_H=1, AC_MSG_ERROR([
*** Liquid War 6 needs Expat (http://www.libexpat.org/)
]))
AC_CHECK_LIB(expat, XML_ParserCreate, HAVE_LIBEXPAT=1, AC_MSG_ERROR([
*** Liquid War 6 needs Expat (http://www.libexpat.org/)
]))

dnl Begin console
AC_CHECK_HEADER(ncurses.h, HAVE_NCURSES_H=1, AC_MSG_WARN([
*** Liquid War 6 recommends ncurses (http://www.gnu.org/software/ncurses/)
]))
AC_CHECK_LIB(ncurses, wresize, HAVE_LIBNCURSES=1, AC_MSG_WARN([
*** Liquid War 6 recommends ncurses (http://www.gnu.org/software/ncurses/)
]))

AC_CHECK_HEADER(readline/readline.h, HAVE_READLINE_READLINE_H=1, AC_MSG_WARN([
*** Liquid War 6 recommends GNU Readline (http://cnswww.cns.cwru.edu/php/chet/readline/rltop.html)
]))
AC_CHECK_LIB(readline, rl_complete, HAVE_LIBREADLINE=1, AC_MSG_WARN([
*** Liquid War 6 recommends GNU Readline (http://cnswww.cns.cwru.edu/php/chet/readline/rltop.html)
]), -lncurses)

AC_ARG_ENABLE([console], AS_HELP_STRING([--enable-console],[enable console default=yes]), [enable_console=$enableval], [enable_console=yes])

AM_CONDITIONAL(LW6_CONSOLE, test x$enable_console = xyes && test x$HAVE_NCURSES_H = x1 && test x$HAVE_LIBNCURSES = x1 && test x$HAVE_READLINE_READLINE_H = x1 && test x$HAVE_LIBREADLINE = x1)

if test -z "${LW6_CONSOLE_TRUE}"; then
	LW6_CONSOLE_ENABLE_CFLAGS="-DLW6_CONSOLE=1"
	LW6_CONSOLE_ENABLE_LDFLAGS="-lreadline -lncurses"
else
	LW6_CONSOLE_ENABLE_CFLAGS=
	LW6_CONSOLE_ENABLE_LDFLAGS=
fi
AC_SUBST(LW6_CONSOLE_ENABLE_CFLAGS)
AC_SUBST(LW6_CONSOLE_ENABLE_LDFLAGS)
dnl End console

dnl Begin GTK
AC_ARG_ENABLE([gtk], AS_HELP_STRING([--enable-gtk],[enable gtk default=yes]), [enable_gtk=$enableval], [enable_gtk=yes])

if which pkg-config && pkg-config --exists gtk+-2.0 && test "x${enable_gtk}" = xyes && test "x${lw6_ms_windows}" != "xyes" && test "x${lw6_mac_os_x}" != "xyes"; then
   	LW6_GTK_ENABLE_CFLAGS="-DLW6_GTK=1 `pkg-config --cflags gtk+-2.0`"
   	LW6_GTK_ENABLE_LDFLAGS="`pkg-config --libs gtk+-2.0`"
	LW6_HAVE_GTK=1
else
   	LW6_GTK_ENABLE_CFLAGS=
   	LW6_GTK_ENABLE_LDFLAGS=
	LW6_HAVE_GTK=
fi

AM_CONDITIONAL(LW6_GTK, test x$LW6_HAVE_GTK = x1)

AC_SUBST(LW6_GTK_ENABLE_CFLAGS)
AC_SUBST(LW6_GTK_ENABLE_LDFLAGS)
dnl End GTK

dnl Begin mod-http
AC_CHECK_HEADER(curl/curl.h, HAVE_CURL_H=1, AC_MSG_WARN([
*** Liquid War 6 recommends libcURL (http://curl.haxx.se/libcurl/)
]))
AC_CHECK_LIB(curl, curl_global_init, HAVE_LIBCURL=1, AC_MSG_WARN([
*** Liquid War 6 recommends libcURL (http://curl.haxx.se/libcurl/)
]))

AC_ARG_ENABLE([mod-http], AS_HELP_STRING([--enable-mod-http],[compile mod-http default=yes]), [enable_mod_http=$enableval], [enable_mod_http=yes])

AM_CONDITIONAL(MOD_HTTP, test x$enable_mod_http = xyes && test x$HAVE_CURL_H = x1 && test x$HAVE_LIBCURL = x1)

if test -z "${MOD_HTTP_TRUE}"; then
	MOD_HTTP_ENABLE_CFLAGS="-DMOD_HTTP=1"
else
	MOD_HTTP_ENABLE_CFLAGS=
fi
AC_SUBST(MOD_HTTP_ENABLE_CFLAGS)
dnl End mod-http

AC_CHECK_HEADER(sqlite3.h, HAVE_SQLITE3_H=1, AC_MSG_ERROR([
*** Liquid War 6 needs SQLite 3 (http://www.sqlite.org/)
]))
if test x${HAVE_LIBDL} = x1; then
   AC_CHECK_LIB(sqlite3, sqlite3_exec, HAVE_LIBSQLITE3=1, AC_MSG_ERROR([
*** Liquid War 6 needs SQLite 3 (http://www.sqlite.org/)
]), -ldl)
else
   AC_CHECK_LIB(sqlite3, sqlite3_exec, HAVE_LIBSQLITE3=1, AC_MSG_ERROR([
*** Liquid War 6 needs SQLite 3 (http://www.sqlite.org/)
]))
fi

AC_CHECK_HEADER(libpng12/png.h, HAVE_LIBPNG12_PNG_H=1)
AC_CHECK_HEADER(libpng14/png.h, HAVE_LIBPNG14_PNG_H=1)
if test x${HAVE_LIBPNG12_PNG_H} = x1 || test x${HAVE_LIBPNG14_PNG_H} = x1; then
   if test x${HAVE_LIBPNG12_PNG_H} = x1; then
      LIBPNG_1X_H=libpng12/png.h
   fi
   if test x${HAVE_LIBPNG14_PNG_H} = x1; then
      LIBPNG_1X_H=libpng14/png.h
   fi
else
   AC_MSG_ERROR([
*** Liquid War 6 needs libpng 1.2 or libpng 1.4 (http://www.libpng.org/pub/png/libpng.html)
])
fi
AC_SUBST(LIBPNG_1X_H)

AC_CHECK_LIB(png, png_sig_cmp, HAVE_LIBPNG=1, AC_MSG_ERROR([
*** Liquid War 6 needs libpng 1.2 or libpng 1.4 (http://www.libpng.org/pub/png/libpng.html)
]))

AC_CHECK_HEADER(jpeglib.h, HAVE_JPEGLIB_H=1, AC_MSG_ERROR([
*** Liquid War 6 needs libjpeg (http://www.ijg.org/)
]))

AC_CHECK_LIB(jpeg, jpeg_start_decompress, HAVE_LIBJPEG=1, AC_MSG_ERROR([
*** Liquid War 6 needs libjpeg (http://www.ijg.org/)
]))

AC_ARG_ENABLE([headless], AS_HELP_STRING([--enable-headless],[accept headless built default=no]), [enable_mod_gl=$enableval], [enable_headless=no])

dnl Begin mod-gl

SDL_EXTRA_CFLAGS=
SDL_EXTRA_LDFLAGS=
SDL_CFLAGS=
SDL_LIBS=
AC_CHECK_PROG(SDL_CONFIG, sdl-config, yes, no)
if test x${SDL_CONFIG} = xyes; then
   SDL_EXTRA_CFLAGS=`sdl-config --cflags`
   SDL_EXTRA_LIBS=`sdl-config --libs`
else
   AC_MSG_WARN([
*** Liquid War 6 needs SDL (http://www.libsdl.org/)
])
fi
SDL_EXTRA="${SDL_EXTRA_CFLAGS} ${SDL_EXTRA_LIBS}"

AC_CHECK_HEADER(SDL/SDL.h, HAVE_SDL_H=1, AC_MSG_WARN([
*** Liquid War 6 needs SDL (http://www.libsdl.org/)
]),[${SDL_EXTRA}])
AC_CHECK_LIB(SDL, SDL_Init, HAVE_LIBSDL=1, AC_MSG_WARN([
*** Liquid War 6 needs SDL (http://www.libsdl.org/)
]),[${SDL_EXTRA}])

AC_CHECK_HEADER(GL/gl.h, HAVE_GL_GL_H=1,AC_MSG_WARN([
*** Liquid War 6 needs Mesa (http://www.mesa3d.org/)
]),[${SDL_EXTRA}])
AC_CHECK_LIB(GL, glBegin, HAVE_LIBGL=1, AC_MSG_WARN([
*** Liquid War 6 needs Mesa (http://www.mesa3d.org/)
]),[${SDL_EXTRA}])

AC_CHECK_HEADER(GL/glu.h, HAVE_GL_GLU_H=1,AC_MSG_WARN([
*** Liquid War 6 needs GLU (http://www.mesa3d.org/)
]),[${SDL_EXTRA}])
AC_CHECK_LIB(GLU, gluBeginCurve, HAVE_LIBGLU=1, AC_MSG_WARN([
*** Liquid War 6 needs GLU (http://www.mesa3d.org/)
]),[${SDL_EXTRA}])

AC_CHECK_HEADER(SDL/SDL_image.h, HAVE_SDL_IMAGE_H=1, AC_MSG_WARN([
*** Liquid War 6 needs SDL_image (http://www.libsdl.org/projects/SDL_image/)
]),[${SDL_EXTRA}])
AC_CHECK_LIB(SDL_image, IMG_Load, HAVE_LIBSDL_IMAGE=1, AC_MSG_WARN([
*** Liquid War 6 needs SDL_image (http://www.libsdl.org/projects/SDL_image/)
]),[${SDL_EXTRA}])

AC_CHECK_HEADER(SDL/SDL_ttf.h, HAVE_SDL_TTF_H=1, AC_MSG_WARN([
*** Liquid War 6 needs SDL_ttf (http://www.libsdl.org/projects/SDL_ttf/)
]),[${SDL_EXTRA}])
AC_CHECK_LIB(SDL_ttf, TTF_Init, HAVE_LIBSDL_TTF=1, AC_MSG_WARN([
*** Liquid War 6 needs SDL_ttf (http://www.libsdl.org/projects/SDL_ttf/)
]),[${SDL_EXTRA}])

AC_ARG_ENABLE([mod-gl], AS_HELP_STRING([--enable-mod-gl],[compile mod-gl default=yes]), [enable_mod_gl=$enableval], [enable_mod_gl=yes])

SDL_CFLAGS=${SDL_EXTRA_CFLAGS}
AC_SUBST(SDL_CFLAGS)

if test "x${lw6_mac_os_x}" = "xyes" ; then
   GL_LIBS="-L/usr/X11/lib -Wl,-framework,OpenGL"
else
   if test x$HAVE_LIBGL = x1; then
      GL_LIBS="-lGL"
   fi
   if test "x${lw6_ms_windows}" = "xyes" ; then
      GL_LIBS="-lopengl32"
   fi
   if test x$HAVE_LIBGLU = x1; then
      GLU_LIBS="-lGLU"
   fi
   if test "x${lw6_ms_windows}" = "xyes" ; then
      GLU_LIBS="-lglu32"
   fi
fi
if test x$enable_mod_gl = xyes; then
   SDL_LIBS="$SDL_EXTRA_LIBS $GL_LIBS $GLU_LIBS -lSDL_image -lSDL_ttf -lz -lpng"
fi
AC_SUBST(SDL_LIBS)

# libGL and libGLU not required (mingw32 + Mac OS X issue)
AM_CONDITIONAL(MOD_GL, test x$enable_mod_gl = xyes && test x$SDL_CONFIG = xyes && test x$HAVE_SDL_H = x1 && test x$HAVE_LIBSDL = x1 && test x$HAVE_SDL_IMAGE_H = x1 && test x$HAVE_LIBSDL_IMAGE = x1 && test x$HAVE_SDL_TTF_H = x1 && test x$HAVE_LIBSDL_TTF = x1 && test x$HAVE_GL_GL_H && test x$HAVE_GL_GLU_H)

if test -z "${MOD_GL_TRUE}"; then
	MOD_GL_ENABLE_CFLAGS="-DMOD_GL=1"
else
	MOD_GL_ENABLE_CFLAGS=
fi
AC_SUBST(MOD_GL_ENABLE_CFLAGS)
dnl End mod-gl

lw6_head=no
if test x$enable_headless != xyes ; then
   if test -z "${MOD_GL_TRUE}"; then
      lw6_head=gl
   fi
   if test x$lw6_head = xno; then
      AC_MSG_ERROR([No graphical support detected, please consider installing SDL (http://www.libsdl.org/), Mesa (http://www.mesa3d.org/), SDL_image (http://www.libsdl.org/projects/SDL_image/), SDL_ttf (http://www.libsdl.org/projects/SDL_ttf/) with development support (headers and libraries).])
   else
      AC_MSG_NOTICE([Good, at least one gfx backend should work])
   fi
fi

dnl Begin mod-csound
AC_CHECK_PROG(CSOUND_CONFIG, csound-config, yes, no)
if test x${CSOUND_CONFIG} != xyes; then
   AC_MSG_WARN([
*** Liquid War 6 recommends csound (http://www.csounds.com/)
])
fi
CSOUND_CFLAGS=`csound-config --cflags`
AC_SUBST(CSOUND_CFLAGS)
CSOUND_LIBS=`csound-config --libs`
AC_SUBST(CSOUND_LIBS)
AC_CHECK_HEADER(csound/csound.h, HAVE_CSOUND_CSOUND_H=1, AC_MSG_NOTICE([Liquid War 6 will probably use libcsound (http://www.csounds.com/) some day but it's not required yet.]))
AC_CHECK_LIB(csound, csoundCreate, HAVE_LIBCSOUND=1, AC_MSG_NOTICE([Liquid War 6 will probably use libcsound (http://www.csounds.com/) some day but it's not required yet.]),${CSOUND_LIBS})

AC_ARG_ENABLE([mod-csound], AS_HELP_STRING([--enable-mod-csound],[compile mod-csound default=yes]), [enable_mod_csound=$enableval], [enable_mod_csound=yes])

AM_CONDITIONAL(MOD_CSOUND, test x$enable_mod_csound = xyes && test x$CSOUND_CONFIG = xyes && test x$HAVE_CSOUND_CSOUND_H = x1 && test x$HAVE_LIBCSOUND = x1)

if test -z "${MOD_CSOUND_TRUE}"; then
	MOD_CSOUND_ENABLE_CFLAGS="-DMOD_CSOUND=1"
else
	MOD_CSOUND_ENABLE_CFLAGS=
fi
AC_SUBST(MOD_CSOUND_ENABLE_CFLAGS)
dnl End mod-csound

dnl Begin mod-ogg
AC_CHECK_HEADER(SDL/SDL_mixer.h, HAVE_SDL_MIXER_H=1, AC_MSG_WARN([
** Liquid War 6 recommends SDL_mixer (http://www.libsdl.org/projects/SDL_mixer/)
]))
AC_CHECK_LIB(SDL_mixer, Mix_OpenAudio, HAVE_LIBSDL_MIXER=1, AC_MSG_WARN([
** Liquid War 6 recommends SDL_mixer (http://www.libsdl.org/projects/SDL_mixer/)
]))
# No need to check ogg.h/libogg, SDL_mixer embeds ogg stuff

AC_ARG_ENABLE([mod-ogg], AS_HELP_STRING([--enable-mod-ogg],[compile mod-ogg default=yes]), [enable_mod_ogg=$enableval], [enable_mod_ogg=yes])

AM_CONDITIONAL(MOD_OGG, test x$enable_mod_ogg = xyes && test x$HAVE_SDL_H = x1 && test x$HAVE_LIBSDL = x1 && test x$HAVE_SDL_MIXER_H = x1 && test x$HAVE_LIBSDL_MIXER = x1)

if test -z "${MOD_OGG_TRUE}"; then
	MOD_OGG_ENABLE_CFLAGS="-DMOD_OGG=1"
	SDL_LIBS_SOUND="-lSDL_mixer "`sdl-config --libs`
else
	MOD_OGG_ENABLE_CFLAGS=
	SDL_LIBS_SOUND=
fi
AC_SUBST(MOD_OGG_ENABLE_CFLAGS)
AC_SUBST(SDL_LIBS_SOUND)
dnl End mod-ogg

LW6_CONFIGURE_ARGS="`echo ${ac_configure_args} | sed 's/^ //g'`"
AC_SUBST(LW6_CONFIGURE_ARGS)

LW6_HOSTNAME="`hostname`"
AC_SUBST(LW6_HOSTNAME)

LW6_HOST_CPU="${host_cpu}"
AC_SUBST(LW6_HOST_CPU)

LW6_HOST_OS="${host_os}"
AC_SUBST(LW6_HOST_OS)

LW6_CODENAME="Gengis Khan"
AC_SUBST(LW6_CODENAME)

AC_ARG_ENABLE([optimize], AS_HELP_STRING([--enable-optimize],[optimize for speed default=no]), [enable_optimize=$enableval], [enable_optimize=no])
if test x${enable_optimize} = xyes ; then
   CPU_FLAGS=""
   # For the x86 series of CPUs, we enable -march, because otherwise
   # one usually ends up with ugly old 386 code which is obviously
   # slow compared to the latest state of the art pentium blah blah.
   #
   # For any other arch, fiddle with CFLAGS yourself 8-)
   if x${lw6_x86} = xyes ; then
      CPU_FLAGS="-march=${host_cpu}"
   fi
   OPTIMIZE_CFLAGS="-fomit-frame-pointer ${CPU_FLAGS} -DLW6_OPTIMIZE=1"
else
   # let automake decide the CFLAGS value
   OPTIMIZE_CFLAGS=
fi
AC_SUBST(OPTIMIZE_CFLAGS)

AC_ARG_ENABLE([allinone], AS_HELP_STRING([--enable-allinone],[stuff internal libraries into main binary default=no]), [enable_allinone=$enableval], [enable_allinone=no])

AC_ARG_ENABLE([fullstatic], AS_HELP_STRING([--enable-fullstatic],[build a completely static binary default=no]), [enable_fullstatic=$enableval], [enable_fullstatic=no])

if test x${enable_fullstatic} = xyes; then
   enable_allinone=yes
fi

if test x${enable_gprof} = xyes; then
   enable_allinone=yes
fi

if test x${enable_gcov} = xyes; then
   enable_allinone=yes
fi

if test "x${lw6_ms_windows}" = "xyes" ; then
   enable_allinone=yes
fi

if test "x${lw6_mac_os_x}" = "xyes" ; then
   enable_allinone=yes
fi
   
if test x${enable_allinone} = xyes ; then
   ALLINONE_CFLAGS="-DLW6_ALLINONE=1"
   ALLINONE_LDFLAGS=
else
   ALLINONE_CFLAGS=
   if ld -v | grep GNU ; then
      ALLINONE_LDFLAGS="-Wl,--export-dynamic"
   else
      ALLINONE_LDFLAGS="-Wl"
   fi
fi
AC_SUBST(ALLINONE_CFLAGS)
AC_SUBST(ALLINONE_LDFLAGS)

AM_CONDITIONAL(ALLINONE, test x${enable_allinone} = xyes)
          
AM_CONDITIONAL(FULLSTATIC, test x${enable_fullstatic} = xyes)

if test x${enable_fullstatic} = xyes ; then
   # Note that the -static option is different from --static.
   # The first one (-static) only causes libtool/gcc to link *internal* libs,
   # whereas the other one (--static) produces a totally static binary,
   # with everything inside, include glibc.
   FULLSTATIC_CFLAGS="-DLW6_FULLSTATIC=1"
   FULLSTATIC_LDFLAGS="--static"
else
   FULLSTATIC_CFLAGS=""
   FULLSTATIC_LDFLAGS=""
fi
AC_SUBST(FULLSTATIC_CFLAGS)

AC_ARG_ENABLE([paranoid], AS_HELP_STRING([--enable-paranoid],[activate paranoid memory debugging default=no]), [enable_paranoid=$enableval], [enable_paranoid=no])
if test x${enable_optimize} = xyes ; then
   enable_paranoid=no
fi
if test x${enable_paranoid} = xyes ; then
   PARANOID_CFLAGS="-DLW6_PARANOID=1"
else
   PARANOID_CFLAGS=
fi
AC_SUBST(PARANOID_CFLAGS)

AC_ARG_ENABLE([gprof], AS_HELP_STRING([--enable-gprof],[add profiling informations into binary default=no]), [enable_gprof=$enableval], [enable_gprof=no])
if test x${enable_gprof} = xyes ; then
   GPROF_CFLAGS="-pg -DLW6_GPROF=1"
   GPROF_LDFLAGS="-pg"
else
   GPROF_CFLAGS=
   GPROF_LDFLAGS=
fi
AC_SUBST(GPROF_CFLAGS)
AC_SUBST(GPROF_LDFLAGS)

AC_ARG_ENABLE([instrument], AS_HELP_STRING([--enable-instrument],[instrument functions for profiling default=no]), [enable_instrument=$enableval], [enable_instrument=no])
if test x${enable_instrument} = xyes ; then
   INSTRUMENT_CFLAGS="-finstrument-functions -DLW6_INSTRUMENT=1"
   if test "x${INSTRUMENT_LDFLAGS}" = "x"; then
      if test "x${lw6_mac_os_x}" = "xyes" ; then
         INSTRUMENT_LDFLAGS="-lSaturn"
      fi
   fi
else
   INSTRUMENT_CFLAGS=
   INSTRUMENT_LDFLAGS=
fi

AC_CHECK_LIB(profiler, ProfilerStart, HAVE_LIBPROFILER=1, AC_MSG_NOTICE([
*** Liquid War 6 suggests Google PerfTools (http://code.google.com/p/google-perftools/)
]))
AC_CHECK_LIB(tcmalloc, tc_malloc, HAVE_LIBTCMALLOC=1, AC_MSG_NOTICE([
*** Liquid War 6 suggests Google PerfTools (http://code.google.com/p/google-perftools/)
]))
AC_ARG_ENABLE([profiler], AS_HELP_STRING([--enable-profiler],[support for Google Profiler default=no]), [enable_profiler=$enableval], [enable_profiler=no])
if test x${enable_profiler} = xyes && test x$HAVE_LIBPROFILER = x1 && test x$HAVE_LIBTCMALLOC = x1 ; then
   PROFILER_CFLAGS="-DLW6_PROFILER=1"
   PROFILER_LDFLAGS="-lprofiler -ltcmalloc"
   AC_SUBST(CFLAGS)
else
   enable_profiler=no
   PROFILER_CFLAGS=
   PROFILER_LDFLAGS=
fi
AC_SUBST(PROFILER_CFLAGS)
AC_SUBST(PROFILER_LDFLAGS)

AC_ARG_ENABLE([gcov], AS_HELP_STRING([--enable-gcov],[add coverage informations into binary default=no]), [enable_gcov=$enableval], [enable_gcov=no])
if test x${enable_gcov} = xyes ; then
   GCOV_CFLAGS="-fprofile-arcs -ftest-coverage -DLW6_GCOV=1"
else
   GCOV_CFLAGS=
fi
AC_SUBST(GCOV_CFLAGS)

AC_ARG_ENABLE([valgrind], AS_HELP_STRING([--enable-valgrind],[prepare binary for valgrind default=no]), [enable_valgrind=$enableval], [enable_valgrind=no])
if test x${enable_valgrind} = xyes ; then
   enable_optimize=no
   VALGRIND_CFLAGS="-DLW6_VALGRIND=1"
   CFLAGS="-O0 -g"
   AC_SUBST(CFLAGS)
else
   # let automake decide the CFLAGS value
   DUMMY="if else fi fails if this is empty"
fi
AC_SUBST(VALGRIND_CFLAGS)

if test x${USE_INCLUDED_LIBINTL} = xyes ; then
   LW6_LIBINTL_LDFLAGS=
   LW6_LIBINTL_LDADD=${LTLIBINTL}
else
   LW6_LIBINTL_LDFLAGS=${LTLIBINTL}
   LW6_LIBINTL_LDADD=
fi
AC_SUBST(LW6_LIBINTL_LDFLAGS)
AC_SUBST(LW6_LIBINTL_LDADD)

if test "x${lw6_ms_windows}" = "xyes" ; then
   LW6_OS_CFLAGS="-DLW6_MS_WINDOWS=1 -DWIN32"
   LW6_OS_LDFLAGS="-lpthreadGC2 -lws2_32 -liphlpapi"
else
   if test "x${lw6_mac_os_x}" = "xyes" ; then
      LW6_OS_CFLAGS="-DLW6_UNIX=1 -DLW6_MAC_OS_X=1"
      LW6_OS_LDFLAGS="-lpthread -Wl,-framework -Wl,CoreFoundation -Wl,-framework -Wl,Cocoa"
   else
      if test "x{lw6_gp2x}" = "xyes" ; then
         LW6_OS_CFLAGS="-DLW6_GP2X=1"
         LW6_OS_LDFLAGS="-lpthread"
      else
         LW6_OS_CFLAGS="-DLW6_UNIX=1"
         LW6_OS_LDFLAGS="-lpthread"
      fi
   fi
fi;
AC_SUBST(LW6_OS_CFLAGS)
AC_SUBST(LW6_OS_LDFLAGS)

LW6_CFLAGS="-Wall ${LW6_X86_CFLAGS} ${ALLINONE_CFLAGS} ${FULLSTATIC_CFLAGS} ${OPTIMIZE_CFLAGS} ${PARANOID_CFLAGS} ${GPROF_CFLAGS} ${INSTRUMENT_CFLAGS} ${PROFILER_CFLAGS} ${GCOV_CFLAGS} ${VALGRIND_CFLAGS} ${LW6_OS_CFLAGS}"
LW6_LDFLAGS="${ALLINONE_LDFLAGS} ${FULLSTATIC_LDFLAGS} ${GPROF_LDFLAGS} ${INSTRUMENT_LDFLAGS} ${PROFILER_LDFLAGS} ${LW6_LIBINTL_LDFLAGS} ${LW6_OS_LDFLAGS}"
AC_SUBST(LW6_CFLAGS)
AC_SUBST(LW6_LDFLAGS)

AC_CONFIG_FILES([Makefile
          intl/Makefile
          po/Makefile.in
          doc/Makefile
          doc/liquidwar6.man
          doc/const.texi.in
          doc/coverage/Makefile
          doc/global/Makefile
          doc/cyclo/Makefile
          doc/example/Makefile
          doc/dtd/Makefile
          src/Makefile
          src/lib/Makefile
          src/lib/liquidwar6.h
	  src/lib/def/Makefile
	  src/lib/sys/Makefile
	  src/lib/sys/sys-build.h
	  src/lib/glb/Makefile
	  src/lib/dyn/Makefile
	  src/lib/cns/Makefile
	  src/lib/hlp/Makefile
	  src/lib/cfg/Makefile
	  src/lib/map/Makefile
	  src/lib/ker/Makefile
	  src/lib/bot/Makefile
	  src/lib/bot/mod-idiot/Makefile
	  src/lib/bot/mod-random/Makefile
	  src/lib/ldr/Makefile
	  src/lib/ldr/ldr-png.h
	  src/lib/tsk/Makefile
	  src/lib/gui/Makefile
	  src/lib/gfx/Makefile
  	  src/lib/gfx/mod-gl/Makefile
 	  src/lib/gfx/mod-gl/gl-utils/Makefile
 	  src/lib/gfx/mod-gl/gl-splash/Makefile
 	  src/lib/gfx/mod-gl/gl-background/Makefile
 	  src/lib/gfx/mod-gl/gl-background/gl-bubbles/Makefile
 	  src/lib/gfx/mod-gl/gl-background/gl-void/Makefile
 	  src/lib/gfx/mod-gl/gl-hud/Makefile
 	  src/lib/gfx/mod-gl/gl-hud/gl-floating/Makefile
 	  src/lib/gfx/mod-gl/gl-hud/gl-tactical/Makefile
 	  src/lib/gfx/mod-gl/gl-menu/Makefile
 	  src/lib/gfx/mod-gl/gl-menu/gl-cylinder/Makefile
 	  src/lib/gfx/mod-gl/gl-view/Makefile
 	  src/lib/gfx/mod-gl/gl-view/gl-flat/Makefile
	  src/lib/dsp/Makefile
	  src/lib/snd/Makefile
	  src/lib/snd/mod-csound/Makefile
	  src/lib/snd/mod-ogg/Makefile
	  src/lib/net/Makefile
	  src/lib/nod/Makefile
	  src/lib/cnx/Makefile
	  src/lib/msg/Makefile
	  src/lib/img/Makefile
	  src/lib/pil/Makefile
	  src/lib/cli/Makefile
	  src/lib/cli/mod-tcp/Makefile
	  src/lib/cli/mod-udp/Makefile
	  src/lib/cli/mod-http/Makefile
	  src/lib/srv/Makefile
	  src/lib/srv/mod-tcpd/Makefile
	  src/lib/srv/mod-udpd/Makefile
	  src/lib/srv/mod-httpd/Makefile
	  src/lib/dat/Makefile
	  src/lib/p2p/Makefile
	  src/mswindows.rc
	  script/Makefile
	  data/Makefile
	  data/icon/Makefile
	  data/gfx/Makefile
  	  data/gfx/gl/Makefile
 	  data/gfx/gl/utils/Makefile
 	  data/gfx/gl/utils/font/Makefile
 	  data/gfx/gl/utils/icon/Makefile
 	  data/gfx/gl/splash/Makefile
 	  data/gfx/gl/splash/image/Makefile
 	  data/gfx/gl/background/Makefile
 	  data/gfx/gl/background/bubbles/Makefile
 	  data/gfx/gl/background/bubbles/image/Makefile
 	  data/gfx/gl/hud/Makefile
 	  data/gfx/gl/hud/floating/Makefile
 	  data/gfx/gl/hud/floating/image/Makefile
 	  data/gfx/gl/hud/tactical/Makefile
 	  data/gfx/gl/hud/tactical/image/Makefile
 	  data/gfx/gl/menu/Makefile
 	  data/gfx/gl/menu/cylinder/Makefile
 	  data/gfx/gl/view/Makefile
 	  data/gfx/gl/view/flat/Makefile
 	  data/snd/Makefile
 	  data/snd/csound/Makefile
 	  data/snd/ogg/Makefile
	  data/snd/ogg/sound/Makefile
	  data/net/Makefile
	  data/srv/Makefile
	  data/srv/tcpd/Makefile
	  data/srv/udpd/Makefile
	  data/srv/httpd/Makefile
	  data/srv/httpd/htdocs/Makefile
	  data/cli/Makefile
	  data/cli/tcp/Makefile
	  data/cli/udp/Makefile
	  data/cli/http/Makefile
	  data/p2p/Makefile
	  data/p2p/sql/Makefile
	  data/p2p/screenshot/Makefile
	  music/Makefile
	  map/Makefile
	  map/acadia/Makefile
	  map/fish/Makefile
	  map/fishy/Makefile
	  map/five-points/Makefile
	  map/ink-and-oil/Makefile
	  map/january/Makefile
	  map/liquid-dream/Makefile
	  map/nine-points/Makefile
	  map/pizza-face/Makefile
	  map/playfield/Makefile
	  map/strange-new-world/Makefile
	  map/subflower/Makefile
	  debian/Makefile
	  debian/liquidwar6.dirs
	  debian/liquidwar6.install
	  pkg/Makefile
	  pkg/liquidwar6.spec
	  pkg/liquidwar6.nsi
	  pkg/Info.plist
	  pkg/liquidwar6.desktop])

AC_OUTPUT

AC_MSG_NOTICE([version is "${VERSION}"])
AC_MSG_NOTICE([prefix is "${prefix}"])
AC_MSG_NOTICE([hostname is "${HOSTNAME}"])
AC_MSG_NOTICE([host_cpu is "${host_cpu}"])
AC_MSG_NOTICE([host_os is "${host_os}"])

if test -z "${LW6_MS_WINDOWS_TRUE}"; then
	AC_MSG_NOTICE([MS Windows support... yes])
else
	AC_MSG_NOTICE([MS Windows support... no])
fi
if test -z "${LW6_MAC_OS_X_TRUE}"; then
	AC_MSG_NOTICE([Mac OS X support... yes])
else
	AC_MSG_NOTICE([Mac OS X support... no])
fi
if test -z "${LW6_GP2X_TRUE}"; then
	AC_MSG_NOTICE([GP2X support... yes])
else
	AC_MSG_NOTICE([GP2X support... no])
fi
if test -z "${LW6_X86_TRUE}"; then
	AC_MSG_NOTICE([X86 support... yes])
else
	AC_MSG_NOTICE([X86 support... no])
fi
if test -z "${LW6_CONSOLE_TRUE}"; then
	AC_MSG_NOTICE([enable console... yes])
else
	AC_MSG_NOTICE([enable console... no])
fi
if test -z "${LW6_GTK_TRUE}"; then
	AC_MSG_NOTICE([enable gtk... yes])
else
	AC_MSG_NOTICE([enable gtk... no])
fi
if test -z "${MOD_GL_TRUE}"; then
	AC_MSG_NOTICE([build mod-gl... yes])
else
	AC_MSG_NOTICE([build mod-gl... no])
fi
if test -z "${MOD_CSOUND_TRUE}"; then
	AC_MSG_NOTICE([build mod-csound... yes])
else
	AC_MSG_NOTICE([build mod-csound... no])
fi
if test -z "${MOD_OGG_TRUE}"; then
	AC_MSG_NOTICE([build mod-ogg... yes])
else
	AC_MSG_NOTICE([build mod-ogg... no])
fi
if test -z "${MOD_HTTP_TRUE}"; then
	AC_MSG_NOTICE([build mod-http... yes])
else
	AC_MSG_NOTICE([build mod-http... no])
fi
if test x${enable_optimize} = xyes; then
	AC_MSG_NOTICE([enable optimize... yes])
else
	AC_MSG_NOTICE([enable optimize... no])
fi
if test -z "${ALLINONE_TRUE}"; then
	AC_MSG_NOTICE([enable allinone... yes])
else
	AC_MSG_NOTICE([enable allinone... no])
fi
if test -z "${FULLSTATIC_TRUE}"; then
	AC_MSG_NOTICE([enable fullstatic... yes])
else
	AC_MSG_NOTICE([enable fullstatic... no])
fi
if test x${enable_paranoid} = xyes; then
	AC_MSG_NOTICE([enable paranoid... yes])
else
	AC_MSG_NOTICE([enable paranoid... no])
fi
if test x${enable_gprof} = xyes; then
	AC_MSG_NOTICE([enable gprof... yes])
else
	AC_MSG_NOTICE([enable gprof... no])
fi
if test x${enable_instrument} = xyes; then
	AC_MSG_NOTICE([enable instrument... yes])
else
	AC_MSG_NOTICE([enable instrument... no])
fi
if test x${enable_profiler} = xyes; then
	AC_MSG_NOTICE([enable profiler... yes])
else
	AC_MSG_NOTICE([enable profiler... no])
fi
if test x${enable_gcov} = xyes; then
	AC_MSG_NOTICE([enable gcov... yes])
else
	AC_MSG_NOTICE([enable gcov... no])
fi
if test x${enable_valgrind} = xyes; then
	AC_MSG_NOTICE([enable valgrind... yes])
else
	AC_MSG_NOTICE([enable valgrind... no])
fi
if test x${enable_htmldoc} = xyes; then
	AC_MSG_NOTICE([enable htmldoc... yes])
else
	AC_MSG_NOTICE([enable htmldoc... no])
fi

${srcdir}/src/stamp.sh auto

