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

#ifndef LIQUIDWAR6DSP_INTERNAL_H
#define LIQUIDWAR6DSP_INTERNAL_H

#include "dsp.h"

#define _LW6DSP_PARAM_DIFF_MISC 0x00000001
#define _LW6DSP_PARAM_DIFF_VIDEO_MODE 0x00000002
#define _LW6DSP_PARAM_DIFF_LOOK 0x00000004
#define _LW6DSP_PARAM_DIFF_MENU 0x00000008
#define _LW6DSP_PARAM_DIFF_LEVEL 0x00000010
#define _LW6DSP_PARAM_DIFF_GAME_STRUCT 0x00000020
#define _LW6DSP_PARAM_DIFF_GAME_STATE 0x00000040
#define _LW6DSP_PARAM_DIFF_PILOT 0x00000080

#define _LW6DSP_IDLE_LIMIT 100

typedef struct _lw6dsp_data_s
{
  int argc;
  const char **argv;
  char *gfx_backend_name;
  lw6gfx_backend_t *gfx_backend;
  lw6gui_video_mode_t video_mode_requested;
  lw6gui_video_mode_t video_mode_obtained;
  lw6gui_fullscreen_modes_t fullscreen_modes;
  lw6sys_mutex_t *render_mutex;
  int run;
  int started;
  int failed;
  lw6gui_resize_callback_func_t resize_callback;
  int64_t start_ticks;
  int rounds;
  int nb_frames;
  int nb_frames_at_last_update;
  int last_frame_rendering_time;
  float instant_fps;
  float average_fps;
  int slow_fps;
  float mps;
  int slow_mps;
  u_int32_t last_menu_id;
  u_int32_t last_level_id;
  u_int32_t last_game_struct_id;
  u_int32_t last_game_state_id;
  lw6dsp_param_t param;
  lw6gui_input_t *input;
  lw6pil_local_cursors_t local_cursors;
#ifdef LW6_MAC_OS_X
  void *macosx_vthread_handler;
  void *macosx_thread_handler;
#endif
} _lw6dsp_data_t;

/* dsp-data.c */
extern _lw6dsp_data_t *_lw6dsp_data_new (lw6sys_context_t * sys_context);
extern void _lw6dsp_data_clear (lw6sys_context_t * sys_context, _lw6dsp_data_t * data);
extern void _lw6dsp_data_free (lw6sys_context_t * sys_context, _lw6dsp_data_t * data);

/* dsp-macosx.m */
extern void *_lw6dsp_macosx_thread_init (lw6sys_context_t * sys_context);
extern void _lw6dsp_macosx_thread_quit (lw6sys_context_t * sys_context, void *ptr);

/* dsp-param.c */
extern void _lw6dsp_param_clear (lw6sys_context_t * sys_context, lw6dsp_param_t * param);
extern int _lw6dsp_param_diff (lw6sys_context_t * sys_context, const _lw6dsp_data_t * data, const lw6dsp_param_t * param);

/* dsp-thread.c */
extern void _lw6dsp_thread_func (lw6sys_context_t * sys_context, _lw6dsp_data_t * data);
extern void _lw6dsp_thread_join (lw6sys_context_t * sys_context, _lw6dsp_data_t * data);

#endif
