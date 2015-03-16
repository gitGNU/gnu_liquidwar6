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

#include "ker.h"
#include "ker-internal.h"

int
_lw6ker_score_sort_quantity_callback (lw6ker_score_t * score_a, lw6ker_score_t * score_b)
{
  int ret = 0;

  if (score_a->has_been_active == score_b->has_been_active)
    {
      if (score_a->fighters_absolute == score_b->fighters_absolute)
	{
	  if (score_a->frags == score_b->frags)
	    {
	      ret = 0;
	    }
	  else
	    {
	      ret = (score_a->frags < score_b->frags) ? -1 : 1;
	    }
	}
      else
	{
	  ret = (score_a->fighters_absolute < score_b->fighters_absolute) ? -1 : 1;
	}
    }
  else
    {
      /*
       * It's important to sort on this parameter
       * to make sure if a team has lost/is respawning it stills
       * appear next to the active teams after being sorted
       * This fixes bug https://savannah.gnu.org/bugs/?35076
       * because this way team_color=-1 will be put at the
       * end of the list when sorting reverse (desc)
       */
      ret = (score_a->team_color < score_b->team_color) ? -1 : 1;
    }

  return ret;
}

int
_lw6ker_score_sort_quantity_callback_desc (lw6ker_score_t * score_a, lw6ker_score_t * score_b)
{
  return (_lw6ker_score_sort_quantity_callback (score_b, score_a));
}

int
_lw6ker_score_sort_frags_callback (lw6ker_score_t * score_a, lw6ker_score_t * score_b)
{
  int ret = 0;

  if (score_a->has_been_active == score_b->has_been_active)
    {
      if (score_a->frags == score_b->frags)
	{
	  if (score_a->fighters_absolute == score_b->fighters_absolute)
	    {
	      ret = 0;
	    }
	  else
	    {
	      ret = (score_a->fighters_absolute < score_b->fighters_absolute) ? -1 : 1;
	    }
	}
      else
	{
	  ret = (score_a->frags < score_b->frags) ? -1 : 1;
	}
    }
  else
    {
      /*
       * It's important to sort on this parameter
       * to make sure if a team has lost/is respawning it stills
       * appear next to the active teams after being sorted
       * This fixes bug https://savannah.gnu.org/bugs/?35076
       * because this way team_color=-1 will be put at the
       * end of the list when sorting reverse (desc)
       */
      ret = (score_a->team_color < score_b->team_color) ? -1 : 1;
    }

  return ret;
}

int
_lw6ker_score_sort_frags_callback_desc (lw6ker_score_t * score_a, lw6ker_score_t * score_b)
{
  return (_lw6ker_score_sort_frags_callback (score_b, score_a));
}
