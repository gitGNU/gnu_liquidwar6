/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015  Christian Mauduit <ufoot@ufoot.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.


  Liquid War 6 homepage : http://www.gnu.org/software/liquidwar6/
  Contact author        : ufoot@ufoot.org
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "hlp.h"
#include "hlp-internal.h"

int
_lw6hlp_populate_references_show (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 0;

  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_PACKAGE_TARNAME, _x_ ("Shows the package tarname, that is, liquidwar6."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_PACKAGE_NAME, _x_ ("Shows the package name, that is, 'Liquid War 6'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_PACKAGE_STRING, _x_ ("Shows the package string, that is, 'Liquid War 6 <version>"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_PACKAGE_ID,
			 _x_ ("Shows the package tarname with its version, that is, 'liquidwar6-<version>"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_VERSION,
			 _x_
			 ("Shows the version. Note that this is different from the standard GNU 'version' command line option which shows a complete message with a short copyright notice. This one will just return the version, without the package tarname or anything else."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_CODENAME,
			 _x_
			 ("Shows the codename associated with this version, generally the name of someone famous who is war-related (a general, an emperor...)."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_VERSION_BASE,
			 _x_
			 ("Shows the version base. This is basically MAJOR.MINOR and determines the level of compatibility of the program. Two programs with the same base version should be able to communicate on the network, share data files and even binary modules if on the same platform."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_VERSION_MAJOR,
			 _x_
			 ("Shows the major version number. This is just used to differenciate alpha/beta releases (using 0) from stable releases (using 6)."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_VERSION_MINOR,
			 _x_ ("Shows the minor version number. This is manually increased at each significant, public release of the game."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_STAMP,
			 _x_
			 ("Shows the build stamp. A very usefull value, more precise than the version to track down binaries. It is incremented each time the core C code is updated. It won't reflect all the programs for it does not take scripts in account, but if you are running a work-in-progress version, it might be very convenient to use this to know what your are running exactly. It's also used as the revision number (the third number afer MAJOR.MINOR)."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_MD5SUM,
			 _x_ ("Shows the MD5 checksum, which has been calculated from the C source files. Complementary with 'show-build-stamp'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_COPYRIGHT, _x_ ("Shows a very short copyright notice."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_LICENSE, _x_ ("Shows the license of the program (GNU GPL v3 or later)."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_HOME_URL, _x_ ("Shows the URL of the program, its homepage."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_BUGS_URL, _x_ ("Shows the URL to make bug reports."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_CONFIGURE_ARGS,
			 _x_
			 ("Shows the arguments that have been passed to the GNU Autoconf './configure' script when building the program. This can be very usefull if you want to know how the program has been built."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_GCC_VERSION,
			 _x_ ("Returns the version of the GNU C compiler which was used to compile the program."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_CFLAGS,
			 _x_
			 ("Shows what value you should put in 'CFLAGS' (environment variable) if you want to compile programs that use Liquid War 6 as a library, and include 'liquidwar6.h'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_LDFLAGS,
			 _x_ ("Shows what value you should put in 'LDFLAGS' (environment variable) if you want to link programs against libliquidwar6."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_HOSTNAME, _x_ ("Shows the name of the host where the binary was compiled."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_DATE, _x_ ("Shows the date when the binary was compiled."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_TIME, _x_ ("Shows the time when the binary was compiled."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_HOST_CPU, _x_ ("Shows the host CPU, as defined by 'host_cpu' in GNU Autoconf."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENDIANNESS,
			 _x_ ("Returns the endianness. 'little' corresponds to x86-like systems, 'big' to ppc-like systems."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_POINTER_SIZE,
			 _x_ ("Returns the pointer size, in bytes. Should be 4 on 32-bit systems and 8 on 64-bit systems."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_X86, _x_ ("Tells wether the CPU belongs to the x86 family."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_AMD64, _x_ ("Tells wether the CPU belongs to the amd64 family."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_HOST_OS, _x_ ("Shows the host OS, as defined by 'host_os' in GNU Autoconf."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_GNU,
			 _x_ ("Returns 1 (true) if host OS is a GNU system, or at least has been considered as such when compiling, 0 (false) if not."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_UNIX,
			 _x_ ("Returns 1 (true) if host OS is a UNIX system, or at least has been considered as such when compiling, 0 (false) if not."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_MS_WINDOWS, _x_ ("Returns 1 (true) if host OS is Microsoft Windows, 0 (false) if not."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_MAC_OS_X, _x_ ("Returns 1 (true) if host OS is Mac OS X, 0 (false) if not."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_GP2X, _x_ ("Returns 1 (true) if host is a GP2X, 0 (false) if not."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_TOP_SRCDIR,
			 _x_ ("Shows the top source directory on the machine where the binary was compiled, as a (possibly) relative path."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ABS_SRCDIR,
			 _x_ ("Shows the top source directory on the machine where the binary was compiled, as an absolute path."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_PREFIX,
			 _x_
			 ("Shows the 'prefix' value as passed to the GNU Autoconf './configure' script when compiling the program. Default is '/usr/local'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_DATADIR,
			 _x_
			 ("Shows the 'datadir' value as passed to the GNU Autoconf './configure' script when compiling the program. Default is '/usr/local/share'. This is the generic, non Liquid War 6 specific data directory. Liquid War 6 related data is stored elsewhere (usually in a sub-directory) see the 'data-dir' switch for more information. 'datadir' is not 'data-dir'. That's the point."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_LIBDIR,
			 _x_
			 ("Shows the 'libdir' value as passed to the GNU Autoconf './configure' script when compiling the program. Default is '/usr/local/lib'. This is the generic, non Liquid War 6 specific library directory. Dedicated Liquid War 6 modules are stored elsewhere (usually in a sub-directory) see the 'mod-dir' switch for more information."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_INCLUDEDIR,
			 _x_
			 ("Shows the 'includedir' value as passed to the GNU Autoconf './configure' script when compiling the program. Default is '/usr/local/include'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_LOCALEDIR,
			 _x_
			 ("Shows the 'localedir' value as passed to the GNU Autoconf './configure' script when compiling the program. Default is '/usr/local/share/locale'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_DOCDIR,
			 _x_
			 ("Shows the 'docdir' value as passed to the GNU Autoconf './configure' script when compiling the program. Default is '/usr/local/share/doc/liquidwar6'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_CONSOLE,
			 _x_
			 ("Shows wether the console has been enabled when building the game. This depends on parameters passed to './configure' and also on the presence of ncurses and readline."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_GTK,
			 _x_
			 ("Shows wether GTK+ support has been enabled when building the game. This depends on parameters passed to './configure' and also on the presence of GTK+ headers and libs. It uses pkg-config to detect it."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_MOD_GL1,
			 _x_
			 ("Shows wether the mod-gl1 graphical backend has been enabled when building the game. This depends on parameters passed to './configure' and also on the presence of SDL and OpenGL related libraries."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_MOD_GLES2,
			 _x_
			 ("Shows wether the mod-gles2 graphical backend has been enabled when building the game. This depends on parameters passed to './configure' and also on the presence of SDL and OpenGL ES related libraries."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_MOD_SOFT,
			 _x_
			 ("Shows wether the mod-soft graphical backend has been enabled when building the game. This depends on parameters passed to './configure' and also on the presence of SDL related libraries."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_MOD_CACA,
			 _x_
			 ("Shows wether the mod-caca graphical backend has been enabled when building the game. This depends on parameters passed to './configure' and also on the presence of libcaca related libraries."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_MOD_CSOUND,
			 _x_
			 ("Shows wether the mod-csound audio backend has been enabled when building the game. This depends on parameters passed to './configure' and also on the presence of the csound library."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_MOD_OGG,
			 _x_
			 ("Shows wether the mod-ogg audio backend has been enabled when building the game. This depends on parameters passed to './configure' and also on the presence of SDL and related libraries."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_MOD_HTTP,
			 _x_
			 ("Shows wether the mod-http network backend has been enabled when building the game. This depends on parameters passed to './configure' and also on the presence of libCurl."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_OPENMP,
			 _x_ ("Shows wether the program was built with OpenMP support. This depends on parameters passed to './configure'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_OPTIMIZE,
			 _x_
			 ("Shows wether the 'optimize' option has been chosen when building the game. This depends on parameters passed to './configure'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_ALLINONE,
			 _x_
			 ("Shows wether the 'allinone' option has been chosen when building the game. This depends on parameters passed to './configure'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_FULLSTATIC,
			 _x_
			 ("Shows wether the 'fullstatic' option has been chosen when building the game. This depends on parameters passed to './configure'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_PARANOID,
			 _x_
			 ("Shows wether the game was build with paranoid memory management. This is for debugging purposes, the default already includes some controls, with turned it's really... paranoid."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_GPROF,
			 _x_ ("Shows wether the game was build with suitable informations for gprof. This depends on parameters passed to './configure'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_INSTRUMENT,
			 _x_
			 ("Shows wether the game was build with the '-finstrument-functions' GCC switch. This depends on parameters passed to './configure'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_PROFILER,
			 _x_ ("Shows wether the game was build with Google Profiler support. This depends on parameters passed to './configure'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_GCOV,
			 _x_ ("Shows wether the game was build with suitable informations for gcov. This depends on parameters passed to './configure'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_ENABLE_VALGRIND,
			 _x_ ("Shows wether the game was build with valgrind later use in mind. This depends on parameters passed to './configure'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_BUILD_BIN_ID,
			 _x_
			 ("Shows the internal 'bin-id' value. This value does not mean anything in itself but it's supposed to change each time you compile the game."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_DEFAULT_USER_DIR,
			 _x_
			 ("Shows the default user directory path. This is where run-time data, config files, log files, are stored. Default is '$HOME/.liquidwar6/'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_DEFAULT_CONFIG_FILE,
			 _x_ ("Shows the default config file path. Default is '$HOME/.liquidwar6/config.xml'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_DEFAULT_LOG_FILE,
			 _x_ ("Shows the default log file path. Default is '$HOME/.liquidwar6/log.csv'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_DEFAULT_PREFIX,
			 _x_
			 ("Shows the default prefix used. This should logically be the value passed to the GNU Autoconf ./configure script when building the game. Most other path are deduced from this one. Default is '/usr/local'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_DEFAULT_MOD_DIR,
			 _x_
			 ("Shows the default module directory path. This is where all dynamically loaded modules are stored. Default is '/usr/local/lib/liquidwar6-<version>'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_DEFAULT_DATA_DIR,
			 _x_
			 ("Shows the default data directory path. This is where the games searches for most of its data,the most important exception being maps, which are stored elsewhere. Default is '/usr/local/share/liquidwar6-<version>/data'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_DEFAULT_MUSIC_DIR,
			 _x_
			 ("Shows the default music directory. This is where builtin musics are stored. Default is '/usr/local/share/liquidwar6-<version>/music'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_DEFAULT_MUSIC_PATH,
			 _x_
			 ("Shows the default music search path. This is where the game searches for musics. It's the combination of command-line arguments and builtin paths. Might return more directories than the one specified in a single 'music-path=dir1:dir2' argument."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_DEFAULT_MAP_DIR,
			 _x_
			 ("Shows the default map directory. This is where builtin maps are stored. Default is '/usr/local/share/liquidwar6-<version>/map'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_DEFAULT_MAP_PATH,
			 _x_
			 ("Shows the default map search path. This is where the game searches for maps. It's the combination of command-line arguments and builtin paths. Might return more directories than the one specified in a single 'map-path=dir1:dir2' argument."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_DEFAULT_SCRIPT_FILE,
			 _x_
			 ("Shows the default main script file path. This file is very important, since the program is more or less a hudge scheme interpreter, and this file is the file loaded by Guile. In short, it is the main program. Default is '/usr/local/share/liquidwar6-<version>/script/liquidwar6.scm'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_CWD, _x_ ("Shows the current working directory, the value that the pwd command would return."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_RUN_DIR,
			 _x_
			 ("Shows the run directory, usually the path where the binary is. It depends on how and where the program is launched. It is guessed from the argc/argv values at runtime."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_USER_DIR,
			 _x_
			 ("Shows the user directory path. This is where run-time data, config files, log files, are stored. Default is '$HOME/.liquidwar6/'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_CONFIG_FILE, _x_ ("Shows the config file path. Default is '$HOME/.liquidwar6/config.xml'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_LOG_FILE, _x_ ("Shows the log file path. Default is '$HOME/.liquidwar6/log.csv'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_PREFIX,
			 _x_
			 ("Shows the prefix used. This should logically be the value passed to the GNU Autoconf ./configure script when building the game. Most other path are deduced from this one. Default is '/usr/local'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_MOD_DIR,
			 _x_
			 ("Shows the module directory path. This is where all dynamically loaded modules are stored. Default is '/usr/local/lib/liquidwar6-<version>'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_DATA_DIR,
			 _x_
			 ("Shows the data directory path. This is where the games searches for most of its data,the most important exception being maps, which are stored elsewhere. Default is '/usr/local/share/liquidwar6-<version>/data'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_MUSIC_DIR,
			 _x_ ("Shows the music directory. This is where builtin maps are stored. Default is '/usr/local/share/liquidwar6-<version>/music'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_MUSIC_PATH,
			 _x_
			 ("Shows the music search path. This is where the game searches for musics. It's the combination of command-line arguments and builtin paths. Might return more directories than the one specified in a single 'music-path=dir1:dir2' argument."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_MAP_DIR,
			 _x_ ("Shows the map directory. This is where builtin maps are stored. Default is '/usr/local/share/liquidwar6-<version>/map'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_MAP_PATH,
			 _x_
			 ("Shows the map search path. This is where the game searches for maps. It's the combination of command-line arguments and builtin paths. Might return more directories than the one specified in a single 'map-path=dir1:dir2' argument."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHOW_SCRIPT_FILE,
			 _x_
			 ("Shows the main script file path. This file is very important, since the program is more or less a hudge scheme interpreter, and this file is the file loaded by Guile. In short, it is the main program. Default is '/usr/local/share/liquidwar6-<version>/script/liquidwar6.scm'."));

  ret = 1;

  return ret;
}
