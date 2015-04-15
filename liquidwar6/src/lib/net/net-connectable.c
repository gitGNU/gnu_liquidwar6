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

#include "net.h"
#include "net-internal.h"

int
_lw6net_connectable_init (lw6sys_context_t * sys_context, _lw6net_connectable_t * connectable, int connectable_cache_hash_size,
			  int connectable_cache_delay_sec)
{
  int ret = 0;

  connectable->connectable_cache_mutex = LW6SYS_MUTEX_CREATE (sys_context);
  connectable->connectable_cache = lw6sys_cache_new (sys_context, NULL, connectable_cache_hash_size, connectable_cache_delay_sec * LW6SYS_TICKS_PER_SEC);
  ret = (connectable->connectable_cache_mutex != NULL && connectable->connectable_cache != NULL);

  return ret;
}

void
_lw6net_connectable_quit (lw6sys_context_t * sys_context, _lw6net_connectable_t * connectable)
{
  if (connectable->connectable_cache)
    {
      lw6sys_cache_free (sys_context, connectable->connectable_cache);
    }
  if (connectable->connectable_cache_mutex)
    {
      LW6SYS_MUTEX_DESTROY (sys_context, connectable->connectable_cache_mutex);
    }
  memset (connectable, 0, sizeof (_lw6net_connectable_t));
}

/**
 * lw6net_is_connectable
 *
 * @sys_context: global system context
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
lw6net_is_connectable (lw6sys_context_t * sys_context, const char *ip, int port)
{
  int ret = 1;			// by default, connectable
  char *key = NULL;
  _lw6net_connectable_t *connectable = &(_lw6net_global_context->connectable);

  key = lw6sys_new_sprintf (sys_context, "%s:%d", ip, port);
  if (key)
    {
      if (LW6SYS_MUTEX_LOCK (sys_context, connectable->connectable_cache_mutex))
	{
	  /*
	   * Yes, test is inverted, if there's no key it means we
	   * can connect as we store probable impossibilities
	   * in the cache, and not positive connects.
	   */
	  if (!lw6sys_cache_has_key (sys_context, connectable->connectable_cache, key))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("destination %s:%d marked as connectable"), ip, port);
	      ret = 1;
	    }
	  else
	    {
	      ret = 0;
	    }
	  LW6SYS_MUTEX_UNLOCK (sys_context, connectable->connectable_cache_mutex);
	}

      LW6SYS_FREE (sys_context, key);
    }

  return ret;
}

/**
 * lw6net_set_connectable
 *
 * @sys_context: global system context
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
lw6net_set_connectable (lw6sys_context_t * sys_context, const char *ip, int port, int status)
{
  char *key = NULL;
  _lw6net_connectable_t *connectable = &(_lw6net_global_context->connectable);


  key = lw6sys_new_sprintf (sys_context, "%s:%d", ip, port);
  if (key)
    {
      if (LW6SYS_MUTEX_LOCK (sys_context, connectable->connectable_cache_mutex))
	{
	  if (status)
	    {
	      /*
	       * The connectable hash actually stores the information
	       * "this destination is *NOT* connectable" so to set
	       * the information to "yes you could connect" one just
	       * needs to remove the entry.
	       */
	      lw6sys_cache_unset (sys_context, connectable->connectable_cache, key);
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("destination %s:%d marked as connectable"), ip, port);
	    }
	  else
	    {
	      lw6sys_cache_set (sys_context, connectable->connectable_cache, key, NULL);
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("destination %s:%d marked as not connectable"), ip, port);
	    }

	  LW6SYS_MUTEX_UNLOCK (sys_context, connectable->connectable_cache_mutex);
	}
      LW6SYS_FREE (sys_context, key);
    }
}
