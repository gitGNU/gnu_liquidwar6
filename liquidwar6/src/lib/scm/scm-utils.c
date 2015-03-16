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

#include "scm.h"

/**
 * lw6scm_utils_to_0str
 *
 * @sys_context: global system context
 * @string: SCM object to convert
 *
 * Helper function, creates a 0 terminated string
 * from a Guile string. A very common task. This function is just
 * a wrapper that performs type checking, memory allocation around
 * standard Guile function.
 *
 * Return value: newly allocated string, pointer must be freed.
 */
char *
lw6scm_utils_to_0str (lw6sys_context_t * sys_context, SCM string)
{
  char *c_string = NULL;
  int length = 0;
  int c_length = 0;

  if (SCM_NFALSEP (string))
    {
      SCM_ASSERT (scm_is_string (string), string, SCM_ARG1, __FUNCTION__);
      /*
       * See the comment in sys/sys-str.c to see why we use
       * 2 trailing '\0' at the end of the string.
       */
      length = scm_c_string_length (string);
    }

  /*
   * We use a double size + 3 extra bytes to make double-triple
   * sure it's zero terminated, you never know, if it's interpreted
   * in UTF-16, who knows, I just don't want to take any risk,
   * string handling is awfull enough...
   */
  c_length = length * 2 * sizeof (char);
  c_string = (char *) LW6SYS_CALLOC (sys_context, c_length + 3);
  if (c_string)
    {
      if (length > 0)
	{
	  scm_to_locale_stringbuf (string, c_string, c_length);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to convert a guile SCM string to a standard C \"'\\0' terminated\" string"));
    }

  return c_string;
}

/**
 * lw6scm_utils_to_scm_str_list
 *
 * @sys_context: global system context
 * @c_list: list object to convert
 *
 * Transform a C list containing strings to a Guile list containing
 * those strings.
 *
 * Return value: Guile object, a list of strings
 */
SCM
lw6scm_utils_to_scm_str_list (lw6sys_context_t * sys_context, lw6sys_list_t * c_list)
{
  SCM ret = SCM_EOL;
  lw6sys_list_t *c_item = NULL;

  for (c_item = c_list; c_item; c_item = lw6sys_list_next (sys_context, c_item))
    {
      if (c_item->data)
	{
	  ret = scm_cons (scm_from_locale_string ((char *) c_item->data), ret);
	}
    }

  ret = scm_reverse (ret);

  return ret;
}

static void
_to_scm_str_assoc_callback (lw6sys_context_t * sys_context, void *func_data, const char *key, void *value)
{
  SCM *ret_p = (SCM *) func_data;
  char *value_str = (char *) value;

  if (value_str && (SCM_CONSP (*ret_p) || (*ret_p) == SCM_EOL))
    {
      (*ret_p) = scm_cons (scm_cons (scm_from_locale_string (key), scm_from_locale_string (value_str)), *ret_p);
    }
}

/**
 * lw6scm_utils_to_scm_str_assoc
 *
 * @sys_context: global system context
 * @c_assoc: assoc object to convert
 *
 * Transform a C assoc containing strings to a Guile assoc containing
 * those strings.
 *
 * Return value: Guile object, an assoc of strings
 */
SCM
lw6scm_utils_to_scm_str_assoc (lw6sys_context_t * sys_context, lw6sys_assoc_t * c_assoc)
{
  SCM ret = SCM_EOL;

  lw6sys_assoc_map (sys_context, c_assoc, _to_scm_str_assoc_callback, &ret);
  if (SCM_CONSP (ret) || ret == SCM_EOL)
    {
      ret = scm_reverse (ret);
    }

  return ret;
}

/**
 * lw6scm_utils_to_sys_str_list
 *
 * @sys_context: global system context
 * @list: SCM object to convert
 *
 * Creates a C list from an SCM list containing strings.
 * Beware of types, function will fail if members are not
 * all strings, it won't convert them.
 *
 * Return value: new C list object
 */
lw6sys_list_t *
lw6scm_utils_to_sys_str_list (lw6sys_context_t * sys_context, SCM list)
{
  lw6sys_list_t *c_list = NULL;

  c_list = lw6sys_list_new (sys_context, lw6sys_free_callback);
  if (c_list)
    {
      if (SCM_CONSP (list))
	{
	  int i, n;
	  char *c_item;
	  SCM item;

	  n = scm_to_int (scm_length (list));
	  for (i = n - 1; i >= 0; --i)
	    {
	      item = scm_list_ref (list, scm_from_int (i));
	      c_item = lw6scm_utils_to_0str (sys_context, item);
	      if (c_item)
		{
		  lw6sys_list_push_front (sys_context, &c_list, c_item);
		  // do not free c_item, list will do it
		}
	    }
	}
    }

  return c_list;
}

/**
 * lw6scm_utils_to_sys_str_assoc
 *
 * @sys_context: global system context
 * @assoc: SCM object to convert
 *
 * Creates a C assoc from an SCM assoc containing strings.
 * Beware of types, function will fail if members are not
 * all strings, it won't convert them.
 *
 * Return value: new C assoc object
 */
lw6sys_assoc_t *
lw6scm_utils_to_sys_str_assoc (lw6sys_context_t * sys_context, SCM assoc)
{
  lw6sys_assoc_t *c_assoc = NULL;

  c_assoc = lw6sys_assoc_new (sys_context, lw6sys_free_callback);
  if (c_assoc)
    {
      if (SCM_CONSP (assoc))
	{
	  int i, n;
	  char *key, *value;
	  SCM item;

	  n = scm_to_int (scm_length (assoc));
	  for (i = 0; i < n; ++i)
	    {
	      item = scm_list_ref (assoc, scm_from_int (i));
	      if (SCM_CONSP (item))
		{
		  key = lw6scm_utils_to_0str (sys_context, scm_car (item));
		  value = lw6scm_utils_to_0str (sys_context, scm_cdr (item));
		  if (key && value)
		    {
		      lw6sys_assoc_set (sys_context, &c_assoc, key, (void *) value);
		      LW6SYS_FREE (sys_context, key);
		      // value must not be freed now
		    }
		}
	    }
	}
    }

  return c_assoc;
}
