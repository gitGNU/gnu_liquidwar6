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
_lw6hlp_populate_references_path (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 0;

  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_USER_DIR,
			_x_
			("Set the user directory path. This is where run-time data, config files, log files, are stored. If you override this value, other parameters such as where the config and log files reside, will change."),
			"$HOME/.liquidwar6");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_CONFIG_FILE,
			_x_
			("Set the config file path. This enables you to use whatever config file you like, keeping all other informations in the same place."),
			"$HOME/.liquidwar6/config.xml");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_LOG_FILE,
			_x_
			("Set the log file path. This enables you to use whatever log file you like, keeping all other informations in the same place."),
			"$HOME/.liquidwar6/log.csv");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_PREFIX,
			_x_
			("Override the prefix value given to the GNU Autoconf ./configure script when building the game. Not all path will be changed, some of them might remain the same, for instance message translations (localedir). But most game-specific data including maps, graphics, sounds, will be searched according to the new given parameter."),
			"/usr/local");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_MOD_DIR,
			_x_
			("Set the module directory path. By changing this you will load dynamic shared libraries (game specific modules such as the graphical backend) from an alternative place. Use this at your own risks, for there can always be a binary incompatibility. You've been warned."),
			"/usr/local/lib/liquidwar6-<version>");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_DATA_DIR,
			_x_
			("Set the data directory. By changing ths value you'll be able to use an alternative data directory."),
			"/usr/local/share/liquidwar6-<version>/data");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_MUSIC_DIR,
			_x_
			("Set the music directory path. By changing this value you'll be able to use your own musics in your own directory. Note that there are other ways to achieve that, but using this option will work. The major side effect is that using this option, you really replace the existing builtin musics by your own. If you simply want to add musics you can store them in $HOME/.liquidwar6/music or in the map directory itself."),
			"/usr/local/share/liquidwar6-<version>/music");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_MUSIC_PATH,
			_x_
			("Set the music search path. By changing this value you'll be able to play with your own musics in your own directory. This is different from 'music-dir', since it includes 'music-dir', plus it adds a number of other search paths. Unlike most other parameters, the values given from the command-line, from the environment variables, or from the config file, are not overwritten, but appended. That is to say if you specify a 'music-path' with the command-line argument 'music-path=path', but also define the 'LW6_MUSIC_PATH' value and finally edit 'config.xml' to change the 'music-path' entry in it, you'll end up with the game searching for musics in all these directories. Additionnally, 'music-dir' and '<user-dir>/music' will always be in the list. Any given value can itself include several pathes, separated by the path separator. This separator is ':' on GNU/Linux, and ';' on Microsoft Windows. For instance, on a GNU/Linux box, you could use the command-line argument 'music-path=/foo/bar/music:/home/user/music/:/music'."),
			"$HOME/.liquidwar6/music:/usr/local/share/liquidwar6-<version>/music");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_MAP_DIR,
			_x_
			("Set the map directory path. By changing this value you'll be able to play with your own maps in your own directory. Note that there are other ways to achieve that, but using this option will work. However, a side effect is that you might not see builtin maps anymore."),
			"/usr/local/share/liquidwar6-<version>/map");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_MAP_PATH,
			_x_
			("Set the map search path. By changing this value you'll be able to play with your own maps in your own directory. This is different from 'map-dir', since it includes 'map-dir', plus it adds a number of other search paths. Unlike most other parameters, the values given from the command-line, from the environment variables, or from the config file, are not overwritten, but appended. That is to say if you specify a 'map-path' with the command-line argument 'map-path=path', but also define the 'LW6_MAP_PATH' value and finally edit 'config.xml' to change the 'map-path' entry in it, you'll end up with the game searching for maps in all these directories. Additionnally, 'map-dir' and '<user-dir>/map' will always be in the list. Any given value can itself include several pathes, separated by the path separator. This separator is ':' on GNU/Linux, and ';' on Microsoft Windows. For instance, on a GNU/Linux box, you could use the command-line argument 'map-path=/foo/bar/map:/home/user/map/:/map'."),
			"$HOME/.liquidwar6/map:/usr/local/share/liquidwar6-<version>/map");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_SCRIPT_FILE,
			_x_
			("Set the main script file path. This file is very important, since the program is more or less a hudge scheme interpreter, and this file is the file loaded by Guile. In short, it is the main program."),
			"/usr/local/share/liquidwar6-<version>/script/liquidwar6.scm");

  ret = 1;

  return ret;
}
