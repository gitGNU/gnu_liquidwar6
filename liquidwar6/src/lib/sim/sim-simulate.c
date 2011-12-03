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

#include "sim.h"

/**
 * lw6sim_simulate
 *
 * @results: out param, results of the simulation
 * @nb_teams: number of teams
 *
 * Runs a simulation of several battle/games on the default map using
 * different team settings. Will test teams up to the given number,
 * for instance if you give 3 as an argument, will run tests with teams
 * 0, 1 and 2 (that's to say a total of 3 teams).
 * 
 * Return value: 1 on success, 0 on failure.
 */
int
lw6sim_simulate (lw6sim_simulation_results_t * results, int nb_teams)
{
  int ret = 0;

  return ret;
}
