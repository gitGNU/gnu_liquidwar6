/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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
 * lw6nod_info_new
 *
 * @program: the program (normally it's liquidwar6)
 * @version: the version
 * @codename: the codename
 * @stamp: the stamp
 * @id: the node id
 * @url: the node public url
 * @title: the node title
 * @description: the node description
 * @password: the node password
 * @bench: the node bench
 * @open_relay: open relay or not
 * @uptime: uptime in seconds
 * @idle_screenshot_size: the size (bytes) of the image to display when game is idle
 * @idle_screenshot_data: the data (jpeg) of the image to display when game is idle
 *
 * Creates a node info object. The arguments correspond to the immutable node
 * attributes, other properties such as how many players are connected or
 * set in other functions like @lw6nod_info_update which can be called later.
 *
 * Return value: newly allocated object, NULL on error.
 */
lw6nod_info_t *
lw6nod_info_new (const char *program,
		 const char *version,
		 const char *codename,
		 int stamp, u_int64_t id, const char *url, const char *title,
		 const char *description, const char *password, int bench,
		 int open_relay, int uptime, int idle_screenshot_size,
		 void *idle_screenshot_data)
{
  lw6nod_info_t *info = NULL;
  int const_init_ret = 0;

  info = (lw6nod_info_t *) LW6SYS_CALLOC (sizeof (lw6nod_info_t));
  if (info)
    {
      info->mutex = lw6sys_mutex_create ();

      const_init_ret =
	_lw6nod_const_info_init (&(info->const_info), program, version,
				 codename, stamp, id, url, title, description,
				 password, bench, open_relay, uptime,
				 idle_screenshot_size, idle_screenshot_data);
      lw6nod_info_idle (info);
      info->discovered_nodes = lw6nod_info_new_discovered_nodes ();
      info->verified_nodes = lw6nod_info_new_verified_nodes ();
      if (info->mutex && const_init_ret && info->discovered_nodes
	  && info->verified_nodes)
	{
	  // ok
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("unable to create nod_info object"));
	  lw6nod_info_free (info);
	  info = NULL;
	}
    }

  return info;
}

/**
 * lw6nod_info_free
 *
 * @info: the node info to free
 *
 * Frees a node info object.
 *
 * Return value: none
 */
void
lw6nod_info_free (lw6nod_info_t * info)
{
  lw6nod_info_idle (info);
  if (info->dyn_info.level)
    {
      LW6SYS_FREE (info->dyn_info.level);
    }
  if (info->mutex)
    {
      lw6sys_mutex_destroy (info->mutex);
    }
  _lw6nod_const_info_reset (&(info->const_info));
  if (info->discovered_nodes)
    {
      lw6sys_hash_free (info->discovered_nodes);
    }
  if (info->verified_nodes)
    {
      lw6sys_list_free (info->verified_nodes);
    }
  LW6SYS_FREE (info);
}

/**
 * lw6nod_info_lock
 *
 * @info: the node info to lock
 *
 * Locks a node info object, this is usefull for some members, typically
 * list of servers, can be accessed by separated threads, one reading,
 * many writing, and these objects (chained lists) certainly do not
 * want to be modified while being read.
 *
 * Return value: 1 if ok, 0 if not.
 */
int
lw6nod_info_lock (lw6nod_info_t * info)
{
  int ret = 0;

  ret = lw6sys_mutex_lock (info->mutex);

  return ret;
}

/**
 * lw6nod_info_unlock
 *
 * @info: the node info to unlock
 *
 * Unlocks a node info object, this is the compation of
 * the @lw6nod_info_lock function.
 *
 * Return value: 1 if ok, 0 if not.
 */
int
lw6nod_info_unlock (lw6nod_info_t * info)
{
  int ret = 0;

  ret = lw6sys_mutex_unlock (info->mutex);

  return ret;
}

/**
 * lw6nod_info_idle
 *
 * @info: the node info to modify
 *
 * Clears a node info object and sets all its variable attributes
 * to NULL/default values. This is what we want when the node is idle,
 * not playing.
 *
 * Return value: none.
 */
void
lw6nod_info_idle (lw6nod_info_t * info)
{
  int locked = 0;

  /*
   * Here we test for mutex before locking for it could
   * be called from the free function, or on creation
   */
  if (info->mutex)
    {
      locked = lw6nod_info_lock (info);
    }

  _lw6nod_dyn_info_reset (&(info->dyn_info));

  if (info->mutex && locked)
    {
      lw6nod_info_unlock (info);
    }
}

/**
 * lw6nod_info_update
 *
 * @info: the node info to update
 * @community_id: the id of the community the node belongs to
 * @round: the current round (can have an offset with real round number)
 * @level: the name of the current level (map)
 * @required_bench: the bench required to connect
 * @nb_colors: number of colors playing
 * @max_nb_colors: max number of colors allowed
 * @nb_cursors: number of cursors playing
 * @max_nb_cursors: max number of cursors allowed
 * @nb_nodes: number of nodes playing
 * @max_nb_nodes: max number of nodes allowed
 * @peer_id_list: list of peers ids, can be NULL
 * @game_screenshot_size: size of screenshot (bytes)
 * @game_screenshot_data: screenshot data (byte buffer, contains JPEG)
 *
 * Set a node info object variable attributes. Call this whenever
 * the node has changed some parameter. Not too often for it's not
 * needed and some operations such as modying the screenshot,
 * can be time consuming.
 *
 * Return value: 1 if OK, 0 if error.
 */
int
lw6nod_info_update (lw6nod_info_t * info, u_int64_t community_id, int round,
		    const char *level, int required_bench, int nb_colors,
		    int max_nb_colors, int nb_cursors, int max_nb_cursors,
		    int nb_nodes, int max_nb_nodes, const char *peer_id_list,
		    int game_screenshot_size, void *game_screenshot_data)
{
  int ret = 0;

  if (lw6nod_info_lock (info))
    {
      ret =
	_lw6nod_dyn_info_update (&(info->dyn_info), community_id, round,
				 level, required_bench, nb_colors,
				 max_nb_colors, nb_cursors, max_nb_cursors,
				 nb_nodes, max_nb_nodes,
				 game_screenshot_size, game_screenshot_data);
      lw6nod_info_unlock (info);
    }

  return ret;
}

/**
 * lw6nod_info_dup_dyn
 *
 * @info: the node info containing the dyn info to duplicate
 *
 * Extracts the dynamic part of an info struct and duplicates
 * it, this is to avoid protection fault error when concurrent
 * threads access this info.
 *
 * Return value: newly allocated object, must be freed.
 */
lw6nod_dyn_info_t *
lw6nod_info_dup_dyn (lw6nod_info_t * info)
{
  lw6nod_dyn_info_t *dyn_info = NULL;
  int ok = 1;
  int i = 0;

  dyn_info = (lw6nod_dyn_info_t *) LW6SYS_CALLOC (sizeof (lw6nod_dyn_info_t));

  if (dyn_info)
    {
      if (lw6nod_info_lock (info))
	{
	  ok = _lw6nod_dyn_info_update
	    (dyn_info, info->dyn_info.community_id_int,
	     info->dyn_info.round, info->dyn_info.level,
	     info->dyn_info.required_bench, info->dyn_info.nb_colors,
	     info->dyn_info.max_nb_colors, info->dyn_info.nb_cursors,
	     info->dyn_info.max_nb_cursors, info->dyn_info.nb_nodes,
	     info->dyn_info.max_nb_nodes,
	     info->dyn_info.game_screenshot_size,
	     info->dyn_info.game_screenshot_data) && ok;
	  for (i = 0; i < LW6NOD_MAX_NB_PEERS; ++i)
	    {
	      ok =
		_lw6nod_ref_info_update (&(dyn_info->community_peers[i]),
					 info->dyn_info.
					 community_peers[i].id_int,
					 info->dyn_info.
					 community_peers[i].url) && ok;
	    }
	  if (!ok)
	    {
	      lw6nod_dyn_info_free (dyn_info);
	      dyn_info = NULL;
	    }
	  lw6nod_info_unlock (info);
	}
    }

  return dyn_info;
}

/**
 * lw6nod_info_new_discovered_nodes
 *
 * Creates a new hash, to be used as a discovered nodes list.
 * Using this function has the
 * advantage of setting the hash options to their defaults.
 * We use a hash to avoid having uselessly long lists containing
 * always the same node due to multiple detections.
 *
 * Return value: an empty hash
 */
lw6sys_hash_t *
lw6nod_info_new_discovered_nodes ()
{
  lw6sys_hash_t *ret;

  ret = lw6sys_hash_new (NULL, _LW6NOD_HASH_SIZE);

  return ret;
}

/**
 * lw6nod_info_add_discovered_node
 *
 * @info: the node info to update
 * @public_url: the address of the discovered node
 *
 * Registers a new server, and queues it as something that should
 * be checked later because it's interesting. We can't insert in the
 * database all the servers we suspect to exist so network threads
 * should use this, then main thread will process discovered servers
 * afterwards. This is also a good way to avoid trivial DOS attacks.
 *
 * Return value: 1 if OK, O if error.
 */
int
lw6nod_info_add_discovered_node (lw6nod_info_t * info, const char *public_url)
{
  int ret = 0;
  char *canonized_url;

  if (strlen (public_url) > 0)
    {
      if (lw6nod_info_lock (info))
	{
	  if (!info->discovered_nodes)
	    {
	      // could be NULL if popping too hard
	      info->discovered_nodes = lw6nod_info_new_discovered_nodes ();
	    }
	  if (info->discovered_nodes)
	    {
	      canonized_url = lw6sys_url_canonize (public_url);
	      if (canonized_url)
		{
		  lw6sys_log (LW6SYS_LOG_DEBUG,
			      _x_ ("adding \"%s\" as a possible node"),
			      canonized_url);
		  lw6sys_hash_set (info->discovered_nodes, canonized_url,
				   NULL);
		  LW6SYS_FREE (canonized_url);
		}
	    }
	  ret = ((info->discovered_nodes) != NULL);
	  lw6nod_info_unlock (info);
	}
    }

  return ret;
}

/**
 * lw6nod_info_pop_discovered_nodes
 *
 * @info: the node info to query
 *
 * Returns a list of all discovered nodes (their public URL)
 * and empties the current queue as well.
 *
 * Return value: a list of dynamically allocated strings.
 */
lw6sys_list_t *
lw6nod_info_pop_discovered_nodes (lw6nod_info_t * info)
{
  lw6sys_list_t *ret = NULL;

  if (lw6nod_info_lock (info))
    {
      if (info->discovered_nodes)
	{
	  ret = lw6sys_hash_keys (info->discovered_nodes);
	  lw6sys_hash_free (info->discovered_nodes);
	}
      else
	{
	  ret = lw6sys_list_new (lw6sys_free_callback);
	}
      info->discovered_nodes = lw6nod_info_new_discovered_nodes ();
      lw6nod_info_unlock (info);
    }

  return ret;
}

/**
 * lw6nod_info_new_verified_nodes
 *
 * Creates a new list, to be filled with nodes and typically passed
 * to @lw6nod_info_set_verified_nodes. Using this function has the
 * advantage of setting the listh options to their defaults.
 *
 * Return value: an empty list
 */
lw6sys_list_t *
lw6nod_info_new_verified_nodes ()
{
  lw6sys_list_t *ret;

  ret = lw6sys_list_new ((lw6sys_free_func_t) lw6nod_info_free);

  return ret;
}

static int
_verified_sort_callback (const lw6sys_list_t ** list_a,
			 const lw6sys_list_t ** list_b)
{
  int ret = 0;
  const lw6nod_info_t *a;
  const lw6nod_info_t *b;

  a = (const lw6nod_info_t *) ((*list_a)->data);
  b = (const lw6nod_info_t *) ((*list_b)->data);

  if (a->const_info.title && b->const_info.title)
    {
      ret = strcasecmp (a->const_info.title, b->const_info.title);
    }
  else if (a->const_info.ref_info.url && b->const_info.ref_info.url)
    {
      ret = strcmp (a->const_info.ref_info.url, b->const_info.ref_info.url);
    }

  return ret;
}

/**
 * lw6nod_info_set_verified_nodes
 *
 * @info: the node info to modify
 * @list: the list of verified nodes, will be freed by this function
 *
 * Sets the list of verified nodes, that is, the list of nodes
 * we are sure to exist, this is typically the list we will
 * display later on a web page. We can't directly display
 * any discovered node, one needs to filter them through main thread.
 * Something very important about this function is that @list will
 * be freed by function, that is, if you call this, then you can
 * (you should) forget your object, it will disappear any time soon.
 *
 * Return value: 1 if OK, 0 on error.
 */
int
lw6nod_info_set_verified_nodes (lw6nod_info_t * info, lw6sys_list_t * list)
{
  int ret = 0;

  lw6sys_sort (&list, _verified_sort_callback);

  if (list)
    {
      if (lw6nod_info_lock (info))
	{
	  if (info->verified_nodes)
	    {
	      lw6sys_list_free (info->verified_nodes);
	    }
	  info->verified_nodes = list;
	  lw6nod_info_unlock (info);
	}
    }

  return ret;
}

/**
 * lw6nod_info_map_verified_nodes
 *
 * @info: the node info concerned
 * @func: the function to apply
 * @func_data: arbitrary pointer holding data to pass to function
 *
 * Calls @lw6sys_hash_map with @func on every member of the list of verified
 * nodes. The reason there's a function for this is that it is
 * very important that list object is locked when doing this.
 * This function does perform a lock/unlock so it is safe.
 *
 * Return value: none.
 */
void
lw6nod_info_map_verified_nodes (lw6nod_info_t * info,
				lw6sys_list_callback_func_t func,
				void *func_data)
{
  if (lw6nod_info_lock (info))
    {
      if (info->verified_nodes)
	{
	  lw6sys_list_map (info->verified_nodes, func, func_data);
	}
      lw6nod_info_unlock (info);
    }
}
