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

#include "p2p.h"
#include "p2p-internal.h"

#define _DEFAULT_PASSWORD_CHECKSUM "PUB"

int
_lw6p2p_tentacle_init (_lw6p2p_tentacle_t * tentacle,
		       _lw6p2p_backends_t * backends, char *remote_url,
		       char *password, u_int64_t local_id,
		       u_int64_t remote_id)
{
  int ret = 1;
  int i = 0;
  //_lw6p2p_tentacle_clear(tentacle,backends);
  char *repr = NULL;

  tentacle->backends = backends;
  tentacle->remote_url = lw6sys_str_copy (remote_url);
  if (password && strlen (password) > 0)
    {
      tentacle->password_checksum =
	lw6sys_password_checksum (remote_url, password);
    }
  else
    {
      tentacle->password_checksum =
	lw6sys_str_copy (_DEFAULT_PASSWORD_CHECKSUM);
    }
  tentacle->local_id = local_id;
  tentacle->remote_id = remote_id;
  if (tentacle->remote_url && tentacle->password_checksum)
    {
      tentacle->nb_cli_connections = backends->nb_cli_backends;
      if (tentacle->nb_cli_connections > 0)
	{
	  tentacle->cli_connections =
	    (lw6cli_connection_t **)
	    LW6SYS_CALLOC (tentacle->nb_cli_connections *
			   sizeof (lw6cli_connection_t *));
	  if (tentacle->cli_connections)
	    {
	      for (i = 0; i < tentacle->nb_cli_connections; ++i)
		{
		  tentacle->cli_connections[i] =
		    lw6cli_open (tentacle->backends->cli_backends[i],
				 remote_url, tentacle->password_checksum,
				 local_id, remote_id);
		  if (tentacle->cli_connections[i])
		    {
		      repr =
			lw6cli_repr (tentacle->backends->cli_backends[i],
				     tentacle->cli_connections[i]);
		      if (repr)
			{
			  lw6sys_log (LW6SYS_LOG_NOTICE,
				      _("connection \"%s\" opened"), repr);
			  LW6SYS_FREE (repr);
			}
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_WARNING,
				  _
				  ("unable to create connection %d to connect on \"%s\""),
				  i, tentacle->remote_url);
		      ret = 0;
		    }
		}
	    }
	  else
	    {
	      ret = 0;
	    }
	}
    }
  else
    {
      ret = 0;
    }

  return ret;
}

void
_lw6p2p_tentacle_clear (_lw6p2p_tentacle_t * tentacle)
{
  int i = 0;

  if (tentacle->nb_cli_connections > 0 && tentacle->cli_connections)
    {
      for (i = 0; i < tentacle->nb_cli_connections; ++i)
	{
	  if (tentacle->cli_connections[i])
	    {
	      lw6cli_close (tentacle->backends->cli_backends[i],
			    tentacle->cli_connections[i]);
	    }
	}
      LW6SYS_FREE (tentacle->cli_connections);
    }
  if (tentacle->remote_url)
    {
      LW6SYS_FREE (tentacle->remote_url);
    }
  if (tentacle->password_checksum)
    {
      LW6SYS_FREE (tentacle->password_checksum);
    }
}

int
_lw6p2p_tentacle_enabled (_lw6p2p_tentacle_t * tentacle)
{
  int ret = 0;

  ret = (tentacle->remote_id != 0);

  return ret;
}

void
_lw6p2p_tentacle_poll (_lw6p2p_tentacle_t * tentacle)
{
  int i;

  for (i = 0; i < tentacle->nb_cli_connections; ++i)
    {
      lw6cli_poll (tentacle->backends->cli_backends[i],
		   tentacle->cli_connections[i]);
    }
  // todo servers
}
