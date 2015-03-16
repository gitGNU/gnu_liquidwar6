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

#include "../srv.h"
#include "mod-httpd-internal.h"

_mod_httpd_response_t *
_mod_httpd_http_error (lw6sys_context_t * sys_context, _mod_httpd_context_t * httpd_context, int status)
{
  _mod_httpd_response_t *response = NULL;
  char *content = NULL;
  char *status_text = NULL;
  char *date_str = NULL;

  switch (status)
    {
    case _MOD_HTTPD_STATUS_401:
      status_text = httpd_context->data.consts.error_401;
      break;
    case _MOD_HTTPD_STATUS_403:
      status_text = httpd_context->data.consts.error_403;
      break;
    case _MOD_HTTPD_STATUS_404:
      status_text = httpd_context->data.consts.error_404;
      break;
    case _MOD_HTTPD_STATUS_405:
      status_text = httpd_context->data.consts.error_405;
      break;
    }

  /*
   * By default, error 500
   */
  if (!status_text)
    {
      status = _MOD_HTTPD_STATUS_500;
      status_text = httpd_context->data.consts.error_500;
    }

  if (status_text)
    {
      date_str = lw6sys_date_rfc1123 (sys_context, 0);
      if (date_str)
	{
	  content =
	    lw6sys_new_sprintf (sys_context,
				httpd_context->data.htdocs.error_html, status,
				status_text, lw6sys_build_get_package_name (),
				httpd_context->data.consts.header_description,
				httpd_context->data.consts.header_keywords,
				lw6sys_build_get_copyright (),
				lw6sys_build_get_package_tarname (sys_context), status, status_text, lw6sys_build_get_package_name (sys_context), date_str);
	  if (content)
	    {
	      response = _mod_httpd_response_from_str (sys_context, httpd_context, status, 1, 0, NULL, httpd_context->data.consts.content_type_html, content);
	      LW6SYS_FREE (sys_context, content);
	      content = NULL;
	    }
	  LW6SYS_FREE (sys_context, date_str);
	}
    }

  return response;
}
