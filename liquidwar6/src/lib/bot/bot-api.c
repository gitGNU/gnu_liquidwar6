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
#endif // HAVE_CONFIG_H

#include "bot.h"

static void
_warning (lw6sys_context_t * sys_context, const char *func_name)
{
  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bot backend function \"%s\" is not defined"), func_name);
}

/**
 * lw6bot_init
 *
 * @sys_context: global system context
 * @backend: backend to use
 * @seed: parameters required to build bot (game state, among other things)
 *
 * Initializes a bot object. Must be performed before any other call.
 * The seed is absolutely required, for a bot really
 * needs to know what map/context it's working on, including at creation time
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6bot_init (lw6sys_context_t * sys_context, lw6bot_backend_t * backend, lw6bot_seed_t * seed)
{
  lw6bot_data_t data;
  int bad_speed = 0;
  int bad_iq = 0;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->init)
    {
      backend->bot_context = NULL;
      memset (&data, 0, sizeof (lw6bot_data_t));
      backend->seed = *seed;
      if (backend->seed.param.speed <= 0.0f)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("incorrect speed %0.1f"), backend->seed.param.speed);
	  bad_speed = 1;
	}
      if (backend->seed.param.iq < 0)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("incorrect iq %d"), backend->seed.param.iq);
	  bad_iq = 1;
	}
      data.param = backend->seed.param;
      data.game_state = seed->game_state;
      if ((!bad_speed) && (!bad_iq))
	{
	  backend->bot_context = backend->init (sys_context, backend->argc, backend->argv, &data);
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return backend->bot_context ? 1 : 0;
}

/**
 * lw6bot_quit
 *
 * @sys_context: global system context
 * @backend: unitialize a bot backend
 *
 * Closes a bot, but does not free all ressources.
 */
void
lw6bot_quit (lw6sys_context_t * sys_context, lw6bot_backend_t * backend)
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
	  backend->quit (sys_context, backend->bot_context);
	  backend->bot_context = NULL;
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;
}

/**
 * lw6bot_next_move
 *
 * @sys_context: global system context
 * @backend: bot to work on
 * @x: next x position (out param)
 * @y: next y position (out param)
 *
 * Queries the bot for its next move, this is actually the one
 * interesting function in the whole bot API.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6bot_next_move (lw6sys_context_t * sys_context, lw6bot_backend_t * backend, int *x, int *y)
{
  int ret = 0;
  lw6bot_data_t data;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->next_move)
    {
      memset (&data, 0, sizeof (lw6bot_data_t));
      data.param = backend->seed.param;
      /*
       * We set the game_state to NULL to avoid referencing
       * some old stuff passed at creation, which
       * could have disappeared.
       */
      data.game_state = NULL;
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
	      data.game_state = lw6pil_pilot_dirty_read (sys_context, backend->seed.pilot);
	    }
	  else
	    {
	      data.game_state = backend->seed.game_state;
	    }
	  break;
	default:
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unknown dirty read mode %d"), backend->seed.dirty_read);
	}
      if (data.game_state)
	{
	  ret = backend->next_move (sys_context, backend->bot_context, x, y, &data);
	}
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}

/**
 * lw6bot_repr
 *
 * @sys_context: global system context
 * @backend: bot to represent
 *
 * Gives a human readable representation of bot
 *
 * Return value: dynamically allocated string.
 */
char *
lw6bot_repr (lw6sys_context_t * sys_context, const lw6bot_backend_t * backend)
{
  char *ret = NULL;

  LW6SYS_BACKEND_FUNCTION_BEGIN;

  if (backend->repr)
    {
      ret = backend->repr (sys_context, backend->bot_context, backend->id);
    }
  else
    {
      _warning (sys_context, __FUNCTION__);
    }

  LW6SYS_BACKEND_FUNCTION_END;

  return ret;
}
