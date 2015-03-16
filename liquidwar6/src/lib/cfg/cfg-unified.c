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

#include <string.h>

#include "cfg.h"
#include "cfg-internal.h"

static char *
_get_option (lw6sys_context_t * sys_context, int argc, const char *argv[], char *key)
{
  char *ret = NULL;
  char *config_file = NULL;
  _lw6cfg_context_t *cfg_context = NULL;

  cfg_context = _lw6cfg_init (sys_context, argc, argv);
  if (cfg_context)
    {
      config_file = lw6sys_get_config_file (sys_context, argc, argv);
      if (config_file)
	{
	  if (_lw6cfg_load (sys_context, cfg_context, config_file))
	    {
	      if (_lw6cfg_option_exists (sys_context, cfg_context, key))
		{
		  ret = _lw6cfg_get_option (sys_context, cfg_context, key);
		}
	    }
	  LW6SYS_FREE (sys_context, config_file);
	}
      _lw6cfg_quit (sys_context, cfg_context);
    }

  return ret;
}


/**
 * lw6cfg_unified_get_value
 *
 * @sys_context: global system context
 * @argc: number of command-line args, as passed to @main
 * @argv: arry of command-line args, as passed to @main
 * @key: the key to query
 *
 * Unified "value" getter, which gets informations from environment
 * variables, command line, and config file. The rules is that the command-line
 * argument always has the last word. It will override any other value.
 * Follows environment variables, which will be used if no command-line
 * argument is supplied. Note that these are "LW6_" prefixed and uppercased
 * environment variables as opposed to lowercased and "dash-separated"
 * keys. Finally, if there's no environment variable, nor any config-file
 * corresponding entry, the value will be searched in the config file.
 * If there's no information in the config file, NULL is returned.
 *
 * Return value: a string with the value. Can be NULL. Must be freed.
 */
char *
lw6cfg_unified_get_value (lw6sys_context_t * sys_context, int argc, const char *argv[], char *key)
{
  char *ret = NULL;

  ret = lw6sys_arg_get_value_with_env (sys_context, argc, argv, key);

  if (!ret)
    {
      ret = _get_option (sys_context, argc, argv, key);
    }

  return ret;
}

/**
 * lw6cfg_unified_get_user_dir
 *
 * @sys_context: global system context
 * @argc: number of command-line args, as passed to @main
 * @argv: arry of command-line args, as passed to @main
 *
 * Gets the user dir, taking all parameters in account, that's to
 * say the "LW6_USER_DIR" env value, the "--user-dir" command-line
 * paramater and the LW6DEF_USER_DIR config file entry.
 *
 * Return value: the directory path, might be NULL, must be freed.
 */
char *
lw6cfg_unified_get_user_dir (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *ret;

  ret = lw6cfg_unified_get_value (sys_context, argc, argv, LW6DEF_USER_DIR);
  if (!ret)
    {
      ret = lw6sys_get_user_dir (sys_context, argc, argv);
    }

  return ret;
}

/**
 * lw6cfg_unified_get_log_file
 *
 * @sys_context: global system context
 * @argc: number of command-line args, as passed to @main
 * @argv: arry of command-line args, as passed to @main
 *
 * Gets the log file, taking all parameters in account, that's to
 * say the "LW6_LOG_FILE" env value, the "--log-file" command-line
 * paramater and the LW6DEF_LOG_FILE config file entry.
 *
 * Return value: the directory path, might be NULL, must be freed.
 */
char *
lw6cfg_unified_get_log_file (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *ret;

  ret = lw6cfg_unified_get_value (sys_context, argc, argv, LW6DEF_LOG_FILE);
  if (!ret)
    {
      ret = lw6sys_get_log_file (sys_context, argc, argv);
    }

  return ret;
}

/**
 * lw6cfg_unified_get_music_path
 *
 * @sys_context: global system context
 * @argc: number of command-line args, as passed to @main
 * @argv: arry of command-line args, as passed to @main
 *
 * Gets the user dir, taking all parameters in account, that's to
 * say the "LW6_MUSIC_PATH" env value, the "--music-path" command-line
 * paramater and the LW6DEF_MUSIC_PATH config file entry.
 *
 * Return value: the directory path, might be NULL, must be freed.
 */
char *
lw6cfg_unified_get_music_path (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *tmp = NULL;
  char *music_path = NULL;
  char *music_path_config = NULL;

  music_path = lw6sys_get_music_path (sys_context, argc, argv);
  if (music_path)
    {
      music_path_config = _get_option (sys_context, argc, argv, LW6DEF_MUSIC_PATH);
      if (music_path_config)
	{
	  tmp = music_path;
	  music_path = lw6sys_env_concat (sys_context, music_path, music_path_config);
	  LW6SYS_FREE (sys_context, tmp);
	  LW6SYS_FREE (sys_context, music_path_config);
	}
    }

  return music_path;
}

/**
 * lw6cfg_unified_get_map_path
 *
 * @sys_context: global system context
 * @argc: number of command-line args, as passed to @main
 * @argv: arry of command-line args, as passed to @main
 *
 * Gets the user dir, taking all parameters in account, that's to
 * say the "LW6_MAP_PATH" env value, the "--map-path" command-line
 * paramater and the LW6DEF_MAP_PATH config file entry.
 *
 * Return value: the directory path, might be NULL, must be freed.
 */
char *
lw6cfg_unified_get_map_path (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *tmp = NULL;
  char *map_path = NULL;
  char *map_path_config = NULL;

  map_path = lw6sys_get_map_path (sys_context, argc, argv);
  if (map_path)
    {
      map_path_config = _get_option (sys_context, argc, argv, LW6DEF_MAP_PATH);
      if (map_path_config)
	{
	  tmp = map_path;
	  map_path = lw6sys_env_concat (sys_context, map_path, map_path_config);
	  LW6SYS_FREE (sys_context, tmp);
	  LW6SYS_FREE (sys_context, map_path_config);
	}
    }

  return map_path;
}
