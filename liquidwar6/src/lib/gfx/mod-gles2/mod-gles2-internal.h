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

#ifndef LIQUIDWAR6GFX_MOD_GLES2_INTERNAL_H
#define LIQUIDWAR6GFX_MOD_GLES2_INTERNAL_H

#include "../gfx.h"
#include "mod-gles2.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

typedef struct _mod_gles2_path_s
{
  char *data_dir;
  char *debug_dir;
}
_mod_gles2_path_t;

typedef struct _mod_gles2_context_s
{
  /*
   * One of the major flaws of the initial opengl1 backend
   * is that it uses per-backend data storage, and each backend
   * tries to handle its data on-the-fly as it's called.
   * There's nothing wrong with that but it makes it really
   * hard to optimize things as in OpenGL, all those backends
   * do share the same physical ressources, and to be efficient,
   * one needs to somehow manage this at a higher level. You
   * cannot allocate ressources on the fly while keeping
   * really efficient. One needs to plan everything from start,
   * at least at the beginning of every frame draw but ideally
   * even before and/or on much more rare occasions, this is
   * why the opengles2 implementation does have much more
   * common shared stuff, which appear together within this
   * context structure.
   */
  lw6gui_video_mode_t video_mode;
  _mod_gles2_path_t path;
}
_mod_gles2_context_t;

/* mod-gles2-path.c */
extern int _mod_gles2_path_init (_mod_gles2_path_t *
				 path, int argc, const char *argv[]);
extern void _mod_gles2_path_quit (_mod_gles2_path_t * path);

/* mod-gles2-repr.c */
extern char *_mod_gles2_repr (_mod_gles2_context_t * gl_context,
			      u_int32_t id);

/* mod-gles2-setup.c */
extern _mod_gles2_context_t *_mod_gles2_init (int argc, const char *argv[],
					      lw6gui_video_mode_t *
					      video_mode,
					      lw6gui_resize_callback_func_t
					      resize_callback);
extern void _mod_gles2_quit (_mod_gles2_context_t * gl_context);

#endif
