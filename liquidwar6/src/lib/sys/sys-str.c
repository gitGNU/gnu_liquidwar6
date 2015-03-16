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

#include <ctype.h>

#include "sys.h"
#include "sys-internal.h"

#ifdef LW6_MS_WINDOWS
#define STR_EOL "\r\n"
#else // LW6_MS_WINDOWS
#define STR_EOL "\n"
#endif // LW6_MS_WINDOWS

#define _STR_BIN_TEXT_OTHER ".,:;!?()'\"-+*/=_ \t\r\n"
#define _STR_BIN_TEXT_MIN_PERCENT 75
/*
 * OK, we don't use Z in our random words, for a common usage is to generate strings
 * which will later be used as network test messages, and our "if it starts with Z then
 * it's Z-compressed" hack can be fooled by this. In practice not using z or Z still
 * makes random enough messages.
 */
#define _STR_RANDOM_WORD "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYabcdefghijklmnopqrstuvwxy"
#define _STR_RANDOM_WORDS "     0123456789ABCDEFGHIJKLMNOPQRSTUVWXYabcdefghijklmnopqrstuvwxy"

typedef struct _join_callback_data_s
{
  char *ret;
  const char *glue;
} _join_callback_data_t;

/**
 * lw6sys_str_copy
 *
 * @sys_context: global system context
 * @src: the string to copy
 *
 * Duplicate a string, creating a new pointer on it, which
 * must be freed afterwards. The main difference with @strdup
 * is that here we use the LW6SYS_MALLOC macro to track down
 * possible memory leaks.
 *
 * Return value: a newly allocated pointer, must be freed.
 */
char *
lw6sys_str_copy (lw6sys_context_t * sys_context, const char *src)
{
  char *copy = NULL;
  int length;

  length = strlen (src);
  copy = LW6SYS_MALLOC (sys_context, length + 1);
  if (copy)
    {
      strncpy (copy, src, length + 1);
      copy[length] = '\0';
    }

  return copy;
}

/**
 * lw6sys_str_concat
 *
 * @sys_context: global system context
 * @str1: the left part to be concatenated
 * @str2: the right part to be concatenated
 *
 * Concatenate 2 strings, and put the result in a newly
 * allocated string. Unlike @strcat which uses the same
 * pointer.
 *
 * Return value: a newly allocated pointer, must be freed.
 */
char *
lw6sys_str_concat (lw6sys_context_t * sys_context, const char *str1, const char *str2)
{
  char *concat = NULL;
  int length;

  length = strlen (str1) + strlen (str2);
  concat = LW6SYS_MALLOC (sys_context, length + 1);
  if (concat)
    {
      snprintf (concat, length + 1, "%s%s", str1, str2);
      concat[length] = '\0';
    }

  return concat;
}

char *
_lw6sys_new_vsnprintf (lw6sys_context_t * sys_context, int n, const char *fmt, va_list ap)
{
  char *ret = NULL;
  int written;

  if (n > 0)
    {
      ret = (char *) LW6SYS_MALLOC (sys_context, n);

      if (ret)
	{
	  written = vsnprintf (ret, n, fmt, ap);
	  if (written < 0 || written >= n)
	    {
	      /*
	       * Problem, either error or string too long.
	       */
	      LW6SYS_FREE (sys_context, ret);
	      ret = NULL;
	    }
	  else
	    {
	      ret[n - 1] = '\0';
	    }
	}
    }

  return ret;
}

static int
more_mem (int n)
{
  return (((n * 15) / 10) + 1);	// +50%+1
}

/**
 * lw6sys_new_sprintf
 *
 * @sys_context: global system context
 * @fmt: a format string, like the one you would pass to @printf
 * @...: optional arguments, like the ones you would pass to @printf
 *
 * An sprintf like function, except it allocates a new
 * string automatically, with "enough space". This is not a highly
 * optimized function, it will allocate plenty of memory, possibly
 * several times, and thus consume time and resources. But it has
 * the great advantage of freeing the programmer of the dirty
 * work of guessing "how log will the sprintf'ed string be?" before
 * even generating it. So it's a time saver for the programmer.
 * Additionnally, helps avoiding memory leaks and buffer overflows.
 *
 * Return value: a new allocated string, must be freed.
 */
char *
lw6sys_new_sprintf (lw6sys_context_t * sys_context, const char *fmt, ...)
{
  char *ret = NULL;
  va_list ap;

  if (fmt != NULL)
    {
      va_start (ap, fmt);

      int n = more_mem (strlen (fmt));

      while (ret == NULL)
	{
	  va_list ap2;

	  va_copy (ap2, ap);
	  ret = _lw6sys_new_vsnprintf (sys_context, n, fmt, ap2);
	  va_end (ap2);

	  /*
	   * Loop until we have our string!
	   * In case there is actually no more memory
	   * this will loop forever. In case there's no more
	   * memory for a simple, game and possibly machine
	   * will collapse anyway...
	   */
	  n = more_mem (n);
	}

      va_end (ap);
    }

  return ret;
}

int
_lw6sys_buf_vsnprintf (lw6sys_context_t * sys_context, char *buf, int len, const char *fmt, va_list ap)
{
  int ret = 0;

  /*
   * buf has at least one char for it's len+1 so even
   * with len=0 buf[0] is fine.
   */
  buf[0] = '\0';

  /*
   * Always call this first to call a segfault if buf is badly
   * sized, better have this error right now than later
   * "we don't know when".
   */
  buf[len] = '\0';

  /*
   * Note: it's important here *not* to log anything,
   * this function is typically called by internal
   * low-level function which do not want to lock
   * anything calling malloc or something.
   */
  ret = (vsnprintf (buf, len, fmt, ap) >= 0);

  return ret;
}

/**
 * lw6sys_buf_sprintf
 *
 * @sys_context: global system context
 * @buf: a buffer of len+1 chars
 * @len: the max length of string
 * @fmt: a format string, like the one you would pass to @printf
 * @...: optional arguments, like the ones you would pass to @printf
 *
 * Almost like snprintf except that it will *always* append a
 * char 0 ('\0') at the end of the string. Therefore buf
 * must be of size len+1.
 *
 * Return value: 1 if success, 0 if failed.
 */
int
lw6sys_buf_sprintf (lw6sys_context_t * sys_context, char *buf, int len, const char *fmt, ...)
{
  int ret = 0;
  va_list ap;

  /*
   * buf has at least one char for it's len+1 so even
   * with len=0 buf[0] is fine.
   */
  buf[0] = '\0';

  if (fmt != NULL)
    {
      va_start (ap, fmt);
      /*
       * Note: it's important here *not* to log anything,
       * this function is typically called by internal
       * low-level function which do not want to lock
       * anything calling malloc or something.
       */
      ret = _lw6sys_buf_vsnprintf (sys_context, buf, len, fmt, ap);
      va_end (ap);
    }

  return ret;
}

/**
 * lw6sys_str_is_blank
 *
 * @sys_context: global system context
 * @str: the string to test
 *
 * Tests wether a string is blank, that is, if it's composed
 * of space, tabs, or carriage returns only.
 *
 * Return value: 1 if blank, 0 if not.
 */
int
lw6sys_str_is_blank (lw6sys_context_t * sys_context, const char *str)
{
  int ret = 1;
  int pos = 0;

  while (ret && str[pos])
    {
      /*
       * A simple >32 test is enough to exclude blanks (' ')
       * but will also get rid of '\t', '\n', '\r', and in
       * a general all characters but real stuff.
       */
      if (str[pos] > 32)
	{
	  ret = 0;
	}
      pos++;
    }

  return ret;
}

/**
 * lw6sys_str_is_null_or_empty
 *
 * @sys_context: global system context
 * @str: the string to test
 *
 * Tests wether a string is NULL or empty (string with 0 chars "").
 *
 * Return value: 1 if NULL or empty, 0 if contains something.
 */
int
lw6sys_str_is_null_or_empty (lw6sys_context_t * sys_context, const char *str)
{
  return (str == NULL || str[0] == '\0');
}

/**
 * lw6sys_str_empty_if_null
 *
 * @sys_context: global system context
 * @str: the string to test
 *
 * Returns always a non-NULL string, if string is NULL, returns ""
 * The argument @str is not passed as const else this function
 * would equate to a disguised cast from const to non-const.
 *
 * Return value: source string or "" if it was NULL
 */
const char *
lw6sys_str_empty_if_null (lw6sys_context_t * sys_context, const char *str)
{
  return str ? str : LW6SYS_STR_EMPTY;
}

/**
 * lw6sys_str_is_same
 *
 * @sys_context: global system context
 * @str_a: 1st string to compare, can be NULL
 * @str_b: 2nd string to compare, can be NULL
 *
 * Compares two strings for equality. Difference with strcmp
 * is that this one won't check for alphabetical order and
 * return -1 or +1, but will check for NULL args.
 * of space, tabs, or carriage returns only.
 *
 * Return value: 1 if same, 0 if not.
 */
int
lw6sys_str_is_same (lw6sys_context_t * sys_context, const char *str_a, const char *str_b)
{
  int ret = 1;

  if (((!str_a) && str_b) || (str_a && (!str_b)))
    {
      ret = 0;
    }
  if (str_a && str_b)
    {
      ret = strcmp (str_a, str_b) ? 0 : 1;
    }

  return ret;
}

/**
 * lw6sys_str_is_same_no_case
 *
 * @sys_context: global system context
 * @str_a: 1st string to compare, can be NULL
 * @str_b: 2nd string to compare, can be NULL
 *
 * Compares two strings for equality. Difference with strcmp
 * is that this one won't check for alphabetical order and
 * return -1 or +1, but will check for NULL args.
 * of space, tabs, or carriage returns only. This function
 * is not case sensitive.
 *
 * Return value: 1 if same, 0 if not.
 */
int
lw6sys_str_is_same_no_case (lw6sys_context_t * sys_context, const char *str_a, const char *str_b)
{
  int ret = 1;

  if (((!str_a) && str_b) || (str_a && (!str_b)))
    {
      ret = 0;
    }
  if (str_a && str_b)
    {
      ret = strcasecmp (str_a, str_b) ? 0 : 1;
    }

  return ret;
}

/**
 * lw6sys_str_starts_with
 *
 * @sys_context: global system context
 * @str: the string to analyse
 * @beginning: the pattern to search
 *
 * Tells wether string starts with a given beginning.
 *
 * Return value: 1 if @str starts with @beginning, 0 if not
 */
int
lw6sys_str_starts_with (lw6sys_context_t * sys_context, const char *str, const char *beginning)
{
  int ret = 0;

  ret = !strncmp (str, beginning, strlen (beginning));

  return ret;
}

/**
 * lw6sys_str_starts_with_no_case
 *
 * @sys_context: global system context
 * @str: the string to analyse
 * @beginning: the pattern to search
 *
 * Tells wether string starts with a given beginning. This function
 * is not case sensitive.
 *
 * Return value: 1 if @str starts with @beginning, 0 if not
 */
int
lw6sys_str_starts_with_no_case (lw6sys_context_t * sys_context, const char *str, const char *beginning)
{
  int ret = 0;

  ret = !strncasecmp (str, beginning, strlen (beginning));

  return ret;
}

/**
 * lw6sys_skip_blanks
 *
 * @sys_context: global system context
 * @str_ptr: a pointer to a string pointer (read/write parameter).
 *
 * Skips blanks at the beginning of a string. The passed parameter
 * is modifed in place. Usefull for parsing.
 *
 * Return value: 1 if blanks were found, else 0.
 */
int
lw6sys_skip_blanks (lw6sys_context_t * sys_context, char **str_ptr)
{
  int ret = 0;

  while (lw6sys_chr_is_space (**str_ptr))
    {
      ret = 1;
      (*str_ptr)++;
    }

  return ret;
}

/**
 * lw6sys_str_cleanup
 *
 * @str: a pointer to the string, which will be modified in-place.
 *
 * Used to clean up some strings, for instance if they
 * come from the network, we don't necessarly want system
 * chars to be displayed on the console. Basically it removes
 * all characters with an ASCII code inferior to 32, that is,
 * all system characters. This way, there won't be any tab,
 * linefeed, or any of such characters left.
 *
 * Return value: none.
 */
void
lw6sys_str_cleanup (lw6sys_context_t * sys_context, char *str)
{
  /*
   * It's important to have pos unsigned here else characters
   * over 127 are handled differently.
   */
  unsigned char *pos;

  pos = (unsigned char *) str;
  while (*pos)
    {
      if ((*pos) < 32)
	{
	  (*pos) = ' ';
	}
      pos++;
    }
}

/**
 * lw6sys_str_cleanup_ascii7
 *
 * @sys_context: global system context
 * @str: a pointer to the string, which will be modified in-place.
 *
 * Used to clean up some strings, for instance if they
 * come from the network, we don't necessarly want system
 * chars to be displayed on the console. Basically it removes
 * all characters with an ASCII code inferior to 32, that is,
 * all system characters. This way, there won't be any tab,
 * linefeed, or any of such characters left. This function
 * will even remove any character above ASCII 127.
 *
 * Return value: none.
 */
void
lw6sys_str_cleanup_ascii7 (lw6sys_context_t * sys_context, char *str)
{
  /*
   * It's important to have pos unsigned here else characters
   * over 127 are handled differently.
   */
  unsigned char *pos;

  pos = (unsigned char *) str;
  while (*pos)
    {
      if ((*pos) < 32 || (*pos) > 127)
	{
	  (*pos) = ' ';
	}
      pos++;
    }
}

static void
_reformat_newline (lw6sys_context_t * sys_context, char **formatted_str, char *append, const char *prefix)
{
  char *new_str = NULL;

  lw6sys_str_cleanup (sys_context, append);
  lw6sys_skip_blanks (sys_context, &append);
  new_str = lw6sys_new_sprintf (sys_context, "%s%s%s%s", *formatted_str, prefix, append, lw6sys_eol ());
  if (new_str)
    {
      LW6SYS_FREE (sys_context, *formatted_str);
      (*formatted_str) = new_str;
    }
}

/**
 * lw6sys_str_reformat
 *
 * @sys_context: global system context
 * @str: a pointer to the string we want to modify
 * @prefix: a prefix to put before each line
 * @nb_colummns: number of columns to use, without prefix.
 *
 * Reformats a string, that is, insert newline characters in the
 * right places to that it fits in a given number of columns.
 * A prefix is appended at the beginning of each line.
 * Will not handle strings which already contain newline
 * characters perfectly.
 *
 * Return value: a newly allocated string, must be freed.
 */
char *
lw6sys_str_reformat (lw6sys_context_t * sys_context, const char *str, const char *prefix, int nb_columns)
{
  char *ret = NULL;
  char *str_copy = NULL;
  char *line_start = NULL;
  char *pos = NULL;
  int i = 0;
  int space_i = 0;

  nb_columns = lw6sys_imax (nb_columns, 1);
  str_copy = lw6sys_str_copy (sys_context, str);
  if (str_copy)
    {
      ret = lw6sys_str_copy (sys_context, "");
      line_start = str_copy;
      i = 0;
      pos = str_copy;
      while (pos[0] && ret)
	{
	  if (lw6sys_chr_is_eol (pos[0]))
	    {
	      while (lw6sys_chr_is_eol (pos[1]))
		{
		  pos++;
		}
	      pos[0] = '\0';
	      _reformat_newline (sys_context, &ret, line_start, prefix);
	      line_start = pos + 1;
	      i = 0;
	      pos = line_start;
	    }
	  else if (i >= nb_columns)
	    {
	      space_i = 0;
	      while (i > 0)
		{
		  if (lw6sys_chr_is_space (line_start[i]))
		    {
		      space_i = i;
		      break;
		    }
		  i--;
		}
	      if (!space_i)
		{
		  i = 1;
		  while (line_start[i] && !space_i)
		    {
		      if (lw6sys_chr_is_eol (line_start[i]) || lw6sys_chr_is_space (line_start[i]))
			{
			  space_i = i;
			  break;
			}
		      i++;
		    }
		  if (!space_i)
		    {
		      space_i = i;
		    }
		}
	      if (space_i)
		{
		  line_start[space_i] = '\0';
		  _reformat_newline (sys_context, &ret, line_start, prefix);
		  line_start += (space_i + 1);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("reformat problem"));
		  line_start += i;
		}
	      i = 0;
	      pos = line_start;
	    }
	  else
	    {
	      i++;
	      pos++;
	    }
	}

      if (ret && line_start)
	{
	  _reformat_newline (sys_context, &ret, line_start, prefix);
	}

      LW6SYS_FREE (sys_context, str_copy);
    }

  return ret;
}

/**
 * lw6sys_str_reformat_this
 *
 * @sys_context: global system context
 * @str: a pointer to the string we want to modify
 * @nb_colummns: number of columns to use, without prefix.
 *
 * Reformats a string, that is, insert newline characters in the
 * right places to that it fits in a given number of columns.
 * This function will modify the buffer so @str must be writeable.
 * Will not handle strings which already contain newline
 * characters perfectly.
 *
 * Return value: none
 */
void
lw6sys_str_reformat_this (lw6sys_context_t * sys_context, char *str, int nb_columns)
{
  int i = 0;
  char *pos = NULL;
  char *last_space = NULL;

  nb_columns = lw6sys_imax (nb_columns, 1);
  pos = str;
  while (*pos)
    {
      if (i > nb_columns && last_space)
	{
	  (*last_space) = '\n';
	  i = (pos - last_space);
	  last_space = NULL;
	}
      if (lw6sys_chr_is_space (*pos))
	{
	  last_space = pos;
	}
      pos++;
      i++;
    }
}

/**
 * lw6sys_eol
 *
 * @sys_context: global system context
 *
 * Returns the value of EOL, that is, the "end of line" sequence.
 * Will simply return "\n" on UNIX and "\r\n" on Microsoft platforms.
 * Note that while this is convenient to write config and example files,
 * for instance, it's a bad idea to use this to generate network messages,
 * because this kind of message needs to be platform independant. Thus any
 * network protocol oriented string would use chr(10) and char(13) directly.
 *
 * Return value: the EOL string, must not be freed.
 */
char *
lw6sys_eol (lw6sys_context_t * sys_context)
{
  return STR_EOL;
}

/**
 * lw6sys_str_split
 *
 * @sys_context: global system context
 * @str: a string
 * @c: the delimiter to split with
 *
 * Splits a string, for instance 'foo,bar' splited with 'o' will
 * return 'f', '' and ',bar'.
 *
 * Return value: a list containing 0-terminated strings.
 */
lw6sys_list_t *
lw6sys_str_split (lw6sys_context_t * sys_context, const char *str, char c)
{
  lw6sys_list_t *ret = NULL;
  char *found = NULL;
  char *pos = NULL;
  char *copy = NULL;
  char *part = NULL;

  copy = lw6sys_str_copy (sys_context, str);
  if (copy)
    {
      ret = lw6sys_list_new (sys_context, lw6sys_free_callback);
      if (ret)
	{
	  pos = copy;
	  while ((found = strchr (pos, c)) != NULL)
	    {
	      (*found) = '\0';
	      part = lw6sys_str_copy (sys_context, pos);
	      if (part)
		{
		  lw6sys_list_push_back (sys_context, &ret, part);
		}
	      pos = found + 1;
	    }
	  part = lw6sys_str_copy (sys_context, pos);
	  if (part)
	    {
	      lw6sys_list_push_back (sys_context, &ret, part);
	    }
	}
      LW6SYS_FREE (sys_context, copy);
    }

  return ret;
}

/**
 * lw6sys_str_split_no_0
 *
 * @sys_context: global system context
 * @str: a string
 * @c: the delimiter to split with
 *
 * Splits a string, ignoring empty '0-length' members. For instance
 * 'foo,bar' splited with 'o' will return 'f' and ',bar'.
 *
 * Return value: a list containing 0-terminated strings.
 */
lw6sys_list_t *
lw6sys_str_split_no_0 (lw6sys_context_t * sys_context, const char *str, char c)
{
  lw6sys_list_t *ret = NULL;
  char *found = NULL;
  char *pos = NULL;
  char *copy = NULL;
  char *part = NULL;

  copy = lw6sys_str_copy (sys_context, str);
  if (copy)
    {
      ret = lw6sys_list_new (sys_context, lw6sys_free_callback);
      if (ret)
	{
	  pos = copy;
	  while ((found = strchr (pos, c)) != NULL)
	    {
	      (*found) = '\0';
	      part = lw6sys_str_copy (sys_context, pos);
	      if (part)
		{
		  if ((*part) != '\0')
		    {
		      lw6sys_list_push_back (sys_context, &ret, part);
		    }
		  else
		    {
		      LW6SYS_FREE (sys_context, part);
		    }
		}
	      pos = found + 1;
	    }
	  part = lw6sys_str_copy (sys_context, pos);
	  if (part)
	    {
	      if ((*part) != '\0')
		{
		  lw6sys_list_push_back (sys_context, &ret, part);
		}
	      else
		{
		  LW6SYS_FREE (sys_context, part);
		}
	    }
	}
      LW6SYS_FREE (sys_context, copy);
    }

  return ret;
}

/**
 * lw6sys_str_split_config_item
 *
 * @sys_context: global system context
 * @str: a string
 *
 * Splits a string, ignoring empty '0-length' members, and using
 * the comma ',' as a separator. This is typically usefull
 * for config elements such as backend lists. Only paths need
 * another separator (platform-dependant).
 *
 * Return value: a list containing 0-terminated strings.
 */
lw6sys_list_t *
lw6sys_str_split_config_item (lw6sys_context_t * sys_context, const char *str)
{
  return lw6sys_str_split_no_0 (sys_context, str, ',');
}

static void
_join_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  _join_callback_data_t *join_callback_data = (_join_callback_data_t *) func_data;
  char *str = (char *) data;
  char *tmp = NULL;

  if (join_callback_data->ret)
    {
      tmp = join_callback_data->ret;
      join_callback_data->ret = lw6sys_new_sprintf (sys_context, "%s%s%s", join_callback_data->ret, join_callback_data->glue, str);
      LW6SYS_FREE (sys_context, tmp);
    }
  else
    {
      join_callback_data->ret = lw6sys_str_copy (sys_context, str);
    }
}

/**
 * lw6sys_str_join
 *
 * @sys_context: global system context
 * @list: list of strings to join
 * @glue: string to add in-between
 *
 * Companion function of @lw6sys_str_split which will do
 * the contrary and join the string. Here we use a string
 * as the glue/separator, more flexible than a simple char
 * in this case.
 *
 * Return value: dynamically allocated string
 */
char *
lw6sys_str_join (lw6sys_context_t * sys_context, lw6sys_list_t * list, const char *glue)
{
  char *ret = NULL;
  _join_callback_data_t join_callback_data;

  join_callback_data.ret = ret;
  join_callback_data.glue = glue;

  lw6sys_list_map (sys_context, list, _join_callback, &join_callback_data);

  ret = join_callback_data.ret;

  return ret;
}

/**
 * lw6sys_str_toupper
 *
 * @sys_context: global system context
 * @str: the string to modify
 *
 * Transforms a string to upper case, the pointer must point
 * to modifiable data.
 *
 * Return value: none, @str pointed data modified in-place
 */
void
lw6sys_str_toupper (lw6sys_context_t * sys_context, char *str)
{
  char *pos = str;

  while (pos[0])
    {
      pos[0] = toupper (pos[0]);
      pos++;
    }
}

/**
 * lw6sys_str_tolower
 *
 * @sys_context: global system context
 * @str: the string to modify
 *
 * Transforms a string to lower case, the pointer must point
 * to modifiable data.
 *
 * Return value: none, @str pointed data modified in-place
 */
void
lw6sys_str_tolower (lw6sys_context_t * sys_context, char *str)
{
  char *pos = str;

  while (pos[0])
    {
      pos[0] = tolower (pos[0]);
      pos++;
    }
}

/**
 * lw6sys_str_truncate
 *
 * @sys_context: global system context
 * @str: the string to truncate
 * @len: the new length
 *
 * Truncates a string to the max given length. If truncated to 3,
 * "abcdef" becomes "abc".
 *
 * Return value: none, @str pointed data modified in-place
 */
void
lw6sys_str_truncate (lw6sys_context_t * sys_context, char *str, int len)
{
  if (strlen (str) > len)
    {
      str[len] = '\0';
    }
}

/**
 * lw6sys_str_truncate_middle
 *
 * @sys_context: global system context
 * @str: the string to truncate
 * @len: the new length
 * @middle: the string to add in the middle
 *
 * Truncates a string to the max given length, by truncating
 * the middle of the string, and putting the string middle
 * at this place. Calling it with "abcdefghijk",5,"X" will give
 * "abXjk".
 *
 * Return value: none, @str pointed data modified in-place
 */
void
lw6sys_str_truncate_middle (lw6sys_context_t * sys_context, char *str, int len, const char *middle)
{
  int orig_len = strlen (str);

  if (orig_len > len)
    {
      int middle_len = 0;
      int middle_pos = 0;
      int tail_len = 0;
      int tail_pos = 0;

      if (middle)
	{
	  middle_len = lw6sys_imin (len, strlen (middle));
	}
      middle_pos = (len - middle_len) / 2;
      if (middle && middle_len > 0)
	{
	  memcpy (str + middle_pos, middle, middle_len);
	}
      tail_pos = middle_pos + middle_len;
      tail_len = lw6sys_imax (0, len - tail_pos);
      if (tail_len > 0)
	{
	  memcpy (str + tail_pos, str + (orig_len - tail_len), tail_len);
	}
      str[len] = '\0';
    }
}

/**
 * lw6sys_str_random
 *
 * @sys_context: global system context
 * @len: the length of the random string to generate.
 *
 * Generates a random string, this is usefull for testing.
 *
 * Return value: newly allocated string
 */
char *
lw6sys_str_random (lw6sys_context_t * sys_context, int len)
{
  unsigned char *ret = NULL;
  int i = 0;

  ret = (unsigned char *) LW6SYS_MALLOC (sys_context, len + 1);
  for (i = 0; i < len; ++i)
    {
      ret[i] = lw6sys_random (sys_context, 254) + 1;
    }
  ret[len] = 0;

  return (char *) ret;
}

/**
 * lw6sys_str_random_words
 *
 * @sys_context: global system context
 * @len: the length of the random string to generate.
 *
 * Generates a random string, this is usefull for testing.
 * This version only generates words with alpha-numerical
 * content (letters and digits plus spaces).
 *
 * Return value: newly allocated string
 */
char *
lw6sys_str_random_words (lw6sys_context_t * sys_context, int len)
{
  unsigned char *ret = NULL;
  int i = 0;
  int cat_len = strlen (_STR_RANDOM_WORDS);

  ret = (unsigned char *) LW6SYS_MALLOC (sys_context, len + 1);
  for (i = 0; i < len; ++i)
    {
      ret[i] = _STR_RANDOM_WORDS[lw6sys_random (sys_context, cat_len)];
    }
  ret[len] = 0;

  return (char *) ret;
}

/**
 * lw6sys_str_random_word
 *
 * @sys_context: global system context
 * @len: the length of the random string to generate.
 *
 * Generates a random string, this is usefull for testing.
 * This version generates on single word with alpha-numerical
 * content (letters and digits but no spaces).
 *
 * Return value: newly allocated string
 */
char *
lw6sys_str_random_word (lw6sys_context_t * sys_context, int len)
{
  unsigned char *ret = NULL;
  int i = 0;
  int cat_len = strlen (_STR_RANDOM_WORD);

  ret = (unsigned char *) LW6SYS_MALLOC (sys_context, len + 1);
  for (i = 0; i < len; ++i)
    {
      ret[i] = _STR_RANDOM_WORD[lw6sys_random (sys_context, cat_len)];
    }
  ret[len] = 0;

  return (char *) ret;
}

static int
_chr_is_text (char c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (strchr (_STR_BIN_TEXT_OTHER, c) != NULL);
}

/**
 * lw6sys_str_is_bin
 *
 * @sys_context: global system context
 * @buf: the buffer to test
 * @len: the length of the buffer
 *
 * Tests wether a buffer is likely to contain a string. This is not
 * a bulletproof function, just a simple heuristic based estimator.
 *
 * Return value: 1 if probably binary, 0 if probably text
 */
int
lw6sys_str_is_bin (lw6sys_context_t * sys_context, const char *buf, int len)
{
  int ret = 0;
  int text_chars = 0;
  int i = 0;

  if (len > 0)
    {
      for (i = 0; i < len; ++i)
	{
	  if (_chr_is_text (buf[i]))
	    {
	      text_chars++;
	    }
	}
      if (((text_chars * 100) / len) < _STR_BIN_TEXT_MIN_PERCENT)
	{
	  ret = 1;
	}
    }

  return ret;
}
