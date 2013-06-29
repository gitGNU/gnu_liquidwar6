/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013  Christian Mauduit <ufoot@ufoot.org>

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

#include <CUnit/CUnit.h>

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
/*
 * We use a low network reliability to be sure (really) sure program behaves
 * like it's hell on earth and almost no packets make it home. Values between
 * 10 and 100 are likely to give good results.
 */
#define _TEST_NODE_NETWORK_RELIABILITY 10
#define _TEST_NODE_TROJAN 0
#define _TEST_BOGUS_BACKEND "bogus"

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
#define _TEST_NODE_TROJAN 0
#define _TEST_BOGUS_BACKEND "bogus"

#define _TEST_NODE_ACK_DELAY_MSEC 7500
#define _TEST_NODE_OOB_DURATION 9000
#define _TEST_NODE_CMD_DURATION (9000+_TEST_NODE_ACK_DELAY_MSEC)
#define _TEST_NODE_API_DURATION_JOIN 500
/*
 * This is a maximum time, a high limit of 10 minutes to
 * avoid stalled process, but test should end much before
 */
#define _TEST_NODE_API_DURATION_THREAD (600000+_TEST_NODE_ACK_DELAY_MSEC)
#define _TEST_NODE_API_DURATION_END 3000
#define _TEST_NODE_POLL_DURATION 100
// 10 times bigger than _LW6PIL_MIN_SEQ_0
#define _TEST_NODE_API_SEQ_0 100000000000LL
#define _TEST_NODE_API_DUMP_SIZE 1000000
#define _TEST_NODE_API_MSG1_STR "message number one"
#define _TEST_NODE_API_MSG2_STR "2"
#define _TEST_NODE_API_MSG3_STR "3 fire!"
#define _TEST_NODE_API_MSGN_STR "All work and no play makes Jack a dull boy"
#define _TEST_NODE_API_MSGN_DELAY_MSEC 2000
#define _TEST_NODE_API_FINAL_SIZE 10000
#define _TEST_NODE_API_NODE4_REFERENCE_RECEIVED 0
#define _TEST_NODE_API_NODE4_LONG_DRAFT_RECEIVED 2
#define _TEST_NODE_API_NODE4_SHORT_DRAFT_RECEIVED 3
#define _TEST_NODE_API_LEN_LIMIT 100
#define _TEST_NODE_MSG_SEQ_0 100000000000LL
#define _TEST_NODE_MSG_NB_SEQS 97
// _TEST_NODE_MSG_NB_PER_SEQ must *not* be 10 or 100 or 1000
#define _TEST_NODE_MSG_NB_PER_SEQ 7
#define _TEST_NODE_MSG_RANDOM_STR_SIZE 10000

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
    int64_t local_seq_0 = 0LL;
    int64_t local_seq_last = 0LL;
    int64_t seq_min = 0LL;
    int64_t seq_max = 0LL;
    int64_t seq_draft = 0LL;
    int64_t seq_reference = 0LL;

    db = lw6p2p_db_open (argc, argv, _TEST_DB_NAME12);
    if (db)
      {
	/*
	 * Passing 0LL as a node id will cause automatic generation of an id
	 */
	node =
	  lw6p2p_node_new (argc, argv, db, lw6cli_default_backends (),
			   lw6srv_default_backends (), _TEST_NODE_BIND_IP,
			   _TEST_NODE_BIND_PORT1, _TEST_NODE_BROADCAST,
			   0LL,
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
	    local_seq_0 = lw6p2p_node_get_local_seq_0 (node);
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("local_seq_0=%" LW6SYS_PRINTF_LL "d"),
			(long long) local_seq_0);
	    local_seq_last = lw6p2p_node_get_local_seq_last (node);
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("local_seq_last=%" LW6SYS_PRINTF_LL "d"),
			(long long) local_seq_last);
	    seq_min = lw6p2p_node_get_seq_min (node);
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("seq_min=%" LW6SYS_PRINTF_LL "d"),
			(long long) seq_min);
	    seq_max = lw6p2p_node_get_seq_max (node);
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("seq_max=%" LW6SYS_PRINTF_LL "d"),
			(long long) seq_max);
	    seq_draft = lw6p2p_node_get_seq_draft (node);
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("seq_draft=%" LW6SYS_PRINTF_LL "d"),
			(long long) seq_draft);
	    seq_reference = lw6p2p_node_get_seq_reference (node);
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("seq_reference=%" LW6SYS_PRINTF_LL "d"),
			(long long) seq_reference);
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
      /*
       * It's important to reset test DBs before using them,
       * else there could be temporaries IDs mismatches.
       * In real use cases, only our local ID changes,
       * and we really do want something with same URL but
       * different ID than before to somewhat "not connect very well",
       * at least for some time.
       */
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
      /*
       * It's important to reset test DBs before using them,
       * else there could be temporaries IDs mismatches.
       * In real use cases, only our local ID changes,
       * and we really do want something with same URL but
       * different ID than before to somewhat "not connect very well",
       * at least for some time.
       */
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
      /*
       * It's important to reset test DBs before using them,
       * else there could be temporaries IDs mismatches.
       * In real use cases, only our local ID changes,
       * and we really do want something with same URL but
       * different ID than before to somewhat "not connect very well",
       * at least for some time.
       */
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
			     lw6sys_generate_id_64 (),
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
			     lw6sys_generate_id_64 (),
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
			     lw6sys_generate_id_64 (),
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
			     lw6sys_generate_id_64 (),
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
			     lw6sys_generate_id_64 (),
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
			     lw6sys_generate_id_64 (),
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

  next_timestamp = lw6sys_get_timestamp () + _TEST_NODE_POLL_DURATION;
  while (lw6sys_get_timestamp () < next_timestamp)
    {
      if (node1)
	{
	  lw6p2p_node_poll (node1, NULL);
	}
      if (node2)
	{
	  lw6p2p_node_poll (node2, NULL);
	}
      if (node3)
	{
	  lw6p2p_node_poll (node3, NULL);
	}
      if (node4)
	{
	  lw6p2p_node_poll (node4, NULL);
	}
      if (node5)
	{
	  lw6p2p_node_poll (node5, NULL);
	}
      if (node6)
	{
	  lw6p2p_node_poll (node6, NULL);
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
  end_timestamp = lw6sys_get_timestamp () + _TEST_NODE_CMD_DURATION;
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
    ret = ret && _cmd_with_backends ("tcp", "tcpd");
    ret = ret && _cmd_with_backends ("udp", "udpd");
    _cmd_with_backends ("http", "httpd");	// even if fails, keep going since http is optional
    ret = ret
      && _cmd_with_backends (lw6cli_default_backends (),
			     lw6srv_default_backends ());
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
  end_timestamp = lw6sys_get_timestamp () + _TEST_NODE_OOB_DURATION;
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
 * Testing node msg
 */
static int
_test_node_msg ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    const int argc = _TEST_ARGC;
    const char *argv[] = { _TEST_ARGV0 };
    lw6p2p_db_t *db = NULL;
    lw6p2p_node_t *node = NULL;
    u_int64_t node_id;
    int i, j, k;
    int64_t seq;
    char *random_str;
    u_int32_t checksum;
    u_int32_t checksums[_TEST_NODE_MSG_NB_SEQS * _TEST_NODE_MSG_NB_PER_SEQ];
    char *msg;
    char *msgs[_TEST_NODE_MSG_NB_SEQS * _TEST_NODE_MSG_NB_PER_SEQ];
    int found;
    lw6sys_progress_t progress;
    float progress_value = 0.0f;

    memset (checksums, 0, sizeof (checksums));
    memset (msgs, 0, sizeof (msgs));
    memset (&progress, 0, sizeof (lw6sys_progress_t));
    lw6sys_progress_default (&progress, &progress_value);

    db = lw6p2p_db_open (argc, argv, _TEST_DB_NAME12);
    if (db)
      {
	/*
	 * Passing 0LL as a node id will cause automatic generation of an id
	 */
	node =
	  lw6p2p_node_new (argc, argv, db, lw6cli_default_backends (),
			   lw6srv_default_backends (), _TEST_NODE_BIND_IP,
			   _TEST_NODE_BIND_PORT1, _TEST_NODE_BROADCAST,
			   0LL,
			   _TEST_NODE_PUBLIC_URL1,
			   _TEST_NODE_TITLE1, _TEST_NODE_DESCRIPTION, NULL,
			   _TEST_NODE_BENCH, _TEST_NODE_OPEN_RELAY,
			   _TEST_NODE_KNOWN_NODES1,
			   _TEST_NODE_NETWORK_RELIABILITY, _TEST_NODE_TROJAN);
	if (node)
	  {
	    node_id = lw6p2p_node_get_id (node);
	    seq = _TEST_NODE_MSG_SEQ_0;
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("step 1, seq_max=%" LW6SYS_PRINTF_LL "d"),
			(long long) seq);
	    for (i = 0, k = 0; i < _TEST_NODE_MSG_NB_SEQS; ++i)
	      {
		seq++;		// fake we're going next round...
		for (j = 0; j < _TEST_NODE_MSG_NB_PER_SEQ; ++j, ++k)
		  {
		    random_str =
		      lw6sys_str_random_words (lw6sys_random
					       (_TEST_NODE_MSG_RANDOM_STR_SIZE)
					       + 1);
		    if (random_str)
		      {
			msg =
			  lw6sys_new_sprintf ("%" LW6SYS_PRINTF_LL "d %"
					      LW6SYS_PRINTF_LL "x k=%d %s",
					      (long long) seq,
					      (long long) node_id,
					      k, random_str);
			if (msg)
			  {
			    checksums[k] = lw6sys_checksum_str (msg);
			    msgs[k] = msg;
			    lw6p2p_node_put_local_msg (node, msg);
			  }
			LW6SYS_FREE (random_str);
		      }
		  }
	      }
	    seq = lw6p2p_node_get_seq_max (node);
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("step 2, seq_max=%" LW6SYS_PRINTF_LL "d"),
			(long long) seq);
	    k = 0;
	    while ((msg =
		    lw6p2p_node_get_next_draft_msg (node, &progress)) != NULL)
	      {
		checksum = lw6sys_checksum_str (msg);
		/*
		 * We need to loop over all received messages for the order might
		 * have change, there's an alphabetical sort on the messages for
		 * a same seq & source.
		 */
		found = 0;
		for (i = 0;
		     i < _TEST_NODE_MSG_NB_SEQS * _TEST_NODE_MSG_NB_PER_SEQ;
		     ++i)
		  {
		    if (checksum == checksums[i]
			&& lw6sys_str_is_same (msg, msgs[i]) && !found)
		      {
			found = 1;
		      }
		  }
		if (!found)
		  {
		    lw6sys_log (LW6SYS_LOG_WARNING,
				_x_
				("unable to find message for k=%d, len=%d msg=\"%s\""),
				(int) k, (int) strlen (msg), msg);
		    ret = 0;
		  }
		LW6SYS_FREE (msg);
		++k;
	      }
	    if (k == _TEST_NODE_MSG_NB_SEQS * _TEST_NODE_MSG_NB_PER_SEQ)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("got %d draft messages"),
			    k);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_ ("only got %d draft messages, needed %d"), k,
			    _TEST_NODE_MSG_NB_SEQS *
			    _TEST_NODE_MSG_NB_PER_SEQ);
		ret = 0;
	      }
	    for (k = 0;
		 k < _TEST_NODE_MSG_NB_SEQS * _TEST_NODE_MSG_NB_PER_SEQ; ++k)
	      {
		if (msgs[k])
		  {
		    LW6SYS_FREE (msgs[k]);
		  }
	      }

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

typedef struct _test_node_api_data_s
{
  lw6p2p_node_t *node;
  float progress_value;
  lw6sys_progress_t progress;
  u_int64_t peer_id;
  char *dump;
  int *done;
  int ret;
} _test_node_api_data_t;

static void
_test_node_api_node1_callback (void *api_data)
{
  _test_node_api_data_t *data = (_test_node_api_data_t *) api_data;
  int64_t end_timestamp = 0LL;

  end_timestamp = lw6sys_get_timestamp () + _TEST_NODE_API_DURATION_THREAD;

  data->ret = 1;

  while (lw6sys_get_timestamp () < end_timestamp && !(*(data->done)))
    {
      lw6p2p_node_poll (data->node, &(data->progress));
      lw6sys_idle ();
    }
}

static void
_test_node_api_node2_callback (void *api_data)
{
  _test_node_api_data_t *data = (_test_node_api_data_t *) api_data;
  int64_t begin_timestamp = 0LL;
  int64_t end_timestamp = 0LL;
  u_int64_t node_id = 0LL;
  int64_t seq = 0LL;
  char *dump = NULL;
  char *msg = NULL;
  char *tmp = NULL;
  int len = 0;
  int dump_sent = 0;
  int64_t next_msgn_timestamp = 0LL;

  begin_timestamp = lw6sys_get_timestamp () + _TEST_NODE_ACK_DELAY_MSEC;
  end_timestamp = begin_timestamp + _TEST_NODE_API_DURATION_THREAD;

  /*
   * This node acts as a server.
   */
  data->ret = lw6p2p_node_server_start (data->node, _TEST_NODE_API_SEQ_0);
  if (data->ret)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("\"%s\" is ready"),
		  ((_lw6p2p_node_t *) data->node)->public_url);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("could not start \"%s\""),
		  ((_lw6p2p_node_t *) data->node)->public_url);
    }

  node_id = lw6p2p_node_get_id (data->node);
  seq = lw6p2p_node_get_seq_max (data->node);
  seq++;			// fake we're going next round...
  while (lw6sys_get_timestamp () < begin_timestamp)
    {
      lw6p2p_node_poll (data->node, &(data->progress));
      lw6sys_idle ();
    }
  while (lw6sys_get_timestamp () < end_timestamp && !(*(data->done)))
    {
      lw6p2p_node_poll (data->node, &(data->progress));
      lw6sys_idle ();
      if (lw6p2p_node_is_dump_needed (data->node))
	{
	  dump = lw6sys_str_random_words (_TEST_NODE_API_DUMP_SIZE);
	  if (dump)
	    {
	      msg =
		lw6sys_new_sprintf ("%" LW6SYS_PRINTF_LL "d %"
				    LW6SYS_PRINTF_LL "x %s", (long long) seq,
				    (long long) node_id, dump);
	      if (msg)
		{
		  len = strlen (msg);
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _x_ ("node2 sent dump draft message len=%d"),
			      len);
		  if (len < _TEST_NODE_API_LEN_LIMIT)
		    {
		      lw6sys_log (LW6SYS_LOG_NOTICE,
				  _x_ ("node2 dump draft message is \"%s\""),
				  msg);
		    }
		  lw6p2p_node_put_local_msg (data->node, msg);
		  LW6SYS_FREE (msg);
		}
	      msg =
		lw6sys_new_sprintf ("%" LW6SYS_PRINTF_LL "d %"
				    LW6SYS_PRINTF_LL "x %s", (long long) seq,
				    (long long) node_id,
				    _TEST_NODE_API_MSG1_STR);
	      if (msg)
		{
		  len = strlen (msg);
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _x_ ("node2 sent draft message len=%d"), len);
		  if (len < _TEST_NODE_API_LEN_LIMIT)
		    {
		      lw6sys_log (LW6SYS_LOG_NOTICE,
				  _x_ ("node2 draft message is \"%s\""), msg);
		    }
		  lw6p2p_node_put_local_msg (data->node, msg);
		  LW6SYS_FREE (msg);
		}
	      seq++;		// fake we're going next round...
	      msg =
		lw6sys_new_sprintf ("%" LW6SYS_PRINTF_LL "d %"
				    LW6SYS_PRINTF_LL "x %s", (long long) seq,
				    (long long) node_id,
				    _TEST_NODE_API_MSG2_STR);
	      if (msg)
		{
		  len = strlen (msg);
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _x_ ("node2 sent draft message len=%d"), len);
		  if (len < _TEST_NODE_API_LEN_LIMIT)
		    {
		      lw6sys_log (LW6SYS_LOG_NOTICE,
				  _x_ ("node2 draft message is \"%s\""), msg);
		    }
		  lw6p2p_node_put_local_msg (data->node, msg);
		  LW6SYS_FREE (msg);
		}
	      msg =
		lw6sys_new_sprintf ("%" LW6SYS_PRINTF_LL "d %"
				    LW6SYS_PRINTF_LL "x %s", (long long) seq,
				    (long long) node_id,
				    _TEST_NODE_API_MSG3_STR);
	      if (msg)
		{
		  len = strlen (msg);
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _x_ ("node2 sent draft message len=%d"), len);
		  if (len < _TEST_NODE_API_LEN_LIMIT)
		    {
		      lw6sys_log (LW6SYS_LOG_NOTICE,
				  _x_ ("node2 draft message is \"%s\""), msg);
		    }
		  lw6p2p_node_put_local_msg (data->node, msg);
		  LW6SYS_FREE (msg);
		}

	      /*
	       * The last message we send is a long one. In real usage one
	       * can obviously send shorter messages, but the idea is that when
	       * testing, sending partial message will trigger the MISS message
	       * automatically. Else if the last message is a short one, MISS is
	       * never sent, just because, well, receiver can really not guess
	       * something is missing
	       */
	      tmp = lw6sys_str_random_word (_TEST_NODE_API_FINAL_SIZE);
	      if (tmp)
		{
		  msg =
		    lw6sys_new_sprintf ("%" LW6SYS_PRINTF_LL "d %"
					LW6SYS_PRINTF_LL "x %s",
					(long long) seq, (long long) node_id,
					tmp);
		  LW6SYS_FREE (tmp);
		}
	      if (msg)
		{
		  len = strlen (msg);
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _x_ ("node2 sent final draft message len=%d"),
			      len);
		  if (len < _TEST_NODE_API_LEN_LIMIT)
		    {
		      lw6sys_log (LW6SYS_LOG_NOTICE,
				  _x_ ("node2 final draft message is \"%s\""),
				  msg);
		    }
		  lw6p2p_node_put_local_msg (data->node, msg);
		  LW6SYS_FREE (msg);
		}

	      LW6SYS_FREE (dump);
	      dump_sent = 1;
	    }
	}

      if (dump_sent)
	{
	  if (next_msgn_timestamp < lw6sys_get_timestamp ())
	    {
	      seq++;		// fake we're going next round...
	      msg =
		lw6sys_new_sprintf ("%" LW6SYS_PRINTF_LL "d %"
				    LW6SYS_PRINTF_LL "x %s", (long long) seq,
				    (long long) node_id,
				    _TEST_NODE_API_MSGN_STR);
	      if (msg)
		{
		  len = strlen (msg);
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _x_ ("node2 sent draft message len=%d"), len);
		  if (len < _TEST_NODE_API_LEN_LIMIT)
		    {
		      lw6sys_log (LW6SYS_LOG_NOTICE,
				  _x_ ("node2 draft message is \"%s\""), msg);
		    }
		  lw6p2p_node_put_local_msg (data->node, msg);
		  LW6SYS_FREE (msg);
		}
	      next_msgn_timestamp =
		lw6sys_get_timestamp () + _TEST_NODE_API_MSGN_DELAY_MSEC;
	    }
	}
    }

  if (data->ret)
    {
      lw6p2p_node_disconnect (data->node);
    }
}

static void
_test_node_api_node3_callback (void *api_data)
{
  _test_node_api_data_t *data = (_test_node_api_data_t *) api_data;
  int64_t end_timestamp = 0LL;

  data->ret = 1;

  end_timestamp = lw6sys_get_timestamp () + _TEST_NODE_API_DURATION_THREAD;
  while (lw6sys_get_timestamp () < end_timestamp && !(*(data->done)))
    {
      lw6p2p_node_poll (data->node, &(data->progress));
      lw6sys_idle ();
    }
}

static void
_test_node_api_node4_callback (void *api_data)
{
  _test_node_api_data_t *data = (_test_node_api_data_t *) api_data;
  int64_t begin_timestamp = 0LL;
  int64_t end_timestamp = 0LL;
  char *msg = NULL;
  int len = 0;
  int64_t seq;
  int long_draft_received = 0;
  int short_draft_received = 0;
  int reference_received = 0;

  begin_timestamp = lw6sys_get_timestamp () + _TEST_NODE_ACK_DELAY_MSEC;
  end_timestamp = begin_timestamp + _TEST_NODE_API_DURATION_THREAD;
  data->ret = 0;

  /*
   * This node acts as a client.
   */
  lw6sys_delay (_TEST_NODE_API_DURATION_JOIN);
  if (lw6p2p_node_client_join
      (data->node, data->peer_id, _TEST_NODE_PUBLIC_URL2, &(data->progress)))
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("\"%s\" joined \"%s\""),
		  ((_lw6p2p_node_t *) data->node)->public_url,
		  _TEST_NODE_PUBLIC_URL2);

      while (lw6sys_get_timestamp () < begin_timestamp)
	{
	  lw6p2p_node_poll (data->node, &(data->progress));
	  lw6sys_idle ();
	}

      seq = lw6p2p_node_get_seq_max (data->node);
      lw6sys_log (LW6SYS_LOG_NOTICE,
		  _x_ ("node4 before loop seq_max=%" LW6SYS_PRINTF_LL "d"),
		  (long long) seq);
      while (lw6sys_get_timestamp () < end_timestamp && !(*(data->done)))
	{
	  lw6p2p_node_poll (data->node, &(data->progress));
	  lw6sys_idle ();

	  while ((msg =
		  lw6p2p_node_get_next_reference_msg (data->node,
						      &(data->progress))) !=
		 NULL)
	    {
	      len = strlen (msg);
	      lw6sys_log (LW6SYS_LOG_NOTICE,
			  _x_ ("node4 received reference message len=%d"),
			  len);
	      if (len < _TEST_NODE_API_LEN_LIMIT)
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _x_ ("node4 reference message is \"%s\""), msg);
		}
	      ++reference_received;
	      LW6SYS_FREE (msg);
	    }
	  while ((msg =
		  lw6p2p_node_get_next_draft_msg (data->node,
						  &(data->progress))) != NULL)
	    {
	      len = strlen (msg);
	      lw6sys_log (LW6SYS_LOG_NOTICE,
			  _x_ ("node4 received draft message len=%d"), len);
	      if (len < _TEST_NODE_API_LEN_LIMIT)
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _x_ ("node4 draft message is \"%s\""), msg);
		  ++short_draft_received;
		}
	      else
		{
		  ++long_draft_received;
		}
	      LW6SYS_FREE (msg);
	    }
	  if (reference_received >= _TEST_NODE_API_NODE4_REFERENCE_RECEIVED &&
	      long_draft_received >= _TEST_NODE_API_NODE4_LONG_DRAFT_RECEIVED
	      && short_draft_received >=
	      _TEST_NODE_API_NODE4_SHORT_DRAFT_RECEIVED)
	    {
	      /*
	       * If there are enough messages, consider the test is done,
	       * this does not necessarly means it succeeded, even if there
	       * are good chances it did, but at least it avoids waiting
	       * uselessly for a long time on fast machines.
	       */
	      lw6sys_log (LW6SYS_LOG_NOTICE,
			  _x_ ("node4, marking test as done"));
	      (*(data->done)) = 1;
	    }
	}
      seq = lw6p2p_node_get_seq_max (data->node);
      lw6sys_log (LW6SYS_LOG_NOTICE,
		  _x_ ("node4 after loop seq_max=%" LW6SYS_PRINTF_LL "d"),
		  (long long) seq);
      /*
       * We don't test the draft received value for its exact value since
       * some extra NOP messages might have been sent
       */
      if (reference_received == _TEST_NODE_API_NODE4_REFERENCE_RECEIVED &&
	  long_draft_received == _TEST_NODE_API_NODE4_LONG_DRAFT_RECEIVED &&
	  short_draft_received >= _TEST_NODE_API_NODE4_LONG_DRAFT_RECEIVED)
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE,
		      _x_
		      ("node4, looks like messages made it here, reference_received=%d, long_draft_received=%d, short_draft_received=%d"),
		      reference_received, long_draft_received,
		      short_draft_received);
	  data->ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_
		      ("node4, mismatch in number of received messages reference_received=%d should be %d long_draft_received=%d should be %d short_draft_received=%d should be %d"),
		      reference_received,
		      _TEST_NODE_API_NODE4_REFERENCE_RECEIVED,
		      long_draft_received,
		      _TEST_NODE_API_NODE4_LONG_DRAFT_RECEIVED,
		      short_draft_received,
		      _TEST_NODE_API_NODE4_SHORT_DRAFT_RECEIVED);
	}
      lw6p2p_node_disconnect (data->node);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("\"%s\" couldn't join \"%s\""),
		  ((_lw6p2p_node_t *) data->node)->public_url,
		  _TEST_NODE_PUBLIC_URL2);
    }
}

static void
_test_node_api_node5_callback (void *api_data)
{
  _test_node_api_data_t *data = (_test_node_api_data_t *) api_data;
  int64_t end_timestamp = 0LL;

  end_timestamp = lw6sys_get_timestamp () + _TEST_NODE_API_DURATION_THREAD;

  data->ret = 1;

  while (lw6sys_get_timestamp () < end_timestamp && !(*(data->done)))
    {
      lw6p2p_node_poll (data->node, &(data->progress));
      lw6sys_idle ();
    }
}

static void
_test_node_api_node6_callback (void *api_data)
{
  _test_node_api_data_t *data = (_test_node_api_data_t *) api_data;
  int64_t end_timestamp = 0LL;

  end_timestamp = lw6sys_get_timestamp () + _TEST_NODE_API_DURATION_THREAD;

  data->ret = 1;

  while (lw6sys_get_timestamp () < end_timestamp && !(*(data->done)))
    {
      lw6p2p_node_poll (data->node, &(data->progress));
      lw6sys_idle ();
    }
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
    lw6sys_thread_handler_t *thread1 = NULL;
    lw6sys_thread_handler_t *thread2 = NULL;
    lw6sys_thread_handler_t *thread3 = NULL;
    lw6sys_thread_handler_t *thread4 = NULL;
    lw6sys_thread_handler_t *thread5 = NULL;
    lw6sys_thread_handler_t *thread6 = NULL;
    _test_node_api_data_t api_data1;
    _test_node_api_data_t api_data2;
    _test_node_api_data_t api_data3;
    _test_node_api_data_t api_data4;
    _test_node_api_data_t api_data5;
    _test_node_api_data_t api_data6;
    int done = 0;

    memset (&api_data1, 0, sizeof (_test_node_api_data_t));
    memset (&api_data2, 0, sizeof (_test_node_api_data_t));
    memset (&api_data3, 0, sizeof (_test_node_api_data_t));
    memset (&api_data4, 0, sizeof (_test_node_api_data_t));
    memset (&api_data5, 0, sizeof (_test_node_api_data_t));
    memset (&api_data6, 0, sizeof (_test_node_api_data_t));

    if (_init_nodes
	(lw6cli_default_backends (),
	 lw6srv_default_backends (), &db12, &db34, &db56, &node1, &node2,
	 &node3, &node4, &node5, &node6))
      {
	api_data1.node = node1;
	api_data2.node = node2;
	api_data3.node = node3;
	api_data4.node = node4;
	api_data5.node = node5;
	api_data6.node = node6;
	api_data1.done = &done;
	api_data2.done = &done;
	api_data3.done = &done;
	api_data4.done = &done;
	api_data5.done = &done;
	api_data6.done = &done;
	lw6sys_progress_default (&(api_data1.progress),
				 &(api_data1.progress_value));
	lw6sys_progress_default (&(api_data2.progress),
				 &(api_data2.progress_value));
	lw6sys_progress_default (&(api_data3.progress),
				 &(api_data3.progress_value));
	lw6sys_progress_default (&(api_data4.progress),
				 &(api_data4.progress_value));
	lw6sys_progress_default (&(api_data5.progress),
				 &(api_data5.progress_value));
	lw6sys_progress_default (&(api_data6.progress),
				 &(api_data6.progress_value));
	lw6sys_progress_begin (&(api_data1.progress));
	lw6sys_progress_begin (&(api_data2.progress));
	lw6sys_progress_begin (&(api_data3.progress));
	lw6sys_progress_begin (&(api_data4.progress));
	lw6sys_progress_begin (&(api_data5.progress));
	lw6sys_progress_begin (&(api_data6.progress));

	api_data4.peer_id = lw6p2p_node_get_id (node2);

	thread1 =
	  lw6sys_thread_create (_test_node_api_node1_callback, NULL,
				(void *) &api_data1);
	thread2 =
	  lw6sys_thread_create (_test_node_api_node2_callback, NULL,
				(void *) &api_data2);
	thread3 =
	  lw6sys_thread_create (_test_node_api_node3_callback, NULL,
				(void *) &api_data3);
	thread4 =
	  lw6sys_thread_create (_test_node_api_node4_callback, NULL,
				(void *) &api_data4);
	thread5 =
	  lw6sys_thread_create (_test_node_api_node5_callback, NULL,
				(void *) &api_data5);
	thread6 =
	  lw6sys_thread_create (_test_node_api_node6_callback, NULL,
				(void *) &api_data6);

	if (thread1 && thread2 && thread3 && thread4 && thread5 && thread6)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("each node running in its own thread"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("unable to start all threads"));
	    ret = 0;
	  }
	if (thread1)
	  {
	    lw6sys_thread_join (thread1);
	  }
	if (thread2)
	  {
	    lw6sys_thread_join (thread2);
	  }
	if (thread3)
	  {
	    lw6sys_thread_join (thread3);
	  }
	if (thread4)
	  {
	    lw6sys_thread_join (thread4);
	  }
	if (thread5)
	  {
	    lw6sys_thread_join (thread5);
	  }
	if (thread6)
	  {
	    lw6sys_thread_join (thread6);
	  }
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("waiting for some time to let nodes handle disconnection"));
	end_timestamp = lw6sys_get_timestamp () + _TEST_NODE_API_DURATION_END;
	while (lw6sys_get_timestamp () < end_timestamp)
	  {
	    _poll_nodes (node1, node2, node3, node4, node5, node6);
	  }

	_quit_nodes (db12, db34, db56, node1, node2, node3, node4, node5,
		     node6);

	ret = ret && api_data1.ret && api_data2.ret && api_data3.ret
	  && api_data4.ret && api_data5.ret && api_data6.ret;
	if (ret)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("all nodes completed their tests successfully"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_
			("at least one node failed to complete its test, results are api_data1.ret=%d, api_data2.ret=%d, api_data3.ret=%d, api_data4.ret=%d, api_data5.ret=%d, api_data6.ret=%d"),
			api_data1.ret, api_data2.ret, api_data3.ret,
			api_data4.ret, api_data5.ret, api_data6.ret);
	  }
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
      /*
       * Replace the following to switch between two modes:
       * - lw6sys_true() -> release mode, all tests enabled
       * - lw6sys_false() -> dev mode, api is the most error-prone suite
       */
      if (lw6sys_true ())
	{
	  ret = _test_db () && _test_entry () && _test_node_init ()
	    && _test_node_oob () && _test_node_cmd () && _test_node_msg ()
	    && _test_node_api ();
	}
      else
	{
	  ret = _test_node_api ();
	}

      lw6net_quit (argc, argv);
    }

  return ret;
}
