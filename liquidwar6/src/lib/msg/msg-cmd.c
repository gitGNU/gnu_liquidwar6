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

char *
_generate_info (char *cmd, lw6nod_info_t * info)
{
  char *ret = NULL;
  char sep = LW6MSG_TELNET_SEP;
  char *base64_title;
  char *base64_description;
  int uptime = 0;

  base64_title = lw6glb_base64_encode_str (info->const_info.title);
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
	    ("%s%c%s%c%s%c\"%s\"%c%d%c%s%c%s%c%s%c%s%c%d%c%d%c%d%c%d%c%s%c%d%c\"%s\"%c%d%c%d%c%d%c%d%c%d%c%d%c%d",
	     cmd, sep, info->const_info.program, sep,
	     info->const_info.version, sep, info->const_info.codename, sep,
	     info->const_info.stamp, sep, info->const_info.id_str, sep,
	     info->const_info.url, sep, base64_title, sep, base64_description,
	     sep, info->const_info.has_password, sep, info->const_info.bench,
	     sep, info->const_info.open_relay, sep, uptime, sep,
	     lw6sys_str_empty_if_null (info->dyn_info.community_id_str), sep,
	     info->dyn_info.round, sep,
	     lw6sys_str_empty_if_null (info->dyn_info.level), sep,
	     info->dyn_info.required_bench, sep, info->dyn_info.nb_colors,
	     sep, info->dyn_info.max_nb_colors, sep,
	     info->dyn_info.nb_cursors, sep, info->dyn_info.max_nb_cursors,
	     sep, info->dyn_info.nb_nodes, sep, info->dyn_info.max_nb_nodes);
	  LW6SYS_FREE (base64_description);
	}
      LW6SYS_FREE (base64_title);
    }

  return ret;
}

/**
 * lw6msg_cmd_generate_hello
 *
 * @info: the node info to use
 *
 * Generate a HELLO command.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_hello (lw6nod_info_t * info)
{
  char *ret = NULL;

  ret = _generate_info (LW6MSG_CMD_HELLO, info);

  return ret;
}

/**
 * lw6msg_cmd_generate_ticket
 *
 * @info: the node info to use
 * @ticket: the ticket to send
 *
 * Generate a TICKET command.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_ticket (lw6nod_info_t * info, u_int64_t ticket)
{
  char *ret = NULL;
  char sep = LW6MSG_TELNET_SEP;
  char *info_str = NULL;

  info_str = _generate_info (LW6MSG_CMD_TICKET, info);
  if (info_str)
    {
      ret =
	lw6sys_new_sprintf ("%s%c%" LW6SYS_PRINTF_LL "x", info_str, sep,
			    ticket);
      LW6SYS_FREE (info_str);
    }

  return ret;
}

/**
 * lw6msg_cmd_generate_foo
 *
 * @info: the node info to use
 * @key: the key to identify the message
 *
 * Generate a FOO command.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_foo (lw6nod_info_t * info, u_int32_t key)
{
  char *ret = NULL;
  char sep = LW6MSG_TELNET_SEP;
  char *info_str = NULL;

  info_str = _generate_info (LW6MSG_CMD_FOO, info);
  if (info_str)
    {
      ret = lw6sys_new_sprintf ("%s%c%08x", info_str, sep, key);
      LW6SYS_FREE (info_str);
    }

  return ret;
}

/**
 * lw6msg_cmd_generate_bar
 *
 * @info: the node info to use
 * @key: the key to identify the message
 *
 * Generate a BAR command.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_bar (lw6nod_info_t * info, u_int32_t key)
{
  char *ret = NULL;
  char sep = LW6MSG_TELNET_SEP;
  char *info_str = NULL;

  info_str = _generate_info (LW6MSG_CMD_BAR, info);
  if (info_str)
    {
      ret = lw6sys_new_sprintf ("%s%c%08x", info_str, sep, key);
      LW6SYS_FREE (info_str);
    }

  return ret;
}

/**
 * lw6msg_cmd_generate_goodbye
 *
 * @info: the node info to use
 *
 * Generate a GOODBYE command.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_goodbye (lw6nod_info_t * info)
{
  char *ret = NULL;

  ret = _generate_info (LW6MSG_CMD_GOODBYE, info);

  return ret;
}

/**
 * lw6msg_cmd_generate_data
 *
 * @serial: the message serial number
 * @i: the message index in the group
 * @n: the number of messages in the group
 * @round: the message round (can have an offset with real round)
 * @ker_msg: the actual content of the message (passed to core algo)
 *
 * Generate a DATA command. Serial is an ever increasing number,
 * i and n are most of the time 1 and 1, they are usefull
 * only in long multipart messages.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_data (int serial, int i, int n, int round, char *ker_msg)
{
  char *ret = NULL;

  ret = lw6sys_new_sprintf ("%d %d %d %d %s", serial, i, n, round, ker_msg);

  return ret;
}

static int
_analyse_info (lw6nod_info_t ** info, char **next, char *msg)
{
  int ret = 0;
  int still_ok = 1;
  char *pos = NULL;
  char *seek = NULL;
  int tmp_int = 0;
  lw6msg_word_t program;
  lw6msg_word_t version;
  lw6msg_word_t codename;
  int stamp = 0;
  u_int64_t id = 0;
  lw6msg_word_t url;
  lw6msg_word_t title;
  lw6msg_word_t description;
  int has_password = 0;
  int bench = 0;
  int open_relay = 0;
  int uptime = 0;
  u_int64_t community_id = 0;
  int round = 0;
  lw6msg_word_t level;
  int required_bench = 0;
  int nb_colors = 0;
  int max_nb_colors = 0;
  int nb_cursors = 0;
  int max_nb_cursors = 0;
  int nb_nodes = 0;
  int max_nb_nodes = 0;

  (*info) = NULL;
  if (next)
    {
      (*next) = NULL;
    }

  pos = msg;

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("analyzing program \"%s\""), pos);
      if (lw6msg_word_first (&program, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad program \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("analyzing version \"%s\""), pos);
      if (lw6msg_word_first (&version, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad version \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("analyzing codename \"%s\""), pos);
      if (lw6msg_word_first (&codename, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad codename \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("analyzing stamp \"%s\""), pos);
      if (lw6msg_word_first_int_gt0 (&stamp, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad stamp \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("analyzing id \"%s\""), pos);
      if (lw6msg_word_first_id_64 (&id, &seek, pos))
	{
	  pos = seek;
	}
    }
  else
    {
      still_ok = 0;
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad id \"%s\""), pos);
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("analyzing url \"%s\""), pos);
      if (lw6msg_word_first (&url, &seek, pos)
	  && lw6sys_url_is_canonized (url.buf))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad url \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("analyzing title \"%s\""), pos);
      if (lw6msg_word_first_base64 (&title, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad title \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing description \"%s\""), pos);
      if (lw6msg_word_first_base64 (&description, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad description \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing has_password \"%s\""), pos);
      if (lw6msg_word_first_int (&has_password, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad has_password \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("analyzing bench \"%s\""), pos);
      if (lw6msg_word_first_int (&bench, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad bench \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("analyzing open_relay \"%s\""), pos);
      if (lw6msg_word_first_int (&open_relay, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad open_relay \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("analyzing uptime \"%s\""), pos);
      if (lw6msg_word_first_int (&uptime, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad uptime \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("analyzing community id \"%s\""),
		  pos);
      if (lw6msg_word_first_id_64 (&community_id, &seek, pos))
	{
	  pos = seek;
	}
    }
  else
    {
      if (lw6msg_word_first_int (&tmp_int, &seek, pos))
	{
	  if (!tmp_int)
	    {
	      community_id = 0;
	      pos = seek;
	    }
	  else
	    {
	      still_ok = 0;
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad community id \"%s\""),
			  pos);
	    }
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad community id \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("analyzing round \"%s\""), pos);
      if (lw6msg_word_first_int (&round, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad round \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("analyzing level \"%s\""), pos);
      if (lw6msg_word_first_base64 (&level, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad level \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing required_bench \"%s\""), pos);
      if (lw6msg_word_first_int (&required_bench, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("bad required_bench \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("analyzing nb_colors \"%s\""), pos);
      if (lw6msg_word_first_int (&nb_colors, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad nb_colors \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing max_nb_colors \"%s\""), pos);
      if (lw6msg_word_first_int (&max_nb_colors, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("bad max_nb_colors \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("analyzing nb_cursors \"%s\""), pos);
      if (lw6msg_word_first_int (&nb_cursors, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad nb_cursors \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing max_nb_cursors \"%s\""), pos);
      if (lw6msg_word_first_int (&max_nb_cursors, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("bad max_nb_cursors \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("analyzing nb_nodes \"%s\""), pos);
      if (lw6msg_word_first_int (&nb_nodes, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad nb_nodes \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing max_nb_nodes \"%s\""), pos);
      if (lw6msg_word_first_int (&max_nb_nodes, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad max_nb_nodes \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      (*info) =
	lw6nod_info_new
	(program.buf, version.buf,
	 codename.buf, stamp, id,
	 url.buf, title.buf,
	 description.buf, NULL, bench, open_relay, uptime, 0, NULL);
      if (*info)
	{
	  if (lw6nod_info_update ((*info), community_id, round, level.buf,
				  required_bench, nb_colors, max_nb_colors,
				  nb_cursors, max_nb_cursors, nb_nodes,
				  max_nb_nodes, 0, NULL))
	    {
	      (*info)->const_info.has_password = has_password;
	      if (next)
		{
		  (*next) = pos;
		}
	      ret = 1;
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to create nod info"));
	}
    }

  return ret;
}

/**
 * lw6msg_cmd_analyse_hello
 *
 * @info: will contain (remote) node info on success
 * @msg: the message to analyse
 *
 * Analyzes a HELLO message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6msg_cmd_analyse_hello (lw6nod_info_t ** info, char *msg)
{
  int ret = 0;

  if (lw6sys_str_starts_with_no_case (msg, LW6MSG_CMD_HELLO))
    {
      if (_analyse_info (info, NULL, msg + strlen (LW6MSG_CMD_HELLO)))
	{
	  ret = 1;
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("parsing HELLO but couldn't find it in \"%s\""), msg);
    }

  return ret;
}

/**
 * lw6msg_cmd_analyse_ticket
 *
 * @info: will contain (remote) node info on success
 * @ticket: if not NULL, will contain the ticket value on success
 * @msg: the message to analyse
 *
 * Analyzes a TICKET message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6msg_cmd_analyse_ticket (lw6nod_info_t ** info, u_int64_t * ticket,
			   char *msg)
{
  int ret = 0;
  char *pos = NULL;
  char *seek = NULL;

  if (lw6sys_str_starts_with_no_case (msg, LW6MSG_CMD_TICKET))
    {
      pos = msg + strlen (LW6MSG_CMD_TICKET);
      if (_analyse_info (info, &seek, pos))
	{
	  pos = seek;
	  if (lw6msg_word_first_id_64 (ticket, &seek, pos))
	    {
	      ret = 1;
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad ticket \"%s\""), pos);
	    }
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("parsing TICKET but couldn't find it in \"%s\""), msg);
    }

  return ret;
}


/**
 * lw6msg_cmd_analyse_foo
 *
 * @info: will contain (remote) node info on success
 * @key: if not NULL, will contain the foo/bar key on success
 * @msg: the message to analyse
 *
 * Analyzes a FOO message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6msg_cmd_analyse_foo (lw6nod_info_t ** info, u_int32_t * key, char *msg)
{
  int ret = 0;
  char *pos = NULL;
  char *seek = NULL;

  if (lw6sys_str_starts_with_no_case (msg, LW6MSG_CMD_FOO))
    {
      pos = msg + strlen (LW6MSG_CMD_FOO);
      if (_analyse_info (info, &seek, pos))
	{
	  pos = seek;
	  if (lw6msg_word_first_id_32 (key, &seek, pos))
	    {
	      ret = 1;
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad key \"%s\""), pos);
	    }
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("parsing FOO but couldn't find it in \"%s\""), msg);
    }

  return ret;
}

/**
 * lw6msg_cmd_analyse_bar
 *
 * @info: will contain (remote) node info on success
 * @key: if not NULL, will contain the foo/bar key on success
 * @msg: the message to analyse
 *
 * Analyzes a BAR message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6msg_cmd_analyse_bar (lw6nod_info_t ** info, u_int32_t * key, char *msg)
{
  int ret = 0;
  char *pos = NULL;
  char *seek = NULL;

  if (lw6sys_str_starts_with_no_case (msg, LW6MSG_CMD_BAR))
    {
      pos = msg + strlen (LW6MSG_CMD_BAR);
      if (_analyse_info (info, &seek, pos))
	{
	  pos = seek;
	  if (lw6msg_word_first_id_32 (key, &seek, pos))
	    {
	      ret = 1;
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("bad key \"%s\""), pos);
	    }
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("parsing BAR but couldn't find it in \"%s\""), msg);
    }

  return ret;
}

/**
 * lw6msg_cmd_analyse_goodbye
 *
 * @info: will contain (remote) node info on success
 * @msg: the message to analyse
 *
 * Analyzes a GOODBYE message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6msg_cmd_analyse_goodbye (lw6nod_info_t ** info, char *msg)
{
  int ret = 0;

  if (lw6sys_str_starts_with_no_case (msg, LW6MSG_CMD_GOODBYE))
    {
      if (_analyse_info (info, NULL, msg + strlen (LW6MSG_CMD_GOODBYE)))
	{
	  ret = 1;
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _x_ ("parsing GOODBYE but couldn't find it in \"%s\""),
		  msg);
    }

  return ret;
}

/**
 * lw6msg_cmd_analyse_data
 *
 * @serial: will contain serial number on success
 * @i: will contain group index on success
 * @n: will contain group size on success
 * @round: will contain round on success (can have an offset with real round)
 * @ker_msg: will contain actual message on success
 *
 * Analyzes a DATA message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6msg_cmd_analyse_data (int *serial, int *i, int *n, int *round,
			 char **ker_msg, char *msg)
{
  int ret = 0;
  char *seek = NULL;
  char *pos = NULL;
  int read_serial = 0;
  int read_i = 0;
  int read_n = 0;
  int read_round = 0;
  char *read_ker_msg = NULL;
  lw6msg_word_t ker_msg_word;

  (*serial) = 0;
  (*i) = 0;
  (*n) = 0;
  (*ker_msg) = NULL;

  seek = pos = msg;

  if (lw6msg_word_first_int_gt0 (&read_serial, &seek, pos))
    {
      if (lw6msg_word_first_int_gt0 (&read_i, &seek, pos))
	{
	  if (lw6msg_word_first_int_gt0 (&read_n, &seek, pos))
	    {
	      if (lw6msg_word_first_int_gt0 (&read_round, &seek, pos))
		{
		  if (lw6msg_word_first (&ker_msg_word, &seek, pos))
		    {
		      read_ker_msg = lw6sys_str_copy (ker_msg_word.buf);
		      if (read_ker_msg)
			{
			  ret = 1;
			  (*serial) = read_serial;
			  (*i) = read_i;
			  (*n) = read_n;
			  (*round) = read_round;
			  (*ker_msg) = read_ker_msg;
			}
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _x_ ("unable to parse ker message"));
		    }
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_DEBUG,
			      _x_ ("unable to parse round"));
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _x_ ("unable to parse group size n"));
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("unable to parse group index i"));
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("unable to parse serial"));
    }

  return ret;
}

/**
 * lw6msg_cmd_guess_from_url
 *
 * @msg: the message to analyse
 *
 * Analyzes a GOODBYE message.
 *
 * Return value: the from url, if found (dynamically allocated)
 */
char *
lw6msg_cmd_guess_from_url (char *msg)
{
  char *ret = NULL;
  lw6nod_info_t *node_info = NULL;
  char *msg_table[] =
    { LW6MSG_CMD_HELLO, LW6MSG_CMD_TICKET, LW6MSG_CMD_FOO, LW6MSG_CMD_BAR,
    LW6MSG_CMD_GOODBYE, NULL
  };
  char *seek = NULL;
  char *pos = NULL;
  char **command = NULL;

  for (command = msg_table; (*command) != NULL && !ret; ++command)
    {
      if (lw6sys_str_starts_with_no_case (msg, *command))
	{
	  pos = msg + strlen (*command);
	  if (_analyse_info (&node_info, &seek, pos))
	    {
	      ret = lw6sys_str_copy (node_info->const_info.url);
	      lw6nod_info_free (node_info);
	      node_info = NULL;
	    }
	}
    }

  return ret;
}
