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

#include "../srv.h"
#include "mod-httpd-internal.h"

#define _HTTPD_SUBDIR "srv/httpd"
#define _CONSTS_FILE "httpd-const.xml"
#define _HTDOCS_DIR "htdocs"
#define _INDEX_HTML_FILE "index.html"
#define _ERROR_HTML_FILE "error.html"
#define _ROBOTS_TXT_FILE "robots.txt"
#define _GPL_TXT_FILE "gpl.txt"
#define _FAVICON_ICO_FILE "favicon.ico"

static void
_read_callback (void *callback_data, char *element, char *key, char *value)
{
  _httpd_consts_t *consts;

  consts = (_httpd_consts_t *) callback_data;

  if (!strcmp (element, "int"))
    {
      lw6cfg_read_xml_int (key, value, "timeout-msec",
			   &(consts->timeout_msec));
      lw6cfg_read_xml_int (key, value, "max-age", &(consts->max_age));
      lw6cfg_read_xml_int (key, value, "in-the-past", &(consts->in_the_past));
      lw6cfg_read_xml_int (key, value, "refresh", &(consts->refresh));
    }
  if (!strcmp (element, "string"))
    {
      lw6cfg_read_xml_string (key, value, "http-version",
			      &(consts->http_version));
      lw6cfg_read_xml_string (key, value, "content-type-html",
			      &(consts->content_type_html));
      lw6cfg_read_xml_string (key, value, "content-type-txt",
			      &(consts->content_type_txt));
      lw6cfg_read_xml_string (key, value, "content-type-jpeg",
			      &(consts->content_type_jpeg));
      lw6cfg_read_xml_string (key, value, "content-type-ico",
			      &(consts->content_type_ico));
      lw6cfg_read_xml_string (key, value, "error-401", &(consts->error_401));
      lw6cfg_read_xml_string (key, value, "error-403", &(consts->error_403));
      lw6cfg_read_xml_string (key, value, "error-404", &(consts->error_404));
      lw6cfg_read_xml_string (key, value, "error-405", &(consts->error_405));
      lw6cfg_read_xml_string (key, value, "error-500", &(consts->error_500));
    }
}

static int
_load_consts (_httpd_consts_t * consts, char *consts_file)
{
  int ret = 0;

  lw6sys_log (LW6SYS_LOG_INFO, _("reading \"%s\""), consts_file);

  ret =
    lw6cfg_read_key_value_xml_file (consts_file, _read_callback,
				    (void *) consts);
  if (ret)
    {
      ret = consts->http_version && consts->content_type_html
	&& consts->content_type_txt && consts->content_type_jpeg
	&& consts->content_type_ico && consts->error_401 && consts->error_403
	&& consts->error_404 && consts->error_405 && consts->error_500;
    }

  return ret;
}

static int
_load_htdocs (_httpd_htdocs_t * htdocs, char *htdocs_dir)
{
  int ret = 1;
  char *filename = NULL;

  lw6sys_log (LW6SYS_LOG_INFO, _("loading htdocs from \"%s\""), htdocs_dir);

  filename = lw6sys_path_concat (htdocs_dir, _INDEX_HTML_FILE);
  if (filename)
    {
      htdocs->index_html = lw6sys_read_file_content (filename);
      LW6SYS_FREE (filename);
    }
  filename = lw6sys_path_concat (htdocs_dir, _ERROR_HTML_FILE);
  if (filename)
    {
      htdocs->error_html = lw6sys_read_file_content (filename);
      LW6SYS_FREE (filename);
    }
  filename = lw6sys_path_concat (htdocs_dir, _ROBOTS_TXT_FILE);
  if (filename)
    {
      htdocs->robots_txt = lw6sys_read_file_content (filename);
      LW6SYS_FREE (filename);
    }
  filename = lw6sys_path_concat (htdocs_dir, _GPL_TXT_FILE);
  if (filename)
    {
      htdocs->gpl_txt = lw6sys_read_file_content (filename);
      LW6SYS_FREE (filename);
    }
  filename = lw6sys_path_concat (htdocs_dir, _FAVICON_ICO_FILE);
  if (filename)
    {
      htdocs->favicon_ico_data =
	lw6sys_read_file_content_bin (&(htdocs->favicon_ico_size), filename);
      LW6SYS_FREE (filename);
    }

  ret = htdocs->index_html && htdocs->error_html && htdocs->robots_txt
    && htdocs->gpl_txt && htdocs->favicon_ico_data;

  return ret;
}

int
_mod_httpd_load_data (_httpd_data_t * httpd_data, char *data_dir)
{
  int ret = 0;
  char *httpd_subdir = NULL;
  char *consts_file = NULL;
  char *htdocs_dir = NULL;

  httpd_subdir = lw6sys_path_concat (data_dir, _HTTPD_SUBDIR);
  if (httpd_subdir)
    {
      consts_file = lw6sys_path_concat (httpd_subdir, _CONSTS_FILE);
      htdocs_dir = lw6sys_path_concat (httpd_subdir, _HTDOCS_DIR);
      if (consts_file && htdocs_dir)
	{
	  ret = _load_consts (&(httpd_data->consts), consts_file)
	    && _load_htdocs (&(httpd_data->htdocs), htdocs_dir);
	}
      if (consts_file)
	{
	  LW6SYS_FREE (consts_file);
	}
      if (htdocs_dir)
	{
	  LW6SYS_FREE (htdocs_dir);
	}
      LW6SYS_FREE (httpd_subdir);
    }

  return ret;
}

static int
_unload_consts (_httpd_consts_t * consts)
{
  int ret = 1;

  if (consts->http_version)
    {
      LW6SYS_FREE (consts->http_version);
    }
  if (consts->content_type_html)
    {
      LW6SYS_FREE (consts->content_type_html);
    }
  if (consts->content_type_txt)
    {
      LW6SYS_FREE (consts->content_type_txt);
    }
  if (consts->content_type_jpeg)
    {
      LW6SYS_FREE (consts->content_type_jpeg);
    }
  if (consts->content_type_ico)
    {
      LW6SYS_FREE (consts->content_type_ico);
    }
  if (consts->error_401)
    {
      LW6SYS_FREE (consts->error_401);
    }
  if (consts->error_403)
    {
      LW6SYS_FREE (consts->error_403);
    }
  if (consts->error_404)
    {
      LW6SYS_FREE (consts->error_404);
    }
  if (consts->error_405)
    {
      LW6SYS_FREE (consts->error_405);
    }
  if (consts->error_500)
    {
      LW6SYS_FREE (consts->error_500);
    }
  memset (consts, 0, sizeof (_httpd_consts_t));

  return ret;
}

static int
_unload_htdocs (_httpd_htdocs_t * htdocs)
{
  int ret = 1;

  if (htdocs->index_html)
    {
      LW6SYS_FREE (htdocs->index_html);
    }
  if (htdocs->error_html)
    {
      LW6SYS_FREE (htdocs->error_html);
    }
  if (htdocs->robots_txt)
    {
      LW6SYS_FREE (htdocs->robots_txt);
    }
  if (htdocs->gpl_txt)
    {
      LW6SYS_FREE (htdocs->gpl_txt);
    }
  if (htdocs->favicon_ico_data)
    {
      LW6SYS_FREE (htdocs->favicon_ico_data);
    }
  memset (htdocs, 0, sizeof (_httpd_htdocs_t));

  return ret;
}

void
_mod_httpd_unload_data (_httpd_data_t * httpd_data)
{
  _unload_htdocs (&(httpd_data->htdocs));
  _unload_consts (&(httpd_data->consts));
}
