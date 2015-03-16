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

#include "sys.h"
#include "sys-internal.h"

#define _HTTP_PORT 80
#define _HTTP_URL_MASK "http://%s:%d/"
#define _HTTP_URL_MASK_PORT_80 "http://%s/"
#define _DOT_DOT_SLASH_SLASH "://"
#define _HTTP "http"
#define _HTTPS "https"
#define _DOT_DOT ':'
#define _MAX_PORT 65535
#define _HTTP_PORT 80
#define _HTTPS_PORT 443
#define _SLASH '/'
#define _SLASH_STR "/"
#define _EMPTY_STR ""
#define _LOCALHOST "localhost"

/**
 * lw6sys_url_http_from_ip_port
 *
 * @sys_context: global system context
 * @ip: IP address
 * @port: IP port
 *
 * Returns an http URL pointing to ip:port that is, adds a heading
 * http:// and a trailing /, and treats port 80 as default. This
 * is used to create public_url in net modules.
 *
 * Return value: a newly allocated string, NULL on error.
 */
extern char *
lw6sys_url_http_from_ip_port (lw6sys_context_t * sys_context, const char *ip, int port)
{
  char *ret = NULL;

  if (port == _HTTP_PORT)
    {
      ret = lw6sys_new_sprintf (sys_context, _HTTP_URL_MASK_PORT_80, ip);
    }
  else
    {
      ret = lw6sys_new_sprintf (sys_context, _HTTP_URL_MASK, ip, port);
    }

  if (ret)
    {
      if (!lw6sys_url_is_canonized (sys_context, ret))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("generated url \"%s\" reported as not canonized"), ret);
	}
    }

  return ret;
}

static int
_is_host (char c)
{
  return (lw6sys_chr_is_alnum (c) || (c == '.') || (c == '-'));
}

static int
_is_port (char c)
{
  return (lw6sys_chr_is_digit (c));
}

/**
 * lw6sys_url_parse
 *
 * @sys_context: global system context
 * @url: the URL to parse
 *
 * Parses a URL, this is not a complete RFC compliant
 * parser, it's only used to transform URLs into
 * their 'canonical' form as well as getting basic
 * info such as on which port one should connect.
 *
 * Return value: a newly allocated struct, NULL on error
 */
lw6sys_url_t *
lw6sys_url_parse (lw6sys_context_t * sys_context, const char *url)
{
  lw6sys_url_t *ret = NULL;
  char *tmp = NULL;
  char *pos = NULL;
  char *seek = NULL;
  char seek_c = '\0';

  ret = (lw6sys_url_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6sys_url_t));
  if (ret)
    {
      tmp = lw6sys_str_copy (sys_context, url);
      lw6sys_str_cleanup_ascii7 (sys_context, tmp);
      if (lw6sys_str_starts_with (sys_context, tmp, _HTTPS))
	{
	  ret->use_ssl = 1;
	}
      pos = tmp;
      seek = strstr (pos, _DOT_DOT_SLASH_SLASH);
      if (seek)
	{
	  pos = seek + strlen (_DOT_DOT_SLASH_SLASH);
	}
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("interpreting host, remaining string is \"%s\""), pos);
      seek = pos;
      while (_is_host (*seek))
	{
	  seek++;
	}
      seek_c = (*seek);
      (*seek) = '\0';
      lw6sys_str_tolower (sys_context, pos);
      if (strlen (pos) > 0)
	{
	  ret->host = lw6sys_str_copy (sys_context, pos);
	}
      else
	{
	  ret->host = lw6sys_str_copy (sys_context, _LOCALHOST);
	}
      (*seek) = seek_c;
      pos = seek;
      if ((*pos) == _DOT_DOT)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("port detected, remaining string is \"%s\""), pos);
	  pos++;
	  seek = pos;
	  while (_is_port (*seek))
	    {
	      seek++;
	    }
	  seek_c = (*seek);
	  (*seek) = '\0';
	  ret->port = lw6sys_atoi (sys_context, pos);
	  (*seek) = seek_c;
	  pos = seek;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("no port detected, remaining string is \"%s\""), pos);
	  if (ret->use_ssl)
	    {
	      ret->port = _HTTPS_PORT;
	    }
	  else
	    {
	      ret->port = _HTTP_PORT;
	    }
	}
      if (ret->port <= 0 || ret->port > _MAX_PORT)
	{
	  ret->port = _HTTP_PORT;
	}
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("interpreting URI, remaining string is \"%s\""), pos);
      if (strlen (pos) > 0 && (*pos) == _SLASH)
	{
	  ret->uri = lw6sys_escape_http_uri (sys_context, pos);
	}
      else
	{
	  ret->uri = lw6sys_new_sprintf (sys_context, "%c", _SLASH);
	}
      if (ret->host && ret->uri)
	{
	  // OK
	}
      else
	{
	  lw6sys_url_free (sys_context, ret);
	  ret = NULL;
	}
      LW6SYS_FREE (sys_context, tmp);
    }

  return ret;
}

/**
 * lw6sys_url_free
 *
 * @sys_context: global system context
 * @url: the url struct to free
 *
 * Frees a URL struct and all its members.
 *
 * Return value: none.
 */
void
lw6sys_url_free (lw6sys_context_t * sys_context, lw6sys_url_t * url)
{
  if (url)
    {
      if (url->host)
	{
	  LW6SYS_FREE (sys_context, url->host);
	}
      if (url->uri)
	{
	  LW6SYS_FREE (sys_context, url->uri);
	}
      LW6SYS_FREE (sys_context, url);
    }
}

/**
 * lw6sys_url_canonize
 *
 * @sys_context: global system context
 * @url: the url to check & transform
 *
 * Checks if a given URL is correct and, if it is,
 * transforms it into its canonical form. This is
 * mostly to get rid of typesettings error, add a
 * tailing /, transform all domain into lowercase,
 * among other things. A canonized url passed into
 * this function should come out exactly the same.
 *
 * Return value: a newly allocated string.
 */
char *
lw6sys_url_canonize (lw6sys_context_t * sys_context, const char *url)
{
  char *ret = NULL;
  lw6sys_url_t *tmp = NULL;
  int len = 0;
  char *protocol = NULL;
  char *tail = NULL;

  if (url && strlen (url) > 0)
    {
      tmp = lw6sys_url_parse (sys_context, url);
      if (tmp)
	{
	  if (tmp->use_ssl)
	    {
	      protocol = _HTTPS;
	    }
	  else
	    {
	      protocol = _HTTP;
	    }

	  len = strlen (tmp->uri);
	  if (len == 0 || (len > 0 && tmp->uri[len - 1] != _SLASH))
	    {
	      tail = _SLASH_STR;
	    }
	  else
	    {
	      tail = _EMPTY_STR;
	    }

	  if ((tmp->port == _HTTP_PORT && !tmp->use_ssl) || (tmp->port == _HTTPS_PORT && tmp->use_ssl))
	    {
	      ret = lw6sys_new_sprintf (sys_context, "%s://%s%s%s", protocol, tmp->host, tmp->uri, tail);
	    }
	  else
	    {
	      ret = lw6sys_new_sprintf (sys_context, "%s://%s:%d%s%s", protocol, tmp->host, tmp->port, tmp->uri, tail);
	    }

	  lw6sys_url_free (sys_context, tmp);
	}
    }
  else
    {
      ret = lw6sys_str_copy (sys_context, "");
    }

  return ret;
}

/**
 * lw6sys_url_is_canonized
 *
 * @sys_context: global system context
 * @url: the URL to check
 *
 * Checks wether an URL is in its canonized form.
 *
 * Return value: 1 if OK (canonized form), 0 if not
 */
int
lw6sys_url_is_canonized (lw6sys_context_t * sys_context, const char *url)
{
  int ret = 0;
  char *canonized_url = NULL;

  canonized_url = lw6sys_url_canonize (sys_context, url);
  if (canonized_url)
    {
      if (lw6sys_str_is_same (sys_context, canonized_url, url))
	{
	  ret = 1;
	}
      LW6SYS_FREE (sys_context, canonized_url);
    }

  return ret;
}
