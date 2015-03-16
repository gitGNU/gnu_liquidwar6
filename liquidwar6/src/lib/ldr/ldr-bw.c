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

int
_lw6ldr_bw_read (lw6sys_context_t * sys_context, _lw6ldr_image_bw_t * image, const char *png_file, lw6sys_progress_t * progress)
{
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  png_infop end_info = NULL;
  /*
   * no need to use end_info for we use the high level interface
   * with png_read_png.
   */
  int ret = 0;
  FILE *f = NULL;

  lw6sys_progress_begin (sys_context, progress);

  png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr)
    {
      info_ptr = png_create_info_struct (png_ptr);
      if (info_ptr)
	{
	  end_info = png_create_info_struct (png_ptr);
	  if (end_info)
	    {
	      f = fopen (png_file, "rb");
	      if (f)
		{
		  png_uint_32 width;
		  png_uint_32 height;
		  png_uint_32 rowbytes;
		  int max_width;
		  int max_height;
		  int bit_depth;
		  int color_type;
		  unsigned char **buf = NULL;
		  int row;
		  int memory_ok = 1;
		  int format_ok = 1;
		  int step;

		  png_init_io (png_ptr, f);

		  png_read_info (png_ptr, info_ptr);

		  png_get_IHDR (png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

		  png_set_expand (png_ptr);
		  png_set_strip_16 (png_ptr);
		  png_set_packswap (png_ptr);

		  if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA)
		    {
		      png_set_rgb_to_gray (png_ptr, 1, -1, -1);
		    }

		  if (color_type & PNG_COLOR_MASK_ALPHA)
		    {
		      png_set_strip_alpha (png_ptr);
		    }

		  png_read_update_info (png_ptr, info_ptr);

		  png_get_IHDR (png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

		  rowbytes = png_get_rowbytes (png_ptr, info_ptr);

		  step = ((color_type & PNG_COLOR_MASK_COLOR) ? 3 : 1) + ((color_type & PNG_COLOR_MASK_ALPHA) ? 1 : 0);

		  if (color_type & PNG_COLOR_MASK_PALETTE)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't load B&W PNG file \"%s\", it is still paletted after filtering"), png_file);
		      format_ok = 0;
		    }

		  max_width = lw6sys_imax (LW6MAP_MAX_BODY_WIDTH, LW6MAP_MAX_TEXTURE_WIDTH);
		  max_height = lw6sys_imax (LW6MAP_MAX_BODY_HEIGHT, LW6MAP_MAX_TEXTURE_HEIGHT);
		  if (width > max_width || height > max_height)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				  _x_
				  ("can't load B&W PNG file \"%s\", it is too big (size=%dx%d max=%dx%d)"),
				  png_file, (int) width, (int) height, max_width, max_height);
		      format_ok = 0;
		    }

		  if (rowbytes != width * step || bit_depth > 8)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				  _x_
				  ("can't load B&W PNG file \"%s\", memory footprint is inconsistent, color_type=%d, rowbytes=%d, width=%d, step=%d, bit_depth=%d"),
				  png_file, color_type, (int) rowbytes, (int) width, step, bit_depth);
		      format_ok = 0;
		    }

		  if (format_ok)
		    {
		      buf = (unsigned char **) LW6SYS_MALLOC (sys_context, height * sizeof (unsigned char *));
		      if (buf)
			{
			  for (row = 0; row < height; ++row)
			    {
			      buf[row] = (unsigned char *) LW6SYS_MALLOC (sys_context, rowbytes * sizeof (unsigned char));
			      if (!buf[row])
				{
				  memory_ok = 0;
				}
			    }
			}
		      else
			{
			  memory_ok = 0;
			}

		      if (memory_ok)
			{
			  lw6sys_progress_update (sys_context, progress, 0, 3, 1);
			  png_read_image (png_ptr, buf);
			  lw6sys_progress_update (sys_context, progress, 0, 3, 2);
			  png_read_end (png_ptr, end_info);

			  image->w = width;
			  image->h = height;
			  image->data = buf;
			  image->step = step;
			  ret = 1;
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to allocate memory for B&W PNG file"));
			}
		    }

		  fclose (f);
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't create png end info struct"));
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't create png info struct"));
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't create png read struct"));
    }

  png_destroy_read_struct (&png_ptr, &info_ptr, &end_info);

  if (!ret)
    {
      _lw6ldr_bw_clear (sys_context, image);
    }

  lw6sys_progress_end (sys_context, progress);

  return ret;
}

/*
 * Clear everything and free data pointers.
 */
extern void
_lw6ldr_bw_clear (lw6sys_context_t * sys_context, _lw6ldr_image_bw_t * image)
{
  int row;

  if (image->data)
    {
      for (row = 0; row < image->h; ++row)
	{
	  if (image->data[row])
	    {
	      LW6SYS_FREE (sys_context, image->data[row]);
	    }
	}
      LW6SYS_FREE (sys_context, image->data);
    }

  memset (image, 0, sizeof (_lw6ldr_image_bw_t));
}

float
_lw6ldr_bw_gray_level (lw6sys_context_t * sys_context, _lw6ldr_image_bw_t * image)
{
  int x, y, w, h;
  float ret = 0.0f;
  int gray_count = 0;
  u_int8_t *row;
  u_int32_t c;

  w = image->w;
  h = image->h;
  if (w > 0 && h > 0)
    {
      for (y = 0; y < h; ++y)
	{
	  row = image->data[y];
	  for (x = 0; x < w; ++x)
	    {
	      c = row[x];
	      gray_count += c >> 6;
	    }
	}
      gray_count = gray_count >> 2;

      ret = ((float) gray_count) / (((float) w) * ((float) h));
    }
  if (ret > 1.0f)
    {
      ret = 1.0f;
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("gray_level=%f, can't be >1"), ret);
    }
  if (ret < 0.0f)
    {
      ret = 0.0f;
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("gray_level=%f, can't be <0"), ret);
    }
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("gray_level=%f"), ret);

  return ret;
}
