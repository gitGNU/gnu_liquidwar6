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

//#include <GL/glu.h>

#include "../../gfx.h"
#include "gl1-utils.h"

static void
_get_text_wh (lw6sys_context_t * sys_context, TTF_Font * font, const char *text, int *width, int *height)
{
  if (TTF_SizeUTF8 (font, text, width, height) == -1)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to calculate font width and height for text \"%s\""), text);
      width = 0;
    }
}

static int
_get_text_width (lw6sys_context_t * sys_context, TTF_Font * font, const char *text)
{
  int width = 0;

  _get_text_wh (sys_context, font, text, &width, NULL);

  return width;
}

static int
_get_text_height (lw6sys_context_t * sys_context, TTF_Font * font, const char *text)
{
  int height = 0;

  _get_text_wh (sys_context, font, text, NULL, &height);

  return height;
}

/*
 * Returns the size of the string if it was drawn
 */
int
mod_gl1_utils_get_system_text_width (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const char *text)
{
  return _get_text_width (sys_context, utils_context->font_data.system, text);
}

/*
 * Returns the size of the string if it was drawn
 */
int
mod_gl1_utils_get_system_text_height (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const char *text)
{
  return _get_text_height (sys_context, utils_context->font_data.system, text);
}

SDL_Surface *
mod_gl1_utils_blended_text_surface (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, TTF_Font * font, SDL_Color color,
				    const char *text)
{
  SDL_Surface *ret = NULL;
  char *utf8 = NULL;

  utf8 = lw6sys_locale_to_utf8 (sys_context, text);
  if (utf8 != NULL)
    {
      ret = TTF_RenderUTF8_Blended (font, utf8, color);
      if (ret)
	{
	  utils_context->surface_counter.new_counter++;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("TTF_RenderUTF8_Blended failed"));
	}
      LW6SYS_FREE (sys_context, utf8);
    }

  return ret;
}

static void
_multiline_text_size_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{

  mod_gl1_utils_multiline_text_callback_data_t *text_callback_data = (mod_gl1_utils_multiline_text_callback_data_t *) func_data;
  char *line = (char *) data;
  int w = 0, h = 0;

  if (!lw6sys_str_is_blank (sys_context, line))
    {
      _get_text_wh (sys_context, text_callback_data->font, line, &w, &h);
      text_callback_data->shape.w = lw6sys_imax (text_callback_data->shape.w, w);
      text_callback_data->shape.h += h;
    }
}

static void
_multiline_text_draw_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{

  mod_gl1_utils_multiline_text_callback_data_t *text_callback_data = (mod_gl1_utils_multiline_text_callback_data_t *) func_data;
  char *line = (char *) data;
  SDL_Surface *buffer = NULL;
  char *utf8 = NULL;
  SDL_Rect src_rect;
  SDL_Rect dst_rect;

  if (!lw6sys_str_is_blank (sys_context, line))
    {
      utf8 = lw6sys_locale_to_utf8 (sys_context, line);
      if (utf8 != NULL)
	{
	  buffer = TTF_RenderUTF8_Blended (text_callback_data->font, utf8, text_callback_data->sdl_color_fg);
	  if (buffer)
	    {
	      text_callback_data->utils_context->surface_counter.new_counter++;

	      src_rect.x = 0;
	      src_rect.y = 0;
	      src_rect.w = lw6sys_imin (buffer->w, text_callback_data->shape.w - 2 * src_rect.x);
	      src_rect.h = buffer->h;
	      dst_rect.x = text_callback_data->pos.x;
	      dst_rect.y = text_callback_data->pos.y;
	      dst_rect.w = src_rect.w;
	      dst_rect.h = src_rect.h;

	      if (dst_rect.y + dst_rect.h < text_callback_data->shape.h - 2 * src_rect.x)
		{
		  SDL_BlitSurface (buffer, &src_rect, text_callback_data->target, &dst_rect);
		}

	      text_callback_data->pos.y += buffer->h;

	      mod_gl1_utils_delete_surface (sys_context, text_callback_data->utils_context, buffer);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("TTF_RenderUTF8_Shaded failed"));
	    }
	  LW6SYS_FREE (sys_context, utf8);
	}
    }
}

mod_gl1_utils_bitmap_t *
mod_gl1_utils_multiline_text_write (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
				    utils_context, TTF_Font * font,
				    const char *text,
				    const lw6map_color_couple_t * color,
				    float alpha_bg, int max_width, int max_height, int border_size, int margin_size, int reformat_width)
{
  char *reformatted = NULL;
  lw6sys_list_t *lines = NULL;
  mod_gl1_utils_bitmap_t *ret = NULL;
  mod_gl1_utils_multiline_text_callback_data_t data;
  Uint32 i_color_bg;
  Uint32 i_color_fg;

  memset (&data, 0, sizeof (mod_gl1_utils_multiline_text_callback_data_t));
  reformatted = lw6sys_str_reformat (sys_context, text, "", reformat_width);
  if (reformatted)
    {
      lines = lw6sys_str_split (sys_context, reformatted, '\n');
      if (lines)
	{
	  data.utils_context = utils_context;
	  data.font = font;
	  lw6sys_list_map (sys_context, lines, _multiline_text_size_callback, (void *) &data);
	  data.shape.w += (2 * (border_size + margin_size));
	  data.shape.h += (2 * (border_size + margin_size));
	  data.pos.x = border_size + margin_size;
	  data.pos.y = border_size + margin_size;
	  data.shape.w = lw6sys_imax (1 + 2 * (border_size + margin_size), lw6sys_imin (data.shape.w, max_width));
	  data.shape.h = lw6sys_imax (1 + 2 * (border_size + margin_size), lw6sys_imin (data.shape.h, max_height));

	  data.target = mod_gl1_utils_create_surface (sys_context, utils_context, data.shape.w, data.shape.h);
	  if (data.target)
	    {
	      data.sdl_color_bg = mod_gl1_utils_color_8_to_sdl (sys_context, color->bg);
	      data.sdl_color_fg = mod_gl1_utils_color_8_to_sdl (sys_context, color->fg);
	      i_color_bg = lw6sys_color_8_to_irgba (color->bg);
	      i_color_fg = lw6sys_color_8_to_irgba (color->fg);

	      mod_gl1_utils_draw_rectfill (sys_context, data.target, 0, 0, data.shape.w - 1, data.shape.h - 1, i_color_fg);
	      mod_gl1_utils_draw_rectfill (sys_context, data.target, border_size, border_size, data.shape.w - 1 - border_size, data.shape.h - 1 - border_size,
					   i_color_bg);

	      lw6sys_list_map (sys_context, lines, _multiline_text_draw_callback, (void *) &data);
	      mod_gl1_utils_draw_set_alpha_for_color (sys_context, data.target, alpha_bg, i_color_bg);
	      ret = mod_gl1_utils_surface2bitmap (sys_context, utils_context, data.target, _x_ ("multiline"));
	      if (!ret)
		{
		  mod_gl1_utils_delete_surface (sys_context, utils_context, data.target);
		}
	    }

	  lw6sys_list_free (sys_context, lines);
	}
      LW6SYS_FREE (sys_context, reformatted);
    }

  return ret;
}

/*
 * Helper internal func
 */
static void
_draw_text (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
	    TTF_Font * font, const char *text, const lw6map_color_couple_t * color, float x, float y, float dw, float dh)
{
  mod_gl1_utils_shaded_text_t *shaded_text;
  int ret = 0;

  shaded_text = mod_gl1_utils_shaded_text_new (sys_context, utils_context, font, text, color);
  if (shaded_text != NULL)
    {
      mod_gl1_utils_shaded_text_display (sys_context, utils_context, shaded_text, x, y, x + shaded_text->texture_w, y + shaded_text->texture_h, dw, dh);
      mod_gl1_utils_shaded_text_free (sys_context, utils_context, shaded_text);
      ret = 1;
    }

  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to draw text \"%s\""), text);
    }
}

/*
 * Draws a font on the screen, at a given position
 */
void
mod_gl1_utils_draw_system_text (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look, const char *text, int x,
				int y)
{
  mod_gl1_utils_set_render_mode_2d_blend (sys_context, utils_context);

  _draw_text (sys_context, utils_context, utils_context->font_data.system,
	      text, &(look->style.color_set.system_color), x, y, utils_context->const_data.system_font_dw, utils_context->const_data.system_font_dh);
}

void
mod_gl1_utils_draw_system_text_top_left (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look,
					 const char **text_list)
{
  int y;
  int w;
  int h;

  mod_gl1_utils_set_render_mode_2d_blend (sys_context, utils_context);

  y = 0;
  while (*text_list)
    {
      _get_text_wh (sys_context, utils_context->font_data.system, *text_list, &w, &h);
      _draw_text (sys_context, utils_context, utils_context->font_data.system,
		  *text_list, &(look->style.color_set.system_color), 0, y, utils_context->const_data.system_font_dw, utils_context->const_data.system_font_dh);
      y += (utils_context->const_data.system_font_hcoef * h) + utils_context->const_data.system_font_dh;
      text_list++;
    }
}

void
mod_gl1_utils_draw_system_text_top_right (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look,
					  const char **text_list)
{
  int x;
  int y;
  int w;
  int h;

  mod_gl1_utils_set_render_mode_2d_blend (sys_context, utils_context);

  x = utils_context->sdl_context.video_mode.width;
  y = 0;
  while (*text_list)
    {
      _get_text_wh (sys_context, utils_context->font_data.system, *text_list, &w, &h);
      _draw_text (sys_context, utils_context, utils_context->font_data.system,
		  *text_list, &(look->style.color_set.system_color),
		  x - w, y, utils_context->const_data.system_font_dw, utils_context->const_data.system_font_dh);
      y += (utils_context->const_data.system_font_hcoef * h) + utils_context->const_data.system_font_dh;
      text_list++;
    }
}

void
mod_gl1_utils_draw_system_text_bottom_left (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look,
					    const char **text_list)
{
  int y;
  int w;
  int h;
  float hcoef = 1.0f;
  const char **tmp_list;
  const char **list_end;

  mod_gl1_utils_set_render_mode_2d_blend (sys_context, utils_context);

  list_end = text_list;
  while (*list_end)
    {
      list_end++;
    }

  y = utils_context->sdl_context.video_mode.height;
  tmp_list = list_end;
  while (tmp_list != text_list)
    {
      tmp_list--;

      _get_text_wh (sys_context, utils_context->font_data.system, *tmp_list, &w, &h);
      y -= (hcoef * h) + utils_context->const_data.system_font_dh;
      _draw_text (sys_context, utils_context, utils_context->font_data.system,
		  *tmp_list, &(look->style.color_set.system_color), 0, y, utils_context->const_data.system_font_dw, utils_context->const_data.system_font_dh);
      hcoef = utils_context->const_data.system_font_hcoef;
    }
}

void
mod_gl1_utils_draw_system_text_bottom_right (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look,
					     const char **text_list)
{
  int x;
  int y;
  int w;
  int h;
  float hcoef = 1.0f;
  const char **tmp_list;
  const char **list_end;

  mod_gl1_utils_set_render_mode_2d_blend (sys_context, utils_context);

  list_end = text_list;
  while (*list_end)
    {
      list_end++;
    }

  x = utils_context->sdl_context.video_mode.width;
  y = utils_context->sdl_context.video_mode.height;
  tmp_list = list_end;
  while (tmp_list != text_list)
    {
      tmp_list--;

      _get_text_wh (sys_context, utils_context->font_data.system, *tmp_list, &w, &h);
      y -= (hcoef * h) + utils_context->const_data.system_font_dh;
      _draw_text (sys_context, utils_context, utils_context->font_data.system,
		  *tmp_list, &(look->style.color_set.system_color),
		  x - w, y, utils_context->const_data.system_font_dw, utils_context->const_data.system_font_dh);
      hcoef = utils_context->const_data.system_font_hcoef;
    }
}
