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

#ifndef LIQUIDWAR6GFX_MOD_GL1_VIEW_FLAT_INTERNAL_H
#define LIQUIDWAR6GFX_MOD_GL1_VIEW_FLAT_INTERNAL_H

#include "gl1-flat.h"

typedef struct _mod_gl1_view_flat_game_context_map_s
{
  int level_id;
  const lw6map_level_t *level;
  mod_gl1_utils_bitmap_array_t map_bitmap_array;
}
_mod_gl1_view_flat_game_context_map_t;

typedef struct _mod_gl1_view_flat_game_context_armies_s
{
  int game_struct_id;
  const lw6ker_game_state_t *game_state;
  mod_gl1_utils_bitmap_array_t armies_bitmap_array;
}
_mod_gl1_view_flat_game_context_armies_t;

typedef struct _mod_gl1_view_flat_game_context_s
{
  _mod_gl1_view_flat_game_context_map_t map;
  _mod_gl1_view_flat_game_context_armies_t armies;
}
_mod_gl1_view_flat_game_context_t;

typedef struct _mod_gl1_view_flat_cursor_context_s
{
  u_int16_t cursor_id;
  char letter;
  int team_color;
  mod_gl1_utils_bitmap_t *bitmap_color;
  mod_gl1_utils_shaded_text_t *shaded_text_letter;
}
_mod_gl1_view_flat_cursor_context_t;

typedef struct _mod_gl1_view_flat_cursors_context_s
{
  u_int32_t level_id;
  lw6map_color_couple_t color;
  mod_gl1_utils_bitmap_t *bitmap_fg_bg;
  _mod_gl1_view_flat_cursor_context_t cursor[LW6MAP_MAX_NB_CURSORS];
}
_mod_gl1_view_flat_cursors_context_t;

typedef struct _mod_gl1_view_flat_const_data_s
{
  float cursor_size;
  float cursor_size_heartbeat_min;
  float cursor_size_heartbeat_max;
  float scroll_limit;
  int scroll_min_offset;
  int cursor_blink_period;
  int cursor_heartbeat_period;
  float cursor_relative_text_y1;
  float cursor_relative_text_y2;
  float cursor_relative_text_x1;
  float cursor_relative_text_dw;
  float cursor_relative_text_dh;
  int tile_size_map;
  int tile_size_armies;
  int border_size_map;
  int border_size_armies;
  int nb_waves;
  int vertices_per_wave;
  int waves_period;
  float waves_amplitude;
}
_mod_gl1_view_flat_const_data_t;

typedef struct _mod_gl1_view_flat_context_s
{
  _mod_gl1_view_flat_game_context_t game_context;
  _mod_gl1_view_flat_cursors_context_t cursors_context;
  _mod_gl1_view_flat_const_data_t const_data;
  lw6gui_viewport_t viewport;
}
_mod_gl1_view_flat_context_t;

/* gl1-flat-cursorcontext.c */
extern int _mod_gl1_view_flat_cursor_context_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						   utils_context,
						   _mod_gl1_view_flat_cursor_context_t
						   * cursor_context,
						   _mod_gl1_view_flat_const_data_t
						   * const_data, const lw6gui_look_t * look, const lw6map_level_t * level, const lw6ker_cursor_t * cursor);
extern int _mod_gl1_view_flat_cursor_context_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						     utils_context,
						     _mod_gl1_view_flat_cursor_context_t
						     * cursor_context,
						     _mod_gl1_view_flat_const_data_t
						     * const_data, const lw6gui_look_t * look, const lw6map_level_t * level, const lw6ker_cursor_t * cursor);
extern int _mod_gl1_view_flat_cursor_context_clear (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
						    _mod_gl1_view_flat_cursor_context_t * cursor_context);
extern int _mod_gl1_view_flat_cursors_context_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
						    _mod_gl1_view_flat_cursors_context_t * cursors_context, _mod_gl1_view_flat_const_data_t * const_data,
						    const lw6gui_look_t * look, const lw6ker_game_state_t * game_state);
extern int _mod_gl1_view_flat_cursors_context_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
						      _mod_gl1_view_flat_cursors_context_t * cursors_context, _mod_gl1_view_flat_const_data_t * const_data,
						      const lw6gui_look_t * look, const lw6ker_game_state_t * game_state);
extern int _mod_gl1_view_flat_cursors_context_clear (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
						     _mod_gl1_view_flat_cursors_context_t * cursors_context);

/* gl1-flat-cursorcontext-data.c */
extern int _mod_gl1_view_flat_load_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_view_flat_context_t * flat_context);
extern void _mod_gl1_view_flat_unload_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					    _mod_gl1_view_flat_context_t * flat_context);

/* gl1-flat-cursorcontext-display.c */
extern void _mod_gl1_view_flat_display_preview (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						utils_context,
						_mod_gl1_view_flat_context_t * flat_context, const lw6gui_look_t * look, const lw6map_level_t * level);
extern void _mod_gl1_view_flat_display_map (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					    utils_context,
					    _mod_gl1_view_flat_context_t *
					    flat_context,
					    const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors);
extern void _mod_gl1_view_flat_display_fighters (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						 utils_context,
						 _mod_gl1_view_flat_context_t
						 * flat_context,
						 const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors);
extern void _mod_gl1_view_flat_display_cursors (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						utils_context,
						_mod_gl1_view_flat_context_t *
						flat_context,
						const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors);

/*
 * In gamecontext.c
 */
extern int _mod_gl1_view_flat_game_context_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						 utils_context,
						 _mod_gl1_view_flat_context_t
						 * flat_context,
						 _mod_gl1_view_flat_game_context_t
						 * game_context,
						 const lw6gui_look_t * look, const lw6map_level_t * level, const lw6ker_game_state_t * game_state);
extern int _mod_gl1_view_flat_game_context_init_map (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						     utils_context,
						     _mod_gl1_view_flat_context_t
						     * flat_context,
						     _mod_gl1_view_flat_game_context_map_t
						     * game_context_map, const lw6gui_look_t * look, const lw6map_level_t * level);
extern int
_mod_gl1_view_flat_game_context_init_armies (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					     utils_context,
					     _mod_gl1_view_flat_context_t *
					     flat_context,
					     _mod_gl1_view_flat_game_context_armies_t
					     * game_context_armies, const lw6gui_look_t * look, const lw6ker_game_state_t * game_state);
extern void _mod_gl1_view_flat_game_context_clear (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						   utils_context,
						   _mod_gl1_view_flat_context_t * flat_context, _mod_gl1_view_flat_game_context_t * game_context);
extern void _mod_gl1_view_flat_game_context_clear_map (lw6sys_context_t * sys_context, mod_gl1_utils_context_t
						       * utils_context,
						       _mod_gl1_view_flat_context_t * flat_context, _mod_gl1_view_flat_game_context_map_t * game_context_map);
extern void
_mod_gl1_view_flat_game_context_clear_armies (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					      utils_context,
					      _mod_gl1_view_flat_context_t * flat_context, _mod_gl1_view_flat_game_context_armies_t * game_context_armies);
extern int _mod_gl1_view_flat_game_context_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						   utils_context,
						   _mod_gl1_view_flat_context_t
						   * flat_context,
						   _mod_gl1_view_flat_game_context_t
						   * game_context,
						   const lw6gui_look_t * look, const lw6map_level_t * level, const lw6ker_game_state_t * game_state);
extern int _mod_gl1_view_flat_game_context_update_map (lw6sys_context_t * sys_context, mod_gl1_utils_context_t
						       * utils_context,
						       _mod_gl1_view_flat_context_t
						       * flat_context,
						       _mod_gl1_view_flat_game_context_map_t
						       * game_context_map, const lw6gui_look_t * look, const lw6map_level_t * level);
extern int
_mod_gl1_view_flat_game_context_update_armies (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					       utils_context,
					       _mod_gl1_view_flat_context_t *
					       flat_context,
					       _mod_gl1_view_flat_game_context_armies_t
					       * game_context_armies, const lw6gui_look_t * look, const lw6ker_game_state_t * game_state);

/* gl1-flat-cursorcontext-setup.c */
extern _mod_gl1_view_flat_context_t *_mod_gl1_view_flat_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void _mod_gl1_view_flat_quit (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_view_flat_context_t * flat_context);

/* gl1-flat-cursorcontext-viewport.c */
extern void
_mod_gl1_view_flat_viewport_update_preview (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					    utils_context,
					    _mod_gl1_view_flat_context_t * flat_context, const lw6gui_look_t * look, const lw6map_level_t * level);
extern void _mod_gl1_view_flat_viewport_update (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						utils_context,
						_mod_gl1_view_flat_context_t *
						flat_context,
						const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors);

#endif // LIQUIDWAR6GFX_MOD_GL1_VIEW_FLAT_INTERNAL_H
