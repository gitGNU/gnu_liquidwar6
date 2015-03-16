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

#include "../../../gfx.h"
#include "gl1-flat.h"
#include "gl1-flat-internal.h"

int
_mod_gl1_view_flat_cursor_context_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					utils_context,
					_mod_gl1_view_flat_cursor_context_t
					* cursor_context,
					_mod_gl1_view_flat_const_data_t
					* const_data, const lw6gui_look_t * look, const lw6map_level_t * level, const lw6ker_cursor_t * cursor)
{
  int ret = 0;
  lw6map_color_couple_t color_couple;
  char text[2] = { 0, 0 };

  if (cursor->enabled)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("view_flat cursor init %x '%c' %d"), (int) cursor->cursor_id, cursor->letter, cursor->team_color);
      cursor_context->cursor_id = cursor->cursor_id;
      cursor_context->letter = cursor->letter;
      cursor_context->team_color = cursor->team_color;
      cursor_context->bitmap_color = mod_gl1_utils_cursor_create_color (sys_context, utils_context, look, level, cursor);

      if (cursor->team_color >= 0)
	{
	  color_couple.fg = look->style.color_set.team_colors[cursor->team_color];
	}
      else
	{
	  color_couple.fg = look->style.color_set.team_color_dead;
	}
      color_couple.bg = look->style.color_set.team_color_dead;

      text[0] = cursor->letter;
      cursor_context->shaded_text_letter = mod_gl1_utils_shaded_text_new (sys_context, utils_context, utils_context->font_data.cursor, text, &color_couple);

      ret = (cursor_context->bitmap_color != NULL) && (cursor_context->shaded_text_letter != NULL);
    }

  if (!ret)
    {
      _mod_gl1_view_flat_cursor_context_clear (sys_context, utils_context, cursor_context);
    }

  return ret;
}

int
_mod_gl1_view_flat_cursor_context_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					  utils_context,
					  _mod_gl1_view_flat_cursor_context_t
					  * cursor_context,
					  _mod_gl1_view_flat_const_data_t
					  * const_data, const lw6gui_look_t * look, const lw6map_level_t * level, const lw6ker_cursor_t * cursor)
{
  int ret = 0;

  if (cursor_context->cursor_id == cursor->cursor_id
      && cursor_context->letter == cursor->letter && cursor_context->team_color == cursor->team_color && cursor_context->bitmap_color)
    {
      // nothing to do
      ret = 1;
    }
  else
    {
      _mod_gl1_view_flat_cursor_context_clear (sys_context, utils_context, cursor_context);
      ret = _mod_gl1_view_flat_cursor_context_init (sys_context, utils_context, cursor_context, const_data, look, level, cursor);
    }

  return ret;
}

int
_mod_gl1_view_flat_cursor_context_clear (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					 _mod_gl1_view_flat_cursor_context_t * cursor_context)
{
  int ret = 1;

  if (cursor_context->bitmap_color)
    {
      mod_gl1_utils_bitmap_free (sys_context, utils_context, cursor_context->bitmap_color);
    }
  if (cursor_context->shaded_text_letter)
    {
      mod_gl1_utils_shaded_text_free (sys_context, utils_context, cursor_context->shaded_text_letter);
    }
  memset (cursor_context, 0, sizeof (_mod_gl1_view_flat_cursor_context_t));

  return ret;
}

int
_mod_gl1_view_flat_cursors_context_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					 utils_context,
					 _mod_gl1_view_flat_cursors_context_t
					 * cursors_context,
					 _mod_gl1_view_flat_const_data_t * const_data, const lw6gui_look_t * look, const lw6ker_game_state_t * game_state)
{
  int ret = 1;
  int i;
  lw6ker_cursor_t cursor;
  lw6map_level_t *level = NULL;

  level = game_state->game_struct->level;
  cursors_context->level_id = level->id;
  cursors_context->color = look->style.color_set.view_color_cursor;
  cursors_context->bitmap_fg_bg = mod_gl1_utils_cursor_create_fg_bg (sys_context, utils_context, look, level);
  ret = ret && (cursors_context->bitmap_fg_bg != NULL);
  for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
    {
      lw6ker_game_state_get_cursor_by_index (sys_context, game_state, &cursor, i);
      ret = ret && _mod_gl1_view_flat_cursor_context_init (sys_context, utils_context, &(cursors_context->cursor[i]), const_data, look, level, &cursor);
    }

  return ret;
}

int
_mod_gl1_view_flat_cursors_context_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					   utils_context,
					   _mod_gl1_view_flat_cursors_context_t
					   * cursors_context,
					   _mod_gl1_view_flat_const_data_t * const_data, const lw6gui_look_t * look, const lw6ker_game_state_t * game_state)
{
  int ret = 1;
  int i;
  lw6ker_cursor_t cursor;
  lw6map_level_t *level = NULL;

  level = game_state->game_struct->level;
  if ((cursors_context->level_id != level->id) || (!lw6map_color_is_same (sys_context, &cursors_context->color, &look->style.color_set.view_color_cursor)))
    {
      _mod_gl1_view_flat_cursors_context_clear (sys_context, utils_context, cursors_context);
      ret = ret && _mod_gl1_view_flat_cursors_context_init (sys_context, utils_context, cursors_context, const_data, look, game_state);
    }
  else
    {
      for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
	{
	  lw6ker_game_state_get_cursor_by_index (sys_context, game_state, &cursor, i);
	  ret = ret && _mod_gl1_view_flat_cursor_context_update (sys_context, utils_context, &(cursors_context->cursor[i]), const_data, look, level, &cursor);
	}
    }

  return ret;
}

int
_mod_gl1_view_flat_cursors_context_clear (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					  _mod_gl1_view_flat_cursors_context_t * cursors_context)
{
  int ret = 1;
  int i;

  if (cursors_context->bitmap_fg_bg)
    {
      mod_gl1_utils_bitmap_free (sys_context, utils_context, cursors_context->bitmap_fg_bg);
    }
  for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
    {
      ret = ret && _mod_gl1_view_flat_cursor_context_clear (sys_context, utils_context, &(cursors_context->cursor[i]));
    }
  cursors_context->color.fg = LW6SYS_COLOR_8_WHITE;
  cursors_context->color.bg = LW6SYS_COLOR_8_BLACK;

  return ret;
}
