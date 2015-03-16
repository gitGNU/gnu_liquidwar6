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

#include <string.h>

#include "map.h"

/**
 * lw6map_style_zero
 *
 * @sys_context: global system context
 * @style: struct to initialize
 *
 * Sets a style struct to zero, simply puts zero everywhere
 * without checking what was here before
 *
 * Return value: none.
 */
void
lw6map_style_zero (lw6sys_context_t * sys_context, lw6map_style_t * style)
{
  memset (style, 0, sizeof (lw6map_style_t));
}

/**
 * lw6map_style_defaults
 *
 * @sys_context: global system context
 * @style: struct to modify
 *
 * Sets a style struct to defaults values, expects
 * the object to be in a consistent style, that's to
 * say either containing real data or being zeroed.
 *
 * Return value: none.
 */
void
lw6map_style_defaults (lw6sys_context_t * sys_context, lw6map_style_t * style)
{
  lw6map_style_clear (sys_context, style);

  style->keep_ratio = LW6MAP_STYLE_DEFAULT_KEEP_RATIO;
  style->zoom = LW6MAP_STYLE_DEFAULT_ZOOM;
  style->zoom_min = LW6MAP_STYLE_DEFAULT_ZOOM_MIN;
  style->zoom_max = LW6MAP_STYLE_DEFAULT_ZOOM_MAX;
  style->x_wrap = LW6MAP_STYLE_DEFAULT_X_WRAP;
  style->y_wrap = LW6MAP_STYLE_DEFAULT_Y_WRAP;
  style->background_style = lw6sys_str_copy (sys_context, LW6MAP_STYLE_DEFAULT_BACKGROUND_STYLE);
  style->hud_style = lw6sys_str_copy (sys_context, LW6MAP_STYLE_DEFAULT_HUD_STYLE);
  style->menu_style = lw6sys_str_copy (sys_context, LW6MAP_STYLE_DEFAULT_MENU_STYLE);
  style->view_style = lw6sys_str_copy (sys_context, LW6MAP_STYLE_DEFAULT_VIEW_STYLE);
  style->animation_density = LW6MAP_STYLE_DEFAULT_ANIMATION_DENSITY;
  style->animation_speed = LW6MAP_STYLE_DEFAULT_ANIMATION_SPEED;
  style->cursor_size = LW6MAP_STYLE_DEFAULT_CURSOR_SIZE;
  style->colorize_cursor = LW6MAP_STYLE_DEFAULT_COLORIZE_CURSOR;
  style->blink_cursor = LW6MAP_STYLE_DEFAULT_BLINK_CURSOR;
  style->hidden_layer_alpha = LW6MAP_STYLE_DEFAULT_HIDDEN_LAYER_ALPHA;
  style->colorize = LW6MAP_STYLE_DEFAULT_COLORIZE;
  style->pixelize = LW6MAP_STYLE_DEFAULT_PIXELIZE;
  style->color_set.color_base.bg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_COLOR_BASE_BG);
  style->color_set.color_base.fg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_COLOR_BASE_FG);
  style->color_set.color_alternate.bg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_COLOR_ALTERNATE_BG);
  style->color_set.color_alternate.fg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_COLOR_ALTERNATE_FG);
  style->color_set.background_color_root.bg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_BACKGROUND_COLOR_ROOT_BG);
  style->color_set.background_color_root.fg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_BACKGROUND_COLOR_ROOT_FG);
  style->color_set.background_color_stuff.bg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_BACKGROUND_COLOR_STUFF_BG);
  style->color_set.background_color_stuff.fg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_BACKGROUND_COLOR_STUFF_FG);
  style->color_set.hud_color_frame.bg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_HUD_COLOR_FRAME_BG);
  style->color_set.hud_color_frame.fg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_HUD_COLOR_FRAME_FG);
  style->color_set.hud_color_text.bg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_HUD_COLOR_TEXT_BG);
  style->color_set.hud_color_text.fg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_HUD_COLOR_TEXT_FG);
  style->color_set.menu_color_default.bg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_MENU_COLOR_DEFAULT_BG);
  style->color_set.menu_color_default.fg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_MENU_COLOR_DEFAULT_FG);
  style->color_set.menu_color_selected.bg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_MENU_COLOR_SELECTED_BG);
  style->color_set.menu_color_selected.fg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_MENU_COLOR_SELECTED_FG);
  style->color_set.menu_color_disabled.bg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_MENU_COLOR_DISABLED_BG);
  style->color_set.menu_color_disabled.fg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_MENU_COLOR_DISABLED_FG);
  style->color_set.view_color_cursor.bg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_VIEW_COLOR_CURSOR_BG);
  style->color_set.view_color_cursor.fg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_VIEW_COLOR_CURSOR_FG);
  style->color_set.view_color_map.bg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_VIEW_COLOR_MAP_BG);
  style->color_set.view_color_map.fg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_VIEW_COLOR_MAP_FG);
  style->color_set.system_color.bg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_SYSTEM_COLOR_BG);
  style->color_set.system_color.fg = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_SYSTEM_COLOR_FG);
  style->color_set.team_color_dead = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_TEAM_COLOR_DEAD);
  style->color_set.team_colors[LW6MAP_TEAM_COLOR_RED] = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_TEAM_COLOR_RED);
  style->color_set.team_colors[LW6MAP_TEAM_COLOR_GREEN] = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_TEAM_COLOR_GREEN);
  style->color_set.team_colors[LW6MAP_TEAM_COLOR_BLUE] = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_TEAM_COLOR_BLUE);
  style->color_set.team_colors[LW6MAP_TEAM_COLOR_YELLOW] = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_TEAM_COLOR_YELLOW);
  style->color_set.team_colors[LW6MAP_TEAM_COLOR_CYAN] = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_TEAM_COLOR_CYAN);
  style->color_set.team_colors[LW6MAP_TEAM_COLOR_MAGENTA] = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_TEAM_COLOR_MAGENTA);
  style->color_set.team_colors[LW6MAP_TEAM_COLOR_ORANGE] = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_TEAM_COLOR_ORANGE);
  style->color_set.team_colors[LW6MAP_TEAM_COLOR_LIGHTBLUE] = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_TEAM_COLOR_LIGHTBLUE);
  style->color_set.team_colors[LW6MAP_TEAM_COLOR_PURPLE] = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_TEAM_COLOR_PURPLE);
  style->color_set.team_colors[LW6MAP_TEAM_COLOR_PINK] = lw6sys_color_a_to_8 (sys_context, LW6MAP_STYLE_DEFAULT_TEAM_COLOR_PINK);
  style->music_file = lw6sys_str_copy (sys_context, LW6MAP_STYLE_DEFAULT_MUSIC_FILE);
  style->music_filter = lw6sys_str_copy (sys_context, LW6MAP_STYLE_DEFAULT_MUSIC_FILTER);
  style->music_exclude = lw6sys_str_copy (sys_context, LW6MAP_STYLE_DEFAULT_MUSIC_EXCLUDE);
  style->waves = LW6MAP_STYLE_DEFAULT_WAVES;
}

/**
 * lw6map_style_clear
 *
 * @sys_context: global system context
 * @style: struct to clear
 *
 * Clears a style struct. This function won't work on an unitialized
 * structure, structure must be zeroed by some CALLOC
 * or something, else automatic freeing of pointers
 * will fail.
 *
 * Return value: none.
 */
void
lw6map_style_clear (lw6sys_context_t * sys_context, lw6map_style_t * style)
{
  if (style)
    {
      if (style->background_style)
	{
	  LW6SYS_FREE (sys_context, style->background_style);
	}

      if (style->hud_style)
	{
	  LW6SYS_FREE (sys_context, style->hud_style);
	}

      if (style->menu_style)
	{
	  LW6SYS_FREE (sys_context, style->menu_style);
	}

      if (style->view_style)
	{
	  LW6SYS_FREE (sys_context, style->view_style);
	}

      if (style->music_file)
	{
	  LW6SYS_FREE (sys_context, style->music_file);
	}

      if (style->music_filter)
	{
	  LW6SYS_FREE (sys_context, style->music_filter);
	}

      if (style->music_exclude)
	{
	  LW6SYS_FREE (sys_context, style->music_exclude);
	}

      lw6map_style_zero (sys_context, style);
    }
}

/**
 * lw6map_style_copy
 *
 * @sys_context: global system context
 * @dst: destination
 * @src: source
 *
 * Copies style data from source to destination.
 * Like with clear, @dst must be either initialized or
 * totally zeroed, else function will fail (core dump)
 *
 * Return value: none.
 */
void
lw6map_style_copy (lw6sys_context_t * sys_context, lw6map_style_t * dst, const lw6map_style_t * src)
{
  lw6map_style_clear (sys_context, dst);
  memcpy (dst, src, sizeof (lw6map_style_t));

  if (src->background_style)
    {
      dst->background_style = lw6sys_str_copy (sys_context, src->background_style);
    }
  if (src->hud_style)
    {
      dst->hud_style = lw6sys_str_copy (sys_context, src->hud_style);
    }
  if (src->menu_style)
    {
      dst->menu_style = lw6sys_str_copy (sys_context, src->menu_style);
    }
  if (src->view_style)
    {
      dst->view_style = lw6sys_str_copy (sys_context, src->view_style);
    }
  if (src->music_file)
    {
      dst->music_file = lw6sys_str_copy (sys_context, src->music_file);
    }
  if (src->music_filter)
    {
      dst->music_filter = lw6sys_str_copy (sys_context, src->music_filter);
    }
  if (src->music_exclude)
    {
      dst->music_exclude = lw6sys_str_copy (sys_context, src->music_exclude);
    }
}

/**
 * lw6map_style_set
 *
 * @sys_context: global system context
 * @style: style struct to modify (out param)
 * @key: key to use
 * @value: value to use
 *
 * Sets a style entry, takes string values and will
 * identify the struct offset and convert the value
 * to whatever C type is needed.
 *
 * Return value: 1 on success, 0 on failure (key not found)
 */
int
lw6map_style_set (lw6sys_context_t * sys_context, lw6map_style_t * style, const char *key, const char *value)
{
  int ret = 1;
  char *formatted_key = NULL;

  formatted_key = lw6sys_keyword_as_key (sys_context, key);
  if (formatted_key)
    {
      if (!strcmp (LW6DEF_KEEP_RATIO, formatted_key))
	{
	  style->keep_ratio = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_ZOOM, formatted_key))
	{
	  style->zoom = lw6sys_atof (sys_context, value);
	}
      else if (!strcmp (LW6DEF_ZOOM_MIN, formatted_key))
	{
	  style->zoom_min = lw6sys_atof (sys_context, value);
	}
      else if (!strcmp (LW6DEF_ZOOM_MAX, formatted_key))
	{
	  style->zoom_max = lw6sys_atof (sys_context, value);
	}
      else if (!strcmp (LW6DEF_X_WRAP, formatted_key))
	{
	  style->x_wrap = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_Y_WRAP, formatted_key))
	{
	  style->y_wrap = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BACKGROUND_STYLE, formatted_key))
	{
	  if (style->background_style)
	    {
	      LW6SYS_FREE (sys_context, style->background_style);
	    }
	  style->background_style = lw6sys_str_copy (sys_context, value);
	}
      else if (!strcmp (LW6DEF_HUD_STYLE, formatted_key))
	{
	  if (style->hud_style)
	    {
	      LW6SYS_FREE (sys_context, style->hud_style);
	    }
	  style->hud_style = lw6sys_str_copy (sys_context, value);
	}
      else if (!strcmp (LW6DEF_MENU_STYLE, formatted_key))
	{
	  if (style->menu_style)
	    {
	      LW6SYS_FREE (sys_context, style->menu_style);
	    }
	  style->menu_style = lw6sys_str_copy (sys_context, value);
	}
      else if (!strcmp (LW6DEF_VIEW_STYLE, formatted_key))
	{
	  if (style->view_style)
	    {
	      LW6SYS_FREE (sys_context, style->view_style);
	    }
	  style->view_style = lw6sys_str_copy (sys_context, value);
	}
      else if (!strcmp (LW6DEF_ANIMATION_DENSITY, formatted_key))
	{
	  style->animation_density = lw6sys_atof (sys_context, value);
	}
      else if (!strcmp (LW6DEF_ANIMATION_SPEED, formatted_key))
	{
	  style->animation_speed = lw6sys_atof (sys_context, value);
	}
      else if (!strcmp (LW6DEF_CURSOR_SIZE, formatted_key))
	{
	  style->cursor_size = lw6sys_atof (sys_context, value);
	}
      else if (!strcmp (LW6DEF_COLORIZE_CURSOR, formatted_key))
	{
	  style->colorize_cursor = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BLINK_CURSOR, formatted_key))
	{
	  style->blink_cursor = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_HIDDEN_LAYER_ALPHA, formatted_key))
	{
	  style->hidden_layer_alpha = lw6sys_atof (sys_context, value);
	}
      else if (!strcmp (LW6DEF_COLORIZE, formatted_key))
	{
	  style->colorize = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_PIXELIZE, formatted_key))
	{
	  style->pixelize = lw6sys_atob (sys_context, value);
	}
      else if (!strcmp (LW6DEF_COLOR_BASE_BG, formatted_key))
	{
	  style->color_set.color_base.bg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_COLOR_BASE_FG, formatted_key))
	{
	  style->color_set.color_base.fg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_COLOR_ALTERNATE_BG, formatted_key))
	{
	  style->color_set.color_alternate.bg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_COLOR_ALTERNATE_FG, formatted_key))
	{
	  style->color_set.color_alternate.fg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BACKGROUND_COLOR_ROOT_BG, formatted_key))
	{
	  style->color_set.background_color_root.bg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BACKGROUND_COLOR_ROOT_FG, formatted_key))
	{
	  style->color_set.background_color_root.fg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BACKGROUND_COLOR_STUFF_BG, formatted_key))
	{
	  style->color_set.background_color_stuff.bg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_BACKGROUND_COLOR_STUFF_FG, formatted_key))
	{
	  style->color_set.background_color_stuff.fg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_HUD_COLOR_FRAME_BG, formatted_key))
	{
	  style->color_set.hud_color_frame.bg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_HUD_COLOR_FRAME_FG, formatted_key))
	{
	  style->color_set.hud_color_frame.fg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_HUD_COLOR_TEXT_BG, formatted_key))
	{
	  style->color_set.hud_color_text.bg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_HUD_COLOR_TEXT_FG, formatted_key))
	{
	  style->color_set.hud_color_text.fg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_MENU_COLOR_DEFAULT_BG, formatted_key))
	{
	  style->color_set.menu_color_default.bg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_MENU_COLOR_DEFAULT_FG, formatted_key))
	{
	  style->color_set.menu_color_default.fg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_MENU_COLOR_SELECTED_BG, formatted_key))
	{
	  style->color_set.menu_color_selected.bg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_MENU_COLOR_SELECTED_FG, formatted_key))
	{
	  style->color_set.menu_color_selected.fg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_MENU_COLOR_DISABLED_BG, formatted_key))
	{
	  style->color_set.menu_color_disabled.bg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_MENU_COLOR_DISABLED_FG, formatted_key))
	{
	  style->color_set.menu_color_disabled.fg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_VIEW_COLOR_CURSOR_BG, formatted_key))
	{
	  style->color_set.view_color_cursor.bg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_VIEW_COLOR_CURSOR_FG, formatted_key))
	{
	  style->color_set.view_color_cursor.fg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_VIEW_COLOR_MAP_BG, formatted_key))
	{
	  style->color_set.view_color_map.bg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_VIEW_COLOR_MAP_FG, formatted_key))
	{
	  style->color_set.view_color_map.fg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_SYSTEM_COLOR_BG, formatted_key))
	{
	  style->color_set.system_color.bg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_SYSTEM_COLOR_FG, formatted_key))
	{
	  style->color_set.system_color.fg = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_DEAD, formatted_key))
	{
	  style->color_set.team_color_dead = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_RED, formatted_key))
	{
	  style->color_set.team_colors[LW6MAP_TEAM_COLOR_RED] = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_GREEN, formatted_key))
	{
	  style->color_set.team_colors[LW6MAP_TEAM_COLOR_GREEN] = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_BLUE, formatted_key))
	{
	  style->color_set.team_colors[LW6MAP_TEAM_COLOR_BLUE] = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_YELLOW, formatted_key))
	{
	  style->color_set.team_colors[LW6MAP_TEAM_COLOR_YELLOW] = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_CYAN, formatted_key))
	{
	  style->color_set.team_colors[LW6MAP_TEAM_COLOR_CYAN] = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_MAGENTA, formatted_key))
	{
	  style->color_set.team_colors[LW6MAP_TEAM_COLOR_MAGENTA] = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_ORANGE, formatted_key))
	{
	  style->color_set.team_colors[LW6MAP_TEAM_COLOR_ORANGE] = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_LIGHTBLUE, formatted_key))
	{
	  style->color_set.team_colors[LW6MAP_TEAM_COLOR_LIGHTBLUE] = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_PURPLE, formatted_key))
	{
	  style->color_set.team_colors[LW6MAP_TEAM_COLOR_PURPLE] = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_PINK, formatted_key))
	{
	  style->color_set.team_colors[LW6MAP_TEAM_COLOR_PINK] = lw6sys_color_a_to_8 (sys_context, value);
	}
      else if (!strcmp (LW6DEF_MUSIC_FILE, formatted_key))
	{
	  if (style->music_file)
	    {
	      LW6SYS_FREE (sys_context, style->music_file);
	    }
	  style->music_file = lw6sys_str_copy (sys_context, value);
	}
      else if (!strcmp (LW6DEF_MUSIC_FILTER, formatted_key))
	{
	  if (style->music_filter)
	    {
	      LW6SYS_FREE (sys_context, style->music_filter);
	    }
	  style->music_filter = lw6sys_str_copy (sys_context, value);
	}
      else if (!strcmp (LW6DEF_MUSIC_EXCLUDE, formatted_key))
	{
	  if (style->music_exclude)
	    {
	      LW6SYS_FREE (sys_context, style->music_exclude);
	    }
	  style->music_exclude = lw6sys_str_copy (sys_context, value);
	}
      else if (!strcmp (LW6DEF_WAVES, formatted_key))
	{
	  style->waves = lw6sys_atob (sys_context, value);
	}
      else
	{
	  ret = 0;		// no entry matches
	}
      LW6SYS_FREE (sys_context, formatted_key);
    }
  else
    {
      ret = 0;
    }

  return ret;
}

/**
 * lw6map_style_get
 *
 * @sys_context: global system context
 * @style: style struct to query
 * @key: key to use
 *
 * Get a style entry, takes a string key and will
 * identify the struct offset. The return value is
 * converted to string, typically the cannonical
 * representation suitable to write in an XML config file.
 *
 * Return value: dynamically allocated string.
 */
char *
lw6map_style_get (lw6sys_context_t * sys_context, const lw6map_style_t * style, const char *key)
{
  char *ret = NULL;
  char *formatted_key = NULL;

  formatted_key = lw6sys_keyword_as_key (sys_context, key);
  if (formatted_key)
    {
      if (!strcmp (LW6DEF_KEEP_RATIO, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, style->keep_ratio);
	}
      else if (!strcmp (LW6DEF_ZOOM, formatted_key))
	{
	  ret = lw6sys_ftoa (sys_context, style->zoom);
	}
      else if (!strcmp (LW6DEF_ZOOM_MIN, formatted_key))
	{
	  ret = lw6sys_ftoa (sys_context, style->zoom_min);
	}
      else if (!strcmp (LW6DEF_ZOOM_MAX, formatted_key))
	{
	  ret = lw6sys_ftoa (sys_context, style->zoom_max);
	}
      else if (!strcmp (LW6DEF_X_WRAP, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, style->x_wrap);
	}
      else if (!strcmp (LW6DEF_Y_WRAP, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, style->y_wrap);
	}
      else if (!strcmp (LW6DEF_BACKGROUND_STYLE, formatted_key))
	{
	  ret = lw6sys_str_copy (sys_context, style->background_style);
	}
      else if (!strcmp (LW6DEF_HUD_STYLE, formatted_key))
	{
	  ret = lw6sys_str_copy (sys_context, style->hud_style);
	}
      else if (!strcmp (LW6DEF_MENU_STYLE, formatted_key))
	{
	  ret = lw6sys_str_copy (sys_context, style->menu_style);
	}
      else if (!strcmp (LW6DEF_VIEW_STYLE, formatted_key))
	{
	  ret = lw6sys_str_copy (sys_context, style->view_style);
	}
      else if (!strcmp (LW6DEF_ANIMATION_DENSITY, formatted_key))
	{
	  ret = lw6sys_ftoa (sys_context, style->animation_density);
	}
      else if (!strcmp (LW6DEF_ANIMATION_SPEED, formatted_key))
	{
	  ret = lw6sys_ftoa (sys_context, style->animation_speed);
	}
      else if (!strcmp (LW6DEF_CURSOR_SIZE, formatted_key))
	{
	  ret = lw6sys_ftoa (sys_context, style->cursor_size);
	}
      else if (!strcmp (LW6DEF_COLORIZE_CURSOR, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, style->colorize_cursor);
	}
      else if (!strcmp (LW6DEF_BLINK_CURSOR, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, style->blink_cursor);
	}
      else if (!strcmp (LW6DEF_HIDDEN_LAYER_ALPHA, formatted_key))
	{
	  ret = lw6sys_ftoa (sys_context, style->hidden_layer_alpha);
	}
      else if (!strcmp (LW6DEF_COLORIZE, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, style->colorize);
	}
      else if (!strcmp (LW6DEF_PIXELIZE, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, style->pixelize);
	}
      else if (!strcmp (LW6DEF_COLOR_BASE_BG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.color_base.bg);
	}
      else if (!strcmp (LW6DEF_COLOR_BASE_FG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.color_base.fg);
	}
      else if (!strcmp (LW6DEF_COLOR_ALTERNATE_BG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.color_alternate.bg);
	}
      else if (!strcmp (LW6DEF_COLOR_ALTERNATE_FG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.color_alternate.fg);
	}
      else if (!strcmp (LW6DEF_BACKGROUND_COLOR_ROOT_BG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.background_color_root.bg);
	}
      else if (!strcmp (LW6DEF_BACKGROUND_COLOR_ROOT_FG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.background_color_root.fg);
	}
      else if (!strcmp (LW6DEF_BACKGROUND_COLOR_STUFF_BG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.background_color_stuff.bg);
	}
      else if (!strcmp (LW6DEF_BACKGROUND_COLOR_STUFF_FG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.background_color_stuff.fg);
	}
      else if (!strcmp (LW6DEF_HUD_COLOR_FRAME_BG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.hud_color_frame.bg);
	}
      else if (!strcmp (LW6DEF_HUD_COLOR_FRAME_FG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.hud_color_frame.fg);
	}
      else if (!strcmp (LW6DEF_HUD_COLOR_TEXT_BG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.color_base.bg);
	}
      else if (!strcmp (LW6DEF_HUD_COLOR_TEXT_FG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.color_base.fg);
	}
      else if (!strcmp (LW6DEF_MENU_COLOR_DEFAULT_BG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.menu_color_default.bg);
	}
      else if (!strcmp (LW6DEF_MENU_COLOR_DEFAULT_FG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.menu_color_default.fg);
	}
      else if (!strcmp (LW6DEF_MENU_COLOR_SELECTED_BG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.menu_color_selected.bg);
	}
      else if (!strcmp (LW6DEF_MENU_COLOR_SELECTED_FG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.menu_color_selected.fg);
	}
      else if (!strcmp (LW6DEF_MENU_COLOR_DISABLED_BG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.menu_color_disabled.bg);
	}
      else if (!strcmp (LW6DEF_MENU_COLOR_DISABLED_FG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.menu_color_disabled.fg);
	}
      else if (!strcmp (LW6DEF_VIEW_COLOR_CURSOR_BG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.view_color_cursor.bg);
	}
      else if (!strcmp (LW6DEF_VIEW_COLOR_CURSOR_FG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.view_color_cursor.fg);
	}
      else if (!strcmp (LW6DEF_VIEW_COLOR_MAP_BG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.view_color_map.bg);
	}
      else if (!strcmp (LW6DEF_VIEW_COLOR_MAP_FG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.view_color_map.fg);
	}
      else if (!strcmp (LW6DEF_SYSTEM_COLOR_BG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.system_color.bg);
	}
      else if (!strcmp (LW6DEF_SYSTEM_COLOR_FG, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.system_color.fg);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_DEAD, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.team_color_dead);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_RED, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.team_colors[LW6MAP_TEAM_COLOR_RED]);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_GREEN, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.team_colors[LW6MAP_TEAM_COLOR_GREEN]);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_BLUE, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.team_colors[LW6MAP_TEAM_COLOR_BLUE]);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_YELLOW, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.team_colors[LW6MAP_TEAM_COLOR_YELLOW]);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_CYAN, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.team_colors[LW6MAP_TEAM_COLOR_CYAN]);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_MAGENTA, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.team_colors[LW6MAP_TEAM_COLOR_MAGENTA]);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_ORANGE, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.team_colors[LW6MAP_TEAM_COLOR_ORANGE]);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_LIGHTBLUE, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.team_colors[LW6MAP_TEAM_COLOR_LIGHTBLUE]);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_PURPLE, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.team_colors[LW6MAP_TEAM_COLOR_PURPLE]);
	}
      else if (!strcmp (LW6DEF_TEAM_COLOR_PINK, formatted_key))
	{
	  ret = lw6sys_color_8_to_a (sys_context, style->color_set.team_colors[LW6MAP_TEAM_COLOR_PINK]);
	}
      else if (!strcmp (LW6DEF_MUSIC_FILE, formatted_key))
	{
	  ret = lw6sys_str_copy (sys_context, style->music_file);
	}
      else if (!strcmp (LW6DEF_MUSIC_FILTER, formatted_key))
	{
	  ret = lw6sys_str_copy (sys_context, style->music_filter);
	}
      else if (!strcmp (LW6DEF_MUSIC_EXCLUDE, formatted_key))
	{
	  ret = lw6sys_str_copy (sys_context, style->music_exclude);
	}
      else if (!strcmp (LW6DEF_WAVES, formatted_key))
	{
	  ret = lw6sys_btoa (sys_context, style->waves);
	}
      LW6SYS_FREE (sys_context, formatted_key);
    }

  return ret;
}

/**
 * lw6map_style_get_default
 *
 * @sys_context: global system context
 * @key: key to query
 *
 * Get the default value for a style entry. This is quite
 * a cost-expensive function given what it does, indeed it
 * will convert anything to a string, and also perform
 * key lookup to fetch the value.
 *
 * Return value: dynamically allocated string.
 */
char *
lw6map_style_get_default (lw6sys_context_t * sys_context, const char *key)
{
  lw6map_style_t *style;
  char *ret = NULL;

  style = (lw6map_style_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6map_style_t));
  if (style)
    {
      lw6map_style_defaults (sys_context, style);
      ret = lw6map_style_get (sys_context, style, key);
      lw6map_style_clear (sys_context, style);
      LW6SYS_FREE (sys_context, style);
    }

  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to get default value for style parameter \"%s\""), key);
    }

  return ret;
}

/**
 * lw6map_color_set_is_same
 *
 * @sys_context: global system context
 * @color_set_a: first item to compare
 * @color_set_b: second item to compare
 *
 * Compares two color sets, telling if they contain
 * the same data.
 *
 * Return value: 1 if same, 0 if different.
 */
int
lw6map_color_set_is_same (lw6sys_context_t * sys_context, const lw6map_color_set_t * color_set_a, const lw6map_color_set_t * color_set_b)
{
  int ret = 0;

  ret = (memcmp (color_set_a, color_set_b, sizeof (lw6map_color_set_t)) == 0);

  return ret;
}

/**
 * lw6map_style_is_same
 *
 * @sys_context: global system context
 * @style_a: first item to compare
 * @style_b: second item to compare
 *
 * Compares two style structures, telling if they contain
 * the same data.
 *
 * Return value: 1 if same, 0 if different.
 */
int
lw6map_style_is_same (lw6sys_context_t * sys_context, const lw6map_style_t * style_a, const lw6map_style_t * style_b)
{
  int ret = 1;

  ret = ret && style_a->keep_ratio == style_b->keep_ratio;
  ret = ret && style_a->zoom == style_b->zoom;
  ret = ret && style_a->zoom_min == style_b->zoom_min;
  ret = ret && style_a->zoom_max == style_b->zoom_max;
  ret = ret && style_a->x_wrap == style_b->x_wrap;
  ret = ret && style_a->y_wrap == style_b->y_wrap;
  ret = ret && lw6sys_str_is_same (sys_context, style_a->background_style, style_b->background_style);
  ret = ret && lw6sys_str_is_same (sys_context, style_a->hud_style, style_b->hud_style);
  ret = ret && lw6sys_str_is_same (sys_context, style_a->menu_style, style_b->menu_style);
  ret = ret && lw6sys_str_is_same (sys_context, style_a->view_style, style_b->view_style);
  ret = ret && style_a->animation_density == style_b->animation_density;
  ret = ret && style_a->animation_speed == style_b->animation_speed;
  ret = ret && style_a->cursor_size == style_b->cursor_size;
  ret = ret && style_a->colorize_cursor == style_b->colorize_cursor;
  ret = ret && style_a->blink_cursor == style_b->blink_cursor;
  ret = ret && style_a->hidden_layer_alpha == style_b->hidden_layer_alpha;
  ret = ret && style_a->colorize == style_b->colorize;
  ret = ret && style_a->pixelize == style_b->pixelize;
  ret = ret && lw6map_color_set_is_same (sys_context, &style_a->color_set, &style_b->color_set);
  ret = ret && lw6sys_str_is_same (sys_context, style_a->music_file, style_b->music_file);
  ret = ret && lw6sys_str_is_same (sys_context, style_a->music_filter, style_b->music_filter);
  ret = ret && lw6sys_str_is_same (sys_context, style_a->music_exclude, style_b->music_exclude);
  ret = ret && style_a->waves == style_b->waves;

  return ret;
}
