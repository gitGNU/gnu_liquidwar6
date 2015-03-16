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

static void
_defaults_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  char *entry = NULL;
  const char *default_value = NULL;

  entry = (char *) data;
  if (entry)
    {
      if (lw6hlp_about (sys_context, NULL, &default_value, NULL, NULL, entry))
	{
	  if (default_value)
	    {
	      if (lw6cfg_must_be_saved (sys_context, entry))
		{
		  lw6cfg_set_option (sys_context, func_data, entry, default_value);
		}
	    }
	}
    }
}

/**
 * lw6cfg_defaults
 *
 * @sys_context: global system context
 * @cfg_context: opaque pointer on a context
 *
 * Sets all values to their defaults.
 *
 * Return value: 1 if success, 0 if error
 */
int
lw6cfg_defaults (lw6sys_context_t * sys_context, void *cfg_context)
{
  int ret = 1;
  _lw6cfg_context_t *_cfg_context = NULL;
  lw6sys_list_t *list = NULL;
  char *path = NULL;
  char *hostname = NULL;
  char *player_name = NULL;

  _cfg_context = (_lw6cfg_context_t *) cfg_context;
  list = lw6hlp_list (sys_context);
  if (list)
    {
      /*
       * First process all options which can be set up from hlp
       * default values, which are hardcoded and constant
       */
      lw6sys_list_map (sys_context, list, _defaults_callback, cfg_context);
      lw6sys_list_free (sys_context, list);

      /*
       * Then process all those which are more or less dynamic
       * and depend on the environment.
       */
      path = lw6sys_get_user_dir (sys_context, _cfg_context->argc, _cfg_context->argv);
      if (path)
	{
	  lw6cfg_set_option (sys_context, cfg_context, LW6DEF_USER_DIR, path);
	  LW6SYS_FREE (sys_context, path);
	}
      else
	{
	  ret = 0;
	}

      path = lw6sys_get_log_file (sys_context, _cfg_context->argc, _cfg_context->argv);
      if (path)
	{
	  lw6cfg_set_option (sys_context, cfg_context, LW6DEF_LOG_FILE, path);
	  LW6SYS_FREE (sys_context, path);
	}
      else
	{
	  ret = 0;
	}

      /*
       * For some options  we don't want the help string to be set,
       * we prefer "" for the help string contains a sample user
       * directory but this would interfere with the real, current,
       * user directory, know at run time.
       */
      lw6cfg_set_option (sys_context, cfg_context, LW6DEF_MUSIC_PATH, "");
      lw6cfg_set_option (sys_context, cfg_context, LW6DEF_MAP_PATH, "");

      player_name = lw6sys_get_username (sys_context);
      if (player_name)
	{
	  lw6cfg_set_option (sys_context, cfg_context, LW6DEF_PLAYER1_NAME, player_name);
	  LW6SYS_FREE (sys_context, player_name);
	}
      else
	{
	  ret = 0;
	}

      hostname = lw6sys_get_hostname (sys_context);
      if (hostname)
	{
	  player_name = lw6sys_new_sprintf (sys_context, _x_ ("player2@%s"), hostname);
	  if (player_name)
	    {
	      lw6cfg_set_option (sys_context, cfg_context, LW6DEF_PLAYER2_NAME, player_name);
	      LW6SYS_FREE (sys_context, player_name);
	    }
	  else
	    {
	      ret = 0;
	    }
	  player_name = lw6sys_new_sprintf (sys_context, _x_ ("player3@%s"), hostname);
	  if (player_name)
	    {
	      lw6cfg_set_option (sys_context, cfg_context, LW6DEF_PLAYER3_NAME, player_name);
	      LW6SYS_FREE (sys_context, player_name);
	    }
	  else
	    {
	      ret = 0;
	    }
	  player_name = lw6sys_new_sprintf (sys_context, _x_ ("player4@%s"), hostname);
	  if (player_name)
	    {
	      lw6cfg_set_option (sys_context, cfg_context, LW6DEF_PLAYER4_NAME, player_name);
	      LW6SYS_FREE (sys_context, player_name);
	    }
	  else
	    {
	      ret = 0;
	    }
	  LW6SYS_FREE (sys_context, hostname);
	}
      else
	{
	  ret = 0;
	}

      /*
       * We set those to false even if they are void (they must not
       * be saved in the config file).
       */
      lw6cfg_set_option_bool (sys_context, cfg_context, LW6DEF_QUICK_START, 0);
      lw6cfg_set_option_bool (sys_context, cfg_context, LW6DEF_DEMO, 0);
      lw6cfg_set_option_bool (sys_context, cfg_context, LW6DEF_SERVER, 0);
    }
  else
    {
      ret = 0;
    }

  return ret;
}
