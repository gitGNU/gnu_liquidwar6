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

#include "gl1-utils.h"

#define _TEST_PNG1 ".png"
#define _TEST_PNG2 ".PNG"

static volatile u_int32_t seq_id = 0;

static void
_set_defaults (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap, const char *desc)
{
  // some values not set, already at zero because of CALLOC
  bitmap->wrap = GL_REPEAT;
  bitmap->filter = GL_LINEAR;
  bitmap->s1 = 0.0f;
  bitmap->t1 = 0.0f;
  bitmap->s2 = mod_gl1_utils_texture_scale (sys_context, bitmap->surface->w);
  bitmap->t2 = mod_gl1_utils_texture_scale (sys_context, bitmap->surface->h);
  bitmap->texture_w = lw6gui_power_of_two_ge (sys_context, bitmap->surface->w);
  bitmap->texture_h = lw6gui_power_of_two_ge (sys_context, bitmap->surface->h);
  bitmap->last_refresh = _lw6gfx_sdl_timer_get_timestamp (sys_context, &(utils_context->sdl_context));
  bitmap->desc = lw6sys_str_copy (sys_context, desc);
  mod_gl1_utils_bitmap_hash_register (sys_context, utils_context, bitmap);
  /*  if (utils_context->initialized)
     {
     mod_gl1_utils_bitmap_refresh (sys_context,utils_context, bitmap);
     } */
}

static int
_has_alpha (lw6sys_context_t * sys_context, const char *filename)
{
  int ret = 0;

  if (strstr (filename, _TEST_PNG1) || strstr (filename, _TEST_PNG2))
    {
      ret = 1;
    }

  return ret;
}

mod_gl1_utils_bitmap_t *
mod_gl1_utils_bitmap_new (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, int width, int height, const char *desc)
{
  mod_gl1_utils_bitmap_t *ret = NULL;

  ret = (mod_gl1_utils_bitmap_t *) LW6SYS_CALLOC (sys_context, sizeof (mod_gl1_utils_bitmap_t));
  if (ret)
    {
      ret->id = 0;
      while (!ret->id)
	{
	  ret->id = ++seq_id;
	}

      ret->surface = mod_gl1_utils_create_surface (sys_context, utils_context, width, height);
      if (ret->surface)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("new bitmap %d %dx%d"), ret->id, width, height);
	  ret->has_alpha = 1;
	  _set_defaults (sys_context, utils_context, ret, desc);
	}
      else
	{
	  LW6SYS_FREE (sys_context, ret);
	  ret = NULL;
	}
    }

  return ret;
}

mod_gl1_utils_bitmap_t *
mod_gl1_utils_bitmap_load (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const char *filename)
{
  mod_gl1_utils_bitmap_t *ret = NULL;
  char *desc = NULL;

  ret = (mod_gl1_utils_bitmap_t *) LW6SYS_CALLOC (sys_context, sizeof (mod_gl1_utils_bitmap_t));
  if (ret)
    {
      ret->id = 0;
      while (!ret->id)
	{
	  ret->id = ++seq_id;
	}

      desc = lw6sys_path_file_only (sys_context, filename);
      if (desc)
	{
	  ret->surface = mod_gl1_utils_load_image (sys_context, utils_context, filename);
	  if (ret->surface)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("bitmap %u \"%s\" %dx%d loaded"), ret->id, filename, ret->surface->w, ret->surface->h);
	      if (ret->surface->format->BytesPerPixel == 4)
		{
		  ret->has_alpha = _has_alpha (sys_context, filename);
		}
	      _set_defaults (sys_context, utils_context, ret, desc);
	    }
	  else
	    {
	      LW6SYS_FREE (sys_context, ret);
	      ret = NULL;
	    }
	  LW6SYS_FREE (sys_context, desc);
	}
      else
	{
	  LW6SYS_FREE (sys_context, ret);
	  ret = NULL;
	}
    }

  return ret;
}

mod_gl1_utils_bitmap_t *
mod_gl1_utils_surface2bitmap (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, SDL_Surface * surface, const char *desc)
{
  mod_gl1_utils_bitmap_t *ret = NULL;

  ret = (mod_gl1_utils_bitmap_t *) LW6SYS_CALLOC (sys_context, sizeof (mod_gl1_utils_bitmap_t));
  if (ret)
    {
      ret->id = 0;
      while (!ret->id)
	{
	  ret->id = ++seq_id;
	}

      ret->surface = surface;
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("bitmap %u from surface %dx%d"), ret->id, ret->surface->w, ret->surface->h);
      ret->has_alpha = 1;
      _set_defaults (sys_context, utils_context, ret, desc);
    }

  return ret;
}

void
mod_gl1_utils_bitmap_free (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap)
{
  if (bitmap)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("free bitmap %u"), bitmap->id);
      mod_gl1_utils_bitmap_hash_unregister (sys_context, utils_context, bitmap);

      if (bitmap->desc)
	{
	  LW6SYS_FREE (sys_context, bitmap->desc);
	}
      if (bitmap->surface)
	{
	  mod_gl1_utils_delete_surface (sys_context, utils_context, bitmap->surface);
	}
      if (bitmap->colorized_surface)
	{
	  mod_gl1_utils_delete_surface (sys_context, utils_context, bitmap->colorized_surface);
	}
      if (bitmap->texture)
	{
	  mod_gl1_utils_schedule_delete_texture (sys_context, utils_context, bitmap->texture);
	}
      LW6SYS_FREE (sys_context, bitmap);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL bitmap"));
    }
}

int
mod_gl1_utils_bitmap_colorize (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap, int colorize,
			       const lw6map_color_couple_t * color)
{
  int ret = 0;

  bitmap->colorize = colorize;
  if (colorize)
    {
      if ((!bitmap->colorized_surface) || (!lw6map_color_is_same (sys_context, color, &(bitmap->colorize_color))))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("colorize bitmap %u"), bitmap->id);
	  if (bitmap->colorized_surface)
	    {
	      mod_gl1_utils_delete_surface (sys_context, utils_context, bitmap->colorized_surface);
	      bitmap->colorized_surface = NULL;
	    }
	  bitmap->colorize_color = *color;
	  bitmap->colorized_surface = mod_gl1_utils_colorize_surface (sys_context, utils_context, bitmap->surface, color, bitmap->has_alpha);
	  mod_gl1_utils_bitmap_clear_texture (sys_context, utils_context, bitmap);
	}
    }
  else
    {
      if (bitmap->colorized_surface)
	{
	  mod_gl1_utils_delete_surface (sys_context, utils_context, bitmap->colorized_surface);
	  bitmap->colorized_surface = NULL;
	}
    }

  return ret;
}

int
mod_gl1_utils_bitmap_set_wrap (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap, GLint wrap)
{
  int ret = 0;

  switch (wrap)
    {
    case GL_REPEAT:
    case GL_CLAMP:
    case GL_CLAMP_TO_EDGE:
      bitmap->wrap = wrap;
      ret = 1;
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unknow GL wrap %d"), wrap);
    }

  return ret;
}

int
mod_gl1_utils_bitmap_set_filter (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap, GLint filter)
{
  int ret = 0;

  /*
   * This is where mipmaping and/or linear filtering is enabled
   * or disabled based on gfx_quality value.
   */
  switch (utils_context->render_param.gfx_quality)
    {
    case LW6GUI_GFX_QUALITY_LOW:
      filter = GL_NEAREST;
      break;
    case LW6GUI_GFX_QUALITY_STANDARD:
      switch (filter)
	{
	case GL_LINEAR_MIPMAP_NEAREST:
	case GL_LINEAR_MIPMAP_LINEAR:
	  filter = GL_LINEAR;
	  break;
	}
      break;
    case LW6GUI_GFX_QUALITY_HIGH:
      // keep given filter
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unknown gfx_quality %d"), utils_context->render_param.gfx_quality);
      break;
    }

  switch (filter)
    {
    case GL_NEAREST:
    case GL_LINEAR:
      bitmap->mipmap = 0;
      if (filter != bitmap->filter)
	{
	  /*
	   * OK, this might sound overkill to delete the texture
	   * just because we change the filtering. In practice
	   * this is rare, a given texture is always rendered
	   * with the same options. In fact, should there be a
	   * bitmap that would be displayed in the same frame with
	   * 2 different filters, we would create 2 bitmaps.
	   * Here the idea is that when one switches from GL_LINEAR
	   * to say GL_LINEAR_MIMAP_LINEAR one has to activate
	   * mimapping, and that is done at texture creation.
	   */
	  mod_gl1_utils_bitmap_clear_texture (sys_context, utils_context, bitmap);
	  bitmap->filter = filter;
	}
      ret = 1;
      break;
    case GL_LINEAR_MIPMAP_NEAREST:
    case GL_LINEAR_MIPMAP_LINEAR:
      bitmap->mipmap = 1;
      if (filter != bitmap->filter)
	{
	  mod_gl1_utils_bitmap_clear_texture (sys_context, utils_context, bitmap);
	  bitmap->filter = filter;
	}
      ret = 1;
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unknow GL filter %d"), filter);
    }

  return ret;
}

int
mod_gl1_utils_bitmap_set_texture_coords (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap, float s1,
					 float t1, float s2, float t2)
{
  int ret = 0;

  if (s1 != t2)
    {
      bitmap->s1 = s1;
      bitmap->s2 = s2;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("same texture coords s1=%f s2=%f"), s1, s2);
    }

  if (t1 != t2)
    {
      bitmap->t1 = t1;
      bitmap->t2 = t2;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("same texture coords t1=%f t2=%f"), t1, t2);
    }

  return ret;
}

void
mod_gl1_utils_bitmap_clear_texture (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap)
{
  if (bitmap->texture)
    {
      mod_gl1_utils_schedule_delete_texture (sys_context, utils_context, bitmap->texture);
      bitmap->texture = 0;
    }
}

void
mod_gl1_utils_bitmap_clear_texture_now (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap)
{
  if (bitmap->texture)
    {
      mod_gl1_utils_delete_texture (sys_context, utils_context, bitmap->texture);
      bitmap->texture = 0;
    }
}

int
mod_gl1_utils_bitmap_refresh (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap)
{
  int ret = 0;
  int64_t bitmap_refresh;
  int64_t timestamp;

  bitmap_refresh = mod_gl1_utils_timer_get_bitmap_refresh (sys_context, utils_context);
  timestamp = _lw6gfx_sdl_timer_get_timestamp (sys_context, &(utils_context->sdl_context));

  if (bitmap->last_refresh < bitmap_refresh)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("bitmap %u is too old bitmap->last_refresh=%"
		       LW6SYS_PRINTF_LL
		       "d timer_get_bitmap_refresh returned %"
		       LW6SYS_PRINTF_LL "d, refreshing"), bitmap->id, (long long) bitmap->last_refresh, (long long) bitmap_refresh);
      mod_gl1_utils_bitmap_clear_texture_now (sys_context, utils_context, bitmap);
      bitmap->last_refresh = timestamp;
      bitmap->need_another_refresh = 1;
    }
  else
    {
      if (bitmap->last_refresh == timestamp)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		      _x_ ("not touching bitmap %u, refreshing it now wouldn't change anything as it has already been refreshed for this frame"), bitmap->id);
	}
      else
	{
	  if (bitmap->need_another_refresh)
	    {
	      bitmap->need_another_refresh = 0;
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			  _x_ ("refreshing bitmap %u, second time, this could hopefully fix bug #28029 (white rectangles)"), bitmap->id);
	    }
	}
    }
  if (!bitmap->texture)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("refresh bitmap %u"), bitmap->id);

      /*
       * Calling set_filter with self value will cause gfx_quality
       * rules to be applied if needed.
       */
      mod_gl1_utils_bitmap_set_filter (sys_context, utils_context, bitmap, bitmap->filter);

      if (bitmap->colorize && bitmap->colorized_surface)
	{
	  bitmap->texture = mod_gl1_utils_surface2texture (sys_context, utils_context, bitmap->colorized_surface, bitmap->mipmap);
	}
      else
	{
	  if (bitmap->surface)
	    {
	      bitmap->texture = mod_gl1_utils_surface2texture (sys_context, utils_context, bitmap->surface, bitmap->mipmap);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("no surface for bitmap \"%u\""), bitmap->id);
	    }
	}
      if (bitmap->texture)
	{
	  ret = 1;
	}
    }
  else
    {
      ret = 1;
    }

  return ret;
}

int
mod_gl1_utils_bitmap_refresh_force (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap)
{
  int ret = 0;

  mod_gl1_utils_bitmap_clear_texture_now (sys_context, utils_context, bitmap);
  ret = mod_gl1_utils_bitmap_refresh (sys_context, utils_context, bitmap);

  return ret;
}

void
mod_gl1_utils_bitmap_bind_no_gen (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap)
{
  glBindTexture (GL_TEXTURE_2D, bitmap->texture);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, bitmap->wrap);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, bitmap->wrap);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, bitmap->filter);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, bitmap->filter);
}

int
mod_gl1_utils_bitmap_bind (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap)
{
  int ret = 0;

  if (bitmap)
    {
      mod_gl1_utils_bitmap_refresh (sys_context, utils_context, bitmap);
      glBindTexture (GL_TEXTURE_2D, bitmap->texture);
      if (bitmap->texture == 0 || !glIsTexture (bitmap->texture))
	{
	  mod_gl1_utils_bitmap_refresh_force (sys_context, utils_context, bitmap);
	  glBindTexture (GL_TEXTURE_2D, bitmap->texture);
	}
      if (bitmap->texture > 0 && glIsTexture (bitmap->texture))
	{
	  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, bitmap->wrap);
	  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, bitmap->wrap);
	  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, bitmap->filter);
	  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, bitmap->filter);
	  ret = 1;
	}
      else
	{
	  if (bitmap->texture != 0)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to bind texture %d"), bitmap->texture);
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to bind NULL bitmap"));
    }

  return ret;
}

int
mod_gl1_utils_bitmap_display (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap, float x1, float y1,
			      float x2, float y2)
{
  int ret = 0;

  ret = mod_gl1_utils_bitmap_bind (sys_context, utils_context, bitmap);

  glMatrixMode (GL_TEXTURE);
  glPushMatrix ();
  glLoadIdentity ();
  glBegin (GL_QUADS);
  glTexCoord2d (bitmap->s1, bitmap->t1);
  glVertex3f (x1, y1, 0.0f);	// top left
  glTexCoord2d (bitmap->s2, bitmap->t1);
  glVertex3f (x2, y1, 0.0f);	// top right
  glTexCoord2d (bitmap->s2, bitmap->t2);
  glVertex3f (x2, y2, 0.0f);	// bottom right
  glTexCoord2d (bitmap->s1, bitmap->t2);
  glVertex3f (x1, y2, 0.0f);	// bottom left
  glEnd ();
  glMatrixMode (GL_TEXTURE);
  glPopMatrix ();

  return ret;
}

int
mod_gl1_utils_bitmap_update_texture (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap)
{
  int ret = 0;

  // todo...

  return ret;
}
