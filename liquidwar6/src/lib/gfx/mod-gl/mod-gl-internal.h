/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

#ifndef LIQUIDWAR6GFX_MOD_GL_INTERNAL_H
#define LIQUIDWAR6GFX_MOD_GL_INTERNAL_H

#include "../gfx.h"
#include "gl-utils/gl-utils.h"
#include "gl-splash/gl-splash.h"
#include "gl-background/gl-background.h"
#include "gl-hud/gl-hud.h"
#include "gl-menu/gl-menu.h"
#include "gl-view/gl-view.h"

typedef struct _mod_gl_context_s
{
  mod_gl_utils_context_t utils_context;
  void *splash_context;
  mod_gl_background_backend_t *background_backend;
  mod_gl_hud_backend_t *hud_backend;
  mod_gl_menu_backend_t *menu_backend;
  mod_gl_view_backend_t *view_backend;
}
_mod_gl_context_t;

/*
 * In repr.c
 */
extern char *_mod_gl_repr (_mod_gl_context_t * gl_context, u_int32_t id);

/*
 * In setup.c
 */
extern _mod_gl_context_t *_mod_gl_init (int argc, char *argv[],
					lw6gui_video_mode_t * video_mode,
					lw6gui_resize_callback_func_t
					resize_callback);
extern void _mod_gl_quit (_mod_gl_context_t * gl_context);
extern int _mod_gl_set_backends (_mod_gl_context_t * gl_context,
				 lw6gui_look_t * look);
extern void _mod_gl_clear_background_backend (_mod_gl_context_t * gl_context);
extern void _mod_gl_clear_hud_backend (_mod_gl_context_t * gl_context);
extern void _mod_gl_clear_menu_backend (_mod_gl_context_t * gl_context);
extern void _mod_gl_clear_view_backend (_mod_gl_context_t * gl_context);
extern int _mod_gl_set_background_backend (_mod_gl_context_t * gl_context,
					   char *background_style);
extern int _mod_gl_set_hud_backend (_mod_gl_context_t * gl_context,
				    char *hud_style);
extern int _mod_gl_set_menu_backend (_mod_gl_context_t * gl_context,
				     char *menu_style);
extern int _mod_gl_set_view_backend (_mod_gl_context_t * gl_context,
				     char *view_style);

#endif
