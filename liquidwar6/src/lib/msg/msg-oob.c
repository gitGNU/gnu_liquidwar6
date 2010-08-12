/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

#include "msg.h"

/**
 * lw6msg_oob_generate_info
 *
 * @info: the node to generate info about
 *
 * Generates a standard response to the INFO question for OOB
 * (out of band) messages. The same message is sent, be it
 * on http or tcp or udp, so it's factorized here. Function
 * will lock the info object when needed.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_oob_generate_info (lw6nod_info_t * info)
{
  char *ret = NULL;
  lw6nod_dyn_info_t *dyn_info = NULL;
  char *level = "";
  int uptime = 0;

  dyn_info = lw6nod_info_dup_dyn (info);
  if (dyn_info)
    {
      uptime =
	(lw6sys_get_timestamp () -
	 info->const_info.creation_timestamp) / 1000;
      if (dyn_info->level)
	{
	  level = dyn_info->level;
	}
      ret =
	lw6sys_new_sprintf
	("Program: %s\nVersion: %s\nCodename: %s\nStamp: %s\nId: %s\nUrl: %s\nTitle: %s\nDescription: %s\nUptime: %d\nLevel: %s\nBench: %d\nRequired bench: %d\nNb-Colors: %d\nMax-Nb-Colors: %d\nNb-Cursors: %d\nMax-Nb-Cursors: %d\nNb-Nodes: %d\nMax-Nb-Nodes: %d\n\n",
	 lw6sys_build_get_package_tarname (), lw6sys_build_get_version (),
	 lw6sys_build_get_codename (), lw6sys_build_get_stamp (),
	 info->const_info.id, info->const_info.url,
	 info->const_info.title, info->const_info.description, uptime,
	 level,
	 info->const_info.bench, dyn_info->required_bench,
	 dyn_info->nb_colors,
	 dyn_info->max_nb_colors,
	 dyn_info->nb_cursors,
	 dyn_info->max_nb_cursors,
	 dyn_info->nb_nodes, dyn_info->max_nb_nodes);
      lw6nod_dyn_info_free (dyn_info);
    }

  return ret;
}

static void
_add_node_txt (void *func_data, void *data)
{
  char **list = (char **) func_data;
  lw6nod_info_t *verified_node = (lw6nod_info_t *) data;
  char *tmp = NULL;

  /*
   * We use this instead of a simple "join with sep=space"
   * on the list object? This is because we can only
   * access it through the map function because of locking issues
   */
  if (list && (*list) && verified_node && verified_node->const_info.url)
    {
      tmp =
	lw6sys_new_sprintf ("%s%s\n", *list, verified_node->const_info.url);
      if (tmp)
	{
	  LW6SYS_FREE (*list);
	  (*list) = tmp;
	}
    }
}

/**
 * lw6msg_oob_generate_list
 *
 * @info: the node to generate info about
 *
 * Generates a standard response to the LIST question for OOB
 * (out of band) messages. The same message is sent, be it
 * on http or tcp or udp, so it's factorized here. Function
 * will lock the info object when needed.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_oob_generate_list (lw6nod_info_t * info)
{
  char *ret = NULL;
  lw6nod_dyn_info_t *dyn_info = NULL;
  char *tmp;

  dyn_info = lw6nod_info_dup_dyn (info);
  if (dyn_info)
    {
      ret = lw6sys_new_sprintf ("");
      if (ret)
	{
	  lw6nod_info_map_verified_nodes (info, _add_node_txt, &ret);
	  if (ret)
	    {
	      tmp = lw6sys_str_concat (ret, "\n");
	      if (tmp)
		{
		  LW6SYS_FREE (ret);
		  ret = tmp;
		}
	    }
	}
      lw6nod_dyn_info_free (dyn_info);
    }

  return ret;
}

/**
 * lw6msg_oob_generate_pong
 *
 * @info: the node to generate info about
 *
 * Generates a standard response to the PING question for OOB
 * (out of band) messages. The same message is sent, be it
 * on http or tcp or udp, so it's factorized here. Function
 * will lock the info object when needed.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_oob_generate_pong (lw6nod_info_t * info)
{
  char *ret = NULL;
  int uptime = 0;

  uptime =
    (lw6sys_get_timestamp () - info->const_info.creation_timestamp) / 1000;
  ret = lw6sys_new_sprintf ("PONG %d\n\n", uptime);

  return ret;
}
