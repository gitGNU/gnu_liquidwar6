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

#ifndef LIQUIDWAR6GFX_H
#define LIQUIDWAR6GFX_H

#include "../sys/sys.h"
#include "../dyn/dyn.h"
#include "../map/map.h"
#include "../ker/ker.h"
#include "../pil/pil.h"
#include "../hlp/hlp.h"
#include "../cfg/cfg.h"
#include "../ldr/ldr.h"
#include "../gui/gui.h"
#include "../vox/vox.h"
#include "../cns/cns.h"

/**
 * The gfx backend is the first argument passed to any gfx function,
 * it contains reference to all the functions which can be used
 * as well as a pointer on associated data. In OO, this would just
 * be an object, with members and methods, using polymorphism through
 * opaque pointers.
 */
typedef struct lw6gfx_backend_s
{
  /// Handle on dynamic library (if it makes sense).
  lw6dyn_dl_handle_t *dl_handle;
  /**
   * Gfx specific data, what is behind this pointer really
   * depends on the gfx engine.
   */
  void *gfx_context;
  /// The argc value passed to main.
  int argc;
  /// The argv value passed to main.
  const char **argv;
  /// Lock used to avoid concurrent access to underlying libs.
  lw6sys_mutex_t *call_lock;
  /**
   * The id of the object, this is non-zero and unique within one run session,
   * incremented at each object creation.
   */
  u_int32_t id;

  /// Pointer on lw6gfx_init callback code.
  void *(*init) (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6gui_video_mode_t * video_mode,
		 lw6gui_resize_callback_func_t resize_callback);
  /// Pointer on lw6gfx_quit callback code.
  void (*quit) (lw6sys_context_t * sys_context, void *gfx_context);
  /// Pointer on lw6gfx_repr callback code.
  char *(*repr) (lw6sys_context_t * sys_context, void *gfx_context, u_int32_t id);
  /// Pointer on lw6gfx_set_video_mode callback code.
  int (*set_video_mode) (lw6sys_context_t * sys_context, void *gfx_context, lw6gui_video_mode_t * video_mode);
  /// Pointer on lw6gfx_get_video_mode callback code.
  int (*get_video_mode) (lw6sys_context_t * sys_context, void *gfx_context, lw6gui_video_mode_t * video_mode);
  /// Pointer on lw6gfx_get_fullscreen_modes callback code.
  int (*get_fullscreen_modes) (lw6sys_context_t * sys_context, void *gfx_context, lw6gui_fullscreen_modes_t * modes);
  /// Pointer on lw6gfx_pump_events code.
  lw6gui_input_t *(*pump_events) (lw6sys_context_t * sys_context, void *gfx_context);
  /// Pointer on lw6gfx_display code.
  int (*display) (lw6sys_context_t * sys_context, void *gfx_context, int mask, const lw6gui_look_t * look,
		  const lw6map_level_t * level,
		  const lw6ker_game_struct_t * game_struct,
		  const lw6ker_game_state_t * game_state,
		  lw6pil_local_cursors_t * local_cursors,
		  lw6gui_menu_t * menu,
		  float progress, float fps, float mps, const char **log_list, int capture, int gfx_debug, int debug_team_id, int debug_layer_id);
}
lw6gfx_backend_t;

/* gfx-api.c */
extern int lw6gfx_init (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend, lw6gui_video_mode_t * video_mode,
			lw6gui_resize_callback_func_t resize_callback);
extern void lw6gfx_quit (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend);
extern char *lw6gfx_repr (lw6sys_context_t * sys_context, const lw6gfx_backend_t * backend);
extern int lw6gfx_set_video_mode (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend, lw6gui_video_mode_t * video_mode);
extern int lw6gfx_get_video_mode (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend, lw6gui_video_mode_t * video_mode);
extern int lw6gfx_get_fullscreen_modes (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend, lw6gui_fullscreen_modes_t * modes);
extern lw6gui_input_t *lw6gfx_pump_events (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend);
extern int lw6gfx_display (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend, int mask,
			   const lw6gui_look_t * look,
			   const lw6map_level_t * level,
			   const lw6ker_game_struct_t * game_struct,
			   const lw6ker_game_state_t * game_state,
			   lw6pil_local_cursors_t * local_cursors,
			   lw6gui_menu_t * menu, float progress, float fps,
			   float mps, const char **log_list, int capture, int gfx_debug, int debug_team_id, int debug_layer_id);

/* gfx-register.c */
extern lw6sys_assoc_t *lw6gfx_get_backends (lw6sys_context_t * sys_context, int argc, const char *argv[]);
extern lw6gfx_backend_t *lw6gfx_create_backend (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *name);
extern void lw6gfx_destroy_backend (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend);

/* gfx-test.c */
extern int lw6gfx_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6gfx_test_run (lw6sys_context_t * sys_context, int mode);

#endif
