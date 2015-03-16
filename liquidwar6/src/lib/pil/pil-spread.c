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

#include "pil.h"
#include "pil-internal.h"


void
_lw6pil_spread_thread_func (lw6sys_context_t * sys_context, _lw6pil_spread_data_t * spread_data)
{
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("spread with team_mask %x begin"), spread_data->team_mask);
  lw6ker_game_state_do_spread (sys_context, spread_data->game_state, spread_data->team_mask);
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("spread with team_mask %x end"), spread_data->team_mask);
}
