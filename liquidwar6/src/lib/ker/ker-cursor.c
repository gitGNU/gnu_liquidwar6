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

#include "ker.h"
#include "ker-internal.h"

void
_lw6ker_cursor_init (lw6sys_context_t * sys_context, lw6ker_cursor_t * cursor, char letter)
{
  memset (cursor, 0, sizeof (lw6ker_cursor_t));
  cursor->letter = letter;
  cursor->team_color = LW6MAP_TEAM_COLOR_INVALID;
}

void
_lw6ker_cursor_update_checksum (lw6sys_context_t * sys_context, const lw6ker_cursor_t * cursor, u_int32_t * checksum)
{
  lw6sys_checksum_update_int64 (sys_context, checksum, cursor->node_id);
  lw6sys_checksum_update_int32 (sys_context, checksum, cursor->cursor_id);
  lw6sys_checksum_update_int32 (sys_context, checksum, cursor->letter);
  lw6sys_checksum_update_int32 (sys_context, checksum, cursor->enabled);
  lw6sys_checksum_update_int32 (sys_context, checksum, cursor->team_color);
  lw6sys_checksum_update_xyz (sys_context, checksum, &(cursor->pos));
  lw6sys_checksum_update_int32 (sys_context, checksum, cursor->fire);
  lw6sys_checksum_update_int32 (sys_context, checksum, cursor->fire2);
  lw6sys_checksum_update_xyz (sys_context, checksum, &(cursor->apply_pos));
  lw6sys_checksum_update_int32 (sys_context, checksum, cursor->pot_offset);
}

int
_lw6ker_cursor_check_node_id (lw6sys_context_t * sys_context, const lw6ker_cursor_t * cursor, u_int64_t node_id)
{
  int ret = 0;

  ret = (lw6sys_check_id_64 (sys_context, node_id) && cursor->node_id == node_id);
  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_
		  ("node id mismatch node_id=%" LW6SYS_PRINTF_LL
		   "x cursor->node_id=%" LW6SYS_PRINTF_LL "x"), (long long) node_id, (long long) cursor->node_id);
    }

  return ret;
}

int
_lw6ker_cursor_get_start_xy (lw6sys_context_t * sys_context, int32_t * x,
			     int32_t * y, int team_color, int position_mode, int random_seed, const lw6sys_whd_t * shape, const lw6map_rules_t * rules)
{
  int ret = 1;
  int32_t px = 50;
  int32_t py = 50;
  u_int32_t checksum = 0;

  /*
   * There was a bug suspected here:
   * https://savannah.gnu.org/bugs/index.php?30842
   */

  checksum = lw6sys_checksum_int32 (sys_context, random_seed + team_color);
  /*
   * Update with something for when seed is 0 (game beginning)
   * then checksum is 0...
   */
  lw6sys_checksum_update_int32 (sys_context, &checksum, shape->w * shape->h);

  if (position_mode == LW6MAP_RULES_POSITION_MODE_TOTAL_RANDOM)
    {
      px = checksum % 100;
      py = lw6sys_checksum_int32 (sys_context, checksum + team_color) % 100;
    }
  else
    {
      if (position_mode == LW6MAP_RULES_POSITION_MODE_PICK_EXISTING)
	{
	  team_color = ((team_color + checksum) & 0xFFFF) % LW6MAP_NB_TEAM_COLORS;
	}
      if (team_color >= 0 && team_color < LW6MAP_MAX_NB_TEAMS)
	{
	  px = rules->start_x[team_color];
	  py = rules->start_y[team_color];
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("invalid team_color=%d"), team_color);
	  ret = 0;
	}
    }

  (*x) = lw6ker_percent (shape->w, px);
  (*y) = lw6ker_percent (shape->h, py);

  return ret;
}

void
_lw6ker_cursor_update_apply_pos (lw6sys_context_t * sys_context, lw6ker_cursor_t * cursor, const _lw6ker_map_struct_t * map_struct)
{
  if (map_struct)
    {
      _lw6ker_map_struct_find_free_slot_near (sys_context, map_struct, &(cursor->apply_pos), (cursor->pos));
    }
  else
    {
      cursor->apply_pos = cursor->pos;
    }
}

/**
 * lw6ker_cursor_reset
 *
 * @sys_context: global system context
 * @cursor: the cursor to reset
 *
 * Sets a cursor to defaults (disabled). This function will not touch
 * the node_id and cursor_id fields, so you can call it on an already
 * used cursor, it will stay usable.
 *
 * Return value: none
 */
void
lw6ker_cursor_reset (lw6sys_context_t * sys_context, lw6ker_cursor_t * cursor)
{
  cursor->enabled = 0;
  cursor->team_color = LW6MAP_TEAM_COLOR_INVALID;
  cursor->pos.x = 0;
  cursor->pos.y = 0;
  cursor->pos.z = 0;
  cursor->fire = 0;
  cursor->fire2 = 0;
  cursor->apply_pos = cursor->pos;
  cursor->pot_offset = 0;
}

int
_lw6ker_cursor_enable (lw6sys_context_t * sys_context, lw6ker_cursor_t * cursor, u_int64_t node_id, u_int16_t cursor_id, int team_color, int32_t x, int32_t y)
{
  int ret = 0;

  if (!cursor->enabled)
    {
      cursor->node_id = node_id;
      cursor->cursor_id = cursor_id;
      lw6ker_cursor_reset (sys_context, cursor);
      cursor->enabled = 1;
      cursor->team_color = team_color;
      cursor->pos.x = x;
      cursor->pos.y = y;
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to enable cursor which is already enabled"));
    }

  return ret;
}

int
_lw6ker_cursor_disable (lw6sys_context_t * sys_context, lw6ker_cursor_t * cursor)
{
  int ret = 0;

  if (cursor->enabled)
    {
      cursor->node_id = 0;
      cursor->cursor_id = 0;
      lw6ker_cursor_reset (sys_context, cursor);
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to disable cursor which is already disabled"));
    }

  return ret;
}

int
_lw6ker_cursor_update (lw6sys_context_t * sys_context,
		       lw6ker_cursor_t * cursor, int32_t x, int32_t y,
		       int fire, int fire2, int32_t pot_offset, const lw6sys_whd_t * shape, const lw6map_rules_t * rules)
{
  int ret = 1;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("cursor update %x %d %d %d"), cursor->cursor_id, x, y, pot_offset);
  cursor->pos.x = x;
  cursor->pos.y = y;
  cursor->fire = fire;
  cursor->fire2 = fire2;
  cursor->pot_offset = pot_offset;
  if (!_lw6ker_cursor_sanity_check (sys_context, cursor, shape, rules))
    {
      ret = 0;
    }

  return ret;
}

int
_lw6ker_cursor_sanity_check (lw6sys_context_t * sys_context, const lw6ker_cursor_t * cursor, const lw6sys_whd_t * shape, const lw6map_rules_t * rules)
{
  int ret = 1;

  if ((cursor->letter < 'a') || (cursor->letter > 'z'))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("invalid letter '%c' for cursor"), cursor->letter);
      ret = 0;
    }
  if (rules)
    {
      if (cursor->pot_offset < 0 || cursor->pot_offset > rules->max_cursor_pot_offset)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad cursor_pot_offset %d"), cursor->pot_offset);
	  ret = 0;
	}
    }
  if ((cursor->enabled
       && ((!lw6sys_check_id_64 (sys_context, cursor->node_id))
	   || (!lw6sys_check_id_16 (sys_context, cursor->cursor_id))
	   || cursor->team_color < 0
	   || cursor->team_color >= LW6MAP_MAX_NB_TEAMS))
      || ((!cursor->enabled) && (cursor->node_id != 0 || cursor->cursor_id != 0 || cursor->team_color != LW6MAP_TEAM_COLOR_INVALID)))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_
		  ("inconsistent enabled cursor values enabled=%d node_id=%"
		   LW6SYS_PRINTF_LL "x cursor_id=%x team_color=%d"),
		  cursor->enabled, (long long) cursor->node_id, (int) cursor->cursor_id, cursor->team_color);
      ret = 0;
    }
  if (shape)
    {
      if ((!lw6sys_shape_check_pos (sys_context, shape, &cursor->pos)) || (!lw6sys_shape_check_pos (sys_context, shape, &cursor->apply_pos)))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_
		      ("bad cursor position shape=%dx%d pos=%d,%d apply_pos=%d,%d"),
		      shape->w, shape->h, cursor->pos.x, cursor->pos.y, cursor->apply_pos.x, cursor->apply_pos.y);
	}
    }

  return ret;
}
