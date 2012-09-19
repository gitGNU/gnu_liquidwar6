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

#ifndef LIQUIDWAR6GFX_SHARED_SDL_H
#define LIQUIDWAR6GFX_SHARED_SDL_H

#include "../gfx.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

typedef struct shared_sdl_path_s
{
  char *data_dir;
  char *debug_dir;
}
shared_sdl_path_t;

typedef struct shared_sdl_const_data_s
{
  float warp_x;
  float warp_y;
  float mode_sleep;
  float quit_sleep;
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
  int joystick_limit;
  int joystick1_index;
  int joystick1_button_a_index;
  int joystick1_button_b_index;
  int joystick1_button_c_index;
  int joystick1_button_d_index;
  int joystick1_button_e_index;
  int joystick1_button_f_index;
  int joystick2_index;
  int joystick2_button_a_index;
  int joystick2_button_b_index;
  int joystick2_button_c_index;
  int joystick2_button_d_index;
  int joystick2_button_e_index;
  int joystick2_button_f_index;
  int joystick_all_buttons;
}
shared_sdl_const_data_t;

typedef struct shared_sdl_timer_s
{
  int64_t timestamp;
  int64_t uptime;
  int32_t cycle;
}
shared_sdl_timer_t;

typedef struct shared_sdl_context_s
{
  shared_sdl_path_t path;
  lw6gui_resize_callback_func_t resize_callback;
  lw6gui_video_mode_t video_mode;
  lw6gui_zone_t viewport;
  shared_sdl_const_data_t const_data;
  lw6gui_input_t input;
  shared_sdl_timer_t timer;
}
shared_sdl_context_t;

/* shared-sdl-const.c */
extern int shared_sdl_load_consts (shared_sdl_context_t * context);
extern void shared_sdl_unload_consts (shared_sdl_context_t * context);

/* shared-sdl-event.c */
extern lw6gui_input_t *shared_sdl_pump_events (shared_sdl_context_t *
					       sdl_context);
/* shared-sdl-backend.c */
extern void shared_sdl_is_GPL_compatible ();

/* shared-sdl-path.c */
extern int shared_sdl_path_init (shared_sdl_path_t *
				 path, int argc, const char *argv[]);
extern void shared_sdl_path_quit (shared_sdl_path_t * path);

/* shared-sdl-timer.c */
extern void shared_sdl_timer_update (shared_sdl_context_t * soft_context);
extern int64_t shared_sdl_timer_get_timestamp (shared_sdl_context_t *
					       soft_context);
extern int64_t shared_sdl_timer_get_uptime (shared_sdl_context_t *
					    soft_context);
extern int32_t shared_sdl_timer_get_cycle (shared_sdl_context_t *
					   soft_context);

#endif // LIQUIDWAR6GFX_SHARED_SDL_H
