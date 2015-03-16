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

/**
 * Miscellaneous parameters for display. One of the idea with
 * this structure is that one must be able to compare two
 * of them with a simple memcmp. Even the progress pointer can
 * be compared this way, the rest is just plain data.
 */
typedef struct lw6dsp_misc_s
{
  /// Display mask, define what must be displayed.
  int mask;
  /// How many frames per second the engine needs to display.
  int target_fps;
  /**
   * How much CPU need be used, the maximum is 1.0f, the idea
   * behind this parameter is that if you set it to a low value,
   * the display thread will yield timeslices letting other threads
   * speed up. On a single-core CPU this can be usefull if the
   * main calculation thread, the one that updates the game state,
   * is falling behind.
   */
  float gfx_cpu_usage;
  /// Dirty read mode.
  int dirty_read;
  /// Wether we want to capture screen and dump it on disk.
  int capture;
  /// Wether to enable GFX debug mode.
  int gfx_debug;
  /// Parameter for debug mode, team ID to show info about.
  int debug_team_id;
  /// Parameter for debug mode, layer ID to show info about.
  int debug_layer_id;
  /// Repeat settings, used by the input module.
  lw6gui_repeat_settings_t repeat_settings;
  /// Delay after which messages disappear, in milliseconds.
  int log_timeout;
  /// Pointer on progress float, provides feedback to user.
  volatile float *progress;
} lw6dsp_misc_t;

/**
 * Parameters used by the display thread. Be carefull, those
 * need be modify with adequate functions, else a (serious)
 * race condition could occur.
 */
typedef struct lw6dsp_param_s
{
  /// Miscellaneous parameters, things that didn't fit elsewhere.
  lw6dsp_misc_t misc;
  /**
   * Video mode. This is separated from the misc parameters for
   * it requires special handling, indeed changing resolution
   * might need calls to special functions to set up a new
   * graphical context.
   */
  lw6gui_video_mode_t video_mode;
  /// Visual parameters.
  lw6gui_look_t *look;
  /**
   * Menu object, the main interface to gather and transmit
   * informations to the user, when not playing.
   */
  lw6gui_menu_t *menu;
  /// The level, as loaded from disk.
  lw6map_level_t *level;
  /**
   * The game struct, that is the level post-processed to be
   * usable by game algorithms.
   */
  lw6ker_game_struct_t *game_struct;
  /**
   * Changeable state of the game. Not that this pointer
   * must really always be available, you can technically
   * change its value but any value passed to it must be
   * available and valid as long as the display is running,
   * else you end up with a good old segfault.
   */
  lw6ker_game_state_t *game_state;
  /**
   * If in dirty-read mode, then game_state will be fetched
   * from this object. It can be NULL if you don't want to
   * use the pilot.
   */
  lw6pil_pilot_t *pilot;
} lw6dsp_param_t;

/**
 * The dsp backend is the first argument passed to any dsp function,
 * it contains reference to all the functions which can be used
 * as well as a pointer on associated data. In OO, this would just
 * be an object, with members and methods, using polymorphism through
 * opaque pointers.
 */
typedef struct lw6dsp_backend_s
{
   /**
   * The id of the object, this is non-zero and unique within one run session,
   * incremented at each object creation.
   */
  u_int32_t id;
  /**
   * Thread running the display code. In fact running in a
   * separate thread is the whole point of this display object.
   */
  lw6sys_thread_handler_t *thread;
  /**
   * Data used by the display code. This is an opaque pointer, the
   * internal structure will store details such as mutexes, parameters,
   * information about the graphics backend, and so on.
   */
  void *data;
  /**
   * Input structure, will be updated by the display thread, so
   * that it contains up-to-date information. This is actually the
   * only way to get input from the user.
   */
  lw6gui_input_t *input;
} lw6dsp_backend_t;

/* dsp-display.c */
extern lw6dsp_backend_t *lw6dsp_create_backend (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *gfx_backend_name);
extern void lw6dsp_destroy_backend (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend);
extern char *lw6dsp_repr (lw6sys_context_t * sys_context, const lw6dsp_backend_t * dsp_backend);
extern int lw6dsp_init (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend, const lw6dsp_param_t * param,
			lw6gui_resize_callback_func_t resize_callback);
extern void lw6dsp_quit (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend);
extern int lw6dsp_update (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend, const lw6dsp_param_t * param);
extern int lw6dsp_get_nb_frames (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend);
extern int lw6dsp_get_last_frame_rendering_time (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend);
extern int lw6dsp_get_instant_fps (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend);
extern int lw6dsp_get_average_fps (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend);
extern int lw6dsp_get_video_mode (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend, lw6gui_video_mode_t * video_mode);
extern int lw6dsp_get_fullscreen_modes (lw6sys_context_t * sys_context, lw6dsp_backend_t * dsp_backend, lw6gui_fullscreen_modes_t * fullscreen_modes);

/* dsp-param.c */
extern void lw6dsp_param_zero (lw6sys_context_t * sys_context, lw6dsp_param_t * param);

/* dsp-test.c */
extern int lw6dsp_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6dsp_test_run (lw6sys_context_t * sys_context, int mode);

#endif
