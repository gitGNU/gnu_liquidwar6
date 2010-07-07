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

#include "ldr-png.h"

#include "ldr.h"
#include "ldr-internal.h"

static int
read_image (lw6map_texture_t * texture, _lw6ldr_image_rgba_t * image)
{
  int ret = 0;

  texture->w = image->w;
  texture->h = image->h;
  texture->data =
    (lw6sys_color_8_t *) LW6SYS_MALLOC (texture->w *
					texture->h *
					sizeof (lw6sys_color_8_t *));

  if (texture->data)
    {
      int row, col;
      lw6sys_color_8_t value;

      for (row = 0; row < texture->h; ++row)
	{
	  for (col = 0; col < texture->w; ++col)
	    {
	      value.r = image->data[row][col * 4];
	      value.g = image->data[row][col * 4 + 1];
	      value.b = image->data[row][col * 4 + 2];
	      value.a = image->data[row][col * 4 + 3];

	      lw6map_texture_set (texture, col, row, value);
	    }
	}

      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_ERROR,
		  _("unable to allocate memory for texture"));
    }

  _lw6ldr_rgba_clear (image);

  if (!ret)
    {
      lw6map_texture_clear (texture);
    }

  return ret;
}

static int
read_alpha_image (lw6map_texture_t * texture, _lw6ldr_image_rgba_t * image)
{
  int ret = 0;

  if (texture->w == image->w && texture->h == image->h)
    {
      int row, col;
      lw6sys_color_8_t value;
      u_int32_t alpha;

      for (row = 0; row < texture->h; ++row)
	{
	  for (col = 0; col < texture->w; ++col)
	    {
	      value = lw6map_texture_get (texture, col, row);
	      alpha = (((u_int32_t) image->data[row][col * 4]) +
		       ((u_int32_t) image->data[row][col * 4 + 1]) +
		       ((u_int32_t) image->data[row][col * 4 + 2])) / 3;
	      /*
	       * This is where we "invert" the layer and consider
	       * black (0) is opaque (alpha 255).
	       */
	      value.a = 0xFF - alpha;
	      lw6map_texture_set (texture, col, row, value);
	    }
	}

      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_ERROR,
		  _
		  ("texture size (%dx%d) and alpha layer size (%dx%d) do not match"),
		  texture->w, texture->h, image->w, image->h);
    }

  _lw6ldr_rgba_clear (image);

  return ret;
}

static int
read_png (lw6map_texture_t * texture, char *texture_dot_png,
	  lw6sys_progress_t * progress)
{
  int ret = 0;
  _lw6ldr_image_rgba_t image;

  lw6sys_log (LW6SYS_LOG_INFO, _("reading PNG texture \"%s\""),
	      texture_dot_png);

  memset (&image, 0, sizeof (_lw6ldr_image_rgba_t));

  if (_lw6ldr_rgba_read_png (&image, texture_dot_png, progress))
    {
      ret = read_image (texture, &image);
    }

  return ret;
}

static int
read_jpeg (lw6map_texture_t * texture, char *texture_dot_jpeg,
	   lw6sys_progress_t * progress)
{
  int ret = 0;
  _lw6ldr_image_rgba_t image;

  lw6sys_log (LW6SYS_LOG_INFO, _("reading JPEG texture \"%s\""),
	      texture_dot_jpeg);

  memset (&image, 0, sizeof (_lw6ldr_image_rgba_t));

  if (_lw6ldr_rgba_read_jpeg (&image, texture_dot_jpeg, progress))
    {
      ret = read_image (texture, &image);
    }

  return ret;
}

static int
read_alpha_jpeg (lw6map_texture_t * texture, char *texture_alpha_dot_jpeg,
		 lw6sys_progress_t * progress)
{
  int ret = 0;
  _lw6ldr_image_rgba_t image;

  lw6sys_log (LW6SYS_LOG_INFO, _("reading JPEG alpha layer \"%s\""),
	      texture_alpha_dot_jpeg);

  memset (&image, 0, sizeof (_lw6ldr_image_rgba_t));

  if (_lw6ldr_rgba_read_jpeg (&image, texture_alpha_dot_jpeg, progress))
    {
      ret = read_alpha_image (texture, &image);
    }

  return ret;
}

/*
 * Read the texture associated to a map. Pointer to texture must be valid,
 * it's modified in-place. The function will automatically figure out
 * if texture.png exists or if we must use foreground.png/background.png.
 * Depth is read-only.
 */
int
lw6ldr_texture_read (lw6map_texture_t * texture, char *dirname,
		     lw6map_param_t * param, lw6ldr_hints_t * hints,
		     int use_texture, int display_w, int display_h,
		     float *ratio, int *texture_exists,
		     lw6sys_progress_t * progress)
{
  int ret = 0;

  lw6map_texture_clear (texture);

  (*texture_exists) = 0;

  if (use_texture && !ret)
    {
      /*
       * First, we try to read texture.png
       */
      char *texture_dot_png;

      texture_dot_png =
	lw6sys_path_concat (dirname, _LW6LDR_FILE_TEXTURE_PNG);
      if (texture_dot_png)
	{
	  if (lw6sys_file_exists (texture_dot_png))
	    {
	      ret = read_png (texture, texture_dot_png, progress);
	      (*texture_exists) = ret;
	    }
	  LW6SYS_FREE (texture_dot_png);
	}
    }

  if (use_texture && !ret)
    {
      /*
       * Second, we try to read texture.jpeg
       */
      char *texture_dot_jpeg = NULL;

      texture_dot_jpeg =
	lw6sys_path_concat (dirname, _LW6LDR_FILE_TEXTURE_JPEG);
      if (texture_dot_jpeg)
	{
	  if (lw6sys_file_exists (texture_dot_jpeg))
	    {
	      ret = read_jpeg (texture, texture_dot_jpeg, progress);
	      (*texture_exists) = ret;
	    }
	  LW6SYS_FREE (texture_dot_jpeg);
	}

      if (!ret)
	{
	  texture_dot_jpeg =
	    lw6sys_path_concat (dirname, _LW6LDR_FILE_TEXTURE_JPG);
	  if (texture_dot_jpeg)
	    {
	      if (lw6sys_file_exists (texture_dot_jpeg))
		{
		  ret = read_jpeg (texture, texture_dot_jpeg, progress);
		  (*texture_exists) = ret;
		}
	      LW6SYS_FREE (texture_dot_jpeg);
	    }
	}
    }

  if (use_texture && ret)
    {
      /*
       * If texture is successfully loaded, apply separate
       * JPEG alpha channel.
       */
      char *texture_alpha_dot_jpeg = NULL;

      texture_alpha_dot_jpeg =
	lw6sys_path_concat (dirname, _LW6LDR_FILE_TEXTURE_ALPHA_JPEG);
      if (texture_alpha_dot_jpeg)
	{
	  if (lw6sys_file_exists (texture_alpha_dot_jpeg))
	    {
	      ret = read_alpha_jpeg (texture, texture_alpha_dot_jpeg, NULL);
	    }
	  LW6SYS_FREE (texture_alpha_dot_jpeg);
	}
      if (!ret)
	{
	  texture_alpha_dot_jpeg =
	    lw6sys_path_concat (dirname, _LW6LDR_FILE_TEXTURE_ALPHA_JPG);
	  if (texture_alpha_dot_jpeg)
	    {
	      if (lw6sys_file_exists (texture_alpha_dot_jpeg))
		{
		  ret =
		    read_alpha_jpeg (texture, texture_alpha_dot_jpeg, NULL);
		}
	      LW6SYS_FREE (texture_alpha_dot_jpeg);
	    }
	}
    }

  if (ret && texture->w > 0 && texture->h > 0)
    {
      if (param->style.keep_ratio)
	{
	  (*ratio) = ((float) texture->w) / ((float) texture->h);
	}
      else
	{
	  (*ratio) = ((float) display_w) / ((float) display_h);
	}
    }
  else
    {
      *ratio = 1.0f;
    }

  if ((*texture_exists) == 0)
    {
      /*
       * If there's no texture, we just return 1 (ok) for now, the code will
       * apply a texture later, with lw6map_texture_from_body for instance.
       */
      ret = 1;
    }

  return ret;
}
