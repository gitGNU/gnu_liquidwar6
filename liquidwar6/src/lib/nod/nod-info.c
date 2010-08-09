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

#include "nod.h"
#include "nod-internal.h"

/**
 * lw6nod_info_new
 *
 * @id: the node id
 * @url: the node public url
 * @title: the node title
 * @description: the node description
 * @bench: the node bench
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
lw6nod_info_new (u_int64_t id, char *url, char *title, char *description,
		 int bench, int idle_screenshot_size,
		 void *idle_screenshot_data)
{
  lw6nod_info_t *info = NULL;
  int const_init_ret = 0;

  info = (lw6nod_info_t *) LW6SYS_CALLOC (sizeof (lw6nod_info_t));
  if (info)
    {
      info->mutex = lw6sys_mutex_create ();

      const_init_ret =
	_lw6nod_const_info_init (&(info->const_info), id, url, title,
				 description, bench, idle_screenshot_size,
				 idle_screenshot_data);
      lw6nod_info_idle (info);
      info->discovered_nodes = lw6sys_list_new (lw6sys_free_callback);
      info->verified_nodes = lw6sys_list_new (lw6sys_free_callback);

      if (info->mutex && const_init_ret && info->discovered_nodes
	  && info->verified_nodes)
	{
	  // ok
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("unable to create nod_info object"));
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
      lw6sys_list_free (info->discovered_nodes);
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
 * @level: the name of the current level (map)
 * @required_bench: the bench required to connect
 * @nb_colors: number of colors playing
 * @max_nb_colors: max number of colors allowed
 * @nb_cursors: number of cursors playing
 * @max_nb_cursors: max number of cursors allowed
 * @nb_nodes: number of nodes playing
 * @max_nb_nodes: max number of nodes allowed
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
lw6nod_info_update (lw6nod_info_t * info, char *level,
		    int required_bench,
		    int nb_colors,
		    int max_nb_colors,
		    int nb_cursors,
		    int max_nb_cursors,
		    int nb_nodes,
		    int max_nb_nodes,
		    int game_screenshot_size, void *game_screenshot_data)
{
  int ret = 0;

  if (lw6nod_info_lock (info))
    {
      ret =
	_lw6nod_dyn_info_update (&(info->dyn_info), level, required_bench,
				 nb_colors, max_nb_colors, nb_cursors,
				 max_nb_cursors, nb_nodes, max_nb_nodes,
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

  dyn_info = (lw6nod_dyn_info_t *) LW6SYS_CALLOC (sizeof (lw6nod_dyn_info_t));

  if (dyn_info)
    {
      if (lw6nod_info_lock (info))
	{
	  if (_lw6nod_dyn_info_update
	      (dyn_info, info->dyn_info.level, info->dyn_info.required_bench,
	       info->dyn_info.nb_colors, info->dyn_info.max_nb_colors,
	       info->dyn_info.nb_cursors, info->dyn_info.max_nb_cursors,
	       info->dyn_info.nb_nodes, info->dyn_info.max_nb_nodes,
	       info->dyn_info.game_screenshot_size,
	       info->dyn_info.game_screenshot_data))
	    {
	      //OK
	    }
	  else
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
lw6nod_info_add_discovered_node (lw6nod_info_t * info, char *public_url)
{
  int ret = 0;
  lw6sys_list_t *list = NULL;
  int exists = 0;
  char *copy_of_url;

  if (lw6nod_info_lock (info))
    {
      if (!info->discovered_nodes)
	{
	  // could be NULL if popping too hard
	  info->discovered_nodes = lw6sys_list_new (lw6sys_free_callback);
	}
      if (info->discovered_nodes)
	{
	  list = info->discovered_nodes;
	  while (list && !exists)
	    {
	      if (list->data && strcmp ((char *) list->data, public_url) == 0)
		{
		  exists = 1;
		}
	      list = list->next_item;
	    }
	  if (!exists)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _("discovered \"%s\""),
			  public_url);
	      copy_of_url = lw6sys_str_copy (public_url);
	      if (copy_of_url)
		{
		  lw6sys_fifo_push (&(info->discovered_nodes),
				    (void *) copy_of_url);
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _("discovered \"%s\" but it was already pending"),
			  public_url);
	    }
	}
      ret = ((info->discovered_nodes) != NULL);
      lw6nod_info_unlock (info);
    }

  return ret;
}

/**
 * lw6nod_info_pop_discovered_node
 *
 * @info: the node info to query
 *
 * Gets the next discovered node, one would typically call this
 * until it returns NULL to test all of the nodes. When it
 * returns NULL, the list is empty, will be populated (or not)
 * later when new servers connect.
 *
 * Return value: the public URL of a node, or NULL if empty list
 */
char *
lw6nod_info_pop_discovered_node (lw6nod_info_t * info)
{
  char *ret = NULL;

  if (lw6nod_info_lock (info))
    {
      if (info->discovered_nodes)
	{
	  ret = (char *) lw6sys_fifo_pop (&(info->discovered_nodes));
	}
      lw6nod_info_unlock (info);
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

  if (lw6nod_info_lock (info))
    {
      if (info->verified_nodes)
	{
	  lw6sys_list_free (info->verified_nodes);
	}
      info->verified_nodes = list;
      lw6nod_info_unlock (info);
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
 * Calls @lw6sys_list_map on every member of the list of verified
 * ndoes. The reason there's a function for this is that it is
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

/**
 * lw6nod_info_generate_oob_info
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
lw6nod_info_generate_oob_info (lw6nod_info_t * info)
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
  char *url = (char *) data;
  char *tmp = NULL;

  /*
   * We use this instead of a simple "join with sep=space"
   * on the list object? This is because we can only
   * access it through the map function because of locking issues
   */
  if (list && (*list) && url)
    {
      tmp = lw6sys_new_sprintf ("%s%s\n", *list, url);
      if (tmp)
	{
	  LW6SYS_FREE (*list);
	  (*list) = tmp;
	}
    }
}

/**
 * lw6nod_info_generate_oob_list
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
lw6nod_info_generate_oob_list (lw6nod_info_t * info)
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
