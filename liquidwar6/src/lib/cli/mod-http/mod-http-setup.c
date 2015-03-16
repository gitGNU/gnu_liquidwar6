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

#include "../cli.h"
#include "mod-http-internal.h"

#define _CURL_FLAGS 0

_mod_http_context_t *
_mod_http_init (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6cnx_properties_t * properties)
{
  _mod_http_context_t *http_context = NULL;
  char *data_dir = NULL;
  int ok = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("http init"));

  http_context = (_mod_http_context_t *) LW6SYS_CALLOC (sys_context, sizeof (_mod_http_context_t));
  if (http_context)
    {
      http_context->curl_init_ret = -1;
      data_dir = lw6sys_get_data_dir (sys_context, argc, argv);
      if (data_dir)
	{
	  if (_mod_http_load_data (sys_context, &(http_context->data), data_dir))
	    {
	      properties->hint_timeout = http_context->data.consts.global_timeout;
	      properties->ping_alter_base = http_context->data.consts.ping_alter_base;
	      properties->ping_alter_percent = http_context->data.consts.ping_alter_percent;
	      /*
	       * HTTP *as a client* considered reliable. This is
	       * not the case of the server, but the client is OK
	       */
	      properties->reliable = 1;
	      properties->backend_id = MOD_HTTP_BACKEND_ID;
	      http_context->curl_init_ret = curl_global_init (_CURL_FLAGS);
	      if (http_context->curl_init_ret == CURLE_OK)
		{
		  ok = 1;
		}
	    }
	  LW6SYS_FREE (sys_context, data_dir);
	}
      if (!ok)
	{
	  _mod_http_quit (sys_context, http_context);
	  http_context = NULL;
	}
    }

  if (!http_context)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _("can't initialize mod_http"));
    }

  return http_context;
}

void
_mod_http_quit (lw6sys_context_t * sys_context, _mod_http_context_t * http_context)
{
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("http quit"));
  if (http_context->curl_init_ret == CURLE_OK)
    {
      curl_global_cleanup ();
    }
  _mod_http_unload_data (sys_context, &(http_context->data));
  LW6SYS_FREE (sys_context, http_context);
}
