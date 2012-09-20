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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gfx-internal.h"

int
_lw6gfx_sdl_load_consts (_lw6gfx_sdl_context_t * sdl_context)
{
  int ret = 0;

  if (sdl_context->funcs.load_consts)
    {
      ret = sdl_context->funcs.load_consts (sdl_context);
    }

  return ret;
}

void
_lw6gfx_sdl_unload_consts (_lw6gfx_sdl_context_t * sdl_context)
{
  if (sdl_context->funcs.unload_consts)
    {
      sdl_context->funcs.unload_consts (sdl_context);
    }
}

lw6gui_input_t *
_lw6gfx_sdl_pump_events (_lw6gfx_sdl_context_t * sdl_context)
{
  lw6gui_input_t *ret = NULL;

  if (sdl_context->funcs.pump_events)
    {
      ret = sdl_context->funcs.pump_events (sdl_context);
    }

  return ret;
}

int
_lw6gfx_sdl_path_init (_lw6gfx_sdl_context_t * sdl_context, int argc,
		       const char *argv[])
{
  int ret = 0;

  if (sdl_context->funcs.path_init)
    {
      ret = sdl_context->funcs.path_init (&(sdl_context->path), argc, argv);
    }

  return ret;
}

void
_lw6gfx_sdl_path_quit (_lw6gfx_sdl_context_t * sdl_context)
{
  if (sdl_context->funcs.path_quit)
    {
      sdl_context->funcs.path_quit (&(sdl_context->path), argc, argv);
    }
}

void
_lw6gfx_sdl_timer_update (_lw6gfx_sdl_context_t * sdl_context)
{
  if (sdl_context->funcs.timer_update)
    {
      sdl_context->funcs.timer_update (&(sdl_context->timer));
    }
}

int64_t
_lw6gfx_sdl_timer_get_timestamp (_lw6gfx_sdl_context_t * sdl_context)
{
  int64_t ret = 0LL;

  if (sdl_context->funcs.timer_get_timestamp)
    {
      ret = sdl_context->funcs.timer_get_timestamp (&(sdl_context->timer));
    }

  return ret;
}

int64_t
_lw6gfx_sdl_timer_get_uptime (_lw6gfx_sdl_context_t * sdl_context)
{
  int64_t ret = 0LL;

  if (sdl_context->funcs.timer_get_uptime)
    {
      ret = sdl_context->funcs.timer_get_uptime (&(sdl_context->timer));
    }

  return ret;
}

int32_t
_lw6gfx_sdl_timer_get_cycle (_lw6gfx_sdl_context_t * sdl_context)
{
  int32_t ret = 0LL;

  if (sdl_context->funcs.timer_get_cycle)
    {
      ret = sdl_context->funcs.timer_get_cycle (&(sdl_context->timer));
    }

  return ret;
}
