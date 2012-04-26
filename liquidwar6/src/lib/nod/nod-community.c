/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

/**
 * lw6nod_info_community_add
 *
 * @info: node info object to modify
 * @id: ID of the new member
 * @url: URL of the new member
 *
 * Adds a new member to the community.
 *
 * Return value: 1 if new member could be added, 0 if not.
 */
int
lw6nod_info_community_add (lw6nod_info_t * info, u_int64_t id,
			   const char *url)
{
  int ret = 0;
  int i = 0;
  int found_i = -1;

  if (!lw6nod_info_community_is_member (info, id, url))
    {
      for (i = 0; i < LW6NOD_MAX_NB_PEERS && found_i < 0; ++i)
	{
	  if (!(info->dyn_info.community_peers[i].id_int))
	    {
	      found_i = i;
	    }
	  if (found_i >= 0)
	    {
	      ret =
		_lw6nod_ref_info_update (&(info->dyn_info.community_peers[i]),
					 id, url);
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _x_ ("can't add node with id=%" LW6SYS_PRINTF_LL
			       "x url=\"%s\", already here, no free slot"),
			  id, url);
	    }
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("can't add node with id=%" LW6SYS_PRINTF_LL
		       "x url=\"%s\", already here, already here"), id, url);
    }

  return ret;
}

/**
 * lw6nod_info_community_is_member
 *
 * @info: node info object to test
 * @id: ID of the member we want to check
 * @url: URL of the member we want to check
 *
 * Tells wether a member is already in the community. Note that if there's
 * a member with the same URL but with a different ID, or a member with the
 * same ID but a different URL, the function will fail, we need URLs and IDs
 * to both be different for the peer to be added. Not respecting this would
 * lead to confusion, while sharing an ID is conceivable over the whole
 * network, it can't be tolerated within a community. Same for the URL.
 *
 * Return value: 1 if new member could be added, 0 if not.
 */
int
lw6nod_info_community_is_member (lw6nod_info_t * info, u_int64_t id,
				 const char *url)
{
  int ret = 0;

  ret = (_lw6nod_node_info_community_get_by_id (info, id) != NULL) ||
    (_lw6nod_node_info_community_get_by_url (info, url) != NULL);

  return ret;
}

/**
 * lw6nod_info_community_has_id
 *
 * @info: node info object to test
 * @id: ID of the member we want to check
 *
 * Tells wether a member exists with this ID. Will test both
 * ourselves and remote peers.
 *
 * Return value: 1 if ID is already taken, 0 if available.
 */
int
lw6nod_info_community_has_id (lw6nod_info_t * info, u_int64_t id)
{
  int ret = 0;

  ret = (_lw6nod_node_info_community_get_by_id (info, id) != NULL);

  return ret;
}

/**
 * lw6nod_info_community_has_url
 *
 * @info: node info object to test
 * @url: URL of the member we want to check
 *
 * Tells wether a member exists with this URL. Will test both
 * ourselves and remote peers.
 *
 * Return value: 1 if URL is already taken, 0 if available.
 */
int
lw6nod_info_community_has_url (lw6nod_info_t * info, const char *url)
{
  int ret = 0;

  ret = (_lw6nod_node_info_community_get_by_url (info, url) != NULL);

  return ret;
}

/**
 * lw6nod_info_community_remove_by_id
 *
 * @info: node info object to modify
 * @id: ID of the member we want to remove
 *
 * Removes a community member by ID.
 *
 * Return value: 1 if successfully removed, 0 if was not present.
 */
int
lw6nod_info_community_remove_by_id (lw6nod_info_t * info, u_int64_t id)
{
  int ret = 0;
  lw6nod_ref_info_t *ref_info = NULL;

  ref_info = _lw6nod_node_info_community_get_by_id (info, id);
  if (ref_info)
    {
      if (ref_info != &(info->const_info.ref_info))
	{
	  _lw6nod_ref_info_reset (ref_info);
	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("can't remove peer with id=%" LW6SYS_PRINTF_LL
			   "x url=\"%s\", this is ourselves"),
		      ref_info->id_int, ref_info->url);
	}
    }

  return ret;
}

/**
 * lw6nod_info_community_remove_by_url
 *
 * @info: node info object to modify
 * @url: URL of the member we want to remove
 *
 * Removes a community member by URL.
 *
 * Return value: 1 if successfully removed, 0 if was not present.
 */
int
lw6nod_info_community_remove_by_url (lw6nod_info_t * info, const char *url)
{
  int ret = 0;
  lw6nod_ref_info_t *ref_info = NULL;

  ref_info = _lw6nod_node_info_community_get_by_url (info, url);
  if (ref_info)
    {
      if (ref_info != &(info->const_info.ref_info))
	{
	  _lw6nod_ref_info_reset (ref_info);
	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("can't remove peer with id=%" LW6SYS_PRINTF_LL
			   "x url=\"%s\", this is ourselves"),
		      ref_info->id_int, ref_info->url);
	}
    }

  return ret;
}

/**
 * lw6nod_info_community_count
 *
 * @info: node info object to modify
 *
 * Tells how many members there are in a community. This include ourselves
 * so this can never be 0, should at least be 1. Note that this is pretty
 * much the same as the nb_nodes member of dyn_info, but this one is
 * calculated dynamically from peer list, while the other one is updated
 * from time to time from game_state information.
 *
 * Return value: number of community members, including this node (us).
 */
int
lw6nod_info_community_count (lw6nod_info_t * info)
{
  int ret = 0;
  int i = 0;

  if (info->const_info.ref_info.id_int)
    {
      ++ret;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_
		  ("strange, our own ID is 0, count will probably wrong"));
    }

  for (i = 0; i < LW6NOD_MAX_NB_PEERS; ++i)
    {
      if (info->dyn_info.community_peers[i].id_int)
	{
	  ++ret;
	}
    }

  return ret;
}

/**
 * lw6nod_info_community_reset
 *
 * @info: node info object to modify
 *
 * Resets all peers, set community to only one member, ourselves.
 *
 * Return value: none.
 */
void
lw6nod_info_community_reset (lw6nod_info_t * info)
{
  int i = 0;

  for (i = 0; i < LW6NOD_MAX_NB_PEERS; ++i)
    {
      _lw6nod_ref_info_reset (&(info->dyn_info.community_peers[i]));
    }
}

lw6nod_ref_info_t *
_lw6nod_node_info_community_get_by_id (lw6nod_info_t * node_info,
				       u_int64_t id)
{
  lw6nod_ref_info_t *ret = NULL;
  int i = 0;

  if (node_info->const_info.ref_info.id_int == id)
    {
      ret = &(node_info->const_info.ref_info);
    }
  if (!ret)
    {
      for (i = 0; i < LW6NOD_MAX_NB_PEERS && !ret; ++i)
	{
	  if (node_info->dyn_info.community_peers[i].id_int == id)
	    {
	      ret = &(node_info->dyn_info.community_peers[i]);
	    }
	}
    }

  return ret;
}

lw6nod_ref_info_t *
_lw6nod_node_info_community_get_by_url (lw6nod_info_t * node_info,
					const char *url)
{
  lw6nod_ref_info_t *ret = NULL;
  int i = 0;

  if (lw6sys_str_is_same (node_info->const_info.ref_info.url, url))
    {
      ret = &(node_info->const_info.ref_info);
    }
  if (!ret)
    {
      for (i = 0; i < LW6NOD_MAX_NB_PEERS && !ret; ++i)
	{
	  if (lw6sys_str_is_same
	      (node_info->dyn_info.community_peers[i].url, url))
	    {
	      ret = &(node_info->dyn_info.community_peers[i]);
	    }
	}
    }

  return ret;
}
