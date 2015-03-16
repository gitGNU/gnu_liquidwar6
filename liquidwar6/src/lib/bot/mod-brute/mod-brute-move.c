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
#include "mod-brute-internal.h"

int
_mod_brute_next_move (lw6sys_context_t * sys_context, _mod_brute_context_t * brute_context, int *x, int *y, lw6bot_data_t * data)
{
  int ret = 0;
  int score_now = 0;
  int score_staying_here = 0;
  int score_moving_there = 0;
  int i, z;
  lw6ker_cursor_t cursor;
  lw6sys_whd_t shape = { 0, 0, 0 };
  int found = 0;
  const lw6ker_fighter_t *fighter;
  int nb_retries;

  lw6ker_game_struct_get_shape (sys_context, data->game_state->game_struct, &shape);
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("anticipating %d rounds"), brute_context->nb_rounds_to_anticipate);

  (*x) = shape.w / 2;
  (*y) = shape.h / 2;
  if (lw6ker_game_state_get_cursor (sys_context, data->game_state, &cursor, data->param.cursor_id))
    {
      (*x) = cursor.pos.x;
      (*y) = cursor.pos.y;
      if (lw6ker_game_state_sync (sys_context, brute_context->game_sandbox, data->game_state))
	{
	  lw6ker_game_state_get_team_info (sys_context, brute_context->game_sandbox, cursor.team_color, NULL, &score_now);
	  for (i = 0; i < brute_context->nb_rounds_to_anticipate; ++i)
	    {
	      lw6ker_game_state_do_round (sys_context, brute_context->game_sandbox);
	    }
	  lw6ker_game_state_get_team_info (sys_context, brute_context->game_sandbox, cursor.team_color, NULL, &score_staying_here);
	  if (lw6ker_game_state_sync (sys_context, brute_context->game_sandbox, data->game_state))
	    {
	      if (score_staying_here < score_now || (score_staying_here == score_now && !lw6sys_random (sys_context, _MOD_BRUTE_STABILITY)))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("loosing by staying here, trying any place..."));
		  cursor.pos.x = lw6sys_random (sys_context, shape.w);
		  cursor.pos.y = lw6sys_random (sys_context, shape.h);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("same or better score staying here, still we try to find the other fighters"));
		  nb_retries = lw6ker_percent (shape.w * shape.h, _MOD_BRUTE_PERCENT_OF_SURFACE_FOR_RETRIES);
		  for (i = 0; i < nb_retries && !found; ++i)
		    {
		      cursor.pos.x = lw6sys_random (sys_context, shape.w);
		      cursor.pos.y = lw6sys_random (sys_context, shape.h);
		      for (z = 0; z < shape.d && !found; ++z)
			{
			  fighter = lw6ker_game_state_get_fighter_ro_safe (sys_context, brute_context->game_sandbox, cursor.pos.x, cursor.pos.y, z);
			  if (fighter && fighter->team_color != cursor.team_color)
			    {
			      found = 1;
			    }
			}
		    }
		}

	      lw6ker_game_state_set_cursor (sys_context, brute_context->game_sandbox, &cursor);
	      for (i = 0; i < brute_context->nb_rounds_to_anticipate; ++i)
		{
		  lw6ker_game_state_do_round (sys_context, brute_context->game_sandbox);
		}
	      lw6ker_game_state_get_team_info (sys_context, brute_context->game_sandbox, cursor.team_color, NULL, &score_moving_there);
	      /*
	       * The >= is important else we would stall and don't
	       * move when anticipation gives no feedback, and this is
	       * not aggressive enough.
	       */
	      if (score_moving_there > score_staying_here
		  || (score_moving_there == score_staying_here && (!lw6sys_random (sys_context, _MOD_BRUTE_STABILITY)) && (score_staying_here <= score_now)))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			      _x_
			      ("it's better to move there score_now=%d score_staying_here=%d score_moving_there=%d"),
			      score_now, score_staying_here, score_moving_there);
		  (*x) = cursor.pos.x;
		  (*y) = cursor.pos.y;
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			      _x_
			      ("it's better to stay here score_now=%d score_staying_here=%d score_moving_there=%d"),
			      score_now, score_staying_here, score_moving_there);
		}
	      ret = 1;
	    }
	}
    }

  return ret;
}
