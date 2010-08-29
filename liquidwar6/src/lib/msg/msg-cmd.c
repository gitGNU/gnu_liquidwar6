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

#include "msg.h"

/**
 * lw6msg_cmd_generate_hello
 *
 * @info: the nod info to use
 * @text_case: the text case (@LW6MSG_UPPERCASE or @LW6MSG_LOWERCASE) to use
 * @sep: the text separator to use
 *
 * Generate a HELLO command.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_hello (lw6nod_info_t * info, int text_case, char text_sep)
{
  char *ret = NULL;
  char *cmd = NULL;
  char *base64_codename;
  char *base64_url;
  char *base64_title;
  char *base64_descrition;

  cmd = lw6sys_str_copy (LW6MSG_CMD_HELLO);
  if (cmd)
    {
      if (text_case == LW6MSG_LOWERCASE)
	{
	}
      //  ret=lw6sys_new_sprintf("%s%c%s%c");
      LW6SYS_FREE (cmd);
    }

  return ret;
}

/**
 * lw6msg_cmd_generate_ticket
 *
 * @info: the nod info to use
 * @text_case: the text case (@LW6MSG_UPPERCASE or @LW6MSG_LOWERCASE) to use
 * @sep: the text separator to use
 *
 * Generate a TICKET command.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_ticket (lw6nod_info_t * info, int text_case,
			    char text_sep, u_int32_t ticket)
{
  char *ret = NULL;

  return ret;
}

/**
 * lw6msg_cmd_generate_foo
 *
 * @text_case: the text case (@LW6MSG_UPPERCASE or @LW6MSG_LOWERCASE) to use
 * @sep: the text separator to use
 *
 * Generate a FOO command.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_foo (int text_case, char text_sep, int key)
{
  char *ret = NULL;

  return ret;
}

/**
 * lw6msg_cmd_generate_bar
 *
 * @text_case: the text case (@LW6MSG_UPPERCASE or @LW6MSG_LOWERCASE) to use
 * @sep: the text separator to use
 *
 * Generate a BAR command.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_bar (int text_case, char text_sep, int key)
{
  char *ret = NULL;

  return ret;
}

/**
 * lw6msg_cmd_generate_goodbye
 *
 * @text_case: the text case (@LW6MSG_UPPERCASE or @LW6MSG_LOWERCASE) to use
 * @sep: the text separator to use
 *
 * Generate a GOODBYE command.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_goodbye (int text_case, char text_sep)
{
  char *ret = NULL;

  return ret;
}

/**
 * lw6msg_cmd_analyse_hello
 *
 * @info: if not NULL, will contain (remote) node info on success
 *
 * Analyzes a HELLO message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6cmd_analyse_hello (lw6nod_info_t ** info)
{
  int ret = 0;

  return ret;
}

/**
 * lw6msg_cmd_analyse_ticket
 *
 * @info: if not NULL, will contain (remote) node info on success
 * @ticket: if not NULL, will contain the ticket value on success
 *
 * Analyzes a TICKET message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6cmd_analyse_ticket (lw6nod_info_t ** info, u_int32_t * ticket)
{
  int ret = 0;

  return ret;
}


/**
 * lw6msg_cmd_analyse_foo
 *
 * @key: if not NULL, will contain the foo/bar key on success
 *
 * Analyzes a FOO message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6cmd_analyse_foo (int *key)
{
  int ret = 0;

  return ret;
}

/**
 * lw6msg_cmd_analyse_bar
 *
 * @key: if not NULL, will contain the foo/bar key on success
 *
 * Analyzes a BAR message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6cmd_analyse_bar (int *key)
{
  int ret = 0;

  return ret;
}

/**
 * lw6msg_cmd_analyse_goodbye
 *
 * Analyzes a GOODBYE message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6cmd_analyse_goodbye ()
{
  int ret = 0;

  return ret;
}
