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

#include "liquidwar6.h"

/*
 * In liquidwar6p2p
 */
static SCM
_scm_lw6p2p_db_new (SCM name)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_name = NULL;
  lw6p2p_db_t *c_db = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (name), name, SCM_ARG1, __FUNCTION__);

  c_name = lw6scm_utils_to_0str (sys_context, name);
  if (c_name)
    {
      c_db = lw6p2p_db_open (sys_context, lw6_global.argc, lw6_global.argv, c_name);
      if (c_db)
	{
	  ret = lw6_make_scm_db (sys_context, c_db);
	}
      LW6SYS_FREE (sys_context, c_name);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_db_reset (SCM name)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_name = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (name), name, SCM_ARG1, __FUNCTION__);

  c_name = lw6scm_utils_to_0str (sys_context, name);
  if (c_name)
    {
      ret = lw6p2p_db_reset (sys_context, lw6_global.argc, lw6_global.argv, c_name) ? SCM_BOOL_T : SCM_BOOL_F;

      LW6SYS_FREE (sys_context, c_name);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_db_default_name ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6p2p_db_default_name (sys_context));

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_new (SCM db, SCM param)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  SCM client_backends = SCM_BOOL_F;
  SCM server_backends = SCM_BOOL_F;
  SCM bind_ip = SCM_BOOL_F;
  SCM bind_port = SCM_BOOL_F;
  SCM broadcast = SCM_BOOL_F;
  SCM node_id = SCM_BOOL_F;
  SCM public_url = SCM_BOOL_F;
  SCM password = SCM_BOOL_F;
  SCM title = SCM_BOOL_F;
  SCM description = SCM_BOOL_F;
  SCM bench = SCM_BOOL_F;
  SCM open_relay = SCM_BOOL_F;
  SCM known_nodes = SCM_BOOL_F;
  SCM network_reliability = SCM_BOOL_F;
  SCM trojan = SCM_BOOL_F;
  lw6p2p_db_t *c_db = NULL;
  char *c_client_backends = NULL;
  char *c_server_backends = NULL;
  char *c_bind_ip = NULL;
  int c_bind_port = 0;
  int c_broadcast = 0;
  char *c_node_id_str = NULL;
  u_int64_t c_node_id_int = 0LL;
  char *c_public_url = NULL;
  char *c_password = NULL;
  char *c_title = NULL;
  char *c_description = NULL;
  int c_open_relay = 0;
  int c_bench = 0;
  char *c_known_nodes = NULL;
  int c_network_reliability = 0;
  int c_trojan = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  if (lw6_global.net_initialized)
    {
      SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.db, db), db, SCM_ARG1, __FUNCTION__);
      SCM_ASSERT (SCM_CONSP (param) || param == SCM_EOL, param, SCM_ARG2, __FUNCTION__);

      client_backends = scm_assoc_ref (param, scm_from_locale_string ("client-backends"));
      server_backends = scm_assoc_ref (param, scm_from_locale_string ("server-backends"));
      bind_ip = scm_assoc_ref (param, scm_from_locale_string ("bind-ip"));
      bind_port = scm_assoc_ref (param, scm_from_locale_string ("bind-port"));
      broadcast = scm_assoc_ref (param, scm_from_locale_string ("broadcast"));
      node_id = scm_assoc_ref (param, scm_from_locale_string ("node-id"));
      public_url = scm_assoc_ref (param, scm_from_locale_string ("public-url"));
      password = scm_assoc_ref (param, scm_from_locale_string ("password"));
      title = scm_assoc_ref (param, scm_from_locale_string ("title"));
      description = scm_assoc_ref (param, scm_from_locale_string ("description"));
      bench = scm_assoc_ref (param, scm_from_locale_string ("bench"));
      open_relay = scm_assoc_ref (param, scm_from_locale_string ("open-relay"));
      known_nodes = scm_assoc_ref (param, scm_from_locale_string ("known-nodes"));
      network_reliability = scm_assoc_ref (param, scm_from_locale_string ("network-reliability"));
      trojan = scm_assoc_ref (param, scm_from_locale_string ("trojan"));

      SCM_ASSERT (scm_is_string (client_backends), param, SCM_ARG2, __FUNCTION__);
      SCM_ASSERT (scm_is_string (server_backends), param, SCM_ARG2, __FUNCTION__);
      SCM_ASSERT (scm_is_string (bind_ip), param, SCM_ARG2, __FUNCTION__);
      SCM_ASSERT (scm_is_integer (bind_port), param, SCM_ARG2, __FUNCTION__);
      SCM_ASSERT (SCM_BOOLP (broadcast), param, SCM_ARG2, __FUNCTION__);
      SCM_ASSERT (scm_is_string (node_id), param, SCM_ARG2, __FUNCTION__);
      SCM_ASSERT (scm_is_string (public_url), param, SCM_ARG2, __FUNCTION__);
      SCM_ASSERT (scm_is_string (password), param, SCM_ARG2, __FUNCTION__);
      SCM_ASSERT (scm_is_string (title), param, SCM_ARG2, __FUNCTION__);
      SCM_ASSERT (scm_is_string (description), param, SCM_ARG2, __FUNCTION__);
      SCM_ASSERT (scm_is_integer (bench), param, SCM_ARG2, __FUNCTION__);
      SCM_ASSERT (SCM_BOOLP (open_relay), param, SCM_ARG2, __FUNCTION__);
      SCM_ASSERT (scm_is_string (known_nodes), param, SCM_ARG2, __FUNCTION__);
      SCM_ASSERT (scm_is_integer (network_reliability), param, SCM_ARG2, __FUNCTION__);
      SCM_ASSERT (SCM_BOOLP (trojan), param, SCM_ARG2, __FUNCTION__);

      c_db = lw6_scm_to_db (sys_context, db);
      if (c_db)
	{
	  c_client_backends = lw6scm_utils_to_0str (sys_context, client_backends);
	  if (c_client_backends)
	    {
	      c_server_backends = lw6scm_utils_to_0str (sys_context, server_backends);
	      if (c_server_backends)
		{
		  c_bind_ip = lw6scm_utils_to_0str (sys_context, bind_ip);
		  if (c_bind_ip)
		    {
		      c_bind_port = scm_to_int (bind_port);
		      c_broadcast = SCM_NFALSEP (broadcast);
		      c_node_id_str = lw6scm_utils_to_0str (sys_context, node_id);
		      if (c_node_id_str)
			{
			  c_node_id_int = lw6sys_id_atol (sys_context, c_node_id_str);
			  c_public_url = lw6scm_utils_to_0str (sys_context, public_url);
			  if (c_public_url)
			    {
			      c_title = lw6scm_utils_to_0str (sys_context, title);
			      if (c_title)
				{
				  c_description = lw6scm_utils_to_0str (sys_context, description);
				  if (c_description)
				    {
				      c_password = lw6scm_utils_to_0str (sys_context, password);
				      if (c_password)
					{
					  c_bench = scm_to_int (bench);
					  c_open_relay = SCM_NFALSEP (open_relay);
					  c_known_nodes = lw6scm_utils_to_0str (sys_context, known_nodes);
					  if (c_known_nodes)
					    {
					      c_network_reliability = scm_to_int (network_reliability);
					      c_trojan = SCM_NFALSEP (trojan);

					      c_node =
						lw6p2p_node_new
						(sys_context, lw6_global.argc,
						 lw6_global.argv, c_db,
						 c_client_backends,
						 c_server_backends, c_bind_ip,
						 c_bind_port, c_broadcast,
						 c_node_id_int,
						 c_public_url,
						 c_title, c_description, c_password, c_bench, c_open_relay, c_known_nodes, c_network_reliability, c_trojan);
					      if (c_node)
						{
						  ret = lw6_make_scm_node (sys_context, c_node, db);
						}
					      LW6SYS_FREE (sys_context, c_known_nodes);
					    }
					  LW6SYS_FREE (sys_context, c_password);
					}
				      LW6SYS_FREE (sys_context, c_description);
				    }
				  LW6SYS_FREE (sys_context, c_title);
				}
			      LW6SYS_FREE (sys_context, c_public_url);
			    }
			  LW6SYS_FREE (sys_context, c_node_id_str);
			}
		      LW6SYS_FREE (sys_context, c_bind_ip);
		    }
		  LW6SYS_FREE (sys_context, c_server_backends);
		}
	      LW6SYS_FREE (sys_context, c_client_backends);
	    }
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_poll (SCM node)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      if (lw6p2p_node_poll (sys_context, c_node, NULL))
	{
	  ret = SCM_BOOL_T;
	}
    }
  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_close (SCM node)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      lw6p2p_node_close (sys_context, c_node);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6p2p_node_get_id (SCM node)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  u_int64_t c_ret_int = 0;
  char *c_ret_str = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_ret_int = lw6p2p_node_get_id (sys_context, c_node);
      c_ret_str = lw6sys_id_ltoa (sys_context, c_ret_int);
      if (c_ret_str)
	{
	  ret = scm_from_locale_string (c_ret_str);
	  LW6SYS_FREE (sys_context, c_ret_str);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static void
_lw6p2p_node_get_entries_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  SCM *ret = (SCM *) func_data;
  lw6p2p_entry_t *entry = (lw6p2p_entry_t *) data;
  SCM item;

  item = SCM_EOL;

  // constant data
  item = scm_cons (scm_cons (scm_from_locale_string ("creation-timestamp"), scm_from_int (entry->creation_timestamp)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("version"), scm_from_locale_string (entry->version)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("codename"), scm_from_locale_string (entry->codename)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("stamp"), scm_from_int (entry->stamp)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("id"), scm_from_locale_string (entry->id)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("url"), scm_from_locale_string (entry->url)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("title"), scm_from_locale_string (entry->title)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("description"), scm_from_locale_string (entry->description)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("has-password"), entry->has_password ? SCM_BOOL_T : SCM_BOOL_F), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("bench"), scm_from_int (entry->bench)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("open-relay"), entry->open_relay ? SCM_BOOL_T : SCM_BOOL_F), item);
  // variable data
  item = scm_cons (scm_cons (scm_from_locale_string ("community-id"), scm_from_locale_string (entry->community_id)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("round"), scm_from_int (entry->round)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("level"), scm_from_locale_string (entry->level)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("required-bench"), scm_from_int (entry->required_bench)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("nb-colors"), scm_from_int (entry->nb_colors)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("max-nb-colors"), scm_from_int (entry->max_nb_colors)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("nb-cursors"), scm_from_int (entry->nb_cursors)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("max-nb-cursors"), scm_from_int (entry->max_nb_cursors)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("nb-nodes"), scm_from_int (entry->nb_nodes)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("max-nb-nodes"), scm_from_int (entry->max_nb_nodes)), item);
  // additionnal data
  item = scm_cons (scm_cons (scm_from_locale_string ("ip"), scm_from_locale_string (entry->ip)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("port"), scm_from_int (entry->port)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("last-ping-timestamp"), scm_from_int (entry->last_ping_timestamp)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("ping-delay-msec"), scm_from_int (entry->ping_delay_msec)), item);
  item = scm_cons (scm_cons (scm_from_locale_string ("available"), entry->available ? SCM_BOOL_T : SCM_BOOL_F), item);

  item = scm_reverse (item);

  (*ret) = scm_cons (item, *ret);
}

static SCM
_scm_lw6p2p_node_get_entries (SCM node, SCM skip_local)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  int c_skip_local = 0;
  lw6sys_list_t *c_ret = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_BOOLP (skip_local), skip_local, SCM_ARG2, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_skip_local = SCM_NFALSEP (skip_local) ? 1 : 0;
      c_ret = lw6p2p_node_get_entries (sys_context, c_node, c_skip_local);
      if (c_ret)
	{
	  ret = SCM_EOL;
	  lw6sys_list_map (sys_context, c_ret, _lw6p2p_node_get_entries_callback, (void *) &ret);
	  lw6sys_list_free (sys_context, c_ret);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_server_start (SCM node, SCM seq_0)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  int64_t c_seq_0 = 0LL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (seq_0), seq_0, SCM_ARG2, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_seq_0 = scm_to_long_long (seq_0);

      ret = lw6p2p_node_server_start (sys_context, c_node, c_seq_0) ? SCM_BOOL_T : SCM_BOOL_F;
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_client_join (SCM node, SCM remote_id, SCM remote_url)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  char *c_remote_id_str = NULL;
  u_int64_t c_remote_id_int = 0LL;
  char *c_remote_url = NULL;
  lw6sys_progress_t progress;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (remote_id), remote_id, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_string (remote_url), remote_url, SCM_ARG3, __FUNCTION__);

  lw6sys_progress_default (sys_context, &progress, &(lw6_global.progress));
  lw6sys_progress_begin (sys_context, &progress);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_remote_id_str = lw6scm_utils_to_0str (sys_context, remote_id);
      if (c_remote_id_str)
	{
	  c_remote_id_int = lw6sys_id_atol (sys_context, c_remote_id_str);
	  if (c_remote_id_int > 0)
	    {
	      c_remote_url = lw6scm_utils_to_0str (sys_context, remote_url);
	      if (c_remote_url)
		{
		  ret = lw6p2p_node_client_join (sys_context, c_node, c_remote_id_int, c_remote_url, &progress) ? SCM_BOOL_T : SCM_BOOL_F;
		  LW6SYS_FREE (sys_context, c_remote_url);
		}
	    }
	  LW6SYS_FREE (sys_context, c_remote_id_str);
	}
    }

  lw6sys_progress_begin (sys_context, &progress);

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_refresh_peer (SCM node, SCM remote_id, SCM remote_url)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  char *c_remote_id_str = NULL;
  u_int64_t c_remote_id_int = 0LL;
  char *c_remote_url = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (remote_id), remote_id, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_string (remote_url), remote_url, SCM_ARG3, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_remote_id_str = lw6scm_utils_to_0str (sys_context, remote_id);
      if (c_remote_id_str)
	{
	  c_remote_id_int = lw6sys_id_atol (sys_context, c_remote_id_str);
	  if (c_remote_id_int > 0)
	    {
	      c_remote_url = lw6scm_utils_to_0str (sys_context, remote_url);
	      if (c_remote_url)
		{
		  ret = lw6p2p_node_refresh_peer (sys_context, c_node, c_remote_id_int, c_remote_url) ? SCM_BOOL_T : SCM_BOOL_F;
		  LW6SYS_FREE (sys_context, c_remote_url);
		}
	    }
	  LW6SYS_FREE (sys_context, c_remote_id_str);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_disconnect (SCM node)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      lw6p2p_node_disconnect (sys_context, c_node);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6p2p_node_update_info (SCM node, SCM param)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  SCM round = SCM_BOOL_F;
  SCM level = SCM_BOOL_F;
  SCM nb_colors = SCM_BOOL_F;
  SCM max_nb_colors = SCM_BOOL_F;
  SCM nb_cursors = SCM_BOOL_F;
  SCM max_nb_cursors = SCM_BOOL_F;
  SCM nb_nodes = SCM_BOOL_F;
  SCM max_nb_nodes = SCM_BOOL_F;
  SCM screenshot = SCM_BOOL_F;
  int c_round = 0;
  char *c_level = NULL;
  int c_nb_colors = 0;
  int c_max_nb_colors = 0;
  int c_nb_cursors = 0;
  int c_max_nb_cursors = 0;
  int c_nb_nodes = 0;
  int c_max_nb_nodes = 0;
  lw6img_jpeg_t *c_screenshot = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_CONSP (param) || param == SCM_EOL, param, SCM_ARG2, __FUNCTION__);

  round = scm_assoc_ref (param, scm_from_locale_string ("round"));
  level = scm_assoc_ref (param, scm_from_locale_string ("level"));
  nb_colors = scm_assoc_ref (param, scm_from_locale_string ("nb-colors"));
  max_nb_colors = scm_assoc_ref (param, scm_from_locale_string ("max-nb-colors"));
  nb_cursors = scm_assoc_ref (param, scm_from_locale_string ("nb-cursors"));
  max_nb_cursors = scm_assoc_ref (param, scm_from_locale_string ("max-nb-cursors"));
  nb_nodes = scm_assoc_ref (param, scm_from_locale_string ("nb-nodes"));
  max_nb_nodes = scm_assoc_ref (param, scm_from_locale_string ("max-nb-nodes"));
  screenshot = scm_assoc_ref (param, scm_from_locale_string ("screenshot"));

  SCM_ASSERT (scm_is_integer (round), param, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_string (level), param, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (nb_colors), param, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (max_nb_colors), param, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (nb_cursors), param, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (max_nb_cursors), param, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (nb_nodes), param, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (max_nb_nodes), param, SCM_ARG2, __FUNCTION__);
  if (SCM_NFALSEP (screenshot))
    {
      SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.jpeg, screenshot), param, SCM_ARG2, __FUNCTION__);
    }

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_round = scm_to_int (round);
      c_level = lw6scm_utils_to_0str (sys_context, level);
      if (c_level)
	{
	  c_nb_colors = scm_to_int (nb_colors);
	  c_max_nb_colors = scm_to_int (max_nb_colors);
	  c_nb_cursors = scm_to_int (nb_cursors);
	  c_max_nb_cursors = scm_to_int (max_nb_cursors);
	  c_nb_nodes = scm_to_int (nb_nodes);
	  c_max_nb_nodes = scm_to_int (max_nb_nodes);
	  if (SCM_NFALSEP (screenshot))
	    {
	      c_screenshot = lw6_scm_to_jpeg (sys_context, screenshot);
	      if (c_screenshot)
		{
		  ret =
		    lw6p2p_node_update_info (sys_context, c_node, c_round, c_level,
					     c_nb_colors, c_max_nb_colors,
					     c_nb_cursors, c_max_nb_cursors,
					     c_nb_nodes, c_max_nb_nodes, c_screenshot->jpeg_size, c_screenshot->jpeg_data) ? SCM_BOOL_T : SCM_BOOL_F;
		}
	    }
	  else
	    {
	      ret =
		lw6p2p_node_update_info (sys_context, c_node, c_round, c_level,
					 c_nb_colors, c_max_nb_colors,
					 c_nb_cursors, c_max_nb_cursors, c_nb_nodes, c_max_nb_nodes, 0, NULL) ? SCM_BOOL_T : SCM_BOOL_F;
	    }
	  LW6SYS_FREE (sys_context, c_level);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_calibrate (SCM node, SCM timestamp, SCM seq_0)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  int64_t c_timestamp = 0LL;
  int64_t c_seq_0 = 0LL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);

  SCM_ASSERT (scm_is_integer (seq_0), seq_0, SCM_ARG2, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_timestamp = scm_to_long_long (timestamp);
      c_seq_0 = scm_to_long_long (seq_0);

      lw6p2p_node_calibrate (sys_context, c_node, c_timestamp, c_seq_0);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6p2p_node_get_local_seq_0 (SCM node)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node;
  int64_t c_ret = 0LL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_ret = lw6p2p_node_get_local_seq_0 (sys_context, c_node);
      ret = scm_from_long_long (c_ret);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_get_local_seq_last (SCM node)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  int64_t c_ret = 0LL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_ret = lw6p2p_node_get_local_seq_last (sys_context, c_node);
      ret = scm_from_long_long (c_ret);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_get_seq_min (SCM node)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  int64_t c_ret = 0LL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_ret = lw6p2p_node_get_seq_min (sys_context, c_node);
      ret = scm_from_long_long (c_ret);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_get_seq_max (SCM node)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  int64_t c_ret = 0LL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_ret = lw6p2p_node_get_seq_max (sys_context, c_node);
      ret = scm_from_long_long (c_ret);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_get_seq_draft (SCM node)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  int64_t c_ret = 0LL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_ret = lw6p2p_node_get_seq_draft (sys_context, c_node);
      ret = scm_from_long_long (c_ret);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_get_seq_reference (SCM node)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  int64_t c_ret = 0LL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_ret = lw6p2p_node_get_seq_reference (sys_context, c_node);
      ret = scm_from_long_long (c_ret);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_is_peer_connected (SCM node, SCM peer_id)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  char *c_peer_id_str = NULL;
  u_int64_t c_peer_id_int = 0LL;
  int c_ret = 0LL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (peer_id), peer_id, SCM_ARG2, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_peer_id_str = lw6scm_utils_to_0str (sys_context, peer_id);
      if (c_peer_id_str)
	{
	  c_peer_id_int = lw6sys_id_atol (sys_context, c_peer_id_str);
	  if (c_peer_id_int > 0)
	    {
	      c_ret = lw6p2p_node_is_peer_connected (sys_context, c_node, c_peer_id_int);
	      ret = scm_from_int (c_ret);
	    }
	  LW6SYS_FREE (sys_context, c_peer_id_str);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_is_peer_registered (SCM node, SCM peer_id)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  char *c_peer_id_str = NULL;
  u_int64_t c_peer_id_int = 0LL;
  int c_ret = 0LL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (peer_id), peer_id, SCM_ARG2, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_peer_id_str = lw6scm_utils_to_0str (sys_context, peer_id);
      if (c_peer_id_str)
	{
	  c_peer_id_int = lw6sys_id_atol (sys_context, c_peer_id_str);
	  if (c_peer_id_int > 0)
	    {
	      c_ret = lw6p2p_node_is_peer_registered (sys_context, c_node, c_peer_id_int);
	      ret = scm_from_int (c_ret);
	    }
	  LW6SYS_FREE (sys_context, c_peer_id_str);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_is_seed_needed (SCM node)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      ret = lw6p2p_node_is_seed_needed (sys_context, c_node) ? SCM_BOOL_T : SCM_BOOL_F;
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_is_dump_needed (SCM node)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      ret = lw6p2p_node_is_dump_needed (sys_context, c_node) ? SCM_BOOL_T : SCM_BOOL_F;
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_put_local_msg (SCM node, SCM msg)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  char *c_msg = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (msg), msg, SCM_ARG2, __FUNCTION__);

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_msg = lw6scm_utils_to_0str (sys_context, msg);
      if (c_msg)
	{
	  ret = lw6p2p_node_put_local_msg (sys_context, c_node, c_msg) ? SCM_BOOL_T : SCM_BOOL_F;
	  LW6SYS_FREE (sys_context, c_msg);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_get_next_reference_msg (SCM node)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  char *c_ret = NULL;
  lw6sys_progress_t progress;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);

  lw6sys_progress_default (sys_context, &progress, &(lw6_global.progress));
  // do *not* call lw6sys_progress_begin here!

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_ret = lw6p2p_node_get_next_reference_msg (sys_context, c_node, &progress);
      if (c_ret)
	{
	  ret = scm_from_locale_string (c_ret);
	  LW6SYS_FREE (sys_context, c_ret);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_get_next_draft_msg (SCM node)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = NULL;
  char *c_ret = NULL;
  lw6sys_progress_t progress;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.node, node), node, SCM_ARG1, __FUNCTION__);

  lw6sys_progress_default (sys_context, &progress, &(lw6_global.progress));
  // do *not* call lw6sys_progress_begin here!

  c_node = lw6_scm_to_node (sys_context, node);
  if (c_node)
    {
      c_ret = lw6p2p_node_get_next_draft_msg (sys_context, c_node, &progress);
      if (c_ret)
	{
	  ret = scm_from_locale_string (c_ret);
	  LW6SYS_FREE (sys_context, c_ret);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/**
 * lw6_register_funcs_p2p
 *
 * @sys_context: global system context
 *
 * Register the functions of the p2p module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_p2p (lw6sys_context_t * sys_context)
{
  int ret = 1;

  /*
   * In liquidwar6p2p
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_DB_NEW, 1, 0, 0, (SCM (*)())_scm_lw6p2p_db_new);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_DB_RESET, 1, 0, 0, (SCM (*)())_scm_lw6p2p_db_reset);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_DB_DEFAULT_NAME, 0, 0, 0, (SCM (*)())_scm_lw6p2p_db_default_name);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_NEW, 2, 0, 0, (SCM (*)())_scm_lw6p2p_node_new);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_POLL, 1, 0, 0, (SCM (*)())_scm_lw6p2p_node_poll);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_CLOSE, 1, 0, 0, (SCM (*)())_scm_lw6p2p_node_close);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_GET_ID, 1, 0, 0, (SCM (*)())_scm_lw6p2p_node_get_id);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_GET_ENTRIES, 2, 0, 0, (SCM (*)())_scm_lw6p2p_node_get_entries);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_SERVER_START, 2, 0, 0, (SCM (*)())_scm_lw6p2p_node_server_start);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_CLIENT_JOIN, 3, 0, 0, (SCM (*)())_scm_lw6p2p_node_client_join);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_REFRESH_PEER, 3, 0, 0, (SCM (*)())_scm_lw6p2p_node_refresh_peer);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_DISCONNECT, 1, 0, 0, (SCM (*)())_scm_lw6p2p_node_disconnect);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_UPDATE_INFO, 2, 0, 0, (SCM (*)())_scm_lw6p2p_node_update_info);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_CALIBRATE, 3, 0, 0, (SCM (*)())_scm_lw6p2p_node_calibrate);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_GET_LOCAL_SEQ_0, 1, 0, 0, (SCM (*)())_scm_lw6p2p_node_get_local_seq_0);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_GET_LOCAL_SEQ_LAST, 1, 0, 0, (SCM (*)())_scm_lw6p2p_node_get_local_seq_last);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_GET_SEQ_MIN, 1, 0, 0, (SCM (*)())_scm_lw6p2p_node_get_seq_min);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_GET_SEQ_MAX, 1, 0, 0, (SCM (*)())_scm_lw6p2p_node_get_seq_max);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_GET_SEQ_DRAFT, 1, 0, 0, (SCM (*)())_scm_lw6p2p_node_get_seq_draft);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_GET_SEQ_REFERENCE, 1, 0, 0, (SCM (*)())_scm_lw6p2p_node_get_seq_reference);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_IS_PEER_CONNECTED, 2, 0, 0, (SCM (*)())_scm_lw6p2p_node_is_peer_connected);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_IS_PEER_REGISTERED, 2, 0, 0, (SCM (*)())_scm_lw6p2p_node_is_peer_registered);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_IS_SEED_NEEDED, 1, 0, 0, (SCM (*)())_scm_lw6p2p_node_is_seed_needed);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_IS_DUMP_NEEDED, 1, 0, 0, (SCM (*)())_scm_lw6p2p_node_is_dump_needed);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_PUT_LOCAL_MSG, 2, 0, 0, (SCM (*)())_scm_lw6p2p_node_put_local_msg);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_GET_NEXT_REFERENCE_MSG, 1, 0, 0, (SCM (*)())_scm_lw6p2p_node_get_next_reference_msg);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6P2P_NODE_GET_NEXT_DRAFT_MSG, 1, 0, 0, (SCM (*)())_scm_lw6p2p_node_get_next_draft_msg);

  return ret;
}
