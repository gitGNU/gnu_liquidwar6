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

/**
 * lw6p2p_entry_new
 *
 * @sys_context: global system context
 * @creation_timestamp: when it has been created, UNIX timestamp
 * @version: version of the node
 * @codename: codename of the node
 * @stamp: stamp of the node
 * @id: id of the node (string representation)
 * @url: public url of the node
 * @title: title of the node
 * @description: description of the node
 * @has_password: wether node is password protected or not
 * @bench: node bench
 * @open_relay: wether the node is in open relay mode or not
 * @communitity_id: id of the community it belongs to (string representation)
 * @round: current round
 * @level: current level played
 * @required_bench: current bench
 * @nb_colors: number of colors playing
 * @max_nb_colors: maximum number of colors
 * @nb_cursors: number of cursors playing
 * @max_nb_cursors: maximum number of cursors
 * @nb_nodes: number of nodes playing
 * @max_nb_nodes: maximum number of nodes
 * @ip: node ip (string representation)
 * @port: node port
 * @last_ping_timestamp: UNIX timestamp of last contact with node
 * @ping_delay_msec: ping delay, in milliseconds
 * @available: wether node is available, wether we can connect to it
 *
 * Creates a new p2p entry. Will accept NULL parameters for strings
 * as well as arbitrary long strings, will simply cut them short if
 * there aren't already limited to max size.
 *
 * Return value: newly allocated object
 */
lw6p2p_entry_t *
lw6p2p_entry_new (lw6sys_context_t * sys_context, int creation_timestamp, char *version, char *codename,
		  int stamp, char *id, char *url, char *title,
		  char *description, int has_password, int bench,
		  int open_relay, char *community_id, int round, char *level,
		  int required_bench, int nb_colors, int max_nb_colors,
		  int nb_cursors, int max_nb_cursors, int nb_nodes,
		  int max_nb_nodes, char *ip, int port, int last_ping_timestamp, int ping_delay_msec, int available)
{
  lw6p2p_entry_t *entry = NULL;

  entry = (lw6p2p_entry_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6p2p_entry_t));
  if (entry)
    {
      entry->creation_timestamp = creation_timestamp;
      if (version)
	{
	  strncpy (entry->version, version, LW6P2P_VERSION_SIZE);
	}
      if (codename)
	{
	  strncpy (entry->codename, codename, LW6P2P_CODENAME_SIZE);
	}
      entry->stamp = stamp;
      if (id)
	{
	  strncpy (entry->id, id, LW6P2P_ID_SIZE);
	}
      if (url)
	{
	  strncpy (entry->url, url, LW6P2P_URL_SIZE);
	}
      if (title)
	{
	  strncpy (entry->title, title, LW6P2P_TITLE_SIZE);
	}
      if (description)
	{
	  strncpy (entry->description, description, LW6P2P_DESCRIPTION_SIZE);
	}
      entry->has_password = has_password;
      entry->bench = bench;
      entry->open_relay = open_relay;
      // variable data
      if (community_id)
	{
	  strncpy (entry->community_id, community_id, LW6P2P_COMMUNITY_ID_SIZE);
	}
      entry->round = round;
      if (level)
	{
	  strncpy (entry->level, level, LW6P2P_LEVEL_SIZE);
	}
      entry->required_bench = required_bench;
      entry->nb_colors = nb_colors;
      entry->max_nb_colors = max_nb_colors;
      entry->nb_cursors = nb_cursors;
      entry->max_nb_cursors = max_nb_cursors;
      entry->nb_nodes = nb_nodes;
      entry->max_nb_nodes = max_nb_nodes;
      // additionnal data
      if (ip)
	{
	  strncpy (entry->ip, ip, LW6P2P_IP_SIZE);
	}
      entry->port = port;
      entry->last_ping_timestamp = last_ping_timestamp;
      entry->ping_delay_msec = ping_delay_msec;
    }

  return entry;
}

/**
 * lw6p2p_entry_free
 *
 * @sys_context: global system context
 * @entry: entry to free
 *
 * Frees a p2p entry.
 *
 * Return value: none.
 */
void
lw6p2p_entry_free (lw6sys_context_t * sys_context, lw6p2p_entry_t * entry)
{
  LW6SYS_FREE (sys_context, entry);
}

/**
 * lw6p2p_entry_repr
 *
 * @sys_context: global system context
 * @entry: entry to represent
 *
 * Gives a human-readable representation of the entry
 *
 * Return value: dynamically allocated string
 */
char *
lw6p2p_entry_repr (lw6sys_context_t * sys_context, const lw6p2p_entry_t * entry)
{
  char *repr = NULL;

  repr = lw6sys_new_sprintf (sys_context, "%s url=%s (%s:%d) available=%d", entry->title, entry->url, entry->ip, entry->port, entry->available);

  return repr;
}
