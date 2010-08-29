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
 * @mode: the mode to use (standard or URL compatible)
 *
 * Generate a HELLO command.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_hello (lw6nod_info_t * info, lw6msg_cmd_mode_t mode)
{
  char *ret = NULL;
  char sep = '\0';
  char *cmd = NULL;
  char *base64_codename;
  char *base64_url;
  char *base64_title;
  char *base64_description;
  int uptime = 0;

  cmd = lw6sys_str_copy (LW6MSG_CMD_HELLO);
  if (cmd)
    {
      switch (mode)
	{
	case LW6MSG_CMD_MODE_URL:
	  lw6sys_str_tolower (cmd);
	  sep = LW6MSG_URL_SEP;
	  break;
	case LW6MSG_CMD_MODE_TELNET:
	  lw6sys_str_toupper (cmd);
	  sep = LW6MSG_TELNET_SEP;
	  break;
	default:
	  lw6sys_log (LW6SYS_LOG_WARNING, _("unknown mode %d"), (int) mode);
	  break;
	}

      base64_codename = lw6glb_base64_encode_str (info->const_info.codename);
      if (base64_codename)
	{
	  base64_url = lw6glb_base64_encode_str (info->const_info.url);
	  if (base64_url)
	    {
	      base64_title =
		lw6glb_base64_encode_str (info->const_info.title);
	      if (base64_title)
		{
		  base64_description =
		    lw6glb_base64_encode_str (info->const_info.description);
		  if (base64_description)
		    {
		      uptime =
			(lw6sys_get_timestamp () -
			 info->const_info.creation_timestamp) / 1000;
		      ret =
			lw6sys_new_sprintf
			("%s%c%s%c%s%c%s%c%d%c%s%c%s%c%s%c%s%c%d", cmd, sep,
			 info->const_info.program, sep,
			 info->const_info.version, sep, base64_codename, sep,
			 info->const_info.stamp, sep, info->const_info.id,
			 sep, base64_url, sep, base64_title, sep,
			 base64_description, sep, uptime);
		      LW6SYS_FREE (base64_description);
		    }
		  LW6SYS_FREE (base64_title);
		}
	      LW6SYS_FREE (base64_url);
	    }
	  LW6SYS_FREE (base64_codename);
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
 * @mode: the mode to use (standard or URL compatible)
 *
 * Generate a TICKET command.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_ticket (lw6nod_info_t * info, lw6msg_cmd_mode_t mode,
			    u_int32_t ticket)
{
  char *ret = NULL;

  return ret;
}

/**
 * lw6msg_cmd_generate_foo
 *
 * @mode: the mode to use (standard or URL compatible)
 *
 * Generate a FOO command.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_foo (lw6msg_cmd_mode_t mode, int key)
{
  char *ret = NULL;

  return ret;
}

/**
 * lw6msg_cmd_generate_bar
 *
 * @mode: the mode to use (standard or URL compatible)
 *
 * Generate a BAR command.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_bar (lw6msg_cmd_mode_t mode, int key)
{
  char *ret = NULL;

  return ret;
}

/**
 * lw6msg_cmd_generate_goodbye
 *
 * @mode: the mode to use (standard or URL compatible)
 *
 * Generate a GOODBYE command.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_goodbye (lw6msg_cmd_mode_t mode)
{
  char *ret = NULL;

  return ret;
}

/**
 * lw6msg_cmd_analyse_hello
 *
 * @info: if not NULL, will contain (remote) node info on success
 * @msg: the message to anaylse
 *
 * Analyzes a HELLO message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6cmd_analyse_hello (lw6nod_info_t ** info, char *msg)
{
  int ret = 0;
  lw6msg_word_t word;
  char *seek=NULL;
  char *pos=NULL;

  if (lw6sys_str_starts_with_no_case(msg,LW6MSG_CMD_HELLO)) {
    pos=msg+strlen(LW6MSG_CMD_HELLO);
    if (lw6msg_word_first(&word,&seek,pos)) {
      // todo...
    }
  } else {
    lw6sys_log(LW6SYS_LOG_DEBUG,_("parsing HELLO but couldn't find it in \"%s\""),msg);
  }

  return ret;
}

/**
 * lw6msg_cmd_analyse_ticket
 *
 * @info: if not NULL, will contain (remote) node info on success
 * @ticket: if not NULL, will contain the ticket value on success
 * @msg: the message to anaylse
 *
 * Analyzes a TICKET message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6cmd_analyse_ticket (lw6nod_info_t ** info, u_int32_t * ticket, char *msg)
{
  int ret = 0;

  return ret;
}


/**
 * lw6msg_cmd_analyse_foo
 *
 * @key: if not NULL, will contain the foo/bar key on success
 * @msg: the message to anaylse
 *
 * Analyzes a FOO message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6cmd_analyse_foo (int *key, char *msg)
{
  int ret = 0;

  return ret;
}

/**
 * lw6msg_cmd_analyse_bar
 *
 * @key: if not NULL, will contain the foo/bar key on success
 * @msg: the message to anaylse
 *
 * Analyzes a BAR message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6cmd_analyse_bar (int *key, char *msg)
{
  int ret = 0;

  return ret;
}

/**
 * lw6msg_cmd_analyse_goodbye
 *
 * @msg: the message to anaylse
 *
 * Analyzes a GOODBYE message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6cmd_analyse_goodbye (char *msg)
{
  int ret = 0;

  return ret;
}
