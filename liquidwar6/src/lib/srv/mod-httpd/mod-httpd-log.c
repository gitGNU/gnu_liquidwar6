/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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

int
_mod_httpd_log (_mod_httpd_context_t * httpd_context,
		_mod_httpd_request_t * request,
		_mod_httpd_response_t * response)
{
  int ret = 0;
  FILE *f = NULL;
  char *date_str = NULL;

  date_str = lw6sys_date_clf ();
  if (date_str)
    {
      if (lw6sys_mutex_lock (httpd_context->access_log_mutex))
	{
	  f = fopen (httpd_context->access_log_file, "ab");
	  if (f)
	    {
	      if (fprintf
		  (f, "%s - - [%s] \"%s\" %d %d\n", request->client_ip,
		   date_str, request->first_line, response->status,
		   response->content_size) > 0)
		{
		  ret = 1;
		}
	      fclose (f);
	    }
	  lw6sys_mutex_unlock (httpd_context->access_log_mutex);
	}
      LW6SYS_FREE (date_str);
    }

  return ret;
}
