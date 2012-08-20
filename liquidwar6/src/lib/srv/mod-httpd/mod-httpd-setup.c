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

#include "../srv.h"
#include "mod-httpd-internal.h"

#define _HTTPD_DIR "httpd"
#define _ACCESS_LOG_FILE "access_log.txt"

_mod_httpd_context_t *
_mod_httpd_init (int argc, const char *argv[],
		 lw6cnx_properties_t * properties,
		 lw6srv_listener_t * listener)
{
  _mod_httpd_context_t *httpd_context = NULL;
  char *user_dir;
  char *data_dir;
  char *httpd_dir;
  int ok = 0;

  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("httpd init"));

  httpd_context =
    (_mod_httpd_context_t *) LW6SYS_CALLOC (sizeof (_mod_httpd_context_t));
  if (httpd_context)
    {
      data_dir = lw6sys_get_data_dir (argc, argv);
      if (data_dir)
	{
	  if (_mod_httpd_load_data (&(httpd_context->data), data_dir))
	    {
	      properties->hint_timeout =
		httpd_context->data.consts.error_timeout;
	      properties->ping_alter_base =
		httpd_context->data.consts.ping_alter_base;
	      properties->ping_alter_percent =
		httpd_context->data.consts.ping_alter_percent;
	      /*
	       * HTTP *as a server* (httpd) marked as non-reliable.
	       * Formally, it *is* reliable *only* if the client
	       * makes requests. If it's the only option we have,
	       * well, go for it, but if we have better (TCP?) just use
	       * something else.
	       */
	      properties->reliable = 0;
	      properties->backend_id = MOD_HTTPD_BACKEND_ID;
	      user_dir = lw6sys_get_user_dir (argc, argv);
	      if (user_dir)
		{
		  if (!lw6sys_dir_exists (user_dir))
		    {
		      lw6sys_create_dir (user_dir);
		    }
		  httpd_dir = lw6sys_path_concat (user_dir, _HTTPD_DIR);
		  if (httpd_dir)
		    {
		      if (!lw6sys_dir_exists (httpd_dir))
			{
			  lw6sys_create_dir (httpd_dir);
			}
		      httpd_context->access_log_file =
			lw6sys_path_concat (httpd_dir, _ACCESS_LOG_FILE);
		      if (httpd_context->access_log_file)
			{
			  if (lw6sys_clear_file
			      (httpd_context->access_log_file))
			    {
			      httpd_context->access_log_mutex =
				lw6sys_mutex_create ();
			      if (httpd_context->access_log_mutex)
				{
				  ok = 1;
				}
			    }
			  else
			    {
			      lw6sys_log (LW6SYS_LOG_WARNING,
					  _x_ ("can't init \"%s\""),
					  httpd_context->access_log_file);
			    }
			}
		      LW6SYS_FREE (httpd_dir);
		    }
		  LW6SYS_FREE (user_dir);
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _x_ ("couldn't read mod-httpd data from \"%s\""),
			  data_dir);
	    }
	  LW6SYS_FREE (data_dir);
	}

      if (!ok)
	{
	  _mod_httpd_quit (httpd_context);
	  httpd_context = NULL;
	}
    }

  if (!httpd_context)
    {
      lw6sys_log (LW6SYS_LOG_ERROR, _("can't initialize mod_httpd"));
    }

  return httpd_context;
}

void
_mod_httpd_quit (_mod_httpd_context_t * httpd_context)
{
  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("httpd quit"));

  if (httpd_context->access_log_mutex)
    {
      lw6sys_mutex_destroy (httpd_context->access_log_mutex);
    }
  if (httpd_context->access_log_file)
    {
      LW6SYS_FREE (httpd_context->access_log_file);
    }

  _mod_httpd_unload_data (&(httpd_context->data));

  LW6SYS_FREE (httpd_context);
}
