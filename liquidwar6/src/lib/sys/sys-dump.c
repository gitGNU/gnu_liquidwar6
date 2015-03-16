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

#define DUMP_FILE "dump.txt"

static char *
get_dump_file (lw6sys_context_t * sys_context, char *user_dir)
{
  char *dump_file = NULL;

  dump_file = lw6sys_path_concat (sys_context, user_dir, DUMP_FILE);

  return dump_file;
}

/**
 * lw6sys_dump_clear
 *
 * @sys_context: global system context
 * @user_dir: the user directory, where user can write data.
 *
 * Clears the dump file. That is, resets it to a "0 byte" file.
 *
 * Return value: none.
 */
void
lw6sys_dump_clear (lw6sys_context_t * sys_context, char *user_dir)
{
  char *dump_file = NULL;

  dump_file = get_dump_file (sys_context, user_dir);
  if (dump_file)
    {
      lw6sys_clear_file (sys_context, dump_file);
      LW6SYS_FREE (sys_context, dump_file);
    }
}

/**
 * lw6sys_dump
 *
 * @sys_context: global system context
 * @user_dir: the user directory, where user can write data.
 * @content: the content to be written in the dump file.
 *
 * Writes the dump file onto the disk. The dump is used for
 * special error messages which do not really fit in the
 * standard log, and require a special treatment. In pratice,
 * it's used to log fatal script (Guile) errors.
 *
 * Return value: 1 if success, 0 if failure.
 */
int
lw6sys_dump (lw6sys_context_t * sys_context, char *user_dir, char *content)
{
  char *dump_file = NULL;
  int ret = 0;

  dump_file = get_dump_file (sys_context, user_dir);
  if (dump_file)
    {
      if (lw6sys_log_get_console_state (sys_context))
	{
	  /*
	   * To log the actual content, we don't use log function, for
	   * they would pollute the CSV and they are not "\n" friendly.
	   */
	  fprintf (stderr, "%s", content);
	  fflush (stderr);
	}
      /*
       * Send a message in log file to tell where the dump is,
       * very convenient to copy-paste then cat file.
       */
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("dump saved in file \"%s\""), dump_file);
      ret = lw6sys_write_file_content (sys_context, dump_file, content);
      LW6SYS_FREE (sys_context, dump_file);
    }

  return ret;
}
