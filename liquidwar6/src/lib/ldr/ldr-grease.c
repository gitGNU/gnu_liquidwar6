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

#define _NB_GREASE_CONFIGURATIONS 16

typedef int _grease_table_t[_NB_GREASE_CONFIGURATIONS];

/*
 * Just to check the below table, here are the
 * binary representations of numbers from 0 to 15
 * 0 : 0000
 * 1 : 0001
 * 2 : 0010
 * 3 : 0011
 * 4 : 0100
 * 5 : 0101
 * 6 : 0110
 * 7 : 0111
 * 8 : 1000
 * 9 : 1001
 * a : 1010
 * b : 1011
 * c : 1100
 * d : 1101
 * e : 1110
 * f : 1111
 * Basically:
 * -> grease 1 : only completely enclosed squares are concerned (0xf)
 * -> grease 2 : we add all the "3 squares arround me" cases (7,b,d,e)
 * -> grease 3 : add all the "2 adjacent squares arround me" cases (3,6,9,c)
 * -> grease 4 : add the opposite squares configuration (5,a)
 * -> grease 5 : add "only 1 squares arround me" cases (1,2,4,8)
 */
static _grease_table_t _GREASE_TABLES[LW6LDR_HINTS_MAX_WALL_GREASE] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1},
{0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1},
{0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

static int
_do_grease (lw6sys_context_t * sys_context, lw6map_layer_t * layer, const lw6map_rules_t * rules, _grease_table_t grease_table, int mode,
	    lw6sys_progress_t * progress)
{
  int ret = 0;
  int x, y, tx, ty;
  int size;
  lw6sys_whd_t shape;
  lw6map_layer_t tmp;
  int table_entry;

  shape = layer->shape;
  tmp.shape = layer->shape;

  size = shape.w * shape.h * sizeof (u_int8_t);
  tmp.data = LW6SYS_MALLOC (sys_context, size);
  if (tmp.data)
    {
      memcpy (tmp.data, layer->data, size);
      for (y = 0; y < shape.h; ++y)
	{
	  for (x = 0; x < shape.w; ++x)
	    {
	      table_entry = 0;

	      ty = y - 1;
	      tx = x;
	      lw6map_coords_fix_xy (rules, &shape, &tx, &ty);
	      if (lw6map_layer_get (&tmp, tx, ty))
		{
		  if (mode)
		    {
		      table_entry |= 0x1;
		    }
		}
	      else
		{
		  if (!mode)
		    {
		      table_entry |= 0x1;
		    }
		}
	      ty = y;
	      tx = x + 1;
	      lw6map_coords_fix_xy (rules, &shape, &tx, &ty);
	      if (lw6map_layer_get (&tmp, tx, ty))
		{
		  if (mode)
		    {
		      table_entry |= 0x2;
		    }
		}
	      else
		{
		  if (!mode)
		    {
		      table_entry |= 0x2;
		    }
		}
	      ty = y + 1;
	      tx = x;
	      lw6map_coords_fix_xy (rules, &shape, &tx, &ty);
	      if (lw6map_layer_get (&tmp, tx, ty))
		{
		  if (mode)
		    {
		      table_entry |= 0x4;
		    }
		}
	      else
		{
		  if (!mode)
		    {
		      table_entry |= 0x4;
		    }
		}
	      ty = y;
	      tx = x - 1;
	      lw6map_coords_fix_xy (rules, &shape, &tx, &ty);
	      if (lw6map_layer_get (&tmp, tx, ty))
		{
		  if (mode)
		    {
		      table_entry |= 0x8;
		    }
		}
	      else
		{
		  if (!mode)
		    {
		      table_entry |= 0x8;
		    }
		}

	      if (grease_table[table_entry])
		{
		  lw6map_layer_set (layer, tx, ty, mode);
		}
	    }
	}
      ret = 1;
      LW6SYS_FREE (sys_context, tmp.data);
    }

  return ret;
}

/**
 * lw6ldr_grease_apply
 *
 * @sys_context: global system context
 * @layer: the layer on which to apply the grease
 * @rules: map rules
 * @hints: map hints
 * @progress: structure to transmit loading progress
 *
 * Reads the map body, that is, all the layers.
 *
 * Return value: 1 if OK, 0 if failed.
 */
int
lw6ldr_grease_apply (lw6sys_context_t * sys_context, lw6map_layer_t * layer, const lw6map_rules_t * rules, const lw6ldr_hints_t * hints,
		     lw6sys_progress_t * progress)
{
  int ret = 0;
  int i = 0;

  if (hints->wall_grease == 0)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("grease==0, nothing to do"));
    }
  else
    {
      if (hints->wall_grease > 0)
	{
	  i = lw6sys_imin (hints->wall_grease - 1, LW6LDR_HINTS_MAX_WALL_GREASE - 1);
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("positive grease %d using table %d"), hints->wall_grease, i);
	  _do_grease (sys_context, layer, rules, _GREASE_TABLES[i], 0, progress);
	}
      else
	{
	  i = lw6sys_imin (-hints->wall_grease - 1, LW6LDR_HINTS_MAX_WALL_GREASE - 1);
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("negative grease %d using table %d"), hints->wall_grease, i);
	  _do_grease (sys_context, layer, rules, _GREASE_TABLES[i], 1, progress);
	}
    }

  ret = 1;

  return ret;
}
