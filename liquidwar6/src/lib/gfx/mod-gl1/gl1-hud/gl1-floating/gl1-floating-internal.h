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

#ifndef LIQUIDWAR6GFX_MOD_GL1_HUD_FLOATING_INTERNAL_H
#define LIQUIDWAR6GFX_MOD_GL1_HUD_FLOATING_INTERNAL_H

#include "gl1-floating.h"

typedef struct _mod_gl1_hud_floating_const_data_s
{
  float clock_y1;
  float clock_x2;
  float clock_size;
  float clock_dw;
  float clock_dh;
  float gauge_relative_bg_inner;
  float gauge_relative_bg_outer;
  float gauge_relative_fg_inner;
  float gauge_relative_fg_outer;
  float gauge_relative_text_size;
  float gauge_relative_text_dw;
  float gauge_relative_text_dh;
  float gauge_relative_percent_only_y1;
  float gauge_relative_percent_y1;
  float gauge_relative_frags_y1;
  int gauge_heartbeat_period;
  float gauge_relative_heartbeat_min;
  float gauge_relative_heartbeat_max;
  int gauge_slices;
  int gauge_loops;
  float gauge_start;
  float gauge_sweep;
  float gauge_min_size;
  float gauge_max_size;
  float gauge_max_health_angle;
  float weapon_ambiance_alpha1;
  float weapon_ambiance_alpha2;
  int score_pie_rotation_period;
  int score_pie_heartbeat_period;
  float score_pie_heartbeat_factor1;
  float score_pie_heartbeat_factor2;
  int score_pie_slices;
  int score_pie_loops;
  float score_pie_inner;
  float score_pie_outer;
  float score_pie_offset;
  float score_pie_text_radius_min;
  float score_pie_text_radius_max;
  float score_pie_text_size;
  float score_pie_text_dw;
  float score_pie_text_dh;
}
_mod_gl1_hud_floating_const_data_t;

typedef struct _mod_gl1_hud_floating_clock_s
{
  mod_gl1_utils_shaded_text_t *clock_text;
} _mod_gl1_hud_floating_clock_t;

typedef struct _mod_gl1_hud_floating_gauges_s
{
  mod_gl1_utils_bitmap_t *gauge_frame;
  mod_gl1_utils_shaded_text_t *percent_texts[LW6MAP_NB_TEAM_COLORS];
  mod_gl1_utils_shaded_text_t *frags_texts[LW6MAP_NB_TEAM_COLORS];
  GLUquadricObj *disk;
}
_mod_gl1_hud_floating_gauges_t;

typedef struct _mod_gl1_hud_floating_score_pie_s
{
  mod_gl1_utils_shaded_text_t *score_texts[LW6MAP_NB_TEAM_COLORS];
  GLUquadricObj *disk;
}
_mod_gl1_hud_floating_score_pie_t;

typedef struct _mod_gl1_hud_floating_context_s
{
  _mod_gl1_hud_floating_const_data_t const_data;
  const lw6gui_look_t *look;
  const lw6ker_game_state_t *game_state;
  lw6pil_local_cursors_t *local_cursors;
  lw6ker_score_array_t score_array;
  _mod_gl1_hud_floating_clock_t clock;
  _mod_gl1_hud_floating_gauges_t gauges;
  _mod_gl1_hud_floating_score_pie_t score_pie;
}
_mod_gl1_hud_floating_context_t;

/* gl1-floating-display.c */
extern void _mod_gl1_hud_floating_display_hud (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					       utils_context,
					       _mod_gl1_hud_floating_context_t
					       * floating_context,
					       const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors);
extern void _mod_gl1_hud_floating_display_score (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						 utils_context,
						 _mod_gl1_hud_floating_context_t
						 * floating_context,
						 const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors);

/*gl1-floating-context.c */
extern int _mod_gl1_hud_floating_context_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					       _mod_gl1_hud_floating_context_t * floating_context);
extern int _mod_gl1_hud_floating_context_begin_hud (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
						    _mod_gl1_hud_floating_context_t * floating_context);
extern void _mod_gl1_hud_floating_context_end_hud (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
						   _mod_gl1_hud_floating_context_t * floating_context);
extern int _mod_gl1_hud_floating_context_update_hud (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
						     _mod_gl1_hud_floating_context_t * floating_context, const lw6gui_look_t * look,
						     const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors);
extern int _mod_gl1_hud_floating_context_begin_score (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
						      _mod_gl1_hud_floating_context_t * floating_context);
extern void _mod_gl1_hud_floating_context_end_score (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
						     _mod_gl1_hud_floating_context_t * floating_context);
extern int _mod_gl1_hud_floating_context_update_score (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
						       _mod_gl1_hud_floating_context_t * floating_context, const lw6gui_look_t * look,
						       const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors);
extern int _mod_gl1_hud_floating_context_clear_hud_clock (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
							  _mod_gl1_hud_floating_context_t * floating_context);
extern int _mod_gl1_hud_floating_context_clear_hud_gauges (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
							   _mod_gl1_hud_floating_context_t * floating_context);
extern int _mod_gl1_hud_floating_context_clear_hud (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
						    _mod_gl1_hud_floating_context_t * floating_context);
extern int _mod_gl1_hud_floating_context_clear_score (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
						      _mod_gl1_hud_floating_context_t * floating_context);
extern int _mod_gl1_hud_floating_context_clear (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
						_mod_gl1_hud_floating_context_t * floating_context);

/* gl1-floating-data.c */
extern int _mod_gl1_hud_floating_load_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					    _mod_gl1_hud_floating_context_t * floating_context);
extern void _mod_gl1_hud_floating_unload_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					       _mod_gl1_hud_floating_context_t * floating_context);

/* gl1-floating-setup.c */
extern _mod_gl1_hud_floating_context_t *_mod_gl1_hud_floating_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void _mod_gl1_hud_floating_quit (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					_mod_gl1_hud_floating_context_t * floating_context);

#endif
