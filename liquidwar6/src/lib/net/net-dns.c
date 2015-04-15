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
_lw6net_dns_init (lw6sys_context_t * sys_context, _lw6net_dns_t * dns, int dns_cache_hash_size, int dns_cache_delay_sec)
{
  int ret = 0;

  dns->dns_gethostbyname_mutex = LW6SYS_MUTEX_CREATE (sys_context);
  dns->dns_cache_mutex = LW6SYS_MUTEX_CREATE (sys_context);
  dns->dns_cache = lw6sys_cache_new (sys_context, lw6sys_free_callback, dns_cache_hash_size, dns_cache_delay_sec * LW6SYS_TICKS_PER_SEC);
  ret = (dns->dns_gethostbyname_mutex != NULL && dns->dns_cache_mutex != NULL && dns->dns_cache != NULL);

  return ret;
}

void
_lw6net_dns_quit (lw6sys_context_t * sys_context, _lw6net_dns_t * dns)
{
  if (dns->dns_cache)
    {
      lw6sys_cache_free (sys_context, dns->dns_cache);
    }
  if (dns->dns_cache_mutex)
    {
      LW6SYS_MUTEX_DESTROY (sys_context, dns->dns_cache_mutex);
    }
  if (dns->dns_gethostbyname_mutex)
    {
      LW6SYS_MUTEX_DESTROY (sys_context, dns->dns_gethostbyname_mutex);
    }
  memset (dns, 0, sizeof (_lw6net_dns_t));
}

static int
_is_digit (char c)
{
  return (c >= '0') && (c <= '9');
}

/**
 * lw6net_dns_is_ip
 *
 * @ip: the string to check
 *
 * Tests if a given string is a valid IP (IPV4). Test is only syntaxic,
 * it's just to know if we're likely to need to query the DNS, it does not
 * mean the IP is *really* valid.
 *
 * Return value: 1 if it's an IP, O if not.
 */
int
lw6net_dns_is_ip (lw6sys_context_t * sys_context, const char *ip)
{
  int ret = 1;
  const char *pos = ip;
  int i;

  for (i = 0; (i < 4) && ret; ++i)
    {
      if (_is_digit (*pos))
	{
	  while ((*pos) >= '0' && (*pos) <= '9')
	    {
	      pos++;
	    }
	  if ((*pos) != '.' && (*pos))
	    {
	      ret = 0;
	    }
	  if (*pos)
	    {
	      pos++;
	    }
	}
      else
	{
	  ret = 0;
	}
    }

  return ret;
}

/**
 * lw6net_dns_gethostbyname
 *
 * @name: name of the host
 *
 * A wrapper over the standard gethostbyname function, will even accept
 * an IP as an input (in this case, will copy it...) and
 * allocate a new string for the result.
 *
 * Return value: an IP if success, NULL on error.
 */
char *
lw6net_dns_gethostbyname (lw6sys_context_t * sys_context, const char *name)
{
  char *ret = NULL;
  struct hostent *h;
  struct in_addr addr;
  char *ntoa_ret = NULL;
  char *cached_ret = NULL;
  char *to_put_in_cache = NULL;
  _lw6net_dns_t *dns = &(_lw6net_global_context->dns);

  if (lw6net_dns_is_ip (sys_context, name))
    {
      /*
       * Yes, gethostbyname would do the job and *not* convert
       * a valid IP but we would need to acquire the lock...
       * Another solution would be to use gethostbyname_r which
       * is reentrant but it's non-standard, so in the general
       * case we go for the mutex/lock option to avoid race
       * conditions. Ideal implementation would use
       * gethostbyname_r when available.
       */
      ret = lw6sys_str_copy (sys_context, name);
    }
  else
    {
      if (LW6SYS_MUTEX_LOCK (sys_context, dns->dns_cache_mutex))
	{
	  cached_ret = lw6sys_cache_get (sys_context, dns->dns_cache, name);
	  if (cached_ret)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("cached DNS \"%s\" -> \"%s\""), name, cached_ret);
	      ret = lw6sys_str_copy (sys_context, cached_ret);
	    }
	  LW6SYS_MUTEX_UNLOCK (sys_context, dns->dns_cache_mutex);
	}
      /*
       * At this stage cached_ret is NOT NULL if something is in the cache
       * but the pointer could be wrong since we unlocked. In pratice cache
       * entries are never removed, but it's safer to rely on ret which has
       * been copied locally.
       */
      if (!ret)
	{
	  if (lw6net_dns_lock (sys_context))
	    {
	      h = gethostbyname (name);
	      if (h && h->h_addrtype == AF_INET && h->h_length >= 4 && h->h_addr_list[0])
		{
		  addr.s_addr = *((u_long *) (h->h_addr_list[0]));
		  ntoa_ret = inet_ntoa (addr);
		  if (ntoa_ret)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("DNS request \"%s\" -> \"%s\""), name, ntoa_ret);
		      to_put_in_cache = lw6sys_str_copy (sys_context, ntoa_ret);

		      ret = lw6sys_str_copy (sys_context, ntoa_ret);
		    }
		}
	      lw6net_dns_unlock (sys_context);
	    }
	}
    }

  if (to_put_in_cache)
    {
      if (LW6SYS_MUTEX_LOCK (sys_context, dns->dns_cache_mutex))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("put in DNS cache \"%s\" -> \"%s\""), name, to_put_in_cache);
	  lw6sys_cache_set (sys_context, dns->dns_cache, name, to_put_in_cache);
	  LW6SYS_MUTEX_UNLOCK (sys_context, dns->dns_cache_mutex);
	}
    }

  return ret;
}

/**
 * lw6net_dns_lock
 *
 * @sys_context: global system context
 *
 * Locks access to dns function @lw6net_dns_gethostbyname.
 * This is because @gethostbyname isn't reentrant plus, even
 * if we didn't use it but its multithreadable equivalent
 * (which is however not standard and always available)
 * other libs (such as @libcurl not to name it) might use
 * this function too so in a general manner it's a good
 * idea to use a mutex to protect multiple accesses to this.
 *
 * Return value: an IP if success, 0 on error.
 */
int
lw6net_dns_lock (lw6sys_context_t * sys_context)
{
  _lw6net_dns_t *dns = &(_lw6net_global_context->dns);

  return LW6SYS_MUTEX_LOCK (sys_context, dns->dns_gethostbyname_mutex);
}

/**
 * lw6net_dns_unlock
 *
 * @sys_context: global system context
 *
 * Unlocks access to dns function @lw6net_dns_gethostbyname.
 *
 * Return value: an IP if success, 0 on error.
 */
int
lw6net_dns_unlock (lw6sys_context_t * sys_context)
{
  _lw6net_dns_t *dns = &(_lw6net_global_context->dns);

  return LW6SYS_MUTEX_UNLOCK (sys_context, dns->dns_gethostbyname_mutex);
}
