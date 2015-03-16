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
#endif // HAVE_CONFIG_H

#include "p2p.h"
#include "p2p-internal.h"

int
_lw6p2p_backends_init_cli (lw6sys_context_t * sys_context, int argc, const char *argv[], _lw6p2p_backends_t * backends, char *client_backends)
{
  int ret = 0;
  lw6sys_list_t *list_backends = NULL;
  char *backend = NULL;

  list_backends = lw6sys_str_split_config_item (sys_context, client_backends);
  if (list_backends)
    {
      if (lw6sys_list_is_empty (sys_context, list_backends))
	{
	  /*
	   * No backend required, we return OK, still we keep going
	   * since the pop_front will make the list NULL and free it.
	   */
	  ret = 1;
	}
      while (list_backends && ((backend = lw6sys_list_pop_front (sys_context, &list_backends)) != NULL))
	{
	  backends->nb_cli_backends++;
	  if (backends->cli_backends)
	    {
	      backends->cli_backends =
		(lw6cli_backend_t **) LW6SYS_REALLOC (sys_context, backends->cli_backends, backends->nb_cli_backends * sizeof (lw6cli_backend_t *));
	    }
	  else
	    {
	      backends->cli_backends = (lw6cli_backend_t **) LW6SYS_MALLOC (sys_context, backends->nb_cli_backends * sizeof (lw6cli_backend_t *));
	    }
	  if (backends->cli_backends)
	    {
	      backends->cli_backends[backends->nb_cli_backends - 1] = lw6cli_create_backend (sys_context, argc, argv, backend);
	      if (backends->cli_backends[backends->nb_cli_backends - 1])
		{
		  if (lw6cli_init (sys_context, backends->cli_backends[backends->nb_cli_backends - 1]))
		    {
		      backends->hint_timeout_max =
			lw6sys_imax (backends->hint_timeout_max, backends->cli_backends[backends->nb_cli_backends - 1]->properties.hint_timeout);
		      ret = 1;
		    }
		  else
		    {
		      lw6cli_destroy_backend (sys_context, backends->cli_backends[backends->nb_cli_backends - 1]);
		      backends->cli_backends[backends->nb_cli_backends - 1] = NULL;
		      backends->nb_cli_backends--;
		    }
		}
	      else
		{
		  backends->nb_cli_backends--;
		}
	    }
	  else
	    {
	      backends->nb_cli_backends--;
	    }
	  LW6SYS_FREE (sys_context, backend);
	}
      list_backends = NULL;
    }
  return (ret);
}

int
_lw6p2p_backends_init_srv (lw6sys_context_t * sys_context, int argc, const char *argv[], _lw6p2p_backends_t * backends, char *server_backends,
			   lw6srv_listener_t * listener)
{
  int ret = 0;
  lw6sys_list_t *list_backends = NULL;
  char *backend = NULL;

  list_backends = lw6sys_str_split_config_item (sys_context, server_backends);
  if (list_backends)
    {
      if (lw6sys_list_is_empty (sys_context, list_backends))
	{
	  /*
	   * No backend required, we return OK, still we keep going
	   * since the pop_front will make the list NULL and free it.
	   */
	  ret = 1;
	}
      while (list_backends && ((backend = lw6sys_list_pop_front (sys_context, &list_backends)) != NULL))
	{
	  backends->nb_srv_backends++;
	  if (backends->srv_backends)
	    {
	      backends->srv_backends =
		(lw6srv_backend_t **) LW6SYS_REALLOC (sys_context, backends->srv_backends, backends->nb_srv_backends * sizeof (lw6srv_backend_t *));
	    }
	  else
	    {
	      backends->srv_backends = (lw6srv_backend_t **) LW6SYS_MALLOC (sys_context, backends->nb_srv_backends * sizeof (lw6srv_backend_t *));
	    }
	  if (backends->srv_backends)
	    {
	      backends->srv_backends[backends->nb_srv_backends - 1] = lw6srv_create_backend (sys_context, argc, argv, backend);
	      if (backends->srv_backends[backends->nb_srv_backends - 1])
		{
		  if (lw6srv_init (sys_context, backends->srv_backends[backends->nb_srv_backends - 1], listener))
		    {
		      backends->hint_timeout_max =
			lw6sys_imax (backends->hint_timeout_max, backends->srv_backends[backends->nb_srv_backends - 1]->properties.hint_timeout);
		      /*
		       * OK, at this state, we have at
		       * least one server backend working,
		       * we consider it's fine.
		       */
		      ret = 1;
		    }
		  else
		    {
		      lw6srv_destroy_backend (sys_context, backends->srv_backends[backends->nb_srv_backends - 1]);
		      backends->srv_backends[backends->nb_srv_backends - 1] = NULL;
		      backends->nb_srv_backends--;
		    }
		}
	      else
		{
		  backends->nb_srv_backends--;
		}
	    }
	  else
	    {
	      backends->nb_srv_backends--;
	    }
	  LW6SYS_FREE (sys_context, backend);
	}
      list_backends = NULL;
    }

  return ret;
}

void
_lw6p2p_backends_clear_cli (lw6sys_context_t * sys_context, _lw6p2p_backends_t * backends)
{
  int i = 0;

  if (backends->cli_backends)
    {
      for (i = 0; i < backends->nb_cli_backends; ++i)
	{
	  if (backends->cli_backends[i])
	    {
	      lw6cli_quit (sys_context, backends->cli_backends[i]);
	      lw6cli_destroy_backend (sys_context, backends->cli_backends[i]);
	      backends->cli_backends[i] = NULL;
	    }
	}
      LW6SYS_FREE (sys_context, backends->cli_backends);
      backends->cli_backends = NULL;
      backends->nb_cli_backends = 0;
    }
}

void
_lw6p2p_backends_clear_srv (lw6sys_context_t * sys_context, _lw6p2p_backends_t * backends)
{
  int i = 0;

  if (backends->srv_backends)
    {
      for (i = 0; i < backends->nb_srv_backends; ++i)
	{
	  if (backends->srv_backends[i])
	    {
	      lw6srv_quit (sys_context, backends->srv_backends[i]);
	      lw6srv_destroy_backend (sys_context, backends->srv_backends[i]);
	      backends->srv_backends[i] = NULL;
	    }
	}
      LW6SYS_FREE (sys_context, backends->srv_backends);
      backends->srv_backends = NULL;
      backends->nb_srv_backends = 0;
    }
}
