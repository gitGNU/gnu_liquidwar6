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
_lw6ker_node_array_init (lw6sys_context_t * sys_context, _lw6ker_node_array_t * node_array)
{
  int i = 0;

  node_array->nb_nodes = 0;
  for (i = 0; i < LW6MAP_MAX_NB_NODES; ++i)
    {
      _lw6ker_node_init (sys_context, &(node_array->nodes[i]));
    }
}

void
_lw6ker_node_array_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_node_array_t * node_array, u_int32_t * checksum)
{
  int i = 0;

  lw6sys_checksum_update_int32 (sys_context, checksum, node_array->nb_nodes);
  for (i = 0; i < LW6MAP_MAX_NB_NODES; ++i)
    {
      _lw6ker_node_update_checksum (sys_context, &(node_array->nodes[i]), checksum);
    }
}

_lw6ker_node_t *
_lw6ker_node_array_find_free (lw6sys_context_t * sys_context, _lw6ker_node_array_t * node_array)
{
  _lw6ker_node_t *ret = NULL;
  int i;

  for (i = 0; i < LW6MAP_MAX_NB_NODES && !ret; ++i)
    {
      if (!node_array->nodes[i].enabled)
	{
	  ret = &(node_array->nodes[i]);
	}
    }

  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("unable to find free node"));
    }

  return ret;
}

void
_lw6ker_node_array_reset (lw6sys_context_t * sys_context, _lw6ker_node_array_t * node_array)
{
  int i;

  node_array->nb_nodes = 0;
  for (i = 0; i < LW6MAP_MAX_NB_NODES; ++i)
    {
      _lw6ker_node_reset (sys_context, &(node_array->nodes[i]));
    }
}

_lw6ker_node_t *
_lw6ker_node_array_get_rw (lw6sys_context_t * sys_context, _lw6ker_node_array_t * node_array, u_int64_t node_id)
{
  _lw6ker_node_t *ret = NULL;
  int i;

  for (i = 0; i < LW6MAP_MAX_NB_NODES; ++i)
    {
      if (node_array->nodes[i].node_id == node_id)
	{
	  ret = &(node_array->nodes[i]);
	}
    }

  return ret;
}

const _lw6ker_node_t *
_lw6ker_node_array_get_ro (lw6sys_context_t * sys_context, const _lw6ker_node_array_t * node_array, u_int64_t node_id)
{
  const _lw6ker_node_t *ret = NULL;
  int i;

  for (i = 0; i < LW6MAP_MAX_NB_NODES; ++i)
    {
      if (node_array->nodes[i].node_id == node_id)
	{
	  ret = &(node_array->nodes[i]);
	}
    }

  return ret;
}

int
_lw6ker_node_array_enable (lw6sys_context_t * sys_context, _lw6ker_node_array_t * node_array, u_int64_t node_id)
{
  int ret = 0;
  _lw6ker_node_t *node;

  node = _lw6ker_node_array_get_rw (sys_context, node_array, node_id);
  if (!node)
    {
      node = _lw6ker_node_array_find_free (sys_context, node_array);
      if (node)
	{
	  _lw6ker_node_enable (sys_context, node, node_id);
	  node_array->nb_nodes++;
	  ret = 1;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("node %" LW6SYS_PRINTF_LL "x already exists, can't enable it twice"), (long long) node_id);
    }

  return ret;
}

int
_lw6ker_node_array_disable (lw6sys_context_t * sys_context, _lw6ker_node_array_t * node_array, u_int64_t node_id)
{
  int ret = 0;
  _lw6ker_node_t *node;

  node = _lw6ker_node_array_get_rw (sys_context, node_array, node_id);
  if (node)
    {
      _lw6ker_node_disable (sys_context, node);
      node_array->nb_nodes--;
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("node %" LW6SYS_PRINTF_LL "x already does not exist, can't disable it"), (long long) node_id);
    }

  return ret;
}

int
_lw6ker_node_array_sanity_check (lw6sys_context_t * sys_context, const _lw6ker_node_array_t * node_array, const lw6map_rules_t * rules)
{
  int ret = 1;
  int i;
  int found = 0;

  for (i = 0; i < LW6MAP_MAX_NB_NODES; ++i)
    {
      ret = ret && _lw6ker_node_sanity_check (sys_context, &(node_array->nodes[i]), rules);
      if (node_array->nodes[i].enabled)
	{
	  found++;
	}
    }
  if (found != node_array->nb_nodes)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("inconsistency in node_array, found %d nodes but array reports %d"), found, node_array->nb_nodes);
      ret = 0;
    }

  return ret;
}
