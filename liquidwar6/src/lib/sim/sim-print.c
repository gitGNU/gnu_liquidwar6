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
 * lw6sim_print
 *
 * @sys_context: global system context
 * @results: data to print
 * @f: file to print data to
 *
 * Pretty prints results on standard output.
 *
 * Return value: none.
 */
void
lw6sim_print (lw6sys_context_t * sys_context, lw6sim_results_t * results, FILE * f)
{
  int i;
  char *color_key = NULL;
  float percent = 0.0f;
  int absolute = 0;

  for (i = 0; i < results->nb_teams; ++i)
    {
      color_key = lw6map_team_color_index_to_key (sys_context, i);
      percent = results->percent[i];
      absolute = results->absolute[i];
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("%s\": %02.1f (%d)"), color_key, percent, absolute);
      fprintf (f, "%s: %02.1f%%", color_key, percent);
      if (i < results->nb_teams - 1)
	{
	  fprintf (f, ", ");
	}
      else
	{
	  fprintf (f, "\n");
	}
    }
}
