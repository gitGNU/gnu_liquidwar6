/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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

#include "../bot.h"
#include "mod-follow-internal.h"

static int
new_path (_mod_follow_context_t * follow_context, lw6bot_data_t * data)
{
  int looser;
  lw6ker_cursor_t cursor;
  int ret = 0;
  lw6sys_whd_t shape = { 0, 0, 0 };
  int x, y, z;
  int max_potential = 0;
  int potential;
  int zone_id;
  int fighter_id;
  lw6ker_fighter_t *fighter;
  lw6sys_xyz_t found = { 0, 0, 0 };

  lw6ker_game_state_get_shape (data->game_state, &shape);
  if (lw6ker_game_state_get_cursor
      (data->game_state, &cursor, data->param.cursor_id))
    {
      looser =
	lw6ker_game_state_get_looser (data->game_state, cursor.team_color);
      if (looser >= 0 && looser < LW6MAP_MAX_NB_TEAMS)
	{
	  for (z = 0; z < shape.d; ++z)
	    {
	      for (y = 0; y < shape.h; ++y)
		{
		  for (x = 0; x < shape.w; ++x)
		    {
		      fighter_id =
			lw6ker_game_state_get_fighter_id (data->game_state, x,
							  y, z);
		      if (fighter_id >= 0)
			{
			  fighter =
			    lw6ker_game_state_get_fighter_by_id
			    (data->game_state, fighter_id);
			  if (fighter)
			    {
			      if (fighter->team_color == looser)
				{
				  zone_id =
				    lw6ker_game_struct_get_zone_id
				    (data->game_state->game_struct, x, y, z);
				  potential =
				    lw6ker_game_state_get_zone_potential
				    (data->game_state, zone_id, looser);
				  if (potential > max_potential)
				    {
				      max_potential = potential;
				      found.x = x;
				      found.y = y;
				      found.z = z;

				      /*
				       * TODO: make it walk through it, not straight...
				       */
				      follow_context->step[0] = found;
				      follow_context->nb_steps = 1;

				      ret = 1;
				    }
				}
			    }
			}
		    }
		}
	    }
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
_mod_follow_next_move (_mod_follow_context_t * follow_context, int *x, int *y,
		       lw6bot_data_t * data)
{
  int ret = 0;
  int rounds = 0;
  int delta_step = 0;
  int32_t z;

  rounds = lw6ker_game_state_get_rounds (data->game_state);

  delta_step =
    (rounds -
     follow_context->last_move_round) * (((100 - data->param.iq) *
					  _MOD_FOLLOW_IQ_0_SPEED) +
					 (data->param.iq *
					  _MOD_FOLLOW_IQ_100_SPEED)) / 100;
  TMP1 ("%d", delta_step);
  follow_context->last_move_round = rounds;
  delta_step = lw6sys_min (delta_step, 1);
  follow_context->current_step += delta_step;

  if (follow_context->current_step >= follow_context->nb_steps)
    {
      new_path (follow_context, data);
    }

  if (follow_context->current_step < follow_context->nb_steps)
    {
      (*x) = follow_context->step[follow_context->current_step].x;
      (*y) = follow_context->step[follow_context->current_step].y;
      z = follow_context->step[follow_context->current_step].z;

      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("follow bot move rounds=%d pos=%d,%d,%d"), rounds, *x,
		  *y, z);
      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("follow bot rounds=%d can't find next move"), rounds);
      (*x) = 0;
      (*y) = 0;
    }

  return ret;
}
