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

#include "sys.h"

/**
 * lw6sys_password_checksum
 *
 * @seed: a seed to blur the password, can be NULL
 * @password: the password, can be NULL
 *
 * Calculates the checksum of a password, and returns
 * it as a string, ready to be sent on the network.
 * If password is empty or NULL, then an empty (but
 * not NULL unless internal error) string will be
 * returned. All LW6 protocols should send
 * these checksums instead of real passwords, then
 * on server side value can be checked against both
 * real password and its checksum. The seed is here
 * so that eavesdropper can't reuse the checksum to
 * connect on random sessions. Seed can typically
 * be the node 'public_url' value.
 *
 * Return value: a dynamically allocated string
 */
char *
lw6sys_password_checksum (char *seed, char *password)
{
  char *ret = NULL;
  u_int32_t checksum = 0;

  if (password && strlen (password) > 0)
    {
      if (seed)
	{
	  checksum = lw6sys_checksum_str (seed);
	}
      lw6sys_checksum_update_str (&checksum, password);
      ret = lw6sys_new_sprintf ("%08x", checksum);
    }
  else
    {
      ret = lw6sys_str_copy ("");
    }

  return ret;
}

/**
 * lw6sys_password_verify
 *
 * @seed: a seed to blur the password, can be NULL
 * @password_here: the local password, can be NULL
 * @password_received: the password received from network, can be NULL
 *
 * Tells wether a password received over the network is valid.
 * The @password_here argument (the local password) will be
 * checksumed so that @password_received is checked against both
 * clear and checksumed values, so it can be in any form.
 *
 * Return value: 1 if OK, passwords are the same, 0 if not.
 */
int
lw6sys_password_verify (char *seed, char *password_here,
			char *password_received)
{
  int ret = 0;
  char *checksum = NULL;

  if ((!password_here) || (password_here && strlen (password_here) == 0))
    {
      // no passwd
      ret = 1;
    }
  if (password_here && strlen (password_here) > 0 && password_received
      && strlen (password_received) > 0)
    {
      if (!(strcmp (password_here, password_received)))
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _("password OK, received as clear text"));
	  ret = 1;
	}
      else
	{
	  checksum = lw6sys_password_checksum (seed, password_here);
	  if (checksum)
	    {
	      if (!(strcmp (checksum, password_received)))
		{
		  lw6sys_log (LW6SYS_LOG_DEBUG,
			      _("password OK, received as checksum"));
		  ret = 1;
		}
	      LW6SYS_FREE (checksum);
	    }
	}
    }

  return ret;
}
