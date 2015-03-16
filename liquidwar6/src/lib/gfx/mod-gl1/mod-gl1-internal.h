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

#ifndef LIQUIDWAR6GFX_MOD_GL1_INTERNAL_H
#define LIQUIDWAR6GFX_MOD_GL1_INTERNAL_H

#include "gl1-utils/gl1-utils.h"
#include "gl1-splash/gl1-splash.h"
#include "gl1-background/gl1-background.h"
#include "gl1-hud/gl1-hud.h"
#include "gl1-menu/gl1-menu.h"
#include "gl1-view/gl1-view.h"

typedef struct _mod_gl1_context_s
{
  mod_gl1_utils_context_t utils_context;
#ifndef LW6_ALLINONE
  lw6dyn_dl_handle_t *shared_sdl_handle;
#else				// LW6_ALLINONE
  void *shared_sdl_handle;
#endif				// LW6_ALLINONE
  void *splash_context;
  mod_gl1_background_backend_t *background_backend;
  mod_gl1_hud_backend_t *hud_backend;
  mod_gl1_menu_backend_t *menu_backend;
  mod_gl1_view_backend_t *view_backend;
}
_mod_gl1_context_t;

/* mod-gl1-backend.c */
extern char *_mod_gl1_repr (lw6sys_context_t * sys_context, _mod_gl1_context_t * gl_context, u_int32_t id);

/* mod-gl1-setup.c */
extern _mod_gl1_context_t *_mod_gl1_init (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6gui_video_mode_t * video_mode,
					  lw6gui_resize_callback_func_t resize_callback);
extern void _mod_gl1_quit (lw6sys_context_t * sys_context, _mod_gl1_context_t * gl_context);
extern int _mod_gl1_set_backends (lw6sys_context_t * sys_context, _mod_gl1_context_t * gl_context, const lw6gui_look_t * look);
extern void _mod_gl1_clear_background_backend (lw6sys_context_t * sys_context, _mod_gl1_context_t * gl_context);
extern void _mod_gl1_clear_hud_backend (lw6sys_context_t * sys_context, _mod_gl1_context_t * gl_context);
extern void _mod_gl1_clear_menu_backend (lw6sys_context_t * sys_context, _mod_gl1_context_t * gl_context);
extern void _mod_gl1_clear_view_backend (lw6sys_context_t * sys_context, _mod_gl1_context_t * gl_context);
extern int _mod_gl1_set_background_backend (lw6sys_context_t * sys_context, _mod_gl1_context_t * gl_context, char *background_style);
extern int _mod_gl1_set_hud_backend (lw6sys_context_t * sys_context, _mod_gl1_context_t * gl_context, char *hud_style);
extern int _mod_gl1_set_menu_backend (lw6sys_context_t * sys_context, _mod_gl1_context_t * gl_context, char *menu_style);
extern int _mod_gl1_set_view_backend (lw6sys_context_t * sys_context, _mod_gl1_context_t * gl_context, char *view_style);

#endif
