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

typedef struct lw6gfx_backend_s
{
  lw6dyn_dl_handle_t *dl_handle;
  void *gfx_context;
  int argc;
  char **argv;
  u_int32_t id;

  void *(*init) (int argc, char *argv[], lw6gui_video_mode_t * video_mode,
		 lw6gui_resize_callback_func_t resize_callback);
  void (*quit) (void *gfx_context);
  char *(*repr) (void *gfx_context, u_int32_t id);
  int (*set_video_mode) (void *gfx_context, lw6gui_video_mode_t * video_mode);
  int (*get_video_mode) (void *gfx_context, lw6gui_video_mode_t * video_mode);
  int (*get_fullscreen_modes) (void *gfx_context,
			       lw6gui_fullscreen_modes_t * modes);
  lw6gui_input_t *(*pump_events) (void *gfx_context);
  int (*display) (void *gfx_context, int mask, lw6gui_look_t * look,
		  lw6map_level_t * level,
		  lw6ker_game_struct_t * game_struct,
		  lw6ker_game_state_t * game_state,
		  lw6pil_local_cursors_t * local_cursors,
		  lw6gui_menu_t * menu,
		  float progress,
		  float fps, float mps,
		  char **log_list, int capture, int gfx_debug,
		  int debug_team_id, int debug_layer_id);
}
lw6gfx_backend_t;

/*
 * In api.c
 */
extern int lw6gfx_init (lw6gfx_backend_t * backend,
			lw6gui_video_mode_t * video_mode,
			lw6gui_resize_callback_func_t resize_callback);
extern void lw6gfx_quit (lw6gfx_backend_t * backend);
extern char *lw6gfx_repr (lw6gfx_backend_t * backend);
extern int lw6gfx_set_video_mode (lw6gfx_backend_t * backend,
				  lw6gui_video_mode_t * video_mode);
extern int lw6gfx_get_video_mode (lw6gfx_backend_t * backend,
				  lw6gui_video_mode_t * video_mode);
extern int lw6gfx_get_fullscreen_modes (lw6gfx_backend_t * backend,
					lw6gui_fullscreen_modes_t * modes);
extern lw6gui_input_t *lw6gfx_pump_events (lw6gfx_backend_t * backend);
extern int lw6gfx_display (lw6gfx_backend_t * backend, int mask,
			   lw6gui_look_t * look, lw6map_level_t * level,
			   lw6ker_game_struct_t * game_struct,
			   lw6ker_game_state_t * game_state,
			   lw6pil_local_cursors_t * local_cursors,
			   lw6gui_menu_t * menu, float progress, float fps,
			   float mps, char **log_list, int capture,
			   int gfx_debug, int debug_team_id,
			   int debug_layer_id);

/*
 * In register.c
 */
extern lw6sys_assoc_t *lw6gfx_get_backends (int argc, char *argv[]);
extern lw6gfx_backend_t *lw6gfx_create_backend (int argc, char *argv[],
						char *name);
extern void lw6gfx_destroy_backend (lw6gfx_backend_t * backend);

/* gfx-test.c */
extern int lw6gfx_test (int mode);

#endif
