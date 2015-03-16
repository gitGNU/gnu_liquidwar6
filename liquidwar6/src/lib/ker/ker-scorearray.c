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

#include <string.h>

#include "ker.h"
#include "ker-internal.h"

int
_lw6ker_score_array_update (lw6sys_context_t * sys_context, lw6ker_score_array_t * score_array, const _lw6ker_game_state_t * game_state)
{
  int ret = 0;
  int i = 0;
  int active_fighters = 0;
  int fighters_per_team = 0;
  int frags = 0;
  int frags_min = 0;
  int consolidated_total = 0;
  int absolute_other = 0;
  int percent_other = 0;
  int diff;

  memset (score_array, 0, sizeof (lw6ker_score_array_t));
  for (i = 0; i < LW6MAP_NB_TEAM_COLORS; ++i)
    {
      score_array->scores[i].team_color = LW6MAP_TEAM_COLOR_INVALID;
    }
  /*
   * In a general manner, this function can be executed from another
   * thread in dirty read mode, so it's wise to make a copy of
   * everything locally before testing it and assuming, for instance,
   * that it is not null. Typicall example, active_fighters must be >0
   * because we're gonna divide by it, so we need to store it in
   * the stack rather than getting the value from the pointer each time.
   */
  active_fighters = game_state->map_state.armies.active_fighters;

  if (active_fighters > 0)
    {
      score_array->active_fighters = active_fighters;
      // LW6MAP_MAX_NB_TEAMS and LW6MAP_NB_TEAM_COLORS should be the same
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS && i < LW6MAP_NB_TEAM_COLORS; ++i)
	{
	  fighters_per_team = game_state->map_state.armies.fighters_per_team[i];
	  frags = game_state->map_state.armies.frags[i];
	  /*
	   * We need to test has_been_active and not just active,
	   * for interesting teams could be dead or something...
	   */
	  if (game_state->map_state.teams[i].has_been_active)
	    {
	      score_array->nb_scores++;
	      score_array->scores[i].has_been_active = 1;
	      score_array->scores[i].team_color = i;
	      score_array->scores[i].fighters_absolute = fighters_per_team;
	      score_array->scores[i].fighters_percent = lw6sys_imax ((fighters_per_team > 0) ? 1 : 0, (100 * fighters_per_team) / active_fighters);
	      score_array->scores[i].fighters_ratio = ((float) fighters_per_team) / ((float) active_fighters);
	      score_array->scores[i].frags = frags;
	    }
	}
      qsort (score_array->scores, LW6MAP_NB_TEAM_COLORS, sizeof (lw6ker_score_t), (void *) _lw6ker_score_sort_quantity_callback_desc);

      absolute_other = 0;
      percent_other = 0;
      for (i = LW6MAP_NB_TEAM_COLORS - 1; i >= 1; --i)
	{
	  absolute_other += score_array->scores[i].fighters_absolute;
	  percent_other += score_array->scores[i].fighters_percent;
	}

      score_array->scores[0].fighters_absolute = active_fighters - absolute_other;
      score_array->scores[0].fighters_percent = 100 - percent_other;

      if (score_array->scores[1].fighters_percent > score_array->scores[0].fighters_percent)
	{
	  /*
	   * 2nd is higher than 1st because of rounding errors
	   */
	  diff = score_array->scores[1].fighters_percent - score_array->scores[0].fighters_percent;
	  for (i = score_array->nb_scores - 1; i >= 1 && diff > 0; --i)
	    {
	      if (score_array->scores[i].fighters_percent > 0)
		{
		  score_array->scores[0].fighters_percent++;
		  score_array->scores[i].fighters_percent--;
		  diff--;
		}
	    }
	}

      if (score_array->scores[0].fighters_absolute > 0 && score_array->scores[0].fighters_percent > 0 && score_array->nb_scores > 0)
	{
	  ret = 1;
	}

      if (game_state->game_struct->rules.respawn_team)
	{
	  qsort (score_array->scores, score_array->nb_scores, sizeof (lw6ker_score_t), (void *) _lw6ker_score_sort_frags_callback_desc);
	  /*
	   * Now we calculate consolidated_percent which is used
	   * for final score display
	   */
	  frags_min = 0;
	  for (i = 0; i < score_array->nb_scores; ++i)
	    {
	      if (score_array->scores[i].frags < frags_min)
		{
		  frags_min = score_array->scores[i].frags;
		}
	    }
	  for (i = 0; i < score_array->nb_scores; ++i)
	    {
	      score_array->scores[i].consolidated_percent = ((score_array->scores[i].frags - frags_min) * 100) + score_array->scores[i].fighters_percent;
	    }
	  for (i = 0; i < score_array->nb_scores; ++i)
	    {
	      consolidated_total += score_array->scores[i].consolidated_percent;
	    }
	  if (consolidated_total > 0)
	    {
	      for (i = 0; i < score_array->nb_scores; ++i)
		{
		  score_array->scores[i].consolidated_percent = 100 * score_array->scores[i].consolidated_percent / consolidated_total;
		}

	      percent_other = 0;
	      for (i = 1; i < score_array->nb_scores; ++i)
		{
		  percent_other += score_array->scores[i].consolidated_percent;
		}

	      score_array->scores[0].consolidated_percent = 100 - percent_other;

	      if (score_array->scores[1].consolidated_percent > score_array->scores[0].consolidated_percent)
		{
		  /*
		   * 2nd is higher than 1st because of rounding errors
		   */
		  diff = score_array->scores[1].consolidated_percent - score_array->scores[0].consolidated_percent;
		  for (i = score_array->nb_scores - 1; i >= 1 && diff > 0; --i)
		    {
		      if (score_array->scores[i].consolidated_percent > 0)
			{
			  score_array->scores[0].consolidated_percent++;
			  score_array->scores[i].consolidated_percent--;
			  diff--;
			}
		    }
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("invalid consolidated_total %d"), consolidated_total);
	    }
	}
      else
	{
	  for (i = 0; i < score_array->nb_scores; ++i)
	    {
	      score_array->scores[i].consolidated_percent = score_array->scores[i].fighters_percent;
	    }
	}
      /*
       * At least one percent per team
       */
      diff = 0;
      for (i = 1; i < score_array->nb_scores; ++i)
	{
	  if (score_array->scores[i].consolidated_percent == 0 && score_array->scores[i].fighters_absolute > 0)
	    {
	      score_array->scores[i].consolidated_percent = 1;
	      diff++;
	    }
	}
      if (score_array->nb_scores == 2)
	{
	  score_array->scores[0].consolidated_percent = 100 - score_array->scores[1].consolidated_percent;
	}
      else if (score_array->nb_scores >= 3)
	{
	  /*
	   * First pass, substract 1 to all those who at least
	   * have 2% left
	   */
	  for (i = score_array->nb_scores - 1; i >= 1; --i)
	    {
	      if (diff > 0)
		{
		  if (score_array->scores[i].consolidated_percent > 1)
		    {
		      score_array->scores[i].consolidated_percent--;
		      diff--;
		    }
		}
	    }
	  /*
	   * Second pass, substract 1 to all those left with
	   * at least 1% left.
	   */
	  for (i = score_array->nb_scores - 1; i >= 1; --i)
	    {
	      if (diff > 0)
		{
		  if (score_array->scores[i].consolidated_percent > 0)
		    {
		      score_array->scores[i].consolidated_percent--;
		      diff--;
		    }
		}
	    }
	}
    }

  return ret;
}

/**
 * lw6ker_score_array_update
 *
 * @sys_context: global system context
 * @score_array: the score array to modify
 * @game_state: the game_state to get the information from
 *
 * Updates a score array, that is, calculates all scores, so that they
 * can be displayed, for instance.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6ker_score_array_update (lw6sys_context_t * sys_context, lw6ker_score_array_t * score_array, const lw6ker_game_state_t * game_state)
{
  return _lw6ker_score_array_update (sys_context, score_array, (const _lw6ker_game_state_t *) game_state);
}
