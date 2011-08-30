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

#include <string.h>

#include "map.h"

#define _EXP_PER_COLOR 2
#define _EXP_PER_WEAPON 3

/**
 * lw6map_exp_get_highest_color_allowed
 *
 * @exp: the player experience
 *
 * Gets the highest color available for a given exp.
 *
 * Return value: a color id
 */
int
lw6map_exp_get_highest_color_allowed (int exp)
{
  int ret = 0;

  ret =
    lw6sys_min (LW6MAP_RULES_MAX_HIGHEST_COLOR_ALLOWED,
		LW6MAP_RULES_MIN_HIGHEST_COLOR_ALLOWED + lw6sys_max (exp,
								     0) /
		_EXP_PER_COLOR);

  return ret;
}

/**
 * lw6map_exp_get_highest_weapon_allowed
 *
 * @exp: the player experience
 *
 * Gets the highest weapon available for a given exp.
 *
 * Return value: a weapon id
 */
int
lw6map_exp_get_highest_weapon_allowed (int exp)
{
  int ret = 0;

  ret =
    lw6sys_min (LW6MAP_RULES_MAX_HIGHEST_WEAPON_ALLOWED,
		LW6MAP_RULES_MIN_HIGHEST_WEAPON_ALLOWED + lw6sys_max (exp,
								      0) /
		_EXP_PER_WEAPON);

  return ret;
}
