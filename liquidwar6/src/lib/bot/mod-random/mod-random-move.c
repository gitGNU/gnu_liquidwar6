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
#include "mod-random-internal.h"

int
_mod_random_next_move (lw6sys_context_t * sys_context, _mod_random_context_t * random_context, int *x, int *y, lw6bot_data_t * data)
{
  int ret = 0;
  int rounds = 0;
  int next_move_round = 0;
  int d_move;

  rounds = lw6ker_game_state_get_rounds (sys_context, data->game_state);
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("random bot move rounds=%d"), rounds);

  d_move = lw6sys_imax (1, (float) _MOD_RANDOM_DEFAULT_ROUNDS_BETWEEN_MOVE / data->param.speed);
  next_move_round = random_context->last_move_round + d_move;
  if (random_context->pos_x < 0 || random_context->pos_y < 0 || rounds > next_move_round)
    {
      random_context->last_move_round = next_move_round;
      random_context->pos_x = lw6sys_random (sys_context, lw6ker_game_state_get_w (sys_context, data->game_state));
      random_context->pos_y = lw6sys_random (sys_context, lw6ker_game_state_get_h (sys_context, data->game_state));
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("random bot moved %d,%d"), random_context->pos_x, random_context->pos_y);
    }

  (*x) = random_context->pos_x;
  (*y) = random_context->pos_y;

  ret = 1;

  return ret;
}
