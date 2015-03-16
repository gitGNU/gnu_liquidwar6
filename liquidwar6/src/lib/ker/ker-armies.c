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

int
_lw6ker_armies_init (lw6sys_context_t * sys_context, _lw6ker_armies_t * armies, const _lw6ker_map_struct_t * map_struct, const lw6map_rules_t * options)
{
  int ret = 0;

  // todo
  ret = 1;
  armies->map_struct = map_struct;
  armies->max_fighters = lw6ker_per100_2 (map_struct->room_for_armies, options->total_armies_size);
  armies->active_fighters = 0;
  armies->fighters = (lw6ker_fighter_t *) LW6SYS_CALLOC (sys_context, armies->max_fighters * sizeof (lw6ker_fighter_t));

  if (armies->fighters)
    {
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to allocate armies"));
    }

  return ret;
}

void
_lw6ker_armies_clear (lw6sys_context_t * sys_context, _lw6ker_armies_t * armies)
{
  if (armies->fighters)
    {
      LW6SYS_FREE (sys_context, armies->fighters);
    }

  armies->fighters = NULL;
  armies->max_fighters = 0;
  armies->active_fighters = 0;
}

int
_lw6ker_armies_sync (lw6sys_context_t * sys_context, _lw6ker_armies_t * dst, const _lw6ker_armies_t * src)
{
  int ret = 0;

  if (dst && src && _lw6ker_map_struct_lazy_compare (sys_context, dst->map_struct, src->map_struct))
    {
      int i;

      dst->max_fighters = src->max_fighters;
      dst->active_fighters = src->active_fighters;
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	{
	  dst->fighters_per_team[i] = src->fighters_per_team[i];
	  dst->frags[i] = src->frags[i];
	}
      memcpy (dst->fighters, src->fighters, src->max_fighters * sizeof (lw6ker_fighter_t));
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("armies_copy only works if dst and src point to the same map_struct"));
    }

  return ret;
}

void
_lw6ker_armies_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_armies_t * armies, u_int32_t * checksum)
{
  int i;

  lw6sys_checksum_update_int32 (sys_context, checksum, armies->max_fighters);
  lw6sys_checksum_update_int32 (sys_context, checksum, armies->active_fighters);
  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      lw6sys_checksum_update_int32 (sys_context, checksum, armies->fighters_per_team[i]);
      lw6sys_checksum_update_int32 (sys_context, checksum, armies->frags[i]);
    }
  for (i = 0; i < armies->max_fighters; ++i)
    {
      _lw6ker_fighter_update_checksum (sys_context, &(armies->fighters[i]), checksum);
    }
}

int32_t
_lw6ker_armies_add_fighter (lw6sys_context_t * sys_context, _lw6ker_armies_t * armies, lw6ker_fighter_t fighter)
{
  int32_t new_id = -1;

  if (armies->active_fighters < armies->max_fighters)
    {
      new_id = armies->active_fighters;
      armies->fighters[new_id] = fighter;
      armies->fighters_per_team[fighter.team_color]++;
      armies->active_fighters++;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add fighter, there are already %d fighters"), armies->active_fighters);
    }

  return new_id;
}

/*
 * Removes the last fighter. There's no way to remove a fighter
 * with a smaller index. The trick is to swap fighters before deletion,
 * then delete.
 */
int
_lw6ker_armies_remove_fighter (lw6sys_context_t * sys_context, _lw6ker_armies_t * armies)
{
  int ret = 0;

  if (armies->active_fighters > 0)
    {
      int32_t last_fighter_id;

      last_fighter_id = armies->active_fighters - 1;
      armies->fighters_per_team[armies->fighters[last_fighter_id].team_color]--;
      /*
       * This memset might not be truely needed but well, it can't harm,
       * and fighter addition/deletion is rare enough that we can take
       * the time to vacuum stuff.
       */
      memset (&(armies->fighters[last_fighter_id]), 0, sizeof (lw6ker_fighter_t));
      armies->active_fighters--;

      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to remove fighter, none to remove"));
    }

  return ret;
}
