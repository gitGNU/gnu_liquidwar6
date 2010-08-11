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
_lw6p2p_explore_discover_nodes_if_needed (_lw6p2p_node_t * node)
{
  int ret = 0;
  int64_t now;

  now = lw6sys_get_timestamp ();
  if (node->explore.last_discover_nodes_timestamp +
      node->db->data.consts.explore_discover_nodes_delay < now)
    {
      node->explore.last_discover_nodes_timestamp = now;
      ret = _lw6p2p_explore_discover_nodes (node);
    }
  else
    {
      // OK, up to date
      ret = 1;
    }

  return ret;
}

int
_lw6p2p_explore_discover_nodes (_lw6p2p_node_t * node)
{
  int ret = 0;

  // todo
  ret = 1;

  return ret;
}

int
_lw6p2p_explore_verify_nodes_if_needed (_lw6p2p_node_t * node)
{
  int ret = 0;
  int64_t now;

  now = lw6sys_get_timestamp ();
  if (node->explore.last_verify_nodes_timestamp +
      node->db->data.consts.explore_verify_nodes_delay < now)
    {
      node->explore.last_verify_nodes_timestamp = now;
      ret = _lw6p2p_explore_verify_nodes (node);
    }
  else
    {
      // OK, up to date
      ret = 1;
    }

  return ret;
}

int
_lw6p2p_explore_verify_nodes (_lw6p2p_node_t * node)
{
  int ret = 0;

  // todo
  ret = 1;

  return ret;
}
