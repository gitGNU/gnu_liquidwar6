/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

#include <string.h>

#include "hlp.h"
#include "hlp-internal.h"

#include "../map/map.h"
#include "../pil/pil.h"
#include "../ldr/ldr.h"
#include "../gui/gui.h"
#include "../net/net.h"

#define DEFAULT_TRUE "true"
#define DEFAULT_FALSE "false"

#define POPULATE_VOID(KEY,ABOUT) lw6sys_hash_set(_references,KEY,new_reference(LW6HLP_TYPE_VOID,KEY,ABOUT,NULL,0,0))
#define POPULATE_STR(KEY,ABOUT,DEFAULT) lw6sys_hash_set(_references,KEY,new_reference(LW6HLP_TYPE_STR,KEY,ABOUT,DEFAULT,0,0))
#define POPULATE_INT(KEY,ABOUT,DEFAULT,MIN,MAX) lw6sys_hash_set(_references,KEY,new_reference(LW6HLP_TYPE_INT,KEY,ABOUT,LW6SYS_QUOTE(DEFAULT),MIN,MAX))
#define POPULATE_FLOAT(KEY,ABOUT,DEFAULT,MIN,MAX) lw6sys_hash_set(_references,KEY,new_reference(LW6HLP_TYPE_FLOAT,KEY,ABOUT,LW6SYS_QUOTE(DEFAULT),MIN,MAX))
#define POPULATE_BOOL(KEY,ABOUT,DEFAULT) lw6sys_hash_set(_references,KEY,new_reference(LW6HLP_TYPE_BOOL,KEY,ABOUT,true_false(DEFAULT),0,0))
#define POPULATE_COLOR(KEY,ABOUT,DEFAULT) lw6sys_hash_set(_references,KEY,new_reference(LW6HLP_TYPE_COLOR,KEY,ABOUT,DEFAULT,0,0))

static lw6sys_hash_t *_references = NULL;

static char *
true_false (int b)
{
  char *ret = DEFAULT_FALSE;

  if (b)
    {
      ret = DEFAULT_TRUE;
    }

  return ret;
}

static _lw6hlp_reference_entry_t *
new_reference (lw6hlp_type_t type, char *key, char *about,
	       char *default_value, int min_value, int max_value)
{
  _lw6hlp_reference_entry_t *ret = NULL;

  ret =
    (_lw6hlp_reference_entry_t *)
    LW6SYS_MALLOC (sizeof (_lw6hlp_reference_entry_t));
  if (ret)
    {
      ret->type = type;
      ret->key = key;
      ret->about = about;
      ret->default_value = default_value;
      ret->min_value = min_value;
      ret->max_value = max_value;
    }

  return ret;
}

static int
populate_references ()
{
  int ret = 0;

  /*
   * Quick
   */
  POPULATE_VOID (LW6DEF_HELP, _("Returns a short help for the program."));
  POPULATE_VOID (LW6DEF_COPYRIGHT,
		 _("Returns the copyright notice for the program."));
  POPULATE_VOID (LW6DEF_VERSION,
		 _
		 ("Returns the version of the program, as defined by the GNU Coding Standards."));
  POPULATE_VOID (LW6DEF_TEST,
		 _
		 ("Runs a (hopefully) complete test suite which will call most internal Liquid War 6 functions and check out wether they work, in a simple context, without any game interference. Usefull for troubleshooting."));
  POPULATE_VOID (LW6DEF_DEFAULTS,
		 _
		 ("Clears the config file and run the game with default settings. Use this if you suspect you have broken something by tweaking user settings, or when upgrading the game to a new version."));
  POPULATE_VOID (LW6DEF_LIST,
		 _
		 ("Returns the list of all keywords which can be queried for information. This includes command-line options, environment variables, and so on. This is the companion option of '--about'. Results obtained with '--list' can be passed to '--about'."));
  POPULATE_STR (LW6DEF_ABOUT,
		_
		("Will allow you to get informations about a given keyword. Let's say that, for instance, you want informations about the keyword 'map-path'. Simply run 'liquidwar6 --about=map-path'. Note that this internal self-documentation system can describe command line switches as well as XML config file parameters or environment variables, and even some Guile script functions. The '--list' command line switch will give you the list of all available keywords."),
		NULL);
  POPULATE_VOID (LW6DEF_DEBUG,
		 _
		 ("Enables debug mode. This will turn on maximum log information, and display everything on stderr, even messages which are normally only stored in the log file."));
  POPULATE_VOID (LW6DEF_PEDIGREE,
		 _
		 ("Display all build values, these are general constants which can help debugging, tracing what binary you are running, and so on. It's a good idea to take a look at the output of 'pedigree' if you have problems running the game."));
  POPULATE_VOID (LW6DEF_HOST,
		 _
		 ("Display all known system host properties, including os and cpu informations."));
  POPULATE_VOID (LW6DEF_AUDIT,
		 _
		 ("Display all path values, defaults and current settings. This output is very usefull to track down problems such as missing directories, broken installations. If you get an error message that suggests some file is missing, then give this option a try."));
  POPULATE_VOID (LW6DEF_MODULES,
		 _
		 ("Tells which modules have been enabled when the game was compiled. It's still possible to add or remove modules afterwards, but this option allows you to know how things were at first."));
  /*
   * Advanced doc
   */
  POPULATE_VOID (LW6DEF_LIST_QUICK,
		 _
		 ("List quick help entries, this includes the GNU standard options and a few troubleshooting tools."));
  POPULATE_VOID (LW6DEF_LIST_DOC,
		 _
		 ("List documentation-related command line options. These commands allow you to list all the keywords related to a given domain."));
  POPULATE_VOID (LW6DEF_LIST_SHOW,
		 _
		 ("List command-line options which begin with '--show-...'. These will display on the console many internal parameters. Usefull when debugging."));
  POPULATE_VOID (LW6DEF_LIST_PATH,
		 _
		 ("List parameters which allow you to override the defaults of the game, and force the game your own file paths and directories."));
  POPULATE_VOID (LW6DEF_LIST_PLAYERS,
		 _
		 ("List player-related entries, that is to say 'who plays'."));
  POPULATE_VOID (LW6DEF_LIST_INPUT,
		 _
		 ("List input (AKA controls) related options. Use these to change keyboard, joystick and mouse settingds."));
  POPULATE_VOID (LW6DEF_LIST_GRAPHICS,
		 _("List graphics options (resolution, fullscreen...)."));
  POPULATE_VOID (LW6DEF_LIST_SOUND, _("List sound options (volume...)."));
  POPULATE_VOID (LW6DEF_LIST_NETWORK, _("List network options."));
  POPULATE_VOID (LW6DEF_LIST_MAP,
		 _
		 ("List map-related entries, excluding rules.xml, hints.xml and style.xml entries."));
  POPULATE_VOID (LW6DEF_LIST_MAP_RULES,
		 _
		 ("List 'options.xml' entries. These parameters enable you to modify the gameplay."));
  POPULATE_VOID (LW6DEF_LIST_MAP_HINTS,
		 _
		 ("List 'hints.xml' entries. These parameters enable you to modify the behavior of the map loader."));
  POPULATE_VOID (LW6DEF_LIST_MAP_STYLE,
		 _
		 ("List 'style.xml' entries. These parameters enable you to modify the aspect of the game."));
  POPULATE_VOID (LW6DEF_LIST_FUNCS,
		 _
		 ("List the C-functions which are exported to Guile, thus usable in scripts."));
  POPULATE_VOID (LW6DEF_LIST_HOOKS, _("List user-modifiable hooks."));
  POPULATE_VOID (LW6DEF_LIST_TUNING,
		 _
		 ("List advanced options which can be used for fine-tuning the game."));
  POPULATE_VOID (LW6DEF_LIST_ALIASES,
		 _
		 ("List the keyword aliases. These are here for convenience."));
  POPULATE_VOID (LW6DEF_EXAMPLE_RULES_XML,
		 _
		 ("Dumps on stdout an example options.xml file. Such a file is normally shipped with the game. It is indeed generated using this command."));
  POPULATE_VOID (LW6DEF_EXAMPLE_HINTS_XML,
		 _
		 ("Dumps on stdout an example hints.xml file. Such a file is normally shipped with the game. It is indeed generated using this command."));
  POPULATE_VOID (LW6DEF_EXAMPLE_STYLE_XML,
		 _
		 ("Dumps on stdout an example style.xml file. Such a file is normally shipped with the game. It is indeed generated using this command."));
  /*
   * Build values
   */
  POPULATE_VOID (LW6DEF_SHOW_BUILD_PACKAGE_TARNAME,
		 _("Shows the package tarname, that is, liquidwar6."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_PACKAGE_NAME,
		 _("Shows the package name, that is, 'Liquid War 6'."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_PACKAGE_STRING,
		 _
		 ("Shows the package string, that is, 'Liquid War 6 <version>"));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_VERSION,
		 _
		 ("Shows the version. Note that this is different from the standard GNU 'version' command line option which shows a complete message with a short copyright notice. This one will just return the version, without the package tarname or anything else."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_CODENAME,
		 _
		 ("Shows the codename associated with this version, generally the name of someone famous who is war-related (a general, an emperor...)."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_STAMP,
		 _
		 ("Shows the build stamp. A very usefull value, more precise than the version to track down binaries. It is incremented each time the core C code is updated. It won't reflect all the programs for it does not take scripts in account, but if you are running a work-in-progress version, it might be very convenient to use this to know what your are running exactly."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_MD5SUM,
		 _
		 ("Shows the MD5 checksum, which has been calculated from the C source files. Complementary with 'show-build-stamp'."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_COPYRIGHT,
		 _("Shows a very short copyright notice."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_LICENSE,
		 _
		 ("Shows the license of the program (GNU GPL v3 or later)."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_URL,
		 _("Shows the URL of the program, its homepage."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_CONFIGURE_ARGS,
		 _
		 ("Shows the arguments that have been passed to the GNU Autoconf './configure' script when building the program. This can be very usefull if you want to know how the program has been built."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_GCC_VERSION,
		 _
		 ("Returns the version of the GNU C compiler which was used to compile the program."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_CFLAGS,
		 _
		 ("Shows what value you should put in 'CFLAGS' (environment variable) if you want to compile programs that use Liquid War 6 as a library, and include 'liquidwar6.h'."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_LDFLAGS,
		 _
		 ("Shows what value you should put in 'LDFLAGS' (environment variable) if you want to link programs against libliquidwar6."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_HOSTNAME,
		 _
		 ("Shows the name of the host where the binary was compiled."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_DATE,
		 _("Shows the date when the binary was compiled."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_TIME,
		 _("Shows the time when the binary was compiled."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_HOST_CPU,
		 _
		 ("Shows the host CPU, as defined by 'host_cpu' in GNU Autoconf."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_ENDIANNESS,
		 _
		 ("Returns the endianness. 'little' corresponds to x86-like systems, 'big' to ppc-like systems."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_POINTER_SIZE,
		 _
		 ("Returns the pointer size, in bytes. Should be 4 on 32-bit systems and 8 on 64-bit systems."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_X86,
		 _("Tells wether the CPU belongs to x86 family."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_HOST_OS,
		 _
		 ("Shows the host OS, as defined by 'host_os' in GNU Autoconf."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_MS_WINDOWS,
		 _
		 ("Returns 1 (true) if host OS is Microsoft Windows 32-bit platform, 0 (false) if not."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_MAC_OS_X,
		 _
		 ("Returns 1 (true) if host OS is Mac OS X, 0 (false) if not."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_GP2X,
		 _("Returns 1 (true) if host is a GP2X, 0 (false) if not."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_TOP_SRCDIR,
		 _
		 ("Shows the top source directory on the machine where the binary was compiled."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_PREFIX,
		 _
		 ("Shows the 'prefix' value as passed to the GNU Autoconf './configure' script when compiling the program. Default is '/usr/local'."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_DATADIR,
		 _
		 ("Shows the 'datadir' value as passed to the GNU Autoconf './configure' script when compiling the program. Default is '/usr/local/share'. This is the generic, non Liquid War 6 specific data directory. Liquid War 6 related data is stored elsewhere (usually in a sub-directory) see the 'data-dir' switch for more information. 'datadir' is not 'data-dir'. That's the point."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_LIBDIR,
		 _
		 ("Shows the 'libdir' value as passed to the GNU Autoconf './configure' script when compiling the program. Default is '/usr/local/lib'. This is the generic, non Liquid War 6 specific library directory. Dedicated Liquid War 6 modules are stored elsewhere (usually in a sub-directory) see the 'mod-dir' switch for more information."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_INCLUDEDIR,
		 _
		 ("Shows the 'includedir' value as passed to the GNU Autoconf './configure' script when compiling the program. Default is '/usr/local/include'."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_LOCALEDIR,
		 _
		 ("Shows the 'localedir' value as passed to the GNU Autoconf './configure' script when compiling the program. Default is '/usr/local/share/locale'."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_DOCDIR,
		 _
		 ("Shows the 'docdir' value as passed to the GNU Autoconf './configure' script when compiling the program. Default is '/usr/local/share/doc/liquidwar6'."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_ENABLE_CONSOLE,
		 _
		 ("Shows wether the console has been enabled when building the game. This depends on parameters passed to './configure' and also on the presence of ncurses and readline."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_ENABLE_GTK,
		 _
		 ("Shows wether GTK+ support has been enabled when building the game. This depends on parameters passed to './configure' and also on the presence of GTK+ headers and libs. It uses pkg-config to detect it."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_ENABLE_MOD_GL,
		 _
		 ("Shows wether the mod-gl graphical backend has been enabled when building the game. This depends on parameters passed to './configure' and also on the presence of SDL and related libraries."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_ENABLE_MOD_CSOUND,
		 _
		 ("Shows wether the mod-csound audio backend has been enabled when building the game. This depends on parameters passed to './configure' and also on the presence of the csound library."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_ENABLE_MOD_OGG,
		 _
		 ("Shows wether the mod-ogg audio backend has been enabled when building the game. This depends on parameters passed to './configure' and also on the presence of SDL and related libraries."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_ENABLE_MOD_HTTP,
		 _
		 ("Shows wether the mod-http network backend has been enabled when building the game. This depends on parameters passed to './configure' and also on the presence of libCurl."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_ENABLE_OPTIMIZE,
		 _
		 ("Shows wether the 'optimize' option has been chosen when building the game. This depends on parameters passed to './configure'."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_ENABLE_ALLINONE,
		 _
		 ("Shows wether the 'allinone' option has been chosen when building the game. This depends on parameters passed to './configure'."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_ENABLE_FULLSTATIC,
		 _
		 ("Shows wether the 'fullstatic' option has been chosen when building the game. This depends on parameters passed to './configure'."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_ENABLE_PARANOID,
		 _
		 ("Shows wether the game was build with paranoid memory management. This is for debugging purposes, the default already includes some controls, with turned it's really... paranoid."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_ENABLE_GPROF,
		 _
		 ("Shows wether the game was build with suitable informations for gprof. This depends on parameters passed to './configure'."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_ENABLE_INSTRUMENT,
		 _
		 ("Shows wether the game was build with the '-finstrument-functions' GCC switch. This depends on parameters passed to './configure'."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_ENABLE_PROFILER,
		 _
		 ("Shows wether the game was build with Google Profiler support. This depends on parameters passed to './configure'."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_ENABLE_GCOV,
		 _
		 ("Shows wether the game was build with suitable informations for gcov. This depends on parameters passed to './configure'."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_ENABLE_VALGRIND,
		 _
		 ("Shows wether the game was build with valgrind later use in mind. This depends on parameters passed to './configure'."));
  POPULATE_VOID (LW6DEF_SHOW_BUILD_BIN_ID,
		 _
		 ("Shows the internal 'bin-id' value. This value does not mean anything in itself but it's supposed to change each time you compile the game."));
  /*
   * Show default paths
   */
  POPULATE_VOID (LW6DEF_SHOW_DEFAULT_USER_DIR,
		 _
		 ("Shows the default user directory path. This is where run-time data, config files, log files, are stored. Default is '$HOME/.liquidwar6/'."));
  POPULATE_VOID (LW6DEF_SHOW_DEFAULT_CONFIG_FILE,
		 _
		 ("Shows the default config file path. Default is '$HOME/.liquidwar6/config.xml'."));
  POPULATE_VOID (LW6DEF_SHOW_DEFAULT_LOG_FILE,
		 _
		 ("Shows the default log file path. Default is '$HOME/.liquidwar6/log.csv'."));
  POPULATE_VOID (LW6DEF_SHOW_DEFAULT_PREFIX,
		 _
		 ("Shows the default prefix used. This should logically be the value passed to the GNU Autoconf ./configure script when building the game. Most other path are deduced from this one. Default is '/usr/local'."));
  POPULATE_VOID (LW6DEF_SHOW_DEFAULT_MOD_DIR,
		 _
		 ("Shows the default module directory path. This is where all dynamically loaded modules are stored. Default is '/usr/local/lib/liquidwar6-<version>'."));
  POPULATE_VOID (LW6DEF_SHOW_DEFAULT_DATA_DIR,
		 _
		 ("Shows the default data directory path. This is where the games searches for most of its data,the most important exception being maps, which are stored elsewhere. Default is '/usr/local/share/liquidwar6-<version>/data'."));
  POPULATE_VOID (LW6DEF_SHOW_DEFAULT_MUSIC_DIR,
		 _
		 ("Shows the default music directory. This is where builtin musics are stored. Default is '/usr/local/share/liquidwar6-<version>/music'."));
  POPULATE_VOID (LW6DEF_SHOW_DEFAULT_MUSIC_PATH,
		 _
		 ("Shows the default music search path. This is where the game searches for musics. It's the combination of command-line arguments and builtin paths. Might return more directories than the one specified in a single 'music-path=dir1:dir2' argument."));
  POPULATE_VOID (LW6DEF_SHOW_DEFAULT_MAP_DIR,
		 _
		 ("Shows the default map directory. This is where builtin maps are stored. Default is '/usr/local/share/liquidwar6-<version>/map'."));
  POPULATE_VOID (LW6DEF_SHOW_DEFAULT_MAP_PATH,
		 _
		 ("Shows the default map search path. This is where the game searches for maps. It's the combination of command-line arguments and builtin paths. Might return more directories than the one specified in a single 'map-path=dir1:dir2' argument."));
  POPULATE_VOID (LW6DEF_SHOW_DEFAULT_SCRIPT_FILE,
		 _
		 ("Shows the default main script file path. This file is very important, since the program is more or less a hudge scheme interpreter, and this file is the file loaded by Guile. In short, it is the main program. Default is '/usr/local/share/liquidwar6-<version>/script/liquidwar6.scm'."));
  /*
   * Show paths
   */
  POPULATE_VOID (LW6DEF_SHOW_CWD,
		 _
		 ("Shows the current working directory, the value that the pwd command would return."));
  POPULATE_VOID (LW6DEF_SHOW_RUN_DIR,
		 _
		 ("Shows the run directory, usually the path where the binary is. It depends on how and where the program is launched. It is guessed from the argc/argv values at runtime."));
  POPULATE_VOID (LW6DEF_SHOW_USER_DIR,
		 _
		 ("Shows the user directory path. This is where run-time data, config files, log files, are stored. Default is '$HOME/.liquidwar6/'."));
  POPULATE_VOID (LW6DEF_SHOW_CONFIG_FILE,
		 _
		 ("Shows the config file path. Default is '$HOME/.liquidwar6/config.xml'."));
  POPULATE_VOID (LW6DEF_SHOW_LOG_FILE,
		 _
		 ("Shows the log file path. Default is '$HOME/.liquidwar6/log.csv'."));
  POPULATE_VOID (LW6DEF_SHOW_PREFIX,
		 _
		 ("Shows the prefix used. This should logically be the value passed to the GNU Autoconf ./configure script when building the game. Most other path are deduced from this one. Default is '/usr/local'."));
  POPULATE_VOID (LW6DEF_SHOW_MOD_DIR,
		 _
		 ("Shows the module directory path. This is where all dynamically loaded modules are stored. Default is '/usr/local/lib/liquidwar6-<version>'."));
  POPULATE_VOID (LW6DEF_SHOW_DATA_DIR,
		 _
		 ("Shows the data directory path. This is where the games searches for most of its data,the most important exception being maps, which are stored elsewhere. Default is '/usr/local/share/liquidwar6-<version>/data'."));
  POPULATE_VOID (LW6DEF_SHOW_MUSIC_DIR,
		 _
		 ("Shows the music directory. This is where builtin maps are stored. Default is '/usr/local/share/liquidwar6-<version>/music'."));
  POPULATE_VOID (LW6DEF_SHOW_MUSIC_PATH,
		 _
		 ("Shows the music search path. This is where the game searches for musics. It's the combination of command-line arguments and builtin paths. Might return more directories than the one specified in a single 'music-path=dir1:dir2' argument."));
  POPULATE_VOID (LW6DEF_SHOW_MAP_DIR,
		 _
		 ("Shows the map directory. This is where builtin maps are stored. Default is '/usr/local/share/liquidwar6-<version>/map'."));
  POPULATE_VOID (LW6DEF_SHOW_MAP_PATH,
		 _
		 ("Shows the map search path. This is where the game searches for maps. It's the combination of command-line arguments and builtin paths. Might return more directories than the one specified in a single 'map-path=dir1:dir2' argument."));
  POPULATE_VOID (LW6DEF_SHOW_SCRIPT_FILE,
		 _
		 ("Shows the main script file path. This file is very important, since the program is more or less a hudge scheme interpreter, and this file is the file loaded by Guile. In short, it is the main program. Default is '/usr/local/share/liquidwar6-<version>/script/liquidwar6.scm'."));
  /*
   * Paths
   */
  POPULATE_STR (LW6DEF_USER_DIR,
		_
		("Set the user directory path. This is where run-time data, config files, log files, are stored. If you override this value, other parameters such as where the config and log files reside, will change."),
		"$HOME/.liquidwar6");
  POPULATE_STR (LW6DEF_CONFIG_FILE,
		_
		("Set the config file path. This enables you to use whatever config file you like, keeping all other informations in the same place."),
		"$HOME/.liquidwar6/config.xml");
  POPULATE_STR (LW6DEF_LOG_FILE,
		_
		("Set the log file path. This enables you to use whatever log file you like, keeping all other informations in the same place."),
		"$HOME/.liquidwar6/log.csv");
  POPULATE_STR (LW6DEF_PREFIX,
		_
		("Override the prefix value given to the GNU Autoconf ./configure script when building the game. Not all path will be changed, some of them might remain the same, for instance message translations (localedir). But most game-specific data including maps, graphics, sounds, will be searched according to the new given parameter."),
		"/usr/local");
  POPULATE_STR (LW6DEF_MOD_DIR,
		_
		("Set the module directory path. By changing this you will load dynamic shared libraries (game specific modules such as the graphical backend) from an alternative place. Use this at your own risks, for there can always be a binary incompatibility. You've been warned."),
		"/usr/local/lib/liquidwar6-<version>");
  POPULATE_STR (LW6DEF_DATA_DIR,
		_
		("Set the data directory. By changing ths value you'll be able to use an alternative data directory."),
		"/usr/local/share/liquidwar6-<version>/data");
  POPULATE_STR (LW6DEF_MUSIC_DIR,
		_
		("Set the music directory path. By changing this value you'll be able to use your own musics in your own directory. Note that there are other ways to achieve that, but using this option will work. The major side effect is that using this option, you really replace the existing builtin musics by your own. If you simply want to add musics you can store them in $HOME/.liquidwar6/music or in the map directory itself."),
		"/usr/local/share/liquidwar6-<version>/music");
  POPULATE_STR (LW6DEF_MUSIC_PATH,
		_
		("Set the music search path. By changing this value you'll be able to play with your own musics in your own directory. This is different from 'music-dir', since it includes 'music-dir', plus it adds a number of other search paths. Unlike most other parameters, the values given from the command-line, from the environment variables, or from the config file, are not overwritten, but appended. That is to say if you specify a 'music-path' with the command-line argument 'music-path=path', but also define the 'LW6_MUSIC_PATH' value and finally edit 'config.xml' to change the 'music-path' entry in it, you'll end up with the game searching for musics in all these directories. Additionnally, 'music-dir' and '<user-dir>/music' will always be in the list. Any given value can itself include several pathes, separated by the path separator. This separator is ':' on GNU/Linux, and ';' on Microsoft Windows. For instance, on a GNU/Linux box, you could use the command-line argument 'music-path=/foo/bar/music:/home/user/music/:/music'."),
		"$HOME/.liquidwar6/music:/usr/local/share/liquidwar6-<version>/music");
  POPULATE_STR (LW6DEF_MAP_DIR,
		_
		("Set the map directory path. By changing this value you'll be able to play with your own maps in your own directory. Note that there are other ways to achieve that, but using this option will work. However, a side effect is that you might not see builtin maps anymore."),
		"/usr/local/share/liquidwar6-<version>/map");
  POPULATE_STR (LW6DEF_MAP_PATH,
		_
		("Set the map search path. By changing this value you'll be able to play with your own maps in your own directory. This is different from 'map-dir', since it includes 'map-dir', plus it adds a number of other search paths. Unlike most other parameters, the values given from the command-line, from the environment variables, or from the config file, are not overwritten, but appended. That is to say if you specify a 'map-path' with the command-line argument 'map-path=path', but also define the 'LW6_MAP_PATH' value and finally edit 'config.xml' to change the 'map-path' entry in it, you'll end up with the game searching for maps in all these directories. Additionnally, 'map-dir' and '<user-dir>/map' will always be in the list. Any given value can itself include several pathes, separated by the path separator. This separator is ':' on GNU/Linux, and ';' on Microsoft Windows. For instance, on a GNU/Linux box, you could use the command-line argument 'map-path=/foo/bar/map:/home/user/map/:/map'."),
		"$HOME/.liquidwar6/map:/usr/local/share/liquidwar6-<version>/map");
  POPULATE_STR (LW6DEF_SCRIPT_FILE,
		_
		("Set the main script file path. This file is very important, since the program is more or less a hudge scheme interpreter, and this file is the file loaded by Guile. In short, it is the main program."),
		"/usr/local/share/liquidwar6-<version>/script/liquidwar6.scm");
  /*
   * Players
   */
  POPULATE_STR (LW6DEF_PLAYER1_NAME,
		_
		("Name of the first player, the player used by default. A default value is provided, you can of course, change it at will."),
		"<username>");
  POPULATE_STR (LW6DEF_PLAYER2_NAME,
		_
		("Name of the second player. A default value is provided, you'll certainly want to change it."),
		"player2@<hostname>");
  POPULATE_STR (LW6DEF_PLAYER3_NAME,
		_
		("Name of the third player. A default value is provided, you'll certainly want to change it."),
		"player3@<hostname>");
  POPULATE_STR (LW6DEF_PLAYER4_NAME,
		_
		("Name of the fourth player. A default value is provided, you'll certainly want to change it."),
		"player4@<hostname>");
  POPULATE_BOOL (LW6DEF_PLAYER1_STATUS,
		 _
		 ("Status of the first player, true if player is activated, false if idle."),
		 1);
  POPULATE_BOOL (LW6DEF_PLAYER2_STATUS,
		 _
		 ("Status of the second player, true if player is activated, false if idle."),
		 1);
  POPULATE_BOOL (LW6DEF_PLAYER3_STATUS,
		 _
		 ("Status of the third player, true if player is activated, false if idle."),
		 0);
  POPULATE_BOOL (LW6DEF_PLAYER4_STATUS,
		 _
		 ("Status of the fourth player, true if player is activated, false if idle."),
		 0);
  POPULATE_STR (LW6DEF_PLAYER1_CONTROL,
		_
		("Control for the first player, must be mouse, keyboard, joystick1, joystick2 or custom."),
		"mouse");
  POPULATE_STR (LW6DEF_PLAYER2_CONTROL,
		_
		("Control for the second player, must be mouse, keyboard, joystick1, joystick2 or custom."),
		"keyboard");
  POPULATE_STR (LW6DEF_PLAYER3_CONTROL,
		_
		("Control for the third player, must be mouse, keyboard, joystick1, joystick2 or custom."),
		"joystick1");
  POPULATE_STR (LW6DEF_PLAYER4_CONTROL,
		_
		("Control for the fourth player, must be mouse, keyboard, joystick1, joystick2 or custom."),
		"joystick2");
  POPULATE_STR (LW6DEF_PLAYER1_BOT,
		_
		("Bot engine for the first player, can be 'no', 'random' or 'idiot'."),
		"");
  POPULATE_STR (LW6DEF_PLAYER2_BOT,
		_
		("Bot engine for the second player, can be 'no', 'random' or 'idiot'."),
		"");
  POPULATE_STR (LW6DEF_PLAYER3_BOT,
		_
		("Bot engine for the third player, can be 'no', 'random' or 'idiot'."),
		"idiot");
  POPULATE_STR (LW6DEF_PLAYER4_BOT,
		_
		("Bot engine for the fourth player, can be 'no', 'random' or 'idiot'."),
		"random");
  POPULATE_STR (LW6DEF_PLAYER1_COLOR,
		_
		("Color of the first player, must be red, green, blue, yellow, cyan, magenta, orange, lightblue, purple or pink"),
		"red");
  POPULATE_STR (LW6DEF_PLAYER2_COLOR,
		_
		("Color of the second player, must be red, green, blue, yellow, cyan, magenta, orange, lightblue, purple or pink"),
		"green");
  POPULATE_STR (LW6DEF_PLAYER3_COLOR,
		_
		("Color of the third player, must be red, green, blue, yellow, cyan, magenta, orange, lightblue, purple or pink"),
		"blue");
  POPULATE_STR (LW6DEF_PLAYER4_COLOR,
		_
		("Color of the fourth player, must be red, green, blue, yellow, cyan, magenta, orange, lightblue, purple or pink"),
		"yellow");
  /*
   * Input
   */
  POPULATE_FLOAT (LW6DEF_MOUSE_SENSITIVITY,
		  _
		  ("Mouse sensitivity, 1.0 is the default, 0.1 is slow, 10 is reponsive. This is used for moving the cursor during the game only, the option has no impact on menu navigation."),
		  1.0, 0, 0);
  POPULATE_FLOAT (LW6DEF_CURSOR_SENSITIVITY,
		  _
		  ("Keyboard and joystick sensitivity while moving the cursor. 1.0 is the default, 0.1 is slow, 10 is reponsive. This is used for moving the cursor during the game only, the option has no impact on menu navigation."),
		  1.0, 0, 0);
  POPULATE_FLOAT (LW6DEF_MAX_CURSOR_SPEED,
		  _
		  ("Maximum cursor speed when cursor is controlled with keyboard or joystick joystick 1. Consider using cursor-sensitivity too."),
		  10.0, 0, 0);
  POPULATE_INT (LW6DEF_REPEAT_DELAY,
		_("Time, in milliseconds, before key repeat will start."),
		500, 0, 0);
  POPULATE_INT (LW6DEF_REPEAT_INTERVAL,
		_
		("Time, in milliseconds, between two repeats, once repeat has started."),
		100, 0, 0);
  POPULATE_STR (LW6DEF_CUSTOM_UP,
		_("Custom keycode to be used as the UP key equivalent."),
		"(c-lw6gui-keyboard-is-pressed 101) ; SDLK_e");
  POPULATE_STR (LW6DEF_CUSTOM_DOWN,
		_("Guile custom code associated to the DOWN key equivalent."),
		"(c-lw6gui-keyboard-is-pressed 100) ; SDLK_d");
  POPULATE_STR (LW6DEF_CUSTOM_LEFT,
		_("Guile custom code associated to the LEFT key equivalent."),
		"(c-lw6gui-keyboard-is-pressed 99) ; SDLK_c");
  POPULATE_STR (LW6DEF_CUSTOM_RIGHT,
		_
		("Guile custom code associated to the RIGHT key equivalent."),
		"(c-lw6gui-keyboard-is-pressed 118) ; SDLK_v");
  POPULATE_STR (LW6DEF_CUSTOM_ESC,
		_("Guile custom code associated to the ESC key equivalent."),
		"(c-lw6gui-keyboard-is-pressed 102) ; SDLK_f");
  POPULATE_STR (LW6DEF_CUSTOM_ENTER,
		_
		("Guile custom code associated to the ENTER key equivalent."),
		"(c-lw6gui-keyboard-is-pressed 104) ; SDLK_h");
  POPULATE_STR (LW6DEF_CUSTOM_PGUP,
		_("Guile custom code associated to the PGUP key equivalent."),
		"(c-lw6gui-keyboard-is-pressed 98) ; SDLK_b");
  POPULATE_STR (LW6DEF_CUSTOM_PGDOWN,
		_
		("Guile custom code associated to the PGDOWN key equivalent."),
		"(c-lw6gui-keyboard-is-pressed 110) ; SDLK_n");
  POPULATE_BOOL (LW6DEF_CLICK_TO_FOCUS,
		 _
		 ("If set to true, you'll need to click with the mouse to select a menuitem or move the cursor in the game. If not, some actions will be taken automatically without the need to click."),
		 0);
  POPULATE_BOOL (LW6DEF_USE_ESC_BUTTON,
		 _
		 ("Decides wether to display an 'esc' (escape) button in the interface. This is usefull for people who control the game with the mouse only, and have a single buttons, or on a touchscreen."),
		 1);
  POPULATE_FLOAT (LW6DEF_ZOOM_STEP,
		  _
		  ("A value, strictly greater than 1, which will be used when zooming. The greater it is, the more sensible the zoom is."),
		  1.1, 0, 0);
  POPULATE_FLOAT (LW6DEF_ZOOM_STICK_DELAY,
		  _
		  ("How long, in msec, the zoom will stick to its default value."),
		  1000, 0, 0);
  /*
   * Graphics
   */
  POPULATE_INT (LW6DEF_WIDTH,
		_
		("Run the game with the given screen width. Note that the graphics backend might ignore this hint. Use with its companion option 'height'.A negative value will force the use of a default value."),
		-1, 0, 0);;
  POPULATE_INT (LW6DEF_HEIGHT,
		_
		("Run the game with the given screen height.Note that the graphics backend might ignore this hint. Use with its companion option 'width'. A negative value will force the use of a default value."),
		-1, 0, 0);
  POPULATE_BOOL (LW6DEF_FULLSCREEN,
		 _
		 ("Force the game to fun fullscreen. Note that the graphics backend might ignore this hint."),
		 LW6GUI_DEFAULT_FULLSCREEN);
  POPULATE_STR (LW6DEF_GFX_BACKEND,
		_
		("Sets the graphics backend AKA 'gfx' to use. For now the only choice is 'gl' and will use an OpenGL/SDL 3D-accelerated driver."),
		"gl");
  POPULATE_INT (LW6DEF_GFX_QUALITY,
		_
		("Sets the overall quality of the graphics backend. Depending on the backend, this can mean different things. For instance for the 'gl' backend, this can change texture filtering (nearest, linear, bilinear...). This is not the same as 'pixelize' which is a per-map option and emulates an old school appearance."),
		LW6GUI_GFX_QUALITY_STANDARD, LW6GUI_GFX_QUALITY_LOW,
		LW6GUI_GFX_QUALITY_HIGH);
  POPULATE_FLOAT (LW6DEF_WINDOWED_MODE_LIMIT,
		  _
		  ("When switching back from fullscreen mode to windowed mode, if we're in maximum resolution, then this coefficient will be applied before resizing the window. The idea is that (obviously) a windowed mode is prefered when a little smaller that totally fullscreen. So set this to a value just below 1.0."),
		  0.95, 0, 0);
  POPULATE_BOOL (LW6DEF_CAPTURE,
		 _
		 ("Enables capture mode, in which a BMP file is dumped on the disk (in your user directory, search for a 'capture' sub-directory)."),
		 0);
  /*
   * Sound
   */
  POPULATE_FLOAT (LW6DEF_SOUND_VOLUME,
		  _
		  ("Set the sound volume.This is a floating point value. 0 is mute. Maximum value is 1."),
		  0.3, 0, 1);
  POPULATE_FLOAT (LW6DEF_MUSIC_VOLUME,
		  _
		  (" Set the music volume.This is a floating point value. 0 is mute. Maximum value is 1."),
		  0.6, 0, 1);
  POPULATE_STR (LW6DEF_SND_BACKEND,
		_
		("Sets the sound backend AKA 'snd' to use. Can be 'ogg' or 'csound' but only 'ogg' will produce sound in the current release."),
		"ogg");
  POPULATE_STR (LW6DEF_AMBIANCE_FILE,
		_
		("A music file which will be used to be played during the menus. If not found, game will fallback on random files."),
		"");
  POPULATE_STR (LW6DEF_AMBIANCE_FILTER,
		_
		("A music filter, used to select the files which are played while navigating in the menus. It works like 'music-filter' except this one is not related to a peculiar map. This is not a complex regex-enabled filter, just a plain string search. Even the '*' wildcard won't work."),
		"Chadburn");
  POPULATE_STR (LW6DEF_AMBIANCE_EXCLUDE,
		_
		("If this string is present in a music file name, this file won't be played during the menus, it will be excluded from the list."),
		"");

  /*
   * Network
   */
  POPULATE_BOOL (LW6DEF_SKIP_NETWORK,
		 _
		 ("If set, then game won't do anything network related. No listen, no connect, no nothing. You are playing locally."),
		 1);
  POPULATE_STR (LW6DEF_PASSWORD,
		_
		("The password to use for network games. Do not use a valuable password, as this is stored as clear text on your hard drive. Still, the game will only send a hash/checksum of the password on the network so eavesdropper won't be able to read it. They can see the hash/checksum and use it if clever, but they can't guess the real password. A blank password means anyone can join your games when you act like a server."),
		"");
  POPULATE_STR (LW6DEF_BIND_IP,
		_
		("The IP address to bind on when listening to network messages. You can use this to specifically use a given network interface, the default will listen on any available interface."),
		LW6NET_ADDRESS_ANY);
  POPULATE_INT (LW6DEF_BIND_PORT,
		_
		("The IP port to bind on when listening to network messages. The default should work out of the box, and will ease up the discovery process. That is, if you use your own settings, automatic detection of your server by other servers might not work so well."),
		LW6NET_DEFAULT_PORT, 1, 65535);
  POPULATE_STR (LW6DEF_CLI_BACKENDS,
		_
		("The client backends to use. Most of the time the default is fine, change it only if you specifically want to disactivate some protocol, or if you want to activate a custom-made client backend. It's a comma separated list."),
		"tcp,udp,http");
  POPULATE_STR (LW6DEF_SRV_BACKENDS,
		_
		("The server backends to use. Most of the time the default is fine, change it only if you specifically want to disactivate some protocol, or if you want to activate a custom-made server backend. It's a comma separated list."),
		"tcpd,udpd,httpd");
  POPULATE_STR (LW6DEF_PUBLIC_URL,
		_
		("The public URL of your server. By default the game will pick up one for you. In fact, the clients discovering your server should guess the public URL, probably http://<your-ip>:<your-port>/ but you might need to use your own settings if you are using NAT or an Apache reverse-proxy to rewrite HTTP requests."),
		"");
  POPULATE_STR (LW6DEF_NODE_TITLE,
		_
		("The title of your node, that is the name which will be displayed when listing servers. This is different from player name, for there can be several players on a single computer. By default this will be set to hostname."),
		"");
  POPULATE_STR (LW6DEF_NODE_DESCRIPTION,
		_
		("The description of your node, that is a text that describes your server. This will typically appear when pointing a web client on the public server URL, it is for general information, so if there's something special about your server, say it here."),
		_("No description."));
  POPULATE_STR (LW6DEF_KNOWN_NODES,
		_
		("List of known nodes, nodes which the program will try to contact first to get the list of other nodes. This is mostly usefull when program is launched for the first time, after this it should keep an up-to-date list of known servers in its internal database and automatically reconnect to them next time it starts. You might want to change this if you really want to connect to a given server which is not publically listed. The list is comma separated."),
		_("http://ufoot.org:8056/,http://ufoot.hd.free.fr:8056/"));

  /*
   * Tuning
   */
  POPULATE_VOID (LW6DEF_RESET,
		 _
		 ("Clears the config file so that the game will run with defaults next time. The idea is to get rid of traces of previous executions. The difference with '--defaults' is that '--reset' does not run the game, while '--defaults' does."));
  POPULATE_BOOL (LW6DEF_RESET_CONFIG_ON_UPGRADE,
		 _
		 ("If set, then a reset (config file set to defaults) is run every time you upgrade the game."),
		 1);
  POPULATE_INT (LW6DEF_LOG_LEVEL,
		_
		("Defines the log level, that is, how verbose the program will be regarding logs and console output. 0 (ERROR) is the minimum, only errors are reported. 1 (WARNING) means errors + warnings. 2 (NOTICE) displays most important messages. 3 (INFO) is the default, the log file will contain all messages but debug stuff. 4 (DEBUG) logs everything, including debug informations."),
		LW6SYS_LOG_DEFAULT_ID, LW6SYS_LOG_ERROR_ID,
		LW6SYS_LOG_DEBUG_ID);
  POPULATE_INT (LW6DEF_TARGET_FPS,
		_
		("Defines how many frames will be displayed per second. Of course this is a maximum value, if your hardware can't keep up with this value, display will just be slow, no matter what value you define here. Note that you might really wish to have something rather low here, to keep network and 'logic' function responsiveness. Passed 60 frames per second, speed is really only for visual comfort, as Liquid War 6 is now so fast-paced that it requires 200 frames/sec to outperform opponents."),
		60, 0, 0);
  POPULATE_INT (LW6DEF_IO_PER_SEC,
		_
		("Defines the number of calls to input/output functions per second. This can affect speed of menus but also cursors, but won't change the speed of the game itself. It's a cosmectic, comfort option."),
		15, 0, 0);
  POPULATE_INT (LW6DEF_COMMANDS_PER_SEC,
		_
		("Defines the number of commands per second. When a command is generated, orders are actually sent to the game engine, for instance, 'this cursor moved there'. So this option will affect game responsiveness, setting this to a high value will make the game more responsive but consume bandwidth on network games."),
		5, 0, 0);
  POPULATE_FLOAT (LW6DEF_LOADER_SLEEP,
		  _
		  ("Defines how long the loader thread should wait between two polls. Default value should fit in most cases."),
		  0.5, 0, 0);
  POPULATE_INT (LW6DEF_PILOT_LAG,
		_
		("Maximum lag, in rounds, until the game engine is slowed down. This will typically be usefull if your computer is too slow for the map resolution and the game speed you set up."),
		10, 0, 0);
  POPULATE_INT (LW6DEF_MEMORY_BAZOOKA_SIZE,
		_
		("The memory bazooka is a brute-force tool, conceived after a full night spent tracking some memory leak. The idea is to keep a track of all allocated pointers, when the data was allocated (timestamp), where in the code (file, line), and even point out what data there is in that place. A memory bazooka report at the end of the game will just show what's left. There should be nothing. This parameter is here to avoid wasting CPU cycles on a feature which is very debug-oriented and does not really make sense for the casual user. Set it to 0 for best performance, something like 100 might just be helpfull, but 1000000 is the right way to seriously debug code."),
		LW6SYS_BAZOOKA_DEFAULT_SIZE, 0, 0);
  POPULATE_BOOL (LW6DEF_MEMORY_BAZOOKA_ERASER,
		 _
		 ("The memory eraser is a tool which will systematically fill allocated memory with 'M', and overwrite all allocated bytes with 'F' before freeing memory. It will even handle realloc calls. This is usefull to track bugs. Indeed, with this option enabled, freshly allocated memory will never contain zeroes unless one calls calloc, and if you ever free some memory zone before being done with it, it will be filled with junk and therefore not be usable. The memory bazooka must be big enough if you want this feature to actually work."),
		 1);
  POPULATE_BOOL (LW6DEF_DISPLAY_SPLASH,
		 _
		 ("Set this to 'false' to disable the display of the splash screen at game startup."),
		 1);
  POPULATE_BOOL (LW6DEF_DISPLAY_BACKGROUND,
		 _
		 ("Decides wether the background animation/image should be displayed at all."),
		 1);
  POPULATE_BOOL (LW6DEF_DISPLAY_PREVIEW,
		 _
		 ("Decides wether a map preview should be displayed when choosing a level."),
		 1);
  POPULATE_BOOL (LW6DEF_DISPLAY_MAP,
		 _
		 ("Debugging option which can be set to 'false' to disable map (level) display when playing."),
		 1);
  POPULATE_BOOL (LW6DEF_DISPLAY_FIGHTERS,
		 _
		 ("Debugging option which can be set to 'false' to disable the display of fighters when playing."),
		 1);
  POPULATE_BOOL (LW6DEF_DISPLAY_CURSORS,
		 _
		 ("Debugging option which can be set to 'false' to disable the display of cursors when playing."),
		 1);
  POPULATE_BOOL (LW6DEF_DISPLAY_HUD,
		 _
		 ("Decides wether the hud (informations while playing) should be displayed."),
		 1);
  POPULATE_BOOL (LW6DEF_DISPLAY_SCORE,
		 _("Decides wether the score screen should be displayed."),
		 1);
  POPULATE_BOOL (LW6DEF_DISPLAY_MENU,
		 _
		 ("Debugging option which can be set to 'false' to disable the display of menus."),
		 1);
  POPULATE_BOOL (LW6DEF_DISPLAY_PROGRESS,
		 _
		 ("Decides wether a progress bar should be displayed when a long operation is realized as a background task."),
		 1);
  POPULATE_BOOL (LW6DEF_DISPLAY_LOG,
		 _
		 ("Set this to 'false' to disable the display of error messages on the screen. Mote that you can miss valuable informations."),
		 1);
  POPULATE_BOOL (LW6DEF_DISPLAY_FPS,
		 _
		 ("Set this to 'true' to display the number of frames per second. When this gets too low... play a smaller map, buy a new computer or contribute and hack Liquid War 6 so that it runs faster!"),
		 0);
  POPULATE_BOOL (LW6DEF_DISPLAY_MPS,
		 _
		 ("Set this to 'true' to display the number of moves per second. In theory the game should maintain this constant but in practise it can get low if your computer is too slow or too busy."),
		 0);
  POPULATE_BOOL (LW6DEF_DISPLAY_URL,
		 _
		 ("Set this to 'true' to display the URL (homepage) of the game. This is mostly used when doing screenshots, so that generated images contain a link to the homepage."),
		 0);
  POPULATE_BOOL (LW6DEF_DISPLAY_MOUSE,
		 _("Set this to 'false' to always hide the mouse pointer."),
		 1);
  POPULATE_BOOL (LW6DEF_DISPLAY_DEBUG_ZONES,
		 _
		 ("Set this to 'true' to display the zones, this is usefull to debug the core algorithm or understand how it works."),
		 0);
  POPULATE_BOOL (LW6DEF_DISPLAY_DEBUG_GRADIENT,
		 _
		 ("Set this to 'true' to display the gradient, this is usefull to debug the core algorithm or understand how it works."),
		 0);
  POPULATE_BOOL (LW6DEF_DISPLAY_CONSOLE,
		 _
		 ("Defines wether the interactive system console must be displayed. Note that console support must have been enabled at compilation time. It might not be available on your computer, for instance if you are running a system such as Microsoft Windows."),
		 0);
  POPULATE_VOID (LW6DEF_SERVER,
		 _
		 ("Start the game in server mode, without requiring any graphics backend. Server mode is usefull if you just want to start a network node without hosting any real game on it. It can be used to list existing nodes and sessions or as a bounce server in case some clients can't contact each other because firewalled. If you only want to start a server game on your computer, don't use this option, just start the game normally and start a game server by clicking on the GUI buttons."));
  POPULATE_VOID (LW6DEF_QUICK_START,
		 _
		 ("Start the game just like if the player had requested a quick start, without showing any menu."));
  POPULATE_VOID (LW6DEF_DEMO,
		 _
		 ("Start the game in demo mode. 2 bots play against each other forever."));
  POPULATE_VOID (LW6DEF_BENCH,
		 _
		 ("Runs a benchmarking test which will report an approximative performance estimation of the game on your computer."));
  POPULATE_INT (LW6DEF_BENCH_VALUE,
		_
		("Contains the current bench value of the computer running the game. This is used internally to choose the right map settings. You can override this value and use your own but... use at your own risk. Pretending you have a faster computer than what you really have can lead to confusion."),
		LW6LDR_DEFAULT_BENCH_VALUE, 0, 0);
  POPULATE_INT (LW6DEF_MAGIC_NUMBER,
		_
		("This 'magic' number probably requires an explanation. It's used to estimate how big a map can be built. The calculus is very approximative, basically bench_value*magic_number=total_fighters_on_map*rounds_per_sec*moves_per_round with total_fighters_on_map depending on various parameters such as map size but also how many fighters are on the map. The map loader will try and adjust the map size so that it is just big enough not to saturate your CPU while being as high-res as possible. The magic number in itself has no real meaning, the higher it gets, the more optimized it means the game is. Normally you shouldn't change this but if you find the map resizing is too agressively pessimistic, or if for some reason bench returns bogus values, you can modify it."),
		LW6LDR_DEFAULT_MAGIC_NUMBER, 0, 0);
  POPULATE_INT (LW6DEF_BIN_ID,
		_
		("The internal 'bin-id' value. Note that this is not necessarly equal to the value returned by 'show-build-bin-id'. When they are different, it is assumed this is because of a software upgrade."),
		0, 0, 0);
  POPULATE_VOID (LW6DEF_CHECK,
		 _
		 ("Running the game with '--check' is almost like running '--test', the difference is that '--check' will not run tests which involve graphics or sound backends, so it's adapted to pure console mode. This can be usefull for automated checks on a build farm, or if you want to check things in a headless (pure console) environment."));
  POPULATE_FLOAT (LW6DEF_BOT_SPEED,
		  _
		  ("The speed of bots, 1 means normal speed, higher value will speed it up, lower will slow it down. Note that this only has an impact on bot engines, not on the game speed itself."),
		  1.0, 0, 0);
  POPULATE_INT (LW6DEF_BOT_IQ,
		_
		("The IQ (intelligence quotient) of bots. Typically, a value of 100 will make the bot behave normally, performing at its best. A value of 0 will just make it act the worst way it can. Values over 100 probably won't change anything compared to 100, but this truely depends on which bot backend you're running."),
		100, 0, 200);
  POPULATE_INT (LW6DEF_DEBUG_TEAM_ID,
		_
		("A team id which will be used for debugging purposes, for instance when displaying gradient."),
		0, 0, LW6MAP_MAX_NB_TEAMS - 1);
  POPULATE_INT (LW6DEF_DEBUG_LAYER_ID,
		_
		("A team id which will be used for debugging purposes, for instance when displaying gradient."),
		0, 0, LW6MAP_MAX_DEPTH - 1);
  POPULATE_INT (LW6DEF_LOG_TIMEOUT,
		_
		("Delay, in msec, for which a log message will stay displayed on the screen."),
		5000, 0, 0);
  POPULATE_FLOAT (LW6DEF_GFX_CPU_USAGE,
		  _
		  ("Percentage of the CPU which will be used by the display thread. It's wise to leave some time to other threads to execute. The OS does it naturally, but setting this helps the whole process by explicitely pausing (sleep call) the display thread. You could change this to a low value if you have lagging games but smooth display."),
		  0.75, 0, 1);
  POPULATE_INT (LW6DEF_DIRTY_READ,
		_
		("How to handle dirty reads and locks when displaying stuff. If set to 0, there will be no dirty reads at all, a lock (mutex) will be set whenever it's needed. If set to 1, display might be done with inconsistent data, however the data itself won't be modified while displaying. If set to 2, displayed data can (and will) be modified while the rendering thread is running."),
		LW6PIL_DIRTY_READ_ALWAYS, LW6PIL_DIRTY_READ_NEVER,
		LW6PIL_DIRTY_READ_ALWAYS);
  POPULATE_BOOL (LW6DEF_GFX_DEBUG,
		 _
		 ("Enables dedicated graphics debugging tools. This is different from 'debug' mode which is global, this one is really graphics specific."),
		 0);
  POPULATE_BOOL (LW6DEF_EXECUTED_AGAIN,
		 _
		 ("This environment variable/keyword is used to detect wether the program has been launched by itself with an internal execv call. This is used as a workarround to set some environment variables (DYLD_LIBRARY_PATH on Mac OS X for instance) before the program is run, as sometimes using setenv() inside the program does not work."),
		 0);
  /*
   * Game settings
   */
  POPULATE_STR (LW6DEF_CHOSEN_MAP,
		_
		("The last map chosen by the player, locally. This is the map which will be used for a quick-start game, a local game, or a game started as a server."),
		"subflower");
  POPULATE_BOOL (LW6DEF_USE_TEXTURE,
		 _
		 ("Defines wether the map texture should be used. Of course if there's no map texture, the texture... won't be used. But if there is one, this parameter will force the game to ignore it and play with solid colors. This probably won't look as nice as the textured map in most cases, but some players might find it more readable and confortable to play when throwing eye candy away."),
		 1);
  POPULATE_BOOL (LW6DEF_USE_RULES_XML,
		 _
		 ("If set, then rules will be picked up from the map defined rules.xml, if it exists. This is the default. Use force-time and force-size to override this and use user-defined values anyway."),
		 1);
  POPULATE_BOOL (LW6DEF_USE_HINTS_XML,
		 _
		 ("If set, then hints will be picked up from the map defined hints.xml, if it exists. This is the default."),
		 1);
  POPULATE_BOOL (LW6DEF_USE_STYLE_XML,
		 _
		 ("If set, then style will be picked up from the map defined style.xml, if it exists. This is the default. Use force-time and force-background to override this and use user-defined values anyway."),
		 1);
  POPULATE_BOOL (LW6DEF_USE_MUSIC_FILE,
		 _
		 ("If set, then the program will use the 'music-file' attribute to choose the music to play. If unset, then a random builtin music will be picked up, regardless of what is specified in 'music-file'."),
		 1);
  POPULATE_STR (LW6DEF_FORCE,
		_
		("A comma separated list of options which should be ignored when reading map XML files. For instance, if this contains 'rounds-per-sec,moves-per-round' then whatever values were defined for this in 'rules.xml', then game will ignore them and use the user's values, stored in 'config.xml', running the game at the requested speed. This ultimately allows the player to control everything despite the values set by the map designer."),
		"respawn-team,color-conflict-mode");
  /*
   * Map rules
   */
  POPULATE_INT (LW6DEF_TOTAL_TIME,
		_
		("Defines the maximum time of the game, in seconds. Note that in some cases, the game can end much earlier if some player has managed to win before the bell rings. Also, technically, this value will be translated into rounds and moves, and the game engine will wait until enough rounds and moves have been played. So if the computer is too slow and the desired speed is not reached, then the game will last for a longer time."),
		LW6MAP_RULES_DEFAULT_TOTAL_TIME, LW6MAP_RULES_MIN_TOTAL_TIME,
		LW6MAP_RULES_MAX_TOTAL_TIME);
  POPULATE_INT (LW6DEF_RESPAWN_TEAM,
		_
		("Defines what to do when a team dies. If set to 0, team disappears forever, if set to 1, team reappears automatically with fresh fighters. It's a deathmatch mode, where the winner is not the one who stays alive the longest time, since it makes no real sens in this case, but the one who has died less often than others."),
		LW6MAP_RULES_DEFAULT_RESPAWN_TEAM,
		LW6MAP_RULES_MIN_RESPAWN_TEAM, LW6MAP_RULES_MAX_RESPAWN_TEAM);
  POPULATE_INT (LW6DEF_RESPAWN_POSITION_MODE,
		_
		("Defines how teams are set up on the map when respawning. 0 means teams respect the pre-defined start positions. 1 means that a random position will be picked, among the existing positions. That is, red could take green's place. 2 means total randomness, teams can appear anywhere."),
		LW6MAP_RULES_DEFAULT_RESPAWN_POSITION_MODE,
		LW6MAP_RULES_MIN_RESPAWN_POSITION_MODE,
		LW6MAP_RULES_MAX_RESPAWN_POSITION_MODE);
  POPULATE_INT (LW6DEF_MOVES_PER_ROUND,
		_
		("Defines how many times fighters move per round. Increasing this will just make fighters move faster, but won't change anything for the rest, that is keyboard and mouse responsivity, and network traffic will stay the same. Multiplying the number of moves per round by the number of rounds per second will give the number of moves per second, which is, in fact, how fast fighters move on the screen."),
		LW6MAP_RULES_DEFAULT_MOVES_PER_ROUND,
		LW6MAP_RULES_MIN_MOVES_PER_ROUND,
		LW6MAP_RULES_MAX_MOVES_PER_ROUND);
  POPULATE_INT (LW6DEF_SPREADS_PER_ROUND,
		_
		("Defines how many times the gradient is spread per round. Gradient spread is a very Liquid War 6 specific feature, just remember that the more often you do it, the more accurately fighters will move. That is, you will be sure they really take the shortest path. Usually this does not have much effect, the default value should fit in most cases, but you might want to decrease it on very simple maps where the gradient is obvious, or increase it on complex maps where you want fighters to be real smart."),
		LW6MAP_RULES_DEFAULT_SPREADS_PER_ROUND,
		LW6MAP_RULES_MIN_SPREADS_PER_ROUND,
		LW6MAP_RULES_MAX_SPREADS_PER_ROUND);
  POPULATE_INT (LW6DEF_ROUNDS_PER_SEC,
		_
		("Defines the overall speed of the game. All other settings being equal, raising this value will cause the game to behave faster. Everything will be faster, except probably the display since your computer will calculate more game positions in a given time and spend more CPU time. It will also increase network traffic. Values between 10 and 50 really make sense."),
		LW6MAP_RULES_DEFAULT_ROUNDS_PER_SEC,
		LW6MAP_RULES_MIN_ROUNDS_PER_SEC,
		LW6MAP_RULES_MAX_ROUNDS_PER_SEC);
  POPULATE_INT (LW6DEF_FIGHTER_ATTACK,
		_
		("Defines how hard fighters will attack others, that is, in one attack, how many life-points the attacked fighter will loose. Increasing this will cause your opponents to melt faster when you attack them. With a low value, it will take ages to take on your opponents. Different styles of game. Can radically change the gameplay."),
		LW6MAP_RULES_DEFAULT_FIGHTER_ATTACK,
		LW6MAP_RULES_MIN_FIGHTER_ATTACK,
		LW6MAP_RULES_MAX_FIGHTER_ATTACK);
  POPULATE_INT (LW6DEF_FIGHTER_DEFENSE,
		_
		("Defines how fast fighters will regenerate after an attack. When this parameter is set low, an attacked fighter, which is very dark and almost dead will take a very long time to regain energy. If the parameter is set high, it can almost instantaneously regain energy."),
		LW6MAP_RULES_DEFAULT_FIGHTER_DEFENSE,
		LW6MAP_RULES_MIN_FIGHTER_DEFENSE,
		LW6MAP_RULES_MAX_FIGHTER_DEFENSE);
  POPULATE_INT (LW6DEF_FIGHTER_NEW_HEALTH,
		_
		("Defines how healthy fighters will be when they appear on the map. This can be either at the beginning of the game of when a fighter changes team. Setting this low will allow battefields to switch from one side to another very fast, for freshly gained fighters will be feeble and very likely to return to their original camp. To calibrate this parameter, keep in mind that the absolute maximum health a fighter can have is always 10000 (ten-thousands)."),
		LW6MAP_RULES_DEFAULT_FIGHTER_NEW_HEALTH,
		LW6MAP_RULES_MIN_FIGHTER_NEW_HEALTH,
		LW6MAP_RULES_MAX_FIGHTER_NEW_HEALTH);
  POPULATE_INT (LW6DEF_FIGHTER_REGENERATE,
		_
		("Defines at which speed fighters will self-regenerate, without even begin packed together. This will allow lone fighters to regenerate a bit by hiding somewhere in the map. This is typically a low value, might even be 0."),
		LW6MAP_RULES_DEFAULT_FIGHTER_REGENERATE,
		LW6MAP_RULES_MIN_FIGHTER_REGENERATE,
		LW6MAP_RULES_MAX_FIGHTER_REGENERATE);
  POPULATE_INT (LW6DEF_SIDE_ATTACK_FACTOR,
		_
		("Defines how hard fighters will attack sideways. It's an algorithm trick, fighters attack by default the opponent right in front, but if there's no fighter there, they will still try to attack someone else, maybe sideways. But doing this their attack is not as strong. This parameter enables you to tune this. This is a percentage."),
		LW6MAP_RULES_DEFAULT_SIDE_ATTACK_FACTOR,
		LW6MAP_RULES_MIN_SIDE_ATTACK_FACTOR,
		LW6MAP_RULES_MAX_SIDE_ATTACK_FACTOR);
  POPULATE_INT (LW6DEF_SIDE_DEFENSE_FACTOR,
		_
		("Defines how fast fighters will regenerate, when being side by side instead of being right in front of the other. This is a percentage."),
		LW6MAP_RULES_DEFAULT_SIDE_DEFENSE_FACTOR,
		LW6MAP_RULES_MIN_SIDE_DEFENSE_FACTOR,
		LW6MAP_RULES_MAX_SIDE_DEFENSE_FACTOR);
  POPULATE_INT (LW6DEF_NB_MOVE_TRIES,
		_
		("Defines how many tries a fighter will do before giving-up moving and choosing another behvior (attack or defense). By tries we mean: how many directions it will try. Going North? Going North-West? Setting this to a low value, your fighters will look very stubborn and always try to move in one direction, neglecting the fact that they could dodge. This can lead to queues of fighters and other strange behaviors. On the other hand, setting it too high will cause fighter to always avoid the enemy, and groups of fighters will just pass each other without any fight. Matter of taste."),
		LW6MAP_RULES_DEFAULT_NB_MOVE_TRIES,
		LW6MAP_RULES_MIN_NB_MOVE_TRIES,
		LW6MAP_RULES_MAX_NB_MOVE_TRIES);
  POPULATE_INT (LW6DEF_NB_ATTACK_TRIES,
		_
		("Defines how many tries a fighter will do before giving-up attacking and choosing another behvior (defense). By tries we mean: how many directions it will try. Going North? Going North-West? Setting this to a low value will make fighters somewhat less aggressive. This idea is that they'll prefer to switch to the next option, that is, defense/regeneration, if there's no opponent right in front of them."),
		LW6MAP_RULES_DEFAULT_NB_ATTACK_TRIES,
		LW6MAP_RULES_MIN_NB_ATTACK_TRIES,
		LW6MAP_RULES_MAX_NB_ATTACK_TRIES);
  POPULATE_INT (LW6DEF_NB_DEFENSE_TRIES,
		_
		("Defines how many tries a fighter will do before giving-up attacking and choosing another behavior (do nothing). By tries we mean: how many directions it will try. Going North? Going North-West? Setting this to a low value, you'll need a very compact pack of fighters for regeneration to operate, else fighters will hang arround unhealthy."),
		LW6MAP_RULES_DEFAULT_NB_DEFENSE_TRIES,
		LW6MAP_RULES_MIN_NB_DEFENSE_TRIES,
		LW6MAP_RULES_MAX_NB_DEFENSE_TRIES);
  POPULATE_INT (LW6DEF_VERTICAL_MOVE,
		_
		("Defines when to process a vertical move (along the Z 'depth' axis). If set to 0, fighters never spontaneously move along this axis. If set to 1, it will be tried just after the first move failed. If set to 2, it will be tried just after the second move failed. And so on."),
		LW6MAP_RULES_DEFAULT_VERTICAL_MOVE,
		LW6MAP_RULES_MIN_VERTICAL_MOVE,
		LW6MAP_RULES_MAX_VERTICAL_MOVE);
  POPULATE_INT (LW6DEF_SINGLE_ARMY_SIZE,
		_
		("Defines the proportion of the whole available space, which will be occupied by an army at the beginning of the game. You can either imagine playing with almost empty maps, or play very crowded with almost no space left. This is a percentage, but will be multiplied by itself to get the actual surface. That is, 50 means 50%*50%, that is, a square of 1/2 the size of a square map, so it represents 25% (1/4) of the total surface."),
		LW6MAP_RULES_DEFAULT_SINGLE_ARMY_SIZE,
		LW6MAP_RULES_MIN_SINGLE_ARMY_SIZE,
		LW6MAP_RULES_MAX_SINGLE_ARMY_SIZE);
  POPULATE_INT (LW6DEF_TOTAL_ARMIES_SIZE,
		_
		("Defines the proportion of the whole available space, which can be occupied by all the armies present together. Setting this low, whenever a new team arrives on the map, fighters might be stolen to other teams, otherwise the ame would get too crowded. This allows you to play with reasonnably enough fighters with 2 players, while still allowing interesting gameplay with many players. This is a percentage, but will be multiplied by itself to get the actual surface. That is, 50 means 50%*50%, that is, a square of 1/2 the size of a square map, so it represents 25% (1/4) of the total surface."),
		LW6MAP_RULES_DEFAULT_TOTAL_ARMIES_SIZE,
		LW6MAP_RULES_MIN_TOTAL_ARMIES_SIZE,
		LW6MAP_RULES_MAX_TOTAL_ARMIES_SIZE);
  POPULATE_INT (LW6DEF_MAX_NB_TEAMS,
		_
		("Defines the maximum number of teams who can enter the game. Really makes sense in network games. Default value is 10, the maximum."),
		LW6MAP_RULES_DEFAULT_MAX_NB_TEAMS,
		LW6MAP_RULES_MIN_MAX_NB_TEAMS, LW6MAP_RULES_MAX_MAX_NB_TEAMS);
  POPULATE_INT (LW6DEF_MAX_NB_CURSORS,
		_
		("Defines the maximum number of cursors who can enter the game. Really makes sense in network games. Default value is 26, the maximum."),
		LW6MAP_RULES_DEFAULT_MAX_NB_CURSORS,
		LW6MAP_RULES_MIN_MAX_NB_CURSORS,
		LW6MAP_RULES_MAX_MAX_NB_CURSORS);
  POPULATE_INT (LW6DEF_MAX_NB_NODES,
		_
		("Defines the maximum number of servers who can enter the game. Really makes sense in network games. Default value is 10, and should fit in most cases. Can be raised up to 26."),
		LW6MAP_RULES_DEFAULT_MAX_NB_NODES,
		LW6MAP_RULES_MIN_MAX_NB_NODES, LW6MAP_RULES_MAX_MAX_NB_NODES);
  POPULATE_INT (LW6DEF_X_POLARITY,
		_
		("Defines how the map will be wrapped on the X (horizontal) axis. If set to 0, nothing is wrapped. If set to 1, the right and left borders are connected, any fighter can disappear on the right border and reappear on the left border, for instance. If set to -1, it will be wrapped but also inversed, that is on a 320x240 map, a fighter disappearing on the left border at position (0,60) will reapper on the right border at position (319,180). You can combine it with 'y-polarity'."),
		LW6MAP_RULES_DEFAULT_X_POLARITY, LW6MAP_RULES_MIN_X_POLARITY,
		LW6MAP_RULES_MAX_X_POLARITY);
  POPULATE_INT (LW6DEF_Y_POLARITY,
		_
		("Defines how the map will be wrapped on the Y (vertical) axis. If set to 0, nothing is wrapped. If set to 1, the top and bottom borders are connected, any fighter can disappear on the top border and reappear on the bottom border, for instance. If set to -1, it will be wrapped but also inversed, that is on a 320x240 map, a fighter disappearing on the bottom border at position (40,239) will reapper on the top border at position (280,0). You can combine it with 'x-polarity'."),
		LW6MAP_RULES_DEFAULT_Y_POLARITY, LW6MAP_RULES_MIN_Y_POLARITY,
		LW6MAP_RULES_MAX_Y_POLARITY);
  POPULATE_INT (LW6DEF_Z_POLARITY,
		_
		("Defines how the map will be wrapped on the Z (deep) axis. If set to 0, nothing is wrapped. If set to 1, when using a 4 layer map, for instance, fighters on layer 1 will be able to go directly to layer 4 even if layers 2 and 3 are filled with walls. A value of -1 is forbidden, this is not like x and y axis, it does not really make sense. Consider this an advanced setting which might save a layer in some tricky cases, the default value of 0 should fit in most cases."),
		LW6MAP_RULES_DEFAULT_Z_POLARITY, LW6MAP_RULES_MIN_Z_POLARITY,
		LW6MAP_RULES_MAX_Z_POLARITY);
  POPULATE_INT (LW6DEF_MAX_ZONE_SIZE,
		_
		("Defines the maximum zone size, which is an internal and rather technical parameter. The idea is that to optimize things, Liquid War 6 divides the battlefield in squares, where it can, and tries to make these squares as big as possible, the idea being that everywhere in this square, fighters follow the same intructions. Just a technical optimization. The problem is that setting it too high will reveal the optimization and its tradeoffs to the player, who will see the fighter behave strangely, following invisible paths. Plus, it's ugly. Depending on your tastes (speed, look'n'feel) you'll prefer something nice or something fast. Note that anyways passed a certain value, this does not optimize anything anymore. In doubt, don't touch it."),
		LW6MAP_RULES_DEFAULT_MAX_ZONE_SIZE,
		LW6MAP_RULES_MIN_MAX_ZONE_SIZE,
		LW6MAP_RULES_MAX_MAX_ZONE_SIZE);
  POPULATE_INT (LW6DEF_ROUND_DELTA,
		_
		("Conditions by how much the cursor potential will be incremented each time gradient is spreaded. Sounds cryptic? It is. The idea is that at each time you move your cursor of 1 pixel, theorically, you'll need in the worst case to move of 1 more pixel to reach any point on the map. Of course this is not true but this is the default asumption, and gradient spread will fix that. Only in Liquid War 6 this is not even the worst case, for you can control your cursor with the mouse and cross walls. Whenever you cross a wall, you might have done a great distance from the fighters' point of view, if the map is a maze. Thus this parameter, which corrects things, experience shows it does give acceptable results to increase the cursor potential by more than one at each turn. Toy arround with this if you find fighters take wrong paths on some given map. If in doubt, don't touch."),
		LW6MAP_RULES_DEFAULT_ROUND_DELTA,
		LW6MAP_RULES_MIN_ROUND_DELTA, LW6MAP_RULES_MAX_ROUND_DELTA);
  POPULATE_INT (LW6DEF_MAX_ROUND_DELTA,
		_
		("This is the companion value of 'round-delta'. Will put an absolute limit to the delta, which (what did you think?) is of course incremented in some cases by the core algorithm. If in doubt, don't touch."),
		LW6MAP_RULES_DEFAULT_MAX_ROUND_DELTA,
		LW6MAP_RULES_MIN_MAX_ROUND_DELTA,
		LW6MAP_RULES_MAX_MAX_ROUND_DELTA);
  POPULATE_INT (LW6DEF_MAX_CURSOR_POT,
		_
		("Defines the maximum cursor potential. Not really any reason to change it. Any high value should produce the same results. Low values might reveal algorithm bugs and inconsistencies."),
		LW6MAP_RULES_DEFAULT_MAX_CURSOR_POT,
		LW6MAP_RULES_MIN_MAX_CURSOR_POT,
		LW6MAP_RULES_MAX_MAX_CURSOR_POT);
  POPULATE_INT (LW6DEF_CURSOR_POT_INIT,
		_
		("Defines the cursor potential at startup. Not really any reason to change it. Theorically, there could be maps where the default value doesn't fit, but none has been seen yet."),
		LW6MAP_RULES_DEFAULT_CURSOR_POT_INIT,
		LW6MAP_RULES_MIN_CURSOR_POT_INIT,
		LW6MAP_RULES_MAX_CURSOR_POT_INIT);
  POPULATE_INT (LW6DEF_MAX_CURSOR_POT_OFFSET,
		_
		("Defines the maximum cursor potential offset. The idea is that in some cases, the potential of a cursor can increase in burst mode, for instance to make this cursor more important than others, so that fighters rally to it, neglecting other cursors (talking about a multi-cursor controlled team). This parameter is here to limit this burst effect and avoid bugs."),
		LW6MAP_RULES_DEFAULT_MAX_CURSOR_POT_OFFSET,
		LW6MAP_RULES_MIN_MAX_CURSOR_POT_OFFSET,
		LW6MAP_RULES_MAX_MAX_CURSOR_POT_OFFSET);
  POPULATE_INT (LW6DEF_START_RED_X,
		_
		("X start position for the red team. This is a percentage of map width, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_RED_X,
		LW6MAP_RULES_MIN_START_RED_X, LW6MAP_RULES_MAX_START_RED_X);
  POPULATE_INT (LW6DEF_START_RED_Y,
		_
		("Y start position for the red team. This is a percentage of map height, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_RED_Y,
		LW6MAP_RULES_MIN_START_RED_Y, LW6MAP_RULES_MAX_START_RED_Y);
  POPULATE_INT (LW6DEF_START_GREEN_X,
		_
		("X start position for the green team. This is a percentage of map width, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_GREEN_X,
		LW6MAP_RULES_MIN_START_GREEN_X,
		LW6MAP_RULES_MAX_START_GREEN_X);
  POPULATE_INT (LW6DEF_START_GREEN_Y,
		_
		("Y start position for the green team. This is a percentage of map height, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_GREEN_Y,
		LW6MAP_RULES_MIN_START_GREEN_Y,
		LW6MAP_RULES_MAX_START_GREEN_Y);
  POPULATE_INT (LW6DEF_START_BLUE_X,
		_
		("X start position for the blue team. This is a percentage of map width, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_BLUE_X,
		LW6MAP_RULES_MIN_START_BLUE_X, LW6MAP_RULES_MAX_START_BLUE_X);
  POPULATE_INT (LW6DEF_START_BLUE_Y,
		_
		("Y start position for the blue team. This is a percentage of map height, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_BLUE_Y,
		LW6MAP_RULES_MIN_START_BLUE_Y, LW6MAP_RULES_MAX_START_BLUE_Y);
  POPULATE_INT (LW6DEF_START_YELLOW_X,
		_
		("X start position for the yellow team. This is a percentage of map width, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_YELLOW_X,
		LW6MAP_RULES_MIN_START_YELLOW_X,
		LW6MAP_RULES_MAX_START_YELLOW_X);
  POPULATE_INT (LW6DEF_START_YELLOW_Y,
		_
		("Y start position for the yellow team. This is a percentage of map height, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_YELLOW_Y,
		LW6MAP_RULES_MIN_START_YELLOW_Y,
		LW6MAP_RULES_MAX_START_YELLOW_Y);
  POPULATE_INT (LW6DEF_START_CYAN_X,
		_
		("X start position for the cyan team. This is a percentage of map width, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_CYAN_X,
		LW6MAP_RULES_MIN_START_CYAN_X, LW6MAP_RULES_MAX_START_CYAN_X);
  POPULATE_INT (LW6DEF_START_CYAN_Y,
		_
		("Y start position for the cyan team. This is a percentage of map height, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_CYAN_Y,
		LW6MAP_RULES_MIN_START_CYAN_Y, LW6MAP_RULES_MAX_START_CYAN_Y);
  POPULATE_INT (LW6DEF_START_MAGENTA_X,
		_
		("X start position for the magenta team. This is a percentage of map width, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_MAGENTA_X,
		LW6MAP_RULES_MIN_START_MAGENTA_X,
		LW6MAP_RULES_MAX_START_MAGENTA_X);
  POPULATE_INT (LW6DEF_START_MAGENTA_Y,
		_
		("Y start position for the magenta team. This is a percentage of map height, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_MAGENTA_Y,
		LW6MAP_RULES_MIN_START_MAGENTA_Y,
		LW6MAP_RULES_MAX_START_MAGENTA_Y);
  POPULATE_INT (LW6DEF_START_ORANGE_X,
		_
		("X start position for the orange team. This is a percentage of map width, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_ORANGE_X,
		LW6MAP_RULES_MIN_START_ORANGE_X,
		LW6MAP_RULES_MAX_START_ORANGE_X);
  POPULATE_INT (LW6DEF_START_ORANGE_Y,
		_
		("Y start position for the orange team. This is a percentage of map height, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_ORANGE_Y,
		LW6MAP_RULES_MIN_START_ORANGE_Y,
		LW6MAP_RULES_MAX_START_ORANGE_Y);
  POPULATE_INT (LW6DEF_START_LIGHTBLUE_X,
		_
		("X start position for the lightblue team. This is a percentage of map width, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_LIGHTBLUE_X,
		LW6MAP_RULES_MIN_START_LIGHTBLUE_X,
		LW6MAP_RULES_MAX_START_LIGHTBLUE_X);
  POPULATE_INT (LW6DEF_START_LIGHTBLUE_Y,
		_
		("Y start position for the lightblue team. This is a percentage of map height, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_LIGHTBLUE_Y,
		LW6MAP_RULES_MIN_START_LIGHTBLUE_Y,
		LW6MAP_RULES_MAX_START_LIGHTBLUE_Y);
  POPULATE_INT (LW6DEF_START_PURPLE_X,
		_
		("X start position for the purple team. This is a percentage of map width, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_PURPLE_X,
		LW6MAP_RULES_MIN_START_PURPLE_X,
		LW6MAP_RULES_MAX_START_PURPLE_X);
  POPULATE_INT (LW6DEF_START_PURPLE_Y,
		_
		("Y start position for the purple team. This is a percentage of map height, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_PURPLE_Y,
		LW6MAP_RULES_MIN_START_PURPLE_Y,
		LW6MAP_RULES_MAX_START_PURPLE_Y);
  POPULATE_INT (LW6DEF_START_PINK_X,
		_
		("X start position for the pink team. This is a percentage of map width, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_PINK_X,
		LW6MAP_RULES_MIN_START_PINK_X, LW6MAP_RULES_MAX_START_PINK_X);
  POPULATE_INT (LW6DEF_START_PINK_Y,
		_
		("Y start position for the pink team. This is a percentage of map height, value between 0 and 100."),
		LW6MAP_RULES_DEFAULT_START_PINK_Y,
		LW6MAP_RULES_MIN_START_PINK_Y, LW6MAP_RULES_MAX_START_PINK_Y);
  POPULATE_INT (LW6DEF_START_POSITION_MODE,
		_
		("Defines how teams are set up on the map at game startup. 0 means teams respect the pre-defined start positions. 1 means that a random position will be picked, among the existing positions. That is, red could take green's place. 2 means total randomness, teams can appear anywhere."),
		LW6MAP_RULES_DEFAULT_START_POSITION_MODE,
		LW6MAP_RULES_MIN_START_POSITION_MODE,
		LW6MAP_RULES_MAX_START_POSITION_MODE);
  POPULATE_INT (LW6DEF_COLOR_CONFLICT_MODE,
		_
		("How to handle color conflicts, that is, when a player requests a color, but this color is already used, what should be done? If 0, wether a color already exists won't affect the color of a new cursor. If 1, then two players on the same computer will be allowed to share the same color/team, but if another computer is already playing with a color, any new computer will need to use another team. If 2, then it's impossible for a new cursor to use a pre-existing color, any new cursor will require a new color, if that color is already used, a new color will be picked randomly."),
		LW6MAP_RULES_DEFAULT_COLOR_CONFLICT_MODE,
		LW6MAP_RULES_MIN_COLOR_CONFLICT_MODE,
		LW6MAP_RULES_MAX_COLOR_CONFLICT_MODE);
  POPULATE_INT (LW6DEF_SPREAD_THREAD,
		_
		("If set to 1, the core algorithm with fire a separate thread to spread the gradient. By default this is turned off (set to 0). Consider this as an experimental feature, the program is already rather heavily threaded, turning this on will probably not offer any significant performance gain, even on SMP systems. This might change in the future."),
		LW6MAP_RULES_DEFAULT_SPREAD_THREAD,
		LW6MAP_RULES_MIN_SPREAD_THREAD,
		LW6MAP_RULES_MAX_SPREAD_THREAD);
  POPULATE_INT (LW6DEF_GLUE_POWER,
		_
		("Defines how sticky and powerfull the glue is. That is, if on 'glue.png' it's pitch black and this parameter is set to 3, then fighters will take 3 steps to do what would normally take only one step."),
		LW6MAP_RULES_DEFAULT_GLUE_POWER,
		LW6MAP_RULES_MIN_GLUE_POWER, LW6MAP_RULES_MAX_GLUE_POWER);
  POPULATE_INT (LW6DEF_BOOST_POWER,
		_
		("Defines how fast and powerfull the boost is. That is, if on 'boost.png' it's pitch black and this parameter is set to 3, then fighters will move and act 3 times than what they would do normally."),
		LW6MAP_RULES_DEFAULT_BOOST_POWER,
		LW6MAP_RULES_MIN_BOOST_POWER, LW6MAP_RULES_MAX_BOOST_POWER);
  POPULATE_INT (LW6DEF_DANGER_POWER,
		_
		("Defines how dangerous are the black zones defined in 'danger.png'. The value is used to decrease the fighter health at each move, so you should compare its value to something like 'fighter-attack'. Being on a dangerous zone is a bit like being attacked by an invisible and unknown ennemy."),
		LW6MAP_RULES_DEFAULT_DANGER_POWER,
		LW6MAP_RULES_MIN_DANGER_POWER, LW6MAP_RULES_MAX_DANGER_POWER);
  POPULATE_INT (LW6DEF_MEDICINE_POWER,
		_
		("Defines how fast fighter will automatically regenerate on black zones defined in 'medicine.png'. The value is used to decrease the fighter health at each move, so you should compare its value to something like 'fighter-defense'. Being on a medicined zone is a bit like being defended by an invisible and unknown friend."),
		LW6MAP_RULES_DEFAULT_MEDICINE_POWER,
		LW6MAP_RULES_MIN_MEDICINE_POWER,
		LW6MAP_RULES_MAX_MEDICINE_POWER);
  POPULATE_INT (LW6DEF_FRAGS_MODE,
		_
		("Defines how points are calculated in deathmatch mode, 0 is old school simple mode. 1 is in a mode in which 1 point is attributed to every winner, and looser looses all the corresponding points (total is always 0). 2 isproportional mode, with a total of 0 kept constant, that is, loosers loose as many points as attributed to winners. 3 is a mode in which at each death, winners are attributed a number of points proportional to their fighters, and loosers scores remain untouched."),
		LW6MAP_RULES_DEFAULT_FRAGS_MODE,
		LW6MAP_RULES_MIN_FRAGS_MODE, LW6MAP_RULES_MAX_FRAGS_MODE);
  POPULATE_INT (LW6DEF_FRAGS_TO_DISTRIBUTE,
		_
		("Defines how many points will be distributed when in deathmatch mode. When a player looses, this amont of points will be substracted to its total, and the same amount of points will be distributed to other live players, proportionnally to how many fighters they have on the battlefield."),
		LW6MAP_RULES_DEFAULT_FRAGS_TO_DISTRIBUTE,
		LW6MAP_RULES_MIN_FRAGS_TO_DISTRIBUTE,
		LW6MAP_RULES_MAX_FRAGS_TO_DISTRIBUTE);
  POPULATE_INT (LW6DEF_FRAGS_FADE_OUT,
		_
		("When a player looses (in deathmatch mode) all player points will be multiplicated by this percentage, for instance if it's 90 and player had 50 points, then player will only have 45 points, then points corresponding to the new death will be added/substrated to its total. This is to avoid players with thousands of points in advance, and keep everyone in the race. A low value will minimize the importance of game start. This is only used in modes where frags are distributed in a proportional way."),
		LW6MAP_RULES_DEFAULT_FRAGS_FADE_OUT,
		LW6MAP_RULES_MIN_FRAGS_FADE_OUT,
		LW6MAP_RULES_MAX_FRAGS_FADE_OUT);
  /*
   * map hints
   */
  POPULATE_BOOL (LW6DEF_RESAMPLE,
		 _
		 ("If set to true, maps will always be resampled to a size which depends on your screen resolution, zoom factor, and the rest. If false, maps will be set at the exact resolution of map.png."),
		 LW6LDR_HINTS_DEFAULT_RESAMPLE);
  POPULATE_INT (LW6DEF_MIN_MAP_WIDTH,
		_
		("Allows you to give a minimum map width. When designing a map you might wonder: this is dumb I'm conceiving this map I know its width, why should I limit it? Now think of the player who decided to play with highly-defined maps because he has a super calculator and a hudge screen. He might redefine this himself, and does not necessarly wishes to fire Gimp to rescale the map."),
		LW6LDR_HINTS_DEFAULT_MIN_MAP_WIDTH, 0, 0);
  POPULATE_INT (LW6DEF_MAX_MAP_WIDTH,
		_
		("Allows you to give a maximum map width. When designing a map you might wonder: this is dumb I'm conceiving this map I know its width, why should I limit it? Now think of the play who plays on a old slowish computer with a tiny screen. He might redefine this himself, and does not necessarly wishes to fire Gimp to rescale the map."),
		LW6LDR_HINTS_DEFAULT_MAX_MAP_WIDTH, 0, 0);
  POPULATE_INT (LW6DEF_MIN_MAP_HEIGHT,
		_
		("Allows you to give a minimum map height. When designing a map you might wonder: this is dumb I'm conceiving this map I know its height, why should I limit it? Now think of the player who decided to play with highly-defined maps because he has a super calculator and a hudge screen. He might redefine this himself, and does not necessarly wishes to fire Gimp to rescale the map."),
		LW6LDR_HINTS_DEFAULT_MIN_MAP_HEIGHT, 0, 0);
  POPULATE_INT (LW6DEF_MAX_MAP_HEIGHT,
		_
		("Allows you to give a maximum map height. When designing a map you might wonder: this is dumb I'm conceiving this map I know its height, why should I limit it? Now think of the play who plays on a old slowish computer with a tiny screen. He might redefine this himself, and does not necessarly wishes to fire Gimp to rescale the map."),
		LW6LDR_HINTS_DEFAULT_MAX_MAP_HEIGHT, 0, 0);
  POPULATE_INT (LW6DEF_MIN_MAP_SURFACE,
		_
		("Allows you to give a minimum map surface. Map surface is simply (width * height). This parameter is just here to save you the hassle of defining both 'min-map-width' and 'min-map-height' in a consistent manner."),
		LW6LDR_HINTS_DEFAULT_MIN_MAP_SURFACE, 0, 0);
  POPULATE_INT (LW6DEF_MAX_MAP_SURFACE,
		_
		("Allows you to give a maximum map surface. Map surface is simply (width * height). This parameter is just here to save you the hassle of defining both 'max-map-width' and 'max-map-height' in a consistent manner."),
		LW6LDR_HINTS_DEFAULT_MAX_MAP_SURFACE, 0, 0);
  POPULATE_FLOAT (LW6DEF_FIGHTER_SCALE,
		  _
		  ("Defines how wide (in pixels) fighters must be. This parameter is very important and will largely condition the number of fighters on the map. It is used when loading the map. If it is, for instance, set to 1, there will be exactly a fighter per pixel on the screen. That is, if you play 640x480 on an empty map, the maximum fighters you could have is about 300000. The idea is that by changing the resolution, you also define the density of the map. In pratice, this is done in the hope that someone with a slow computer will pick up a low resolution and therefore play small levels. Conversely, someone with a brand new computer with powerfull CPU & GPU will use great resolutions and be happy with many fighters on the map. Still, changing the resolution after loading the map will not affet the number of fighters. Same for network games, the first player, who loads the map, defines its properties according to its own settings."),
		  LW6LDR_HINTS_DEFAULT_FIGHTER_SCALE, 0, 0);
  POPULATE_BOOL (LW6DEF_DOWNSIZE_USING_FIGHTER_SCALE,
		 _
		 ("If set, then the game will automatically downsize a map according to the 'fighter-scale' parameter. Downsizing means: a 1600x1200 maps becomes 200x150, for instance. Downsizing causes fighters to be bigger because map resolution is lower. This can be usefull if you don't want fighters to be too small."),
		 LW6LDR_HINTS_DEFAULT_DOWNSIZE_USING_FIGHTER_SCALE);
  POPULATE_BOOL (LW6DEF_UPSIZE_USING_FIGHTER_SCALE,
		 _
		 ("If set, then the game will automatically upsize a map according to the 'fighter-scale' parameter. Upsizing means: a 160x120 maps becomes 400x300, for instance. Upsizing causes fighters to be smaller because map resolution is higher. This can be usefull if you don't want fighters to be too big."),
		 LW6LDR_HINTS_DEFAULT_UPSIZE_USING_FIGHTER_SCALE);
  POPULATE_BOOL (LW6DEF_DOWNSIZE_USING_BENCH_VALUE,
		 _
		 ("If set, then the game will automatically downsize a map according to the 'bench-value' parameter. Downsizing means: a 1600x1200 maps becomes 200x150, for instance. Downsizing causes fighters to be bigger because map resolution is lower. This will avoid running the game on a too big map, with your computer not being able to handle it at the required speed."),
		 LW6LDR_HINTS_DEFAULT_DOWNSIZE_USING_BENCH_VALUE);
  POPULATE_BOOL (LW6DEF_UPSIZE_USING_BENCH_VALUE,
		 _
		 ("If set, then the game will automatically upsize a map according to the 'fighter-scale' parameter. Upsizing means: a 160x120 maps becomes 400x300, for instance. Upsizing causes fighters to be smaller because map resolution is higher. This will avoid useless pixelish 'jumbo fighters' look when your computer is powerfull enough to do better."),
		 LW6LDR_HINTS_DEFAULT_UPSIZE_USING_BENCH_VALUE);
  POPULATE_BOOL (LW6DEF_GUESS_COLORS,
		 _
		 ("Defines wether colors should be set automatically from texture colors. If set to true, then the program will try to pick up colors automatically from the texture, and will override the values of the color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg parameters. How these colors are picked up can't be garanteed, so if the map does not have strong contrast or if there can be any form of ambiguity, it's safe to set this to false and define one's own colors."),
		 LW6LDR_HINTS_DEFAULT_GUESS_COLORS);
  POPULATE_BOOL (LW6DEF_BACKGROUND_COLOR_AUTO,
		 _
		 ("Defines wether hud colors will be set automatically from base and alternate colors. This is a time saver to keep map designers from requiring to redefined every single color in the game. You only need to set color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg. Then hud_color_frame_bg, hud_color_frame_fg, hud_color_text_bg and hud_color_text_fg will be automatically set."),
		 LW6LDR_HINTS_DEFAULT_BACKGROUND_COLOR_AUTO);
  POPULATE_BOOL (LW6DEF_HUD_COLOR_AUTO,
		 _
		 ("Defines wether hud colors will be set automatically from base and alternate colors. This is a time saver to keep map designers from requiring to redefined every single color in the game. You only need to set color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg. Then hud_color_frame_bg, hud_color_frame_fg, hud_color_text_bg and hud_color_text_fg will be automatically set."),
		 LW6LDR_HINTS_DEFAULT_HUD_COLOR_AUTO);
  POPULATE_BOOL (LW6DEF_MENU_COLOR_AUTO,
		 _
		 ("Defines wether menu colors will be set automatically from base and alternate colors. This is a time saver to keep map designers from requiring to redefined every single color in the game. You only need to set color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg. Then menu_color_default_bg, menu_color_default_fg, menu_color_selected_bg, menu_color_selected_fg, menu_color_disabled_bg and menu_color_disabled_fg will be automatically set."),
		 LW6LDR_HINTS_DEFAULT_MENU_COLOR_AUTO);
  POPULATE_BOOL (LW6DEF_VIEW_COLOR_AUTO,
		 _
		 ("Defines wether view colors will be set automatically from base and alternate colors. This is a time saver to keep map designers from requiring to redefined every single color in the game. You only need to set color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg. Then view_color_cursor_bg, view_color_cursor_fg, view_color_map_bg and view_color_map_fg will be automatically set."),
		 LW6LDR_HINTS_DEFAULT_VIEW_COLOR_AUTO);
  POPULATE_BOOL (LW6DEF_SYSTEM_COLOR_AUTO,
		 _
		 ("Defines wether system colors will be set automatically from base and alternate colors. This is a time saver to keep map designers from requiring to redefined every single color in the game. You only need to set color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg. Then system_color_bg and system_color_fg will be automatically set."),
		 LW6LDR_HINTS_DEFAULT_SYSTEM_COLOR_AUTO);
  POPULATE_INT (LW6DEF_WALL_GREASE,
		_
		("This parameter allows you to make walls (aka map foreground) thicker, or thiner, when map is loaded. Indeed, when map are resampled, and especially when they are downscaled, some walls may disappear, or some passages may be blocked. The loader can't automatically figure out wether it's more important to keep an existing wall or to keep an open passage for fighters. This parameter helps doing so, if you set it to a low value, level will be less greasy, and many passages might open themselves. On the contrary, if grease is at a high level, then a thin line of almost isolated pixels might become a thick wall. There's no real garantee your wall or passage will always be present, but it's a same bet to assume on a 'tunnel-like' level one needs to set grease to a low value, and on a 'wide open' level with few walls one needs to set grease to a high value."),
		LW6LDR_HINTS_DEFAULT_WALL_GREASE,
		LW6LDR_HINTS_MIN_WALL_GREASE, LW6LDR_HINTS_MAX_WALL_GREASE);
  POPULATE_BOOL (LW6DEF_GUESS_MOVES_PER_SEC,
		 _
		 ("If set, then loader will use 'time-to-cross-level' to guess the game speed parameters."),
		 LW6LDR_HINTS_DEFAULT_GUESS_MOVES_PER_SEC);
  POPULATE_FLOAT (LW6DEF_SPEED,
		  _
		  ("This parameter is the main parameter on which game speed depends. The map loader will garantee, by downscaling the map, that to cross the level (by crossing the level we mean, for instance, going from top-left corner to bottom-right corner in a straight line) a fighter will take a constant amount of time. Under the hood, the loader might of course rescale the map but it will also change game speed so that, at the end, fighters take a constant time to cross the level. This is, indeed, the most important thing, players do not care much if internally there are X or Y moves per second, the global game experience depends on how fast fighter movement looks on the screen. The default settings corresponds roughly to one second to cross the level. If you set this to 2.0, it will go twice faster."),
		  LW6LDR_HINTS_DEFAULT_SPEED, 0, 0);
  /*
   * Map style
   */
  POPULATE_BOOL (LW6DEF_KEEP_RATIO,
		 _
		 ("Defines wether the map should keep its ratio, or if it should be stretched to fill the shape of your screen."),
		 LW6MAP_STYLE_DEFAULT_KEEP_RATIO);
  POPULATE_FLOAT (LW6DEF_ZOOM,
		  _
		  ("Defines the map zoom. If lower than 1.0, map will occupy only a fraction of the screen, if greater than 1.0, some areas will be outside the screen, and the player will need to scroll through it."),
		  LW6MAP_STYLE_DEFAULT_ZOOM, 0, 0);
  POPULATE_FLOAT (LW6DEF_ZOOM_MIN,
		  _
		  ("Defines the min map zoom. If set to a low value, you'll be able to dynamically view a very small, reduced map."),
		  LW6MAP_STYLE_DEFAULT_ZOOM_MIN, 0, 0);
  POPULATE_FLOAT (LW6DEF_ZOOM_MAX,
		  _
		  ("Defines the max map zoom. If set to a high value, you'll be able to dynamically view the map with hudge fighters, seeing only a fraction of the level."),
		  LW6MAP_STYLE_DEFAULT_ZOOM_MAX, 0, 0);
  POPULATE_BOOL (LW6DEF_X_WRAP,
		 _
		 ("Defines wether the map should be wrapped on the x axis. This is the companion of 'x-polarity', if no polarity is defined, map can't be wrapped, but in some cases, one might wish to have a map with polarity but without wrapping if, for instance, textures do not tile nicely."),
		 LW6MAP_STYLE_DEFAULT_X_WRAP);
  POPULATE_BOOL (LW6DEF_Y_WRAP,
		 _
		 ("Defines wether the map should be wrapped on the y axis. This is the companion of 'y-polarity', if no polarity is defined, map can't be wrapped, but in some cases, one might wish to have a map with polarity but without wrapping if, for instance, textures do not tile nicely."),
		 LW6MAP_STYLE_DEFAULT_Y_WRAP);
  POPULATE_STR (LW6DEF_BACKGROUND_STYLE,
		_
		("The background defines, of course, what is displayed at the background, but it also conditions the colors used for other items, such as the menus for instance. The possible values are 'void' and 'bubbles'."),
		LW6MAP_STYLE_DEFAULT_BACKGROUND_STYLE);
  POPULATE_STR (LW6DEF_HUD_STYLE,
		_
		("The hud is where informations about the game are displayed. This means, who is winning, are other status-like informations. Possible values include 'floating' and 'tactical'."),
		LW6MAP_STYLE_DEFAULT_HUD_STYLE);
  POPULATE_STR (LW6DEF_MENU_STYLE,
		_
		("The menu style is simply the name of the engine used to power the menu system. The only possible value, for now, is 'cylinder'."),
		LW6MAP_STYLE_DEFAULT_MENU_STYLE);
  POPULATE_STR (LW6DEF_VIEW_STYLE,
		_
		("The view style conditions which renderer is used for the map, the area where fighters are displayed. This is not the graphics backend. Indeed, the graphics backend defines which technical tool one uses (which library) one runs, wether this parameter says what kind of rendering one wants."),
		LW6MAP_STYLE_DEFAULT_VIEW_STYLE);
  POPULATE_FLOAT (LW6DEF_ANIMATION_DENSITY,
		  _
		  ("Density of the background animation, that is, for instance, if the background animation is about displaying bubbles, using a high value will display many bubbles. A value of 1.0 corresponds to the default setting."),
		  LW6MAP_STYLE_DEFAULT_ANIMATION_DENSITY, 0, 10);
  POPULATE_FLOAT (LW6DEF_ANIMATION_SPEED,
		  _
		  ("Speed of the background animation, that is, for instance, if the background animation is about displaying bubbles, using a high value will cause bubbles to move very fast. A value of 1.0 corresponds to the default setting."),
		  LW6MAP_STYLE_DEFAULT_ANIMATION_SPEED, 0, 10);
  POPULATE_FLOAT (LW6DEF_CURSOR_SIZE,
		  _
		  ("Size of the cursors on the map. 1 is the default, setting it to a higher value will make cursors bigger, a lower value will make them smaller."),
		  LW6MAP_STYLE_DEFAULT_CURSOR_SIZE, 0, 10);
  POPULATE_FLOAT (LW6DEF_HIDDEN_LAYER_ALPHA,
		  _
		  ("Whenever players are supposed to be hidden behind a wall, for instance if they are in layer 2 and layer 1 is filled with walls, it's still possible to see them, but with a low alpha value (almost transparent). This parameter allows you to trick this value, 0 will make these players absolutely invisible, 1 will make them totally opaque, like if they were on layer 1."),
		  LW6MAP_STYLE_DEFAULT_HIDDEN_LAYER_ALPHA, 0, 1);
  POPULATE_BOOL (LW6DEF_COLORIZE,
		 _
		 ("If set, then all background drawings including textures will use the bakground colors. This means, for instance, that if background colors are set automatically by color-auto from the map texture, then the background will adopt the same range of colors than the map itself. In short, the background will mimic the map."),
		 LW6MAP_STYLE_DEFAULT_COLORIZE);
  POPULATE_BOOL (LW6DEF_PIXELIZE,
		 _
		 ("Depending on the renderer capabilities, will try to pixelize some parts of the game. This can be used to emulate the old LW5 appearance."),
		 LW6MAP_STYLE_DEFAULT_PIXELIZE);
  POPULATE_COLOR (LW6DEF_COLOR_BASE_BG,
		  _
		  ("Defines the base color, more precisely, its bg (background) part. Colors are always defined by a bg/fg pair. Most colors in the game can be deduced from this one, usually to color a map you only need to define color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg."),
		  LW6MAP_STYLE_DEFAULT_COLOR_BASE_BG);
  POPULATE_COLOR (LW6DEF_COLOR_BASE_FG,
		  _
		  ("Defines the base color, more precisely, its fg (foreground) part. Colors are always defined by a bg/fg pair. Most colors in the game can be deduced from this one, usually to color a map you only need to define color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg."),
		  LW6MAP_STYLE_DEFAULT_COLOR_BASE_FG);
  POPULATE_COLOR (LW6DEF_COLOR_ALTERNATE_BG,
		  _
		  ("Defines the alternate color, more precisely, its bg (background) part. Colors are always defined by a bg/fg pair. Most colors in the game can be deduced from this one, usually to color a map you only need to define color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg."),
		  LW6MAP_STYLE_DEFAULT_COLOR_ALTERNATE_BG);
  POPULATE_COLOR (LW6DEF_COLOR_ALTERNATE_FG,
		  _
		  ("Defines the alternate color, more precisely, its fg (foreground) part. Colors are always defined by a bg/fg pair. Most colors in the game can be deduced from this one, usually to color a map you only need to define color-base-bg, color-base-fg, color-alternate-bg and color-alternate-fg."),
		  LW6MAP_STYLE_DEFAULT_COLOR_ALTERNATE_FG);
  POPULATE_COLOR (LW6DEF_BACKGROUND_COLOR_ROOT_BG,
		  _
		  ("Defines the main background color. This is, for instance, the color which will be used to clear the screen before drawing thing. Will be automatically guessed from the map texture if color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_BACKGROUND_COLOR_ROOT_BG);
  POPULATE_COLOR (LW6DEF_BACKGROUND_COLOR_ROOT_FG,
		  _
		  ("Defines a color which will be used together with color-base-bg to compose the background. It can be wise to have a minimum contrast between this color and color-base-bg, but it is not mandatory, especially if other colors are manually redefined. Will be automatically guessed from the map texture if color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_BACKGROUND_COLOR_ROOT_FG);
  POPULATE_COLOR (LW6DEF_BACKGROUND_COLOR_STUFF_BG,
		  _
		  ("Defines a color which will be used together with color-alternate-fg to draw things (animations, sprites, text, whatever) in the background. It should be different enough from color-alternate-fg so that one can really distinguish these colors. Will be automatically guessed from the map texture if color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_BACKGROUND_COLOR_STUFF_BG);
  POPULATE_COLOR (LW6DEF_BACKGROUND_COLOR_STUFF_FG,
		  _
		  ("Defines a color which will be used to draw things (animations, sprites, text, whatever) in the background. It should be different enough from color-alternate-bg so that one can really distinguish these colors. Think of this as the sprite, the text, the whatever-needs-to-be-seen-uses-this color. Will be automatically guessed from the map texture if color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_BACKGROUND_COLOR_STUFF_FG);
  POPULATE_COLOR (LW6DEF_HUD_COLOR_FRAME_BG,
		  _
		  ("Defines the background color for the hud frame. Ignored if hud-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_HUD_COLOR_FRAME_BG);
  POPULATE_COLOR (LW6DEF_HUD_COLOR_FRAME_FG,
		  _
		  ("Defines the foreground color for the hud frame. Ignored if hud-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_HUD_COLOR_FRAME_FG);
  POPULATE_COLOR (LW6DEF_HUD_COLOR_TEXT_BG,
		  _
		  ("Defines the background color for hud text. Ignored if hud-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_HUD_COLOR_TEXT_BG);
  POPULATE_COLOR (LW6DEF_HUD_COLOR_TEXT_FG,
		  _
		  ("Defines the foreground color for hud text. Ignored if hud-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_HUD_COLOR_TEXT_FG);
  POPULATE_COLOR (LW6DEF_MENU_COLOR_DEFAULT_BG,
		  _
		  ("Defines the default background color for menus. Ignored if menu-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_MENU_COLOR_DEFAULT_BG);
  POPULATE_COLOR (LW6DEF_MENU_COLOR_DEFAULT_FG,
		  _
		  ("Defines the default foreground color for menus. In fact, this is the main color for menu text, the color used to draw letters in menus. Ignored if menu-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_MENU_COLOR_DEFAULT_FG);
  POPULATE_COLOR (LW6DEF_MENU_COLOR_SELECTED_BG,
		  _
		  ("Defines the background color for a selected menu item. Ignored if menu-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_MENU_COLOR_SELECTED_BG);
  POPULATE_COLOR (LW6DEF_MENU_COLOR_SELECTED_FG,
		  _
		  ("Defines the foreground color for a selected menu item. Ignored if menu-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_MENU_COLOR_SELECTED_FG);
  POPULATE_COLOR (LW6DEF_MENU_COLOR_DISABLED_BG,
		  _
		  ("Defines the background color for a disabled menu item. Ignored if menu-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_MENU_COLOR_DISABLED_BG);
  POPULATE_COLOR (LW6DEF_MENU_COLOR_DISABLED_FG,
		  _
		  ("Defines the foreground color for a disabled menu item. Ignored if menu-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_MENU_COLOR_DISABLED_FG);
  POPULATE_COLOR (LW6DEF_VIEW_COLOR_CURSOR_BG,
		  _
		  ("Defines the background cursor color. Will typically be used to draw the shape of the cursor. Ignored if view-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_VIEW_COLOR_CURSOR_BG);
  POPULATE_COLOR (LW6DEF_VIEW_COLOR_CURSOR_FG,
		  _
		  ("Defines the foreground cursor color. Will typically be used to draw text in the cursor. Ignored if view-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_VIEW_COLOR_CURSOR_FG);
  POPULATE_COLOR (LW6DEF_VIEW_COLOR_MAP_BG,
		  _
		  ("Defines the background map color. If there's no map texture defined or if use-texture is false, this is the color of the places where armies will go. Ignored if view-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_VIEW_COLOR_MAP_BG);
  POPULATE_COLOR (LW6DEF_VIEW_COLOR_MAP_FG,
		  _
		  ("Defines the foreground map color. If there's no map texture defined or if use-texture is false, this is the color of walls, what armies can't go through. Ignored if view-color-auto is set. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_VIEW_COLOR_MAP_FG);
  POPULATE_COLOR (LW6DEF_SYSTEM_COLOR_BG,
		  _
		  ("Defines the system background color, used when displaying system info, such as the number of frames per second. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_SYSTEM_COLOR_BG);
  POPULATE_COLOR (LW6DEF_SYSTEM_COLOR_FG,
		  _
		  ("Defines the system foreground color, used when displaying system info, such as the number of frames per second. This will typically be text color. Can be #RGB, #RGBA, #RRGGBB or #RRGGBBAA."),
		  LW6MAP_STYLE_DEFAULT_SYSTEM_COLOR_FG);
  POPULATE_COLOR (LW6DEF_TEAM_COLOR_DEAD,
		  _
		  ("Defines the color for the teams when they are dead. By default it is black, this means when a team is weak it becomes black. Syntax is HTML-like, #RGB or #RRGGBB."),
		  LW6MAP_STYLE_DEFAULT_TEAM_COLOR_DEAD);
  POPULATE_COLOR (LW6DEF_TEAM_COLOR_RED,
		  _
		  ("Defines the color for the red team. Syntax is HTML-like, #RGB or #RRGGBB."),
		  LW6MAP_STYLE_DEFAULT_TEAM_COLOR_RED);
  POPULATE_COLOR (LW6DEF_TEAM_COLOR_GREEN,
		  _
		  ("Defines the color for the green team. Syntax is HTML-like, #RGB or #RRGGBB."),
		  LW6MAP_STYLE_DEFAULT_TEAM_COLOR_GREEN);
  POPULATE_COLOR (LW6DEF_TEAM_COLOR_BLUE,
		  _
		  ("Defines the color for the blue team. Syntax is HTML-like, #RGB or #RRGGBB."),
		  LW6MAP_STYLE_DEFAULT_TEAM_COLOR_BLUE);
  POPULATE_COLOR (LW6DEF_TEAM_COLOR_YELLOW,
		  _
		  ("Defines the color for the yellow team. Syntax is HTML-like, #RGB or #RRGGBB."),
		  LW6MAP_STYLE_DEFAULT_TEAM_COLOR_YELLOW);
  POPULATE_COLOR (LW6DEF_TEAM_COLOR_CYAN,
		  _
		  ("Defines the color for the cyan team. Syntax is HTML-like, #RGB or #RRGGBB."),
		  LW6MAP_STYLE_DEFAULT_TEAM_COLOR_CYAN);
  POPULATE_COLOR (LW6DEF_TEAM_COLOR_MAGENTA,
		  _
		  ("Defines the color for the magenta team. Syntax is HTML-like, #RGB or #RRGGBB."),
		  LW6MAP_STYLE_DEFAULT_TEAM_COLOR_MAGENTA);
  POPULATE_COLOR (LW6DEF_TEAM_COLOR_ORANGE,
		  _
		  ("Defines the color for the orange team. Syntax is HTML-like, #RGB or #RRGGBB."),
		  LW6MAP_STYLE_DEFAULT_TEAM_COLOR_ORANGE);
  POPULATE_COLOR (LW6DEF_TEAM_COLOR_LIGHTBLUE,
		  _
		  ("Defines the color for the light blue team. Syntax is HTML-like, #RGB or #RRGGBB."),
		  LW6MAP_STYLE_DEFAULT_TEAM_COLOR_LIGHTBLUE);
  POPULATE_COLOR (LW6DEF_TEAM_COLOR_PURPLE,
		  _
		  ("Defines the color for the purple team. Syntax is HTML-like, #RGB or #RRGGBB."),
		  LW6MAP_STYLE_DEFAULT_TEAM_COLOR_PURPLE);
  POPULATE_COLOR (LW6DEF_TEAM_COLOR_PINK,
		  _
		  ("Defines the color for the pink team. Syntax is HTML-like, #RGB or #RRGGBB."),
		  LW6MAP_STYLE_DEFAULT_TEAM_COLOR_PINK);
  POPULATE_STR (LW6DEF_MUSIC_FILE,
		_
		("Allows you to play a custom music file (typically your own ogg music) and override default game music. If file does not exist, game will use its internal music. The file will be searched for in the current 'music-path' but also in the current map directory. No absolute or even relative path are allowed, only a plain filename with no slash or backslash. Avoid special characters at all cost."),
		LW6MAP_STYLE_DEFAULT_MUSIC_FILE);
  POPULATE_STR (LW6DEF_MUSIC_FILTER,
		_
		("A music filter, used when files are played randomly. This is not a complex regex-enabled filter, just a plain string search. Even the '*' wildcard won't work. If you want precise control on what music file to play, please consider reorganizing your files and/or use the 'music-file' parameter."),
		LW6MAP_STYLE_DEFAULT_MUSIC_FILTER);
  POPULATE_STR (LW6DEF_MUSIC_EXCLUDE,
		_
		("If this string is found in a music file name, it will be excluded from the list when playing in random mode."),
		LW6MAP_STYLE_DEFAULT_MUSIC_EXCLUDE);
  /*
   * Aliases
   */
  POPULATE_VOID ("h", _("An alias for '--help'."));
  POPULATE_VOID ("?", _("An alias for '--help'."));
  POPULATE_VOID ("v", _("An alias for '--version'."));
  POPULATE_VOID ("c", _("An alias for '--copyright'."));
  POPULATE_VOID (LW6DEF_COPY, _("An alias for '--copyright'."));
  POPULATE_VOID (LW6DEF_COPYLEFT, _("An alias for '--copyright'."));
  POPULATE_VOID (LW6DEF_CFLAGS, _("An alias for '--show-build-cflags'."));
  POPULATE_VOID (LW6DEF_LDFLAGS, _("An alias for '--show-build-ldflags'."));
  ret = 1;
  return ret;
}

/**
 * lw6hlp_reference_init
 *
 * Initializes the help reference, this must be called before
 * any call to lw6hlp_about or such help related functions.
 *
 * Return value: 1 on success, 0 if failed
 */
int
lw6hlp_reference_init ()
{
  int ret = 0;
  if (!_references)
    {
      _references =
	lw6sys_hash_new (lw6sys_free_callback, LW6HLP_APPROX_NB_ENTRIES);
      if (_references)
	{
	  ret = populate_references ();
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("can't init, help reference already set up"));
    }

  return ret;
}

/**
 * lw6hlp_reference_quit
 *
 * Un-initializes the help reference, this must be called at the
 * end of the program.
 *
 * Return value: 1 on success, 0 if failed
 */
void
lw6hlp_reference_quit ()
{
  if (_references)
    {
      lw6sys_hash_free (_references);
      _references = NULL;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("can't quit, help reference is not set"));
    }
}

_lw6hlp_reference_entry_t *
_lw6hlp_reference_get_entry (char *key)
{
  _lw6hlp_reference_entry_t *entry = NULL;
  char *formatted_key = NULL;
  if (_references)
    {
      formatted_key = lw6sys_keyword_as_key (key);
      if (formatted_key)
	{
	  entry =
	    (_lw6hlp_reference_entry_t *) lw6sys_hash_get (_references,
							   formatted_key);
	  if (!entry)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _("entry %s does not exists in help reference"),
			  key);
	    }
	  LW6SYS_FREE (formatted_key);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("trying to get entry before reference is set up"));
    }

  return entry;
}
