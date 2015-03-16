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
#endif // HAVE_CONFIG_H

#include <string.h>

#include "ker.h"
#include "ker-internal.h"

/*
 * FIGHTER_MOVE_? offset or used when moving fighters,
 * and calculating "what's in that direction?".
 */
int32_t _lw6ker_fighter_t_MOVE_X_OFFSET[LW6KER_NB_DIRS] = { 0, 1, 1, 1, 1, 0, 0, -1, -1, -1, -1, 0 };
int32_t _lw6ker_fighter_t_MOVE_Y_OFFSET[LW6KER_NB_DIRS] = { -1, -1, 0, 0, 1, 1, 1, 1, 0, 0, -1, -1 };

void
_lw6ker_fighter_clear (lw6sys_context_t * sys_context, lw6ker_fighter_t * fighter)
{
  memset (fighter, 0, sizeof (lw6ker_fighter_t));
}

void
_lw6ker_fighter_update_checksum (lw6sys_context_t * sys_context, const lw6ker_fighter_t * fighter, u_int32_t * checksum)
{
  lw6sys_checksum_update_int32 (sys_context, checksum, fighter->team_color);
  lw6sys_checksum_update_int32 (sys_context, checksum, fighter->last_direction);
  lw6sys_checksum_update_int32 (sys_context, checksum, fighter->health);
  lw6sys_checksum_update_int32 (sys_context, checksum, fighter->act_counter);
  lw6sys_checksum_update_int32 (sys_context, checksum, fighter->pos.x);
  lw6sys_checksum_update_int32 (sys_context, checksum, fighter->pos.y);
  lw6sys_checksum_update_int32 (sys_context, checksum, fighter->pos.z);
}

void
_lw6ker_fighter_print_debug (lw6sys_context_t * sys_context, lw6ker_fighter_t * fighter)
{
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
	      _x_
	      ("fighter team_color=%d, last_direction=%d, health=%d, x=%d, y=%d, z=%d"),
	      fighter->team_color, fighter->last_direction, fighter->health, (int) fighter->pos.x, (int) fighter->pos.y, (int) fighter->pos.z);
}

void
_lw6ker_fighter_move (lw6sys_context_t * sys_context,
		      lw6ker_fighter_t * fighter, int32_t fighter_id, int32_t x, int32_t y, int32_t z, _lw6ker_map_state_t * map_state)
{
  _lw6ker_map_state_set_fighter_id (map_state, fighter->pos.x, fighter->pos.y, fighter->pos.z, -1);
  fighter->pos.x = x;
  fighter->pos.y = y;
  fighter->pos.z = z;
  _lw6ker_map_state_set_fighter_id (map_state, x, y, z, fighter_id);
}

void
_lw6ker_fighter_attack (lw6sys_context_t * sys_context,
			lw6ker_fighter_t * fighter, int32_t x, int32_t y,
			int32_t z, _lw6ker_map_state_t * map_state, int32_t fighter_attack, int32_t fighter_new_health)
{
  lw6ker_fighter_t *enemy;

  enemy = _lw6ker_map_state_get_fighter_rw_unsafe (map_state, x, y, z);
  enemy->health -= fighter_attack;
  if (enemy->health <= 0)
    {
      /*
       * Reflect changes in pre-calculated values
       */
      (map_state->armies.fighters_per_team[fighter->team_color])++;
      (map_state->armies.fighters_per_team[enemy->team_color])--;
      /*
       * Give the conquered enemy some health
       */
      enemy->health = fighter_new_health;
      /*
       * Change it to our team
       */
      enemy->team_color = fighter->team_color;
    }
}

void
_lw6ker_fighter_defend (lw6sys_context_t * sys_context,
			lw6ker_fighter_t * fighter, int32_t x, int32_t y, int32_t z, _lw6ker_map_state_t * map_state, int32_t fighter_defense)
{
  lw6ker_fighter_t *ally;

  ally = _lw6ker_map_state_get_fighter_rw_unsafe (map_state, x, y, z);
  ally->health += fighter_defense;
  if (ally->health > LW6MAP_MAX_FIGHTER_HEALTH)
    {
      ally->health = LW6MAP_MAX_FIGHTER_HEALTH;
    }
}

void
_lw6ker_fighter_regenerate (lw6sys_context_t * sys_context, lw6ker_fighter_t * fighter, int32_t fighter_regenerate)
{
  fighter->health += fighter_regenerate;
  if (fighter->health > LW6MAP_MAX_FIGHTER_HEALTH)
    {
      fighter->health = LW6MAP_MAX_FIGHTER_HEALTH;
    }
}
