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

#include "net.h"
#include "net-internal.h"

int
_lw6net_connectable_init (_lw6net_connectable_t * connectable,
			  int connectable_cache_hash_size)
{
  int ret = 0;

  connectable->connectable_cache_mutex = lw6sys_mutex_create ();
  connectable->connectable_cache =
    lw6sys_hash_new (lw6sys_free_callback, connectable_cache_hash_size);
  ret = (connectable->connectable_cache_mutex != NULL
	 && connectable->connectable_cache != NULL);

  return ret;
}

void
_lw6net_connectable_quit (_lw6net_connectable_t * connectable)
{
  if (connectable->connectable_cache)
    {
      lw6sys_hash_free (connectable->connectable_cache);
    }
  if (connectable->connectable_cache_mutex)
    {
      lw6sys_mutex_destroy (connectable->connectable_cache_mutex);
    }
  memset (connectable, 0, sizeof (_lw6net_connectable_t));
}

/**
 * lw6net_is_connectable
 *
 * @ip: IP address
 * @port: IP port
 *
 * Tells wether we're likely to be able to connect on a given
 * host and port. This is to save ressources, any call to
 * connect which fails will register an entry that says
 * "OK this is rotten, don't try it before some time, you'll
 * waste your ressources trying to do this".
 *
 * Return value: 1 if connectable, 0 if not.
 */
int
lw6net_is_connectable (const char *ip, int port)
{
  int ret = 1;			// by default, connectable
  int64_t *limit_timestamp = NULL;
  char *key = NULL;
  _lw6net_connectable_t *connectable = &(_lw6net_global_context->connectable);

  key = lw6sys_new_sprintf ("%s:%d", ip, port);
  if (key)
    {
      if (lw6sys_mutex_lock (connectable->connectable_cache_mutex))
	{
	  limit_timestamp =
	    lw6sys_hash_get (connectable->connectable_cache, key);
	  if (limit_timestamp)
	    {
	      if ((*limit_timestamp) >= lw6sys_get_timestamp ())
		{
		  /*
		   * Timestamp is enough in the future, so
		   * we can consider this one is *NOT* connectable.
		   */
		  ret = 0;
		}
	      else
		{
		  /*
		   * Second case, timestamp is too old, we remove it,
		   * technically we could keep it, the results would
		   * be the same, but after some time the hash
		   * would be filled with garbage.
		   */
		  lw6sys_hash_unset (connectable->connectable_cache, key);
		  lw6sys_log (LW6SYS_LOG_INFO,
			      _x_ ("destination %s:%d marked as connectable"),
			      ip, port);
		}
	    }
	  lw6sys_mutex_unlock (connectable->connectable_cache_mutex);
	}

      LW6SYS_FREE (key);
    }

  return ret;
}

/**
 * lw6net_set_connectable
 *
 * @ip: IP address
 * @port: IP port
 * @status: status, set to 1 if connectable, 0 if not
 *
 * Registers a destination ip:port as connectable or not.
 * Connectable means, there are chances that a connect on this
 * might return true. The information will be kept in a cache.
 * This is to avoid too many low-level calls to connect,
 * the information will be kept in cache "for some time",
 * and this way connect will return "can't connect" right
 * away and not even try to connect, therefore saving ressources.
 *
 * Return value: none.
 */
void
lw6net_set_connectable (const char *ip, int port, int status)
{
  int64_t *limit_timestamp = NULL;
  char *key = NULL;
  _lw6net_connectable_t *connectable = &(_lw6net_global_context->connectable);


  key = lw6sys_new_sprintf ("%s:%d", ip, port);
  if (key)
    {
      if (lw6sys_mutex_lock (connectable->connectable_cache_mutex))
	{
	  if (status)
	    {
	      /*
	       * The connectable hash actually stores the information
	       * "this destination is *NOT* connectable" so to set
	       * the information to "yes you could connect" one just
	       * needs to remove the entry.
	       */
	      lw6sys_hash_unset (connectable->connectable_cache, key);
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _x_ ("destination %s:%d marked as connectable"), ip,
			  port);
	    }
	  else
	    {
	      limit_timestamp = (int64_t *) LW6SYS_CALLOC (sizeof (int64_t));;
	      if (limit_timestamp)
		{
		  /*
		   * Set an expiration time in the near future, based
		   * on configuration. This way of proceeding will replace
		   * a previous limit_timestamp, so if it was already
		   * marked as unconnectable, it will be marked again.
		   */
		  (*limit_timestamp) =
		    lw6sys_get_timestamp () +
		    _lw6net_global_context->
		    const_data.connectable_cache_delay_sec *
		    LW6SYS_TICKS_PER_SEC;
		  lw6sys_hash_set (connectable->connectable_cache, key,
				   limit_timestamp);
		  lw6sys_log (LW6SYS_LOG_INFO,
			      _x_
			      ("destination %s:%d marked as not connectable"),
			      ip, port);
		}
	    }

	  lw6sys_mutex_unlock (connectable->connectable_cache_mutex);
	}
      LW6SYS_FREE (key);
    }
}
