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

#ifndef LIQUIDWAR6BOT_RANDOM_INTERNAL_H
#define LIQUIDWAR6BOT_RANDOM_INTERNAL_H

#define _MOD_RANDOM_DEFAULT_ROUNDS_BETWEEN_MOVE 100

#include "../bot.h"

typedef struct _mod_random_context_s
{
  int last_move_round;
  int pos_x;
  int pos_y;
}
_mod_random_context_t;

/* mod-random-setup.c */
extern _mod_random_context_t *_mod_random_init (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6bot_data_t * data);
extern void _mod_random_quit (lw6sys_context_t * sys_context, _mod_random_context_t * random_context);

/* mod-random-move.c */
extern int _mod_random_next_move (lw6sys_context_t * sys_context, _mod_random_context_t * random_context, int *x, int *y, lw6bot_data_t * data);

/* mod-random-repr.c */
extern char *_mod_random_repr (lw6sys_context_t * sys_context, _mod_random_context_t * random_context, u_int32_t id);

#endif // LIQUIDWAR6BOT_RANDOM_INTERNAL_H
