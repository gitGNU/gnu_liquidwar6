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

#include "map.h"

/**
 * lw6map_teams_zero
 *
 * @sys_context: global system context
 * @teams: data to initialize
 *
 * Zeros the teams struct, this is not the same as setting to defaults.
 *
 * Return value: none.
 */
void
lw6map_teams_zero (lw6sys_context_t * sys_context, lw6map_teams_t * teams)
{
  memset (teams, 0, sizeof (lw6map_teams_t));
}

/**
 * lw6map_teams_defaults
 *
 * @sys_context: global system context
 * @teams: data to initialize
 *
 * Set the teams struct to its defaults.
 *
 * Return value: none.
 */
void
lw6map_teams_defaults (lw6sys_context_t * sys_context, lw6map_teams_t * teams)
{
  teams->player_color[LW6MAP_TEAMS_PLAYER1_INDEX] = lw6map_team_color_key_to_index (sys_context, LW6MAP_TEAMS_DEFAULT_PLAYER1_COLOR);
  teams->player_color[LW6MAP_TEAMS_PLAYER2_INDEX] = lw6map_team_color_key_to_index (sys_context, LW6MAP_TEAMS_DEFAULT_PLAYER2_COLOR);
  teams->player_color[LW6MAP_TEAMS_PLAYER3_INDEX] = lw6map_team_color_key_to_index (sys_context, LW6MAP_TEAMS_DEFAULT_PLAYER3_COLOR);
  teams->player_color[LW6MAP_TEAMS_PLAYER4_INDEX] = lw6map_team_color_key_to_index (sys_context, LW6MAP_TEAMS_DEFAULT_PLAYER4_COLOR);
  teams->nb_bots = LW6MAP_TEAMS_DEFAULT_NB_BOTS;
  teams->bot_speed = LW6MAP_TEAMS_DEFAULT_BOT_SPEED;
  teams->bot_iq = LW6MAP_TEAMS_DEFAULT_BOT_IQ;
  teams->bot[LW6MAP_TEAMS_BOT1_INDEX].color = lw6map_team_color_key_to_index (sys_context, LW6MAP_TEAMS_DEFAULT_BOT1_COLOR);
  teams->bot[LW6MAP_TEAMS_BOT2_INDEX].color = lw6map_team_color_key_to_index (sys_context, LW6MAP_TEAMS_DEFAULT_BOT2_COLOR);
  teams->bot[LW6MAP_TEAMS_BOT3_INDEX].color = lw6map_team_color_key_to_index (sys_context, LW6MAP_TEAMS_DEFAULT_BOT3_COLOR);
  teams->bot[LW6MAP_TEAMS_BOT4_INDEX].color = lw6map_team_color_key_to_index (sys_context, LW6MAP_TEAMS_DEFAULT_BOT4_COLOR);
  teams->bot[LW6MAP_TEAMS_BOT5_INDEX].color = lw6map_team_color_key_to_index (sys_context, LW6MAP_TEAMS_DEFAULT_BOT5_COLOR);
  teams->bot[LW6MAP_TEAMS_BOT6_INDEX].color = lw6map_team_color_key_to_index (sys_context, LW6MAP_TEAMS_DEFAULT_BOT6_COLOR);
  teams->bot[LW6MAP_TEAMS_BOT7_INDEX].color = lw6map_team_color_key_to_index (sys_context, LW6MAP_TEAMS_DEFAULT_BOT7_COLOR);
  teams->bot[LW6MAP_TEAMS_BOT8_INDEX].color = lw6map_team_color_key_to_index (sys_context, LW6MAP_TEAMS_DEFAULT_BOT8_COLOR);
  teams->bot[LW6MAP_TEAMS_BOT9_INDEX].color = lw6map_team_color_key_to_index (sys_context, LW6MAP_TEAMS_DEFAULT_BOT9_COLOR);
  teams->bot[LW6MAP_TEAMS_BOT1_INDEX].ai = lw6sys_str_copy (sys_context, LW6MAP_TEAMS_DEFAULT_BOT1_AI);
  teams->bot[LW6MAP_TEAMS_BOT2_INDEX].ai = lw6sys_str_copy (sys_context, LW6MAP_TEAMS_DEFAULT_BOT2_AI);
  teams->bot[LW6MAP_TEAMS_BOT3_INDEX].ai = lw6sys_str_copy (sys_context, LW6MAP_TEAMS_DEFAULT_BOT3_AI);
  teams->bot[LW6MAP_TEAMS_BOT4_INDEX].ai = lw6sys_str_copy (sys_context, LW6MAP_TEAMS_DEFAULT_BOT4_AI);
  teams->bot[LW6MAP_TEAMS_BOT5_INDEX].ai = lw6sys_str_copy (sys_context, LW6MAP_TEAMS_DEFAULT_BOT5_AI);
  teams->bot[LW6MAP_TEAMS_BOT6_INDEX].ai = lw6sys_str_copy (sys_context, LW6MAP_TEAMS_DEFAULT_BOT6_AI);
  teams->bot[LW6MAP_TEAMS_BOT7_INDEX].ai = lw6sys_str_copy (sys_context, LW6MAP_TEAMS_DEFAULT_BOT7_AI);
  teams->bot[LW6MAP_TEAMS_BOT8_INDEX].ai = lw6sys_str_copy (sys_context, LW6MAP_TEAMS_DEFAULT_BOT8_AI);
  teams->bot[LW6MAP_TEAMS_BOT9_INDEX].ai = lw6sys_str_copy (sys_context, LW6MAP_TEAMS_DEFAULT_BOT9_AI);
}

/**
 * lw6map_teams_clear
 *
 * @sys_context: global system context
 * @teams: data to initialize
 *
 * Clears the teams struct, this is not the same as setting to defaults.
 * This one supposes the struct has been properly initialized, at least
 * zeroed before usage, it might contain pointers which should be freed.
 *
 * Return value: none.
 */
void
lw6map_teams_clear (lw6sys_context_t * sys_context, lw6map_teams_t * teams)
{
  int i;

  for (i = 0; i < LW6MAP_TEAMS_MAX_NB_BOTS; ++i)
    {
      if (teams->bot[i].ai)
	{
	  LW6SYS_FREE (sys_context, teams->bot[i].ai);
	}
    }
  lw6map_teams_zero (sys_context, teams);
}

/**
 * lw6map_teams_copy
 *
 * @sys_context: global system context
 * @src: source
 * @dst: destination
 *
 * Copies the contents of the teams struct. It's a real duplicate,
 * any string is reallocated.
 *
 * Return value: none.
 */
void
lw6map_teams_copy (lw6sys_context_t * sys_context, lw6map_teams_t * dst, const lw6map_teams_t * src)
{
  int i;

  lw6map_teams_clear (sys_context, dst);
  memcpy (dst, src, sizeof (lw6map_teams_t));

  for (i = 0; i < LW6MAP_TEAMS_MAX_NB_BOTS; ++i)
    {
      if (src->bot[i].ai)
	{
	  dst->bot[i].ai = lw6sys_str_copy (sys_context, src->bot[i].ai);
	}
    }
}

/**
 * lw6map_teams_set
 *
 * @sys_context: global system context
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
lw6map_teams_set (lw6sys_context_t * sys_context, lw6map_teams_t * teams, const char *key, const char *value)
{
  int ret = 1;
  char *formatted_key = NULL;

  formatted_key = lw6sys_keyword_as_key (sys_context, key);
  if (formatted_key)
    {
      if (!strcmp (LW6DEF_PLAYER1_COLOR, formatted_key))
	{
	  teams->player_color[LW6MAP_TEAMS_PLAYER1_INDEX] = lw6map_team_color_key_to_index (sys_context, value);
	}
      else if (!strcmp (LW6DEF_PLAYER2_COLOR, formatted_key))
	{
	  teams->player_color[LW6MAP_TEAMS_PLAYER2_INDEX] = lw6map_team_color_key_to_index (sys_context, value);
	}
      else if (!strcmp (LW6DEF_PLAYER3_COLOR, formatted_key))
	{
	  teams->player_color[LW6MAP_TEAMS_PLAYER3_INDEX] = lw6map_team_color_key_to_index (sys_context, value);
	}
      else if (!strcmp (LW6DEF_PLAYER4_COLOR, formatted_key))
	{
	  teams->player_color[LW6MAP_TEAMS_PLAYER4_INDEX] = lw6map_team_color_key_to_index (sys_context, value);
	}
      else if (!strcmp (LW6DEF_NB_BOTS, formatted_key))
	{
	  teams->nb_bots = lw6sys_imax (LW6MAP_TEAMS_MIN_NB_BOTS, lw6sys_imin (LW6MAP_TEAMS_MAX_NB_BOTS, lw6sys_atoi (sys_context, value)));
	}
      else if (!strcmp (LW6DEF_BOT_SPEED, formatted_key))
	{
	  teams->bot_speed = lw6sys_atof (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT_IQ, formatted_key))
	{
	  teams->bot_iq = lw6sys_imax (LW6MAP_TEAMS_MIN_BOT_IQ, lw6sys_imin (LW6MAP_TEAMS_MAX_BOT_IQ, lw6sys_atoi (sys_context, value)));
	}
      else if (!strcmp (LW6DEF_BOT1_COLOR, formatted_key))
	{
	  teams->bot[LW6MAP_TEAMS_BOT1_INDEX].color = lw6map_team_color_key_to_index (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT2_COLOR, formatted_key))
	{
	  teams->bot[LW6MAP_TEAMS_BOT2_INDEX].color = lw6map_team_color_key_to_index (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT3_COLOR, formatted_key))
	{
	  teams->bot[LW6MAP_TEAMS_BOT3_INDEX].color = lw6map_team_color_key_to_index (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT4_COLOR, formatted_key))
	{
	  teams->bot[LW6MAP_TEAMS_BOT4_INDEX].color = lw6map_team_color_key_to_index (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT5_COLOR, formatted_key))
	{
	  teams->bot[LW6MAP_TEAMS_BOT5_INDEX].color = lw6map_team_color_key_to_index (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT6_COLOR, formatted_key))
	{
	  teams->bot[LW6MAP_TEAMS_BOT6_INDEX].color = lw6map_team_color_key_to_index (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT7_COLOR, formatted_key))
	{
	  teams->bot[LW6MAP_TEAMS_BOT7_INDEX].color = lw6map_team_color_key_to_index (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT8_COLOR, formatted_key))
	{
	  teams->bot[LW6MAP_TEAMS_BOT8_INDEX].color = lw6map_team_color_key_to_index (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT9_COLOR, formatted_key))
	{
	  teams->bot[LW6MAP_TEAMS_BOT9_INDEX].color = lw6map_team_color_key_to_index (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT1_AI, formatted_key))
	{
	  if (teams->bot[LW6MAP_TEAMS_BOT1_INDEX].ai)
	    {
	      LW6SYS_FREE (sys_context, teams->bot[LW6MAP_TEAMS_BOT1_INDEX].ai);
	    }
	  teams->bot[LW6MAP_TEAMS_BOT1_INDEX].ai = lw6sys_str_copy (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT2_AI, formatted_key))
	{
	  if (teams->bot[LW6MAP_TEAMS_BOT2_INDEX].ai)
	    {
	      LW6SYS_FREE (sys_context, teams->bot[LW6MAP_TEAMS_BOT2_INDEX].ai);
	    }
	  teams->bot[LW6MAP_TEAMS_BOT2_INDEX].ai = lw6sys_str_copy (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT3_AI, formatted_key))
	{
	  if (teams->bot[LW6MAP_TEAMS_BOT3_INDEX].ai)
	    {
	      LW6SYS_FREE (sys_context, teams->bot[LW6MAP_TEAMS_BOT3_INDEX].ai);
	    }
	  teams->bot[LW6MAP_TEAMS_BOT3_INDEX].ai = lw6sys_str_copy (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT4_AI, formatted_key))
	{
	  if (teams->bot[LW6MAP_TEAMS_BOT4_INDEX].ai)
	    {
	      LW6SYS_FREE (sys_context, teams->bot[LW6MAP_TEAMS_BOT4_INDEX].ai);
	    }
	  teams->bot[LW6MAP_TEAMS_BOT4_INDEX].ai = lw6sys_str_copy (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT5_AI, formatted_key))
	{
	  if (teams->bot[LW6MAP_TEAMS_BOT5_INDEX].ai)
	    {
	      LW6SYS_FREE (sys_context, teams->bot[LW6MAP_TEAMS_BOT5_INDEX].ai);
	    }
	  teams->bot[LW6MAP_TEAMS_BOT5_INDEX].ai = lw6sys_str_copy (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT6_AI, formatted_key))
	{
	  if (teams->bot[LW6MAP_TEAMS_BOT6_INDEX].ai)
	    {
	      LW6SYS_FREE (sys_context, teams->bot[LW6MAP_TEAMS_BOT6_INDEX].ai);
	    }
	  teams->bot[LW6MAP_TEAMS_BOT6_INDEX].ai = lw6sys_str_copy (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT7_AI, formatted_key))
	{
	  if (teams->bot[LW6MAP_TEAMS_BOT7_INDEX].ai)
	    {
	      LW6SYS_FREE (sys_context, teams->bot[LW6MAP_TEAMS_BOT7_INDEX].ai);
	    }
	  teams->bot[LW6MAP_TEAMS_BOT7_INDEX].ai = lw6sys_str_copy (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT8_AI, formatted_key))
	{
	  if (teams->bot[LW6MAP_TEAMS_BOT8_INDEX].ai)
	    {
	      LW6SYS_FREE (sys_context, teams->bot[LW6MAP_TEAMS_BOT8_INDEX].ai);
	    }
	  teams->bot[LW6MAP_TEAMS_BOT8_INDEX].ai = lw6sys_str_copy (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BOT9_AI, formatted_key))
	{
	  if (teams->bot[LW6MAP_TEAMS_BOT9_INDEX].ai)
	    {
	      LW6SYS_FREE (sys_context, teams->bot[LW6MAP_TEAMS_BOT9_INDEX].ai);
	    }
	  teams->bot[LW6MAP_TEAMS_BOT9_INDEX].ai = lw6sys_str_copy (sys_context, value);
	}
      else
	{
	  ret = 0;
	}
      LW6SYS_FREE (sys_context, formatted_key);
    }
  else
    {
      ret = 0;
    }

  return ret;
}

static char *
_get_bot_color (lw6sys_context_t * sys_context, const lw6map_bot_info_t * bot_info)
{
  char *ret = NULL;

  if (lw6map_team_color_is_valid (bot_info->color))
    {
      ret = lw6sys_str_copy (sys_context, lw6map_team_color_index_to_key (sys_context, bot_info->color));
    }

  return ret;
}

static char *
_get_bot_ai (lw6sys_context_t * sys_context, const lw6map_bot_info_t * bot_info)
{
  char *ret = NULL;

  if (bot_info->ai)
    {
      ret = lw6sys_str_copy (sys_context, bot_info->ai);
    }

  return ret;
}

/**
 * lw6map_teams_get
 *
 * @sys_context: global system context
 * @teams: the teams to modify
 * @key: the key to modify
 *
 * Gets one single parameter in a teams structure. Value is
 * converted as a string.
 *
 * Return value: dynamically allocated string, NULL on error.
 */
char *
lw6map_teams_get (lw6sys_context_t * sys_context, const lw6map_teams_t * teams, const char *key)
{
  char *ret = NULL;
  char *formatted_key = NULL;

  formatted_key = lw6sys_keyword_as_key (sys_context, key);
  if (formatted_key)
    {
      if (!strcmp (LW6DEF_PLAYER1_COLOR, formatted_key))
	{
	  ret = lw6sys_str_copy (sys_context, lw6map_team_color_index_to_key (sys_context, teams->player_color[LW6MAP_TEAMS_PLAYER1_INDEX]));
	}
      else if (!strcmp (LW6DEF_PLAYER2_COLOR, formatted_key))
	{
	  ret = lw6sys_str_copy (sys_context, lw6map_team_color_index_to_key (sys_context, teams->player_color[LW6MAP_TEAMS_PLAYER2_INDEX]));
	}
      else if (!strcmp (LW6DEF_PLAYER3_COLOR, formatted_key))
	{
	  ret = lw6sys_str_copy (sys_context, lw6map_team_color_index_to_key (sys_context, teams->player_color[LW6MAP_TEAMS_PLAYER3_INDEX]));
	}
      else if (!strcmp (LW6DEF_PLAYER4_COLOR, formatted_key))
	{
	  ret = lw6sys_str_copy (sys_context, lw6map_team_color_index_to_key (sys_context, teams->player_color[LW6MAP_TEAMS_PLAYER4_INDEX]));
	}
      else if (!strcmp (LW6DEF_NB_BOTS, formatted_key))
	{
	  ret = lw6sys_itoa (sys_context, teams->nb_bots);
	}
      else if (!strcmp (LW6DEF_BOT_SPEED, formatted_key))
	{
	  ret = lw6sys_ftoa (sys_context, teams->bot_speed);
	}
      else if (!strcmp (LW6DEF_BOT_IQ, formatted_key))
	{
	  ret = lw6sys_itoa (sys_context, teams->bot_iq);
	}
      else if (!strcmp (LW6DEF_BOT1_COLOR, formatted_key))
	{
	  ret = _get_bot_color (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT1_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT2_COLOR, formatted_key))
	{
	  ret = _get_bot_color (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT2_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT3_COLOR, formatted_key))
	{
	  ret = _get_bot_color (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT3_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT4_COLOR, formatted_key))
	{
	  ret = _get_bot_color (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT4_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT5_COLOR, formatted_key))
	{
	  ret = _get_bot_color (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT5_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT6_COLOR, formatted_key))
	{
	  ret = _get_bot_color (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT6_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT7_COLOR, formatted_key))
	{
	  ret = _get_bot_color (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT7_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT8_COLOR, formatted_key))
	{
	  ret = _get_bot_color (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT8_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT9_COLOR, formatted_key))
	{
	  ret = _get_bot_color (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT9_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT1_AI, formatted_key))
	{
	  ret = _get_bot_ai (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT1_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT2_AI, formatted_key))
	{
	  ret = _get_bot_ai (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT2_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT3_AI, formatted_key))
	{
	  ret = _get_bot_ai (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT3_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT4_AI, formatted_key))
	{
	  ret = _get_bot_ai (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT4_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT5_AI, formatted_key))
	{
	  ret = _get_bot_ai (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT5_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT6_AI, formatted_key))
	{
	  ret = _get_bot_ai (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT6_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT7_AI, formatted_key))
	{
	  ret = _get_bot_ai (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT7_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT8_AI, formatted_key))
	{
	  ret = _get_bot_ai (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT8_INDEX]));
	}
      else if (!strcmp (LW6DEF_BOT9_AI, formatted_key))
	{
	  ret = _get_bot_ai (sys_context, &(teams->bot[LW6MAP_TEAMS_BOT9_INDEX]));
	}
      LW6SYS_FREE (sys_context, formatted_key);
    }

  return ret;
}

/**
 * lw6map_teams_get_default
 *
 * @sys_context: global system context
 * @key: the key we want informations about.
 *
 * Gets the default value for a given teams key.
 *
 * Return value: dynamically allocated string, NULL on error.
 */
char *
lw6map_teams_get_default (lw6sys_context_t * sys_context, const char *key)
{
  lw6map_teams_t *teams;
  char *ret = NULL;

  teams = (lw6map_teams_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6map_teams_t));
  if (teams)
    {
      lw6map_teams_defaults (sys_context, teams);
      ret = lw6map_teams_get (sys_context, teams, key);
      lw6map_teams_clear (sys_context, teams);
      LW6SYS_FREE (sys_context, teams);
    }

  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to get default value for teams parameter \"%s\""), key);
    }

  return ret;
}

/**
 * lw6map_teams_is_same
 *
 * @sys_context: global system context
 * @teams_a: one struct to compare
 * @teams_b: another struct to compare
 *
 * Compares the contents of two teams structs.
 *
 * Return value: 1 if they contain the same thing, 0 if not
 */
int
lw6map_teams_is_same (lw6sys_context_t * sys_context, const lw6map_teams_t * teams_a, const lw6map_teams_t * teams_b)
{
  int ret = 1;
  int i;

  for (i = 0; i < LW6MAP_TEAMS_NB_PLAYERS; ++i)
    {
      ret = ret && teams_a->player_color[i] == teams_b->player_color[i];
    }

  ret = ret && teams_a->nb_bots == teams_b->nb_bots;
  ret = ret && teams_a->bot_speed == teams_b->bot_speed;
  ret = ret && teams_a->bot_iq == teams_b->bot_iq;

  for (i = 0; i < LW6MAP_TEAMS_MAX_NB_BOTS; ++i)
    {
      ret = ret && teams_a->bot[i].color == teams_b->bot[i].color;
      ret = ret && lw6sys_str_is_same (sys_context, teams_a->bot[i].ai, teams_b->bot[i].ai);
    }

  return ret;
}
