/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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
_generate_info (const char *cmd, lw6nod_info_t * info)
{
  char *ret = NULL;
  char sep = LW6MSG_TELNET_SEP;
  char *base64_title;
  char *base64_description;
  char *base64_level;
  char *peer_id_list;
  int uptime = 0;

  base64_title =
    lw6glb_base64_encode_str (lw6sys_str_empty_if_null
			      (info->const_info.title));
  if (base64_title)
    {
      base64_description =
	lw6glb_base64_encode_str (lw6sys_str_empty_if_null
				  (info->const_info.description));
      if (base64_description)
	{
	  base64_level =
	    lw6glb_base64_encode_str (lw6sys_str_empty_if_null
				      (info->dyn_info.level));
	  if (base64_level)
	    {
	      peer_id_list =
		lw6nod_info_community_get_peer_id_list_str (info);
	      if (peer_id_list)
		{
		  uptime =
		    (lw6sys_get_timestamp () -
		     info->const_info.creation_timestamp) / 1000;
		  ret =
		    lw6sys_new_sprintf
		    ("%s%c%s%c%s%c\"%s\"%c%d%c%s%c%s%c%s%c%s%c%d%c%d%c%d%c%d%c%s%c%d%c\"%s\"%c%d%c%d%c%d%c%d%c%d%c%d%c%d%c\"%s\"",
		     cmd, sep, info->const_info.program, sep,
		     info->const_info.version, sep, info->const_info.codename,
		     sep, info->const_info.stamp, sep,
		     info->const_info.ref_info.id_str, sep,
		     info->const_info.ref_info.url, sep, base64_title, sep,
		     base64_description, sep, info->const_info.has_password,
		     sep, info->const_info.bench, sep,
		     info->const_info.open_relay, sep, uptime, sep,
		     lw6sys_str_empty_if_null (info->
					       dyn_info.community_id_str),
		     sep, info->dyn_info.round, sep, base64_level, sep,
		     info->dyn_info.required_bench, sep,
		     info->dyn_info.nb_colors, sep,
		     info->dyn_info.max_nb_colors, sep,
		     info->dyn_info.nb_cursors, sep,
		     info->dyn_info.max_nb_cursors, sep,
		     info->dyn_info.nb_nodes, sep,
		     info->dyn_info.max_nb_nodes, sep, peer_id_list);
		  LW6SYS_FREE (sys_context, peer_id_list);
		}
	      LW6SYS_FREE (base64_level);
	    }
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
			    (long long) ticket);
      LW6SYS_FREE (sys_context, info_str);
    }

  return ret;
}

/**
 * lw6msg_cmd_generate_foo
 *
 * @info: the node info to use
 * @key: the key to identify the message
 * @serial: serial number of latest data message
 *
 * Generate a FOO command.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_foo (lw6nod_info_t * info, u_int32_t key, int serial)
{
  char *ret = NULL;
  char sep = LW6MSG_TELNET_SEP;
  char *info_str = NULL;

  info_str = _generate_info (LW6MSG_CMD_FOO, info);
  if (info_str)
    {
      ret =
	lw6sys_new_sprintf ("%s%c%08x%c%d", info_str, sep, key, sep, serial);
      LW6SYS_FREE (sys_context, info_str);
    }

  return ret;
}

/**
 * lw6msg_cmd_generate_bar
 *
 * @info: the node info to use
 * @key: the key to identify the message
 * @serial: serial number of latest data message
 *
 * Generate a BAR command.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_bar (lw6nod_info_t * info, u_int32_t key, int serial)
{
  char *ret = NULL;
  char sep = LW6MSG_TELNET_SEP;
  char *info_str = NULL;

  info_str = _generate_info (LW6MSG_CMD_BAR, info);
  if (info_str)
    {
      ret =
	lw6sys_new_sprintf ("%s%c%08x%c%d", info_str, sep, key, sep, serial);
      LW6SYS_FREE (sys_context, info_str);
    }

  return ret;
}

/**
 * lw6msg_cmd_generate_join
 *
 * @info: the node info to use
 * @seq: the current seq
 * @serial: the serial message number to start with
 *
 * Generate a JOIN command. The seq parameter, if 0, means we
 * want to request to join to a server. Wether this is a real
 * server or a physical client acting as a server is out of
 * consideration, 0 means request to join, period. If greater
 * than 0, means we are accepting a client, and then the value
 * is our current seq, which the client must use to calibrate
 * its own data. The serial number is here to avoid querying
 * messages before the join and keep the serie complete.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_join (lw6nod_info_t * info, int64_t seq, int serial)
{
  char *ret = NULL;
  char sep = LW6MSG_TELNET_SEP;
  char *info_str = NULL;

  info_str = _generate_info (LW6MSG_CMD_JOIN, info);
  if (info_str)
    {
      ret =
	lw6sys_new_sprintf ("%s%c%" LW6SYS_PRINTF_LL "d%c%d", info_str, sep,
			    (long long) seq, sep, serial);
      LW6SYS_FREE (sys_context, info_str);
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
 * @seq: the message seq (round + an offset)
 * @ker_msg: the actual content of the message (passed to core algo)
 *
 * Generate a DATA command. Serial is an ever increasing number,
 * i and n are most of the time 1 and 1, they are usefull
 * only in long multipart messages.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_data (int serial, int i, int n, int64_t seq,
			  const char *ker_msg)
{
  char *ret = NULL;

  ret =
    lw6sys_new_sprintf ("%s %d %d %d %" LW6SYS_PRINTF_LL "d %s",
			LW6MSG_CMD_DATA, serial, i, n, (long long) seq,
			ker_msg);

  return ret;
}

/**
 * lw6msg_cmd_generate_meta
 *
 * @serial: the message serial number
 * @i: the message index in the group
 * @n: the number of messages in the group
 * @seq: the message seq (round + an offset)
 * @meta_array: the content to send
 *
 * Generate a META command. Serial is an ever increasing number,
 * i and n are most of the time 1 and 1, they are usefull
 * only in long multipart messages.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_meta (int serial, int i, int n, int64_t seq,
			  const lw6msg_meta_array_t * meta_array)
{
  char *ret = NULL;
  char *meta_str = NULL;

  meta_str = lw6msg_meta_array2str (meta_array);
  if (meta_str)
    {
      ret =
	lw6sys_new_sprintf ("%s %d %d %d %" LW6SYS_PRINTF_LL "d %"
			    LW6SYS_PRINTF_LL "x %s", LW6MSG_CMD_META,
			    serial, i, n, (long long) seq,
			    (long long) meta_array->logical_from, meta_str);
      if (ret)
	{
	  if (strlen (ret) < LW6MSG_MAX_WORD_SIZE)
	    {
	      // OK, everthing's fine
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			  _x_ ("message \"%s\" is too long"), ret);
	      LW6SYS_FREE (sys_context, ret);
	      ret = NULL;
	    }
	}
      LW6SYS_FREE (sys_context, meta_str);
    }

  return ret;
}

/**
 * lw6msg_cmd_generate_miss
 *
 * @id_from: id of the node which didn't send data correctly
 * @id_to: id of the node which didn't receive data correctly
 * @serial_min: minimum serial number of unsent/unreceived messages
 * @serial_max: maximum serial number of unsent/unreceived messages
 *
 * Generate a MISS command. This will request anyone who has the
 * messages mentionned in stock to resent them to the one who's
 * asking for them.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_cmd_generate_miss (u_int64_t id_from, u_int64_t id_to, int serial_min,
			  int serial_max)
{
  char *ret = NULL;

  ret =
    lw6sys_new_sprintf ("%s %" LW6SYS_PRINTF_LL "x %" LW6SYS_PRINTF_LL
			"x %d %d", LW6MSG_CMD_MISS, (long long) id_from,
			(long long) id_to, serial_min, serial_max);

  return ret;
}

static int
_analyse_info (lw6nod_info_t ** info, lw6nod_info_t * local_info, char **next,
	       const char *msg)
{
  int ret = 0;
  int still_ok = 1;
  const char *pos = NULL;
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
  lw6msg_word_t peer_id_list;
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
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing program \"%s\""), pos);
      if (lw6msg_word_first (&program, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("bad program \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing version \"%s\""), pos);
      if (lw6msg_word_first (&version, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("bad version \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing codename \"%s\""), pos);
      if (lw6msg_word_first (&codename, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("bad codename \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing stamp \"%s\""), pos);
      if (lw6msg_word_first_int_32_gt0 (&stamp, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bad stamp \"%s\""),
		      pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("analyzing id \"%s\""),
		  pos);
      if (lw6msg_word_first_id_64 (&id, &seek, pos))
	{
	  pos = seek;
	}
    }
  else
    {
      still_ok = 0;
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bad id \"%s\""), pos);
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("analyzing url \"%s\""),
		  pos);
      if (lw6msg_word_first (&url, &seek, pos)
	  && lw6sys_url_is_canonized (url.buf))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bad url \"%s\""),
		      pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing title \"%s\""), pos);
      if (lw6msg_word_first_base64 (&title, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bad title \"%s\""),
		      pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing description \"%s\""), pos);
      if (lw6msg_word_first_base64 (&description, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("bad description \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing has_password \"%s\""), pos);
      if (lw6msg_word_first_int_32 (&has_password, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("bad has_password \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing bench \"%s\""), pos);
      if (lw6msg_word_first_int_32 (&bench, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bad bench \"%s\""),
		      pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing open_relay \"%s\""), pos);
      if (lw6msg_word_first_int_32 (&open_relay, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("bad open_relay \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing uptime \"%s\""), pos);
      if (lw6msg_word_first_int_32 (&uptime, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bad uptime \"%s\""),
		      pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing community id \"%s\""), pos);
      if (lw6msg_word_first_id_64 (&community_id, &seek, pos))
	{
	  pos = seek;
	}
    }
  else
    {
      if (lw6msg_word_first_int_32 (&tmp_int, &seek, pos))
	{
	  if (!tmp_int)
	    {
	      community_id = 0;
	      pos = seek;
	    }
	  else
	    {
	      still_ok = 0;
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			  _x_ ("bad community id \"%s\""), pos);
	    }
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("bad community id \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing round \"%s\""), pos);
      if (lw6msg_word_first_int_32 (&round, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bad round \"%s\""),
		      pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing level \"%s\""), pos);
      if (lw6msg_word_first_base64 (&level, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bad level \"%s\""),
		      pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing required_bench \"%s\""), pos);
      if (lw6msg_word_first_int_32 (&required_bench, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("bad required_bench \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing nb_colors \"%s\""), pos);
      if (lw6msg_word_first_int_32 (&nb_colors, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		      _x_ ("bad nb_colors \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing max_nb_colors \"%s\""), pos);
      if (lw6msg_word_first_int_32 (&max_nb_colors, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("bad max_nb_colors \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing nb_cursors \"%s\""), pos);
      if (lw6msg_word_first_int_32 (&nb_cursors, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("bad nb_cursors \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing max_nb_cursors \"%s\""), pos);
      if (lw6msg_word_first_int_32 (&max_nb_cursors, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("bad max_nb_cursors \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing nb_nodes \"%s\""), pos);
      if (lw6msg_word_first_int_32 (&nb_nodes, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("bad nb_nodes \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing max_nb_nodes \"%s\""), pos);
      if (lw6msg_word_first_int_32 (&max_nb_nodes, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("bad max_nb_nodes \"%s\""), pos);
	}
    }

  if (still_ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("analyzing peer_id_list \"%s\""), pos);
      if (lw6msg_word_first (&peer_id_list, &seek, pos))
	{
	  pos = seek;
	}
      else
	{
	  still_ok = 0;
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("bad peer_id_list \"%s\""), pos);
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
				  max_nb_nodes, peer_id_list.buf, 0, NULL))
	    {
	      if (local_info)
		{
		  /*
		   * Only if local_info is set, we update the local_info
		   * peer database too, this is usually performed on join
		   * messages.
		   */
		  lw6nod_info_community_set_peer_id_list_str (local_info,
							      peer_id_list.buf);
		}
	      (*info)->const_info.has_password = has_password;
	      if (next)
		{
		  (*next) = (char *) pos;
		}
	      ret = 1;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			  _x_ ("unable to update nod info"));
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("unable to create nod info"));
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
lw6msg_cmd_analyse_hello (lw6nod_info_t ** info, const char *msg)
{
  int ret = 0;

  if (lw6sys_str_starts_with_no_case (msg, LW6MSG_CMD_HELLO))
    {
      if (_analyse_info (info, NULL, NULL, msg + strlen (LW6MSG_CMD_HELLO)))
	{
	  ret = 1;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
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
			   const char *msg)
{
  int ret = 0;
  const char *pos = NULL;
  char *seek = NULL;

  if (lw6sys_str_starts_with_no_case (msg, LW6MSG_CMD_TICKET))
    {
      pos = msg + strlen (LW6MSG_CMD_TICKET);
      if (_analyse_info (info, NULL, &seek, pos))
	{
	  pos = seek;
	  if (lw6msg_word_first_id_64 (ticket, &seek, pos))
	    {
	      ret = 1;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			  _x_ ("bad ticket \"%s\""), pos);
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("parsing TICKET but couldn't find it in \"%s\""), msg);
    }

  return ret;
}


/**
 * lw6msg_cmd_analyse_foo
 *
 * @info: will contain (remote) node info on success
 * @key: if not NULL, will contain the foo/bar key on success
 * @serial: if not NULL, will contain the latest serial number of peer
 * @msg: the message to analyse
 *
 * Analyzes a FOO message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6msg_cmd_analyse_foo (lw6nod_info_t ** info, u_int32_t * key, int *serial,
			const char *msg)
{
  int ret = 0;
  const char *pos = NULL;
  char *seek = NULL;

  if (lw6sys_str_starts_with_no_case (msg, LW6MSG_CMD_FOO))
    {
      pos = msg + strlen (LW6MSG_CMD_FOO);
      if (_analyse_info (info, NULL, &seek, pos))
	{
	  pos = seek;
	  if (lw6msg_word_first_id_32 (key, &seek, pos))
	    {
	      pos = seek;
	      if (lw6msg_word_first_int_32_ge0 (serial, &seek, pos))
		{
		  ret = 1;
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			      _x_ ("bad serial \"%s\""), pos);
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			  _x_ ("bad key \"%s\""), pos);
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("parsing FOO but couldn't find it in \"%s\""), msg);
    }

  return ret;
}

/**
 * lw6msg_cmd_analyse_bar
 *
 * @info: will contain (remote) node info on success
 * @key: if not NULL, will contain the foo/bar key on success
 * @serial: if not NULL, will contain the latest serial number of peer
 * @msg: the message to analyse
 *
 * Analyzes a BAR message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6msg_cmd_analyse_bar (lw6nod_info_t ** info, u_int32_t * key, int *serial,
			const char *msg)
{
  int ret = 0;
  const char *pos = NULL;
  char *seek = NULL;

  if (lw6sys_str_starts_with_no_case (msg, LW6MSG_CMD_BAR))
    {
      pos = msg + strlen (LW6MSG_CMD_BAR);
      if (_analyse_info (info, NULL, &seek, pos))
	{
	  pos = seek;
	  if (lw6msg_word_first_id_32 (key, &seek, pos))
	    {
	      pos = seek;
	      if (lw6msg_word_first_int_32_ge0 (serial, &seek, pos))
		{
		  ret = 1;
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			      _x_ ("bad serial \"%s\""), pos);
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			  _x_ ("bad key \"%s\""), pos);
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("parsing BAR but couldn't find it in \"%s\""), msg);
    }

  return ret;
}

/**
 * lw6msg_cmd_analyse_join
 *
 * @info: will contain (remote) node info on success
 * @local_info: local node info to be updated (peer_id list), can be NULL
 * @seq: sequence used to initialize communication
 * @serial: serial used to initialize communication
 * @msg: the message to analyse
 *
 * Analyzes a JOIN message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6msg_cmd_analyse_join (lw6nod_info_t ** info, lw6nod_info_t * local_info,
			 int64_t * seq, int *serial, const char *msg)
{
  int ret = 0;
  const char *pos = NULL;
  char *seek = NULL;

  if (lw6sys_str_starts_with_no_case (msg, LW6MSG_CMD_JOIN))
    {
      pos = msg + strlen (LW6MSG_CMD_JOIN);
      if (_analyse_info (info, local_info, &seek, pos))
	{
	  pos = seek;
	  if (lw6msg_word_first_int_64 (seq, &seek, pos))
	    {
	      pos = seek;
	      if (lw6msg_word_first_int_32 (serial, &seek, pos))
		{
		  ret = 1;
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			      _x_ ("bad serial \"%s\""), pos);
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
			  _x_ ("bad seq \"%s\""), pos);
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("parsing JOIN but couldn't find it in \"%s\""), msg);
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
lw6msg_cmd_analyse_goodbye (lw6nod_info_t ** info, const char *msg)
{
  int ret = 0;

  if (lw6sys_str_starts_with_no_case (msg, LW6MSG_CMD_GOODBYE))
    {
      if (_analyse_info (info, NULL, NULL, msg + strlen (LW6MSG_CMD_GOODBYE)))
	{
	  ret = 1;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
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
 * @seq: will contain seq on success (round + an offset)
 * @ker_msg: will contain actual message on success
 * @msg: the message to analyze
 *
 * Analyzes a DATA message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6msg_cmd_analyse_data (int *serial, int *i, int *n, int64_t * seq,
			 char **ker_msg, const char *msg)
{
  int ret = 0;
  char *seek = NULL;
  const char *pos = NULL;
  int read_serial = 0;
  int read_i = 0;
  int read_n = 0;
  int64_t read_seq = 0;
  char *read_ker_msg = NULL;
  lw6msg_word_t data_word;
  lw6msg_word_t ker_msg_word;

  (*serial) = 0;
  (*i) = 0;
  (*n) = 0;
  (*ker_msg) = NULL;

  pos = msg;
  seek = (char *) pos;

  if (lw6msg_word_first (&data_word, &seek, pos)
      && lw6sys_str_is_same_no_case (data_word.buf, LW6MSG_CMD_DATA))
    {
      pos = seek;
      if (lw6msg_word_first_int_32_gt0 (&read_serial, &seek, pos))
	{
	  pos = seek;
	  if (lw6msg_word_first_int_32_ge0 (&read_i, &seek, pos))
	    {
	      pos = seek;
	      if (lw6msg_word_first_int_32_gt0 (&read_n, &seek, pos))
		{
		  pos = seek;
		  if (lw6msg_word_first_int_64_gt0 (&read_seq, &seek, pos))
		    {
		      pos = seek;
		      if (lw6msg_word_first (&ker_msg_word, &seek, pos))
			{
			  pos = seek;
			  read_ker_msg = lw6sys_str_copy (ker_msg_word.buf);
			  if (read_ker_msg)
			    {
			      ret = 1;
			      (*serial) = read_serial;
			      (*i) = read_i;
			      (*n) = read_n;
			      (*seq) = read_seq;
			      (*ker_msg) = read_ker_msg;
			    }
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				      _x_ ("unable to parse ker message"));
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_ ("unable to parse seq"));
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			      _x_ ("unable to parse group size n"));
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			  _x_ ("unable to parse group index i"));
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("unable to parse serial"));
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("unable to parse DATA"));
    }

  return ret;
}

/**
 * lw6msg_cmd_analyse_meta
 *
 * @serial: will contain serial number on success
 * @i: will contain group index on success
 * @n: will contain group size on success
 * @seq: will contain seq on success (round + an offset)
 * @meta_array: will contain the content on success
 * @msg: the message to analyze
 *
 * Analyzes a META message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6msg_cmd_analyse_meta (int *serial, int *i, int *n, int64_t * seq,
			 lw6msg_meta_array_t * meta_array, const char *msg)
{
  int ret = 0;
  char *seek = NULL;
  const char *pos = NULL;
  int read_serial = 0;
  int read_i = 0;
  int read_n = 0;
  int64_t read_seq = 0;
  u_int64_t read_logical_from = 0LL;
  lw6msg_word_t meta_word;

  (*serial) = 0;
  (*i) = 0;
  (*n) = 0;

  pos = msg;
  seek = (char *) pos;

  if (lw6msg_word_first (&meta_word, &seek, pos)
      && lw6sys_str_is_same_no_case (meta_word.buf, LW6MSG_CMD_META))
    {
      pos = seek;
      if (lw6msg_word_first_int_32_gt0 (&read_serial, &seek, pos))
	{
	  pos = seek;
	  if (lw6msg_word_first_int_32_ge0 (&read_i, &seek, pos))
	    {
	      pos = seek;
	      if (lw6msg_word_first_int_32_gt0 (&read_n, &seek, pos))
		{
		  pos = seek;
		  if (lw6msg_word_first_int_64_gt0 (&read_seq, &seek, pos))
		    {
		      pos = seek;
		      if (lw6msg_word_first_id_64
			  (&read_logical_from, &seek, pos))
			{
			  pos = seek;
			  if (!lw6sys_str_is_null_or_empty (pos))
			    {
			      pos = seek;
			      if (lw6msg_meta_str2array (meta_array, pos))
				{
				  ret = 1;
				  (*serial) = read_serial;
				  (*i) = read_i;
				  (*n) = read_n;
				  (*seq) = read_seq;
				  meta_array->logical_from =
				    read_logical_from;
				}
			      else
				{
				  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
					      _x_
					      ("unable to parse meta message (stage 2), pos=\"%s\""),
					      pos);
				}
			    }
			  else
			    {
			      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
					  _x_
					  ("unable to parse meta message (stage 1), no content"));
			    }
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				      _x_ ("unable to parse logical_from"));
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_ ("unable to parse seq"));
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			      _x_ ("unable to parse group size n"));
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			  _x_ ("unable to parse group index i"));
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("unable to parse serial"));
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("unable to parse META"));
    }

  return ret;
}

/**
 * lw6msg_cmd_analyse_miss
 *
 * @id_from: will contain the id of the node which didn't send data correctly
 * @id_to: will contain the id of the node which didn't receive data correctly
 * @serial_min: will contain the minimum serial number of unsent/unreceived messages
 * @serial_max: will contain the maximum serial number of unsent/unreceived messages
 * @msg: the message to analyze
 *
 * Analyzes a MISS message.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6msg_cmd_analyse_miss (u_int64_t * id_from, u_int64_t * id_to,
			 int *serial_min, int *serial_max, const char *msg)
{
  int ret = 0;
  char *seek = NULL;
  const char *pos = NULL;
  u_int64_t read_id_from = 0LL;
  u_int64_t read_id_to = 0LL;
  int read_serial_min = 0;
  int read_serial_max = 0;
  lw6msg_word_t miss_word;

  (*id_from) = 0LL;
  (*id_to) = 0LL;
  (*serial_min) = 0;
  (*serial_max) = 0;

  pos = msg;
  seek = (char *) pos;

  if (lw6msg_word_first (&miss_word, &seek, pos)
      && lw6sys_str_is_same_no_case (miss_word.buf, LW6MSG_CMD_MISS))
    {
      pos = seek;
      if (lw6msg_word_first_id_64 (&read_id_from, &seek, pos))
	{
	  pos = seek;
	  if (lw6msg_word_first_id_64 (&read_id_to, &seek, pos))
	    {
	      pos = seek;
	      if (lw6msg_word_first_int_32_gt0 (&read_serial_min, &seek, pos))
		{
		  pos = seek;
		  if (lw6msg_word_first_int_32_gt0
		      (&read_serial_max, &seek, pos))
		    {
		      ret = 1;
		      (*id_from) = read_id_from;
		      (*id_to) = read_id_to;
		      (*serial_min) = read_serial_min;
		      (*serial_max) = read_serial_max;
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_ ("unable to parse serial_max"));
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			      _x_ ("unable to parse serial_min"));
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			  _x_ ("unable to parse id_to"));
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		      _x_ ("unable to parse id_from"));
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("unable to parse MISS"));
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
lw6msg_cmd_guess_from_url (const char *msg)
{
  char *ret = NULL;
  lw6nod_info_t *node_info = NULL;
  char *msg_table[] =
    { LW6MSG_CMD_HELLO, LW6MSG_CMD_TICKET, LW6MSG_CMD_FOO, LW6MSG_CMD_BAR,
    LW6MSG_CMD_JOIN,
    LW6MSG_CMD_GOODBYE, NULL
  };
  char *seek = NULL;
  const char *pos = NULL;
  char **command = NULL;

  for (command = msg_table; (*command) != NULL && !ret; ++command)
    {
      if (lw6sys_str_starts_with_no_case (msg, *command))
	{
	  pos = msg + strlen (*command);
	  if (_analyse_info (&node_info, NULL, &seek, pos))
	    {
	      ret = lw6sys_str_copy (node_info->const_info.ref_info.url);
	      lw6nod_info_free (node_info);
	      node_info = NULL;
	    }
	}
    }

  return ret;
}
