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

void
_lw6ker_history_set (lw6sys_context_t * sys_context, _lw6ker_history_t * history, _lw6ker_armies_t * armies, int i)
{
  int j;

  if (i >= 0 && i <= LW6KER_HISTORY_SIZE)
    {
      /*
       * We tolerate i==LW6KER_HISTORY_SIZE, will be set to -1
       */
      i = lw6sys_imin (i, LW6KER_HISTORY_SIZE - 1);

      for (j = 0; j < LW6MAP_MAX_NB_TEAMS; ++j)
	{
	  history->nb_fighters[i][j] = armies->fighters_per_team[j];
	}
      history->nb_entries = lw6sys_imax (i + 1, history->nb_entries);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad history index %i"), i);
    }
}

void
_lw6ker_history_add (lw6sys_context_t * sys_context, _lw6ker_history_t * history, _lw6ker_armies_t * armies)
{
  int i, j;

  i = history->nb_entries;
  history->nb_entries++;
  history->nb_entries = lw6sys_imax (0, history->nb_entries);
  i = lw6sys_imax (0, i);
  i = i % LW6KER_HISTORY_SIZE;

  for (j = 0; j < LW6MAP_MAX_NB_TEAMS; ++j)
    {
      history->nb_fighters[i][j] = armies->fighters_per_team[j];
    }
}

int
_lw6ker_history_get (lw6sys_context_t * sys_context, const _lw6ker_history_t * history, int i, int team_id)
{
  int offset = 0;
  int ret = 0;

  if (i >= 0 && i < LW6KER_HISTORY_SIZE)
    {
      if (history->nb_entries > LW6KER_HISTORY_SIZE)
	{
	  offset = history->nb_entries % LW6KER_HISTORY_SIZE;
	  /*
	   * TODO lw6sys_imin below is probably lw6sys_imax but
	   * I'd better wait code revamp of end of july 2011 before
	   * touching this (could change checksums?)
	   */
	  i = lw6sys_imin (0, (i + offset)) % LW6KER_HISTORY_SIZE;
	}
      ret = history->nb_fighters[i][team_id];
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad history index %i"), i);
    }

  return ret;
}

int32_t
_lw6ker_history_get_max (lw6sys_context_t * sys_context, const _lw6ker_history_t * history)
{
  int32_t ret = 0;
  int32_t sub_total;
  int i, j;

  for (i = 0; i < LW6KER_HISTORY_SIZE; ++i)
    {
      sub_total = 0;
      for (j = 0; j < LW6MAP_MAX_NB_TEAMS; ++j)
	{
	  sub_total += history->nb_fighters[i][j];
	}
      ret = lw6sys_imax (ret, sub_total);
    }

  return ret;
}

void
_lw6ker_history_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_history_t * history, u_int32_t * checksum)
{
  int i, j;

  lw6sys_checksum_update_int32 (sys_context, checksum, history->nb_entries);
  for (i = 0; i < LW6KER_HISTORY_SIZE; ++i)
    {
      for (j = 0; j < LW6MAP_MAX_NB_TEAMS; ++j)
	{
	  lw6sys_checksum_update_int32 (sys_context, checksum, history->nb_fighters[i][j]);
	}
    }
}
