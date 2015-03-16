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

/**
 * lw6nod_info_community_add
 *
 * @sys_context: global system context
 * @info: node info object to modify
 * @id: ID of the new member
 * @url: URL of the new member, can be NULL
 *
 * Adds a new member to the community.
 *
 * Return value: 1 if new member could be added, 0 if not.
 */
int
lw6nod_info_community_add (lw6sys_context_t * sys_context, lw6nod_info_t * info, u_int64_t id, const char *url)
{
  int ret = 0;
  int i = 0;
  int found_i = -1;

  if (lw6sys_check_id (sys_context, id) && id != info->const_info.ref_info.id_int)
    {
      if (((url != NULL)
	   && (!lw6nod_info_community_is_member (sys_context, info, id, url)))
	  || ((url == NULL) && (!lw6nod_info_community_has_id (sys_context, info, id))) || ((url != NULL)
											    &&
											    (lw6nod_info_community_has_id_without_url
											     (sys_context, info, id))))
	{
	  for (i = 0; i < LW6NOD_MAX_NB_PEERS && found_i < 0; ++i)
	    {
	      if ((!(info->dyn_info.community_peers[i].id_int)) || (info->dyn_info.community_peers[i].id_int == id))
		{
		  found_i = i;
		}
	      if (found_i >= 0)
		{
		  ret = _lw6nod_ref_info_update (sys_context, &(info->dyn_info.community_peers[i]), id, url);
		}
	      else
		{
		  if (url)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
				  _x_ ("can't add node with id=%" LW6SYS_PRINTF_LL "x url=\"%s\", no free slot"), (long long) id, url);
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("can't add node with id=%" LW6SYS_PRINTF_LL "x, no free slot"), (long long) id);
		    }
		}
	    }
	}
      else
	{
	  if (url)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("can't add node with id=%" LW6SYS_PRINTF_LL "x url=\"%s\", already here"), (long long) id, url);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("can't add node with id=%" LW6SYS_PRINTF_LL "x, already here"), (long long) id);
	    }
	}
    }

  return ret;
}

/**
 * lw6nod_info_community_is_member
 *
 * @sys_context: global system context
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
lw6nod_info_community_is_member (lw6sys_context_t * sys_context, lw6nod_info_t * info, u_int64_t id, const char *url)
{
  int ret = 0;

  ret = (_lw6nod_node_info_community_get_by_id (sys_context, info, id) != NULL) || (_lw6nod_node_info_community_get_by_url (sys_context, info, url) != NULL);

  return ret;
}

/**
 * lw6nod_info_community_has_id
 *
 * @sys_context: global system context
 * @info: node info object to test
 * @id: ID of the member we want to check
 *
 * Tells wether a member exists with this ID. Will test both
 * ourselves and remote peers.
 *
 * Return value: 1 if ID is already taken, 0 if available.
 */
int
lw6nod_info_community_has_id (lw6sys_context_t * sys_context, lw6nod_info_t * info, u_int64_t id)
{
  int ret = 0;

  ret = (_lw6nod_node_info_community_get_by_id (sys_context, info, id) != NULL);

  return ret;
}

/**
 * lw6nod_info_community_has_id_without_url
 *
 * @sys_context: global system context
 * @info: node info object to test
 * @id: ID of the member we want to check
 *
 * Tells wether a member exists with this ID, but for which we
 * don't know the URL, that is, url is NULL.
 *
 * Return value: 1 if ID is already taken and has NULL url, 0 else.
 */
int
lw6nod_info_community_has_id_without_url (lw6sys_context_t * sys_context, lw6nod_info_t * info, u_int64_t id)
{
  int ret = 0;
  lw6nod_ref_info_t *ref_info = NULL;

  ref_info = _lw6nod_node_info_community_get_by_id (sys_context, info, id);

  ret = ((ref_info != NULL) && (ref_info->url == NULL));

  return ret;
}

/**
 * lw6nod_info_community_has_url
 *
 * @sys_context: global system context
 * @info: node info object to test
 * @url: URL of the member we want to check
 *
 * Tells wether a member exists with this URL. Will test both
 * ourselves and remote peers.
 *
 * Return value: 1 if URL is already taken, 0 if available.
 */
int
lw6nod_info_community_has_url (lw6sys_context_t * sys_context, lw6nod_info_t * info, const char *url)
{
  int ret = 0;

  ret = (_lw6nod_node_info_community_get_by_url (sys_context, info, url) != NULL);

  return ret;
}

/**
 * lw6nod_info_community_get_id_from_url
 *
 * @sys_context: global system context
 * @info: node info object to test
 * @url: URL of the member we want to check
 *
 * Returns the id of the node with this URL, if it's known to us.
 *
 * Return value: id if it's the community, else 0
 */
int64_t
lw6nod_info_community_get_id_from_url (lw6sys_context_t * sys_context, lw6nod_info_t * info, const char *url)
{
  int64_t ret = 0LL;
  lw6nod_ref_info_t *ref_info = NULL;

  ref_info = _lw6nod_node_info_community_get_by_url (sys_context, info, url);
  if (ref_info)
    {
      ret = ref_info->id_int;
    }

  return ret;
}

/**
 * lw6nod_info_community_get_url_from_id
 *
 * @sys_context: global system context
 * @info: node info object to test
 * @id: ID of the member we want to check
 *
 * Returns the id of the node with this URL, if it's known to us.
 *
 * Return value: url if it's the community else NULL, must be freed
 */
char *
lw6nod_info_community_get_url_from_id (lw6sys_context_t * sys_context, lw6nod_info_t * info, int64_t id)
{
  char *ret = NULL;
  lw6nod_ref_info_t *ref_info = NULL;

  ref_info = _lw6nod_node_info_community_get_by_id (sys_context, info, id);
  if (ref_info && ref_info->url)
    {
      ret = lw6sys_str_copy (sys_context, ref_info->url);
    }

  return ret;
}

/**
 * lw6nod_info_community_remove_by_id
 *
 * @sys_context: global system context
 * @info: node info object to modify
 * @id: ID of the member we want to remove
 *
 * Removes a community member by ID.
 *
 * Return value: 1 if successfully removed, 0 if was not present.
 */
int
lw6nod_info_community_remove_by_id (lw6sys_context_t * sys_context, lw6nod_info_t * info, u_int64_t id)
{
  int ret = 0;
  lw6nod_ref_info_t *ref_info = NULL;

  ref_info = _lw6nod_node_info_community_get_by_id (sys_context, info, id);
  if (ref_info)
    {
      if (ref_info != &(info->const_info.ref_info))
	{
	  _lw6nod_ref_info_reset (sys_context, ref_info);
	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("can't remove peer with id=%" LW6SYS_PRINTF_LL "x url=\"%s\", this is ourselves"), (long long) ref_info->id_int, ref_info->url);
	}
    }

  return ret;
}

/**
 * lw6nod_info_community_remove_by_url
 *
 * @sys_context: global system context
 * @info: node info object to modify
 * @url: URL of the member we want to remove
 *
 * Removes a community member by URL.
 *
 * Return value: 1 if successfully removed, 0 if was not present.
 */
int
lw6nod_info_community_remove_by_url (lw6sys_context_t * sys_context, lw6nod_info_t * info, const char *url)
{
  int ret = 0;
  lw6nod_ref_info_t *ref_info = NULL;

  ref_info = _lw6nod_node_info_community_get_by_url (sys_context, info, url);
  if (ref_info)
    {
      if (ref_info != &(info->const_info.ref_info))
	{
	  _lw6nod_ref_info_reset (sys_context, ref_info);
	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("can't remove peer with id=%" LW6SYS_PRINTF_LL "x url=\"%s\", this is ourselves"), (long long) ref_info->id_int, ref_info->url);
	}
    }

  return ret;
}

/**
 * lw6nod_info_community_count
 *
 * @sys_context: global system context
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
lw6nod_info_community_count (lw6sys_context_t * sys_context, lw6nod_info_t * info)
{
  int ret = 0;
  int i = 0;

  if (info->const_info.ref_info.id_int)
    {
      ++ret;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("strange, our own ID is 0, count will probably wrong"));
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
 * @sys_context: global system context
 * @info: node info object to modify
 *
 * Resets all peers, set community to only one member, ourselves.
 *
 * Return value: none.
 */
void
lw6nod_info_community_reset (lw6sys_context_t * sys_context, lw6nod_info_t * info)
{
  int i = 0;

  for (i = 0; i < LW6NOD_MAX_NB_PEERS; ++i)
    {
      _lw6nod_ref_info_reset (sys_context, &(info->dyn_info.community_peers[i]));
    }
}

/**
 * lw6nod_info_community_get_peer_id_list_str
 *
 * @sys_context: global system context
 * @info: node to query
 *
 * Builds a string containing all peer ids, separated by a
 * separator.
 *
 * Return value: newly allocated string
 */
char *
lw6nod_info_community_get_peer_id_list_str (lw6sys_context_t * sys_context, lw6nod_info_t * info)
{
  char *ret = NULL;
  char *tmp = NULL;
  u_int64_t peer_id = 0LL;
  int i = 0;

  ret = lw6sys_str_copy (sys_context, "");

  for (i = 0; i < LW6NOD_MAX_NB_PEERS && ret; ++i)
    {
      peer_id = info->dyn_info.community_peers[i].id_int;
      if (peer_id)
	{
	  if (strlen (ret) > 0)
	    {
	      tmp = lw6sys_new_sprintf (sys_context, "%s%c%" LW6SYS_PRINTF_LL "x", ret, LW6NOD_PEER_ID_LIST_SEP, (long long) peer_id);
	    }
	  else
	    {
	      tmp = lw6sys_new_sprintf (sys_context, "%" LW6SYS_PRINTF_LL "x", (long long) peer_id);
	    }
	  if (tmp)
	    {
	      LW6SYS_FREE (sys_context, ret);
	      ret = tmp;
	      tmp = NULL;
	    }
	}
    }

  return ret;
}

/**
 * lw6nod_info_community_set_peer_id_list_str
 *
 * @sys_context: global system context
 * @info: node to modify
 * @peer_id_list_str: new value
 *
 * Interprets a peer_id_list_str and puts it into the node data
 * structures. Note that this function won't really copy the
 * list, instead it will populate the dyn_info struct with
 * the right values.
 *
 * Return value: none
 */
void
lw6nod_info_community_set_peer_id_list_str (lw6sys_context_t * sys_context, lw6nod_info_t * info, const char *peer_id_list_str)
{
  char *pos = NULL;
  char *tmp = NULL;
  char *end = NULL;
  u_int64_t peer_id = 0LL;
  int done = 0;

  tmp = lw6sys_str_copy (sys_context, peer_id_list_str);
  if (tmp)
    {
      pos = tmp;
      while ((!done) && pos[0])
	{
	  end = strchr (pos, LW6NOD_PEER_ID_LIST_SEP);
	  if (end == NULL)
	    {
	      done = 1;
	    }
	  else
	    {
	      end[0] = '\0';
	    }
	  peer_id = lw6sys_id_atol (sys_context, pos);
	  if (lw6sys_check_id (sys_context, peer_id))
	    {
	      if (peer_id != info->const_info.ref_info.id_int)
		{
		  if (lw6nod_info_community_has_id (sys_context, info, peer_id))
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("peer_id \"%" LW6SYS_PRINTF_LL "x\" already in the community"), (long long) peer_id);
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("adding peer_id \"%" LW6SYS_PRINTF_LL "x\" to the community"), (long long) peer_id);
		      /*
		       * Calling it with a NULL url, we might have received the URL from a peer
		       * but we don't care until we are *really* connected to this host and
		       * have its real URL.
		       */
		      lw6nod_info_community_add (sys_context, info, peer_id, NULL);
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("not adding %" LW6SYS_PRINTF_LL "x, this is ourselves"), (long long) peer_id);
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad id \"%s\" in peer_id_list_str \"%s\""), pos, peer_id_list_str);
	    }
	  pos = end + 1;
	}
      LW6SYS_FREE (sys_context, tmp);
    }
}

/**
 * lw6nod_info_community_id_without_url_map
 *
 * @sys_context: global system context
 * @info: node to process
 * @func: function to use as a callback
 * @func_data: data passed along with the function
 *
 * Applies function func to all the members of the community
 * which have an id but not an URL...
 *
 * Return value: none.
 */
void
lw6nod_info_community_id_without_url_map (lw6sys_context_t * sys_context, lw6nod_info_t * info, lw6nod_id_callback_func_t func, void *func_data)
{
  u_int64_t peer_id = 0LL;
  int i = 0;

  for (i = 0; i < LW6NOD_MAX_NB_PEERS; ++i)
    {
      peer_id = info->dyn_info.community_peers[i].id_int;
      if (peer_id && !(info->dyn_info.community_peers[i].url))
	{
	  func (sys_context, func_data, peer_id);
	}
    }
}

lw6nod_ref_info_t *
_lw6nod_node_info_community_get_by_id (lw6sys_context_t * sys_context, lw6nod_info_t * node_info, u_int64_t id)
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
_lw6nod_node_info_community_get_by_url (lw6sys_context_t * sys_context, lw6nod_info_t * node_info, const char *url)
{
  lw6nod_ref_info_t *ret = NULL;
  int i = 0;

  if (lw6sys_str_is_same (sys_context, node_info->const_info.ref_info.url, url))
    {
      ret = &(node_info->const_info.ref_info);
    }
  if (!ret)
    {
      for (i = 0; i < LW6NOD_MAX_NB_PEERS && !ret; ++i)
	{
	  if (lw6sys_str_is_same (sys_context, node_info->dyn_info.community_peers[i].url, url))
	    {
	      ret = &(node_info->dyn_info.community_peers[i]);
	    }
	}
    }

  return ret;
}
