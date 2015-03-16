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

static int
_read_png (lw6sys_context_t * sys_context, lw6map_layer_t * layer, _lw6ldr_image_bw_t * image, const lw6ldr_resampler_t * resampler,
	   lw6sys_progress_t * progress)
{
  int ret = 0;

  layer->shape.w = resampler->target_w;
  layer->shape.h = resampler->target_h;
  layer->shape.d = 1;		// allways 1
  layer->data = (unsigned char *) LW6SYS_MALLOC (sys_context, layer->shape.w * layer->shape.h * sizeof (unsigned char *));

  if (layer->data)
    {
      int row, col, row2, col2;
      unsigned int value;

      for (row = 0; row < layer->shape.h; ++row)
	{
	  lw6sys_progress_update (sys_context, progress, 0, layer->shape.h, row);
	  for (col = 0; col < layer->shape.w; ++col)
	    {
	      lw6ldr_resampler_target2source (sys_context, resampler, &col2, &row2, col, row);

	      value = image->data[row2][col2 * image->step];
	      /*
	       * For now we do not handle layer the full way. There's
	       * a big TODO here, one could read in another file (say
	       * an XML file) a parameter that would state "maximum layer
	       * is 5" and maybe even the various thresholds for the
	       * different levels. For now using layer 0 or 1 is just like
	       * in Liquid War 5, we have flat levels, will be fine for
	       * a some time anyway.
	       */
	      value = (value > 127) ? 1 : 0;
	      lw6map_layer_set (layer, col, row, value);
	    }
	}

      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to allocate memory for layer"));
    }

  return ret;
}

/**
 * lw6ldr_layer_read_first
 *
 * @sys_context: global system context
 * @layer: layer to update (out param)
 * @filename: name of PNG file
 * @param: parameters of the map
 * @hints: hints of the map
 * @display_w: width of display
 * @display_h: height of display
 * @target_ratio: width/height ratio we want
 * @bench_value: bench for this computer
 * @magic_number: arbitrary constant
 * @expected_depth: depth of map according to files available
 * @progress: progress object to provide feedback (in/out)
 *
 * Reads the first layer, that is map.png. This function has
 * many parameters since it will try and guess the final
 * (real) resolution of the map.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6ldr_layer_read_first (lw6sys_context_t * sys_context, lw6map_layer_t * layer, const char *filename,
			 lw6map_param_t * param, const lw6ldr_hints_t * hints,
			 int display_w, int display_h, float target_ratio, int bench_value, int magic_number, int expected_depth, lw6sys_progress_t * progress)
{
  int ret = 0;
  lw6ldr_resampler_t resampler;
  _lw6ldr_image_bw_t image;
  lw6sys_progress_t progress1;
  lw6sys_progress_t progress2;
  float gray_level;

  lw6sys_progress_split (sys_context, &progress1, &progress2, progress);

  lw6map_layer_clear (sys_context, layer);
  memset (&image, 0, sizeof (_lw6ldr_image_bw_t));
  if (_lw6ldr_bw_read (sys_context, &image, filename, &progress1))
    {
      if (target_ratio <= 0.0f)
	{
	  target_ratio = ((float) image.w) / ((float) image.h);
	}
      gray_level = _lw6ldr_bw_gray_level (sys_context, &image);
      lw6ldr_resampler_init (sys_context, &resampler, param, hints, image.w, image.h,
			     display_w, display_h, target_ratio, bench_value, magic_number, expected_depth, gray_level);
      ret = _read_png (sys_context, layer, &image, &resampler, &progress2);

      _lw6ldr_bw_clear (sys_context, &image);
    }

  if (!ret)
    {
      lw6map_layer_clear (sys_context, layer);
    }

  return ret;
}

/**
 * lw6ldr_layer_read_next
 *
 * @sys_context: global system context
 * @layer: layer to update (out param)
 * @filename: name of PNG file
 * @target_w: width we want
 * @target_h: height we want
 *
 * Reads a layer, knowing the exact size we want. This is typically
 * to load layer2-7.png once map.png has been loaded.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6ldr_layer_read_next (lw6sys_context_t * sys_context, lw6map_layer_t * layer, const char *filename, int target_w, int target_h)
{
  int ret = 0;
  lw6ldr_resampler_t resampler;
  _lw6ldr_image_bw_t image;

  lw6map_layer_clear (sys_context, layer);
  memset (&image, 0, sizeof (_lw6ldr_image_bw_t));
  if (_lw6ldr_bw_read (sys_context, &image, filename, NULL))
    {
      lw6ldr_resampler_force (sys_context, &resampler, image.w, image.h, target_w, target_h);
      ret = _read_png (sys_context, layer, &image, &resampler, NULL);

      _lw6ldr_bw_clear (sys_context, &image);
    }

  if (!ret)
    {
      lw6map_layer_clear (sys_context, layer);
    }

  return ret;
}
