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

#ifndef LIQUIDWAR6GFX_MOD_GL1_VIEW_FLAT_H
#define LIQUIDWAR6GFX_MOD_GL1_VIEW_FLAT_H

#include "../../../gfx.h"
#include "../../../../map/map.h"
#include "../../gl1-utils/gl1-utils.h"
#include "../gl1-view.h"

/* gl1-flat-backend.c */
extern mod_gl1_view_backend_t *mod_gl1_view_flat_create_backend (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);

/* gl1-flat-display.c */
extern void mod_gl1_view_flat_display_preview (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					       utils_context, void *flat_context, const lw6gui_look_t * look, const lw6map_level_t * level);
extern void mod_gl1_view_flat_display_map (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					   utils_context,
					   void *flat_context,
					   const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors);
extern void mod_gl1_view_flat_display_fighters (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
						utils_context,
						void *flat_context,
						const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors);
extern void mod_gl1_view_flat_display_cursors (lw6sys_context_t * sys_context, mod_gl1_utils_context_t *
					       utils_context,
					       void *flat_context,
					       const lw6gui_look_t * look, const lw6ker_game_state_t * game_state, lw6pil_local_cursors_t * local_cursors);

/* gl1-flat-setup.c */
extern void *mod_gl1_view_flat_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context);
extern void mod_gl1_view_flat_quit (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *flat_context);

#endif
