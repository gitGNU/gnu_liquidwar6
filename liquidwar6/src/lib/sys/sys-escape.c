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

#define _ESCAPE_HTTP_URI_LEN 3
#define _ESCAPE_HTTP_URI_MASK "%02X"
#define _ESCAPE_HTTP_URI_MAX_LEN 1024
#define _ESCAPE_HTML_ATTRIBUTE_LEN 6
#define _ESCAPE_HTML_ATTRIBUTE_QUOT_CHAR '"'
#define _ESCAPE_HTML_ATTRIBUTE_QUOT_STR "&quot;"
#define _ESCAPE_HTML_ATTRIBUTE_LT_CHAR '<'
#define _ESCAPE_HTML_ATTRIBUTE_LT_STR "&lt;"
#define _ESCAPE_HTML_ATTRIBUTE_GT_CHAR '>'
#define _ESCAPE_HTML_ATTRIBUTE_GT_STR "&gt;"
#define _ESCAPE_HTML_ATTRIBUTE_AMP_CHAR '&'
#define _ESCAPE_HTML_ATTRIBUTE_AMP_STR "&amp;"
#define _ESCAPE_HTML_ATTRIBUTE_MAX_LEN 4096
#define _ESCAPE_SQL_VALUE_LEN 2
#define _ESCAPE_SQL_VALUE_QUOT_CHAR '\''
#define _ESCAPE_SQL_VALUE_QUOT_STR "''"
#define _ESCAPE_SQL_VALUE_MAX_LEN 2048

/**
 * lw6sys_escape_http_uri
 *
 * @sys_context: global system context
 * @src: the string to escape
 *
 * Transforms a string so that it does not contain any non-valid
 * URL chars, it will mostly convert chars over 128 into their
 * an hexadecimal code which replaces them in URLs. Note that this function
 * is non really standard compliant for it won't encode '%' but keep
 * it the same. This is to allow using it several times on the same
 * string and avoid double-triple encoding of '%'. In practice it's
 * not recommended to have public_url for nodes with '%' in them,
 * and the program will never generate such url when guessing urls.
 *
 * Return value: newly allocated string.
 */
char *
lw6sys_escape_http_uri (lw6sys_context_t * sys_context, const char *src)
{
  char *ret = NULL;
  int len = 0;
  int i = 0, j = 0;
  unsigned char c;
  char *hexa;

  src = lw6sys_str_empty_if_null (sys_context, src);
  len = lw6sys_imin (strlen (src), _ESCAPE_HTTP_URI_MAX_LEN);
  ret = (char *) LW6SYS_CALLOC (sys_context, _ESCAPE_HTTP_URI_LEN * len + 1);
  if (ret)
    {
      for (i = 0, j = 0; i < len && j < _ESCAPE_HTTP_URI_MAX_LEN; ++i)
	{
	  c = src[i];
	  if (c >= ' ')
	    {
	      if (c == '_' || c == '-' || c == '/' || c == '.' || c == ','
		  || c == '#' || c == ':' || c == ';' || c == '?' || c == '&' || c == '%' || lw6sys_chr_is_alnum (c))
		{
		  ret[j++] = c;
		}
	      else
		{
		  hexa = lw6sys_new_sprintf (sys_context, _ESCAPE_HTTP_URI_MASK, (int) c);
		  if (hexa)
		    {
		      ret[j++] = '%';
		      if (strlen (hexa) == 2)
			{
			  ret[j++] = hexa[0];
			  ret[j++] = hexa[1];
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("incorrect hexa conversion for %d \"%s\""), (int) c, hexa);
			}
		      LW6SYS_FREE (sys_context, hexa);
		    }
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("invalid character %d in uri"), (int) c);
	    }
	}
      if (j <= _ESCAPE_HTTP_URI_LEN * len)
	{
	  ret[j++] = '\0';
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("http uri escape string too long %d for %d/%d"), j, _ESCAPE_HTTP_URI_LEN * len, len);
	}
    }

  return ret;
}

/**
 * lw6sys_escape_html_attribute
 *
 * @sys_context: global system context
 * @src: the string to escape
 *
 * Transforms a string so that it can fit in a html field,
 * this is typically for alt="" or title="" fields so it
 * will convert a double quote into its equivalent escaped char.
 *
 * Return value: newly allocated string.
 */
char *
lw6sys_escape_html_attribute (lw6sys_context_t * sys_context, const char *src)
{
  char *ret = NULL;
  int len = 0;
  int i = 0, j = 0, k = 0;
  unsigned char c;
  char *quot = NULL;

  src = lw6sys_str_empty_if_null (sys_context, src);
  len = lw6sys_imin (strlen (src), _ESCAPE_HTML_ATTRIBUTE_MAX_LEN);
  ret = (char *) LW6SYS_CALLOC (sys_context, _ESCAPE_HTML_ATTRIBUTE_LEN * len + 1);
  if (ret)
    {
      for (i = 0, j = 0; i < len && j < _ESCAPE_HTML_ATTRIBUTE_MAX_LEN; ++i)
	{
	  c = src[i];
	  c = lw6sys_imax (' ', c);
	  if (c != _ESCAPE_HTML_ATTRIBUTE_QUOT_CHAR
	      && c != _ESCAPE_HTML_ATTRIBUTE_LT_CHAR && c != _ESCAPE_HTML_ATTRIBUTE_GT_CHAR && c != _ESCAPE_HTML_ATTRIBUTE_AMP_CHAR)
	    {
	      ret[j++] = c;
	    }
	  else
	    {
	      switch (c)
		{
		case _ESCAPE_HTML_ATTRIBUTE_QUOT_CHAR:
		  quot = _ESCAPE_HTML_ATTRIBUTE_QUOT_STR;
		  break;
		case _ESCAPE_HTML_ATTRIBUTE_LT_CHAR:
		  quot = _ESCAPE_HTML_ATTRIBUTE_LT_STR;
		  break;
		case _ESCAPE_HTML_ATTRIBUTE_GT_CHAR:
		  quot = _ESCAPE_HTML_ATTRIBUTE_GT_STR;
		  break;
		case _ESCAPE_HTML_ATTRIBUTE_AMP_CHAR:
		  quot = _ESCAPE_HTML_ATTRIBUTE_AMP_STR;
		  break;
		default:
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to find a substitude for char %d"), (int) c);
		  quot = "";
		}
	      for (k = 0; k < _ESCAPE_HTML_ATTRIBUTE_LEN && quot[k]; ++k)
		{
		  ret[j++] = quot[k];
		}
	    }
	}
      if (j <= _ESCAPE_HTML_ATTRIBUTE_LEN * len)
	{
	  ret[j++] = '\0';
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("html attribute escape string too long %d for %d/%d"), j, _ESCAPE_HTML_ATTRIBUTE_LEN * len, len);
	}
    }

  return ret;
}

/**
 * lw6sys_escape_sql_value
 *
 * @sys_context: global system context
 * @src: the string to escape
 *
 * Transforms a string so that it can fit as an SQL parameter,
 * it will get rid
 * URL chars, it will mostly convert chars over 128 into an
 * hexadecimal form which replaces them in URLs.
 *
 * Return value: newly allocated string.
 */
char *
lw6sys_escape_sql_value (lw6sys_context_t * sys_context, const char *src)
{
  char *ret = NULL;
  int len = 0;
  int i = 0, j = 0, k = 0;
  unsigned char c;
  char *quot = NULL;

  src = lw6sys_str_empty_if_null (sys_context, src);
  len = lw6sys_imin (strlen (src), _ESCAPE_SQL_VALUE_MAX_LEN);
  ret = (char *) LW6SYS_CALLOC (sys_context, _ESCAPE_SQL_VALUE_LEN * len + 1);
  if (ret)
    {
      for (i = 0, j = 0; i < len && j < _ESCAPE_SQL_VALUE_MAX_LEN; ++i)
	{
	  c = src[i];
	  c = lw6sys_imax (' ', c);
	  if (c != _ESCAPE_SQL_VALUE_QUOT_CHAR)
	    {
	      ret[j++] = c;
	    }
	  else
	    {
	      quot = _ESCAPE_SQL_VALUE_QUOT_STR;
	      for (k = 0; k < _ESCAPE_SQL_VALUE_LEN && quot[k]; ++k)
		{
		  ret[j++] = quot[k];
		}
	    }
	}
      if (j <= _ESCAPE_SQL_VALUE_LEN * len)
	{
	  ret[j++] = '\0';
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("sql value escape string too long %d for %d/%d"), j, _ESCAPE_SQL_VALUE_LEN * len, len);
	}
    }

  return ret;
}
