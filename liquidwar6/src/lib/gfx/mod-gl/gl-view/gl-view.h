/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

#ifndef LIQUIDWAR6GFX_MOD_GL_VIEW_H
#define LIQUIDWAR6GFX_MOD_GL_VIEW_H

#include <ltdl.h>

#include "../../../sys/sys.h"
#include "../../../map/map.h"
#include "gl-view.h"
#include "../mod-gl.h"
#include "../gl-utils/gl-utils.h"

typedef struct mod_gl_view_backend_s
{
  void *view_context;
  char *backend_name;

  void *(*init) (mod_gl_utils_context_t * utils_context);
  void (*quit) (mod_gl_utils_context_t * utils_context, void *view_context);
  void (*display_preview) (mod_gl_utils_context_t * utils_context,
			   void *view_context, lw6gui_look_t * look,
			   lw6map_level_t * level);
  void (*display_map) (mod_gl_utils_context_t * utils_context,
		       void *view_context, lw6gui_look_t * look,
		       lw6ker_game_state_t * game_state,
		       lw6pil_local_cursors_t * local_cursors);
  void (*display_fighters) (mod_gl_utils_context_t * utils_context,
			    void *view_context, lw6gui_look_t * look,
			    lw6ker_game_state_t * game_state,
			    lw6pil_local_cursors_t * local_cursors);
  void (*display_cursors) (mod_gl_utils_context_t * utils_context,
			   void *view_context, lw6gui_look_t * look,
			   lw6ker_game_state_t * game_state,
			   lw6pil_local_cursors_t * local_cursors);
}
mod_gl_view_backend_t;

/*
 * In api.c
 */
extern int mod_gl_view_init (mod_gl_utils_context_t * utils_context,
			     mod_gl_view_backend_t * backend);
extern void mod_gl_view_quit (mod_gl_utils_context_t * utils_context,
			      mod_gl_view_backend_t * backend);
extern void mod_gl_view_display_preview (mod_gl_utils_context_t *
					 utils_context,
					 mod_gl_view_backend_t * backend,
					 lw6gui_look_t * look,
					 lw6map_level_t * level);
extern void mod_gl_view_display_map (mod_gl_utils_context_t * utils_context,
				     mod_gl_view_backend_t * backend,
				     lw6gui_look_t * look,
				     lw6ker_game_state_t * game_state,
				     lw6pil_local_cursors_t * local_cursors);
extern void mod_gl_view_display_fighters (mod_gl_utils_context_t *
					  utils_context,
					  mod_gl_view_backend_t * backend,
					  lw6gui_look_t * look,
					  lw6ker_game_state_t * game_state,
					  lw6pil_local_cursors_t *
					  local_cursors);
extern void mod_gl_view_display_cursors (mod_gl_utils_context_t *
					 utils_context,
					 mod_gl_view_backend_t * backend,
					 lw6gui_look_t * look,
					 lw6ker_game_state_t * game_state,
					 lw6pil_local_cursors_t *
					 local_cursors);

/*
 * In backend.c
 */
extern mod_gl_view_backend_t
  * mod_gl_view_create_backend (mod_gl_utils_context_t * utils_context,
				char *name);
extern void mod_gl_view_destroy_backend (mod_gl_utils_context_t *
					 utils_context,
					 mod_gl_view_backend_t * backend);

#endif
