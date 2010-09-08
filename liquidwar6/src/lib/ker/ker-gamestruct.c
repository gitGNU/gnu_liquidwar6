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

#include "ker.h"
#include "ker-internal.h"

/*
 * This is used to stamp game_structs as they are created.
 * Note that there's no race condition due to the fact that this
 * is global here, in fact even when 2 processes would share
 * this sequence id, it would not matter for they would then
 * try and identify the objects in their on per-process lists,
 * structures, Guile object, whatever they use.
 */
static u_int32_t seq_id = 0;

lw6ker_game_struct_t *
lw6ker_game_struct_new (lw6map_level_t * level, lw6sys_progress_t * progress)
{
  lw6ker_game_struct_t *ret = NULL;

  ret =
    (lw6ker_game_struct_t *) LW6SYS_CALLOC (sizeof (lw6ker_game_struct_t));

  if (ret)
    {
      ret->id = 0;
      while (!ret->id)
	{
	  ret->id = ++seq_id;
	}
      ret->level = level;
      lw6map_rules_copy (&(ret->rules), &(level->param.rules));
      _lw6ker_map_struct_init (&(ret->map_struct), level, progress);
    }

  return ret;
}

void
lw6ker_game_struct_free (lw6ker_game_struct_t * game_struct)
{
  _lw6ker_map_struct_clear (&(game_struct->map_struct));
  LW6SYS_FREE (game_struct);
}

void
lw6ker_game_struct_point_to (lw6ker_game_struct_t * game_struct,
			     lw6map_level_t * level)
{
  game_struct->level = level;
}

int
lw6ker_game_struct_memory_footprint (lw6ker_game_struct_t * game_struct)
{
  int ret = 0;

  // todo

  return ret;
}

char *
lw6ker_game_struct_repr (lw6ker_game_struct_t * game_struct)
{
  char *ret = NULL;
  float compression = 0.0f;

  if (game_struct)
    {
      /*
       * do not use references to level here,
       * function is called when freeing the object
       */
      compression =
	_lw6ker_map_struct_get_compression (&(game_struct->map_struct)) *
	100.0f;

      ret =
	lw6sys_new_sprintf ("%u (%dx%dx%d, compression=%0.1f%%)",
			    game_struct->id, game_struct->map_struct.shape.w,
			    game_struct->map_struct.shape.h,
			    game_struct->map_struct.shape.d, compression);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, "game_struct",
		  _x_ ("can't generate string id for NULL game_struct"));
    }

  return ret;
}

lw6ker_game_struct_t *
lw6ker_game_struct_dup (lw6ker_game_struct_t * game_struct,
			lw6sys_progress_t * progress)
{
  lw6ker_game_struct_t *ret = NULL;

  lw6sys_progress_begin (progress);

  ret =
    (lw6ker_game_struct_t *) LW6SYS_CALLOC (sizeof (lw6ker_game_struct_t));

  if (ret)
    {
      memcpy (ret, game_struct, sizeof (lw6ker_game_struct_t));
      lw6sys_progress_update (progress, 0, 8, 1);
      /*
       * Set id after the memcpy, else will be overwritten
       */
      ret->id = 0;
      while (!ret->id)
	{
	  ret->id = ++seq_id;
	}
      ret->map_struct.places =
	(lw6ker_place_struct_t *)
	LW6SYS_MALLOC (sizeof (lw6ker_place_struct_t) *
		       ret->map_struct.nb_places);
      lw6sys_progress_update (progress, 0, 8, 2);
      if (ret->map_struct.places)
	{
	  memcpy (ret->map_struct.places, game_struct->map_struct.places,
		  sizeof (lw6ker_place_struct_t) * ret->map_struct.nb_places);
	}
      lw6sys_progress_update (progress, 0, 8, 3);
      ret->map_struct.zones =
	(lw6ker_zone_struct_t *) LW6SYS_MALLOC (sizeof (lw6ker_zone_struct_t)
						* ret->map_struct.nb_zones);
      lw6sys_progress_update (progress, 0, 8, 4);
      if (ret->map_struct.zones)
	{
	  memcpy (ret->map_struct.zones, game_struct->map_struct.zones,
		  sizeof (lw6ker_zone_struct_t) * ret->map_struct.nb_zones);
	}
      lw6sys_progress_update (progress, 0, 8, 5);
      ret->map_struct.slots =
	(lw6ker_slot_struct_t *) LW6SYS_MALLOC (sizeof (lw6ker_slot_struct_t)
						* ret->map_struct.nb_slots);
      lw6sys_progress_update (progress, 0, 8, 6);
      if (ret->map_struct.slots)
	{
	  memcpy (ret->map_struct.slots, game_struct->map_struct.slots,
		  sizeof (lw6ker_slot_struct_t) * ret->map_struct.nb_slots);
	}
      lw6sys_progress_update (progress, 0, 8, 7);
      if ((!ret->map_struct.zones) || (!ret->map_struct.slots))
	{
	  if (ret->map_struct.zones)
	    {
	      LW6SYS_FREE (ret->map_struct.zones);
	    }
	  if (ret->map_struct.slots)
	    {
	      LW6SYS_FREE (ret->map_struct.slots);
	    }
	  LW6SYS_FREE (ret);
	  ret = NULL;
	}
    }

  if (ret)
    {
      if (lw6ker_game_struct_checksum (ret) ==
	  lw6ker_game_struct_checksum (game_struct))
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("game_struct dup %d->%d"),
		      game_struct->id, ret->id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("game_struct checkum mismatch after dup"));
	}
    }

  lw6sys_progress_end (progress);


  return ret;
}

void
_lw6ker_game_struct_update_checksum (lw6ker_game_struct_t *
				     game_struct, u_int32_t * checksum)
{
  _lw6ker_map_struct_update_checksum (&(game_struct->map_struct), checksum);
  lw6map_rules_update_checksum (&(game_struct->rules), checksum);
}

u_int32_t
lw6ker_game_struct_checksum (lw6ker_game_struct_t * game_struct)
{
  u_int32_t ret = 0;

  _lw6ker_game_struct_update_checksum (game_struct, &ret);

  return ret;
}
