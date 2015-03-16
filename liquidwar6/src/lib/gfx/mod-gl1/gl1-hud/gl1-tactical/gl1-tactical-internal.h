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

#ifndef LIQUIDWAR6GFX_MOD_GL1_HUD_TACTICAL_INTERNAL_H
#define LIQUIDWAR6GFX_MOD_GL1_HUD_TACTICAL_INTERNAL_H

#include "gl1-tactical.h"

typedef struct _mod_gl1_hud_tactical_bitmap_data_s
{
  mod_gl1_utils_bitmap_t *frame;
}
_mod_gl1_hud_tactical_bitmap_data_t;

typedef struct _mod_gl1_hud_tactical_const_data_s
{
  float viewport_x;
  float viewport_y;
  float viewport_w;
  float viewport_h;
}
_mod_gl1_hud_tactical_const_data_t;

typedef struct _mod_gl1_hud_tactical_context_s
{
  _mod_gl1_hud_tactical_bitmap_data_t bitmap_data;
  _mod_gl1_hud_tactical_const_data_t const_data;
}
_mod_gl1_hud_tactical_context_t;

/* gl1-tactical-display.c */
extern void _mod_gl1_hud_tactical_display_hud (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					       utils_context,
					       _mod_gl1_hud_tactical_context_t
					       * tactical_context,
					       const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors);
extern void _mod_gl1_hud_tactical_display_score (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						 utils_context,
						 _mod_gl1_hud_tactical_context_t
						 * tactical_context,
						 const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors);

/* gl1-tactical-data.c */
extern int _mod_gl1_hud_tactical_load_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					    _mod_gl1_hud_tactical_context_t * tactical_context);
extern void _mod_gl1_hud_tactical_unload_data (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					       _mod_gl1_hud_tactical_context_t * tactical_context);

/* gl1-tactical-setup.c */
extern _mod_gl1_hud_tactical_context_t *_mod_gl1_hud_tactical_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void _mod_gl1_hud_tactical_quit (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
					_mod_gl1_hud_tactical_context_t * tactical_context);

#endif
