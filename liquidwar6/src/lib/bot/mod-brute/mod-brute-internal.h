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

#ifndef LIQUIDWAR6BOT_BRUTE_INTERNAL_H
#define LIQUIDWAR6BOT_BRUTE_INTERNAL_H

#include "../bot.h"

#define _MOD_BRUTE_PERCENT_OF_SURFACE_FOR_RETRIES 10
#define _MOD_BRUTE_STABILITY 10
#define _MOD_BRUTE_IQ_DIVISOR 33.0f

typedef struct _mod_brute_context_s
{
  int nb_rounds_to_anticipate;
  lw6ker_game_state_t *game_sandbox;
}
_mod_brute_context_t;

/* mod-brute-setup.c */
extern _mod_brute_context_t *_mod_brute_init (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6bot_data_t * data);
extern void _mod_brute_quit (lw6sys_context_t * sys_context, _mod_brute_context_t * brute_context);

/* mod-brute-move.c */
extern int _mod_brute_next_move (lw6sys_context_t * sys_context, _mod_brute_context_t * brute_context, int *x, int *y, lw6bot_data_t * data);

/* mod-brute-repr.c */
extern char *_mod_brute_repr (lw6sys_context_t * sys_context, _mod_brute_context_t * brute_context, u_int32_t id);

#endif // LIQUIDWAR6BOT_BRUTE_INTERNAL_H
