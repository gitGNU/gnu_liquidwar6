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

#include "map.h"

/**
 * lw6map_meta_layer_set
 *
 * @sys_context: global system context
 * @meta_layer: the meta_layer structure
 * @x: x coord
 * @y: y coord
 * @value: the value to set at this place
 *
 * Simple setter for the meta_layer struct.
 *
 * Return value: none
 */
void
lw6map_meta_layer_set (lw6sys_context_t * sys_context, lw6map_meta_layer_t * meta_layer, int x, int y, u_int8_t value)
{
  if (meta_layer->data)
    {
      meta_layer->data[y * meta_layer->shape.w + x] = value;
    }
}

/**
 * lw6map_meta_layer_get
 *
 * @sys_context: global system context
 * @meta_layer: the meta_layer structure
 * @x: x coord
 * @y: y coord
 *
 * Simple getter for the meta_layer struct.
 *
 * Return value: the value at this place
 */
u_int8_t
lw6map_meta_layer_get (lw6sys_context_t * sys_context, const lw6map_meta_layer_t * meta_layer, int x, int y)
{
  int ret = 0;

  if (meta_layer->data)
    {
      ret = meta_layer->data[y * meta_layer->shape.w + x];
    }

  return ret;
}

/**
 * lw6map_meta_layer_clear
 *
 * @sys_context: global system context
 * @meta_layer: the meta_layer to clear
 *
 * Clears a meta_layer struct. This means freeing the pointer
 * if it's non NULL and setting everything to 0.
 *
 * Return value: none
 */
void
lw6map_meta_layer_clear (lw6sys_context_t * sys_context, lw6map_meta_layer_t * meta_layer)
{
  if (meta_layer->data)
    {
      LW6SYS_FREE (sys_context, meta_layer->data);
    }

  memset (meta_layer, 0, sizeof (lw6map_meta_layer_t));
}

/**
 * lw6map_meta_layer_builtin_custom
 *
 * @sys_context: global system context
 * @meta_layer: the object to init
 * @w: width
 * @h: height
 * @analog: wether to use analog mode (0-255) or boolean (0-1)
 * @noise_percent: the quantity of noise to initialise the layer with
 * @seed: a pseudo-random seed to feed the pseudo-random generator
 *
 * Builds a custom metalyer, suitable for tests or demo, letting the choice
 * of its size and the noise to fill it with. If noise is 100 then metalayer
 * is "full". If noise is 0, then meta layer is empty.
 *
 * Return value: 1 if OK, 0 on failure.
 */
int
lw6map_meta_layer_builtin_custom (lw6sys_context_t * sys_context, lw6map_meta_layer_t * meta_layer, int w, int h, int analog, int noise_percent, int seed)
{
  int ret = 0;
  int i, n;
  u_int32_t checksum;

  lw6map_meta_layer_clear (sys_context, meta_layer);
  if (noise_percent == 0)
    {
      // ok, we leave pointer NULL, code should handler this fine
      ret = 1;
    }
  else
    {
      noise_percent = lw6sys_imax (1, noise_percent);
      noise_percent = lw6sys_imin (100, noise_percent);
      seed = lw6sys_checksum_int32 (sys_context, seed);
      meta_layer->data = (u_int8_t *) LW6SYS_CALLOC (sys_context, w * h * sizeof (u_int8_t));
      if (meta_layer->data)
	{
	  meta_layer->shape.w = w;
	  meta_layer->shape.h = h;
	  meta_layer->shape.d = 1;
	  n = w * h;
	  for (i = 0; i < n; ++i)
	    {
	      checksum = lw6sys_checksum_int32 (sys_context, seed + i);
	      if ((checksum % 100) < noise_percent)
		{
		  if (analog)
		    {
		      meta_layer->data[i] = checksum & 0xFF;
		    }
		  else
		    {
		      meta_layer->data[i] = 1;
		    }
		}
	    }
	  ret = 1;
	}
    }

  return ret;
}
