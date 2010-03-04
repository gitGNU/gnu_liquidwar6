/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

#include "ker.h"
#include "ker-internal.h"

int
_lw6ker_team_init (lw6ker_team_t * team, lw6ker_map_struct_t * map_struct,
		   lw6map_rules_t * rules)
{
  int ret = 0;
  int i;

  team->active = 0;
  team->map_struct = map_struct;
  team->gradient =
    (lw6ker_zone_state_t *) LW6SYS_CALLOC (map_struct->nb_zones *
					   sizeof (lw6ker_zone_state_t));
  team->cursor_ref_pot = rules->cursor_pot_init;
  team->last_spread_dir = LW6KER_DIR_NNE;

  if (team->gradient)
    {
      for (i = 0; i < map_struct->nb_zones; ++i)
	{
	  /*
	   * Set direction to "uncalculated"
	   */
	  team->gradient[i].direction_to_cursor = -1;
	  /*
	   * Set closest cursor to the center of the square
	   */
	  team->gradient[i].closest_cursor_pos.x =
	    map_struct->zones[i].pos.x + map_struct->zones[i].size / 2;
	  team->gradient[i].closest_cursor_pos.y =
	    map_struct->zones[i].pos.y + map_struct->zones[i].size / 2;
	}
      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _("unable to allocate gradient"));
    }

  return ret;
}

void
_lw6ker_team_clear (lw6ker_team_t * team)
{
  if (team->gradient)
    {
      LW6SYS_FREE (team->gradient);
    }

  memset (team, 0, sizeof (lw6ker_team_t));
}

int
_lw6ker_team_sync (lw6ker_team_t * dst, lw6ker_team_t * src)
{
  int ret = 0;

  if (dst && src
      && _lw6ker_map_struct_lazy_compare (dst->map_struct, src->map_struct))
    {
      dst->active = src->active;
      memcpy (dst->gradient, src->gradient,
	      src->map_struct->nb_zones * sizeof (lw6ker_zone_state_t));
      dst->cursor_ref_pot = src->cursor_ref_pot;
      dst->last_spread_dir = src->last_spread_dir;
      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _
		  ("team_copy only works if dst and src point to the same map_struct"));
    }

  return ret;
}

void
_lw6ker_team_update_checksum (lw6ker_team_t * team, u_int32_t * checksum)
{
  int i;

  lw6sys_checksum_update_int32 (checksum, team->active);
  // map_struct checksumed elsewhere
  for (i = 0; i < team->map_struct->nb_zones; ++i)
    {
      _lw6ker_zone_state_update_checksum (&(team->gradient[i]), checksum);
    }
  lw6sys_checksum_update_int32 (checksum, team->cursor_ref_pot);
  lw6sys_checksum_update_int32 (checksum, team->last_spread_dir);
}

void
lw6ker_team_activate (lw6ker_team_t * team, lw6sys_xyz_t pos)
{
  team->active = 1;
}

void
lw6ker_team_unactivate (lw6ker_team_t * team)
{
  team->active = 0;
}

void
lw6ker_team_normalize_pot (lw6ker_team_t * team, lw6map_rules_t * rules)
{
  int32_t i;
  int32_t n;
  u_int32_t min_pot = rules->max_cursor_pot;
  u_int32_t max_pot = 0;
  u_int32_t delta;
  lw6ker_zone_state_t *zone_states = team->gradient;

  n = team->map_struct->nb_zones;
  for (i = 0; i < n; ++i)
    {
      min_pot = lw6sys_min (min_pot, zone_states[i].potential);
      max_pot = lw6sys_max (max_pot, zone_states[i].potential);
    }

  /*
   * Theorically, we could use delta=min_pot, but in the
   * case of an extremely big map, for which even after
   * long spreading, there are still pot=0 points, we
   * fallback on max_pot/2, that is we divide by 2
   * the cursor potential.
   */
  delta = lw6sys_max (min_pot, max_pot / 2);

  for (i = 0; i < n; ++i)
    {
      zone_states[i].potential -= delta;
      if (zone_states[i].potential <= 0
	  || zone_states[i].potential > rules->max_cursor_pot)
	{
	  zone_states[i].potential = rules->cursor_pot_init;
	}
      zone_states[i].direction_to_cursor = -1;
    }

  team->cursor_ref_pot = max_pot - delta;
  if (team->cursor_ref_pot <= 0 || team->cursor_ref_pot > max_pot)
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("inconsistent cursor_ref_pot=%d (max_pot=%d, delta=%d)"),
		  team->cursor_ref_pot, max_pot, delta);
      team->cursor_ref_pot = rules->cursor_pot_init;
    }
}
