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

#include "p2p.h"
#include "p2p-internal.h"

_lw6p2p_cli_oob_callback_data_t *
_lw6p2p_cli_oob_callback_data_new (lw6sys_context_t * sys_context, lw6cli_backend_t * backend, _lw6p2p_node_t * node, const char *public_url)
{
  _lw6p2p_cli_oob_callback_data_t *ret = NULL;

  ret = (_lw6p2p_cli_oob_callback_data_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6p2p_cli_oob_callback_data_t));
  if (ret)
    {
      ret->backend = backend;
      ret->node_info = node->node_info;
      ret->cli_oob = lw6cli_oob_new (sys_context, public_url, _lw6p2p_cli_oob_verify_callback_func, (void *) node);
      if (!ret->cli_oob)
	{
	  LW6SYS_FREE (sys_context, ret);
	  ret = NULL;
	}
    }

  return ret;
}

void
_lw6p2p_cli_oob_callback_data_free (lw6sys_context_t * sys_context, _lw6p2p_cli_oob_callback_data_t * cli_oob)
{
  if (cli_oob)
    {
      if (cli_oob->cli_oob)
	{
	  lw6cli_oob_free (sys_context, cli_oob->cli_oob);
	}
      LW6SYS_FREE (sys_context, cli_oob);
    }
}

int
_lw6p2p_cli_oob_filter (lw6sys_context_t * sys_context, _lw6p2p_cli_oob_callback_data_t * cli_oob)
{
  int ret = 1;
  lw6sys_thread_handler_t *thread;

  if (cli_oob && cli_oob->cli_oob)
    {
      thread = cli_oob->cli_oob->thread;
      if (thread)
	{
	  if (lw6sys_thread_is_callback_done (sys_context, thread))
	    {
	      /*
	       * We don't need to join the thread, it will
	       * be done when deleting the cli_oob object.
	       */
	      ret = 0;
	    }
	}
      else
	{
	  ret = 0;		// no thread, we exit
	}
    }

  return ret;
}

void
_lw6p2p_cli_oob_callback (lw6sys_context_t * sys_context, void *callback_data)
{
  _lw6p2p_cli_oob_callback_data_t *cli_oob = (_lw6p2p_cli_oob_callback_data_t *) callback_data;
  int ret = 0;

  ret = lw6cli_process_oob (sys_context, cli_oob->backend, cli_oob->node_info, &(cli_oob->cli_oob->data));

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("_cli_oob_callback done ret=%d"), ret);
}

int
_lw6p2p_cli_oob_verify_callback_func (lw6sys_context_t * sys_context, void *func_data, const char *url, const char *ip, int port, int ping_delay_msec,
				      lw6sys_assoc_t * assoc)
{
  int ret = 0;
  _lw6p2p_node_t *node = (_lw6p2p_node_t *) func_data;
  char *remote_program = NULL;
  char *remote_version = NULL;
  char *remote_codename = NULL;
  char *remote_stamp_str = NULL;
  int remote_stamp_int = 0;
  char *remote_id = NULL;
  char *remote_url = NULL;
  char *remote_title = NULL;
  char *remote_description = NULL;
  int bench = 0;
  char *open_relay_str = NULL;
  int open_relay_int = _LW6P2P_DB_FALSE;
  char *has_password_str = NULL;
  int has_password_int = _LW6P2P_DB_FALSE;
  int now = 0;
  int uptime = 0;
  char *community_id = NULL;
  int round = 0;
  char *level = NULL;
  int required_bench = 0;
  int nb_colors = 0;
  int max_nb_colors = 0;
  int nb_cursors = 0;
  int max_nb_cursors = 0;
  int nb_nodes = 0;
  int max_nb_nodes = 0;

  remote_program = lw6sys_assoc_get (sys_context, assoc, LW6MSG_OOB_PROGRAM);
  remote_version = lw6sys_assoc_get (sys_context, assoc, LW6MSG_OOB_VERSION);
  remote_codename = lw6sys_assoc_get (sys_context, assoc, LW6MSG_OOB_CODENAME);
  remote_stamp_str = lw6sys_assoc_get (sys_context, assoc, LW6MSG_OOB_STAMP);
  if (remote_stamp_str)
    {
      remote_stamp_int = lw6sys_atoi (sys_context, remote_stamp_str);
    }
  remote_id = lw6sys_assoc_get (sys_context, assoc, LW6MSG_OOB_ID);
  remote_url = lw6sys_assoc_get (sys_context, assoc, LW6MSG_OOB_URL);
  remote_title = lw6sys_assoc_get (sys_context, assoc, LW6MSG_OOB_TITLE);
  remote_description = lw6sys_assoc_get (sys_context, assoc, LW6MSG_OOB_DESCRIPTION);

  if (remote_program && remote_version && remote_codename && remote_stamp_int > 0 && remote_id && remote_url && remote_title && remote_description)
    {
      if (lw6sys_str_is_same (sys_context, remote_program, lw6sys_build_get_package_tarname ()))
	{
	  if (lw6sys_str_is_same (sys_context, remote_url, url))
	    {
	      if (remote_version && remote_codename && remote_stamp_int && remote_id && remote_title && remote_description)
		{
		  if (!lw6sys_str_is_same (sys_context, url, node->public_url) && !lw6sys_str_is_same (sys_context, remote_id, node->node_id_str))
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("confirmed node \"%s\""), url);
		      has_password_str = lw6msg_utils_get_assoc_str_with_default (sys_context, assoc, LW6MSG_OOB_HAS_PASSWORD, LW6MSG_NO);
		      has_password_int = (has_password_str
					  && lw6sys_str_starts_with_no_case (sys_context, has_password_str, LW6MSG_YES)) ? _LW6P2P_DB_TRUE : _LW6P2P_DB_FALSE;
		      bench = lw6msg_utils_get_assoc_int_with_default (sys_context, assoc, LW6MSG_OOB_BENCH, 0);
		      open_relay_str = lw6msg_utils_get_assoc_str_with_default (sys_context, assoc, LW6MSG_OOB_OPEN_RELAY, LW6MSG_NO);
		      open_relay_int = (open_relay_str
					&& lw6sys_str_starts_with_no_case (sys_context, open_relay_str, LW6MSG_YES)) ? _LW6P2P_DB_TRUE : _LW6P2P_DB_FALSE;
		      now = _lw6p2p_db_now (sys_context, node->db);
		      uptime = lw6msg_utils_get_assoc_int_with_default (sys_context, assoc, LW6MSG_OOB_UPTIME, 0);
		      community_id = lw6sys_assoc_get (sys_context, assoc, LW6MSG_OOB_COMMUNITY);
		      round = lw6msg_utils_get_assoc_int_with_default (sys_context, assoc, LW6MSG_OOB_ROUND, 0);
		      level = lw6msg_utils_get_assoc_str_with_default (sys_context, assoc, LW6MSG_OOB_LEVEL, LW6SYS_STR_EMPTY);
		      required_bench = lw6msg_utils_get_assoc_int_with_default (sys_context, assoc, LW6MSG_OOB_REQUIRED_BENCH, 0);
		      nb_colors = lw6msg_utils_get_assoc_int_with_default (sys_context, assoc, LW6MSG_OOB_NB_COLORS, 0);
		      max_nb_colors = lw6msg_utils_get_assoc_int_with_default (sys_context, assoc, LW6MSG_OOB_MAX_NB_COLORS, 0);
		      nb_cursors = lw6msg_utils_get_assoc_int_with_default (sys_context, assoc, LW6MSG_OOB_NB_CURSORS, 0);
		      max_nb_cursors = lw6msg_utils_get_assoc_int_with_default (sys_context, assoc, LW6MSG_OOB_MAX_NB_CURSORS, 0);
		      nb_nodes = lw6msg_utils_get_assoc_int_with_default (sys_context, assoc, LW6MSG_OOB_NB_NODES, 0);
		      max_nb_nodes = lw6msg_utils_get_assoc_int_with_default (sys_context, assoc, LW6MSG_OOB_MAX_NB_NODES, 0);


		      ret =
			_lw6p2p_node_update_peer_info_x (sys_context, node, remote_version,
							 remote_codename,
							 remote_stamp_int,
							 remote_id,
							 remote_url,
							 remote_title,
							 remote_description,
							 has_password_int,
							 bench,
							 open_relay_int,
							 now - uptime,
							 community_id, round,
							 level, required_bench, nb_colors, max_nb_colors, nb_cursors, max_nb_cursors, nb_nodes, max_nb_nodes);
		      ret = _lw6p2p_node_update_peer_net (sys_context, node, remote_id, remote_url, ip, port, now, ping_delay_msec);
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
				  _x_
				  ("node url=\"%s\" id=\"%s\" is local, not updating, this should only happen when several nodes share the same db"),
				  url, node->node_id_str);
		    }
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("wrong url \"%s\" vs \"%s\""), remote_url, url);
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("wrong remote program \"%s\""), remote_program);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("answer does not containn the required fields"));
    }

  return ret;
}
