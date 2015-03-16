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

#include "nod.h"
#include "nod-internal.h"

static int
_set_community_id (lw6sys_context_t * sys_context, lw6nod_dyn_info_t * dyninfo, u_int64_t community_id)
{
  int ret = 0;

  if (dyninfo->community_id_str)
    {
      LW6SYS_FREE (sys_context, dyninfo->community_id_str);
      dyninfo->community_id_str = NULL;
    }
  if (community_id != LW6NOD_COMMUNITY_ID_NONE)
    {
      if (lw6sys_check_id (sys_context, community_id))
	{
	  dyninfo->community_id_str = lw6sys_id_ltoa (sys_context, community_id);
	}
      if (dyninfo->community_id_str != NULL)
	{
	  dyninfo->community_id_int = community_id;
	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("invalid community id \"%" LW6SYS_PRINTF_LL "x\""), (long long) community_id);
	  dyninfo->community_id_int = LW6NOD_COMMUNITY_ID_NONE;
	}
    }
  else
    {
      // OK, no community, set to NONE
      dyninfo->community_id_int = LW6NOD_COMMUNITY_ID_NONE;
      ret = 1;
    }

  return ret;
}

void
_lw6nod_dyn_info_reset (lw6sys_context_t * sys_context, lw6nod_dyn_info_t * dyn_info)
{
  int i;
  _set_community_id (sys_context, dyn_info, LW6NOD_COMMUNITY_ID_NONE);
  if (dyn_info->community_id_str)
    {
      LW6SYS_FREE (sys_context, dyn_info->community_id_str);
      dyn_info->community_id_str = NULL;
    }
  for (i = 0; i < LW6NOD_MAX_NB_PEERS; ++i)
    {
      _lw6nod_ref_info_reset (sys_context, &(dyn_info->community_peers[i]));
    }
  dyn_info->round = 0;
  if (dyn_info->level)
    {
      LW6SYS_FREE (sys_context, dyn_info->level);
      dyn_info->level = NULL;
    }
  dyn_info->required_bench = 0;
  dyn_info->nb_colors = 0;
  dyn_info->max_nb_colors = 0;
  dyn_info->nb_cursors = 0;
  dyn_info->max_nb_cursors = 0;
  dyn_info->nb_nodes = 0;
  dyn_info->max_nb_nodes = 0;
  if (dyn_info->game_screenshot_data)
    {
      LW6SYS_FREE (sys_context, dyn_info->game_screenshot_data);
      dyn_info->game_screenshot_data = NULL;
    }
}

int
_lw6nod_dyn_info_update (lw6sys_context_t * sys_context, lw6nod_dyn_info_t * dyn_info, u_int64_t community_id,
			 int round, const char *level, int required_bench,
			 int nb_colors, int max_nb_colors, int nb_cursors,
			 int max_nb_cursors, int nb_nodes, int max_nb_nodes, int game_screenshot_size, void *game_screenshot_data)
{
  int ret = 1;

  _set_community_id (sys_context, dyn_info, community_id);
  dyn_info->round = round;
  /*
   * This test about pointers being different is both an optimization
   * (will run faster if pointer are equal as it does nothing) and a
   * protection, else we could free the pointer before duplicating it.
   */
  if (level != dyn_info->level)
    {
      if (dyn_info->level)
	{
	  LW6SYS_FREE (sys_context, dyn_info->level);
	  dyn_info->level = NULL;
	}
      dyn_info->level = lw6sys_str_copy (sys_context, lw6sys_str_empty_if_null (sys_context, level));
    }
  dyn_info->required_bench = required_bench;
  dyn_info->nb_colors = nb_colors;
  dyn_info->max_nb_colors = max_nb_colors;
  dyn_info->nb_cursors = nb_cursors;
  dyn_info->max_nb_cursors = max_nb_cursors;
  dyn_info->nb_nodes = nb_nodes;
  dyn_info->max_nb_nodes = max_nb_nodes;

  dyn_info->game_screenshot_size = game_screenshot_size;
  /*
   * This test about pointers being different is both an optimization
   * (will run faster if pointer are equal as it does nothing) and a
   * protection, else we could free the pointer before duplicating it.
   */
  if (game_screenshot_data != dyn_info->game_screenshot_data)
    {
      if (dyn_info->game_screenshot_data)
	{
	  LW6SYS_FREE (sys_context, dyn_info->game_screenshot_data);
	  dyn_info->game_screenshot_data = NULL;
	}
      if (game_screenshot_size > 0)
	{
	  dyn_info->game_screenshot_data = LW6SYS_MALLOC (sys_context, game_screenshot_size);
	  if (dyn_info->game_screenshot_data)
	    {
	      memcpy (dyn_info->game_screenshot_data, game_screenshot_data, game_screenshot_size);
	    }
	}
    }

  if (level)
    {
      ret = (dyn_info->level) && ret;
    }
  if (game_screenshot_data)
    {
      ret = (dyn_info->game_screenshot_data) && ret;
    }

  return ret;
}

/**
 * lw6nod_dyn_info_free
 *
 * @sys_context: global system context
 * @dyn_info: the dyn info struct to free
 *
 * Frees a dyn info object, to be used after a
 * call to @lw6nod_info_dup_dyn for instance.
 *
 * Return value: none
 */
void
lw6nod_dyn_info_free (lw6sys_context_t * sys_context, lw6nod_dyn_info_t * dyn_info)
{
  _lw6nod_dyn_info_reset (sys_context, dyn_info);
  if (dyn_info->community_id_str)
    {
      LW6SYS_FREE (sys_context, dyn_info->community_id_str);
    }
  if (dyn_info->level)
    {
      LW6SYS_FREE (sys_context, dyn_info->level);
    }
  LW6SYS_FREE (sys_context, dyn_info);
}
