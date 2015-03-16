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

#include "ldr-png.h"

#include "ldr.h"
#include "ldr-internal.h"

/**
 * lw6ldr_body_read
 *
 * @sys_context: global system context
 * @body: the body to read, must point to allocated memory
 * @dirname: the directory of the map
 * @param: map parameters
 * @hints: map hints
 * @display_w: the display width
 * @display_h: the display height
 * @ratio: wished map ratio
 * @bench_value: the bench value (depends on computer capacity)
 * @magic_number: arbitrary constant
 * @progress: structure to transmit loading progress
 *
 * Reads the map body, that is, all the layers.
 *
 * Return value: 1 if OK, 0 if failed.
 */
int
lw6ldr_body_read (lw6sys_context_t * sys_context, lw6map_body_t * body, const char *dirname,
		  lw6map_param_t * param, const lw6ldr_hints_t * hints,
		  int display_w, int display_h, float ratio, int bench_value, int magic_number, lw6sys_progress_t * progress)
{
  int ret = 0;
  char *dot_png;
  char *extra_layers_png[LW6MAP_MAX_BODY_DEPTH] = { _LW6LDR_FILE_LAYER2_PNG,
    _LW6LDR_FILE_LAYER3_PNG,
    _LW6LDR_FILE_LAYER4_PNG,
    _LW6LDR_FILE_LAYER5_PNG,
    _LW6LDR_FILE_LAYER6_PNG,
    _LW6LDR_FILE_LAYER7_PNG,
    NULL
  };
  int layer;
  lw6sys_progress_t progress1;
  lw6sys_progress_t progress2;
  int expected_depth = 1;

  lw6sys_progress_split (sys_context, &progress1, &progress2, progress);

  lw6map_body_clear (sys_context, body);

  for (layer = 1; layer < LW6MAP_MAX_BODY_DEPTH; ++layer)
    {
      dot_png = lw6sys_path_concat (sys_context, dirname, extra_layers_png[layer - 1]);
      if (dot_png)
	{
	  if (lw6sys_file_exists (sys_context, dot_png))
	    {
	      expected_depth++;
	    }
	  LW6SYS_FREE (sys_context, dot_png);
	}
    }

  dot_png = lw6sys_path_concat (sys_context, dirname, _LW6LDR_FILE_MAP_PNG);
  if (dot_png)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading \"%s\""), dot_png);

      ret =
	lw6ldr_layer_read_first (sys_context, &(body->layers[0]), dot_png, param, hints,
				 display_w, display_h, ratio, bench_value, magic_number, expected_depth, &progress1);
      LW6SYS_FREE (sys_context, dot_png);
      if (ret)
	{
	  body->shape = body->layers[0].shape;
	  body->shape.d = 1;
	  for (layer = 1; layer < LW6MAP_MAX_BODY_DEPTH; ++layer)
	    {
	      dot_png = lw6sys_path_concat (sys_context, dirname, extra_layers_png[layer - 1]);
	      if (dot_png)
		{
		  if (lw6sys_file_exists (sys_context, dot_png))
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading extra layer %d \"%s\""), layer, dot_png);
		      ret = ret && lw6ldr_layer_read_next (sys_context, &(body->layers[layer]), dot_png, body->shape.w, body->shape.h);
		      if (ret)
			{
			  body->shape.d++;
			}
		    }
		  LW6SYS_FREE (sys_context, dot_png);
		}
	    }

	  ret = ret && lw6ldr_grease_apply (sys_context, &(body->layers[0]), &param->rules, hints, &progress2);
	  for (layer = 1; layer < body->shape.d; ++layer)
	    {
	      ret = ret && lw6ldr_grease_apply (sys_context, &(body->layers[layer]), &param->rules, hints, NULL);
	    }
	  ret = ret && lw6map_body_check_and_fix_holes (sys_context, body, &param->rules);

	  ret = ret && lw6ldr_meta_layer_read_if_exists (sys_context, &(body->glue), dirname, _LW6LDR_FILE_GLUE_PNG, body->shape.w, body->shape.h, 1);
	  ret = ret && lw6ldr_meta_layer_read_if_exists (sys_context, &(body->boost), dirname, _LW6LDR_FILE_BOOST_PNG, body->shape.w, body->shape.h, 1);
	  ret = ret && lw6ldr_meta_layer_read_if_exists (sys_context, &(body->danger), dirname, _LW6LDR_FILE_DANGER_PNG, body->shape.w, body->shape.h, 1);
	  ret = ret && lw6ldr_meta_layer_read_if_exists (sys_context, &(body->medicine), dirname, _LW6LDR_FILE_MEDICINE_PNG, body->shape.w, body->shape.h, 1);
	  ret = ret
	    && lw6ldr_meta_layer_read_if_exists (sys_context, &(body->one_way_north), dirname, _LW6LDR_FILE_ONE_WAY_NORTH_PNG, body->shape.w, body->shape.h,
						 0);
	  ret = ret
	    && lw6ldr_meta_layer_read_if_exists (sys_context, &(body->one_way_east), dirname, _LW6LDR_FILE_ONE_WAY_EAST_PNG, body->shape.w, body->shape.h, 0);
	  ret = ret
	    && lw6ldr_meta_layer_read_if_exists (sys_context, &(body->one_way_south), dirname, _LW6LDR_FILE_ONE_WAY_SOUTH_PNG, body->shape.w, body->shape.h,
						 0);
	  ret = ret
	    && lw6ldr_meta_layer_read_if_exists (sys_context, &(body->one_way_west), dirname, _LW6LDR_FILE_ONE_WAY_WEST_PNG, body->shape.w, body->shape.h, 0);

	  if (ret)
	    {
	      lw6map_body_fix_checksum (sys_context, body);
	    }
	}
    }

  return ret;
}
