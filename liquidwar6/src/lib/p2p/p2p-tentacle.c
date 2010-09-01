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
  lw6sys_url_t *parsed_url = NULL;

  tentacle->backends = backends;
  tentacle->remote_url = lw6sys_str_copy (remote_url);
  parsed_url = lw6sys_url_parse (remote_url);
  if (parsed_url)
    {
      /*
       * In theory this could be a long blocking call,
       * in practise, when we're at this stage OOB messages
       * probably have already initialized the DNS cache
       * so response will be fast.
       */
      tentacle->remote_ip = lw6net_dns_gethostbyname (parsed_url->host);
      tentacle->remote_port = parsed_url->port;
      lw6sys_url_free (parsed_url);
    }
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
  tentacle->local_id_int = local_id;
  tentacle->local_id_str = lw6sys_id_ltoa (local_id);
  tentacle->remote_id_int = remote_id;
  tentacle->remote_id_str = lw6sys_id_ltoa (remote_id);

  if (tentacle->remote_url && tentacle->remote_ip
      && tentacle->password_checksum && tentacle->local_id_str
      && tentacle->remote_id_str)
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
				 remote_url, tentacle->remote_ip,
				 tentacle->remote_port,
				 tentacle->password_checksum,
				 tentacle->local_id_str,
				 tentacle->remote_id_str);
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
  if (tentacle->remote_ip)
    {
      LW6SYS_FREE (tentacle->remote_ip);
    }
  if (tentacle->password_checksum)
    {
      LW6SYS_FREE (tentacle->password_checksum);
    }
  if (tentacle->local_id_str)
    {
      LW6SYS_FREE (tentacle->local_id_str);
    }
  if (tentacle->remote_id_str)
    {
      LW6SYS_FREE (tentacle->remote_id_str);
    }
  memset (tentacle, 0, sizeof (_lw6p2p_tentacle_t));
}

int
_lw6p2p_tentacle_enabled (_lw6p2p_tentacle_t * tentacle)
{
  int ret = 0;

  ret = (tentacle->remote_id_int != 0);

  return ret;
}

void
_lw6p2p_tentacle_poll (_lw6p2p_tentacle_t * tentacle,
		       lw6nod_info_t * node_info)
{
  int i;
  char *msg;

  if (!tentacle->hello_sent)
    {
      msg = lw6msg_cmd_generate_hello (node_info);
      if (msg)
	{
	  for (i = 0; i < tentacle->nb_cli_connections; ++i)
	    {
	      if (lw6cli_send (tentacle->backends->cli_backends[i],
			       tentacle->cli_connections[i], msg))
		{
		  tentacle->hello_sent = 1;
		}
	    }
	  LW6SYS_FREE (msg);
	}
    }
  for (i = 0; i < tentacle->nb_cli_connections; ++i)
    {
      lw6cli_poll (tentacle->backends->cli_backends[i],
		   tentacle->cli_connections[i]);
    }

  // todo servers
}
