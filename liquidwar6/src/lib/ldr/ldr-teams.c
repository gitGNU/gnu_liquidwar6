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

#include <dirent.h>

#include "ldr.h"
#include "ldr-internal.h"

typedef struct teams_update_data_s
{
  lw6map_teams_t *teams;
  lw6sys_assoc_t *values;
}
teams_update_data_t;

static void
_read_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  lw6map_teams_t *teams_data;

  teams_data = (lw6map_teams_t *) callback_data;

  lw6map_teams_set (sys_context, teams_data, key, value);
}

/**
 * lw6ldr_teams_read
 *
 * @sys_context: global system context
 * @param: the teams struct to fill with values (read/write parameter)
 * @dirname: the directory of the map
 *
 * Read the teams (teams.xml) of a map. Pointer to teams must be valid,
 * and values already initialized, either zeroed or filled in with defaults
 * or custom values.
 *
 * Return value: 1 if success, 0 if failed.
 */
int
lw6ldr_teams_read (lw6sys_context_t * sys_context, lw6map_teams_t * teams, const char *dirname)
{
  int ret = 0;
  char *buf = NULL;

  buf = lw6sys_path_concat (sys_context, dirname, _LW6LDR_FILE_TEAMS_XML);

  if (buf)
    {
      if (lw6sys_file_exists (sys_context, buf))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading teams \"%s\""), buf);
	  ret = lw6cfg_read_key_value_xml_file (sys_context, buf, _read_callback, (void *) teams);
	}
      else
	{
	  // using defaults
	  ret = 1;
	}

      LW6SYS_FREE (sys_context, buf);
    }

  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to read map teams"));
    }

  return ret;
}

static void
_teams_update_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  teams_update_data_t *update_data;
  char *key;
  char *value;

  key = (char *) data;
  update_data = (teams_update_data_t *) func_data;

  if (lw6sys_assoc_has_key (sys_context, update_data->values, key))
    {
      value = lw6sys_assoc_get (sys_context, update_data->values, key);
      lw6map_teams_set (sys_context, update_data->teams, key, value);
    }
}

/**
 * lw6ldr_teams_update
 *
 * @sys_context: global system context
 * @teams: the teams struct to fill with values (read/write parameter)
 * @values: an assoc containing strings with the new values
 *
 * Overrides teams with values. Pointer to teams must be valid,
 * and values already initialized, either zeroed or filled in with defaults
 * or custom values. Not all parameters need be defined in values. It can
 * even be NULL. The idea is just that if something is defined in values,
 * it will override the existing teams.
 *
 * Return value: 1 if success, 0 if failed.
 */
int
lw6ldr_teams_update (lw6sys_context_t * sys_context, lw6map_teams_t * teams, lw6sys_assoc_t * values)
{
  int ret = 0;
  lw6sys_list_t *list;
  teams_update_data_t data;

  if (values)
    {
      list = lw6hlp_list_map_teams (sys_context);
      if (list)
	{
	  data.teams = teams;
	  data.values = values;
	  lw6sys_list_map (sys_context, list, _teams_update_callback, &data);
	  lw6sys_list_free (sys_context, list);
	  ret = 1;
	}
    }
  else
    {
      // NULL values is allowed
      ret = 1;
    }

  return ret;
}
