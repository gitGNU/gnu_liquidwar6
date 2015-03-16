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

#include "../bot.h"
#include "mod-follow-internal.h"

static int
_new_path (lw6sys_context_t * sys_context, _mod_follow_context_t * follow_context, lw6bot_data_t * data)
{
  int looser;
  lw6ker_cursor_t cursor;
  int ret = 0;
  lw6sys_whd_t shape = { 0, 0, 0 };
  lw6sys_xyz_t current_pos;
  lw6sys_xyz_t next_pos;
  int looping = 0;
  int i;

  lw6ker_game_state_get_shape (sys_context, data->game_state, &shape);
  if (lw6ker_game_state_get_cursor (sys_context, data->game_state, &cursor, data->param.cursor_id))
    {
      looser = lw6ker_game_state_get_looser (sys_context, data->game_state, cursor.team_color);
      if (looser >= 0 && looser < LW6MAP_MAX_NB_TEAMS)
	{
	  current_pos = cursor.pos;
	  follow_context->nb_steps = 0;
	  while (follow_context->nb_steps < _MOD_FOLLOW_MAX_PATH_SIZE && !looping)
	    {
	      lw6ker_move_get_best_next_pos (sys_context, data->game_state, &next_pos, &current_pos, looser);
	      for (i = lw6sys_imax (0, follow_context->nb_steps - _MOD_FOLLOW_LOOPING_BUFFER_SIZE); i < follow_context->nb_steps && !looping; ++i)
		{
		  if (follow_context->step[i].x == next_pos.x && follow_context->step[i].y == next_pos.y && follow_context->step[i].z == next_pos.z)
		    {
		      looping = 1;
		    }
		}
	      follow_context->step[follow_context->nb_steps] = next_pos;
	      current_pos = next_pos;
	      follow_context->nb_steps++;
	    }
	  ret = 1;
	}
    }

  if (!ret)
    {
      if (cursor.pos.x <= 0 && cursor.pos.y <= 0)
	{
	  follow_context->step[0].x = shape.w / 2;
	  follow_context->step[0].y = shape.h / 2;
	  follow_context->step[0].z = 0;
	}
      else
	{
	  follow_context->step[0] = cursor.pos;
	}
      follow_context->nb_steps = 1;
    }

  follow_context->current_step = 0;

  return ret;
}

int
_mod_follow_next_move (lw6sys_context_t * sys_context, _mod_follow_context_t * follow_context, int *x, int *y, lw6bot_data_t * data)
{
  int ret = 0;
  int rounds = 0;
  int delta_step = 0;
  int32_t z;

  rounds = lw6ker_game_state_get_rounds (sys_context, data->game_state);

  delta_step =
    (rounds - follow_context->last_move_round) * (((100 - data->param.iq) * _MOD_FOLLOW_IQ_0_SPEED) + (data->param.iq * _MOD_FOLLOW_IQ_100_SPEED)) / 100;
  follow_context->last_move_round = rounds;
  delta_step = lw6sys_imin (delta_step, 1);
  follow_context->current_step += delta_step;

  if (follow_context->current_step >= follow_context->nb_steps)
    {
      _new_path (sys_context, follow_context, data);
    }

  if (follow_context->current_step < follow_context->nb_steps)
    {
      (*x) = follow_context->step[follow_context->current_step].x;
      (*y) = follow_context->step[follow_context->current_step].y;
      z = follow_context->step[follow_context->current_step].z;

      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("follow bot move rounds=%d pos=%d,%d,%d"), rounds, *x, *y, z);
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("follow bot rounds=%d can't find next move"), rounds);
      (*x) = 0;
      (*y) = 0;
    }

  return ret;
}
