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

#ifndef LIQUIDWAR6GFX_MOD_SOFT_INTERNAL_H
#define LIQUIDWAR6GFX_MOD_SOFT_INTERNAL_H

#include "../shared-sdl/shared-sdl.h"
#include "mod-soft.h"

typedef struct _mod_soft_path_s
{
  char *data_dir;
  char *debug_dir;
}
_mod_soft_path_t;

typedef struct _mod_soft_caps_s
{
  int bpp;
}
_mod_soft_caps_t;

typedef struct _mod_soft_const_data_s
{
  int dummy;
}
_mod_soft_const_data_t;

typedef struct _mod_soft_timer_s
{
  int64_t timestamp;
  int64_t uptime;
  int32_t cycle;
}
_mod_soft_timer_t;

typedef struct _mod_soft_context_s
{
  shared_sdl_context_t shared_sdl_context;
  /*
   * One of the major flaws of the initial opensoft backend
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
   * why the opensoft implementation does have much more
   * common shared stuff, which appear together within this
   * context structure.
   */
  _mod_soft_caps_t caps;
  _mod_soft_path_t path;
  _mod_soft_const_data_t const_data;
#ifndef LW6_ALLINONE
  lw6dyn_dl_handle_t *shared_sdl_handle;
#endif
}
_mod_soft_context_t;

/* mod-soft-const.c */
extern int _mod_soft_load_consts (_mod_soft_context_t * context);
extern void _mod_soft_unload_consts (_mod_soft_context_t * context);

/* mod-soft-mode.c */
extern int _mod_soft_set_video_mode (_mod_soft_context_t * soft_context,
				     lw6gui_video_mode_t * video_mode);
extern int _mod_soft_resize_video_mode (_mod_soft_context_t * soft_context,
					lw6gui_video_mode_t * video_mode);
extern int _mod_soft_get_video_mode (_mod_soft_context_t * soft_context,
				     lw6gui_video_mode_t * video_mode);
extern int _mod_soft_sync_viewport (_mod_soft_context_t * soft_context);
extern int _mod_soft_sync_mode (_mod_soft_context_t * soft_context,
				int force);
extern int _mod_soft_set_resize_callback (_mod_soft_context_t *
					  soft_context,
					  lw6gui_resize_callback_func_t
					  resize_callback);
extern void _mod_soft_call_resize_callback (_mod_soft_context_t *
					    soft_context);

/* mod-soft-path.c */
extern int _mod_soft_path_init (_mod_soft_path_t *
				path, int argc, const char *argv[]);
extern void _mod_soft_path_quit (_mod_soft_path_t * path);

/* mod-soft-repr.c */
extern char *_mod_soft_repr (_mod_soft_context_t * soft_context,
			     u_int32_t id);

/* mod-soft-resolution.c */
extern int _mod_soft_get_fullscreen_modes (_mod_soft_context_t *
					   soft_context,
					   lw6gui_fullscreen_modes_t * modes);
extern void _mod_soft_find_closest_resolution (_mod_soft_context_t *
					       soft_context,
					       int *closest_width,
					       int *closest_height,
					       int wished_width,
					       int wished_height);

/* mod-soft-setup.c */
extern _mod_soft_context_t *_mod_soft_init (int argc, const char *argv[],
					    lw6gui_video_mode_t *
					    video_mode,
					    lw6gui_resize_callback_func_t
					    resize_callback);
extern void _mod_soft_quit (_mod_soft_context_t * soft_context);

/* mod-soft-viewport.c */
extern void _mod_soft_viewport_drawable_max (_mod_soft_context_t *
					     soft_context);

#endif // LIQUIDWAR6GFX_MOD_SOFT_INTERNAL_H
