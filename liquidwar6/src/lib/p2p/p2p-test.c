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

#include "p2p.h"
#include "p2p-internal.h"

#define _TEST_ARGC 1
#define _TEST_ARGV0 "prog"
#define _TEST_NET_LOG 1
#define _TEST_DB_NAME12 "p2p.test.12"
#define _TEST_DB_NAME34 "p2p.test.34"
#define _TEST_DB_NAME56 "p2p.test.56"

#define _TEST_ENTRY_CREATION_TIMESTAMP 123
#define _TEST_ENTRY_VERSION "0.0.0"
#define _TEST_ENTRY_CODENAME "zozo"
#define _TEST_ENTRY_STAMP 456
#define _TEST_ENTRY_ID "1234123412341234"
#define _TEST_ENTRY_URL "http://my.site.com:1234"
#define _TEST_ENTRY_TITLE "My Server"
#define _TEST_ENTRY_DESCRIPTION "This is my server"
#define _TEST_ENTRY_HAS_PASSWORD 0
#define _TEST_ENTRY_BENCH 10
#define _TEST_ENTRY_OPEN_RELAY 0
#define _TEST_ENTRY_COMMUNITY_ID "1234123412341234"
#define _TEST_ENTRY_ROUND 1000
#define _TEST_ENTRY_LEVEL "the-map"
#define _TEST_ENTRY_REQUIRED_BENCH 5
#define _TEST_ENTRY_NB_COLORS 0
#define _TEST_ENTRY_MAX_NB_COLORS 5
#define _TEST_ENTRY_NB_CURSORS 0
#define _TEST_ENTRY_MAX_NB_CURSORS 5
#define _TEST_ENTRY_NB_NODES 0
#define _TEST_ENTRY_MAX_NB_NODES 5
#define _TEST_ENTRY_IP "127.0.0.1"
#define _TEST_ENTRY_PORT 1234
#define _TEST_ENTRY_LAST_PING_TIMESTAMP 123456789
#define _TEST_ENTRY_PING_DELAY_MSEC 500
#define _TEST_ENTRY_AVAILABLE 1

#define _TEST_NODE_BIND_IP LW6NET_ADDRESS_LOOPBACK
#define _TEST_NODE_BIND_PORT1 (LW6NET_DEFAULT_PORT + 11)
#define _TEST_NODE_BIND_PORT2 (LW6NET_DEFAULT_PORT + 12)
#define _TEST_NODE_BIND_PORT3 (LW6NET_DEFAULT_PORT + 13)
#define _TEST_NODE_BIND_PORT4 (LW6NET_DEFAULT_PORT + 14)
#define _TEST_NODE_BIND_PORT5 (LW6NET_DEFAULT_PORT + 15)
#define _TEST_NODE_BIND_PORT6 (LW6NET_DEFAULT_PORT + 16)
#define _TEST_NODE_BROADCAST 1
/*
 * following depends on LW6NET_DEFAULT_PORT
 */
#define _TEST_NODE_PUBLIC_URL1 "http://localhost:8067/"
#define _TEST_NODE_PUBLIC_URL2 "http://localhost:8068/"
#define _TEST_NODE_PUBLIC_URL3 "http://localhost:8069/"
#define _TEST_NODE_PUBLIC_URL4 "http://localhost:8070/"
#define _TEST_NODE_PUBLIC_URL5 "http://localhost:8071/"
#define _TEST_NODE_PUBLIC_URL6 "http://localhost:8072/"
#define _TEST_NODE_IP1 NULL
#define _TEST_NODE_IP2 NULL
#define _TEST_NODE_IP3 LW6NET_ADDRESS_LOOPBACK
#define _TEST_NODE_IP4 "127.1.2.3"
#define _TEST_NODE_TITLE1 "Node 1"
#define _TEST_NODE_TITLE2 "Node 2"
#define _TEST_NODE_TITLE3 "Node 3"
#define _TEST_NODE_TITLE4 "Node 4"
#define _TEST_NODE_TITLE5 "Node 5"
#define _TEST_NODE_TITLE6 "Node 6"
#define _TEST_NODE_DESCRIPTION "This is the description of a node."
#define _TEST_NODE_PASSWORD "toto"
#define _TEST_NODE_BENCH 10
#define _TEST_NODE_OPEN_RELAY 0
#define _TEST_NODE_KNOWN_NODES1 "http://localhost:8068/"
#define _TEST_NODE_KNOWN_NODES2 "http://localhost:8069/"
#define _TEST_NODE_KNOWN_NODES3 "http://localhost:8070/"
#define _TEST_NODE_KNOWN_NODES4 "http://localhost:8068/"
#define _TEST_NODE_KNOWN_NODES5 "http://localhost:8067/,http://localhost:8068/"
#define _TEST_NODE_KNOWN_NODES6 "http://localhost:8067/,http://localhost:8068/"
#define _TEST_NODE_NETWORK_RELIABILITY 100
#define _TEST_NODE_TROJAN 0
#define _TEST_BOGUS_BACKEND "bogus"

#define TEST_NODE_OOB_DURATION 9000
#define TEST_NODE_CMD_DURATION 6000
#define _TEST_ENTRY_HAS_PASSWORD 0
#define _TEST_ENTRY_BENCH 10
#define _TEST_ENTRY_OPEN_RELAY 0
#define _TEST_ENTRY_COMMUNITY_ID "1234123412341234"
#define _TEST_ENTRY_ROUND 1000
#define _TEST_ENTRY_LEVEL "the-map"
#define _TEST_ENTRY_REQUIRED_BENCH 5
#define _TEST_ENTRY_NB_COLORS 0
#define _TEST_ENTRY_MAX_NB_COLORS 5
#define _TEST_ENTRY_NB_CURSORS 0
#define _TEST_ENTRY_MAX_NB_CURSORS 5
#define _TEST_ENTRY_NB_NODES 0
#define _TEST_ENTRY_MAX_NB_NODES 5
#define _TEST_ENTRY_IP "127.0.0.1"
#define _TEST_ENTRY_PORT 1234
#define _TEST_ENTRY_LAST_PING_TIMESTAMP 123456789
#define _TEST_ENTRY_PING_DELAY_MSEC 500
#define _TEST_ENTRY_AVAILABLE 1

#define _TEST_NODE_BIND_IP LW6NET_ADDRESS_LOOPBACK
#define _TEST_NODE_BIND_PORT1 (LW6NET_DEFAULT_PORT + 11)
#define _TEST_NODE_BIND_PORT2 (LW6NET_DEFAULT_PORT + 12)
#define _TEST_NODE_BIND_PORT3 (LW6NET_DEFAULT_PORT + 13)
#define _TEST_NODE_BIND_PORT4 (LW6NET_DEFAULT_PORT + 14)
#define _TEST_NODE_BIND_PORT5 (LW6NET_DEFAULT_PORT + 15)
#define _TEST_NODE_BIND_PORT6 (LW6NET_DEFAULT_PORT + 16)
#define _TEST_NODE_BROADCAST 1
/*
 * following depends on LW6NET_DEFAULT_PORT
 */
#define _TEST_NODE_PUBLIC_URL1 "http://localhost:8067/"
#define _TEST_NODE_PUBLIC_URL2 "http://localhost:8068/"
#define _TEST_NODE_PUBLIC_URL3 "http://localhost:8069/"
#define _TEST_NODE_PUBLIC_URL4 "http://localhost:8070/"
#define _TEST_NODE_PUBLIC_URL5 "http://localhost:8071/"
#define _TEST_NODE_PUBLIC_URL6 "http://localhost:8072/"
#define _TEST_NODE_IP1 NULL
#define _TEST_NODE_IP2 NULL
#define _TEST_NODE_IP3 LW6NET_ADDRESS_LOOPBACK
#define _TEST_NODE_IP4 "127.1.2.3"
#define _TEST_NODE_TITLE1 "Node 1"
#define _TEST_NODE_TITLE2 "Node 2"
#define _TEST_NODE_TITLE3 "Node 3"
#define _TEST_NODE_TITLE4 "Node 4"
#define _TEST_NODE_TITLE5 "Node 5"
#define _TEST_NODE_TITLE6 "Node 6"
#define _TEST_NODE_DESCRIPTION "This is the description of a node."
#define _TEST_NODE_PASSWORD "toto"
#define _TEST_NODE_BENCH 10
#define _TEST_NODE_OPEN_RELAY 0
#define _TEST_NODE_KNOWN_NODES1 "http://localhost:8068/"
#define _TEST_NODE_KNOWN_NODES2 "http://localhost:8069/"
#define _TEST_NODE_KNOWN_NODES3 "http://localhost:8070/"
#define _TEST_NODE_KNOWN_NODES4 "http://localhost:8068/"
#define _TEST_NODE_KNOWN_NODES5 "http://localhost:8067/,http://localhost:8068/"
#define _TEST_NODE_KNOWN_NODES6 "http://localhost:8067/,http://localhost:8068/"
#define _TEST_NODE_NETWORK_RELIABILITY 100
#define _TEST_NODE_TROJAN 0
#define _TEST_BOGUS_BACKEND "bogus"

#define TEST_NODE_OOB_DURATION 9000
#define TEST_NODE_CMD_DURATION 6000
#define TEST_NODE_API_DURATION 3000
#define TEST_NODE_POLL_DURATION 100

/* 
 * Testing db
 */
static int
_test_db ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    const int argc = _TEST_ARGC;
    const char *argv[] = { _TEST_ARGV0 };
    lw6p2p_db_t *db = NULL;
    char *repr = NULL;

    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_
		("default database name is \"%s\" but we use \"%s\" for some tests"),
		lw6p2p_db_default_name (), _TEST_DB_NAME12);

    db = lw6p2p_db_open (argc, argv, _TEST_DB_NAME12);
    if (db)
      {
	repr = lw6p2p_db_repr (db);
	if (repr)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("created db \"%s\""), repr);
	    LW6SYS_FREE (repr);
	  }
	lw6p2p_db_close (db);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("can't create db"));
	ret = 0;
      }

    if (lw6p2p_db_reset (argc, argv, _TEST_DB_NAME12))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("deleted db"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("can't delete db"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/* 
 * Testing entry
 */
static int
_test_entry ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6p2p_entry_t *entry;
    char *repr;

    entry = lw6p2p_entry_new (_TEST_ENTRY_CREATION_TIMESTAMP,
			      _TEST_ENTRY_VERSION,
			      _TEST_ENTRY_CODENAME,
			      _TEST_ENTRY_STAMP,
			      _TEST_ENTRY_ID,
			      _TEST_ENTRY_URL,
			      _TEST_ENTRY_TITLE,
			      _TEST_ENTRY_DESCRIPTION,
			      _TEST_ENTRY_HAS_PASSWORD,
			      _TEST_ENTRY_BENCH,
			      _TEST_ENTRY_OPEN_RELAY,
			      _TEST_ENTRY_COMMUNITY_ID,
			      _TEST_ENTRY_ROUND,
			      _TEST_ENTRY_LEVEL,
			      _TEST_ENTRY_REQUIRED_BENCH,
			      _TEST_ENTRY_NB_COLORS,
			      _TEST_ENTRY_MAX_NB_COLORS,
			      _TEST_ENTRY_NB_CURSORS,
			      _TEST_ENTRY_MAX_NB_CURSORS,
			      _TEST_ENTRY_NB_NODES,
			      _TEST_ENTRY_MAX_NB_NODES,
			      _TEST_ENTRY_IP,
			      _TEST_ENTRY_PORT,
			      _TEST_ENTRY_LAST_PING_TIMESTAMP,
			      _TEST_ENTRY_PING_DELAY_MSEC,
			      _TEST_ENTRY_AVAILABLE);
    if (entry)
      {
	repr = lw6p2p_entry_repr (entry);
	if (repr)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("entry created: \"%s\""),
			repr);
	    LW6SYS_FREE (repr);
	  }
	lw6p2p_entry_free (entry);
	entry = NULL;
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("unable to create entry"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/* 
 * Testing node initialisation
 */
static int
_test_node_init ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    const int argc = _TEST_ARGC;
    const char *argv[] = { _TEST_ARGV0 };
    lw6p2p_db_t *db = NULL;
    lw6p2p_node_t *node = NULL;
    char *id_str;
    char *repr = NULL;

    db = lw6p2p_db_open (argc, argv, _TEST_DB_NAME12);
    if (db)
      {
	node =
	  lw6p2p_node_new (argc, argv, db, lw6cli_default_backends (),
			   lw6srv_default_backends (), _TEST_NODE_BIND_IP,
			   _TEST_NODE_BIND_PORT1, _TEST_NODE_BROADCAST,
			   _TEST_NODE_PUBLIC_URL1,
			   _TEST_NODE_TITLE1, _TEST_NODE_DESCRIPTION, NULL,
			   _TEST_NODE_BENCH, _TEST_NODE_OPEN_RELAY,
			   _TEST_NODE_KNOWN_NODES1,
			   _TEST_NODE_NETWORK_RELIABILITY, _TEST_NODE_TROJAN);
	if (node)
	  {
	    repr = lw6p2p_node_repr (node);
	    if (repr)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("created node \"%s\""),
			    repr);
		LW6SYS_FREE (repr);
	      }
	    id_str = lw6sys_id_ltoa (lw6p2p_node_get_id (node));
	    if (id_str)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("get_id returns %s"),
			    id_str);
		LW6SYS_FREE (id_str);
	      }
	    lw6p2p_node_close (node);
	    lw6p2p_node_close (node);	// yes, do it twice just to check
	    lw6p2p_node_free (node);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("can't create node"));
	    ret = 0;
	  }
	lw6p2p_db_close (db);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("can't create db"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/* 
 * Initializes up to 6 nodes
 */
static int
_init_nodes (char *cli_backends, char *srv_backends, lw6p2p_db_t ** db12,
	     lw6p2p_db_t ** db34, lw6p2p_db_t ** db56, lw6p2p_node_t ** node1,
	     lw6p2p_node_t ** node2, lw6p2p_node_t ** node3,
	     lw6p2p_node_t ** node4, lw6p2p_node_t ** node5,
	     lw6p2p_node_t ** node6)
{
  const int argc = _TEST_ARGC;
  const char *argv[] = { _TEST_ARGV0 };
  char *repr = NULL;
  int ret = 0;

  if (db12)
    {
      lw6p2p_db_reset (argc, argv, _TEST_DB_NAME12);
      (*db12) = lw6p2p_db_open (argc, argv, _TEST_DB_NAME12);
      if (*db12)
	{
	  repr = lw6p2p_db_repr (*db12);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("created db \"%s\""), repr);
	      LW6SYS_FREE (repr);
	    }
	}
    }
  if (db34)
    {
      lw6p2p_db_reset (argc, argv, _TEST_DB_NAME34);
      (*db34) = lw6p2p_db_open (argc, argv, _TEST_DB_NAME34);
      if (*db34)
	{
	  repr = lw6p2p_db_repr (*db34);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("created db \"%s\""), repr);
	      LW6SYS_FREE (repr);
	    }
	}
    }
  if (db56)
    {
      lw6p2p_db_reset (argc, argv, _TEST_DB_NAME56);
      (*db56) = lw6p2p_db_open (argc, argv, _TEST_DB_NAME56);
      if (*db56)
	{
	  repr = lw6p2p_db_repr (*db56);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("created db \"%s\""), repr);
	      LW6SYS_FREE (repr);
	    }
	}
    }
  if (db12 && (*db12))
    {
      if (node1)
	{
	  (*node1) =
	    lw6p2p_node_new (argc, argv, *db12, cli_backends,
			     srv_backends, _TEST_NODE_BIND_IP,
			     _TEST_NODE_BIND_PORT1, _TEST_NODE_BROADCAST,
			     _TEST_NODE_PUBLIC_URL1,
			     _TEST_NODE_TITLE1,
			     _TEST_NODE_DESCRIPTION, _TEST_NODE_PASSWORD,
			     _TEST_NODE_BENCH, _TEST_NODE_OPEN_RELAY,
			     _TEST_NODE_KNOWN_NODES1,
			     _TEST_NODE_NETWORK_RELIABILITY,
			     _TEST_NODE_TROJAN);
	  if (*node1)
	    {
	      repr = lw6p2p_node_repr (*node1);
	      if (repr)
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("created node1 \"%s\""),
			      repr);
		  LW6SYS_FREE (repr);
		}
	    }
	}

      if (node2)
	{
	  (*node2) =
	    lw6p2p_node_new (argc, argv, *db12, cli_backends,
			     srv_backends, _TEST_NODE_BIND_IP,
			     _TEST_NODE_BIND_PORT2, _TEST_NODE_BROADCAST,
			     _TEST_NODE_PUBLIC_URL2,
			     _TEST_NODE_TITLE2, _TEST_NODE_DESCRIPTION, NULL,
			     _TEST_NODE_BENCH, _TEST_NODE_OPEN_RELAY,
			     _TEST_NODE_KNOWN_NODES2,
			     _TEST_NODE_NETWORK_RELIABILITY,
			     _TEST_NODE_TROJAN);
	  if (*node2)
	    {
	      repr = lw6p2p_node_repr (*node2);
	      if (repr)
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("created node2 \"%s\""),
			      repr);
		  LW6SYS_FREE (repr);
		}
	    }
	}
    }
  if (db34 && (*db34))
    {
      if (node3)
	{
	  (*node3) =
	    lw6p2p_node_new (argc, argv, *db34, cli_backends,
			     srv_backends, _TEST_NODE_BIND_IP,
			     _TEST_NODE_BIND_PORT3, _TEST_NODE_BROADCAST,
			     _TEST_NODE_PUBLIC_URL3,
			     _TEST_NODE_TITLE3,
			     _TEST_NODE_DESCRIPTION, _TEST_NODE_PASSWORD,
			     _TEST_NODE_BENCH, _TEST_NODE_OPEN_RELAY,
			     _TEST_NODE_KNOWN_NODES3,
			     _TEST_NODE_NETWORK_RELIABILITY,
			     _TEST_NODE_TROJAN);
	  if (*node3)
	    {
	      repr = lw6p2p_node_repr (*node3);
	      if (repr)
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("created node3 \"%s\""),
			      repr);
		  LW6SYS_FREE (repr);
		}
	    }
	}

      if (node4)
	{
	  (*node4) =
	    lw6p2p_node_new (argc, argv, *db34, cli_backends,
			     srv_backends, _TEST_NODE_BIND_IP,
			     _TEST_NODE_BIND_PORT4, _TEST_NODE_BROADCAST,
			     _TEST_NODE_PUBLIC_URL4,
			     _TEST_NODE_TITLE4, _TEST_NODE_DESCRIPTION, NULL,
			     _TEST_NODE_BENCH, _TEST_NODE_OPEN_RELAY,
			     _TEST_NODE_KNOWN_NODES4,
			     _TEST_NODE_NETWORK_RELIABILITY,
			     _TEST_NODE_TROJAN);
	  if (*node4)
	    {
	      repr = lw6p2p_node_repr (*node4);
	      if (repr)
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("created node4 \"%s\""),
			      repr);
		  LW6SYS_FREE (repr);
		}
	    }
	}
    }

  if (db56 && (*db56))
    {
      if (node5)
	{
	  (*node5) =
	    lw6p2p_node_new (argc, argv, *db56, cli_backends,
			     srv_backends, _TEST_NODE_BIND_IP,
			     _TEST_NODE_BIND_PORT5, _TEST_NODE_BROADCAST,
			     _TEST_NODE_PUBLIC_URL5,
			     _TEST_NODE_TITLE5,
			     _TEST_NODE_DESCRIPTION, NULL,
			     _TEST_NODE_BENCH, _TEST_NODE_OPEN_RELAY,
			     _TEST_NODE_KNOWN_NODES5,
			     _TEST_NODE_NETWORK_RELIABILITY,
			     _TEST_NODE_TROJAN);
	  if (*node5)
	    {
	      repr = lw6p2p_node_repr (*node5);
	      if (repr)
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("created node5 \"%s\""),
			      repr);
		  LW6SYS_FREE (repr);
		}
	    }
	}

      if (node6)
	{
	  (*node6) =
	    lw6p2p_node_new (argc, argv, *db56, cli_backends,
			     srv_backends, _TEST_NODE_BIND_IP,
			     _TEST_NODE_BIND_PORT6, _TEST_NODE_BROADCAST,
			     _TEST_NODE_PUBLIC_URL6,
			     _TEST_NODE_TITLE6, _TEST_NODE_DESCRIPTION, NULL,
			     _TEST_NODE_BENCH, _TEST_NODE_OPEN_RELAY,
			     _TEST_NODE_KNOWN_NODES6,
			     _TEST_NODE_NETWORK_RELIABILITY,
			     _TEST_NODE_TROJAN);
	  if (*node6)
	    {
	      repr = lw6p2p_node_repr (*node6);
	      if (repr)
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("created node6 \"%s\""),
			      repr);
		  LW6SYS_FREE (repr);
		}
	    }
	}
    }

  if (((!node1) || (node1 && *node1))
      && ((!node2) || (node2 && *node2)) && ((!node3) || (node3 && *node3))
      && ((!node4) || (node4 && *node4)) && ((!node5) || (node5 && *node5))
      && ((!node6) || (node6 && *node6)))
    {
      ret = 1;
    }
  else
    {
      if (node1 && *node1)
	{
	  lw6p2p_node_free (*node1);
	  (*node1) = NULL;
	}
      if (node2 && *node2)
	{
	  lw6p2p_node_free (*node2);
	  (*node2) = NULL;
	}
      if (node3 && *node3)
	{
	  lw6p2p_node_free (*node3);
	  (*node3) = NULL;
	}
      if (node4 && *node4)
	{
	  lw6p2p_node_free (*node4);
	  (*node4) = NULL;
	}
      if (node5 && *node5)
	{
	  lw6p2p_node_free (*node4);
	  (*node4) = NULL;
	}
      if (node6 && *node6)
	{
	  lw6p2p_node_free (*node6);
	  (*node6) = NULL;
	}
      if (db12 && *db12)
	{
	  lw6p2p_db_close (*db12);
	  (*db12) = NULL;
	}
      if (db34 && *db34)
	{
	  lw6p2p_db_close (*db34);
	  (*db34) = NULL;
	}
      if (db56 && *db56)
	{
	  lw6p2p_db_close (*db56);
	  (*db56) = NULL;
	}
    }

  return ret;
}

static void
_quit_nodes (lw6p2p_db_t * db12, lw6p2p_db_t * db34, lw6p2p_db_t * db56,
	     lw6p2p_node_t * node1, lw6p2p_node_t * node2,
	     lw6p2p_node_t * node3, lw6p2p_node_t * node4,
	     lw6p2p_node_t * node5, lw6p2p_node_t * node6)
{
  if (node1)
    {
      lw6p2p_node_close (node1);
      lw6p2p_node_free (node1);
    }
  if (node2)
    {
      lw6p2p_node_close (node2);
      lw6p2p_node_free (node2);
    }
  if (node3)
    {
      lw6p2p_node_close (node3);
      lw6p2p_node_free (node3);
    }
  if (node4)
    {
      lw6p2p_node_close (node4);
      lw6p2p_node_free (node4);
    }
  if (node5)
    {
      lw6p2p_node_close (node5);
      lw6p2p_node_free (node5);
    }
  if (node6)
    {
      lw6p2p_node_close (node6);
      lw6p2p_node_free (node6);
    }
  if (db12)
    {
      lw6p2p_db_close (db12);
    }
  if (db34)
    {
      lw6p2p_db_close (db34);
    }
  if (db56)
    {
      lw6p2p_db_close (db56);
    }
}

static void
_poll_nodes (lw6p2p_node_t * node1, lw6p2p_node_t * node2,
	     lw6p2p_node_t * node3, lw6p2p_node_t * node4,
	     lw6p2p_node_t * node5, lw6p2p_node_t * node6)
{
  int64_t next_timestamp = 00L;

  next_timestamp = lw6sys_get_timestamp () + TEST_NODE_POLL_DURATION;
  while (lw6sys_get_timestamp () < next_timestamp)
    {
      if (node1)
	{
	  lw6p2p_node_poll (node1);
	}
      if (node2)
	{
	  lw6p2p_node_poll (node2);
	}
      if (node3)
	{
	  lw6p2p_node_poll (node3);
	}
      if (node4)
	{
	  lw6p2p_node_poll (node4);
	}
      if (node5)
	{
	  lw6p2p_node_poll (node5);
	}
      if (node6)
	{
	  lw6p2p_node_poll (node6);
	}
    }
  lw6sys_idle ();
}

static void
_print_entry_callback (void *func_data, void *data)
{
  _lw6p2p_node_t *node = (_lw6p2p_node_t *) func_data;
  lw6p2p_entry_t *entry = (lw6p2p_entry_t *) data;
  char *repr = NULL;

  repr = lw6p2p_entry_repr (entry);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("node \"%s\" knows entry \"%s\""),
		  node->node_info->const_info.title, repr);
      LW6SYS_FREE (repr);
    }
}

/* 
 * Testing node connection
 */
static int
_cmd_with_backends (char *cli_backends, char *srv_backends)
{
  int ret = 1;
  lw6p2p_db_t *db12 = NULL;
  lw6p2p_db_t *db34 = NULL;
  lw6p2p_db_t *db56 = NULL;
  lw6p2p_node_t *node1 = NULL;
  lw6p2p_node_t *node2 = NULL;
  lw6p2p_node_t *node3 = NULL;
  lw6p2p_node_t *node4 = NULL;
  lw6p2p_node_t *node5 = NULL;
  lw6p2p_node_t *node6 = NULL;
  int64_t end_timestamp = 0;
  lw6sys_list_t *entries = NULL;

  lw6sys_log (LW6SYS_LOG_NOTICE,
	      _x_ ("testing cmd with backends \"%s\" and \"%s\""),
	      cli_backends, srv_backends);

  ret = 0;
  end_timestamp = lw6sys_get_timestamp () + TEST_NODE_CMD_DURATION;
  if (_init_nodes
      (cli_backends, srv_backends, &db12, &db34, &db56, &node1, &node2,
       &node3, &node4, &node5, &node6))
    {
      if (_lw6p2p_node_register_tentacle
	  ((_lw6p2p_node_t *) node1, _TEST_NODE_PUBLIC_URL2, _TEST_NODE_IP2,
	   lw6p2p_node_get_id (node2))
	  && _lw6p2p_node_register_tentacle ((_lw6p2p_node_t *) node2,
					     _TEST_NODE_PUBLIC_URL1,
					     _TEST_NODE_IP1,
					     lw6p2p_node_get_id (node1))
	  && _lw6p2p_node_register_tentacle ((_lw6p2p_node_t *) node1,
					     _TEST_NODE_PUBLIC_URL3,
					     _TEST_NODE_IP3,
					     lw6p2p_node_get_id (node3))
	  && _lw6p2p_node_register_tentacle ((_lw6p2p_node_t *) node3,
					     _TEST_NODE_PUBLIC_URL1,
					     _TEST_NODE_IP1,
					     lw6p2p_node_get_id (node1))
	  && _lw6p2p_node_register_tentacle ((_lw6p2p_node_t *) node1,
					     _TEST_NODE_PUBLIC_URL4,
					     _TEST_NODE_IP4,
					     lw6p2p_node_get_id (node4))
	  && _lw6p2p_node_register_tentacle ((_lw6p2p_node_t *) node4,
					     _TEST_NODE_PUBLIC_URL1,
					     _TEST_NODE_IP1,
					     lw6p2p_node_get_id (node1))
	  && _lw6p2p_node_register_tentacle ((_lw6p2p_node_t *) node4,
					     _TEST_NODE_PUBLIC_URL2,
					     _TEST_NODE_IP2,
					     lw6p2p_node_get_id (node2))
	  && _lw6p2p_node_register_tentacle ((_lw6p2p_node_t *) node6,
					     _TEST_NODE_PUBLIC_URL2,
					     _TEST_NODE_IP2,
					     lw6p2p_node_get_id (node2))
	  && _lw6p2p_node_register_tentacle ((_lw6p2p_node_t *) node5,
					     _TEST_NODE_PUBLIC_URL1,
					     _TEST_NODE_IP1,
					     lw6p2p_node_get_id (node1)))
	{
	  while (lw6sys_get_timestamp () < end_timestamp)
	    {
	      _poll_nodes (node1, node2, node3, node4, node5, node6);
	    }

	  entries = lw6p2p_node_get_entries (node1);
	  if (entries)
	    {
	      lw6sys_list_map (entries, _print_entry_callback,
			       (void *) node1);
	      lw6sys_list_free (entries);
	    }

	  ret = 1;
	}
      _quit_nodes (db12, db34, db56, node1, node2, node3, node4, node5,
		   node6);
    }

  return ret;
}

/* 
 * Testing node connection
 */
static int
_test_node_cmd ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    ret = ret
      && _cmd_with_backends (lw6cli_default_backends (),
			     lw6srv_default_backends ());
    ret = ret && _cmd_with_backends ("udp", "udpd");
    ret = ret && _cmd_with_backends ("tcp", "tcpd");
    _cmd_with_backends ("http", "httpd");	// even if fails, keep going since http is optional
    _cmd_with_backends ("", _TEST_BOGUS_BACKEND);
    _cmd_with_backends (_TEST_BOGUS_BACKEND, "");
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
_oob_with_backends (char *cli_backends, char *srv_backends)
{
  int ret = 1;
  lw6p2p_db_t *db12 = NULL;
  lw6p2p_db_t *db34 = NULL;
  lw6p2p_db_t *db56 = NULL;
  lw6p2p_node_t *node1 = NULL;
  lw6p2p_node_t *node2 = NULL;
  lw6p2p_node_t *node3 = NULL;
  lw6p2p_node_t *node4 = NULL;
  lw6p2p_node_t *node5 = NULL;
  lw6p2p_node_t *node6 = NULL;
  int64_t end_timestamp = 0LL;

  lw6sys_log (LW6SYS_LOG_NOTICE,
	      _x_ ("testing oob with backends \"%s\" and \"%s\""),
	      cli_backends, srv_backends);

  ret = 0;
  end_timestamp = lw6sys_get_timestamp () + TEST_NODE_OOB_DURATION;
  if (_init_nodes
      (cli_backends, srv_backends, &db12, &db34, &db56, &node1, &node2,
       &node3, &node4, &node5, &node6))
    {
      while (lw6sys_get_timestamp () < end_timestamp)
	{
	  _poll_nodes (node1, node2, node3, node4, node5, node6);
	}

      ret = 1;

      _quit_nodes (db12, db34, db56, node1, node2, node3, node4, node5,
		   node6);
    }

  return ret;
}

/* 
 * Testing node connection
 */
static int
_test_node_oob ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    ret = ret
      && _oob_with_backends (lw6cli_default_backends (),
			     lw6srv_default_backends ());
    ret = ret && _oob_with_backends ("", lw6srv_default_backends ());
    ret = ret && _oob_with_backends (lw6cli_default_backends (), "");
    _oob_with_backends ("", _TEST_BOGUS_BACKEND);
    _oob_with_backends (_TEST_BOGUS_BACKEND, "");
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/* 
 * Testing node api
 */
static int
_test_node_api ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6p2p_db_t *db12 = NULL;
    lw6p2p_db_t *db34 = NULL;
    lw6p2p_db_t *db56 = NULL;
    lw6p2p_node_t *node1 = NULL;
    lw6p2p_node_t *node2 = NULL;
    lw6p2p_node_t *node3 = NULL;
    lw6p2p_node_t *node4 = NULL;
    lw6p2p_node_t *node5 = NULL;
    lw6p2p_node_t *node6 = NULL;
    int64_t end_timestamp = 0LL;
    _lw6p2p_node_t *_node = NULL;

    end_timestamp = lw6sys_get_timestamp () + TEST_NODE_API_DURATION;
    if (_init_nodes
	(lw6cli_default_backends (),
	 lw6srv_default_backends (), &db12, &db34, &db56, &node1, &node2,
	 &node3, &node4, &node5, &node6))
      {
	_poll_nodes (node1, node2, node3, node4, node5, node6);
	if (lw6p2p_node_server_start (node1))
	  {
	    _poll_nodes (node1, node2, node3, node4, node5, node6);
	    _node = (_lw6p2p_node_t *) node1;
	    if (lw6p2p_node_client_join
		(node2, _node->node_info->const_info.ref_info.id_int,
		 _node->node_info->const_info.ref_info.url))
	      {
		while (lw6sys_get_timestamp () < end_timestamp)
		  {
		    _poll_nodes (node1, node2, node3, node4, node5, node6);
		  }
		lw6p2p_node_disconnect (node2);
	      }
	    lw6p2p_node_disconnect (node1);
	  }

	_quit_nodes (db12, db34, db56, node1, node2, node3, node4, node5,
		     node6);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6p2p_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @p2p module test suite. This test can fail if one
 * cannot bind on some network port, in a general manner it is
 * dependent on the network environment, so it's better if there's
 * some sort of human control on it.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6p2p_test (int mode)
{
  int ret = 0;
  const int argc = _TEST_ARGC;
  const char *argv[] = { _TEST_ARGV0 };

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6glb_test (mode);
      lw6cfg_test (mode);
      lw6net_test (mode);
      lw6nod_test (mode);
      lw6cnx_test (mode);
      lw6msg_test (mode);
      lw6cli_test (mode);
      lw6srv_test (mode);
      lw6dat_test (mode);
    }

  if (lw6net_init (argc, argv, _TEST_NET_LOG))
    {
      //ret = _test_db () && _test_entry () && _test_node_init ()
      //        && _test_node_oob () && _test_node_cmd () && _test_node_api ();
      ret = _test_node_api ();

      lw6net_quit (argc, argv);
    }

  return ret;
}
