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

#include "p2p.h"
#include "p2p-internal.h"

_lw6p2p_cli_oob_callback_data_t *
_lw6p2p_cli_oob_callback_data_new (lw6cli_backend_t * backend,
				   _lw6p2p_node_t * node, char *public_url)
{
  _lw6p2p_cli_oob_callback_data_t *ret = NULL;

  ret =
    (_lw6p2p_cli_oob_callback_data_t *)
    LW6SYS_CALLOC (sizeof (_lw6p2p_cli_oob_callback_data_t));
  if (ret)
    {
      ret->backend = backend;
      ret->node_info = node->node_info;
      ret->cli_oob =
	lw6cli_oob_new (public_url, _lw6p2p_cli_oob_verify_callback_func,
			(void *) node);
      if (!ret->cli_oob)
	{
	  LW6SYS_FREE (ret);
	  ret = NULL;
	}
    }

  return ret;
}

void
_lw6p2p_cli_oob_callback_data_free (_lw6p2p_cli_oob_callback_data_t * cli_oob)
{
  if (cli_oob)
    {
      if (cli_oob->cli_oob)
	{
	  lw6cli_oob_free (cli_oob->cli_oob);
	}
      LW6SYS_FREE (cli_oob);
    }
}

int
_lw6p2p_cli_oob_filter (_lw6p2p_cli_oob_callback_data_t * cli_oob)
{
  int ret = 1;
  void *thread;

  if (cli_oob && cli_oob->cli_oob)
    {
      thread = cli_oob->cli_oob->thread;
      if (thread)
	{
	  if (lw6sys_thread_is_callback_done (thread))
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
_lw6p2p_cli_oob_callback (void *callback_data)
{
  _lw6p2p_cli_oob_callback_data_t *cli_oob =
    (_lw6p2p_cli_oob_callback_data_t *) callback_data;
  int ret = 0;

  ret =
    lw6cli_process_oob (cli_oob->backend, cli_oob->node_info,
			&(cli_oob->cli_oob->data));

  lw6sys_log (LW6SYS_LOG_DEBUG, _("_cli_oob_callback done ret=%d"), ret);
}

int
_lw6p2p_cli_oob_verify_callback_func (void *func_data, char *url, char *ip,
				      int port, int ping_delay_msec,
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
  int now = 0;
  int uptime = 0;
  int bench = 0;
  char *level = NULL;
  int required_bench;
  int nb_colors = 0;
  int max_nb_colors = 0;
  int nb_cursors = 0;
  int max_nb_cursors = 0;
  int nb_nodes = 0;
  int max_nb_nodes = 0;

  char *query = NULL;

  remote_program = lw6sys_assoc_get (assoc, LW6MSG_OOB_PROGRAM);
  remote_version = lw6sys_assoc_get (assoc, LW6MSG_OOB_VERSION);
  remote_codename = lw6sys_assoc_get (assoc, LW6MSG_OOB_CODENAME);
  if (lw6sys_assoc_get (assoc, LW6MSG_OOB_STAMP))
    {
      remote_stamp_int =
	lw6sys_atoi (lw6sys_assoc_get (assoc, LW6MSG_OOB_STAMP));
    }
  remote_id = lw6sys_assoc_get (assoc, LW6MSG_OOB_ID);
  remote_url = lw6sys_assoc_get (assoc, LW6MSG_OOB_URL);
  remote_title = lw6sys_assoc_get (assoc, LW6MSG_OOB_TITLE);
  remote_description = lw6sys_assoc_get (assoc, LW6MSG_OOB_DESCRIPTION);

  if (remote_program && remote_version && remote_codename
      && remote_stamp_int > 0 && remote_id && remote_url
      && remote_title && remote_description)
    {
      if (lw6sys_str_is_same
	  (remote_program, lw6sys_build_get_package_tarname ()))
	{
	  if (lw6sys_str_is_same (remote_url, url))
	    {
	      remote_version = lw6sys_escape_sql_value (remote_version);
	      remote_codename = lw6sys_escape_sql_value (remote_codename);
	      remote_stamp_str = lw6sys_itoa (remote_stamp_int);
	      remote_id = lw6sys_escape_sql_value (remote_id);
	      remote_title = lw6sys_escape_sql_value (remote_title);
	      remote_description =
		lw6sys_escape_sql_value (remote_description);
	      if (remote_version && remote_codename
		  && remote_stamp_str && remote_id && remote_title
		  && remote_description)
		{
		  if (!lw6sys_str_is_same (url, node->public_url)
		      && !lw6sys_str_is_same (remote_id, node->node_id_str))
		    {
		      lw6sys_log (LW6SYS_LOG_DEBUG,
				  _("confirmed node \"%s\""), url);
		      now = lw6p2p_db_now ();
		      uptime =
			lw6msg_utils_get_assoc_int_with_default (assoc,
								 LW6MSG_OOB_UPTIME,
								 0);
		      bench =
			lw6msg_utils_get_assoc_int_with_default (assoc,
								 LW6MSG_OOB_BENCH,
								 0);
		      level =
			lw6msg_utils_get_assoc_str_with_default (assoc,
								 LW6MSG_OOB_LEVEL,
								 "");
		      required_bench =
			lw6msg_utils_get_assoc_int_with_default (assoc,
								 LW6MSG_OOB_REQUIRED_BENCH,
								 0);
		      nb_colors =
			lw6msg_utils_get_assoc_int_with_default (assoc,
								 LW6MSG_OOB_NB_COLORS,
								 0);
		      max_nb_colors =
			lw6msg_utils_get_assoc_int_with_default (assoc,
								 LW6MSG_OOB_MAX_NB_COLORS,
								 0);
		      nb_cursors =
			lw6msg_utils_get_assoc_int_with_default (assoc,
								 LW6MSG_OOB_NB_CURSORS,
								 0);
		      max_nb_cursors =
			lw6msg_utils_get_assoc_int_with_default (assoc,
								 LW6MSG_OOB_MAX_NB_CURSORS,
								 0);
		      nb_nodes =
			lw6msg_utils_get_assoc_int_with_default (assoc,
								 LW6MSG_OOB_NB_NODES,
								 0);
		      max_nb_nodes =
			lw6msg_utils_get_assoc_int_with_default (assoc,
								 LW6MSG_OOB_MAX_NB_NODES,
								 0);
		      query =
			lw6sys_new_sprintf (_lw6p2p_db_get_query
					    (node->db,
					     _LW6P2P_UPDATE_NODE_SQL),
					    now - uptime, remote_version,
					    remote_codename, remote_stamp_int,
					    remote_id, remote_title,
					    remote_description, bench, level,
					    required_bench, nb_colors,
					    max_nb_colors, nb_cursors,
					    max_nb_cursors, nb_nodes,
					    max_nb_nodes, ip, port, now,
					    ping_delay_msec, url);
		      if (query)
			{
			  if (_lw6p2p_db_lock (node->db))
			    {
			      ret =
				_lw6p2p_db_exec_ignore_data (node->db, query);
			      _lw6p2p_db_unlock (node->db);
			    }
			  LW6SYS_FREE (query);
			}
		    }
		  else
		    {
		      lw6sys_log (LW6SYS_LOG_WARNING,
				  _
				  ("node url=\"%s\" id=\"%s\" is local, not updating"),
				  url, node->node_id_str);
		    }
		  if (remote_version)
		    {
		      LW6SYS_FREE (remote_version);
		    }
		  if (remote_codename)
		    {
		      LW6SYS_FREE (remote_codename);
		    }
		  if (remote_stamp_str)
		    {
		      LW6SYS_FREE (remote_stamp_str);
		    }
		  if (remote_id)
		    {
		      LW6SYS_FREE (remote_id);
		    }
		  if (remote_title)
		    {
		      LW6SYS_FREE (remote_title);
		    }
		  if (remote_description)
		    {
		      LW6SYS_FREE (remote_description);
		    }
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _("wrong url \"%s\" vs \"%s\""), remote_url, url);
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("wrong remote program \"%s\""), remote_program);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _("answer does not containn the required fields"));
    }

  return ret;
}
