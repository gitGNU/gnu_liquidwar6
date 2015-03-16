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

/**
 * lw6sys_clear_file
 *
 * @sys_context: global system context
 * @filename: absolute or relative filename
 *
 * Clears a file, that is, make it a 0 byte file, empty, ready
 * to be filled if needed. If this function is called successfully,
 * program can reasonnably assume file will be writable during its
 * execution.
 *
 * Return value: 1 if success, 0 if failure.
 */
int
lw6sys_clear_file (lw6sys_context_t * sys_context, const char *filename)
{
  int ret = 0;
  FILE *f = NULL;
  char *dir = NULL;

  dir = lw6sys_path_parent (sys_context, filename);
  if (dir)
    {
      if (!lw6sys_dir_exists (sys_context, dir))
	{
	  lw6sys_create_dir_silent (sys_context, dir);
	}
      LW6SYS_FREE (sys_context, dir);
    }

  f = fopen (filename, "wb");
  if (f)
    {
      fclose (f);
      ret = 1;
    }
  else
    {
      // unwise to log something here, can be called from log_...
    }

  return ret;
}

static void *
_read_bin (lw6sys_context_t * sys_context, int *filesize, const char *filename)
{
  char *file_content = NULL;
  FILE *f;
  int size;
  size_t read = 0;

  f = fopen (filename, "r");
  if (f)
    {
      fseek (f, 0, SEEK_END);
      size = ftell (f);
      if (size >= 0)
	{
	  fseek (f, 0, SEEK_SET);
	  file_content = LW6SYS_CALLOC (sys_context, size + 1);
	  if (file_content)
	    {
	      clearerr (f);
	      while ((!feof (f)) && (!ferror (f)) && (read < size))
		{
		  read += fread (file_content + read, sizeof (char), size - read, f);
		}
	      if (read < size)
		{
		  /*
		   * Under Microsoft platforms, ftell always returns a
		   * bigger, rounded value, greater than the real size.
		   */
#ifndef LW6_MS_WINDOWS
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			      _x_ ("couldn't read all of file \"%s\", size was supposed to be %d but only read %d"), filename, size, (int) read);
#endif
		}
	      else if (read > size)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("inconsistency in file \"%s\", size is %d but read %d"), filename, size, (int) read);
		}
	    }
	}
      fclose (f);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't open file \"%s\" for reading"), filename);
    }

  if (filesize)
    {
      if (file_content)
	{
	  (*filesize) = read;
	}
      else
	{
	  (*filesize) = 0;
	}
    }

  return (void *) file_content;
}

/**
 * lw6sys_read_file_content
 *
 * @sys_context: global system context
 * @filename: absolute or relative filename
 *
 * Reads the content of a file, and returns it as a string.
 * Note that content might or might not be ascii or binary,
 * the function will however put a tailing 0 character
 * at the end so that low-level standard C functions do not
 * segfault when used with the returned value.
 *
 * Return value: a newly allocated pointer, must be freed.
 */
char *
lw6sys_read_file_content (lw6sys_context_t * sys_context, const char *filename)
{
  char *ret = NULL;

  ret = (char *) _read_bin (sys_context, NULL, filename);

  return ret;
}

/**
 * lw6sys_read_file_content_bin
 *
 * @sys_context: global system context
 * @filesize: will contain the file size, in bytes
 * @filename: absolute or relative filename
 *
 * Reads the content of a file, and returns it as a binary
 * buffer. Even if not ascii or binary,
 * the function will however put a tailing 0 character
 * at the end so that low-level standard C functions do not
 * segfault when used with the returned value. This 0 character
 * is not included in @filesize so if there are 4 bytes in the
 * file the 5 bytes will be allocated, this is just for string
 * functions not to explode if called by accident. The @filesize
 * can be NULL, in that case function is just like
 * the @lw6sys_read_file_content function.
 *
 * Return value: a newly allocated pointer, must be freed.
 */
void *
lw6sys_read_file_content_bin (lw6sys_context_t * sys_context, int *filesize, const char *filename)
{
  void *ret = NULL;

  ret = _read_bin (sys_context, filesize, filename);

  return ret;
}

/**
 * lw6sys_write_file_content
 *
 * @sys_context: global system context
 * @filename: absolute or relative filename
 * @content: the content to be written.
 *
 * Writes the content into the file. Content is assumed to
 * be a string, function will segfault if it's not correctly
 * 0 terminated as in C string convention. So this function
 * will not allow you to write down arbitrary binary data,
 * however LW6 uses mostly text files to store information,
 * and opaque binary data usage is not recommended.
 */
int
lw6sys_write_file_content (lw6sys_context_t * sys_context, const char *filename, const char *content)
{
  FILE *f = NULL;
  int ret = 0;

  f = fopen (filename, "wb");
  if (f)
    {
      if (strlen (content) == fprintf (f, "%s", content))
	{
	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to write %d bytes in file \"%s\""), (int) strlen (content), filename);
	}
      fclose (f);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable open file \"%s\" for reading"), filename);
    }

  return ret;
}
