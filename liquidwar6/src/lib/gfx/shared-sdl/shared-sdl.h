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

#ifndef LIQUIDWAR6GFX_SHARED_SDL_H
#define LIQUIDWAR6GFX_SHARED_SDL_H

#include "../gfx-internal.h"

/* shared-sdl-backend.c */
extern void shared_sdl_is_GPL_compatible ();

/* shared-sdl-const.c */
extern int shared_sdl_load_consts (lw6sys_context_t * sys_context, _lw6gfx_sdl_context_t * sdl_context);
extern void shared_sdl_unload_consts (lw6sys_context_t * sys_context, _lw6gfx_sdl_context_t * sdl_context);

/* shared-sdl-event.c */
extern lw6gui_input_t *shared_sdl_pump_events (lw6sys_context_t * sys_context, _lw6gfx_sdl_context_t *
					       sdl_context, _lw6gfx_sdl_event_callback_t event_callback_func, void *event_callback_data);
/* shared-sdl-path.c */
extern int shared_sdl_path_init (lw6sys_context_t * sys_context, _lw6gfx_sdl_path_t * path, int argc, const char *argv[]);
extern void shared_sdl_path_quit (lw6sys_context_t * sys_context, _lw6gfx_sdl_path_t * path);

/* shared-sdl-timer.c */
extern void shared_sdl_timer_update (lw6sys_context_t * sys_context, _lw6gfx_sdl_timer_t * timer);
extern int64_t shared_sdl_timer_get_timestamp (lw6sys_context_t * sys_context, const _lw6gfx_sdl_timer_t * timer);
extern int64_t shared_sdl_timer_get_uptime (lw6sys_context_t * sys_context, const _lw6gfx_sdl_timer_t * timer);
extern int32_t shared_sdl_timer_get_cycle (lw6sys_context_t * sys_context, const _lw6gfx_sdl_timer_t * timer);

#endif // LIQUIDWAR6GFX_SHARED_SDL_H
