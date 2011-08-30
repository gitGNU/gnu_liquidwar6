/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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
  lw6ldr_teams_t *teams;
  lw6sys_assoc_t *values;
}
teams_update_data_t;

/**
 * lw6ldr_teams_defaults
 *
 * @teams: data to initialize
 *
 * Set the teams struct to its defaults.
 *
 * Return value: none.
 */
void
lw6ldr_teams_defaults (lw6ldr_teams_t * teams)
{
  teams->player_color =
    lw6map_team_color_key_to_index (LW6LDR_TEAMS_DEFAULT_PLAYER_COLOR);
  teams->nb_bots = LW6LDR_TEAMS_DEFAULT_NB_BOTS;
  teams->bot_speed = LW6LDR_TEAMS_DEFAULT_BOT_SPEED;
  teams->bot_iq = LW6LDR_TEAMS_DEFAULT_BOT_IQ;
  teams->bot[LW6LDR_TEAMS_BOT1_INDEX].color =
    lw6map_team_color_key_to_index (LW6LDR_TEAMS_DEFAULT_BOT1_COLOR);
  teams->bot[LW6LDR_TEAMS_BOT2_INDEX].color =
    lw6map_team_color_key_to_index (LW6LDR_TEAMS_DEFAULT_BOT2_COLOR);
  teams->bot[LW6LDR_TEAMS_BOT3_INDEX].color =
    lw6map_team_color_key_to_index (LW6LDR_TEAMS_DEFAULT_BOT3_COLOR);
  teams->bot[LW6LDR_TEAMS_BOT4_INDEX].color =
    lw6map_team_color_key_to_index (LW6LDR_TEAMS_DEFAULT_BOT4_COLOR);
  teams->bot[LW6LDR_TEAMS_BOT5_INDEX].color =
    lw6map_team_color_key_to_index (LW6LDR_TEAMS_DEFAULT_BOT5_COLOR);
  teams->bot[LW6LDR_TEAMS_BOT6_INDEX].color =
    lw6map_team_color_key_to_index (LW6LDR_TEAMS_DEFAULT_BOT6_COLOR);
  teams->bot[LW6LDR_TEAMS_BOT7_INDEX].color =
    lw6map_team_color_key_to_index (LW6LDR_TEAMS_DEFAULT_BOT7_COLOR);
  teams->bot[LW6LDR_TEAMS_BOT8_INDEX].color =
    lw6map_team_color_key_to_index (LW6LDR_TEAMS_DEFAULT_BOT8_COLOR);
  teams->bot[LW6LDR_TEAMS_BOT9_INDEX].color =
    lw6map_team_color_key_to_index (LW6LDR_TEAMS_DEFAULT_BOT9_COLOR);
  teams->bot[LW6LDR_TEAMS_BOT1_INDEX].ai =
    lw6sys_str_copy (LW6LDR_TEAMS_DEFAULT_BOT1_AI);
  teams->bot[LW6LDR_TEAMS_BOT2_INDEX].ai =
    lw6sys_str_copy (LW6LDR_TEAMS_DEFAULT_BOT2_AI);
  teams->bot[LW6LDR_TEAMS_BOT3_INDEX].ai =
    lw6sys_str_copy (LW6LDR_TEAMS_DEFAULT_BOT3_AI);
  teams->bot[LW6LDR_TEAMS_BOT4_INDEX].ai =
    lw6sys_str_copy (LW6LDR_TEAMS_DEFAULT_BOT4_AI);
  teams->bot[LW6LDR_TEAMS_BOT5_INDEX].ai =
    lw6sys_str_copy (LW6LDR_TEAMS_DEFAULT_BOT5_AI);
  teams->bot[LW6LDR_TEAMS_BOT6_INDEX].ai =
    lw6sys_str_copy (LW6LDR_TEAMS_DEFAULT_BOT6_AI);
  teams->bot[LW6LDR_TEAMS_BOT7_INDEX].ai =
    lw6sys_str_copy (LW6LDR_TEAMS_DEFAULT_BOT7_AI);
  teams->bot[LW6LDR_TEAMS_BOT8_INDEX].ai =
    lw6sys_str_copy (LW6LDR_TEAMS_DEFAULT_BOT8_AI);
  teams->bot[LW6LDR_TEAMS_BOT9_INDEX].ai =
    lw6sys_str_copy (LW6LDR_TEAMS_DEFAULT_BOT9_AI);
}

/**
 * lw6ldr_teams_zero
 *
 * @teams: data to initialize
 *
 * Zeros the teams struct, this is not the same as setting to defaults.
 *
 * Return value: none.
 */
void
lw6ldr_teams_zero (lw6ldr_teams_t * teams)
{
  memset (teams, 0, sizeof (lw6ldr_teams_t));
}

/**
 * lw6ldr_teams_clear
 *
 * @teams: data to initialize
 *
 * Clears the teams struct, this is not the same as setting to defaults.
 * This one supposes the struct has been properly initialized, at least
 * zeroed before usage, it might contain pointers which should be freed.
 *
 * Return value: none.
 */
void
lw6ldr_teams_clear (lw6ldr_teams_t * teams)
{
  int i;

  for (i = 0; i < LW6LDR_TEAMS_MAX_NB_BOTS; ++i)
    {
      if (teams->bot[i].ai)
	{
	  LW6SYS_FREE (teams->bot[i].ai);
	}
    }
  lw6ldr_teams_zero (teams);
}

static void
read_callback (void *callback_data, char *element, char *key, char *value)
{
  lw6ldr_teams_t *teams_data;

  teams_data = (lw6ldr_teams_t *) callback_data;

  lw6ldr_teams_set (teams_data, key, value);
}

/**
 * lw6ldr_teams_read
 *
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
lw6ldr_teams_read (lw6ldr_teams_t * teams, char *dirname)
{
  int ret = 0;
  char *buf = NULL;

  buf = lw6sys_path_concat (dirname, _LW6LDR_FILE_TEAMS_XML);

  if (buf)
    {
      if (lw6sys_file_exists (buf))
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("reading teams \"%s\""), buf);
	  ret =
	    lw6cfg_read_key_value_xml_file (buf, read_callback,
					    (void *) teams);
	}
      else
	{
	  // using defaults
	  ret = 1;
	}

      LW6SYS_FREE (buf);
    }

  if (!ret)
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to read map teams"));
    }

  return ret;
}

/**
 * lw6ldr_teams_set
 *
 * @teams: the teams to modify
 * @key: the key to modify
 * @value: the value to affect to the key, as a string
 *
 * Sets one single parameter in a teams structure. Value must
 * always be passed as a string, will be converted to the right
 * type automatically when storing it in the structure.
 *
 * Return value: 1 if success, 0 if failed. Note that while 0 really
 * means there's a problem, some affectations can fail and return 1,
 * needs to be worked on.
 */
int
lw6ldr_teams_set (lw6ldr_teams_t * teams, char *key, char *value)
{
  int ret = 1;
  char *formatted_key = NULL;

  formatted_key = lw6sys_keyword_as_key (key);
  if (formatted_key)
    {
      if (!strcmp (LW6DEF_PLAYER_COLOR, formatted_key))
	{
	  teams->player_color = lw6map_team_color_key_to_index (value);
	}
      else if (!strcmp (LW6DEF_NB_BOTS, formatted_key))
	{
	  teams->nb_bots =
	    lw6sys_max (LW6LDR_TEAMS_MIN_NB_BOTS,
			lw6sys_min (LW6LDR_TEAMS_MAX_NB_BOTS,
				    lw6sys_atoi (value)));
	}
      else if (!strcmp (LW6DEF_BOT_SPEED, formatted_key))
	{
	  teams->bot_speed = lw6sys_atof (value);
	}
      else if (!strcmp (LW6DEF_BOT_IQ, formatted_key))
	{
	  teams->bot_iq =
	    lw6sys_max (LW6LDR_TEAMS_MIN_BOT_IQ,
			lw6sys_min (LW6LDR_TEAMS_MAX_BOT_IQ,
				    lw6sys_atoi (value)));
	}
      else if (!strcmp (LW6DEF_BOT1_COLOR, formatted_key))
	{
	  teams->bot[LW6LDR_TEAMS_BOT1_INDEX].color =
	    lw6map_team_color_key_to_index (value);
	}
      else if (!strcmp (LW6DEF_BOT2_COLOR, formatted_key))
	{
	  teams->bot[LW6LDR_TEAMS_BOT2_INDEX].color =
	    lw6map_team_color_key_to_index (value);
	}
      else if (!strcmp (LW6DEF_BOT3_COLOR, formatted_key))
	{
	  teams->bot[LW6LDR_TEAMS_BOT3_INDEX].color =
	    lw6map_team_color_key_to_index (value);
	}
      else if (!strcmp (LW6DEF_BOT4_COLOR, formatted_key))
	{
	  teams->bot[LW6LDR_TEAMS_BOT4_INDEX].color =
	    lw6map_team_color_key_to_index (value);
	}
      else if (!strcmp (LW6DEF_BOT5_COLOR, formatted_key))
	{
	  teams->bot[LW6LDR_TEAMS_BOT5_INDEX].color =
	    lw6map_team_color_key_to_index (value);
	}
      else if (!strcmp (LW6DEF_BOT6_COLOR, formatted_key))
	{
	  teams->bot[LW6LDR_TEAMS_BOT6_INDEX].color =
	    lw6map_team_color_key_to_index (value);
	}
      else if (!strcmp (LW6DEF_BOT7_COLOR, formatted_key))
	{
	  teams->bot[LW6LDR_TEAMS_BOT7_INDEX].color =
	    lw6map_team_color_key_to_index (value);
	}
      else if (!strcmp (LW6DEF_BOT8_COLOR, formatted_key))
	{
	  teams->bot[LW6LDR_TEAMS_BOT8_INDEX].color =
	    lw6map_team_color_key_to_index (value);
	}
      else if (!strcmp (LW6DEF_BOT9_COLOR, formatted_key))
	{
	  teams->bot[LW6LDR_TEAMS_BOT9_INDEX].color =
	    lw6map_team_color_key_to_index (value);
	}
      else if (!strcmp (LW6DEF_BOT1_AI, formatted_key))
	{
	  if (teams->bot[LW6LDR_TEAMS_BOT1_INDEX].ai)
	    {
	      LW6SYS_FREE (teams->bot[LW6LDR_TEAMS_BOT1_INDEX].ai);
	    }
	  teams->bot[LW6LDR_TEAMS_BOT1_INDEX].ai = lw6sys_str_copy (value);
	}
      else if (!strcmp (LW6DEF_BOT2_AI, formatted_key))
	{
	  if (teams->bot[LW6LDR_TEAMS_BOT2_INDEX].ai)
	    {
	      LW6SYS_FREE (teams->bot[LW6LDR_TEAMS_BOT2_INDEX].ai);
	    }
	  teams->bot[LW6LDR_TEAMS_BOT2_INDEX].ai = lw6sys_str_copy (value);
	}
      else if (!strcmp (LW6DEF_BOT3_AI, formatted_key))
	{
	  if (teams->bot[LW6LDR_TEAMS_BOT3_INDEX].ai)
	    {
	      LW6SYS_FREE (teams->bot[LW6LDR_TEAMS_BOT3_INDEX].ai);
	    }
	  teams->bot[LW6LDR_TEAMS_BOT3_INDEX].ai = lw6sys_str_copy (value);
	}
      else if (!strcmp (LW6DEF_BOT4_AI, formatted_key))
	{
	  if (teams->bot[LW6LDR_TEAMS_BOT4_INDEX].ai)
	    {
	      LW6SYS_FREE (teams->bot[LW6LDR_TEAMS_BOT4_INDEX].ai);
	    }
	  teams->bot[LW6LDR_TEAMS_BOT4_INDEX].ai = lw6sys_str_copy (value);
	}
      else if (!strcmp (LW6DEF_BOT5_AI, formatted_key))
	{
	  if (teams->bot[LW6LDR_TEAMS_BOT5_INDEX].ai)
	    {
	      LW6SYS_FREE (teams->bot[LW6LDR_TEAMS_BOT5_INDEX].ai);
	    }
	  teams->bot[LW6LDR_TEAMS_BOT5_INDEX].ai = lw6sys_str_copy (value);
	}
      else if (!strcmp (LW6DEF_BOT6_AI, formatted_key))
	{
	  if (teams->bot[LW6LDR_TEAMS_BOT6_INDEX].ai)
	    {
	      LW6SYS_FREE (teams->bot[LW6LDR_TEAMS_BOT6_INDEX].ai);
	    }
	  teams->bot[LW6LDR_TEAMS_BOT6_INDEX].ai = lw6sys_str_copy (value);
	}
      else if (!strcmp (LW6DEF_BOT7_AI, formatted_key))
	{
	  if (teams->bot[LW6LDR_TEAMS_BOT7_INDEX].ai)
	    {
	      LW6SYS_FREE (teams->bot[LW6LDR_TEAMS_BOT7_INDEX].ai);
	    }
	  teams->bot[LW6LDR_TEAMS_BOT7_INDEX].ai = lw6sys_str_copy (value);
	}
      else if (!strcmp (LW6DEF_BOT8_AI, formatted_key))
	{
	  if (teams->bot[LW6LDR_TEAMS_BOT8_INDEX].ai)
	    {
	      LW6SYS_FREE (teams->bot[LW6LDR_TEAMS_BOT8_INDEX].ai);
	    }
	  teams->bot[LW6LDR_TEAMS_BOT8_INDEX].ai = lw6sys_str_copy (value);
	}
      else if (!strcmp (LW6DEF_BOT9_AI, formatted_key))
	{
	  if (teams->bot[LW6LDR_TEAMS_BOT9_INDEX].ai)
	    {
	      LW6SYS_FREE (teams->bot[LW6LDR_TEAMS_BOT9_INDEX].ai);
	    }
	  teams->bot[LW6LDR_TEAMS_BOT9_INDEX].ai = lw6sys_str_copy (value);
	}
      else
	{
	  ret = 0;
	}
      LW6SYS_FREE (formatted_key);
    }
  else
    {
      ret = 0;
    }

  return ret;
}

static char *
_get_bot_color (lw6ldr_bot_info_t * bot_info)
{
  char *ret = NULL;

  if (lw6map_team_color_is_valid (bot_info->color))
    {
      ret =
	lw6sys_str_copy (lw6map_team_color_index_to_key (bot_info->color));
    }

  return ret;
}

static char *
_get_bot_ai (lw6ldr_bot_info_t * bot_info)
{
  char *ret = NULL;

  if (bot_info->ai)
    {
      ret = lw6sys_str_copy (bot_info->ai);
    }

  return ret;
}

/**
 * lw6ldr_teams_get
 *
 * @teams: the teams to modify
 * @key: the key to modify
 *
 * Gets one single parameter in a teams structure. Value is
 * converted as a string.
 *
 * Return value: dynamically allocated string, NULL on error.
 */
char *
lw6ldr_teams_get (lw6ldr_teams_t * teams, char *key)
{
  char *ret = NULL;
  char *formatted_key = NULL;

  formatted_key = lw6sys_keyword_as_key (key);
  if (formatted_key)
    {
      if (!strcmp (LW6DEF_PLAYER_COLOR, formatted_key))
	{
	  ret =
	    lw6sys_str_copy (lw6map_team_color_index_to_key
			     (teams->player_color));
	}
      else if (!strcmp (LW6DEF_NB_BOTS, formatted_key))
	{
	  ret = lw6sys_itoa (teams->nb_bots);
	}
      else if (!strcmp (LW6DEF_BOT_SPEED, formatted_key))
	{
	  ret = lw6sys_ftoa (teams->bot_speed);
	}
      else if (!strcmp (LW6DEF_BOT_IQ, formatted_key))
	{
	  ret = lw6sys_itoa (teams->bot_iq);
	}
      else if (!strcmp (LW6DEF_BOT1_COLOR, formatted_key))
	{
	  ret = _get_bot_color (&(teams->bot[LW6LDR_TEAMS_BOT1_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT2_COLOR, formatted_key))
	{
	  ret = _get_bot_color (&(teams->bot[LW6LDR_TEAMS_BOT2_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT3_COLOR, formatted_key))
	{
	  ret = _get_bot_color (&(teams->bot[LW6LDR_TEAMS_BOT3_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT4_COLOR, formatted_key))
	{
	  ret = _get_bot_color (&(teams->bot[LW6LDR_TEAMS_BOT4_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT5_COLOR, formatted_key))
	{
	  ret = _get_bot_color (&(teams->bot[LW6LDR_TEAMS_BOT5_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT6_COLOR, formatted_key))
	{
	  ret = _get_bot_color (&(teams->bot[LW6LDR_TEAMS_BOT6_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT7_COLOR, formatted_key))
	{
	  ret = _get_bot_color (&(teams->bot[LW6LDR_TEAMS_BOT7_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT8_COLOR, formatted_key))
	{
	  ret = _get_bot_color (&(teams->bot[LW6LDR_TEAMS_BOT8_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT9_COLOR, formatted_key))
	{
	  ret = _get_bot_color (&(teams->bot[LW6LDR_TEAMS_BOT9_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT1_AI, formatted_key))
	{
	  ret = _get_bot_ai (&(teams->bot[LW6LDR_TEAMS_BOT1_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT2_AI, formatted_key))
	{
	  ret = _get_bot_ai (&(teams->bot[LW6LDR_TEAMS_BOT2_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT3_AI, formatted_key))
	{
	  ret = _get_bot_ai (&(teams->bot[LW6LDR_TEAMS_BOT3_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT4_AI, formatted_key))
	{
	  ret = _get_bot_ai (&(teams->bot[LW6LDR_TEAMS_BOT4_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT5_AI, formatted_key))
	{
	  ret = _get_bot_ai (&(teams->bot[LW6LDR_TEAMS_BOT5_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT6_AI, formatted_key))
	{
	  ret = _get_bot_ai (&(teams->bot[LW6LDR_TEAMS_BOT6_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT7_AI, formatted_key))
	{
	  ret = _get_bot_ai (&(teams->bot[LW6LDR_TEAMS_BOT7_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT8_AI, formatted_key))
	{
	  ret = _get_bot_ai (&(teams->bot[LW6LDR_TEAMS_BOT8_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT9_AI, formatted_key))
	{
	  ret = _get_bot_ai (&(teams->bot[LW6LDR_TEAMS_BOT9_INDEX]));
	}
      LW6SYS_FREE (formatted_key);
    }

  return ret;
}

/**
 * lw6ldr_teams_get_default
 *
 * @key: the key we want informations about.
 *
 * Gets the default value for a given teams key.
 *
 * Return value: dynamically allocated string, NULL on error.
 */
char *
lw6ldr_teams_get_default (char *key)
{
  lw6ldr_teams_t *teams;
  char *ret = NULL;

  teams = (lw6ldr_teams_t *) LW6SYS_CALLOC (sizeof (lw6ldr_teams_t));
  if (teams)
    {
      lw6ldr_teams_defaults (teams);
      ret = lw6ldr_teams_get (teams, key);
      lw6ldr_teams_clear (teams);
      LW6SYS_FREE (teams);
    }

  if (!ret)
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_
		  ("unable to get default value for teams parameter \"%s\""),
		  key);
    }

  return ret;
}

static void
teams_update_callback (void *func_data, void *data)
{
  teams_update_data_t *update_data;
  char *key;
  char *value;

  key = (char *) data;
  update_data = (teams_update_data_t *) func_data;

  if (lw6sys_assoc_has_key (update_data->values, key))
    {
      value = lw6sys_assoc_get (update_data->values, key);
      lw6ldr_teams_set (update_data->teams, key, value);
    }
}

/**
 * lw6ldr_teams_update
 *
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
lw6ldr_teams_update (lw6ldr_teams_t * teams, lw6sys_assoc_t * values)
{
  int ret = 0;
  lw6sys_list_t *list;
  teams_update_data_t data;

  if (values)
    {
      list = lw6hlp_list_map_teams ();
      if (list)
	{
	  data.teams = teams;
	  data.values = values;
	  lw6sys_list_map (list, teams_update_callback, &data);
	  lw6sys_list_free (list);
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
