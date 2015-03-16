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

/*
 * This is used to stamp game_structs as they are created.
 * Note that there's no race condition due to the fact that this
 * is global here, in fact even when 2 processes would share
 * this sequence id, it would not matter for they would then
 * try and identify the objects in their on per-process lists,
 * structures, Guile object, whatever they use.
 */
static volatile u_int32_t seq_id = 0;

/*
 * The sequence generation is done by a function since seq_id is
 * static and cannot be accessed from elsewhere, but we decided
 * to put some constructors outside this file (in ker-hexa.c for
 * instance).
 */
void
_lw6ker_game_struct_set_id (lw6sys_context_t * sys_context, _lw6ker_game_struct_t * game_struct)
{
  game_struct->id = 0;
  while (!game_struct->id)
    {
      game_struct->id = ++seq_id;
    }
}

_lw6ker_game_struct_t *
_lw6ker_game_struct_new (lw6sys_context_t * sys_context, const lw6map_level_t * level, lw6sys_progress_t * progress)
{
  _lw6ker_game_struct_t *ret = NULL;

  ret = (_lw6ker_game_struct_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6ker_game_struct_t));
  if (ret)
    {
      _lw6ker_game_struct_set_id (sys_context, ret);
      ret->level = level;
      lw6map_rules_copy (sys_context, &(ret->rules), &(level->param.rules));
      _lw6ker_map_struct_init (sys_context, &(ret->map_struct), level, progress);
    }

  return ret;
}

/**
 * lw6ker_game_struct_new
 *
 * @sys_context: global system context
 * @level: the level on which the game_struct is based
 * @progress: progress indicator
 *
 * Creates a new game_struct from a level. The game_struct is different from
 * the level in the sense that the game_struct does contain algorithmic specific
 * optimizations, it's a ready-to-use struct desgined for execution speed, while
 * the plain level just stores information.
 *
 * Return value: newly allocated object
 */
lw6ker_game_struct_t *
lw6ker_game_struct_new (lw6sys_context_t * sys_context, const lw6map_level_t * level, lw6sys_progress_t * progress)
{
  return (lw6ker_game_struct_t *) _lw6ker_game_struct_new (sys_context, level, progress);
}

void
_lw6ker_game_struct_free (lw6sys_context_t * sys_context, _lw6ker_game_struct_t * game_struct)
{
  _lw6ker_map_struct_clear (sys_context, &(game_struct->map_struct));
  LW6SYS_FREE (sys_context, game_struct);
}

/**
 * lw6ker_game_struct_free
 *
 * @sys_context: global system context
 * @game_struct: the game_struct to free
 *
 * Frees a game_struct object, releasing all required stuff. The source
 * level must still be available when freeing this.
 *
 * Return value: none
 */
void
lw6ker_game_struct_free (lw6sys_context_t * sys_context, lw6ker_game_struct_t * game_struct)
{
  _lw6ker_game_struct_free (sys_context, (_lw6ker_game_struct_t *) game_struct);
}

void
_lw6ker_game_struct_point_to (lw6sys_context_t * sys_context, _lw6ker_game_struct_t * game_struct, const lw6map_level_t * level)
{
  game_struct->level = level;
}

/**
 * lw6ker_game_struct_point_to
 *
 * @sys_context: global system context
 * @game_struct: the game_struct to modify
 * @level: the level to point to
 *
 * This can be used when one makes a copy (dup) of a level and for some
 * reason want the game_struct to point on this new copy. Of course you should
 * make the game_struct point to a level that is identical to the one
 * that was used to construct the object in the first place. Use at your own
 * risk.
 *
 * Return value: none
 */
void
lw6ker_game_struct_point_to (lw6sys_context_t * sys_context, lw6ker_game_struct_t * game_struct, const lw6map_level_t * level)
{
  _lw6ker_game_struct_point_to (sys_context, (_lw6ker_game_struct_t *) game_struct, level);
}

int
_lw6ker_game_struct_memory_footprint (lw6sys_context_t * sys_context, const _lw6ker_game_struct_t * game_struct)
{
  int ret = 0;

  // todo

  return ret;
}

/**
 * lw6ker_game_struct_memory_footprint
 *
 * @sys_context: global system context
 * @game_struct: the game_struct to query
 *
 * Returns the approximative amount of memory taken by the object.
 *
 * Return value: number of bytes (approximation)
 */
int
lw6ker_game_struct_memory_footprint (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct)
{
  return _lw6ker_game_struct_memory_footprint (sys_context, (const _lw6ker_game_struct_t *) game_struct);
}

char *
_lw6ker_game_struct_repr (lw6sys_context_t * sys_context, const _lw6ker_game_struct_t * game_struct)
{
  char *ret = NULL;
  float compression = 0.0f;

  if (game_struct)
    {
      /*
       * do not use references to level here,
       * function is called when freeing the object
       */
      compression = _lw6ker_map_struct_get_compression (sys_context, &(game_struct->map_struct)) * 100.0f;

      ret =
	lw6sys_new_sprintf (sys_context, "%u (%dx%dx%d, compression=%0.1f%%)",
			    game_struct->id, game_struct->map_struct.shape.w, game_struct->map_struct.shape.h, game_struct->map_struct.shape.d, compression);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't generate string id for NULL game_struct"));
    }

  return ret;
}

/**
 * lw6ker_game_struct_repr
 *
 * @sys_context: global system context
 * @game_struct: the game_struct to query
 *
 * Gives a readable representation of the object.
 *
 * Return value: newly allocated string, must be freed
 */
char *
lw6ker_game_struct_repr (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct)
{
  return _lw6ker_game_struct_repr (sys_context, (const _lw6ker_game_struct_t *) game_struct);
}

_lw6ker_game_struct_t *
_lw6ker_game_struct_dup (lw6sys_context_t * sys_context, const _lw6ker_game_struct_t * game_struct, lw6sys_progress_t * progress)
{
  _lw6ker_game_struct_t *ret = NULL;

  lw6sys_progress_begin (sys_context, progress);

  ret = (_lw6ker_game_struct_t *) LW6SYS_MALLOC (sys_context, sizeof (_lw6ker_game_struct_t));
  if (ret)
    {
      memcpy (ret, game_struct, sizeof (_lw6ker_game_struct_t));
      lw6sys_progress_update (sys_context, progress, 0, 8, 1);
      /*
       * Set id after the memcpy, else will be overwritten
       */
      _lw6ker_game_struct_set_id (sys_context, ret);
      ret->map_struct.places = (_lw6ker_place_struct_t *) LW6SYS_MALLOC (sys_context, sizeof (_lw6ker_place_struct_t) * ret->map_struct.nb_places);
      lw6sys_progress_update (sys_context, progress, 0, 8, 2);
      if (ret->map_struct.places)
	{
	  memcpy (ret->map_struct.places, game_struct->map_struct.places, sizeof (_lw6ker_place_struct_t) * ret->map_struct.nb_places);
	}
      lw6sys_progress_update (sys_context, progress, 0, 8, 3);
      ret->map_struct.zones = (_lw6ker_zone_struct_t *) LW6SYS_MALLOC (sys_context, sizeof (_lw6ker_zone_struct_t) * ret->map_struct.nb_zones);
      lw6sys_progress_update (sys_context, progress, 0, 8, 4);
      if (ret->map_struct.zones)
	{
	  memcpy (ret->map_struct.zones, game_struct->map_struct.zones, sizeof (_lw6ker_zone_struct_t) * ret->map_struct.nb_zones);
	}
      lw6sys_progress_update (sys_context, progress, 0, 8, 5);
      ret->map_struct.slots = (_lw6ker_slot_struct_t *) LW6SYS_MALLOC (sys_context, sizeof (_lw6ker_slot_struct_t) * ret->map_struct.nb_slots);
      lw6sys_progress_update (sys_context, progress, 0, 8, 6);
      if (ret->map_struct.slots)
	{
	  memcpy (ret->map_struct.slots, game_struct->map_struct.slots, sizeof (_lw6ker_slot_struct_t) * ret->map_struct.nb_slots);
	}
      lw6sys_progress_update (sys_context, progress, 0, 8, 7);
      if ((!ret->map_struct.zones) || (!ret->map_struct.slots))
	{
	  if (ret->map_struct.zones)
	    {
	      LW6SYS_FREE (sys_context, ret->map_struct.zones);
	    }
	  if (ret->map_struct.slots)
	    {
	      LW6SYS_FREE (sys_context, ret->map_struct.slots);
	    }
	  LW6SYS_FREE (sys_context, ret);
	  ret = NULL;
	}
    }

  if (ret)
    {
      if (_lw6ker_game_struct_checksum (sys_context, ret) == _lw6ker_game_struct_checksum (sys_context, game_struct))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("game_struct dup %d->%d"), game_struct->id, ret->id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("game_struct checkum mismatch after dup"));
	}
    }

  lw6sys_progress_end (sys_context, progress);


  return ret;
}

/**
 * lw6ker_game_struct_dup
 *
 * @sys_context: global system context
 * @game_struct: the game_struct to copy
 * @progress: progress indicator
 *
 * Dups (copy) a game_struct object. The newly created object points to
 * the same game_struct but is an independant copy, you can play
 * a whole different game on it. In practice this is often used
 * to create the game_struct objects for anticipation in network games.
 *
 * Return value: newly created object
 */
lw6ker_game_struct_t *
lw6ker_game_struct_dup (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct, lw6sys_progress_t * progress)
{
  return (lw6ker_game_struct_t *) _lw6ker_game_struct_dup (sys_context, (const _lw6ker_game_struct_t *) game_struct, progress);
}

void
_lw6ker_game_struct_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_game_struct_t * game_struct, u_int32_t * checksum)
{
  lw6map_rules_update_checksum (sys_context, &(game_struct->rules), checksum);
  _lw6ker_map_struct_update_checksum (sys_context, &(game_struct->map_struct), checksum);
}

u_int32_t
_lw6ker_game_struct_checksum (lw6sys_context_t * sys_context, const _lw6ker_game_struct_t * game_struct)
{
  u_int32_t ret = 0;

  _lw6ker_game_struct_update_checksum (sys_context, game_struct, &ret);

  return ret;
}

/**
 * lw6ker_game_struct_checksum
 *
 * @sys_context: global system context
 * @game_struct: the game_struct to query
 *
 * Calculates the checksum of a game_struct, this can be very usefull
 * to make sure two structs are identicall (prevent network errors and/or
 * cheating).
 *
 * Return value: 32-bit checksum
 */
u_int32_t
lw6ker_game_struct_checksum (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct)
{
  return _lw6ker_game_struct_checksum (sys_context, (const _lw6ker_game_struct_t *) game_struct);
}

/**
 * lw6ker_game_struct_get_shape
 *
 * @sys_context: global system context
 * @game_struct: the game_struct to query
 * @shape: the shape (out param)
 *
 * Retrieves the shape (w*h*d)of the game_struct.
 *
 * Return value: none.
 */
void
lw6ker_game_struct_get_shape (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct, lw6sys_whd_t * shape)
{
  (*shape) = ((const _lw6ker_game_struct_t *) game_struct)->map_struct.shape;
}

/**
 * lw6ker_game_struct_get_w
 *
 * @sys_context: global system context
 * @game_struct: the game_struct to query
 *
 * Retrieves the width (shape.w) of the game_struct.
 *
 * Return value: the width.
 */
int
lw6ker_game_struct_get_w (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct)
{
  return ((const _lw6ker_game_struct_t *) game_struct)->map_struct.shape.w;
}

/**
 * lw6ker_game_struct_get_h
 *
 * @sys_context: global system context
 * @game_struct: the game_struct to query
 *
 * Retrieves the height (shape.h) of the game_struct.
 *
 * Return value: the height.
 */
int
lw6ker_game_struct_get_h (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct)
{
  return ((const _lw6ker_game_struct_t *) game_struct)->map_struct.shape.h;
}

/**
 * lw6ker_game_struct_get_d
 *
 * @sys_context: global system context
 * @game_struct: the game_struct to query
 *
 * Retrieves the depth (shape.d, AKA number of layers) of the game_struct.
 *
 * Return value: the depth.
 */
int
lw6ker_game_struct_get_d (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct)
{
  return ((const _lw6ker_game_struct_t *) game_struct)->map_struct.shape.d;
}

/**
 * lw6ker_game_struct_is_fg
 *
 * @sys_context: global system context
 * @game_struct: the game_struct to query
 * @x: x position
 * @y: y position
 * @z: z position
 *
 * Tests wether a given position is foreground, that is, occupied by a wall.
 *
 * Return value: 1 if foreground (wall, fighters can't move), 0 if not
 */
int
lw6ker_game_struct_is_fg (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct, int32_t x, int32_t y, int32_t z)
{
  return (_lw6ker_map_struct_get_zone_id (&(((const _lw6ker_game_struct_t *) game_struct)->map_struct), x, y, z) < 0);
}

/**
 * lw6ker_game_struct_is_bg
 *
 * @sys_context: global system context
 * @game_struct: the game_struct to query
 * @x: x position
 * @y: y position
 * @z: z position
 *
 * Tests wether a given position is background, that is, there's no wall.
 *
 * Return value: 1 if background (wall, fighters can move), 0 if not
 */
int
lw6ker_game_struct_is_bg (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct, int32_t x, int32_t y, int32_t z)
{
  return (_lw6ker_map_struct_get_zone_id (&(((const _lw6ker_game_struct_t *) game_struct)->map_struct), x, y, z) >= 0);
}

/**
 * lw6ker_game_struct_get_zones_info
 *
 * @sys_context: global system context
 * @game_struct: game_struct to query
 * @nb_zones: the maximum zone size (out param, can be NULL)
 * @max_zone_size: the maximum zone size (out param, can be NULL)
 *
 * This function gets information about the internal zoning system, can
 * be used for debugging.
 *
 * Return value: none.
 */
void
lw6ker_game_struct_get_zones_info (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct, int *nb_zones, int *max_zone_size)
{
  if (nb_zones)
    {
      (*nb_zones) = ((const _lw6ker_game_struct_t *) game_struct)->map_struct.nb_zones;
    }
  if (max_zone_size)
    {
      (*max_zone_size) = ((const _lw6ker_game_struct_t *) game_struct)->map_struct.max_zone_size;
    }
}

/**
 * lw6ker_game_struct_get_zone_info
 *
 * @sys_context: global system context
 * @game_struct: game_struct to query
 * @i: index of the zone to query
 * @zone_pos: coord of the zone, top-left corner (out param, can be NULL)
 * @zone_size: size of the zone (out param, can be NULL)
 *
 * This function gets information about the internal zoning system, can
 * be used for debugging.
 *
 * Return value: none
 */
void
lw6ker_game_struct_get_zone_info (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct, int i, lw6sys_xyz_t * zone_pos, int *zone_size)
{
  _lw6ker_zone_struct_t *zone;

  zone = &(((const _lw6ker_game_struct_t *) game_struct)->map_struct.zones[i]);
  if (zone_pos)
    {
      (*zone_pos) = zone->pos;
    }
  if (zone_size)
    {
      (*zone_size) = zone->size;
    }
}

/**
 * lw6ker_game_struct_get_zone_id
 *
 * @sys_context: global system context
 * @game_struct: the game_struct to query
 * @x: x pos
 * @y: y pos
 * @z: z pos
 *
 * Gets the zone id for a given position. The id returned can then be
 * used to query for a potential, for instance.
 *
 * Return value: the zone id
 */
int32_t
lw6ker_game_struct_get_zone_id (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct, int32_t x, int32_t y, int32_t z)
{
  return _lw6ker_map_struct_get_zone_id (&(((const _lw6ker_game_struct_t *) game_struct)->map_struct), x, y, z);
}

/**
 * lw6ker_game_struct_find_free_slot_near
 *
 * @sys_context: global system context
 * @game_struct: the game_struct to query
 * @there: the closest free slot (out param)
 * @here: where we'd like to be
 *
 * Tries to find the closest free slot (there) near a given position (here).
 * This is typically used internally to find out where to apply the cursor
 * when it's flying over walls.
 *
 * Return value: none
 */
void
lw6ker_game_struct_find_free_slot_near (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct, lw6sys_xyz_t * there, lw6sys_xyz_t here)
{
  _lw6ker_map_struct_find_free_slot_near (sys_context, &(((const _lw6ker_game_struct_t *) game_struct)->map_struct), there, here);
}
