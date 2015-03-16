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
#include "mod-idiot-internal.h"

static int
_new_target (lw6sys_context_t * sys_context, _mod_idiot_context_t * idiot_context, lw6bot_data_t * data)
{
  int team_color = LW6MAP_TEAM_COLOR_INVALID;
  int x = -1;
  int y = -1;
  int z = 0;
  int ret = 0;
  int i = 0;
  int found = 0;
  int fighter_id = -1;
  const lw6ker_fighter_t *fighter = NULL;
  lw6sys_whd_t shape = { 0, 0, 0 };
  lw6ker_cursor_t cursor;

  if (lw6ker_game_state_get_cursor (sys_context, data->game_state, &cursor, data->param.cursor_id))
    {
      x = cursor.pos.x;
      y = cursor.pos.y;
      team_color = cursor.team_color;

      lw6ker_game_state_get_shape (sys_context, data->game_state, &shape);

      idiot_context->start_pos_x = x;
      idiot_context->start_pos_y = y;

      for (i = 0; i < _MOD_IDIOT_NB_RETRIES && !found; ++i)
	{
	  x = lw6sys_random (sys_context, shape.w);
	  y = lw6sys_random (sys_context, shape.h);
	  idiot_context->target_pos_x = x;
	  idiot_context->target_pos_y = y;
	  if (data->param.iq >= _MOD_IDIOT_IQ_LIMIT1 && data->param.iq < _MOD_IDIOT_IQ_LIMIT2)
	    {
	      // here, we don't search anymore, just choose random
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("idiot bot acting randomly, choosed %d,%d"), x, y);
	      found = 1;
	    }
	  else
	    {
	      for (z = 0; z < shape.d && !found; ++z)
		{
		  fighter_id = lw6ker_game_state_get_fighter_id (sys_context, data->game_state, x, y, z);
		  if (fighter_id >= 0)
		    {
		      fighter = lw6ker_game_state_get_fighter_ro_by_id (sys_context, data->game_state, fighter_id);
		      if (fighter != NULL)
			{
			  if (data->param.iq >= _MOD_IDIOT_IQ_LIMIT2 && fighter->team_color != team_color)
			    {
			      // default behavior, find an opponent
			      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
					  _x_ ("idiot bot acting normally, choosed opponent of color %d at %d,%d"), fighter->team_color, x, y);
			      found = 1;
			    }
			  if (data->param.iq < _MOD_IDIOT_IQ_LIMIT1 && fighter->team_color == team_color)
			    {
			      // acting stupid, centering on self
			      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
					  _x_ ("idiot bot acting stupid, choosed self color %d at %d,%d"), fighter->team_color, x, y);
			      found = 1;
			    }
			}
		    }
		}
	      if (!found)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("idiot bot found nothing at %d,%d"), x, y);
		}
	    }
	}

      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_
		  ("idiot bot moved start=%d,%d target=%d,%d after %d tries"),
		  idiot_context->start_pos_x, idiot_context->start_pos_y, idiot_context->target_pos_x, idiot_context->target_pos_y, i);

      ret = found;
    }

  return ret;
}

int
_mod_idiot_next_move (lw6sys_context_t * sys_context, _mod_idiot_context_t * idiot_context, int *x, int *y, lw6bot_data_t * data)
{
  int ret = 0;
  int rounds = 0;
  int d_move;
  int d_wait;
  int dt = 0;
  int start_of_move;
  int end_of_move;
  float average_size = 0.0f;
  lw6sys_whd_t shape = { 0, 0, 0 };

  lw6ker_game_state_get_shape (sys_context, data->game_state, &shape);

  rounds = lw6ker_game_state_get_rounds (sys_context, data->game_state);
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("idiot bot move rounds=%d"), rounds);

  average_size = (shape.w + shape.h) / 2.0f;
  d_move = lw6sys_imax (1, (((float) _MOD_IDIOT_DEFAULT_MOVE_ROUNDS) * average_size) / (((float) _MOD_IDIOT_REFERENCE_SIZE) * data->param.speed));
  d_wait = lw6sys_imax (1, (((float) _MOD_IDIOT_DEFAULT_WAIT_ROUNDS) * average_size) / (((float) _MOD_IDIOT_REFERENCE_SIZE) * data->param.speed));
  if (idiot_context->last_move_round < 0 || (rounds >= idiot_context->last_move_round + d_move + d_wait))
    {
      _new_target (sys_context, idiot_context, data);
      if (idiot_context->last_move_round >= 0)
	{
	  idiot_context->last_move_round += d_move + d_wait;
	}
      else
	{
	  idiot_context->last_move_round = 0;
	}
    }

  start_of_move = idiot_context->last_move_round;
  end_of_move = start_of_move + d_move;

  if (rounds > start_of_move && rounds < end_of_move)
    {
      dt = lw6sys_imax (1, end_of_move - start_of_move);
      (*x) = ((end_of_move - rounds) * idiot_context->start_pos_x + (rounds - start_of_move) * idiot_context->target_pos_x) / dt;
      (*y) = ((end_of_move - rounds) * idiot_context->start_pos_y + (rounds - start_of_move) * idiot_context->target_pos_y) / dt;
    }
  else if (rounds >= end_of_move)
    {
      (*x) = idiot_context->target_pos_x;
      (*y) = idiot_context->target_pos_y;
    }
  else
    {
      (*x) = idiot_context->start_pos_x;
      (*y) = idiot_context->start_pos_y;
    }

  ret = 1;

  return ret;
}
