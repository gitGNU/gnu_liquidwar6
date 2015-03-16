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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gfx-internal.h"
#include "shared-sdl/shared-sdl.h"

int
_lw6gfx_sdl_bind_funcs (lw6sys_context_t * sys_context, _lw6gfx_sdl_funcs_t * funcs, void *handle)
{
  int ret = 0;

#ifdef LW6_ALLINONE
  funcs->load_consts = shared_sdl_load_consts;
  funcs->unload_consts = shared_sdl_unload_consts;
  funcs->pump_events = shared_sdl_pump_events;
  funcs->path_init = shared_sdl_path_init;
  funcs->path_quit = shared_sdl_path_quit;
  funcs->timer_update = shared_sdl_timer_update;
  funcs->timer_get_timestamp = shared_sdl_timer_get_timestamp;
  funcs->timer_get_uptime = shared_sdl_timer_get_uptime;
  funcs->timer_get_cycle = shared_sdl_timer_get_cycle;

  ret = 1;
#else // LW6_ALLINONE
#if  MOD_GL1 || MOD_GLES2 || MOD_SOFT
  funcs->load_consts = lw6dyn_dlsym (sys_context, handle, "shared_sdl_load_consts");
  funcs->unload_consts = lw6dyn_dlsym (sys_context, handle, "shared_sdl_unload_consts");
  funcs->pump_events = lw6dyn_dlsym (sys_context, handle, "shared_sdl_pump_events");
  funcs->path_init = lw6dyn_dlsym (sys_context, handle, "shared_sdl_path_init");
  funcs->path_quit = lw6dyn_dlsym (sys_context, handle, "shared_sdl_path_quit");
  funcs->timer_update = lw6dyn_dlsym (sys_context, handle, "shared_sdl_timer_update");
  funcs->timer_get_timestamp = lw6dyn_dlsym (sys_context, handle, "shared_sdl_timer_get_timestamp");
  funcs->timer_get_uptime = lw6dyn_dlsym (sys_context, handle, "shared_sdl_timer_get_uptime");
  funcs->timer_get_cycle = lw6dyn_dlsym (sys_context, handle, "shared_sdl_timer_get_cycle");

  ret = (funcs->load_consts && funcs->unload_consts && funcs->pump_events
	 && funcs->path_init && funcs->path_quit && funcs->timer_update && funcs->timer_get_timestamp && funcs->timer_get_uptime && funcs->timer_get_cycle);
#endif // MOD_GL1 || MOD_GLES2 || MOD_SOFT
#endif // LW6_ALLINONE

  return ret;
}

void
_lw6gfx_sdl_unbind_funcs (lw6sys_context_t * sys_context, _lw6gfx_sdl_funcs_t * funcs)
{
  memset (funcs, 0, sizeof (_lw6gfx_sdl_funcs_t));
}

static void
_warning (lw6sys_context_t * sys_context, const char *func_name)
{
  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("gfx sdl function \"%s\" is not defined"), func_name);
}

int
_lw6gfx_sdl_load_consts (lw6sys_context_t * sys_context, _lw6gfx_sdl_context_t * sdl_context)
{
  int ret = 0;

  if (sdl_context->funcs.load_consts)
    {
      ret = sdl_context->funcs.load_consts (sys_context, sdl_context);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  return ret;
}

void
_lw6gfx_sdl_unload_consts (lw6sys_context_t * sys_context, _lw6gfx_sdl_context_t * sdl_context)
{
  if (sdl_context->funcs.unload_consts)
    {
      sdl_context->funcs.unload_consts (sys_context, sdl_context);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }
}

lw6gui_input_t *
_lw6gfx_sdl_pump_events (lw6sys_context_t * sys_context, _lw6gfx_sdl_context_t * sdl_context, _lw6gfx_sdl_event_callback_t event_callback_func,
			 void *event_callback_data)
{
  lw6gui_input_t *ret = NULL;

  if (sdl_context->funcs.pump_events)
    {
      ret = sdl_context->funcs.pump_events (sys_context, sdl_context, event_callback_func, event_callback_data);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  return ret;
}

int
_lw6gfx_sdl_path_init (lw6sys_context_t * sys_context, _lw6gfx_sdl_context_t * sdl_context, int argc, const char *argv[])
{
  int ret = 0;

  if (sdl_context->funcs.path_init)
    {
      ret = sdl_context->funcs.path_init (sys_context, &(sdl_context->path), argc, argv);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  return ret;
}

void
_lw6gfx_sdl_path_quit (lw6sys_context_t * sys_context, _lw6gfx_sdl_context_t * sdl_context)
{
  if (sdl_context->funcs.path_quit)
    {
      sdl_context->funcs.path_quit (sys_context, &(sdl_context->path));
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }
}

void
_lw6gfx_sdl_timer_update (lw6sys_context_t * sys_context, _lw6gfx_sdl_context_t * sdl_context)
{
  if (sdl_context->funcs.timer_update)
    {
      sdl_context->funcs.timer_update (sys_context, &(sdl_context->timer));
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }
}

int64_t
_lw6gfx_sdl_timer_get_timestamp (lw6sys_context_t * sys_context, const _lw6gfx_sdl_context_t * sdl_context)
{
  int64_t ret = 0LL;

  if (sdl_context->funcs.timer_get_timestamp)
    {
      ret = sdl_context->funcs.timer_get_timestamp (sys_context, &(sdl_context->timer));
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  return ret;
}

int64_t
_lw6gfx_sdl_timer_get_uptime (lw6sys_context_t * sys_context, const _lw6gfx_sdl_context_t * sdl_context)
{
  int64_t ret = 0LL;

  if (sdl_context->funcs.timer_get_uptime)
    {
      ret = sdl_context->funcs.timer_get_uptime (sys_context, &(sdl_context->timer));
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  return ret;
}

int32_t
_lw6gfx_sdl_timer_get_cycle (lw6sys_context_t * sys_context, const _lw6gfx_sdl_context_t * sdl_context)
{
  int32_t ret = 0LL;

  if (sdl_context->funcs.timer_get_cycle)
    {
      ret = sdl_context->funcs.timer_get_cycle (sys_context, &(sdl_context->timer));
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  return ret;
}
