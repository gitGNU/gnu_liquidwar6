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

/*
 * Re-implementation of inet_ntoa (inet_ntoa_r is not standard...)
 * See also: http://www.uclibc.org/[...]/libc/inet/addr.c
 */
char *
_lw6net_inet_ntoa (lw6sys_context_t * sys_context, struct in_addr in)
{
  char *ret = NULL;
  in_addr_t addr = ntohl (in.s_addr);

  ret = lw6sys_new_sprintf (sys_context, "%d.%d.%d.%d", (addr >> 24) & 0xff, (addr >> 16) & 0xff, (addr >> 8) & 0xff, addr & 0xff);

  return ret;
}

/*
 * inet_aton on UNIX and inet_addr on MS-Windows
 */
int
_lw6net_inet_aton (lw6sys_context_t * sys_context, struct in_addr *in, const char *ip)
{
  int ret = 0;

  memset (in, 0, sizeof (struct in_addr));

  /*
   * https://savannah.gnu.org/bugs/?34060
   * "WARNING! inet_addr() failed,..."
   * For some reason on MS-Windows but not on GNU/Linux,
   * trying to translate address 0.0.0.0 raises a warning.
   * We just leave the struct zeroed if we're in this peculiar
   * case, the zeroed struct should be fine.
   */
  if (strcmp (ip, LW6NET_ADDRESS_ANY))
    {
#ifdef LW6_MS_WINDOWS
      if ((in->s_addr = inet_addr (ip)) != 0)
#else
      if (inet_aton (ip, in) != 0)
#endif
	{
	  ret = 1;
	}
      else
	{
#ifdef LW6_MS_WINDOWS
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("inet_addr() failed, ip=\"%s\""), ip);
	  lw6net_last_error (sys_context);
#else
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("inet_aton() failed, ip=\"%s\""), ip);
	  lw6net_last_error (sys_context);
#endif
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("address \"%s\" identified as \"ANY\""), ip);
      ret = 1;
    }

  return ret;
}
