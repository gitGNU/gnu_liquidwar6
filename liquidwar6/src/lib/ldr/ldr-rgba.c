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

#undef HAVE_STDDEF_H		// redefined by jpeglib.h
#undef HAVE_STDLIB_H		// redefined by jpeglib.h

#include "ldr-png.h"
#include <jpeglib.h>

#define LW6_SKIP_WINDOWS_H

#include "ldr.h"
#include "ldr-internal.h"

int
_lw6ldr_rgba_read_png (lw6sys_context_t * sys_context, _lw6ldr_image_rgba_t * image, const char *png_file, lw6sys_progress_t * progress)
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

		  png_init_io (png_ptr, f);

		  png_read_info (png_ptr, info_ptr);

		  png_get_IHDR (png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

		  png_set_expand (png_ptr);
		  png_set_strip_16 (png_ptr);
		  png_set_packswap (png_ptr);

		  if (!(color_type & PNG_COLOR_MASK_COLOR))
		    {
		      png_set_gray_to_rgb (png_ptr);
		    }

		  if (!(color_type & PNG_COLOR_MASK_ALPHA))
		    {
		      png_set_filler (png_ptr, 255, PNG_FILLER_AFTER);
		    }

		  png_read_update_info (png_ptr, info_ptr);

		  png_get_IHDR (png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

		  rowbytes = png_get_rowbytes (png_ptr, info_ptr);

		  if (color_type & PNG_COLOR_MASK_PALETTE)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't load RGBA PNG file \"%s\", it is still paletted after filtering"), png_file);
		      format_ok = 0;
		    }

		  max_width = lw6sys_imax (LW6MAP_MAX_BODY_WIDTH, LW6MAP_MAX_TEXTURE_WIDTH);
		  max_height = lw6sys_imax (LW6MAP_MAX_BODY_HEIGHT, LW6MAP_MAX_TEXTURE_HEIGHT);
		  if (width > max_width || height > max_height)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				  _x_
				  ("can't load RGBA PNG file \"%s\", it is too big (size=%dx%d max=%dx%d)"),
				  png_file, (int) width, (int) height, max_width, max_height);
		      format_ok = 0;
		    }

		  if (rowbytes != width * 4 || bit_depth > 8)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				  _x_
				  ("can't load RGBA PNG file \"%s\", memory footprint is inconsistent, color_type=%d, rowbytes=%d, width=%d, step=%d, bit_depth=%d"),
				  png_file, color_type, (int) rowbytes, (int) width, 4, bit_depth);
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
			  ret = 1;
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to allocate memory for RGBA PNG file"));
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
      _lw6ldr_rgba_clear (sys_context, image);
    }

  lw6sys_progress_end (sys_context, progress);

  return ret;
}

int
_lw6ldr_rgba_read_jpeg (lw6sys_context_t * sys_context, _lw6ldr_image_rgba_t * image, const char *jpeg_file, lw6sys_progress_t * progress)
{
  int ret = 0;
  FILE *f = NULL;
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPARRAY buffer;
  int row_stride;
  int i;
  int j;
  unsigned char **buf;
  int memory_ok;
  int size_ok;
  int max_width;
  int max_height;

  lw6sys_progress_begin (sys_context, progress);

  /*
   * This function probably lacks many error checks.
   */

  f = fopen (jpeg_file, "rb");
  if (f != NULL)
    {
      memset (&cinfo, 0, sizeof (cinfo));
      jpeg_create_decompress (&cinfo);
      cinfo.err = jpeg_std_error (&jerr);
      jpeg_stdio_src (&cinfo, f);
      jpeg_read_header (&cinfo, TRUE);

      memory_ok = 1;
      size_ok = 1;

      max_width = lw6sys_imax (LW6MAP_MAX_BODY_WIDTH, LW6MAP_MAX_TEXTURE_WIDTH);
      max_height = lw6sys_imax (LW6MAP_MAX_BODY_HEIGHT, LW6MAP_MAX_TEXTURE_HEIGHT);
      if (cinfo.output_width > max_width || cinfo.output_height > max_height)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_
		      ("can't load RGBA JPEG file \"%s\", it is too big (size=%dx%d max=%dx%d)"),
		      jpeg_file, (int) cinfo.output_width, (int) cinfo.output_height, max_width, max_height);
	  size_ok = 0;
	}

      if (size_ok)
	{
	  jpeg_start_decompress (&cinfo);

	  buf = (unsigned char **) LW6SYS_MALLOC (sys_context, cinfo.output_height * sizeof (unsigned char *));
	  if (buf)
	    {
	      for (j = 0; j < cinfo.output_height; ++j)
		{
		  buf[j] = (unsigned char *) LW6SYS_MALLOC (sys_context, cinfo.output_width * 4 * sizeof (unsigned char));
		  if (!buf[j])
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
	      switch (cinfo.output_components)
		{
		case 3:	// color
		  row_stride = cinfo.output_width * cinfo.output_components;
		  buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) & cinfo, JPOOL_IMAGE, row_stride, 1);
		  while (cinfo.output_scanline < cinfo.output_height)
		    {
		      lw6sys_progress_update (sys_context, progress, 0, cinfo.output_height, cinfo.output_scanline);
		      j = lw6sys_imax (lw6sys_imin (cinfo.output_scanline, cinfo.output_height - 1), 0);

		      jpeg_read_scanlines (&cinfo, buffer, 1);
		      for (i = 0; i < cinfo.output_width; i++)
			{
			  buf[j][i * 4 + 0] = buffer[0][i * 3 + 0];
			  buf[j][i * 4 + 1] = buffer[0][i * 3 + 1];
			  buf[j][i * 4 + 2] = buffer[0][i * 3 + 2];
			  buf[j][i * 4 + 3] = 255;
			}
		    }
		  image->w = cinfo.output_width;
		  image->h = cinfo.output_height;
		  image->data = buf;
		  ret = 1;
		  break;

		case 1:	// grayscale
		  row_stride = cinfo.output_width * cinfo.output_components;
		  buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) & cinfo, JPOOL_IMAGE, row_stride, 1);
		  while (cinfo.output_scanline < cinfo.output_height)
		    {
		      lw6sys_progress_update (sys_context, progress, 0, cinfo.output_height, cinfo.output_scanline);
		      j = lw6sys_imax (lw6sys_imin (cinfo.output_scanline, cinfo.output_height - 1), 0);

		      jpeg_read_scanlines (&cinfo, buffer, 1);
		      for (i = 0; i < cinfo.output_width; i++)
			{
			  buf[j][i * 4 + 0] = buf[j][i * 4 + 1] = buf[j][i * 4 + 2] = buffer[0][i];
			  buf[j][i * 4 + 3] = 255;
			}
		    }
		  image->w = cinfo.output_width;
		  image->h = cinfo.output_height;
		  image->data = buf;
		  ret = 1;
		  break;

		default:
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			      _x_ ("unable to handle jpeg file output_components must be 3 (RGB) but is %d"), cinfo.output_components);
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to allocate memory for RGBA JPEG file"));
	    }
	  jpeg_finish_decompress (&cinfo);
	}

      jpeg_destroy_decompress (&cinfo);

      fclose (f);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to read jpeg file \"%s\""), jpeg_file);
    }

  if (!ret)
    {
      _lw6ldr_rgba_clear (sys_context, image);
    }

  lw6sys_progress_end (sys_context, progress);

  return ret;
}


/*
 * Clear everything and free data pointers.
 */
extern void
_lw6ldr_rgba_clear (lw6sys_context_t * sys_context, _lw6ldr_image_rgba_t * image)
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

  memset (image, 0, sizeof (_lw6ldr_image_rgba_t));
}
