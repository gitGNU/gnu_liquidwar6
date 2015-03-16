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
#endif

#include "sim.h"
#include "sim-internal.h"

/**
 * lw6sim_results_zero
 *
 * @sys_context: global system context
 * @results: out param, will be cleared
 *
 * Fills the struct with zeroes.
 *
 * Return value: none.
 */
void
lw6sim_results_zero (lw6sys_context_t * sys_context, lw6sim_results_t * results)
{
  memset (results, 0, sizeof (lw6sim_results_t));
}

/**
 * lw6sim_results_update_percents
 *
 * @sys_context: global system context
 * @results: results set to work on (in/out param)
 *
 * Updates the structure so that the percent members
 * are up to date.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6sim_results_update_percents (lw6sys_context_t * sys_context, lw6sim_results_t * results)
{
  int ret = 0;
  int i;
  int absolute_total = 0;

  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      absolute_total += results->absolute[i];
    }
  if (absolute_total > 0)
    {
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	{
	  results->percent[i] = (((float) results->absolute[i]) * 100.0f) / ((float) absolute_total);
	}
      ret = 1;
    }

  return ret;
}
