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
#include <math.h>

#include "ker.h"
#include "ker-internal.h"

/*
 * The code in this file is heavily inspired from Liquid War 5,
 * it's an adaptation of "src/mesh.c".
 */

/*
 * Internal structure used just when creating the mesh.
 * The idea is to have w*h zones, of size 1x1. Of course
 * these will be grouped afterwards, but when creating the
 * zones, it's usefull to have this in memory.
 * This was called "MESHER" in Liquid War 5.
 */
typedef struct _draft_zone_s
{
  lw6sys_xyz_t pos;
  int32_t used;
  int32_t size;
  int32_t link[LW6KER_NB_DIRS];
  int32_t corres;
}
_draft_zone_t;

typedef struct _draft_zones_s
{
  lw6sys_whd_t shape;
  int32_t max_zone_size;
  _draft_zone_t *zones;
}
_draft_zones_t;

void
_lw6ker_map_struct_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_map_struct_t * map_struct, u_int32_t * checksum)
{
  int i;

  lw6sys_checksum_update_whd (sys_context, checksum, &(map_struct->shape));
  lw6sys_checksum_update_int32 (sys_context, checksum, map_struct->nb_places);
  lw6sys_checksum_update_int32 (sys_context, checksum, map_struct->nb_zones);
  lw6sys_checksum_update_int32 (sys_context, checksum, map_struct->nb_slots);
  lw6sys_checksum_update_int32 (sys_context, checksum, map_struct->room_for_armies);
  lw6sys_checksum_update_int32 (sys_context, checksum, map_struct->max_zone_size);
  for (i = 0; i < map_struct->nb_places; ++i)
    {
      _lw6ker_place_struct_update_checksum (sys_context, &(map_struct->places[i]), checksum);
    }
  for (i = 0; i < map_struct->nb_zones; ++i)
    {
      _lw6ker_zone_struct_update_checksum (sys_context, &(map_struct->zones[i]), checksum);
    }
  for (i = 0; i < map_struct->nb_slots; ++i)
    {
      _lw6ker_slot_struct_update_checksum (sys_context, &(map_struct->slots[i]), checksum);
    }
}

int
_lw6ker_map_struct_lazy_compare (lw6sys_context_t * sys_context, const _lw6ker_map_struct_t * map_struct_a, const _lw6ker_map_struct_t * map_struct_b)
{
  int ret = 0;

  if (map_struct_a && map_struct_b)
    {
      if (map_struct_a == map_struct_b)
	{
	  ret = 1;
	}
      else
	{
	  ret = (map_struct_a->shape.w == map_struct_b->shape.w) &&
	    (map_struct_a->shape.h == map_struct_b->shape.h) &&
	    (map_struct_a->shape.d == map_struct_b->shape.d) &&
	    (map_struct_a->nb_places == map_struct_b->nb_places) &&
	    (map_struct_a->nb_zones == map_struct_b->nb_zones) &&
	    (map_struct_a->nb_slots == map_struct_b->nb_slots) &&
	    (map_struct_a->room_for_armies == map_struct_b->room_for_armies) && (map_struct_a->max_zone_size == map_struct_b->max_zone_size);
	}
    }

  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("map_structs do not look the same"));
    }

  return ret;
}

static int32_t
_relative_index_no_check (const lw6map_level_t * level, int32_t x, int32_t y, int32_t z)
{
  return z * level->body.shape.w * level->body.shape.h + y * level->body.shape.w + x;
}

static int32_t
_relative_index (const lw6map_level_t * level, int32_t x, int32_t y, int32_t z)
{
  lw6map_coords_fix_xy (&(level->param.rules), &(level->body.shape), &x, &y);
  lw6map_coords_fix_z (&(level->param.rules), &(level->body.shape), &z);

  return _relative_index_no_check (level, x, y, z);
}

/*
 * Creates a new _draft_zones_t object. The idea is to create a structure which
 * is of the same nature as the final MAP_STRUCT "zones", but unoptimized.
 * We create a structure (a "MESH" in Liquid War 5)) where each square
 * has pointers on its neighbours. As there are 12 directions, these is
 * very redundant. This is just a temporary struct which will be
 * optimized afterwards.
 */
static _draft_zones_t *
_draft_zones_new (lw6sys_context_t * sys_context, const lw6map_level_t * level, lw6sys_progress_t * progress)
{
  _draft_zones_t *ret;

  lw6sys_progress_begin (sys_context, progress);

  ret = (_draft_zones_t *) LW6SYS_CALLOC (sys_context, sizeof (_draft_zones_t));
  if (ret)
    {
      int32_t w, h, d, size;

      ret->shape = level->body.shape;
      ret->max_zone_size = 1;

      w = ret->shape.w;
      h = ret->shape.h;
      d = ret->shape.d;
      size = w * h * d;

      if (size >= LW6MAP_MIN_BODY_SURFACE)
	{
	  ret->zones = (_draft_zone_t *) LW6SYS_CALLOC (sys_context, size * sizeof (_draft_zone_t));

	  if (ret->zones)
	    {
	      int32_t i, j, x, y, z;

	      for (i = 0; i < size; ++i)
		{
		  ret->zones[i].size = 1;
		  for (j = 0; j < LW6KER_NB_DIRS; ++j)
		    {
		      ret->zones[i].link[j] = -1;
		    }
		}

	      for (z = 0; z < d; ++z)
		{
		  for (y = 0; y < h; ++y)
		    {
		      for (x = 0; x < w; ++x)
			{
			  i = _relative_index_no_check (level, x, y, z);

			  ret->zones[i].pos.x = x;
			  ret->zones[i].pos.y = y;
			  ret->zones[i].pos.z = z;
			  ret->zones[i].used = lw6map_body_get (&(level->body), x, y, z) > 0 ? 1 : 0;
			}
		    }
		}

	      for (z = 0; z < d; ++z)
		{
		  for (y = 0; y < h; ++y)
		    {
		      for (x = 0; x < w; ++x)
			{
			  i = _relative_index (level, x, y, z);
			  if (ret->zones[i].used)
			    {
			      j = _relative_index (level, x, y - 1, z);
			      if (ret->zones[j].used && i != j)
				{
				  ret->zones[i].link[LW6KER_DIR_NNW] = j;
				  ret->zones[i].link[LW6KER_DIR_NNE] = j;
				}
			      j = _relative_index (level, x + 1, y - 1, z);
			      if (ret->zones[j].used && i != j)
				{
				  ret->zones[i].link[LW6KER_DIR_NE] = j;
				}
			      j = _relative_index (level, x + 1, y, z);
			      if (ret->zones[j].used && i != j)
				{
				  ret->zones[i].link[LW6KER_DIR_ENE] = j;
				  ret->zones[i].link[LW6KER_DIR_ESE] = j;
				}
			      j = _relative_index (level, x + 1, y + 1, z);
			      if (ret->zones[j].used && i != j)
				{
				  ret->zones[i].link[LW6KER_DIR_SE] = j;
				}
			      j = _relative_index (level, x, y + 1, z);
			      if (ret->zones[j].used && i != j)
				{
				  ret->zones[i].link[LW6KER_DIR_SSE] = j;
				  ret->zones[i].link[LW6KER_DIR_SSW] = j;
				}
			      j = _relative_index (level, x - 1, y + 1, z);
			      if (ret->zones[j].used && i != j)
				{
				  ret->zones[i].link[LW6KER_DIR_SW] = j;
				}
			      j = _relative_index (level, x - 1, y, z);
			      if (ret->zones[j].used && i != j)
				{
				  ret->zones[i].link[LW6KER_DIR_WSW] = j;
				  ret->zones[i].link[LW6KER_DIR_WNW] = j;
				}
			      j = _relative_index (level, x - 1, y - 1, z);
			      if (ret->zones[j].used && i != j)
				{
				  ret->zones[i].link[LW6KER_DIR_NW] = j;
				}
			      j = _relative_index (level, x, y, z - 1);
			      if (ret->zones[j].used && i != j)
				{
				  ret->zones[i].link[LW6KER_DIR_UP] = j;
				}
			      j = _relative_index (level, x, y, z + 1);
			      if (ret->zones[j].used && i != j)
				{
				  ret->zones[i].link[LW6KER_DIR_DOWN] = j;
				}
			    }
			}
		    }
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to allocate %d draft zones"), size);
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("map is too small, size=%d, LW6MAP_MIN_BODY_SURFACE=%d"), size, LW6MAP_MIN_BODY_SURFACE);
	}

    }

  lw6sys_progress_end (sys_context, progress);

  return ret;
}

static void
_draft_zones_free (lw6sys_context_t * sys_context, _draft_zones_t * draft_zones)
{
  if (draft_zones)
    {
      if (draft_zones->zones)
	{
	  LW6SYS_FREE (sys_context, draft_zones->zones);
	}
      LW6SYS_FREE (sys_context, draft_zones);
    }
}

/*
 * Here we scan the whole _draft_zones_t data, and whenever we find
 * 4 zones which form a square and respect "some conditions",
 * we group them into a bigger zone. The only thing to be carefull
 * about is to handle correctly all the pointers (links) on
 * other zones, especially those that link back to the squares
 * we suppress.
 */
static int32_t
_draft_zones_group (lw6sys_context_t * sys_context, const lw6map_level_t * level, _draft_zones_t * draft_zones, int32_t step)
{
  int32_t found = 0;
  int32_t x, y, z, w, h, d, size, i, j, k;
  int32_t i_ne, i_se, i_sw, i_nw, i_up, i_down;
  _draft_zone_t *zone_ne, *zone_se, *zone_sw, *zone_nw, *zone_up, *zone_down;
  int32_t *test;

  w = draft_zones->shape.w;
  h = draft_zones->shape.h;
  d = draft_zones->shape.d;
  size = d * h * w;

  for (z = 0; z < d; z++)
    {
      for (y = 0; y < h - step; y += step * 2)
	{
	  for (x = 0; x < w - step; x += step * 2)
	    {
	      i = z * w * h + y * w + x;
	      if (draft_zones->zones[i].used)
		{
		  i_ne = i + step;
		  i_se = i + (w + 1) * step;
		  i_sw = i + w * step;
		  i_nw = i;
		  zone_ne = &draft_zones->zones[i_ne];
		  zone_se = &draft_zones->zones[i_se];
		  zone_sw = &draft_zones->zones[i_sw];
		  zone_nw = &draft_zones->zones[i_nw];

		  i_up = i - w * h;	// may be out of bounds
		  if (i_up >= 0 && i_up < size)
		    {
		      zone_up = &draft_zones->zones[i_up];
		    }
		  else
		    {
		      zone_up = NULL;
		    }
		  i_down = i + w * h;	// may be out of bounds
		  if (i_down >= 0 && i_down < size)
		    {
		      zone_down = &draft_zones->zones[i_down];
		    }
		  else
		    {
		      zone_down = NULL;
		    }

		  /*
		   * This test contains all the criteria which make
		   * us decide wether we group the squares are not.
		   * It's a wise choice to be rather restrictive.
		   * For instance the fact tha NE SW NW NE links
		   * actually point to something might seem useless.
		   * It's not, the main reason is that it's a rather
		   * good choice that whenever there's an obstacle,
		   * the zones around it are forced to be small.
		   */
		  if (zone_ne->used && zone_ne->size == step
		      && zone_se->used && zone_se->size == step
		      && zone_sw->used && zone_sw->size == step
		      && zone_nw->used && zone_nw->size == step
		      && zone_ne->link[LW6KER_DIR_NNW] ==
		      zone_ne->link[LW6KER_DIR_NNE]
		      && zone_ne->link[LW6KER_DIR_ENE] ==
		      zone_ne->link[LW6KER_DIR_ESE]
		      && zone_se->link[LW6KER_DIR_ENE] ==
		      zone_se->link[LW6KER_DIR_ESE]
		      && zone_se->link[LW6KER_DIR_SSE] ==
		      zone_se->link[LW6KER_DIR_SSW]
		      && zone_sw->link[LW6KER_DIR_SSE] ==
		      zone_sw->link[LW6KER_DIR_SSW]
		      && zone_sw->link[LW6KER_DIR_WSW] ==
		      zone_sw->link[LW6KER_DIR_WNW]
		      && zone_nw->link[LW6KER_DIR_WSW] ==
		      zone_nw->link[LW6KER_DIR_WNW]
		      && zone_nw->link[LW6KER_DIR_NNW] ==
		      zone_nw->link[LW6KER_DIR_NNE]
		      && zone_ne->link[LW6KER_DIR_NE] >= 0
		      && zone_se->link[LW6KER_DIR_SE] >= 0
		      && zone_sw->link[LW6KER_DIR_SW] >= 0
		      && zone_nw->link[LW6KER_DIR_NW] >= 0
		      && ((!zone_up) || (zone_up && zone_up->size >= step)) && ((!zone_down) || (zone_down && zone_down->size >= step)))
		    {
		      /*
		       * Delete unused zones
		       */
		      zone_ne->used = 0;
		      zone_se->used = 0;
		      zone_sw->used = 0;

		      /*
		       * Double the zone size of the remaining zone
		       */
		      zone_nw->size = step * 2;

		      /*
		       * Change the links of the remaining zone
		       */
		      zone_nw->link[LW6KER_DIR_NNE] = zone_ne->link[LW6KER_DIR_NNE];
		      zone_nw->link[LW6KER_DIR_NE] = zone_ne->link[LW6KER_DIR_NE];
		      zone_nw->link[LW6KER_DIR_ENE] = zone_ne->link[LW6KER_DIR_ENE];
		      zone_nw->link[LW6KER_DIR_ESE] = zone_se->link[LW6KER_DIR_ESE];
		      zone_nw->link[LW6KER_DIR_SE] = zone_se->link[LW6KER_DIR_SE];
		      zone_nw->link[LW6KER_DIR_SSE] = zone_se->link[LW6KER_DIR_SSE];
		      zone_nw->link[LW6KER_DIR_SSW] = zone_sw->link[LW6KER_DIR_SSW];
		      zone_nw->link[LW6KER_DIR_SW] = zone_sw->link[LW6KER_DIR_SW];
		      zone_nw->link[LW6KER_DIR_WSW] = zone_sw->link[LW6KER_DIR_WSW];

		      /*
		       * Change all the links of the adjacent zones,
		       * which were pointing on the deleted zones,
		       * and must now point on the remaining one.
		       */
		      for (j = 0; j < LW6KER_DIR_UP; ++j)
			{
			  for (k = 0; k < LW6KER_DIR_UP; ++k)
			    {
			      if (zone_nw->link[j] >= 0)
				{
				  test = &(draft_zones->zones[zone_nw->link[j]].link[k]);
				  if (*test == i_ne || *test == i_se || *test == i_sw)
				    {
				      *test = i_nw;
				    }
				}
			    }
			}

		      /*
		       * Now treat vertical (cross-layer) zones links
		       * which were pointing on the deleted zones
		       */
		      if (d > 1)
			{
			  j = _relative_index (level, draft_zones->zones[i_ne].pos.x, draft_zones->zones[i_ne].pos.y, draft_zones->zones[i_ne].pos.z + 1);
			  if (draft_zones->zones[j].used)
			    {
			      draft_zones->zones[j].link[LW6KER_DIR_UP] = i_nw;
			    }
			  j = _relative_index (level, draft_zones->zones[i_se].pos.x, draft_zones->zones[i_se].pos.y, draft_zones->zones[i_se].pos.z + 1);
			  if (draft_zones->zones[j].used)
			    {
			      draft_zones->zones[j].link[LW6KER_DIR_UP] = i_nw;
			    }
			  j = _relative_index (level, draft_zones->zones[i_sw].pos.x, draft_zones->zones[i_sw].pos.y, draft_zones->zones[i_sw].pos.z + 1);
			  if (draft_zones->zones[j].used)
			    {
			      draft_zones->zones[j].link[LW6KER_DIR_UP] = i_nw;
			    }
			  j = _relative_index (level, draft_zones->zones[i_ne].pos.x, draft_zones->zones[i_ne].pos.y, draft_zones->zones[i_ne].pos.z - 1);
			  if (draft_zones->zones[j].used)
			    {
			      draft_zones->zones[j].link[LW6KER_DIR_DOWN] = i_nw;
			    }
			  j = _relative_index (level, draft_zones->zones[i_se].pos.x, draft_zones->zones[i_se].pos.y, draft_zones->zones[i_se].pos.z - 1);
			  if (draft_zones->zones[j].used)
			    {
			      draft_zones->zones[j].link[LW6KER_DIR_DOWN] = i_nw;
			    }
			  j = _relative_index (level, draft_zones->zones[i_sw].pos.x, draft_zones->zones[i_sw].pos.y, draft_zones->zones[i_sw].pos.z - 1);
			  if (draft_zones->zones[j].used)
			    {
			      draft_zones->zones[j].link[LW6KER_DIR_DOWN] = i_nw;
			    }
			}

		      draft_zones->max_zone_size = zone_nw->size;
		      ++found;
		    }
		}
	    }
	}
    }

  return found;
}

static int
_fix_one_way (lw6sys_context_t * sys_context, _lw6ker_map_struct_t * map_struct, const lw6map_level_t * level)
{
  int ret = 0;
  int x, y;
  int i;
  int one_way_north = 0;
  int one_way_east = 0;
  int one_way_south = 0;
  int one_way_west = 0;
  int allow_north = 0;
  int allow_east = 0;
  int allow_south = 0;
  int allow_west = 0;
  _lw6ker_zone_struct_t *zone_struct;

  for (i = 0; i < map_struct->nb_zones; ++i)
    {
      zone_struct = &(map_struct->zones[i]);
      for (y = 0; y < zone_struct->size; y++)
	{
	  for (x = 0; x < zone_struct->size; x++)
	    {
	      one_way_north = 0;
	      one_way_east = 0;
	      one_way_south = 0;
	      one_way_west = 0;
	      one_way_north += lw6map_meta_layer_get (sys_context, &level->body.one_way_north, zone_struct->pos.x + x, zone_struct->pos.y + y);
	      one_way_east += lw6map_meta_layer_get (sys_context, &level->body.one_way_east, zone_struct->pos.x + x, zone_struct->pos.y + y);
	      one_way_south += lw6map_meta_layer_get (sys_context, &level->body.one_way_south, zone_struct->pos.x + x, zone_struct->pos.y + y);
	      one_way_west += lw6map_meta_layer_get (sys_context, &level->body.one_way_west, zone_struct->pos.x + x, zone_struct->pos.y + y);
	    }
	}
      if (one_way_north == one_way_south)
	{
	  /*
	   * This traps the case where both are equal to zero and
	   * also the case where there's a conflict between two
	   * directions. In such a case we just leave the way open
	   * rather than risk isolated zones.
	   */
	  allow_north = 1;
	  allow_south = 1;
	}
      else
	{
	  if (one_way_north > one_way_south)
	    {
	      allow_north = 1;
	      allow_south = 0;
	    }
	  else
	    {
	      allow_north = 0;
	      allow_south = 1;
	    }
	}
      if (one_way_east == one_way_west)
	{
	  /*
	   * This traps the case where both are equal to zero and
	   * also the case where there's a conflict between two
	   * directions. In such a case we just leave the way open
	   * rather than risk isolated zones.
	   */
	  allow_east = 1;
	  allow_west = 1;
	}
      else
	{
	  if (one_way_east > one_way_west)
	    {
	      allow_east = 1;
	      allow_west = 0;
	    }
	  else
	    {
	      allow_east = 0;
	      allow_west = 1;
	    }
	}

      zone_struct->one_way_dir_even = -1;
      zone_struct->one_way_dir_odd = -1;
      if ((!allow_north) && allow_east && allow_west)
	{
	  zone_struct->one_way_dir_even = LW6KER_DIR_SSE;
	  zone_struct->one_way_dir_odd = LW6KER_DIR_SSW;
	}
      if ((!allow_east) && allow_north && allow_south)
	{
	  zone_struct->one_way_dir_even = LW6KER_DIR_WSW;
	  zone_struct->one_way_dir_odd = LW6KER_DIR_WNW;
	}
      if ((!allow_south) && allow_east && allow_west)
	{
	  zone_struct->one_way_dir_even = LW6KER_DIR_NNW;
	  zone_struct->one_way_dir_odd = LW6KER_DIR_NNE;
	}
      if ((!allow_west) && allow_north && allow_south)
	{
	  zone_struct->one_way_dir_even = LW6KER_DIR_ENE;
	  zone_struct->one_way_dir_odd = LW6KER_DIR_ESE;
	}
      if ((!allow_north) && (!allow_east))
	{
	  zone_struct->one_way_dir_even = LW6KER_DIR_SW;
	  zone_struct->one_way_dir_odd = LW6KER_DIR_SW;
	}
      if ((!allow_east) && (!allow_south))
	{
	  zone_struct->one_way_dir_even = LW6KER_DIR_NW;
	  zone_struct->one_way_dir_odd = LW6KER_DIR_NW;
	}
      if ((!allow_south) && (!allow_west))
	{
	  zone_struct->one_way_dir_even = LW6KER_DIR_NE;
	  zone_struct->one_way_dir_odd = LW6KER_DIR_NE;
	}
      if ((!allow_west) && (!allow_north))
	{
	  zone_struct->one_way_dir_even = LW6KER_DIR_SE;
	  zone_struct->one_way_dir_odd = LW6KER_DIR_SE;
	}
      if (!allow_north)
	{
	  zone_struct->link[LW6KER_DIR_ESE] = -1;
	  zone_struct->link[LW6KER_DIR_SE] = -1;
	  zone_struct->link[LW6KER_DIR_SSE] = -1;
	  zone_struct->link[LW6KER_DIR_SSW] = -1;
	  zone_struct->link[LW6KER_DIR_SW] = -1;
	  zone_struct->link[LW6KER_DIR_WSW] = -1;
	}
      if (!allow_east)
	{
	  zone_struct->link[LW6KER_DIR_SSW] = -1;
	  zone_struct->link[LW6KER_DIR_SW] = -1;
	  zone_struct->link[LW6KER_DIR_WSW] = -1;
	  zone_struct->link[LW6KER_DIR_WNW] = -1;
	  zone_struct->link[LW6KER_DIR_NW] = -1;
	  zone_struct->link[LW6KER_DIR_NNW] = -1;
	}
      if (!allow_south)
	{
	  zone_struct->link[LW6KER_DIR_WNW] = -1;
	  zone_struct->link[LW6KER_DIR_NW] = -1;
	  zone_struct->link[LW6KER_DIR_NNW] = -1;
	  zone_struct->link[LW6KER_DIR_NNE] = -1;
	  zone_struct->link[LW6KER_DIR_NE] = -1;
	  zone_struct->link[LW6KER_DIR_ENE] = -1;
	}
      if (!allow_west)
	{
	  zone_struct->link[LW6KER_DIR_NNE] = -1;
	  zone_struct->link[LW6KER_DIR_NE] = -1;
	  zone_struct->link[LW6KER_DIR_ENE] = -1;
	  zone_struct->link[LW6KER_DIR_ESE] = -1;
	  zone_struct->link[LW6KER_DIR_SE] = -1;
	  zone_struct->link[LW6KER_DIR_SSE] = -1;
	}
    }

  ret = 1;

  return ret;
}

/*
 * Transform the temporary _draft_zones_t into the proper
 * final MAP_STRUCT. The major change is to actually
 * delete and remove from memory all the unused ZONES.
 * We "compact" the zones so that instead of using
 * tons of RAM, we use just what we need. While this
 * complicates a little bit the algorithms (we can't
 * assume any more than y * w + x = i) reducing memory
 * usage is always a good thing, the more we reduce it,
 * the more we get a chance to have things cached.
 */
static int
_draft_zones_to_map_struct (lw6sys_context_t * sys_context, _lw6ker_map_struct_t * map_struct, _draft_zones_t * draft_zones, lw6sys_progress_t * progress)
{
  int ret = 0;
  int32_t w, h, d, size, nb_zones;
  int32_t i, j, k, temp;

  lw6sys_progress_begin (sys_context, progress);

  w = draft_zones->shape.w;
  h = draft_zones->shape.h;
  d = draft_zones->shape.d;
  size = w * h * d;
  nb_zones = 0;

  for (i = 0; i < size; ++i)
    {
      if (draft_zones->zones[i].used)
	{
	  ++nb_zones;
	}
    }

  map_struct->shape = draft_zones->shape;
  map_struct->nb_places = w * h;
  map_struct->nb_slots = w * h * d;
  map_struct->nb_zones = nb_zones;
  map_struct->max_zone_size = draft_zones->max_zone_size;

  map_struct->zones = (_lw6ker_zone_struct_t *) LW6SYS_MALLOC (sys_context, nb_zones * sizeof (_lw6ker_zone_struct_t));

  if (map_struct->zones)
    {
      j = 0;
      for (i = 0; i < size; ++i)
	{
	  if (draft_zones->zones[i].used)
	    {
	      map_struct->zones[j].pos = draft_zones->zones[i].pos;
	      map_struct->zones[j].size = draft_zones->zones[i].size;
	      for (k = 0; k < LW6KER_NB_DIRS; ++k)
		{
		  map_struct->zones[j].link[k] = draft_zones->zones[i].link[k];
		}
	      draft_zones->zones[i].corres = j;

	      j++;
	    }
	  else
	    {
	      draft_zones->zones[i].corres = -1;
	    }
	}

      for (j = 0; j < nb_zones; ++j)
	{
	  for (k = 0; k < LW6KER_NB_DIRS; ++k)
	    {
	      if ((temp = (map_struct->zones[j].link[k])) >= 0)
		{
		  map_struct->zones[j].link[k] = draft_zones->zones[temp].corres;
		}
	      else
		{
		  map_struct->zones[j].link[k] = -1;
		}
	    }
	}
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create zones for map struct"));
    }

  lw6sys_progress_end (sys_context, progress);

  return ret;
}

/*
 * Init places, that is, mostly read and aggregate meta-layer info
 */
static int
_init_places (lw6sys_context_t * sys_context, _lw6ker_map_struct_t * map_struct, const lw6map_level_t * level, lw6sys_progress_t * progress)
{
  int ret = 0;
  int x, y;
  int i;
  lw6sys_whd_t shape;
  int32_t act_incr = 0;
  int glue_power;
  int boost_power;
  int danger_power;
  int medicine_power;
  u_int32_t meta_layer_value;
  int act_incr_min = 0;
  int act_incr_max = 0;
  int danger_correction = 0;
  int medicine_correction = 0;

  lw6sys_progress_begin (sys_context, progress);

  shape = map_struct->shape;
  map_struct->places = (_lw6ker_place_struct_t *) LW6SYS_CALLOC (sys_context, map_struct->shape.w * map_struct->shape.h * sizeof (_lw6ker_place_struct_t));

  if (map_struct->places)
    {
      for (y = 0; y < shape.h; ++y)
	{
	  for (x = 0; x < shape.w; ++x)
	    {
	      map_struct->places[_lw6ker_map_struct_place_index (map_struct, x, y)].act_incr = _LW6KER_ACT_LIMIT;
	      /*
	       * no need to set health_correction to 0, was done
	       * by the CALLOC
	       */
	    }
	}

      glue_power = level->param.rules.glue_power;
      if (level->body.glue.data && glue_power > 0)
	{
	  if (lw6sys_shape_is_same_xy (sys_context, &level->body.glue.shape, &shape))
	    {
	      act_incr_min = _LW6KER_ACT_LIMIT / glue_power;
	      for (y = 0; y < shape.h; ++y)
		{
		  for (x = 0; x < shape.w; ++x)
		    {
		      meta_layer_value = lw6map_meta_layer_get (sys_context, &level->body.glue, x, y);
		      act_incr = ((0xFF - meta_layer_value) * _LW6KER_ACT_LIMIT + meta_layer_value * act_incr_min) / 0xFF - _LW6KER_ACT_LIMIT;
		      map_struct->places[_lw6ker_map_struct_place_index (map_struct, x, y)].act_incr += act_incr;
		    }
		}
	    }
	}

      boost_power = level->param.rules.boost_power;
      if (level->body.boost.data && boost_power > 0)
	{
	  if (lw6sys_shape_is_same_xy (sys_context, &level->body.boost.shape, &shape))
	    {
	      act_incr_max = _LW6KER_ACT_LIMIT * boost_power;
	      for (y = 0; y < shape.h; ++y)
		{
		  for (x = 0; x < shape.w; ++x)
		    {
		      meta_layer_value = lw6map_meta_layer_get (sys_context, &level->body.boost, x, y);
		      act_incr = ((0xFF - meta_layer_value) * _LW6KER_ACT_LIMIT + meta_layer_value * act_incr_max) / 0xFF - _LW6KER_ACT_LIMIT;
		      map_struct->places[_lw6ker_map_struct_place_index (map_struct, x, y)].act_incr += act_incr;
		    }
		}
	    }
	}

      danger_power = level->param.rules.danger_power;
      if (level->body.danger.data && danger_power > 0)
	{
	  if (lw6sys_shape_is_same_xy (sys_context, &level->body.danger.shape, &shape))
	    {
	      for (y = 0; y < shape.h; ++y)
		{
		  for (x = 0; x < shape.w; ++x)
		    {
		      meta_layer_value = lw6map_meta_layer_get (sys_context, &level->body.danger, x, y);
		      danger_correction = (meta_layer_value * danger_power) / 0xFF;
		      map_struct->places[_lw6ker_map_struct_place_index (map_struct, x, y)].health_correction -= danger_correction;
		    }
		}
	    }
	}

      medicine_power = level->param.rules.medicine_power;
      if (level->body.medicine.data && medicine_power > 0)
	{
	  if (lw6sys_shape_is_same_xy (sys_context, &level->body.medicine.shape, &shape))
	    {
	      for (y = 0; y < shape.h; ++y)
		{
		  for (x = 0; x < shape.w; ++x)
		    {
		      meta_layer_value = lw6map_meta_layer_get (sys_context, &level->body.medicine, x, y);
		      medicine_correction = (meta_layer_value * medicine_power) / 0xFF;
		      map_struct->places[_lw6ker_map_struct_place_index (map_struct, x, y)].health_correction += medicine_correction;
		    }
		}
	    }
	}

      for (y = 0; y < shape.h; ++y)
	{
	  for (x = 0; x < shape.w; ++x)
	    {
	      i = _lw6ker_map_struct_place_index (map_struct, x, y);
	      map_struct->places[i].act_incr = lw6sys_imax (1, map_struct->places[i].act_incr);
	      /*
	       * health_correction need not be fixed
	       */
	    }
	}

      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create places for map struct"));
    }

  lw6sys_progress_end (sys_context, progress);

  return ret;
}

/*
 * Create the slots, the main interest in doing this is to
 * have a quick way to get the right zone for a given (x,y).
 */
static int
_init_slots (lw6sys_context_t * sys_context, _lw6ker_map_struct_t * map_struct, const lw6map_level_t * level, lw6sys_progress_t * progress)
{
  int ret = 0;
  lw6sys_progress_t progress1;
  lw6sys_progress_t progress2;

  lw6sys_progress_split (sys_context, &progress1, &progress2, progress);

  lw6sys_progress_begin (sys_context, progress);

  map_struct->slots =
    (_lw6ker_slot_struct_t *) LW6SYS_CALLOC (sys_context, map_struct->shape.w * map_struct->shape.h * map_struct->shape.d * sizeof (_lw6ker_slot_struct_t));

  if (map_struct->slots)
    {
      int32_t i, x, y, z;

      _lw6ker_zone_struct_t *zone;

      /*
       * Set up the id lookup table
       */
      for (y = 0; y < map_struct->shape.h; ++y)
	{
	  lw6sys_progress_update (sys_context, &progress1, 0, map_struct->shape.h, y);
	  // z after y so that progress is more fluid
	  for (z = 0; z < map_struct->shape.d; ++z)
	    {
	      for (x = 0; x < map_struct->shape.w; ++x)
		{
		  _lw6ker_map_struct_set_zone_id (map_struct, x, y, z, -1);
		}
	    }
	}

      for (i = 0; i < map_struct->nb_zones; ++i)
	{
	  lw6sys_progress_update (sys_context, &progress2, 0, map_struct->nb_zones, i);
	  zone = &map_struct->zones[i];
	  for (y = 0; y < map_struct->zones[i].size; ++y)
	    {
	      for (x = 0; x < map_struct->zones[i].size; ++x)
		{
		  _lw6ker_map_struct_set_zone_id (map_struct, zone->pos.x + x, zone->pos.y + y, zone->pos.z, i);
		  map_struct->room_for_armies++;
		}
	    }
	}

      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create slots for map struct"));
    }

  lw6sys_progress_end (sys_context, progress);

  return ret;
}

/*
 * Returns a "useless" ratio which is "how much we compressed
 * the map by using all this ZONE wizardry. Just to keep track
 * of wether it's usefull to implement this. Debugging tool.
 */
float
_lw6ker_map_struct_get_compression (lw6sys_context_t * sys_context, const _lw6ker_map_struct_t * map_struct)
{
  float ret = 0.0f;

  if (map_struct->nb_slots > 0)
    {
      ret = ((float) map_struct->nb_zones) / ((float) (map_struct->nb_slots));
    }

  return ret;
}

/*
 * This is one key function: it calls the transformation of a lw6map_level_t,
 * that is a plain object which reflects only the content of PNG & XML
 * files, into an in-memory "ready for algorithmic stuff" structure.
 */
int
_lw6ker_map_struct_init (lw6sys_context_t * sys_context, _lw6ker_map_struct_t * map_struct, const lw6map_level_t * level, lw6sys_progress_t * progress)
{
  int ret = 0;
  _draft_zones_t *_draft_zones;
  int progress_i;
  int progress_n;
  lw6sys_progress_t progress1;
  lw6sys_progress_t progress2;
  lw6sys_progress_t progress3;
  lw6sys_progress_t progress4;
  lw6sys_progress_t progress5;

  lw6sys_progress_begin (sys_context, progress);
  lw6sys_progress_split5 (sys_context, &progress1, &progress2, &progress3, &progress4, &progress5, progress);

  memset (map_struct, 0, sizeof (_lw6ker_map_struct_t));

  _draft_zones = _draft_zones_new (sys_context, level, &progress1);
  if (_draft_zones)
    {
      int32_t i;

      lw6sys_progress_begin (sys_context, &progress3);

      ret = 1;

      i = 1;
      progress_n = 0;
      while (i <= level->param.rules.max_zone_size / 2)
	{
	  progress_n++;
	  i *= 2;
	}

      i = 1;
      progress_i = 0;
      while (i <= level->param.rules.max_zone_size / 2 && _draft_zones_group (sys_context, level, _draft_zones, i))
	{
	  lw6sys_progress_update (sys_context, &progress3, 0, progress_n, progress_i++);
	  i *= 2;
	}
      lw6sys_progress_end (sys_context, &progress3);

      ret = ret && _draft_zones_to_map_struct (sys_context, map_struct, _draft_zones, &progress4);

      _draft_zones_free (sys_context, _draft_zones);

      ret = ret && _fix_one_way (sys_context, map_struct, level);
      ret = ret && _init_slots (sys_context, map_struct, level, &progress5);
      ret = ret && _init_places (sys_context, map_struct, level, &progress1);

      _lw6ker_map_struct_sanity_check (sys_context, map_struct);

      if (ret)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_
		      ("map struct created %dx%dx%d, %d zones, %1.1f%% compression"),
		      map_struct->shape.w, map_struct->shape.h,
		      map_struct->shape.d, map_struct->nb_zones, _lw6ker_map_struct_get_compression (sys_context, map_struct) * 100.0f);
	}
    }

  lw6sys_progress_end (sys_context, progress);

  return ret;
}

void
_lw6ker_map_struct_clear (lw6sys_context_t * sys_context, _lw6ker_map_struct_t * map_struct)
{
  if (map_struct)
    {
      if (map_struct->places)
	{
	  LW6SYS_FREE (sys_context, map_struct->places);
	}
      if (map_struct->zones)
	{
	  LW6SYS_FREE (sys_context, map_struct->zones);
	}
      if (map_struct->slots)
	{
	  LW6SYS_FREE (sys_context, map_struct->slots);
	}
      memset (map_struct, 0, sizeof (_lw6ker_map_struct_t));
    }
}

void
_lw6ker_map_struct_find_free_slot_near (lw6sys_context_t * sys_context, const _lw6ker_map_struct_t * map_struct, lw6sys_xyz_t * there, lw6sys_xyz_t here)
{
  int found = 0;

  there->x = there->y = there->z = 0;

  if (lw6sys_shape_check_pos (sys_context, &(map_struct->shape), &here))
    {
      if (_lw6ker_map_struct_get_zone_id (map_struct, here.x, here.y, here.z) >= 0)
	{
	  *there = here;
	  found = 1;
	}
      else
	{
	  int32_t angle, radius;
	  int32_t max_radius, max_angle;
	  lw6sys_xyz_t test;

	  max_radius = map_struct->shape.w + map_struct->shape.h;	// +, not *
	  for (radius = 0; radius < max_radius && !found; ++radius)
	    {
	      max_angle = radius * M_PI * 2;
	      for (angle = 0; angle < max_angle && !found; ++angle)
		{
		  test.x = here.x + (lw6ker_cos ((angle * LW6KER_TRIGO_2PI) / max_angle) * radius) / LW6KER_TRIGO_RADIUS;
		  test.y = here.y - (lw6ker_sin ((angle * LW6KER_TRIGO_2PI) / max_angle) * radius) / LW6KER_TRIGO_RADIUS;
		  test.z = 0;
		  if (lw6sys_shape_check_pos (sys_context, &map_struct->shape, &test))
		    {
		      for (test.z = 0; test.z < map_struct->shape.d; ++test.z)
			{
			  if (_lw6ker_map_struct_get_zone_id (map_struct, test.x, test.y, test.z) >= 0)
			    {
			      found = 1;
			      there->x = test.x;
			      there->y = test.y;
			    }
			}
		    }
		}
	    }
	}
    }

  if (!found)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to find a free zone near %dx%d"), here.x, here.y);
    }
}

int
_lw6ker_map_struct_sanity_check (lw6sys_context_t * sys_context, const _lw6ker_map_struct_t * map_struct)
{
  int ret = 1;
  int x, y, z, w, h, d;
  int nb_places, nb_zones, nb_slots;
  int zone_id;
  int i, j;
  int z1, z2;

  w = map_struct->shape.w;
  h = map_struct->shape.h;
  d = map_struct->shape.d;
  nb_places = map_struct->nb_places;
  nb_zones = map_struct->nb_zones;
  nb_slots = map_struct->nb_slots;

  if (nb_places != w * h)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("inconsistent nb_places=%d shape=%dx%d"), nb_places, w, h);
      ret = 0;
    }

  if (nb_slots != w * h * d)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("inconsistent nb_slots=%d shape=%dx%dx%d"), nb_slots, w, h, d);
      ret = 0;
    }

  for (z = 0; z < d; z++)
    {
      for (y = 0; y < h; y++)
	{
	  for (x = 0; x < w; x++)
	    {
	      zone_id = _lw6ker_map_struct_get_zone_id (map_struct, x, y, z);
	      if (zone_id == -1 || (zone_id >= 0 && zone_id < nb_zones))
		{
		  if (zone_id >= 0)
		    {
		      z1 = map_struct->zones[zone_id].pos.z;
		      for (i = 0; i < LW6KER_NB_DIRS; ++i)
			{
			  j = map_struct->zones[zone_id].link[i];
			  if (j == -1 || (j >= 0 && j < nb_zones))
			    {
			      if (j >= 0)
				{
				  z2 = map_struct->zones[j].pos.z;
				  switch (i)
				    {
				    case LW6KER_DIR_UP:
				      if (!(z2 == z1 - 1 || z2 == d - 1))
					{
					  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad UP link in %d,%d,%d z1=%d z2=%d"), x, y, z, z1, z2);
					  ret = 0;
					}
				      break;
				    case LW6KER_DIR_DOWN:
				      if (!(z2 == z1 + 1 || z2 == 0))
					{
					  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad DOWN link in %d,%d,%d z1=%d z2=%d"), x, y, z, z1, z2);
					  ret = 0;
					}
				      break;
				    default:
				      // no check for other dirs
				      break;
				    }
				}
			    }
			  else
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad link %d->%d in %d,%d,%d"), i, j, x, y, z);
			      ret = 0;
			    }
			}
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad zone id %d in %d,%d,%d"), zone_id, x, y, z);
		  ret = 0;
		}
	    }
	}
    }

  return ret;
}
