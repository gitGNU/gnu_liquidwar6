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
#include "mod-brute-internal.h"

_mod_brute_context_t *
_mod_brute_init (int argc, char *argv[], lw6bot_data_t * data)
{
  _mod_brute_context_t *brute_context = NULL;
  float iq_divided = 0.0f;

  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("brute init"));

  brute_context =
    (_mod_brute_context_t *) LW6SYS_CALLOC (sizeof (_mod_brute_context_t));
  if (brute_context)
    {
      iq_divided = data->param.iq / _MOD_BRUTE_IQ_DIVISOR;
      brute_context->nb_rounds_to_anticipate = 1 + iq_divided * iq_divided;
      brute_context->game_sandbox =
	lw6ker_game_state_dup (data->game_state, NULL);
      if (!brute_context->game_sandbox)
	{
	  _mod_brute_quit (brute_context);
	  brute_context = NULL;
	}
    }

  return brute_context;
}

void
_mod_brute_quit (_mod_brute_context_t * brute_context)
{
  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("brute quit"));
  if (brute_context->game_sandbox)
    {
      lw6ker_game_state_free (brute_context->game_sandbox);
      brute_context->game_sandbox = NULL;
    }
  LW6SYS_FREE (brute_context);
}
