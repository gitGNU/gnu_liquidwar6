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

#include "map.h"

#define _EXP_PER_TEAM_COLOR 2
#define _EXP_PER_WEAPON 3

/**
 * lw6map_exp_get_highest_team_color_allowed
 *
 * @sys_context: global system context
 * @exp: the player experience
 *
 * Gets the highest color available for a given exp.
 *
 * Return value: a color id
 */
int
lw6map_exp_get_highest_team_color_allowed (lw6sys_context_t * sys_context, int exp)
{
  int ret = 0;

  if (_EXP_PER_TEAM_COLOR > 0)
    {
      ret =
	lw6sys_imin (LW6MAP_RULES_MAX_HIGHEST_TEAM_COLOR_ALLOWED, LW6MAP_RULES_MIN_HIGHEST_TEAM_COLOR_ALLOWED + lw6sys_imax (exp, 0) / _EXP_PER_TEAM_COLOR);
    }
  else
    {
      ret = LW6MAP_RULES_MAX_HIGHEST_TEAM_COLOR_ALLOWED;
    }

  return ret;
}

/**
 * lw6map_exp_get_highest_weapon_allowed
 *
 * @sys_context: global system context
 * @exp: the player experience
 *
 * Gets the highest weapon available for a given exp.
 *
 * Return value: a weapon id
 */
int
lw6map_exp_get_highest_weapon_allowed (lw6sys_context_t * sys_context, int exp)
{
  int ret = 0;

  if (_EXP_PER_WEAPON > 0)
    {
      ret = lw6sys_imin (LW6MAP_RULES_MAX_HIGHEST_WEAPON_ALLOWED, LW6MAP_RULES_MIN_HIGHEST_WEAPON_ALLOWED + lw6sys_imax (exp, 0) / _EXP_PER_WEAPON);
    }
  else
    {
      ret = LW6MAP_RULES_MAX_HIGHEST_WEAPON_ALLOWED;
    }

  return ret;
}

/**
 * lw6map_exp_is_team_color_allowed
 *
 * @sys_context: global system context
 * @rules: set of rules to use
 * @team_color_id: color id to test
 *
 * Tests wether a team color is allowed for a given set of rules.
 *
 * Return value: 1 if allowed, 0 if not.
 */
int
lw6map_exp_is_team_color_allowed (lw6sys_context_t * sys_context, const lw6map_rules_t * rules, int team_color_id)
{
  int ret = 0;

  ret = lw6map_team_color_is_valid (team_color_id) && (ret <= lw6map_exp_get_highest_team_color_allowed (sys_context, rules->exp));

  return ret;
}

/**
 * lw6map_exp_is_weapon_allowed
 *
 * @sys_context: global system context
 * @rules: set of rules to use
 * @weapon_id: weapon id to test
 *
 * Tests wether a weapon is allowed for a given set of rules.
 *
 * Return value: 1 if allowed, 0 if not.
 */
int
lw6map_exp_is_weapon_allowed (lw6sys_context_t * sys_context, const lw6map_rules_t * rules, int weapon_id)
{
  int ret = 0;

  ret = lw6map_weapon_is_valid (weapon_id) && (ret <= lw6map_exp_get_highest_weapon_allowed (sys_context, rules->exp));

  return ret;
}

/**
 * lw6map_exp_get_unlocked_team_color
 *
 * @sys_context: global system context
 * @exp: exp to test
 *
 * Get the unlocked team color for a given exp, if applyable.
 *
 * Return value: -1 if nothing unlocked, else the team color
 */
int
lw6map_exp_get_unlocked_team_color (lw6sys_context_t * sys_context, int exp)
{
  int ret = LW6MAP_TEAM_COLOR_INVALID;
  int i, before, now;

  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      before = lw6map_exp_get_highest_team_color_allowed (sys_context, exp - 1);
      now = lw6map_exp_get_highest_team_color_allowed (sys_context, exp);
      if (before < now)
	{
	  ret = now;
	}
    }

  return ret;
}

/**
 * lw6map_exp_get_unlocked_weapon
 *
 * @sys_context: global system context
 * @exp: exp to test
 *
 * Get the unlocked primary weapon for a given exp, if applyable.
 *
 * Return value: -1 if nothing unlocked, else the weapon id
 */
int
lw6map_exp_get_unlocked_weapon (lw6sys_context_t * sys_context, int exp)
{
  int ret = LW6MAP_WEAPON_NONE;
  int i, before, now;

  for (i = 0; i <= LW6MAP_MAX_WEAPON_ID; ++i)
    {
      before = lw6map_exp_get_highest_weapon_allowed (sys_context, exp - 1);
      now = lw6map_exp_get_highest_weapon_allowed (sys_context, exp);
      if (before < now)
	{
	  ret = now;
	}
    }

  return ret;
}
