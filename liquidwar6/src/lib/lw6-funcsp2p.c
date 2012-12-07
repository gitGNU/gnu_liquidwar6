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
#endif // HAVE_CONFIG_H

#include "liquidwar6.h"

/*
 * In liquidwar6p2p
 */
static SCM
_scm_lw6p2p_db_new (SCM name)
{
  SCM ret = SCM_BOOL_F;
  char *c_name = NULL;
  lw6p2p_db_t *c_db;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (name), name, SCM_ARG1, __FUNCTION__);

  c_name = lw6scm_utils_to_0str (name);
  if (c_name)
    {
      c_db = lw6p2p_db_open (lw6_global.argc, lw6_global.argv, c_name);
      if (c_db)
	{
	  ret = lw6_make_scm_db (c_db);
	}
      LW6SYS_FREE (c_name);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_db_reset (SCM name)
{
  SCM ret = SCM_BOOL_F;
  char *c_name = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (name), name, SCM_ARG1, __FUNCTION__);

  c_name = lw6scm_utils_to_0str (name);
  if (c_name)
    {
      ret =
	lw6p2p_db_reset (lw6_global.argc, lw6_global.argv,
			 c_name) ? SCM_BOOL_T : SCM_BOOL_F;

      LW6SYS_FREE (c_name);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_db_default_name ()
{
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6p2p_db_default_name ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_new (SCM db, SCM param)
{
  lw6p2p_node_t *c_node;
  SCM ret = SCM_BOOL_F;
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
  lw6p2p_db_t *c_db;
  char *c_client_backends;
  char *c_server_backends;
  char *c_bind_ip;
  int c_bind_port;
  int c_broadcast;
  char *c_node_id_str;
  u_int64_t c_node_id_int;
  char *c_public_url;
  char *c_password;
  char *c_title;
  char *c_description;
  int c_open_relay;
  int c_bench;
  char *c_known_nodes;
  int c_network_reliability;
  int c_trojan;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  if (lw6_global.net_initialized)
    {
      SCM_ASSERT (SCM_SMOB_PREDICATE
		  (lw6_global.smob_types.db, db), db, SCM_ARG1, __FUNCTION__);
      SCM_ASSERT (SCM_CONSP (param)
		  || param == SCM_EOL, param, SCM_ARG2, __FUNCTION__);

      client_backends =
	scm_assoc_ref (param, scm_from_locale_string ("client-backends"));
      server_backends =
	scm_assoc_ref (param, scm_from_locale_string ("server-backends"));
      bind_ip = scm_assoc_ref (param, scm_from_locale_string ("bind-ip"));
      bind_port = scm_assoc_ref (param, scm_from_locale_string ("bind-port"));
      broadcast = scm_assoc_ref (param, scm_from_locale_string ("broadcast"));
      node_id = scm_assoc_ref (param, scm_from_locale_string ("node-id"));
      public_url =
	scm_assoc_ref (param, scm_from_locale_string ("public-url"));
      password = scm_assoc_ref (param, scm_from_locale_string ("password"));
      title = scm_assoc_ref (param, scm_from_locale_string ("title"));
      description =
	scm_assoc_ref (param, scm_from_locale_string ("description"));
      bench = scm_assoc_ref (param, scm_from_locale_string ("bench"));
      open_relay =
	scm_assoc_ref (param, scm_from_locale_string ("open-relay"));
      known_nodes =
	scm_assoc_ref (param, scm_from_locale_string ("known-nodes"));
      network_reliability =
	scm_assoc_ref (param, scm_from_locale_string ("network-reliability"));
      trojan = scm_assoc_ref (param, scm_from_locale_string ("trojan"));

      SCM_ASSERT (scm_is_string (client_backends), param, SCM_ARG2,
		  __FUNCTION__);
      SCM_ASSERT (scm_is_string (server_backends), param, SCM_ARG2,
		  __FUNCTION__);
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
      SCM_ASSERT (scm_is_integer (network_reliability), param, SCM_ARG2,
		  __FUNCTION__);
      SCM_ASSERT (SCM_BOOLP (trojan), param, SCM_ARG2, __FUNCTION__);

      c_db = lw6_scm_to_db (db);
      if (c_db)
	{
	  c_client_backends = lw6scm_utils_to_0str (client_backends);
	  if (c_client_backends)
	    {
	      c_server_backends = lw6scm_utils_to_0str (server_backends);
	      if (c_server_backends)
		{
		  c_bind_ip = lw6scm_utils_to_0str (bind_ip);
		  if (c_bind_ip)
		    {
		      c_bind_port = scm_to_int (bind_port);
		      c_broadcast = SCM_NFALSEP (broadcast);
		      c_node_id_str = lw6scm_utils_to_0str (node_id);
		      if (c_node_id_str)
			{
			  c_node_id_int = lw6sys_id_atol (c_node_id_str);
			  c_public_url = lw6scm_utils_to_0str (public_url);
			  if (c_public_url)
			    {
			      c_title = lw6scm_utils_to_0str (title);
			      if (c_title)
				{
				  c_description =
				    lw6scm_utils_to_0str (description);
				  if (c_description)
				    {
				      c_password =
					lw6scm_utils_to_0str (password);
				      if (c_password)
					{
					  c_bench = scm_to_int (bench);
					  c_open_relay =
					    SCM_NFALSEP (open_relay);
					  c_known_nodes =
					    lw6scm_utils_to_0str
					    (known_nodes);
					  if (c_known_nodes)
					    {
					      c_network_reliability =
						scm_to_int
						(network_reliability);
					      c_trojan = SCM_NFALSEP (trojan);

					      c_node =
						lw6p2p_node_new
						(lw6_global.argc,
						 lw6_global.argv, c_db,
						 c_client_backends,
						 c_server_backends, c_bind_ip,
						 c_bind_port, c_broadcast,
						 c_node_id_int,
						 c_public_url,
						 c_title,
						 c_description, c_password,
						 c_bench, c_open_relay,
						 c_known_nodes,
						 c_network_reliability,
						 c_trojan);
					      if (c_node)
						{
						  ret =
						    lw6_make_scm_node (c_node,
								       db);
						}
					      LW6SYS_FREE (c_known_nodes);
					    }
					  LW6SYS_FREE (c_password);
					}
				      LW6SYS_FREE (c_description);
				    }
				  LW6SYS_FREE (c_title);
				}
			      LW6SYS_FREE (c_public_url);
			    }
			  LW6SYS_FREE (c_node_id_str);
			}
		      LW6SYS_FREE (c_bind_ip);
		    }
		  LW6SYS_FREE (c_server_backends);
		}
	      LW6SYS_FREE (c_client_backends);
	    }
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_poll (SCM node)
{
  lw6p2p_node_t *c_node;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.node,
	       node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (node);
  if (c_node)
    {
      if (lw6p2p_node_poll (c_node, NULL))
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
  lw6p2p_node_t *c_node;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.node,
	       node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (node);
  if (c_node)
    {
      lw6p2p_node_close (c_node);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6p2p_node_get_id (SCM node)
{
  lw6p2p_node_t *c_node;
  SCM ret = SCM_BOOL_F;
  u_int64_t c_ret_int = 0;
  char *c_ret_str = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.node,
	       node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (node);
  if (c_node)
    {
      c_ret_int = lw6p2p_node_get_id (c_node);
      c_ret_str = lw6sys_id_ltoa (c_ret_int);
      if (c_ret_str)
	{
	  ret = scm_from_locale_string (c_ret_str);
	  LW6SYS_FREE (c_ret_str);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static void
_lw6p2p_node_get_entries_callback (void *func_data, void *data)
{
  SCM *ret = (SCM *) func_data;
  lw6p2p_entry_t *entry = (lw6p2p_entry_t *) data;
  SCM item;

  item = SCM_EOL;

  // constant data
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("creation-timestamp"),
	       scm_from_int (entry->creation_timestamp)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("version"),
	       scm_from_locale_string (entry->version)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("codename"),
	       scm_from_locale_string (entry->codename)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("stamp"), scm_from_int (entry->stamp)),
	      item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("id"),
	       scm_from_locale_string (entry->id)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("url"),
	       scm_from_locale_string (entry->url)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("title"),
	       scm_from_locale_string (entry->title)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("description"),
	       scm_from_locale_string (entry->description)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("has-password"),
	       entry->has_password ? SCM_BOOL_T : SCM_BOOL_F), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("bench"), scm_from_int (entry->bench)),
	      item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("open-relay"),
	       entry->open_relay ? SCM_BOOL_T : SCM_BOOL_F), item);
  // variable data
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("community-id"),
	       scm_from_locale_string (entry->community_id)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("round"), scm_from_int (entry->round)),
	      item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("level"),
	       scm_from_locale_string (entry->level)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("required-bench"),
	       scm_from_int (entry->required_bench)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("nb-colors"),
	       scm_from_int (entry->nb_colors)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("max-nb-colors"),
	       scm_from_int (entry->max_nb_colors)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("nb-cursors"),
	       scm_from_int (entry->nb_cursors)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("max-nb-cursors"),
	       scm_from_int (entry->max_nb_cursors)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("nb-nodes"),
	       scm_from_int (entry->nb_nodes)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("max-nb-nodes"),
	       scm_from_int (entry->max_nb_nodes)), item);
  // additionnal data
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("ip"),
	       scm_from_locale_string (entry->ip)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("port"), scm_from_int (entry->port)),
	      item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("last-ping-timestamp"),
	       scm_from_int (entry->last_ping_timestamp)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("ping-delay-msec"),
	       scm_from_int (entry->ping_delay_msec)), item);
  item =
    scm_cons (scm_cons
	      (scm_from_locale_string ("available"),
	       entry->available ? SCM_BOOL_T : SCM_BOOL_F), item);

  item = scm_reverse (item);

  (*ret) = scm_cons (item, *ret);
}

static SCM
_scm_lw6p2p_node_get_entries (SCM node)
{
  lw6p2p_node_t *c_node;
  SCM ret = SCM_BOOL_F;
  lw6sys_list_t *c_ret = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.node,
	       node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (node);
  if (c_node)
    {
      c_ret = lw6p2p_node_get_entries (c_node);
      if (c_ret)
	{
	  ret = SCM_EOL;
	  lw6sys_list_map (c_ret, _lw6p2p_node_get_entries_callback,
			   (void *) &ret);
	  lw6sys_list_free (c_ret);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_server_start (SCM node, SCM seq_0)
{
  lw6p2p_node_t *c_node;
  int64_t c_seq_0;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.node,
	       node), node, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (seq_0), seq_0, SCM_ARG2, __FUNCTION__);

  c_node = lw6_scm_to_node (node);
  if (c_node)
    {
      c_seq_0 = scm_to_long_long (seq_0);

      ret =
	lw6p2p_node_server_start (c_node, c_seq_0) ? SCM_BOOL_T : SCM_BOOL_F;
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_client_join (SCM node, SCM remote_id, SCM remote_url)
{
  lw6p2p_node_t *c_node;
  char *c_remote_id_str = NULL;
  u_int64_t c_remote_id_int = 0LL;
  char *c_remote_url = NULL;
  lw6sys_progress_t progress;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.node,
	       node), node, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (remote_id), remote_id, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_string (remote_url), remote_url, SCM_ARG3, __FUNCTION__);

  lw6sys_progress_default (&progress, &(lw6_global.progress));
  lw6sys_progress_begin (&progress);

  c_node = lw6_scm_to_node (node);
  if (c_node)
    {
      c_remote_id_str = lw6scm_utils_to_0str (remote_id);
      if (c_remote_id_str)
	{
	  c_remote_id_int = lw6sys_id_atol (c_remote_id_str);
	  if (c_remote_id_int > 0)
	    {
	      c_remote_url = lw6scm_utils_to_0str (remote_url);
	      if (c_remote_url)
		{
		  ret =
		    lw6p2p_node_client_join (c_node, c_remote_id_int,
					     c_remote_url,
					     &progress) ? SCM_BOOL_T :
		    SCM_BOOL_F;
		  LW6SYS_FREE (c_remote_url);
		}
	    }
	  LW6SYS_FREE (c_remote_id_str);
	}
    }

  lw6sys_progress_begin (&progress);

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_disconnect (SCM node)
{
  lw6p2p_node_t *c_node;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.node,
	       node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (node);
  if (c_node)
    {
      lw6p2p_node_disconnect (c_node);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6p2p_node_update_info (SCM node, SCM param)
{
  lw6p2p_node_t *c_node;
  SCM round = SCM_BOOL_F;
  SCM level = SCM_BOOL_F;
  SCM nb_colors = SCM_BOOL_F;
  SCM max_nb_colors = SCM_BOOL_F;
  SCM nb_cursors = SCM_BOOL_F;
  SCM max_nb_cursors = SCM_BOOL_F;
  SCM nb_nodes = SCM_BOOL_F;
  SCM max_nb_nodes = SCM_BOOL_F;
  SCM screenshot = SCM_BOOL_F;
  SCM ret = SCM_BOOL_F;
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
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.node,
	       node), node, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_CONSP (param)
	      || param == SCM_EOL, param, SCM_ARG2, __FUNCTION__);

  round = scm_assoc_ref (param, scm_from_locale_string ("round"));
  level = scm_assoc_ref (param, scm_from_locale_string ("level"));
  nb_colors = scm_assoc_ref (param, scm_from_locale_string ("nb-colors"));
  max_nb_colors =
    scm_assoc_ref (param, scm_from_locale_string ("max-nb-colors"));
  nb_cursors = scm_assoc_ref (param, scm_from_locale_string ("nb-cursors"));
  max_nb_cursors =
    scm_assoc_ref (param, scm_from_locale_string ("max-nb-cursors"));
  nb_nodes = scm_assoc_ref (param, scm_from_locale_string ("nb-nodes"));
  max_nb_nodes =
    scm_assoc_ref (param, scm_from_locale_string ("max-nb-nodes"));
  screenshot = scm_assoc_ref (param, scm_from_locale_string ("screenshot"));

  SCM_ASSERT (scm_is_integer (round), param, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_string (level), param, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (nb_colors), param, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (max_nb_colors), param, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (nb_cursors), param, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (max_nb_cursors), param, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (nb_nodes), param, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (max_nb_nodes), param, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.jpeg,
	       screenshot), param, SCM_ARG2, __FUNCTION__);

  c_node = lw6_scm_to_node (node);
  if (c_node)
    {
      c_round = scm_to_int (round);
      c_level = lw6scm_utils_to_0str (level);
      if (c_level)
	{
	  c_nb_colors = scm_to_int (nb_colors);
	  c_max_nb_colors = scm_to_int (max_nb_colors);
	  c_nb_cursors = scm_to_int (nb_cursors);
	  c_max_nb_cursors = scm_to_int (max_nb_cursors);
	  c_nb_nodes = scm_to_int (nb_nodes);
	  c_max_nb_nodes = scm_to_int (max_nb_nodes);
	  c_screenshot = lw6_scm_to_jpeg (screenshot);
	  if (c_screenshot)
	    {
	      ret =
		lw6p2p_node_update_info (c_node, c_round, c_level,
					 c_nb_colors, c_max_nb_colors,
					 c_nb_cursors, c_max_nb_cursors,
					 c_nb_nodes, c_max_nb_nodes,
					 c_screenshot->jpeg_size,
					 c_screenshot->jpeg_data) ? SCM_BOOL_T
		: SCM_BOOL_F;
	    }
	  LW6SYS_FREE (c_level);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_calibrate (SCM node, SCM timestamp, SCM seq_0)
{
  lw6p2p_node_t *c_node;
  int64_t c_timestamp = 0LL;
  int64_t c_seq_0 = 0LL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.node,
	       node), node, SCM_ARG1, __FUNCTION__);

  SCM_ASSERT (scm_is_integer (seq_0), seq_0, SCM_ARG2, __FUNCTION__);

  c_node = lw6_scm_to_node (node);
  if (c_node)
    {
      c_timestamp = scm_to_long_long (timestamp);
      c_seq_0 = scm_to_long_long (seq_0);

      lw6p2p_node_calibrate (c_node, c_timestamp, c_seq_0);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6p2p_node_get_seq_max (SCM node)
{
  lw6p2p_node_t *c_node;
  int64_t c_ret = 0LL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.node,
	       node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (node);
  if (c_node)
    {
      c_ret = lw6p2p_node_get_seq_max (c_node);
      ret = scm_from_long_long (c_ret);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_is_seed_needed (SCM node)
{
  lw6p2p_node_t *c_node;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.node,
	       node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (node);
  if (c_node)
    {
      ret = lw6p2p_node_is_seed_needed (c_node) ? SCM_BOOL_T : SCM_BOOL_F;
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_is_dump_needed (SCM node)
{
  lw6p2p_node_t *c_node;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.node,
	       node), node, SCM_ARG1, __FUNCTION__);

  c_node = lw6_scm_to_node (node);
  if (c_node)
    {
      ret = lw6p2p_node_is_dump_needed (c_node) ? SCM_BOOL_T : SCM_BOOL_F;
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_put_local_msg (SCM node, SCM msg)
{
  lw6p2p_node_t *c_node;
  char *c_msg = NULL;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.node, node), node, SCM_ARG1,
	      __FUNCTION__);
  SCM_ASSERT (scm_is_string (msg), msg, SCM_ARG2, __FUNCTION__);

  c_node = lw6_scm_to_node (node);
  if (c_node)
    {
      c_msg = lw6scm_utils_to_0str (msg);
      if (c_msg)
	{
	  ret =
	    lw6p2p_node_put_local_msg (c_node,
				       c_msg) ? SCM_BOOL_T : SCM_BOOL_F;
	  LW6SYS_FREE (c_msg);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_get_next_reference_msg (SCM node)
{
  lw6p2p_node_t *c_node;
  char *c_ret = NULL;
  SCM ret = SCM_BOOL_F;
  lw6sys_progress_t progress;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.node,
	       node), node, SCM_ARG1, __FUNCTION__);

  lw6sys_progress_default (&progress, &(lw6_global.progress));
  // do *not* call lw6sys_progress_begin here!

  c_node = lw6_scm_to_node (node);
  if (c_node)
    {
      c_ret = lw6p2p_node_get_next_reference_msg (c_node, &progress);
      if (c_ret)
	{
	  ret = scm_from_locale_string (c_ret);
	  LW6SYS_FREE (c_ret);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6p2p_node_get_next_draft_msg (SCM node)
{
  lw6p2p_node_t *c_node;
  char *c_ret = NULL;
  SCM ret = SCM_BOOL_F;
  lw6sys_progress_t progress;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE
	      (lw6_global.smob_types.node,
	       node), node, SCM_ARG1, __FUNCTION__);

  lw6sys_progress_default (&progress, &(lw6_global.progress));
  // do *not* call lw6sys_progress_begin here!

  c_node = lw6_scm_to_node (node);
  if (c_node)
    {
      c_ret = lw6p2p_node_get_next_draft_msg (c_node, &progress);
      if (c_ret)
	{
	  ret = scm_from_locale_string (c_ret);
	  LW6SYS_FREE (c_ret);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/**
 * lw6_register_funcs_p2p
 *
 * Register the functions of the p2p module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_p2p ()
{
  int ret = 1;

  /*
   * In liquidwar6p2p
   */
  ret = ret && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_DB_NEW, 1, 0, 0,
				      (SCM (*)())_scm_lw6p2p_db_new);
  ret = ret && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_DB_RESET, 1, 0, 0,
				      (SCM (*)())_scm_lw6p2p_db_reset);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_DB_DEFAULT_NAME, 0, 0, 0,
			      (SCM (*)())_scm_lw6p2p_db_default_name);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_NODE_NEW, 2, 0, 0,
			      (SCM (*)())_scm_lw6p2p_node_new);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_NODE_POLL, 1, 0, 0,
			      (SCM (*)())_scm_lw6p2p_node_poll);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_NODE_CLOSE, 1, 0, 0,
			      (SCM (*)())_scm_lw6p2p_node_close);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_NODE_GET_ID, 1, 0, 0,
			      (SCM (*)())_scm_lw6p2p_node_get_id);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_NODE_GET_ENTRIES, 1, 0, 0,
			      (SCM (*)())_scm_lw6p2p_node_get_entries);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_NODE_SERVER_START, 2, 0, 0,
			      (SCM (*)())_scm_lw6p2p_node_server_start);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_NODE_CLIENT_JOIN, 3, 0, 0,
			      (SCM (*)())_scm_lw6p2p_node_client_join);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_NODE_DISCONNECT, 1, 0, 0,
			      (SCM (*)())_scm_lw6p2p_node_disconnect);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_NODE_UPDATE_INFO, 2, 0, 0,
			      (SCM (*)())_scm_lw6p2p_node_update_info);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_NODE_CALIBRATE, 3, 0, 0,
			      (SCM (*)())_scm_lw6p2p_node_calibrate);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_NODE_GET_SEQ_MAX, 1, 0, 0,
			      (SCM (*)())_scm_lw6p2p_node_get_seq_max);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_NODE_IS_SEED_NEEDED, 1, 0, 0,
			      (SCM (*)())_scm_lw6p2p_node_is_seed_needed);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_NODE_IS_DUMP_NEEDED, 1, 0, 0,
			      (SCM (*)())_scm_lw6p2p_node_is_dump_needed);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_NODE_PUT_LOCAL_MSG, 2, 0, 0,
			      (SCM (*)())_scm_lw6p2p_node_put_local_msg);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_NODE_GET_NEXT_REFERENCE_MSG, 1,
			      0, 0,
			      (SCM (*)
			       ())_scm_lw6p2p_node_get_next_reference_msg);
  ret = ret
    && lw6scm_c_define_gsubr (LW6DEF_C_LW6P2P_NODE_GET_NEXT_DRAFT_MSG, 1, 0,
			      0,
			      (SCM (*)())_scm_lw6p2p_node_get_next_draft_msg);

  return ret;
}
