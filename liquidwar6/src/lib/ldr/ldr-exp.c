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

#include "ldr.h"
#include "ldr-internal.h"

/**
 * lw6ldr_exp_validate
 *
 * @level: the level to validate
 * @user_dir: user directory
 *
 * Validates a level, acknowledges you've won it. Upgrades exp.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6ldr_exp_validate (lw6map_level_t * level, char *user_dir)
{
  int player_exp = LW6MAP_RULES_MIN_EXP;
  int new_player_exp = LW6MAP_RULES_MIN_EXP;
  int map_exp = LW6MAP_RULES_DEFAULT_EXP;
  int ret = 0;

  lw6cfg_load_exp (user_dir, &player_exp);
  map_exp = level->param.rules.exp;

  if (player_exp == map_exp)
    {
      /*
       * OK, upgrading...
       */
      new_player_exp = lw6sys_min (LW6MAP_RULES_MAX_EXP, player_exp + 1);
      if (new_player_exp > player_exp)
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE,
		      _("You just gained experience (exp=%d)."),
		      new_player_exp);
	  ret = lw6cfg_save_exp (user_dir, new_player_exp);
	}
    }
  else if (player_exp > map_exp)
    {
      // nothing to do...
      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_
		  ("can't validate level with player_exp=%d and map_exp=%d"),
		  player_exp, map_exp);
    }

  return ret;
}
