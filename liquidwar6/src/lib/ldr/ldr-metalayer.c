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
_read_png (lw6sys_context_t * sys_context, lw6map_meta_layer_t * meta_layer, _lw6ldr_image_bw_t * image, lw6ldr_resampler_t * resampler, int analog,
	   lw6sys_progress_t * progress)
{
  int ret = 0;

  meta_layer->shape.w = resampler->target_w;
  meta_layer->shape.h = resampler->target_h;
  meta_layer->shape.d = 1;	// allways 1
  meta_layer->data = (unsigned char *) LW6SYS_MALLOC (sys_context, meta_layer->shape.w * meta_layer->shape.h * sizeof (unsigned char *));

  if (meta_layer->data)
    {
      int row, col, row2, col2;
      unsigned int value;

      for (row = 0; row < meta_layer->shape.h; ++row)
	{
	  lw6sys_progress_update (sys_context, progress, 0, meta_layer->shape.h, row);
	  for (col = 0; col < meta_layer->shape.w; ++col)
	    {
	      lw6ldr_resampler_target2source (sys_context, resampler, &col2, &row2, col, row);
	      /*
	       * For metalayers, we always "invert" the layer meaning,
	       * that is black is "metalayer param set to full" and
	       * white is "metalayer param set to none". This is
	       * the contrary of what is stored internally in the PNG.
	       */
	      value = 0xFF - image->data[row2][col2 * image->step];
	      if (!analog)
		{
		  value = value ? 1 : 0;
		}
	      lw6map_meta_layer_set (sys_context, meta_layer, col, row, value);
	    }
	}

      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to allocate memory for meta_layer"));
    }

  return ret;
}

/**
 * lw6ldr_meta_layer_read
 *
 * @sys_context: global system context
 * @meta_layer: the meta layer to read
 * @filename: the file to open
 * @target_w: the wanted width
 * @target_h: the wanted height
 * @analog: wether to use analog info (0-255) or boolean (0-1)
 *
 * Reads a meta-layer from the disj, resampling is done according
 * to the given parameters.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6ldr_meta_layer_read (lw6sys_context_t * sys_context, lw6map_meta_layer_t * meta_layer, const char *filename, int target_w, int target_h, int analog)
{
  int ret = 0;
  lw6ldr_resampler_t resampler;
  _lw6ldr_image_bw_t image;

  lw6map_meta_layer_clear (sys_context, meta_layer);
  memset (&image, 0, sizeof (_lw6ldr_image_bw_t));
  if (_lw6ldr_bw_read (sys_context, &image, filename, NULL))
    {
      lw6ldr_resampler_force (sys_context, &resampler, image.w, image.h, target_w, target_h);
      ret = _read_png (sys_context, meta_layer, &image, &resampler, analog, NULL);

      _lw6ldr_bw_clear (sys_context, &image);
    }

  if (!ret)
    {
      lw6map_meta_layer_clear (sys_context, meta_layer);
    }

  return ret;
}

/**
 * lw6ldr_meta_layer_read_if_exists
 *
 * @sys_context: global system context
 * @meta_layer: the meta layer to read
 * @dirname: the map directory
 * @file_only: the meta-layer file name only (without the path)
 * @target_w: the wanted width
 * @target_h: the wanted height
 * @analog: wether to use analog info (0-255) or boolean (0-1)
 *
 * Reads a meta-layer from the disj, resampling is done according
 * to the given parameters. This function is different
 * from @lw6ldr_meta_layer_read for it will 1) concatenate @dirname
 * and @file_only and 2) return OK (1) if file does not exist.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6ldr_meta_layer_read_if_exists (lw6sys_context_t * sys_context, lw6map_meta_layer_t * meta_layer, const char *dirname, const char *file_only, int target_w,
				  int target_h, int analog)
{
  int ret = 0;
  char *dot_png = NULL;

  dot_png = lw6sys_path_concat (sys_context, dirname, file_only);
  if (dot_png)
    {
      if (lw6sys_file_exists (sys_context, dot_png))
	{
	  ret = lw6ldr_meta_layer_read (sys_context, meta_layer, dot_png, target_w, target_h, analog);
	}
      else
	{
	  /*
	   * OK, file does not exist, does not matter, meta-layers
	   * are an optional feature.
	   */
	  ret = 1;
	}
      LW6SYS_FREE (sys_context, dot_png);
    }

  return ret;
}
