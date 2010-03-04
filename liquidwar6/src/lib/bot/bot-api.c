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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "bot.h"

static void
_warning (const char *func_name)
{
  lw6sys_log (LW6SYS_LOG_WARNING,
	      _("bot backend function \"%s\" is not defined"), func_name);
}

int
lw6bot_init (lw6bot_backend_t * backend, lw6bot_seed_t * seed)
{
  lw6bot_data_t data;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->init)
    {
      memset (&data, 0, sizeof (lw6bot_data_t));
      backend->seed = *seed;
      if (backend->seed.param.speed <= 0.0f)
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _("incorrect speed %0.1f"),
		      backend->seed.param.speed);
	  backend->seed.param.iq = 1.0f;
	}
      if (backend->seed.param.iq < 0)
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _("incorrect iq %d"),
		      backend->seed.param.iq);
	  backend->seed.param.iq = 100;
	}
      data.param = backend->seed.param;
      data.game_state = seed->game_state;
      backend->bot_context =
	backend->init (backend->argc, backend->argv, &data);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return backend->bot_context ? 1 : 0;
}

void
lw6bot_quit (lw6bot_backend_t * backend)
{
  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->quit)
    {
      /*
       * It's important to check that backend is not NULL for
       * quit can *really* be called several times on the same backend
       */
      if (backend->bot_context)
	{
	  backend->quit (backend->bot_context);
	  backend->bot_context = NULL;
	}
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

int
lw6bot_next_move (lw6bot_backend_t * backend, int *x, int *y)
{
  int ret = 0;
  lw6bot_data_t data;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->next_move)
    {
      memset (&data, 0, sizeof (lw6bot_data_t));
      data.param = backend->seed.param;
      switch (backend->seed.dirty_read)
	{
	case LW6PIL_DIRTY_READ_NEVER:
	case LW6PIL_DIRTY_READ_SYNC_ONLY:
	  /*
	   * In those cases we assume the game_state we have has been
	   * synchronized elsewhere, typically in the display thread.
	   */
	  data.game_state = backend->seed.game_state;
	  break;
	case LW6PIL_DIRTY_READ_ALWAYS:
	  if (backend->seed.pilot)
	    {
	      data.game_state = lw6pil_pilot_dirty_read (backend->seed.pilot);
	    }
	  break;
	default:
	  lw6sys_log (LW6SYS_LOG_WARNING, _("unknown dirty read mode %d"),
		      backend->seed.dirty_read);
	}
      if (data.game_state)
	{
	  ret = backend->next_move (backend->bot_context, x, y, &data);
	}
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

char *
lw6bot_repr (lw6bot_backend_t * backend)
{
  char *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->repr)
    {
      ret = backend->repr (backend->bot_context, backend->id);
    }
  else
    {
      _warning (__FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}
