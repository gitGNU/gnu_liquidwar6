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

#define _MAX_FILE_SIZE 1000000

/**
 * lw6sys_stream_file_to_str
 *
 * @sys_context: global system context
 * @f: file to get input from, typically stdin
 *
 * Will read file/stream and return it as a string. This is not
 * for serious stream operation since it will return only when
 * stream is closed, and read all file into memory before doing anything.
 * It's also limited in size since it uses a fixed length buffer,
 * so this is just for quick testing, typically used by command line switches
 * which are used to test encoding/decoding functions. Do not use it
 * to read a filesystem file, @lw6sys_read_file_content is much better.
 *
 * Return value: newly allocated string.
 */
char *
lw6sys_stream_file_to_str (lw6sys_context_t * sys_context, FILE * f)
{
  char *ret = NULL;
  int read = 0;
  int size = _MAX_FILE_SIZE;

  ret = (char *) LW6SYS_MALLOC (sys_context, size + 1);

  if (ret)
    {
      while ((!feof (f)) && (!ferror (f)) && (read < size))
	{
	  read += fread (ret + read, sizeof (char), size - read, f);
	}
      if (read < size)
	{
	  ret[read] = '\0';
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("file too long, function \"%s\" can't handle it"), __FUNCTION__);
	  LW6SYS_FREE (sys_context, ret);
	  ret = NULL;
	}
      if (ret)
	{
	  ret[size - 1] = '\0';
	}
    }

  return ret;
}

/**
 * lw6sys_stream_str_to_file
 *
 * @sys_context: global system context
 * @f: file to receive the string
 * @str: the string to output
 *
 * Here only for API consistency, will just put string to
 * file (just a simple fprint).
 *
 * Return value: none.
 */
void
lw6sys_stream_str_to_file (lw6sys_context_t * sys_context, FILE * f, char *str)
{
  fprintf (f, "%s", str);
}
