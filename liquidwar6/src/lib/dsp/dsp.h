
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

#ifndef LIQUIDWAR6DSP_H
#define LIQUIDWAR6DSP_H

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
#include "../gfx/gfx.h"

typedef struct lw6dsp_misc_s
{
  int mask;
  int target_fps;
  float gfx_cpu_usage;
  int dirty_read;
  int capture;
  int gfx_debug;
  int debug_team_id;
  int debug_layer_id;
  lw6gui_repeat_settings_t repeat_settings;
  int log_timeout;
  volatile float *progress;
} lw6dsp_misc_t;

typedef struct lw6dsp_param_s
{
  lw6dsp_misc_t misc;
  lw6gui_video_mode_t video_mode;
  lw6gui_look_t *look;
  lw6gui_menu_t *menu;
  lw6map_level_t *level;
  lw6ker_game_struct_t *game_struct;
  lw6ker_game_state_t *game_state;
  lw6pil_pilot_t *pilot;
} lw6dsp_param_t;

typedef struct lw6dsp_backend_s
{
  u_int32_t id;
  void *thread;
  void *data;
  lw6gui_input_t *input;
} lw6dsp_backend_t;

/* dsp-display.c */
extern lw6dsp_backend_t *lw6dsp_create_backend (int argc, const char *argv[],
						const char *gfx_backend_name);
extern void lw6dsp_destroy_backend (lw6dsp_backend_t * dsp_backend);
extern char *lw6dsp_repr (lw6dsp_backend_t * dsp_backend);
extern int lw6dsp_init (lw6dsp_backend_t * dsp_backend,
			lw6dsp_param_t * param,
			lw6gui_resize_callback_func_t resize_callback);
extern void lw6dsp_quit (lw6dsp_backend_t * dsp_backend);
extern int lw6dsp_update (lw6dsp_backend_t * dsp_backend,
			  lw6dsp_param_t * param);
extern int lw6dsp_get_nb_frames (lw6dsp_backend_t * dsp_backend);
extern int lw6dsp_get_last_frame_rendering_time (lw6dsp_backend_t *
						 dsp_backend);
extern int lw6dsp_get_instant_fps (lw6dsp_backend_t * dsp_backend);
extern int lw6dsp_get_average_fps (lw6dsp_backend_t * dsp_backend);
extern int lw6dsp_get_video_mode (lw6dsp_backend_t * dsp_backend,
				  lw6gui_video_mode_t * video_mode);
extern int lw6dsp_get_fullscreen_modes (lw6dsp_backend_t * dsp_backend,
					lw6gui_fullscreen_modes_t *
					fullscreen_modes);

/* dsp-param.c */
extern void lw6dsp_param_zero (lw6dsp_param_t * param);

/* dsp-test.c */
extern int lw6dsp_test (int mode);

#endif
