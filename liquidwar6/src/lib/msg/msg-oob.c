/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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
#include "../net/net.h"		// only for the #defines, no bin dep

/**
 * lw6msg_oob_generate_info
 *
 * @info: the node to generate info about
 *
 * Generates a standard response to the INFO question for OOB
 * (out of band) messages. The same message is sent, be it
 * on http or tcp or udp, so it's factorized here. Function
 * will lock the info object when needed.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_oob_generate_info (lw6nod_info_t * info)
{
  char *ret = NULL;
  lw6nod_dyn_info_t *dyn_info = NULL;
  int uptime = 0;
  char *open_relay = NULL;
  char *has_password = NULL;

  dyn_info = lw6nod_info_dup_dyn (info);
  if (dyn_info)
    {
      has_password = info->const_info.has_password ? LW6MSG_YES : LW6MSG_NO;
      open_relay = info->const_info.open_relay ? LW6MSG_YES : LW6MSG_NO;
      uptime =
	(lw6sys_get_timestamp () -
	 info->const_info.creation_timestamp) / 1000;
      ret =
	lw6sys_new_sprintf
	("%s %s\n%s %s\n%s %s\n%s %s\n%s %s\n%s %s\n%s %s\n%s %s\n%s %s\n%s %d\n%s %s\n%s %d\n%s %s\n%s %d\n%s %s\n%s %d\n%s %d\n%s %d\n%s %d\n%s %d\n%s %d\n%s %d\n\n",
	 LW6MSG_OOB_PROGRAM,
	 lw6sys_build_get_package_tarname (),
	 LW6MSG_OOB_VERSION,
	 lw6sys_build_get_version (),
	 LW6MSG_OOB_CODENAME,
	 lw6sys_build_get_codename (),
	 LW6MSG_OOB_STAMP,
	 lw6sys_build_get_stamp (),
	 LW6MSG_OOB_ID,
	 info->const_info.ref_info.id_str,
	 LW6MSG_OOB_URL,
	 info->const_info.ref_info.url,
	 LW6MSG_OOB_TITLE,
	 info->const_info.title,
	 LW6MSG_OOB_DESCRIPTION,
	 info->const_info.description,
	 LW6MSG_OOB_HAS_PASSWORD,
	 has_password,
	 LW6MSG_OOB_BENCH,
	 info->const_info.bench,
	 LW6MSG_OOB_OPEN_RELAY,
	 open_relay,
	 LW6MSG_OOB_UPTIME,
	 uptime,
	 LW6MSG_OOB_COMMUNITY,
	 lw6sys_str_empty_if_null (dyn_info->community_id_str),
	 LW6MSG_OOB_ROUND,
	 dyn_info->round,
	 LW6MSG_OOB_LEVEL,
	 lw6sys_str_empty_if_null (dyn_info->level),
	 LW6MSG_OOB_REQUIRED_BENCH,
	 dyn_info->required_bench,
	 LW6MSG_OOB_NB_COLORS,
	 dyn_info->nb_colors,
	 LW6MSG_OOB_MAX_NB_COLORS,
	 dyn_info->max_nb_colors,
	 LW6MSG_OOB_NB_CURSORS,
	 dyn_info->nb_cursors,
	 LW6MSG_OOB_MAX_NB_CURSORS,
	 dyn_info->max_nb_cursors,
	 LW6MSG_OOB_NB_NODES,
	 dyn_info->nb_nodes, LW6MSG_OOB_MAX_NB_NODES, dyn_info->max_nb_nodes);
      lw6nod_dyn_info_free (dyn_info);
    }

  return ret;
}

static void
_add_node_txt (void *func_data, void *data)
{
  char **list = (char **) func_data;
  lw6nod_info_t *verified_node = (lw6nod_info_t *) data;
  char *tmp = NULL;

  /*
   * We use this instead of a simple "join with sep=space"
   * on the list object? This is because we can only
   * access it through the map function because of locking issues
   */
  if (list && (*list) && verified_node
      && verified_node->const_info.ref_info.url)
    {
      if (strlen (*list) + strlen (verified_node->const_info.ref_info.url) <=
	  LW6NET_PPPOE_MTU - 1)
	{
	  tmp =
	    lw6sys_new_sprintf ("%s%s\n", *list,
				verified_node->const_info.ref_info.url);
	  if (tmp)
	    {
	      LW6SYS_FREE (*list);
	      (*list) = tmp;
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_
		      ("truncating list, very long lists do not make any sense anyway, and it could cause problems on UDP"));
	}
    }
}

/**
 * lw6msg_oob_generate_list
 *
 * @info: the node to generate info about
 *
 * Generates a standard response to the LIST question for OOB
 * (out of band) messages. The same message is sent, be it
 * on http or tcp or udp, so it's factorized here. Function
 * will lock the info object when needed. There's a max length
 * because we don't want the udp buffer to be saturated + too
 * long lists do not really mean anything anyway.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_oob_generate_list (lw6nod_info_t * info)
{
  char *ret = NULL;
  lw6nod_dyn_info_t *dyn_info = NULL;
  char *tmp;

  dyn_info = lw6nod_info_dup_dyn (info);
  if (dyn_info)
    {
      ret = lw6sys_new_sprintf ("");
      if (ret)
	{
	  lw6nod_info_map_verified_nodes (info, _add_node_txt, &ret);
	  if (ret)
	    {
	      tmp = lw6sys_str_concat (ret, "\n");
	      if (tmp)
		{
		  LW6SYS_FREE (ret);
		  ret = tmp;
		}
	    }
	}
      lw6nod_dyn_info_free (dyn_info);
    }

  return ret;
}

/**
 * lw6msg_oob_generate_pong
 *
 * @info: the node to generate info about
 *
 * Generates a standard response to the PING question for OOB
 * (out of band) messages. The same message is sent, be it
 * on http or tcp or udp, so it's factorized here. Function
 * will lock the info object when needed.
 *
 * Return value: newly allocated string.
 */
char *
lw6msg_oob_generate_pong (lw6nod_info_t * info)
{
  char *ret = NULL;

  ret = lw6sys_new_sprintf ("PONG %s\n\n", info->const_info.ref_info.url);

  return ret;
}

/**
 * lw6msg_oob_generate_request
 * 
 * @command: the command to send (PING, INFO, LIST)
 * @remote_url: the remote URL (used to seed password)
 * @password: the password, can be NULL or ""
 * @local_url: the public URL to send along with the message, can be NULL or ""
 *
 * Generates a simple clear text OOB request, with a password if needed.
 *
 * Return value: a newly allocated string
 */
char *
lw6msg_oob_generate_request (const char *command, const char *remote_url,
			     const char *password, const char *local_url)
{
  char *ret = NULL;
  char *password_checksum = NULL;

  if (remote_url && strlen (remote_url) > 0 && password
      && strlen (password) > 0)
    {
      password_checksum = lw6cnx_password_checksum (remote_url, password);
    }
  if (password_checksum)
    {
      if (local_url && strlen (local_url) > 0)
	{
	  ret =
	    lw6sys_new_sprintf ("%s %s %s", command, password_checksum,
				local_url);
	}
      else
	{
	  ret = lw6sys_new_sprintf ("%s %s", command, password_checksum);
	}
      LW6SYS_FREE (password_checksum);
    }
  else
    {
      if (local_url && strlen (local_url) > 0)
	{
	  ret = lw6sys_new_sprintf ("%s %s", command, local_url);
	}
      else
	{
	  ret = lw6sys_new_sprintf ("%s", command);
	}
    }

  return ret;
}

/**
 * lw6msg_oob_analyse_request
 *
 * @syntax_ok: will contain 1 if syntax is OK, 0 if not
 * @command: the command (out param, needs *not* to be freed) 
 * @password_ok: will contain 1 if password is OK, 0 if not
 * @remote_url: the URL detected, if provided (out param, does needs to be freed)
 * @request: the request to analyse
 * @local_url: the local url (used to seed password)
 * @password: the password to check against
 *
 * Analyses a simple OOB message of the form COMMAND <passwd> <url>. 
 *
 * Return value: 1 if OK, 0 if not. If 0, check the value of password_ok. 
 */
int
lw6msg_oob_analyse_request (int *syntax_ok, char **command, int *password_ok,
			    char **remote_url, const char *request,
			    const char *local_url, const char *password)
{
  int ret = 0;
  char *copy = NULL;
  char *seek = NULL;
  char *pos = NULL;
  char seek_c = '\0';
  char *param1 = NULL;
  char *param2 = NULL;
  char *received_password = NULL;
  char *received_url = NULL;

  (*syntax_ok) = 0;
  (*command) = NULL;
  (*password_ok) = 0;
  (*remote_url) = NULL;

  copy = lw6sys_str_copy (request);
  if (copy)
    {
      seek = copy;
      while (lw6sys_chr_is_space (*seek))
	{
	  seek++;
	}
      pos = seek;

      if (lw6sys_str_starts_with_no_case (pos, LW6MSG_OOB_PING))
	{
	  (*command) = LW6MSG_OOB_PING;
	  (*password_ok) = 1;
	  seek += strlen (LW6MSG_OOB_PING);
	}
      else if (lw6sys_str_starts_with_no_case (pos, LW6MSG_OOB_INFO))
	{
	  (*command) = LW6MSG_OOB_INFO;
	  seek += strlen (LW6MSG_OOB_INFO);
	}
      else if (lw6sys_str_starts_with_no_case (pos, LW6MSG_OOB_LIST))
	{
	  (*command) = LW6MSG_OOB_LIST;
	  seek += strlen (LW6MSG_OOB_LIST);
	}

      if (*command)
	{
	  pos = seek;
	  while (lw6sys_chr_is_space (*seek))
	    {
	      seek++;
	    }
	  if (seek != pos || lw6sys_chr_is_eol (*pos) || (*pos) == '\0')
	    {
	      (*syntax_ok) = 1;
	      if (seek != pos)
		{
		  pos = seek;
		  while ((*seek) && !lw6sys_chr_is_space (*seek)
			 && !lw6sys_chr_is_eol (*seek))
		    {
		      seek++;
		    }
		  if (seek != pos)
		    {
		      seek_c = (*seek);
		      (*seek) = '\0';
		      param1 = lw6sys_str_copy (pos);
		      (*seek) = seek_c;
		      pos = seek;
		      while (lw6sys_chr_is_space (*seek))
			{
			  seek++;
			}
		      if (seek != pos)
			{
			  pos = seek;
			  while ((*seek) && !lw6sys_chr_is_space (*seek)
				 && !lw6sys_chr_is_eol (*seek))
			    {
			      seek++;
			    }
			  if (seek != pos)
			    {
			      seek_c = (*seek);
			      (*seek) = '\0';
			      param2 = lw6sys_str_copy (pos);
			    }
			}
		    }
		}
	      if (param1 && param2)
		{
		  received_password = param1;
		  received_url = param2;
		  param1 = NULL;
		  param2 = NULL;
		}
	      if (param1)
		{
		  if (lw6sys_url_is_canonized (param1))
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _x_
				  ("param1=\"%s\" is canonized URL, considering it an URL"),
				  param1);
		      received_url = param1;
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _x_
				  ("param1=\"%s\" is not a canonized URL, considering it a password"),
				  param1);
		      received_password = param1;
		    }
		  param1 = NULL;
		}

	      /*
	       * We only check password now that we have chosen which
	       * field is password, even if the value is NULL
	       */
	      if (lw6cnx_password_verify (local_url, password,
					  received_password))
		{
		  (*password_ok) = 1;
		}
	      if (*password_ok)
		{
		  (*remote_url) = received_url;
		  ret = 1;
		}
	      else
		{
		  if (received_url)
		    {
		      LW6SYS_FREE (received_url);
		    }
		}
	      if (received_password)
		{
		  LW6SYS_FREE (received_password);
		}
	      if (!ret)
		{
		  if (*remote_url)
		    {
		      LW6SYS_FREE (*remote_url);
		      (*remote_url) = NULL;
		    }
		}
	    }
	}
      LW6SYS_FREE (copy);
    }

  return ret;
}

/**
 * lw6msg_oob_analyse_pong
 *
 * @text: the text of the message to parse
 *
 * Analyses a PONG message and gets the public_url from it, if
 * it exists.
 *
 * Return value: newly allocated string containing public_url if OK, NULL on error.
 */
char *
lw6msg_oob_analyse_pong (const char *text)
{
  char *ret = NULL;
  char *copy = NULL;
  char *key = NULL;
  char *value = NULL;

  copy = lw6sys_str_copy (text);
  if (copy)
    {
      if (lw6msg_utils_parse_key_value_to_ptr (&key, &value, text))
	{
	  if (lw6sys_str_is_same (key, LW6MSG_OOB_PONG))
	    {
	      ret = value;
	    }
	  else
	    {
	      LW6SYS_FREE (value);
	    }
	  LW6SYS_FREE (key);
	}
      LW6SYS_FREE (copy);
    }

  return ret;
}
