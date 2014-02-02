/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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

#include "../../mod-gl1.h"
#include "gl1-tactical-internal.h"

static void
display_frame (mod_gl1_utils_context_t * utils_context,
	       _mod_gl1_hud_tactical_context_t * tactical_context)
{
  mod_gl1_utils_set_render_mode_2d_blend (utils_context);

  mod_gl1_utils_bitmap_bind (utils_context,
			     tactical_context->bitmap_data.frame);
  mod_gl1_utils_bitmap_display (utils_context,
				tactical_context->bitmap_data.frame, 0.0f,
				0.0f,
				utils_context->sdl_context.video_mode.width,
				utils_context->sdl_context.video_mode.height);
}


void
_mod_gl1_hud_tactical_display_hud (mod_gl1_utils_context_t * utils_context,
				   _mod_gl1_hud_tactical_context_t *
				   tactical_context,
				   const lw6gui_look_t * look,
				   const lw6ker_game_state_t * game_state,
				   lw6pil_local_cursors_t * local_cursors)
{
  display_frame (utils_context, tactical_context);
}

void
mod_gl1_hud_tactical_display_hud (mod_gl1_utils_context_t * utils_context,
				  void *hud_context,
				  const lw6gui_look_t * look,
				  const lw6ker_game_state_t * game_state,
				  lw6pil_local_cursors_t * local_cursors)
{
  _mod_gl1_hud_tactical_display_hud (utils_context,
				     (_mod_gl1_hud_tactical_context_t *)
				     hud_context, look, game_state,
				     local_cursors);
}

void
_mod_gl1_hud_tactical_display_score (mod_gl1_utils_context_t * utils_context,
				     _mod_gl1_hud_tactical_context_t *
				     tactical_context,
				     const lw6gui_look_t * look,
				     const lw6ker_game_state_t * game_state,
				     lw6pil_local_cursors_t * local_cursors)
{
  //
}

void
mod_gl1_hud_tactical_display_score (mod_gl1_utils_context_t * utils_context,
				    void *hud_context,
				    const lw6gui_look_t * look,
				    const lw6ker_game_state_t * game_state,
				    lw6pil_local_cursors_t * local_cursors)
{
  _mod_gl1_hud_tactical_display_score (utils_context,
				       (_mod_gl1_hud_tactical_context_t *)
				       hud_context, look, game_state,
				       local_cursors);
}
