/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013  Christian Mauduit <ufoot@ufoot.org>

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

#ifndef LIQUIDWAR6GFX_MOD_CACA_INTERNAL_H
#define LIQUIDWAR6GFX_MOD_CACA_INTERNAL_H

#include "../gfx-internal.h"
#include "mod-caca.h"

#include <caca.h>

typedef struct _mod_caca_path_s
{
  char *data_dir;
  char *debug_dir;
}
_mod_caca_path_t;

typedef struct _mod_caca_const_data_s
{
  int event_timeout_microseconds;
  int canvas_create_width;
  int canvas_create_height;
  float quit_sleep;
  char *video_mode_default;
  char *video_mode_fallback;

  lw6sys_color_8_t bg_color;
  lw6sys_color_8_t fg_color;
  lw6sys_color_8_t team_color[10];

  /* KEYS */
  int keysym1_up;
  int keysym2_up;
  int keysym3_up;
  int keysym4_up;
  int keysym1_down;
  int keysym2_down;
  int keysym3_down;
  int keysym4_down;
  int keysym1_left;
  int keysym2_left;
  int keysym3_left;
  int keysym4_left;
  int keysym1_right;
  int keysym2_right;
  int keysym3_right;
  int keysym4_right;
  int keysym1_enter;
  int keysym2_enter;
  int keysym1_esc;
  int keysym2_esc;
  int keysym1_ctrl;
  int keysym2_ctrl;
  int keysym1_alt;
  int keysym2_alt;
  int keysym1_pgup;
  int keysym2_pgup;
  int keysym1_pgdown;
  int keysym2_pgdown;
  int keysym_quit;
}
_mod_caca_const_data_t;

typedef struct _mod_caca_context_s
{
  lw6gui_resize_callback_func_t resize_callback;
  lw6gui_video_mode_t video_mode;
  _mod_caca_path_t path;
  _mod_caca_const_data_t const_data;
  const char *version;
  const char **driver_list;
  caca_display_t *display;
  caca_canvas_t *canvas;
  lw6gui_input_t input;
}
_mod_caca_context_t;

/* mod-caca-const.c */
extern int _mod_caca_load_consts (_mod_caca_context_t * context);
extern void _mod_caca_unload_consts (_mod_caca_context_t * context);

/* mod-caca-game.c */
extern int _mod_caca_display_map(_mod_caca_context_t * caca_context, lw6gui_look_t * look,
				 lw6ker_game_state_t * game_state, lw6ker_game_struct_t * game_struct,
				 lw6pil_local_cursors_t * local_cursors);

/* mod-caca-menu.c */
extern int _mod_caca_display_menu(_mod_caca_context_t * caca_context, lw6gui_look_t * look,
				  lw6gui_menu_t * menu);

/* mod-caca-splash.c */
extern void plasma_anim(_mod_caca_context_t * caca_context);

/* mod-caca-utils.c */
extern int _mod_caca_display_hud(_mod_caca_context_t * caca_context, lw6gui_look_t * look,
				 lw6ker_game_state_t * game_state, lw6ker_game_struct_t * game_struct);
extern int _mod_caca_display_score(_mod_caca_context_t * caca_context, lw6gui_look_t * look,
				   lw6ker_game_state_t * game_state, lw6ker_game_struct_t * game_struct);

/* mod-caca-display.c */
extern int _mod_caca_display (_mod_caca_context_t * caca_context, int mask,
			      lw6gui_look_t * look, lw6map_level_t * level,
			      lw6ker_game_struct_t * game_struct,
			      lw6ker_game_state_t * game_state,
			      lw6pil_local_cursors_t * local_cursors,
			      lw6gui_menu_t * menu, float progress, float fps,
			      float mps, char **log_list, int capture,
			      int gfx_debug, int debug_team_id,
			      int debug_layer_id);

/* mod-caca-event.c */
extern lw6gui_input_t *_mod_caca_pump_events (_mod_caca_context_t *
					      caca_context);

/* mod-caca-mode.c */
extern int _mod_caca_set_video_mode (_mod_caca_context_t * caca_context,
				     lw6gui_video_mode_t * video_mode);
extern int _mod_caca_resize_video_mode (_mod_caca_context_t * caca_context,
					lw6gui_video_mode_t * video_mode);
extern int _mod_caca_get_video_mode (_mod_caca_context_t * caca_context,
				     lw6gui_video_mode_t * video_mode);
extern int _mod_caca_set_resize_callback (_mod_caca_context_t *
					  caca_context,
					  lw6gui_resize_callback_func_t
					  resize_callback);
extern void _mod_caca_call_resize_callback (_mod_caca_context_t *
					    caca_context);

/* mod-caca-path.c */
extern int _mod_caca_path_init (_mod_caca_path_t *
				path, int argc, const char *argv[]);
extern void _mod_caca_path_quit (_mod_caca_path_t * path);

/* mod-caca-repr.c */
extern char *_mod_caca_repr (_mod_caca_context_t * caca_context,
			     u_int32_t id);

/* mod-caca-resolution.c */
extern int _mod_caca_get_fullscreen_modes (_mod_caca_context_t *
					   caca_context,
					   lw6gui_fullscreen_modes_t * modes);
extern void _mod_caca_find_closest_resolution (_mod_caca_context_t *
					       caca_context,
					       int *closest_width,
					       int *closest_height,
					       int wished_width,
					       int wished_height);

/* mod-caca-setup.c */
extern _mod_caca_context_t *_mod_caca_init (int argc, const char *argv[],
					    lw6gui_video_mode_t *
					    video_mode,
					    lw6gui_resize_callback_func_t
					    resize_callback);
extern void _mod_caca_quit (_mod_caca_context_t * caca_context);

#endif // LIQUIDWAR6GFX_MOD_CACA_INTERNAL_H
