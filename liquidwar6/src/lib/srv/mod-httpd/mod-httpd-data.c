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

#define _HTTPD_SUBDIR "srv/httpd"
#define _CONSTS_FILE "httpd-const.xml"
#define _HTDOCS_DIR "htdocs"
#define _INDEX_HTML_FILE "index.html"
#define _ERROR_HTML_FILE "error.html"
#define _ROBOTS_TXT_FILE "robots.txt"
#define _GPL_TXT_FILE "gpl.txt"
#define _FAVICON_ICO_FILE "favicon.ico"

static void
_read_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  _mod_httpd_consts_t *consts;

  consts = (_mod_httpd_consts_t *) callback_data;

  if (!strcmp (element, "int"))
    {
      lw6cfg_read_xml_int (sys_context, key, value, "error-timeout", &(consts->error_timeout));
      lw6cfg_read_xml_int (sys_context, key, value, "ping-alter-base", &(consts->ping_alter_base));
      lw6cfg_read_xml_int (sys_context, key, value, "ping-alter-percent", &(consts->ping_alter_percent));
      lw6cfg_read_xml_int (sys_context, key, value, "max-age", &(consts->max_age));
      lw6cfg_read_xml_int (sys_context, key, value, "in-the-past", &(consts->in_the_past));
      lw6cfg_read_xml_int (sys_context, key, value, "refresh-index-header", &(consts->refresh_index_header));
      lw6cfg_read_xml_int (sys_context, key, value, "refresh-screenshot-header", &(consts->refresh_screenshot_header));
      lw6cfg_read_xml_int (sys_context, key, value, "refresh-index-js", &(consts->refresh_index_js));
      lw6cfg_read_xml_int (sys_context, key, value, "refresh-screenshot-js", &(consts->refresh_screenshot_js));
    }
  if (!strcmp (element, "string"))
    {
      lw6cfg_read_xml_string (sys_context, key, value, "http-version", &(consts->http_version));
      lw6cfg_read_xml_string (sys_context, key, value, "header-description", &(consts->header_description));
      lw6cfg_read_xml_string (sys_context, key, value, "header-keywords", &(consts->header_keywords));
      lw6cfg_read_xml_string (sys_context, key, value, "content-type-html", &(consts->content_type_html));
      lw6cfg_read_xml_string (sys_context, key, value, "content-type-txt", &(consts->content_type_txt));
      lw6cfg_read_xml_string (sys_context, key, value, "content-type-jpeg", &(consts->content_type_jpeg));
      lw6cfg_read_xml_string (sys_context, key, value, "content-type-ico", &(consts->content_type_ico));
      lw6cfg_read_xml_string (sys_context, key, value, "error-401", &(consts->error_401));
      lw6cfg_read_xml_string (sys_context, key, value, "error-403", &(consts->error_403));
      lw6cfg_read_xml_string (sys_context, key, value, "error-404", &(consts->error_404));
      lw6cfg_read_xml_string (sys_context, key, value, "error-405", &(consts->error_405));
      lw6cfg_read_xml_string (sys_context, key, value, "error-500", &(consts->error_500));
      lw6cfg_read_xml_string (sys_context, key, value, "auth-realm", &(consts->auth_realm));
    }
}

static int
_load_consts (lw6sys_context_t * sys_context, _mod_httpd_consts_t * consts, const char *consts_file)
{
  int ret = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading \"%s\""), consts_file);

  ret = lw6cfg_read_key_value_xml_file (sys_context, consts_file, _read_callback, (void *) consts);
  if (ret)
    {
      ret = consts->http_version && consts->header_description
	&& consts->header_keywords && consts->content_type_html
	&& consts->content_type_txt && consts->content_type_jpeg
	&& consts->content_type_ico && consts->error_401 && consts->error_403
	&& consts->error_404 && consts->error_405 && consts->error_500 && consts->auth_realm;
    }

  return ret;
}

static int
_load_htdocs (lw6sys_context_t * sys_context, _mod_httpd_htdocs_t * htdocs, const char *htdocs_dir)
{
  int ret = 1;
  char *filename = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("loading htdocs from \"%s\""), htdocs_dir);

  filename = lw6sys_path_concat (sys_context, htdocs_dir, _INDEX_HTML_FILE);
  if (filename)
    {
      htdocs->index_html = lw6sys_read_file_content (sys_context, filename);
      LW6SYS_FREE (sys_context, filename);
    }
  filename = lw6sys_path_concat (sys_context, htdocs_dir, _ERROR_HTML_FILE);
  if (filename)
    {
      htdocs->error_html = lw6sys_read_file_content (sys_context, filename);
      LW6SYS_FREE (sys_context, filename);
    }
  filename = lw6sys_path_concat (sys_context, htdocs_dir, _ROBOTS_TXT_FILE);
  if (filename)
    {
      htdocs->robots_txt = lw6sys_read_file_content (sys_context, filename);
      LW6SYS_FREE (sys_context, filename);
    }
  filename = lw6sys_path_concat (sys_context, htdocs_dir, _GPL_TXT_FILE);
  if (filename)
    {
      htdocs->gpl_txt = lw6sys_read_file_content (sys_context, filename);
      LW6SYS_FREE (sys_context, filename);
    }
  filename = lw6sys_path_concat (sys_context, htdocs_dir, _FAVICON_ICO_FILE);
  if (filename)
    {
      htdocs->favicon_ico_data = lw6sys_read_file_content_bin (sys_context, &(htdocs->favicon_ico_size), filename);
      LW6SYS_FREE (sys_context, filename);
    }

  ret = htdocs->index_html && htdocs->error_html && htdocs->robots_txt && htdocs->gpl_txt && htdocs->favicon_ico_data;

  return ret;
}

int
_mod_httpd_load_data (lw6sys_context_t * sys_context, _mod_httpd_data_t * httpd_data, const char *data_dir)
{
  int ret = 0;
  char *httpd_subdir = NULL;
  char *consts_file = NULL;
  char *htdocs_dir = NULL;

  httpd_subdir = lw6sys_path_concat (sys_context, data_dir, _HTTPD_SUBDIR);
  if (httpd_subdir)
    {
      consts_file = lw6sys_path_concat (sys_context, httpd_subdir, _CONSTS_FILE);
      htdocs_dir = lw6sys_path_concat (sys_context, httpd_subdir, _HTDOCS_DIR);
      if (consts_file && htdocs_dir)
	{
	  ret = _load_consts (sys_context, &(httpd_data->consts), consts_file) && _load_htdocs (sys_context, &(httpd_data->htdocs), htdocs_dir);
	}
      if (consts_file)
	{
	  LW6SYS_FREE (sys_context, consts_file);
	}
      if (htdocs_dir)
	{
	  LW6SYS_FREE (sys_context, htdocs_dir);
	}
      LW6SYS_FREE (sys_context, httpd_subdir);
    }

  return ret;
}

static int
_unload_consts (lw6sys_context_t * sys_context, _mod_httpd_consts_t * consts)
{
  int ret = 1;

  if (consts->http_version)
    {
      LW6SYS_FREE (sys_context, consts->http_version);
    }
  if (consts->header_description)
    {
      LW6SYS_FREE (sys_context, consts->header_description);
    }
  if (consts->header_keywords)
    {
      LW6SYS_FREE (sys_context, consts->header_keywords);
    }
  if (consts->content_type_html)
    {
      LW6SYS_FREE (sys_context, consts->content_type_html);
    }
  if (consts->content_type_txt)
    {
      LW6SYS_FREE (sys_context, consts->content_type_txt);
    }
  if (consts->content_type_jpeg)
    {
      LW6SYS_FREE (sys_context, consts->content_type_jpeg);
    }
  if (consts->content_type_ico)
    {
      LW6SYS_FREE (sys_context, consts->content_type_ico);
    }
  if (consts->error_401)
    {
      LW6SYS_FREE (sys_context, consts->error_401);
    }
  if (consts->error_403)
    {
      LW6SYS_FREE (sys_context, consts->error_403);
    }
  if (consts->error_404)
    {
      LW6SYS_FREE (sys_context, consts->error_404);
    }
  if (consts->error_405)
    {
      LW6SYS_FREE (sys_context, consts->error_405);
    }
  if (consts->error_500)
    {
      LW6SYS_FREE (sys_context, consts->error_500);
    }
  if (consts->auth_realm)
    {
      LW6SYS_FREE (sys_context, consts->auth_realm);
    }
  memset (consts, 0, sizeof (_mod_httpd_consts_t));

  return ret;
}

static int
_unload_htdocs (lw6sys_context_t * sys_context, _mod_httpd_htdocs_t * htdocs)
{
  int ret = 1;

  if (htdocs->index_html)
    {
      LW6SYS_FREE (sys_context, htdocs->index_html);
    }
  if (htdocs->error_html)
    {
      LW6SYS_FREE (sys_context, htdocs->error_html);
    }
  if (htdocs->robots_txt)
    {
      LW6SYS_FREE (sys_context, htdocs->robots_txt);
    }
  if (htdocs->gpl_txt)
    {
      LW6SYS_FREE (sys_context, htdocs->gpl_txt);
    }
  if (htdocs->favicon_ico_data)
    {
      LW6SYS_FREE (sys_context, htdocs->favicon_ico_data);
    }
  memset (htdocs, 0, sizeof (_mod_httpd_htdocs_t));

  return ret;
}

void
_mod_httpd_unload_data (lw6sys_context_t * sys_context, _mod_httpd_data_t * httpd_data)
{
  _unload_htdocs (sys_context, &(httpd_data->htdocs));
  _unload_consts (sys_context, &(httpd_data->consts));
}
