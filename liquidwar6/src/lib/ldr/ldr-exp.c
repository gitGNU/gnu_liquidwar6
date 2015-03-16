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

#include "ldr.h"
#include "ldr-internal.h"

/**
 * lw6ldr_exp_validate
 *
 * @sys_context: global system context
 * @level: the level to validate
 * @user_dir: user directory
 *
 * Validates a level, acknowledges you've won it. Upgrades exp.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6ldr_exp_validate (lw6sys_context_t * sys_context, const lw6map_level_t * level, const char *user_dir)
{
  int player_exp = LW6MAP_RULES_MIN_EXP;
  int new_player_exp = LW6MAP_RULES_MIN_EXP;
  int map_exp = LW6MAP_RULES_DEFAULT_EXP;
  int ret = 0;

  lw6cfg_load_exp (sys_context, user_dir, &player_exp);
  /*
   * It's important to use vanilla_exp and not rules.exp this
   * way we use the real exp from the level, not our own, else
   * we would ever increase exp playing any level
   */
  map_exp = level->metadata.vanilla_exp;

  if (player_exp == map_exp)
    {
      /*
       * OK, upgrading...
       */
      new_player_exp = lw6sys_imin (LW6MAP_RULES_MAX_EXP, player_exp + 1);
      if (new_player_exp > player_exp)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _("You just gained experience (exp=%d)."), new_player_exp);
	  ret = lw6cfg_save_exp (sys_context, user_dir, new_player_exp);
	}
    }
  else if (player_exp > map_exp)
    {
      // nothing to do...
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't validate level with player_exp=%d and map_exp=%d"), player_exp, map_exp);
    }

  return ret;
}

void
_lw6ldr_exp_fix (lw6sys_context_t * sys_context, lw6map_rules_t * rules, int exp)
{
  rules->exp = lw6sys_imax (LW6MAP_RULES_MIN_EXP, lw6sys_imax (exp, rules->exp));
  rules->highest_team_color_allowed = lw6sys_imin (rules->highest_team_color_allowed, lw6map_exp_get_highest_team_color_allowed (sys_context, exp));
  rules->highest_weapon_allowed = lw6sys_imin (rules->highest_weapon_allowed, lw6map_exp_get_highest_weapon_allowed (sys_context, exp));
}
