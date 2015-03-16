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

#ifndef LIQUIDWAR6SIM_INTERNAL_H
#define LIQUIDWAR6SIM_INTERNAL_H

#include "sim.h"

#define _LW6SIM_MASK_MIN 1
#define _LW6SIM_BOT_ROUNDS_PERIOD 10
#define _LW6SIM_SIMULATION_TIME 60

/* sim-mask */
extern int _lw6sim_mask_get_max (lw6sys_context_t * sys_context, int nb_teams);
extern int _lw6sim_mask_is_valid (lw6sys_context_t * sys_context, int mask);

#endif
