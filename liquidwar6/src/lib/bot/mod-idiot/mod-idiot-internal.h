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

#ifndef LIQUIDWAR6BOT_IDIOT_INTERNAL_H
#define LIQUIDWAR6BOT_IDIOT_INTERNAL_H

#include "../bot.h"

#define _MOD_IDIOT_NB_RETRIES 1000
#define _MOD_IDIOT_DEFAULT_MOVE_ROUNDS 25
#define _MOD_IDIOT_DEFAULT_WAIT_ROUNDS 50
#define _MOD_IDIOT_REFERENCE_SIZE 200
#define _MOD_IDIOT_IQ_LIMIT1 33
#define _MOD_IDIOT_IQ_LIMIT2 66

typedef struct _mod_idiot_context_s
{
  int last_move_round;
  int start_pos_x;
  int start_pos_y;
  int target_pos_x;
  int target_pos_y;
}
_mod_idiot_context_t;

/* mod-idiot-setup.c */
extern _mod_idiot_context_t *_mod_idiot_init (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6bot_data_t * data);
extern void _mod_idiot_quit (lw6sys_context_t * sys_context, _mod_idiot_context_t * idiot_context);

/* mod-idiot-move.c */
extern int _mod_idiot_next_move (lw6sys_context_t * sys_context, _mod_idiot_context_t * idiot_context, int *x, int *y, lw6bot_data_t * data);

/* mod-idiot-repr.c */
extern char *_mod_idiot_repr (lw6sys_context_t * sys_context, _mod_idiot_context_t * idiot_context, u_int32_t id);

#endif // LIQUIDWAR6BOT_IDIOT_INTERNAL_H
