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

#include "../../gfx.h"
#include "gl1-utils.h"

#define _DESC_TEXTURE_1X1 "texture_1x1"

static void
_bitmap_free (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, mod_gl1_utils_bitmap_t * bitmap)
{
  if (bitmap)
    {
      mod_gl1_utils_bitmap_free (sys_context, utils_context, bitmap);
    }
}

mod_gl1_utils_bitmap_t *
mod_gl1_utils_texture_1x1_color2bitmap (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, lw6sys_color_8_t color_8)
{
  mod_gl1_utils_bitmap_t *ret = NULL;
  Uint32 pixel;

  ret = mod_gl1_utils_bitmap_new (sys_context, utils_context, 1, 1, _DESC_TEXTURE_1X1);
  if (ret)
    {
      pixel = lw6sys_color_8_to_irgba (color_8);
      mod_gl1_utils_putpixel (sys_context, ret->surface, 0, 0, pixel);
      mod_gl1_utils_bitmap_clear_texture (sys_context, utils_context, ret);
    }

  return ret;
}

int
mod_gl1_utils_texture_1x1_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, const lw6gui_look_t * look)
{
  int ret = 1;
  int i, j;
  lw6sys_color_8_t color_tmp;

  mod_gl1_utils_texture_1x1_clear (sys_context, utils_context);

  ret =
    ((utils_context->textures_1x1.color_base_fg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.color_base.fg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.color_base_bg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.color_base.bg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.color_alternate_fg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.color_alternate.fg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.color_alternate_bg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.color_alternate.bg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.background_color_root_fg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.background_color_root.fg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.background_color_root_bg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.background_color_root.bg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.background_color_stuff_fg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.background_color_stuff.fg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.background_color_stuff_bg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.background_color_stuff.bg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.hud_color_frame_fg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.hud_color_frame.fg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.hud_color_frame_bg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.hud_color_frame.bg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.hud_color_text_fg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.hud_color_text.fg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.hud_color_text_bg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.hud_color_text.bg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.menu_color_default_fg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.menu_color_default.fg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.menu_color_default_bg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.menu_color_default.bg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.menu_color_selected_fg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.menu_color_selected.fg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.menu_color_selected_bg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.menu_color_selected.bg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.menu_color_disabled_fg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.menu_color_disabled.fg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.menu_color_disabled_bg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.menu_color_disabled.bg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.view_color_cursor_fg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.view_color_cursor.fg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.view_color_cursor_bg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.view_color_cursor.bg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.view_color_map_fg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.view_color_map.fg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.view_color_map_bg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.view_color_map.bg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.system_color_fg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.system_color.fg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.system_color_bg =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.system_color.bg)) != NULL) && ret;
  ret =
    ((utils_context->textures_1x1.team_color_dead =
      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, look->style.color_set.team_color_dead)) != NULL) && ret;
  for (i = 0; i < LW6MAP_NB_TEAM_COLORS; ++i)
    {
      color_tmp = look->style.color_set.team_colors[i];
      ret = ((utils_context->textures_1x1.team_colors[i] = mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, color_tmp)) != NULL) && ret;
      for (j = 0; j <= MOD_GL1_UTILS_TRANSPARENCY_SCALE; ++j)
	{
	  color_tmp.a = (j * 255) / MOD_GL1_UTILS_TRANSPARENCY_SCALE;
	  ret =
	    ((utils_context->textures_1x1.team_colors_transparency[i][j] =
	      mod_gl1_utils_texture_1x1_color2bitmap (sys_context, utils_context, color_tmp)) != NULL) && ret;
	}
    }

  return ret;
}

void
mod_gl1_utils_texture_1x1_clear (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  int i, j;

  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.color_base_fg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.color_base_bg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.color_alternate_fg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.color_alternate_bg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.background_color_root_fg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.background_color_root_bg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.background_color_stuff_fg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.background_color_stuff_bg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.hud_color_frame_fg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.hud_color_frame_bg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.hud_color_text_fg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.hud_color_text_bg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.menu_color_default_fg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.menu_color_default_bg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.menu_color_selected_fg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.menu_color_selected_bg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.menu_color_disabled_fg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.menu_color_disabled_bg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.view_color_cursor_fg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.view_color_cursor_bg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.view_color_map_fg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.view_color_map_bg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.system_color_fg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.system_color_bg);
  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.team_color_dead);
  for (i = 0; i < LW6MAP_NB_TEAM_COLORS; ++i)
    {
      _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.team_colors[i]);
      for (j = 0; j <= MOD_GL1_UTILS_TRANSPARENCY_SCALE; ++j)
	{
	  _bitmap_free (sys_context, utils_context, utils_context->textures_1x1.team_colors_transparency[i][j]);
	}
    }
  memset (&(utils_context->textures_1x1), 0, sizeof (mod_gl1_utils_texture_1x1_t));
}
