/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013  Christian Mauduit <ufoot@ufoot.org>

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

#ifdef LW6_CONSOLE
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
/*
 * Need stdio.h for some old readline headers, see bug #24315 on Savannah
 */
#include <stdio.h>
#include <readline/readline.h>
#endif

#include "cns.h"

#define PROMPT "lw6> "

/**
 * lw6cns_handler_install
 *
 * @callback: handler function.
 *
 * Installs a console handler.
 *
 * Return value: none.
 */
void
lw6cns_handler_install (lw6cns_callback_func_t callback)
{
#ifdef LW6_CONSOLE
  fflush (stdin);
  fflush (stdout);
  fflush (stderr);

  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("opening console"));

  rl_set_keyboard_input_timeout (1);
  rl_callback_handler_install (PROMPT, callback);
#else
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("no console support"));
#endif
}

/**
 * lw6cns_handler_poll
 *
 * Polling function for console, must be called on a regular basis.
 *
 * Return value: none.
 */
void
lw6cns_handler_poll ()
{
#ifdef LW6_CONSOLE
  fd_set rfds;
  struct timeval tv;
  int retval = 1;
  int c;

  /*
   * It's important to have a while here, for characters
   * are read one by one, and input could be slowed if
   * we had a simple "if".
   */
  while (retval)
    {
      FD_ZERO (&rfds);
      FD_SET (0, &rfds);	// stdin == fd 0

      tv.tv_sec = 0;
      tv.tv_usec = 0;

      retval = select (1, &rfds, NULL, NULL, &tv);

      if (retval > 0 && FD_ISSET (0, &rfds) && !feof (stdin))
	{
	  c = fgetc (stdin);
	  ungetc (c, stdin);
	  rl_callback_read_char ();
	}
    }
#endif
}

/**
 * lw6cns_handler_remove
 * 
 * Remove console handler.
 *
 * Return value: none.
 */
void
lw6cns_handler_remove ()
{
#ifdef LW6_CONSOLE
  rl_callback_handler_remove ();
  fprintf (stdout, "\n");
  fflush (stdout);

  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("console closed"));
#endif
}
