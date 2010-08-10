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

#include "p2p.h"
#include "p2p-internal.h"

int
_lw6p2p_flush_verified_nodes_if_needed (_lw6p2p_node_t * node)
{
  int ret = 0;
  int64_t now;

  now = lw6sys_get_timestamp ();
  if (node->flush.last_verified_nodes_timestamp +
      node->db->data.consts.flush_verified_nodes_delay < now)
    {
      node->flush.last_verified_nodes_timestamp = now;
      ret = _lw6p2p_flush_verified_nodes (node);
    }
  else
    {
      // OK, up to date
      ret = 1;
    }

  return ret;
}

int
_lw6p2p_flush_verified_nodes (_lw6p2p_node_t * node)
{
  int ret = 0;
  lw6sys_list_t *list;
  lw6sys_hash_t *hash;
  char *url;
  char *title;
  int i;
  lw6nod_info_t *verified_node = NULL;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("flush verified nodes"));

  /*
   * TODO: make this transit through the database and servers
   * be tested for good...
   */
  hash = lw6nod_info_new_verified_nodes ();
  if (hash)
    {
      list = lw6sys_str_split_config_item (node->known_nodes);
      if (list)
	{
	  ret = 1;
	  i = 0;
	  while ((url = lw6sys_list_pop_front (&list)) != NULL)
	    {
	      i++;
	      title = lw6sys_itoa (i);
	      if (title)
		{
		  verified_node =
		    lw6nod_info_new (lw6sys_generate_id_64 (), url, title,
				     "todo...", 10, 0, NULL);
		  if (verified_node)
		    {
		      lw6sys_hash_set (hash, url, verified_node);
		    }
		  LW6SYS_FREE (title);
		}
	      LW6SYS_FREE (url);
	    }
	  if (list)
	    {
	      lw6sys_list_free (list);
	    }
	}
      ret = lw6nod_info_set_verified_nodes (node->node_info, hash);
    }

  return ret;
}

int
_lw6p2p_flush_discovered_nodes_if_needed (_lw6p2p_node_t * node)
{
  int ret = 0;
  int64_t now;

  now = lw6sys_get_timestamp ();
  if (node->flush.last_discovered_nodes_timestamp +
      node->db->data.consts.flush_discovered_nodes_delay < now)
    {
      node->flush.last_discovered_nodes_timestamp = now;
      ret = _lw6p2p_flush_discovered_nodes (node);
    }
  else
    {
      // OK, up to date
      ret = 1;
    }

  return ret;
}

int
_lw6p2p_flush_discovered_nodes (_lw6p2p_node_t * node)
{
  int ret = 0;

  lw6sys_log (LW6SYS_LOG_DEBUG, _("flush discovered nodes"));

  return ret;
}
