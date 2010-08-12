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

#include "net.h"
#include "net-internal.h"


int
_lw6net_dns_init ()
{
  int ret = 0;

  _lw6net_global_context->dns_mutex = lw6sys_mutex_create ();
  ret = (_lw6net_global_context->dns_mutex != NULL);

  return ret;
}

void
_lw6net_dns_quit ()
{
  if (_lw6net_global_context->dns_mutex)
    {
      lw6sys_mutex_destroy (_lw6net_global_context->dns_mutex);
      _lw6net_global_context->dns_mutex = NULL;
    }
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
lw6net_dns_is_ip (char *ip)
{
  int ret = 1;
  char *seek = ip;
  int i;

  for (i = 0; (i < 4) && ret; ++i)
    {
      if (_is_digit (*seek))
	{
	  while ((*seek) >= '0' && (*seek) <= '9')
	    {
	      seek++;
	    }
	  if ((*seek) != '.' && (*seek))
	    {
	      ret = 0;
	    }
	  if (*seek)
	    {
	      seek++;
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
 * Return value: an IP if success, 0 on error.
 */
char *
lw6net_dns_gethostbyname (char *name)
{
  char *ret = NULL;
  struct hostent *h;
  struct in_addr addr;
  char *ntoa_ret = NULL;

  if (lw6net_dns_is_ip (name))
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
      ret = lw6sys_str_copy (name);
    }
  else
    {
      if (lw6sys_mutex_lock (_lw6net_global_context->dns_mutex))
	{
	  h = gethostbyname (name);
	  if (h && h->h_addrtype == AF_INET && h->h_length >= 4
	      && h->h_addr_list[0])
	    {
#ifdef UNIX
	      addr = *((struct in_addr *) (h->h_addr_list[0]));
#else
	      addr.s_addr = *((u_long *) (h->h_addr_list[0]));
#endif
	      ntoa_ret = inet_ntoa (addr);
	      if (ntoa_ret)
		{
		  ret = lw6sys_str_copy (ntoa_ret);
		}
	    }
	  lw6sys_mutex_unlock (_lw6net_global_context->dns_mutex);
	}
    }

  return ret;
}
