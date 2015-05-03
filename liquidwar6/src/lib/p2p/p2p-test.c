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

#define _TEST_TENTACLE_DURATION_THREAD 6000
#define _TEST_TENTACLE_BIND_IP1 "127.0.0.1"
#define _TEST_TENTACLE_BIND_PORT1 8058
#define _TEST_TENTACLE_URL1 "127.0.0.1:8058"
#define _TEST_TENTACLE_ID1 0x1234123412341234LL
#define _TEST_TENTACLE_ID1_STR "1234123412341234"
#define _TEST_TENTACLE_BIND_IP2 "127.0.0.1"
#define _TEST_TENTACLE_BIND_PORT2 8059
#define _TEST_TENTACLE_URL2 "127.0.0.1:8059"
#define _TEST_TENTACLE_ID2 0x2345234523452345LL
#define _TEST_TENTACLE_ID2_STR "2345234523452345"
#define _TEST_TENTACLE_PASSWORD "toto"
#define _TEST_TENTACLE_NETWORK_RELIABILITY 10
#define _TEST_TENTACLE_TICKET_TABLE_HASH_SIZE 13
#define _TEST_TENTACLE_SEND_BEST_RELIABLE_STR1 "best reliable message ONE"
#define _TEST_TENTACLE_SEND_BEST_UNRELIABLE_STR1 "best unreliable message ONE"
#define _TEST_TENTACLE_SEND_REDUNDANT_STR1 "redundant message ONE"
#define _TEST_TENTACLE_SEND_BEST_RELIABLE_STR2 "best reliable message TWO"
#define _TEST_TENTACLE_SEND_BEST_UNRELIABLE_STR2 "best unreliable message TWO"
#define _TEST_TENTACLE_SEND_REDUNDANT_STR2 "redundant message TWO"
#define _TEST_TENTACLE_ACCEPT_DELAY 1500

#define _TEST_TENTACLE1_PROGRAM "foo"
#define _TEST_TENTACLE1_VERSION "123"
#define _TEST_TENTACLE1_CODENAME "toto"
#define _TEST_TENTACLE1_STAMP 12
#define _TEST_TENTACLE1_ID _TEST_TENTACLE_ID1
#define _TEST_TENTACLE1_URL _TEST_TENTACLE_URL1
#define _TEST_TENTACLE1_TITLE "fake"
#define _TEST_TENTACLE1_DESCRIPTION "this is a fake description"
#define _TEST_TENTACLE1_PASSWORD NULL
#define _TEST_TENTACLE1_BENCH 10
#define _TEST_TENTACLE1_OPEN_RELAY 0
#define _TEST_TENTACLE1_UPTIME 10
#define _TEST_TENTACLE1_IDLE_SCREENSHOT_SIZE 0
#define _TEST_TENTACLE1_IDLE_SCREENSHOT_DATA NULL

#define _TEST_TENTACLE2_PROGRAM "foo"
#define _TEST_TENTACLE2_VERSION "123"
#define _TEST_TENTACLE2_CODENAME "toto"
#define _TEST_TENTACLE2_STAMP 12
#define _TEST_TENTACLE2_ID _TEST_TENTACLE_ID2
#define _TEST_TENTACLE2_URL _TEST_TENTACLE_URL2
#define _TEST_TENTACLE2_TITLE "fake"
#define _TEST_TENTACLE2_DESCRIPTION "this is a fake description"
#define _TEST_TENTACLE2_PASSWORD NULL
#define _TEST_TENTACLE2_BENCH 10
#define _TEST_TENTACLE2_OPEN_RELAY 0
#define _TEST_TENTACLE2_UPTIME 10
#define _TEST_TENTACLE2_IDLE_SCREENSHOT_SIZE 0
#define _TEST_TENTACLE2_IDLE_SCREENSHOT_DATA NULL

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
#define _TEST_NODE_UNREACHABLE_URL "http://ufoot.org:9000/"
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
#define _TEST_NODE_REFRESH_DURATION 3000
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

typedef struct _lw6p2p_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
} _lw6p2p_test_data_t;

static _lw6p2p_test_data_t _test_data = { 0, NULL };

/*
 * Testing db
 */
static void
_test_db ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    const int argc = _TEST_ARGC;
    const char *argv[] = { _TEST_ARGV0 };
    lw6p2p_db_t *db = NULL;
    char *repr = NULL;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		_x_ ("default database name is \"%s\" but we use \"%s\" for some tests"), lw6p2p_db_default_name (sys_context), _TEST_DB_NAME12);

    db = lw6p2p_db_open (sys_context, argc, argv, _TEST_DB_NAME12);
    if (LW6SYS_TEST_ACK (db))
      {
	repr = lw6p2p_db_repr (sys_context, db);
	if (LW6SYS_TEST_ACK (repr))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("created db \"%s\""), repr);
	    LW6SYS_FREE (sys_context, repr);
	  }
	lw6p2p_db_close (sys_context, db);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't create db"));
	ret = 0;
      }

    if (LW6SYS_TEST_ACK (lw6p2p_db_reset (sys_context, argc, argv, _TEST_DB_NAME12)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("deleted db"));
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't delete db"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing entry
 */
static void
_test_entry ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6p2p_entry_t *entry;
    char *repr;

    entry = lw6p2p_entry_new (sys_context, _TEST_ENTRY_CREATION_TIMESTAMP,
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
			      _TEST_ENTRY_IP, _TEST_ENTRY_PORT, _TEST_ENTRY_LAST_PING_TIMESTAMP, _TEST_ENTRY_PING_DELAY_MSEC, _TEST_ENTRY_AVAILABLE);
    if (LW6SYS_TEST_ACK (entry))
      {
	repr = lw6p2p_entry_repr (sys_context, entry);
	if (LW6SYS_TEST_ACK (repr))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("entry created: \"%s\""), repr);
	    LW6SYS_FREE (sys_context, repr);
	  }
	lw6p2p_entry_free (sys_context, entry);
	entry = NULL;
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create entry"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

typedef struct _test_tentacle_data_s
{
  _lw6p2p_tentacle_t tentacle;
  _lw6p2p_backends_t backends;
  lw6nod_info_t *node_info;
  lw6srv_listener_t *listener;
  float progress_value;
  lw6sys_progress_t progress;
  int *done;
  int ret;
} _test_tentacle_data_t;

static int
_test_tentacle_poll_step1_accept_tcp (lw6sys_context_t * sys_context, lw6srv_listener_t * listener, int64_t now)
{
  int ret = 1;
  char *ip = NULL;
  int port = 0;
  int sock = LW6NET_SOCKET_INVALID;
  lw6srv_tcp_accepter_t *tcp_accepter = NULL;
  char *guessed_public_url = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("polling node TCP sock=%d"), listener->tcp_sock);

  if (lw6net_socket_is_valid (sys_context, listener->tcp_sock))
    {
      sock = lw6net_tcp_accept (sys_context, &ip, &port, listener->tcp_sock, _TEST_TENTACLE_ACCEPT_DELAY);
      if (lw6net_socket_is_valid (sys_context, sock) && ip != NULL && port > 0)
	{
	  tcp_accepter = lw6srv_tcp_accepter_new (sys_context, ip, port, sock);
	  if (tcp_accepter)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("TCP connection from %s:%d"), ip, port);
	      lw6sys_list_push_front (sys_context, &(listener->tcp_accepters), tcp_accepter);

	      /*
	       * Now we register this peer as a potential server,
	       * it will be qualified as a real server (or not) later
	       */
	      guessed_public_url = lw6sys_url_http_from_ip_port (sys_context, ip, LW6NET_DEFAULT_PORT);
	      if (guessed_public_url)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("discovered node \"%s\" from guessed url"), guessed_public_url);
		  LW6SYS_FREE (sys_context, guessed_public_url);
		}

	      ip = NULL;	// tcp_accepter will free it
	    }
	  else
	    {
	      ret = 0;
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("bad socket accepted %d"), sock);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("bad socket in listener %d"), listener->tcp_sock);
    }

  if (ip)
    {
      LW6SYS_FREE (sys_context, ip);
    }

  return ret;
}

static int
_test_tentacle_poll_step2_recv_udp (lw6sys_context_t * sys_context, lw6srv_listener_t * listener, int64_t now)
{
  int ret = 1;
  char buf[LW6NET_UDP_MINIMAL_BUF_SIZE + 1];
  char *ip1 = NULL;
  char *ip2 = NULL;
  int port1 = 0;
  int port2 = 0;
  char *line = NULL;
  lw6srv_udp_buffer_t *udp_buffer = NULL;
  char *guessed_public_url = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("polling node UDP sock=%d"), listener->udp_sock);
  memset (buf, 0, LW6NET_UDP_MINIMAL_BUF_SIZE + 1);
  if (listener->udp_sock >= 0)
    {
      if (lw6net_udp_peek (sys_context, listener->udp_sock, buf, LW6NET_UDP_MINIMAL_BUF_SIZE, &ip1, &port1))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("received data from %s:%d"), ip1, port1);
	  line = lw6net_recv_line_udp (sys_context, listener->udp_sock, &ip2, &port2);
	  if (line)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("UDP connection from %s:%d"), ip2, port2);
	      udp_buffer = lw6srv_udp_buffer_new (sys_context, ip2, port2, line);
	      if (udp_buffer)
		{
		  lw6sys_list_push_front (sys_context, &(listener->udp_buffers), udp_buffer);
		  /*
		   * Now we register this peer as a potential server,
		   * it will be qualified as a real server (or not) later
		   */
		  guessed_public_url = lw6sys_url_http_from_ip_port (sys_context, ip2, LW6NET_DEFAULT_PORT);
		  if (guessed_public_url)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("discovered node \"%s\" from guessed url"), guessed_public_url);
		      LW6SYS_FREE (sys_context, guessed_public_url);
		    }

		  line = NULL;	// udp_buffer will free it
		  ip2 = NULL;	// udp_buffer will free it
		}
	      else
		{
		  ret = 0;
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("udp data received from %s:%d, but it's not correct"), ip1, port1);
	    }
	}
    }

  if (ip1)
    {
      LW6SYS_FREE (sys_context, ip1);
    }
  if (ip2)
    {
      LW6SYS_FREE (sys_context, ip2);
    }
  if (line)
    {
      LW6SYS_FREE (sys_context, line);
    }

  return ret;
}

static int
_test_tentacle_tcp_accepter_reply (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  int ret = 1;
  _test_tentacle_data_t *tentacle_data = (_test_tentacle_data_t *) func_data;
  lw6srv_tcp_accepter_t *tcp_accepter = (lw6srv_tcp_accepter_t *) data;

  if (LW6SYS_TEST_ACK (tcp_accepter != NULL))
    {
      int i = 0;
      int analyse_tcp_ret = 0;
      u_int64_t remote_id = 0L;

      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("processing tcp reply sock=%d"), tcp_accepter->sock);
      lw6net_tcp_peek (sys_context, &(tcp_accepter->sock), tcp_accepter->first_line, LW6SRV_PROTOCOL_BUFFER_SIZE, 0);
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("received TCP data \"%s\""), tcp_accepter->first_line);

      for (i = 0; i < tentacle_data->backends.nb_srv_backends && ret; ++i)
	{
	  analyse_tcp_ret =
	    lw6srv_analyse_tcp (sys_context, tentacle_data->backends.srv_backends[i], tcp_accepter, tentacle_data->node_info, &remote_id, NULL);
	  if (analyse_tcp_ret & LW6SRV_ANALYSE_DEAD)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("dead accepter, closing socket %d"), tcp_accepter->sock);
	      lw6net_socket_close (sys_context, &(tcp_accepter->sock));
	      ret = 0;		// free this accepter
	    }
	  else
	    {
	      if (analyse_tcp_ret & LW6SRV_ANALYSE_UNDERSTANDABLE)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("accepter OK, message understood"));
		  lw6srv_feed_with_tcp (sys_context, tentacle_data->backends.srv_backends[i], tentacle_data->tentacle.srv_connections[i], tcp_accepter);
		  ret = 0;	// free this accepter
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("accepter OK but message ununderstandable"));
		}
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("received TCP data but pointer is NULL"));
      tentacle_data->ret = 0;
    }

  return ret;
}

static int
_test_tentacle_udp_buffer_reply (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  int ret = 1;
  _test_tentacle_data_t *tentacle_data = (_test_tentacle_data_t *) func_data;
  lw6srv_udp_buffer_t *udp_buffer = (lw6srv_udp_buffer_t *) data;

  if (LW6SYS_TEST_ACK (udp_buffer != NULL))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("received UDP data \"%s\""), udp_buffer->line);
      ret = 0;			// free this buffer
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("received UDP data but pointer is NULL"));
      tentacle_data->ret = 0;
    }

  return ret;
}

static int
_test_tentacle_poll_step3_reply (lw6sys_context_t * sys_context, lw6srv_listener_t * listener, int64_t now, _test_tentacle_data_t * data)
{
  int ret = 1;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("processing reply, len(tcp)=%d len(udp)=%d"), lw6sys_list_length (sys_context, listener->tcp_accepters),
	      lw6sys_list_length (sys_context, listener->udp_buffers));

  lw6sys_list_filter (sys_context, &(listener->tcp_accepters), _test_tentacle_tcp_accepter_reply, data);
  lw6sys_list_filter (sys_context, &(listener->udp_buffers), _test_tentacle_udp_buffer_reply, data);

  return ret;
}


static void
_test_tentacle1_thread_callback (lw6sys_context_t * sys_context, void *tentacle_data)
{
  _test_tentacle_data_t *data = (_test_tentacle_data_t *) tentacle_data;
  int64_t end_timestamp = 0LL;
  lw6cnx_ticket_table_t ticket_table;
  u_int32_t logical_ticket_sig = 0;
  int nb_sent_best_reliable = 0;
  int nb_sent_best_unreliable = 0;
  int nb_sent_redundant = 0;
  int64_t now = 0LL;

  end_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_TENTACLE_DURATION_THREAD;

  data->ret = 1;

  lw6cnx_ticket_table_zero (sys_context, &ticket_table);
  if (lw6cnx_ticket_table_init (sys_context, &ticket_table, _TEST_TENTACLE_TICKET_TABLE_HASH_SIZE))
    {
      if (_lw6p2p_tentacle_init
	  (sys_context, &(data->tentacle), &(data->backends), data->listener,
	   _TEST_TENTACLE_URL1, _TEST_TENTACLE_URL2, _TEST_TENTACLE_BIND_IP2,
	   _TEST_TENTACLE_PASSWORD, _TEST_TENTACLE_ID1, _TEST_TENTACLE_ID2, _TEST_TENTACLE_NETWORK_RELIABILITY))
	{
	  while ((now = lw6sys_get_timestamp (sys_context)) < end_timestamp && !(*(data->done)))
	    {
	      lw6sys_idle (sys_context);
	      logical_ticket_sig =
		lw6msg_ticket_calc_sig (sys_context, lw6cnx_ticket_table_get_send
					(sys_context, &ticket_table,
					 _TEST_TENTACLE_ID2_STR), _TEST_TENTACLE_ID1, _TEST_TENTACLE_ID2, _TEST_TENTACLE_SEND_BEST_UNRELIABLE_STR1);
	      if (_lw6p2p_tentacle_send_best
		  (sys_context, &(data->tentacle), lw6sys_get_timestamp (sys_context),
		   &ticket_table, logical_ticket_sig, _TEST_TENTACLE_ID1, _TEST_TENTACLE_ID2, _TEST_TENTACLE_SEND_BEST_UNRELIABLE_STR1, 0))
		{
		  nb_sent_best_unreliable++;
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			      _x_ ("tentacle 1 couldn't sent \"%s\" (best unreliable)"), _TEST_TENTACLE_SEND_BEST_UNRELIABLE_STR1);
		}
	      logical_ticket_sig =
		lw6msg_ticket_calc_sig (sys_context, lw6cnx_ticket_table_get_send
					(sys_context, &ticket_table,
					 _TEST_TENTACLE_ID2_STR), _TEST_TENTACLE_ID1, _TEST_TENTACLE_ID2, _TEST_TENTACLE_SEND_BEST_RELIABLE_STR1);
	      if (_lw6p2p_tentacle_send_best
		  (sys_context, &(data->tentacle), lw6sys_get_timestamp (sys_context),
		   &ticket_table, logical_ticket_sig, _TEST_TENTACLE_ID1, _TEST_TENTACLE_ID2, _TEST_TENTACLE_SEND_BEST_RELIABLE_STR1, 1))
		{
		  nb_sent_best_reliable++;
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("tentacle 1 couldn't send \"%s\" (best reliable)"),
			      _TEST_TENTACLE_SEND_BEST_RELIABLE_STR1);
		}
	      logical_ticket_sig =
		lw6msg_ticket_calc_sig (sys_context, lw6cnx_ticket_table_get_send
					(sys_context, &ticket_table, _TEST_TENTACLE_ID2_STR), _TEST_TENTACLE_ID1, _TEST_TENTACLE_ID2,
					_TEST_TENTACLE_SEND_REDUNDANT_STR1);
	      if (_lw6p2p_tentacle_send_redundant
		  (sys_context, &(data->tentacle), lw6sys_get_timestamp (sys_context), &ticket_table, logical_ticket_sig, _TEST_TENTACLE_ID1,
		   _TEST_TENTACLE_ID2, _TEST_TENTACLE_SEND_REDUNDANT_STR1))
		{
		  nb_sent_redundant++;
		}
	      else
		{
		  if (nb_sent_redundant == 0)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
				  _x_ ("tentacle 1 couldn't send \"%s\" (redundant), but as it is the first time, might just require some time to work"),
				  _TEST_TENTACLE_SEND_REDUNDANT_STR1);
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("tentacle 1 couldn't send \"%s\" (redundant), but it could do it before"),
				  _TEST_TENTACLE_SEND_REDUNDANT_STR1);
		    }
		}

	      _test_tentacle_poll_step1_accept_tcp (sys_context, data->listener, now);
	      _test_tentacle_poll_step2_recv_udp (sys_context, data->listener, now);
	      _test_tentacle_poll_step3_reply (sys_context, data->listener, now, data);
	      _lw6p2p_tentacle_poll_queues (sys_context, NULL, &(data->tentacle));

	      /*
	       * Snoozing a bit to avoid filling up buffers
	       * with junk too fast, we're not testing bandwidth
	       * here, only the basic "how does data sending works
	       * when everything looks OK".
	       */
	      lw6sys_snooze (sys_context);
	      // todo...
	    }

	  if (LW6SYS_TEST_ACK (nb_sent_best_unreliable > 0) && LW6SYS_TEST_ACK (nb_sent_best_reliable > 0) && LW6SYS_TEST_ACK (nb_sent_redundant > 0))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			  _x_
			  ("sent data nb_sent_best_unreliable=%d nb_sent_best_reliable=%d nb_sent_redundant=%d"),
			  nb_sent_best_unreliable, nb_sent_best_reliable, nb_sent_redundant);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			  _x_
			  ("error sending data nb_sent_best_unreliable=%d nb_sent_best_reliable=%d nb_sent_redundant=%d"),
			  nb_sent_best_unreliable, nb_sent_best_reliable, nb_sent_redundant);
	      data->ret = 0;
	    }

	  _lw6p2p_tentacle_clear (sys_context, &(data->tentacle));
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to init tentacle1"));
	  data->ret = 0;
	}
      lw6cnx_ticket_table_clear (sys_context, &ticket_table);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to init ticket table 1"));
      data->ret = 0;
    }
}

static void
_test_tentacle2_thread_callback (lw6sys_context_t * sys_context, void *tentacle_data)
{
  _test_tentacle_data_t *data = (_test_tentacle_data_t *) tentacle_data;
  int64_t end_timestamp = 0LL;
  lw6cnx_ticket_table_t ticket_table;
  u_int32_t logical_ticket_sig = 0;
  int nb_sent_best_reliable = 0;
  int nb_sent_best_unreliable = 0;
  int nb_sent_redundant = 0;
  int64_t now = 0LL;

  end_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_TENTACLE_DURATION_THREAD;

  data->ret = 1;

  lw6cnx_ticket_table_zero (sys_context, &ticket_table);
  if (lw6cnx_ticket_table_init (sys_context, &ticket_table, _TEST_TENTACLE_TICKET_TABLE_HASH_SIZE))
    {
      if (_lw6p2p_tentacle_init
	  (sys_context, &(data->tentacle), &(data->backends), data->listener,
	   _TEST_TENTACLE_URL2, _TEST_TENTACLE_URL1, _TEST_TENTACLE_BIND_IP1,
	   _TEST_TENTACLE_PASSWORD, _TEST_TENTACLE_ID2, _TEST_TENTACLE_ID1, _TEST_TENTACLE_NETWORK_RELIABILITY))
	{
	  while ((now = lw6sys_get_timestamp (sys_context)) < end_timestamp && !(*(data->done)))
	    {
	      lw6sys_idle (sys_context);
	      logical_ticket_sig =
		lw6msg_ticket_calc_sig (sys_context, lw6cnx_ticket_table_get_send
					(sys_context, &ticket_table,
					 _TEST_TENTACLE_ID1_STR), _TEST_TENTACLE_ID2, _TEST_TENTACLE_ID1, _TEST_TENTACLE_SEND_BEST_UNRELIABLE_STR2);
	      if (_lw6p2p_tentacle_send_best
		  (sys_context, &(data->tentacle), lw6sys_get_timestamp (sys_context),
		   &ticket_table, logical_ticket_sig, _TEST_TENTACLE_ID2, _TEST_TENTACLE_ID1, _TEST_TENTACLE_SEND_BEST_UNRELIABLE_STR2, 0))
		{
		  nb_sent_best_unreliable++;
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			      _x_ ("tentacle 2 couldn't send \"%s\" (best unreliable)"), _TEST_TENTACLE_SEND_BEST_UNRELIABLE_STR2);
		}
	      logical_ticket_sig =
		lw6msg_ticket_calc_sig (sys_context, lw6cnx_ticket_table_get_send
					(sys_context, &ticket_table,
					 _TEST_TENTACLE_ID1_STR), _TEST_TENTACLE_ID2, _TEST_TENTACLE_ID1, _TEST_TENTACLE_SEND_BEST_RELIABLE_STR2);
	      if (_lw6p2p_tentacle_send_best
		  (sys_context, &(data->tentacle), lw6sys_get_timestamp (sys_context),
		   &ticket_table, logical_ticket_sig, _TEST_TENTACLE_ID2, _TEST_TENTACLE_ID1, _TEST_TENTACLE_SEND_BEST_RELIABLE_STR2, 1))
		{
		  nb_sent_best_reliable++;
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("tentacle 2 couldn't send \"%s\" (best reliable)"),
			      _TEST_TENTACLE_SEND_BEST_RELIABLE_STR2);
		}
	      logical_ticket_sig =
		lw6msg_ticket_calc_sig (sys_context, lw6cnx_ticket_table_get_send
					(sys_context, &ticket_table, _TEST_TENTACLE_ID1_STR), _TEST_TENTACLE_ID2, _TEST_TENTACLE_ID1,
					_TEST_TENTACLE_SEND_REDUNDANT_STR2);
	      if (_lw6p2p_tentacle_send_redundant
		  (sys_context, &(data->tentacle), lw6sys_get_timestamp (sys_context), &ticket_table, logical_ticket_sig, _TEST_TENTACLE_ID2,
		   _TEST_TENTACLE_ID1, _TEST_TENTACLE_SEND_REDUNDANT_STR2))
		{
		  nb_sent_redundant++;
		}
	      else
		{
		  if (nb_sent_redundant == 0)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
				  _x_ ("tentacle 2 couldn't send \"%s\" (redundant), but as it is the first time, might just require some time to work"),
				  _TEST_TENTACLE_SEND_REDUNDANT_STR2);
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("tentacle 2 couldn't send \"%s\" (redundant), but it could do it before"),
				  _TEST_TENTACLE_SEND_REDUNDANT_STR2);
		    }
		}

	      _test_tentacle_poll_step1_accept_tcp (sys_context, data->listener, now);
	      _test_tentacle_poll_step2_recv_udp (sys_context, data->listener, now);
	      _lw6p2p_tentacle_poll_queues (sys_context, NULL, &(data->tentacle));

	      /*
	       * Snoozing a bit to avoid filling up buffers
	       * with junk too fast, we're not testing bandwidth
	       * here, only the basic "how does data sending works
	       * when everything looks OK".
	       */
	      lw6sys_snooze (sys_context);
	      // todo...
	    }

	  if (LW6SYS_TEST_ACK (nb_sent_best_unreliable > 0) && LW6SYS_TEST_ACK (nb_sent_best_reliable > 0) && LW6SYS_TEST_ACK (nb_sent_redundant > 0))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			  _x_
			  ("sent data nb_sent_best_unreliable=%d nb_sent_best_reliable=%d nb_sent_redundant=%d"),
			  nb_sent_best_unreliable, nb_sent_best_reliable, nb_sent_redundant);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			  _x_
			  ("error sending data nb_sent_best_unreliable=%d nb_sent_best_reliable=%d nb_sent_redundant=%d"),
			  nb_sent_best_unreliable, nb_sent_best_reliable, nb_sent_redundant);
	      data->ret = 0;
	    }

	  _lw6p2p_tentacle_clear (sys_context, &(data->tentacle));
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to init tentacle2"));
	  data->ret = 0;
	}
      lw6cnx_ticket_table_clear (sys_context, &ticket_table);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to init ticket table 2"));
      data->ret = 0;
    }
}

static int
_tentacle_with_backends (lw6sys_context_t * sys_context, char *cli_backends, char *srv_backends)
{
  int ret = 1;
  lw6sys_thread_handler_t *thread1 = NULL;
  lw6sys_thread_handler_t *thread2 = NULL;
  _test_tentacle_data_t tentacle_data1;
  _test_tentacle_data_t tentacle_data2;
  int done = 0;
  const int argc = _TEST_ARGC;
  const char *argv[] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing tentacle with backends \"%s\" and \"%s\""), cli_backends, srv_backends);

  memset (&tentacle_data1, 0, sizeof (_test_tentacle_data_t));
  memset (&tentacle_data2, 0, sizeof (_test_tentacle_data_t));

  if (ret)
    {
      tentacle_data1.listener = lw6srv_start (sys_context, _TEST_TENTACLE_BIND_IP1, _TEST_TENTACLE_BIND_PORT1);
      if (LW6SYS_TEST_ACK (tentacle_data1.listener))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("listener1 listening on ip=%s port=%d"), _TEST_TENTACLE_BIND_IP1, _TEST_TENTACLE_BIND_PORT1);
	  if (LW6SYS_TEST_ACK (_lw6p2p_backends_init_cli (sys_context, argc, argv, &(tentacle_data1.backends), cli_backends)))
	    {
	      if (LW6SYS_TEST_ACK (_lw6p2p_backends_init_srv (sys_context, argc, argv, &(tentacle_data1.backends), srv_backends, tentacle_data1.listener)))
		{
		  tentacle_data1.node_info = lw6nod_info_new (sys_context, _TEST_TENTACLE1_PROGRAM, _TEST_TENTACLE1_VERSION, _TEST_TENTACLE1_CODENAME,
							      _TEST_TENTACLE1_STAMP, _TEST_TENTACLE1_ID, _TEST_TENTACLE1_URL, _TEST_TENTACLE1_TITLE,
							      _TEST_TENTACLE1_DESCRIPTION, _TEST_TENTACLE1_PASSWORD, _TEST_TENTACLE1_BENCH,
							      _TEST_TENTACLE1_OPEN_RELAY, _TEST_TENTACLE1_UPTIME, _TEST_TENTACLE1_IDLE_SCREENSHOT_SIZE,
							      _TEST_TENTACLE1_IDLE_SCREENSHOT_DATA);
		  if (LW6SYS_TEST_ACK (tentacle_data1.node_info != NULL))
		    {
		      // proceed
		    }
		  else
		    {
		      ret = 0;
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to open srv backends1 \"%s\""), srv_backends);
		  ret = 0;
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to open cli backends1 \"%s\""), cli_backends);
	      ret = 0;
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to start listener1 on ip=%s port=%d"), _TEST_TENTACLE_BIND_IP1, _TEST_TENTACLE_BIND_PORT1);
	  ret = 0;
	}
    }

  if (ret)
    {
      tentacle_data2.listener = lw6srv_start (sys_context, _TEST_TENTACLE_BIND_IP2, _TEST_TENTACLE_BIND_PORT2);
      if (LW6SYS_TEST_ACK (tentacle_data2.listener))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("listener2 listening on ip=%s port=%d"), _TEST_TENTACLE_BIND_IP2, _TEST_TENTACLE_BIND_PORT2);
	  if (LW6SYS_TEST_ACK (_lw6p2p_backends_init_cli (sys_context, argc, argv, &(tentacle_data2.backends), cli_backends)))
	    {
	      if (LW6SYS_TEST_ACK (_lw6p2p_backends_init_srv (sys_context, argc, argv, &(tentacle_data2.backends), srv_backends, tentacle_data2.listener)))
		{
		  tentacle_data2.node_info = lw6nod_info_new (sys_context, _TEST_TENTACLE2_PROGRAM, _TEST_TENTACLE2_VERSION, _TEST_TENTACLE2_CODENAME,
							      _TEST_TENTACLE2_STAMP, _TEST_TENTACLE2_ID, _TEST_TENTACLE2_URL, _TEST_TENTACLE2_TITLE,
							      _TEST_TENTACLE2_DESCRIPTION, _TEST_TENTACLE2_PASSWORD, _TEST_TENTACLE2_BENCH,
							      _TEST_TENTACLE2_OPEN_RELAY, _TEST_TENTACLE2_UPTIME, _TEST_TENTACLE2_IDLE_SCREENSHOT_SIZE,
							      _TEST_TENTACLE2_IDLE_SCREENSHOT_DATA);
		  if (LW6SYS_TEST_ACK (tentacle_data2.node_info != NULL))
		    {
		      // proceed
		    }
		  else
		    {
		      ret = 0;
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to open srv backends2 \"%s\""), srv_backends);
		  ret = 0;
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to open cli backends2 \"%s\""), cli_backends);
	      ret = 0;
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to start listener2 on ip=%s port=%d"), _TEST_TENTACLE_BIND_IP2, _TEST_TENTACLE_BIND_PORT2);
	  ret = 0;
	}
    }

  if (ret)
    {
      thread1 = lw6sys_thread_create (sys_context, _test_tentacle1_thread_callback, NULL, (void *) &tentacle_data1);
      thread2 = lw6sys_thread_create (sys_context, _test_tentacle2_thread_callback, NULL, (void *) &tentacle_data2);

      tentacle_data1.done = &done;
      tentacle_data2.done = &done;

      lw6sys_progress_default (sys_context, &(tentacle_data1.progress), &(tentacle_data1.progress_value));
      lw6sys_progress_default (sys_context, &(tentacle_data2.progress), &(tentacle_data2.progress_value));
      lw6sys_progress_begin (sys_context, &(tentacle_data1.progress));
      lw6sys_progress_begin (sys_context, &(tentacle_data2.progress));

      if (LW6SYS_TEST_ACK (thread1 && thread2))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("each tentacle running in its own thread"));
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to start all threads"));
	  ret = 0;
	}

      if (thread1)
	{
	  lw6sys_thread_join (sys_context, thread1);
	}
      if (thread2)
	{
	  lw6sys_thread_join (sys_context, thread2);
	}
    }

  _lw6p2p_backends_clear_srv (sys_context, &(tentacle_data1.backends));
  _lw6p2p_backends_clear_cli (sys_context, &(tentacle_data1.backends));
  _lw6p2p_backends_clear_srv (sys_context, &(tentacle_data2.backends));
  _lw6p2p_backends_clear_cli (sys_context, &(tentacle_data2.backends));
  if (tentacle_data1.node_info)
    {
      lw6nod_info_free (sys_context, tentacle_data1.node_info);
    }
  if (tentacle_data2.node_info)
    {
      lw6nod_info_free (sys_context, tentacle_data2.node_info);
    }
  if (tentacle_data1.listener)
    {
      lw6srv_stop (sys_context, tentacle_data1.listener);
      tentacle_data1.listener = NULL;
    }
  if (tentacle_data2.listener)
    {
      lw6srv_stop (sys_context, tentacle_data2.listener);
      tentacle_data2.listener = NULL;
    }
  ret = ret && tentacle_data1.ret && tentacle_data2.ret;
  if (LW6SYS_TEST_ACK (ret))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("all tentacles completed their tests successfully"));
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_
		  ("at least one tentacle failed to complete its test, results are tentacle_data1.ret=%d, tentacle_data2.ret=%d"),
		  tentacle_data1.ret, tentacle_data2.ret);
    }

  return ret;
}

/*
 * Testing tentacle
 */
static void
_test_tentacle ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    ret = ret && _tentacle_with_backends (sys_context, "tcp", "tcpd");
    ret = ret && _tentacle_with_backends (sys_context, "udp", "udpd");
    _tentacle_with_backends (sys_context, "http", "httpd");	// even if fails, keep going since http is optional
    ret = ret && _tentacle_with_backends (sys_context, lw6cli_default_backends (sys_context), lw6srv_default_backends (sys_context));
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing node initialisation
 */
static void
_test_node_init ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

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

    db = lw6p2p_db_open (sys_context, argc, argv, _TEST_DB_NAME12);
    if (LW6SYS_TEST_ACK (db))
      {
	/*
	 * Passing 0LL as a node id will cause automatic generation of an id
	 */
	node =
	  lw6p2p_node_new (sys_context, argc, argv, db, lw6cli_default_backends (sys_context),
			   lw6srv_default_backends (sys_context), _TEST_NODE_BIND_IP,
			   _TEST_NODE_BIND_PORT1, _TEST_NODE_BROADCAST,
			   0LL,
			   _TEST_NODE_PUBLIC_URL1,
			   _TEST_NODE_TITLE1, _TEST_NODE_DESCRIPTION, NULL,
			   _TEST_NODE_BENCH, _TEST_NODE_OPEN_RELAY, _TEST_NODE_KNOWN_NODES1, _TEST_NODE_NETWORK_RELIABILITY, _TEST_NODE_TROJAN);
	if (LW6SYS_TEST_ACK (node))
	  {
	    repr = lw6p2p_node_repr (sys_context, node);
	    if (LW6SYS_TEST_ACK (repr))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("created node \"%s\""), repr);
		LW6SYS_FREE (sys_context, repr);
	      }
	    id_str = lw6sys_id_ltoa (sys_context, lw6p2p_node_get_id (sys_context, node));
	    if (LW6SYS_TEST_ACK (id_str))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("get_id returns %s"), id_str);
		LW6SYS_FREE (sys_context, id_str);
	      }
	    local_seq_0 = lw6p2p_node_get_local_seq_0 (sys_context, node);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("local_seq_0=%" LW6SYS_PRINTF_LL "d"), (long long) local_seq_0);
	    local_seq_last = lw6p2p_node_get_local_seq_last (sys_context, node);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("local_seq_last=%" LW6SYS_PRINTF_LL "d"), (long long) local_seq_last);
	    seq_min = lw6p2p_node_get_seq_min (sys_context, node);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("seq_min=%" LW6SYS_PRINTF_LL "d"), (long long) seq_min);
	    seq_max = lw6p2p_node_get_seq_max (sys_context, node);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("seq_max=%" LW6SYS_PRINTF_LL "d"), (long long) seq_max);
	    seq_draft = lw6p2p_node_get_seq_draft (sys_context, node);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("seq_draft=%" LW6SYS_PRINTF_LL "d"), (long long) seq_draft);
	    seq_reference = lw6p2p_node_get_seq_reference (sys_context, node);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("seq_reference=%" LW6SYS_PRINTF_LL "d"), (long long) seq_reference);
	    lw6p2p_node_close (sys_context, node);
	    lw6p2p_node_close (sys_context, node);	// yes, do it twice just to check
	    lw6p2p_node_free (sys_context, node);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't create node"));
	    ret = 0;
	  }
	lw6p2p_db_close (sys_context, db);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't create db"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Initializes up to 6 nodes
 */
static int
_init_nodes (lw6sys_context_t * sys_context, char *cli_backends, char *srv_backends, lw6p2p_db_t ** db12,
	     lw6p2p_db_t ** db34, lw6p2p_db_t ** db56, lw6p2p_node_t ** node1,
	     lw6p2p_node_t ** node2, lw6p2p_node_t ** node3, lw6p2p_node_t ** node4, lw6p2p_node_t ** node5, lw6p2p_node_t ** node6)
{
  const int argc = _TEST_ARGC;
  const char *argv[] = { _TEST_ARGV0 };
  char *repr = NULL;
  int ret = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init nodes db12=%p db34=%p db56=%p node1=%p node2=%p node3=%p node4=%p node5=%p node6=%p"), db12, db34,
	      db56, node1, node2, node3, node4, node5, node6);

  if (db12)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init db12"));
      /*
       * It's important to reset test DBs before using them,
       * else there could be temporaries IDs mismatches.
       * In real use cases, only our local ID changes,
       * and we really do want something with same URL but
       * different ID than before to somewhat "not connect very well",
       * at least for some time.
       */
      lw6p2p_db_reset (sys_context, argc, argv, _TEST_DB_NAME12);
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("reseted db12"));
      (*db12) = lw6p2p_db_open (sys_context, argc, argv, _TEST_DB_NAME12);
      if (*db12)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("opened db12"));
	  repr = lw6p2p_db_repr (sys_context, *db12);
	  if (LW6SYS_TEST_ACK (repr))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("created db \"%s\""), repr);
	      LW6SYS_FREE (sys_context, repr);
	    }
	}
    }
  if (db34)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init db34"));
      /*
       * It's important to reset test DBs before using them,
       * else there could be temporaries IDs mismatches.
       * In real use cases, only our local ID changes,
       * and we really do want something with same URL but
       * different ID than before to somewhat "not connect very well",
       * at least for some time.
       */
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("reseted db34"));
      lw6p2p_db_reset (sys_context, argc, argv, _TEST_DB_NAME34);
      (*db34) = lw6p2p_db_open (sys_context, argc, argv, _TEST_DB_NAME34);
      if (*db34)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("opened db34"));
	  repr = lw6p2p_db_repr (sys_context, *db34);
	  if (LW6SYS_TEST_ACK (repr))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("created db \"%s\""), repr);
	      LW6SYS_FREE (sys_context, repr);
	    }
	}
    }
  if (db56)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init db56"));
      /*
       * It's important to reset test DBs before using them,
       * else there could be temporaries IDs mismatches.
       * In real use cases, only our local ID changes,
       * and we really do want something with same URL but
       * different ID than before to somewhat "not connect very well",
       * at least for some time.
       */
      lw6p2p_db_reset (sys_context, argc, argv, _TEST_DB_NAME56);
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("reseted db56"));
      (*db56) = lw6p2p_db_open (sys_context, argc, argv, _TEST_DB_NAME56);
      if (*db56)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("opened db56"));
	  repr = lw6p2p_db_repr (sys_context, *db56);
	  if (LW6SYS_TEST_ACK (repr))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("created db \"%s\""), repr);
	      LW6SYS_FREE (sys_context, repr);
	    }
	}
    }
  if (db12 && (*db12))
    {
      if (node1)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init node1"));
	  (*node1) =
	    lw6p2p_node_new (sys_context, argc, argv, *db12, cli_backends,
			     srv_backends, _TEST_NODE_BIND_IP,
			     _TEST_NODE_BIND_PORT1, _TEST_NODE_BROADCAST,
			     lw6sys_generate_id_64 (sys_context),
			     _TEST_NODE_PUBLIC_URL1,
			     _TEST_NODE_TITLE1,
			     _TEST_NODE_DESCRIPTION, _TEST_NODE_PASSWORD,
			     _TEST_NODE_BENCH, _TEST_NODE_OPEN_RELAY, _TEST_NODE_KNOWN_NODES1, _TEST_NODE_NETWORK_RELIABILITY, _TEST_NODE_TROJAN);
	  if (*node1)
	    {
	      repr = lw6p2p_node_repr (sys_context, *node1);
	      if (LW6SYS_TEST_ACK (repr))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("created node1 \"%s\""), repr);
		  LW6SYS_FREE (sys_context, repr);
		}
	    }
	}

      if (node2)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init node2"));
	  (*node2) =
	    lw6p2p_node_new (sys_context, argc, argv, *db12, cli_backends,
			     srv_backends, _TEST_NODE_BIND_IP,
			     _TEST_NODE_BIND_PORT2, _TEST_NODE_BROADCAST,
			     lw6sys_generate_id_64 (sys_context),
			     _TEST_NODE_PUBLIC_URL2,
			     _TEST_NODE_TITLE2, _TEST_NODE_DESCRIPTION, NULL,
			     _TEST_NODE_BENCH, _TEST_NODE_OPEN_RELAY, _TEST_NODE_KNOWN_NODES2, _TEST_NODE_NETWORK_RELIABILITY, _TEST_NODE_TROJAN);
	  if (*node2)
	    {
	      repr = lw6p2p_node_repr (sys_context, *node2);
	      if (LW6SYS_TEST_ACK (repr))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("created node2 \"%s\""), repr);
		  LW6SYS_FREE (sys_context, repr);
		}
	    }
	}
    }
  if (db34 && (*db34))
    {
      if (node3)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init node3"));
	  (*node3) =
	    lw6p2p_node_new (sys_context, argc, argv, *db34, cli_backends,
			     srv_backends, _TEST_NODE_BIND_IP,
			     _TEST_NODE_BIND_PORT3, _TEST_NODE_BROADCAST,
			     lw6sys_generate_id_64 (sys_context),
			     _TEST_NODE_PUBLIC_URL3,
			     _TEST_NODE_TITLE3,
			     _TEST_NODE_DESCRIPTION, _TEST_NODE_PASSWORD,
			     _TEST_NODE_BENCH, _TEST_NODE_OPEN_RELAY, _TEST_NODE_KNOWN_NODES3, _TEST_NODE_NETWORK_RELIABILITY, _TEST_NODE_TROJAN);
	  if (*node3)
	    {
	      repr = lw6p2p_node_repr (sys_context, *node3);
	      if (LW6SYS_TEST_ACK (repr))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("created node3 \"%s\""), repr);
		  LW6SYS_FREE (sys_context, repr);
		}
	    }
	}

      if (node4)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init node4"));
	  (*node4) =
	    lw6p2p_node_new (sys_context, argc, argv, *db34, cli_backends,
			     srv_backends, _TEST_NODE_BIND_IP,
			     _TEST_NODE_BIND_PORT4, _TEST_NODE_BROADCAST,
			     lw6sys_generate_id_64 (sys_context),
			     _TEST_NODE_PUBLIC_URL4,
			     _TEST_NODE_TITLE4, _TEST_NODE_DESCRIPTION, NULL,
			     _TEST_NODE_BENCH, _TEST_NODE_OPEN_RELAY, _TEST_NODE_KNOWN_NODES4, _TEST_NODE_NETWORK_RELIABILITY, _TEST_NODE_TROJAN);
	  if (*node4)
	    {
	      repr = lw6p2p_node_repr (sys_context, *node4);
	      if (LW6SYS_TEST_ACK (repr))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("created node4 \"%s\""), repr);
		  LW6SYS_FREE (sys_context, repr);
		}
	    }
	}
    }

  if (db56 && (*db56))
    {
      if (node5)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init node5"));
	  (*node5) =
	    lw6p2p_node_new (sys_context, argc, argv, *db56, cli_backends,
			     srv_backends, _TEST_NODE_BIND_IP,
			     _TEST_NODE_BIND_PORT5, _TEST_NODE_BROADCAST,
			     lw6sys_generate_id_64 (sys_context),
			     _TEST_NODE_PUBLIC_URL5,
			     _TEST_NODE_TITLE5,
			     _TEST_NODE_DESCRIPTION, NULL,
			     _TEST_NODE_BENCH, _TEST_NODE_OPEN_RELAY, _TEST_NODE_KNOWN_NODES5, _TEST_NODE_NETWORK_RELIABILITY, _TEST_NODE_TROJAN);
	  if (*node5)
	    {
	      repr = lw6p2p_node_repr (sys_context, *node5);
	      if (LW6SYS_TEST_ACK (repr))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("created node5 \"%s\""), repr);
		  LW6SYS_FREE (sys_context, repr);
		}
	    }
	}

      if (node6)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init node6"));
	  (*node6) =
	    lw6p2p_node_new (sys_context, argc, argv, *db56, cli_backends,
			     srv_backends, _TEST_NODE_BIND_IP,
			     _TEST_NODE_BIND_PORT6, _TEST_NODE_BROADCAST,
			     lw6sys_generate_id_64 (sys_context),
			     _TEST_NODE_PUBLIC_URL6,
			     _TEST_NODE_TITLE6, _TEST_NODE_DESCRIPTION, NULL,
			     _TEST_NODE_BENCH, _TEST_NODE_OPEN_RELAY, _TEST_NODE_KNOWN_NODES6, _TEST_NODE_NETWORK_RELIABILITY, _TEST_NODE_TROJAN);
	  if (*node6)
	    {
	      repr = lw6p2p_node_repr (sys_context, *node6);
	      if (LW6SYS_TEST_ACK (repr))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("created node6 \"%s\""), repr);
		  LW6SYS_FREE (sys_context, repr);
		}
	    }
	}
    }

  if (((!node1) || (node1 && *node1))
      && ((!node2) || (node2 && *node2)) && ((!node3) || (node3 && *node3))
      && ((!node4) || (node4 && *node4)) && ((!node5) || (node5 && *node5)) && ((!node6) || (node6 && *node6)))
    {
      ret = 1;
    }
  else
    {
      if (node1 && *node1)
	{
	  lw6p2p_node_free (sys_context, *node1);
	  (*node1) = NULL;
	}
      if (node2 && *node2)
	{
	  lw6p2p_node_free (sys_context, *node2);
	  (*node2) = NULL;
	}
      if (node3 && *node3)
	{
	  lw6p2p_node_free (sys_context, *node3);
	  (*node3) = NULL;
	}
      if (node4 && *node4)
	{
	  lw6p2p_node_free (sys_context, *node4);
	  (*node4) = NULL;
	}
      if (node5 && *node5)
	{
	  lw6p2p_node_free (sys_context, *node4);
	  (*node4) = NULL;
	}
      if (node6 && *node6)
	{
	  lw6p2p_node_free (sys_context, *node6);
	  (*node6) = NULL;
	}
      if (db12 && *db12)
	{
	  lw6p2p_db_close (sys_context, *db12);
	  (*db12) = NULL;
	}
      if (db34 && *db34)
	{
	  lw6p2p_db_close (sys_context, *db34);
	  (*db34) = NULL;
	}
      if (db56 && *db56)
	{
	  lw6p2p_db_close (sys_context, *db56);
	  (*db56) = NULL;
	}
    }

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init nodes done ret=%d"), ret);

  return ret;
}

static void
_quit_nodes (lw6sys_context_t * sys_context, lw6p2p_db_t * db12, lw6p2p_db_t * db34, lw6p2p_db_t * db56,
	     lw6p2p_node_t * node1, lw6p2p_node_t * node2, lw6p2p_node_t * node3, lw6p2p_node_t * node4, lw6p2p_node_t * node5, lw6p2p_node_t * node6)
{
  if (node1)
    {
      lw6p2p_node_close (sys_context, node1);
      lw6p2p_node_free (sys_context, node1);
    }
  if (node2)
    {
      lw6p2p_node_close (sys_context, node2);
      lw6p2p_node_free (sys_context, node2);
    }
  if (node3)
    {
      lw6p2p_node_close (sys_context, node3);
      lw6p2p_node_free (sys_context, node3);
    }
  if (node4)
    {
      lw6p2p_node_close (sys_context, node4);
      lw6p2p_node_free (sys_context, node4);
    }
  if (node5)
    {
      lw6p2p_node_close (sys_context, node5);
      lw6p2p_node_free (sys_context, node5);
    }
  if (node6)
    {
      lw6p2p_node_close (sys_context, node6);
      lw6p2p_node_free (sys_context, node6);
    }
  if (db12)
    {
      lw6p2p_db_close (sys_context, db12);
    }
  if (db34)
    {
      lw6p2p_db_close (sys_context, db34);
    }
  if (db56)
    {
      lw6p2p_db_close (sys_context, db56);
    }
}

static void
_poll_nodes (lw6sys_context_t * sys_context, lw6p2p_node_t * node1, lw6p2p_node_t * node2, lw6p2p_node_t * node3, lw6p2p_node_t * node4, lw6p2p_node_t * node5,
	     lw6p2p_node_t * node6)
{
  int64_t next_timestamp = 00L;

  next_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_NODE_POLL_DURATION;
  while (lw6sys_get_timestamp (sys_context) < next_timestamp)
    {
      if (node1)
	{
	  lw6p2p_node_poll (sys_context, node1, NULL);
	}
      if (node2)
	{
	  lw6p2p_node_poll (sys_context, node2, NULL);
	}
      if (node3)
	{
	  lw6p2p_node_poll (sys_context, node3, NULL);
	}
      if (node4)
	{
	  lw6p2p_node_poll (sys_context, node4, NULL);
	}
      if (node5)
	{
	  lw6p2p_node_poll (sys_context, node5, NULL);
	}
      if (node6)
	{
	  lw6p2p_node_poll (sys_context, node6, NULL);
	}
    }
  lw6sys_idle (sys_context);
}

static void
_print_entry_callback (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  _lw6p2p_node_t *node = (_lw6p2p_node_t *) func_data;
  lw6p2p_entry_t *entry = (lw6p2p_entry_t *) data;
  char *repr = NULL;

  repr = lw6p2p_entry_repr (sys_context, entry);
  if (repr)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node \"%s\" knows entry \"%s\""), node->node_info->const_info.title, repr);
      LW6SYS_FREE (sys_context, repr);
    }
}

/*
 * Testing node connection
 */
static int
_cmd_with_backends (lw6sys_context_t * sys_context, char *cli_backends, char *srv_backends)
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

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing cmd with backends \"%s\" and \"%s\""), cli_backends, srv_backends);

  ret = 0;
  end_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_NODE_CMD_DURATION;
  if (_init_nodes (sys_context, cli_backends, srv_backends, &db12, &db34, &db56, &node1, &node2, &node3, &node4, &node5, &node6))
    {
      if (LW6SYS_TEST_ACK (_lw6p2p_node_register_tentacle
			   (sys_context, (_lw6p2p_node_t *) node1, _TEST_NODE_PUBLIC_URL2,
			    _TEST_NODE_IP2, lw6p2p_node_get_id (sys_context, node2))
			   &&
			   _lw6p2p_node_register_tentacle (sys_context, (_lw6p2p_node_t *)
							   node2,
							   _TEST_NODE_PUBLIC_URL1,
							   _TEST_NODE_IP1,
							   lw6p2p_node_get_id
							   (sys_context, node1))
			   &&
			   _lw6p2p_node_register_tentacle (sys_context, (_lw6p2p_node_t *)
							   node1,
							   _TEST_NODE_PUBLIC_URL3,
							   _TEST_NODE_IP3,
							   lw6p2p_node_get_id
							   (sys_context, node3))
			   &&
			   _lw6p2p_node_register_tentacle (sys_context, (_lw6p2p_node_t *)
							   node3,
							   _TEST_NODE_PUBLIC_URL1,
							   _TEST_NODE_IP1,
							   lw6p2p_node_get_id
							   (sys_context, node1))
			   &&
			   _lw6p2p_node_register_tentacle (sys_context, (_lw6p2p_node_t *)
							   node1,
							   _TEST_NODE_PUBLIC_URL4,
							   _TEST_NODE_IP4,
							   lw6p2p_node_get_id
							   (sys_context, node4))
			   &&
			   _lw6p2p_node_register_tentacle (sys_context, (_lw6p2p_node_t *)
							   node4,
							   _TEST_NODE_PUBLIC_URL1,
							   _TEST_NODE_IP1,
							   lw6p2p_node_get_id
							   (sys_context, node1))
			   &&
			   _lw6p2p_node_register_tentacle (sys_context, (_lw6p2p_node_t *)
							   node4,
							   _TEST_NODE_PUBLIC_URL2,
							   _TEST_NODE_IP2,
							   lw6p2p_node_get_id
							   (sys_context, node2))
			   &&
			   _lw6p2p_node_register_tentacle (sys_context, (_lw6p2p_node_t *)
							   node6,
							   _TEST_NODE_PUBLIC_URL2,
							   _TEST_NODE_IP2,
							   lw6p2p_node_get_id
							   (sys_context, node2))
			   && _lw6p2p_node_register_tentacle (sys_context, (_lw6p2p_node_t *) node5, _TEST_NODE_PUBLIC_URL1, _TEST_NODE_IP1,
							      lw6p2p_node_get_id (sys_context, node1))))
	{
	  while (lw6sys_get_timestamp (sys_context) < end_timestamp)
	    {
	      _poll_nodes (sys_context, node1, node2, node3, node4, node5, node6);
	    }

	  entries = lw6p2p_node_get_entries (sys_context, node1, 0);
	  if (entries)
	    {
	      lw6sys_list_map (sys_context, entries, _print_entry_callback, (void *) node1);
	      lw6sys_list_free (sys_context, entries);
	    }

	  ret = 1;
	}
      _quit_nodes (sys_context, db12, db34, db56, node1, node2, node3, node4, node5, node6);
    }

  return ret;
}

/*
 * Testing node connection
 */
static void
_test_node_cmd ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    ret = ret && _cmd_with_backends (sys_context, "tcp", "tcpd");
    ret = ret && _cmd_with_backends (sys_context, "udp", "udpd");
    _cmd_with_backends (sys_context, "http", "httpd");	// even if fails, keep going since http is optional
    ret = ret && _cmd_with_backends (sys_context, lw6cli_default_backends (sys_context), lw6srv_default_backends (sys_context));
    _cmd_with_backends (sys_context, "", _TEST_BOGUS_BACKEND);
    _cmd_with_backends (sys_context, _TEST_BOGUS_BACKEND, "");
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_oob_with_backends (lw6sys_context_t * sys_context, char *cli_backends, char *srv_backends)
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
  int64_t refresh_timestamp = 0LL;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing oob with backends \"%s\" and \"%s\""), cli_backends, srv_backends);

  ret = 0;
  refresh_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_NODE_REFRESH_DURATION;
  end_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_NODE_OOB_DURATION;
  if (_init_nodes (sys_context, cli_backends, srv_backends, &db12, &db34, &db56, &node1, &node2, &node3, &node4, &node5, &node6))
    {
      ret = 1;

      while (lw6sys_get_timestamp (sys_context) < refresh_timestamp)
	{
	  _poll_nodes (sys_context, node1, node2, node3, node4, node5, node6);
	}
      ret = lw6p2p_node_refresh_peer (sys_context, node1, lw6p2p_node_get_id (sys_context, node2), _TEST_NODE_UNREACHABLE_URL) && ret;
      ret = lw6p2p_node_refresh_peer (sys_context, node2, lw6p2p_node_get_id (sys_context, node3), _TEST_NODE_UNREACHABLE_URL) && ret;
      ret = lw6p2p_node_refresh_peer (sys_context, node3, lw6p2p_node_get_id (sys_context, node4), _TEST_NODE_UNREACHABLE_URL) && ret;
      ret = lw6p2p_node_refresh_peer (sys_context, node4, lw6p2p_node_get_id (sys_context, node5), _TEST_NODE_UNREACHABLE_URL) && ret;
      ret = lw6p2p_node_refresh_peer (sys_context, node5, lw6p2p_node_get_id (sys_context, node6), _TEST_NODE_UNREACHABLE_URL) && ret;
      ret = lw6p2p_node_refresh_peer (sys_context, node6, lw6p2p_node_get_id (sys_context, node1), _TEST_NODE_UNREACHABLE_URL) && ret;
      while (lw6sys_get_timestamp (sys_context) < end_timestamp)
	{
	  _poll_nodes (sys_context, node1, node2, node3, node4, node5, node6);
	}

      _quit_nodes (sys_context, db12, db34, db56, node1, node2, node3, node4, node5, node6);
    }

  return ret;
}

/*
 * Testing node connection
 */
static void
_test_node_oob ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    ret = ret && _oob_with_backends (sys_context, lw6cli_default_backends (sys_context), lw6srv_default_backends (sys_context));
    ret = ret && _oob_with_backends (sys_context, "", lw6srv_default_backends (sys_context));
    ret = ret && _oob_with_backends (sys_context, lw6cli_default_backends (sys_context), "");
    _oob_with_backends (sys_context, "", _TEST_BOGUS_BACKEND);
    _oob_with_backends (sys_context, _TEST_BOGUS_BACKEND, "");
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing node msg
 */
static void
_test_node_msg ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

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
    lw6sys_progress_default (sys_context, &progress, &progress_value);

    db = lw6p2p_db_open (sys_context, argc, argv, _TEST_DB_NAME12);
    if (LW6SYS_TEST_ACK (db))
      {
	/*
	 * Passing 0LL as a node id will cause automatic generation of an id
	 */
	node =
	  lw6p2p_node_new (sys_context, argc, argv, db, lw6cli_default_backends (sys_context),
			   lw6srv_default_backends (sys_context), _TEST_NODE_BIND_IP,
			   _TEST_NODE_BIND_PORT1, _TEST_NODE_BROADCAST,
			   0LL,
			   _TEST_NODE_PUBLIC_URL1,
			   _TEST_NODE_TITLE1, _TEST_NODE_DESCRIPTION, NULL,
			   _TEST_NODE_BENCH, _TEST_NODE_OPEN_RELAY, _TEST_NODE_KNOWN_NODES1, _TEST_NODE_NETWORK_RELIABILITY, _TEST_NODE_TROJAN);
	if (LW6SYS_TEST_ACK (node))
	  {
	    node_id = lw6p2p_node_get_id (sys_context, node);
	    seq = _TEST_NODE_MSG_SEQ_0;
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("step 1, seq_max=%" LW6SYS_PRINTF_LL "d"), (long long) seq);
	    for (i = 0, k = 0; i < _TEST_NODE_MSG_NB_SEQS; ++i)
	      {
		seq++;		// fake we're going next round...
		for (j = 0; j < _TEST_NODE_MSG_NB_PER_SEQ; ++j, ++k)
		  {
		    random_str = lw6sys_str_random_words (sys_context, lw6sys_random (sys_context, _TEST_NODE_MSG_RANDOM_STR_SIZE) + 1);
		    if (LW6SYS_TEST_ACK (random_str))
		      {
			msg =
			  lw6sys_new_sprintf (sys_context,
					      "%" LW6SYS_PRINTF_LL "d %" LW6SYS_PRINTF_LL "x k=%d %s", (long long) seq, (long long) node_id, k, random_str);
			if (LW6SYS_TEST_ACK (msg))
			  {
			    checksums[k] = lw6sys_checksum_str (sys_context, msg);
			    msgs[k] = msg;
			    lw6p2p_node_put_local_msg (sys_context, node, msg);
			  }
			LW6SYS_FREE (sys_context, random_str);
		      }
		  }
	      }
	    seq = lw6p2p_node_get_seq_max (sys_context, node);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("step 2, seq_max=%" LW6SYS_PRINTF_LL "d"), (long long) seq);
	    k = 0;
	    while ((msg = lw6p2p_node_get_next_draft_msg (sys_context, node, &progress)) != NULL)
	      {
		checksum = lw6sys_checksum_str (sys_context, msg);
		/*
		 * We need to loop over all received messages for the order might
		 * have change, there's an alphabetical sort on the messages for
		 * a same seq & source.
		 */
		found = 0;
		for (i = 0; i < _TEST_NODE_MSG_NB_SEQS * _TEST_NODE_MSG_NB_PER_SEQ; ++i)
		  {
		    if (checksum == checksums[i] && lw6sys_str_is_same (sys_context, msg, msgs[i]) && !found)
		      {
			found = 1;
		      }
		  }
		if (!LW6SYS_TEST_ACK (found))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to find message for k=%d, len=%d msg=\"%s\""), (int) k, (int) strlen (msg), msg);
		    ret = 0;
		  }
		LW6SYS_FREE (sys_context, msg);
		++k;
	      }
	    if (LW6SYS_TEST_ACK (k == _TEST_NODE_MSG_NB_SEQS * _TEST_NODE_MSG_NB_PER_SEQ))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("got %d draft messages"), k);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_ ("only got %d draft messages, needed %d"), k, _TEST_NODE_MSG_NB_SEQS * _TEST_NODE_MSG_NB_PER_SEQ);
		ret = 0;
	      }
	    for (k = 0; k < _TEST_NODE_MSG_NB_SEQS * _TEST_NODE_MSG_NB_PER_SEQ; ++k)
	      {
		if (msgs[k])
		  {
		    LW6SYS_FREE (sys_context, msgs[k]);
		  }
	      }

	    lw6p2p_node_free (sys_context, node);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't create node"));
	    ret = 0;
	  }
	lw6p2p_db_close (sys_context, db);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't create db"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
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
_test_node_api_node1_callback (lw6sys_context_t * sys_context, void *api_data)
{
  _test_node_api_data_t *data = (_test_node_api_data_t *) api_data;
  int64_t end_timestamp = 0LL;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("begin node api test 1"));

  end_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_NODE_API_DURATION_THREAD;

  data->ret = 1;

  while (lw6sys_get_timestamp (sys_context) < end_timestamp && !(*(data->done)))
    {
      lw6p2p_node_poll (sys_context, data->node, &(data->progress));
      lw6sys_idle (sys_context);
    }

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("end node api test 1 data->ret=%d"), data->ret);
}

static void
_test_node_api_node2_callback (lw6sys_context_t * sys_context, void *api_data)
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

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("begin node api test 2"));

  begin_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_NODE_ACK_DELAY_MSEC;
  end_timestamp = begin_timestamp + _TEST_NODE_API_DURATION_THREAD;

  /*
   * This node acts as a server.
   */
  data->ret = lw6p2p_node_server_start (sys_context, data->node, _TEST_NODE_API_SEQ_0);
  if (data->ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("\"%s\" is ready"), ((_lw6p2p_node_t *) data->node)->public_url);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("could not start \"%s\""), ((_lw6p2p_node_t *) data->node)->public_url);
    }

  node_id = lw6p2p_node_get_id (sys_context, data->node);
  seq = lw6p2p_node_get_seq_max (sys_context, data->node);
  seq++;			// fake we're going next round...
  while (lw6sys_get_timestamp (sys_context) < begin_timestamp)
    {
      lw6p2p_node_poll (sys_context, data->node, &(data->progress));
      lw6sys_idle (sys_context);
    }
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node2 before loop seq_max=%" LW6SYS_PRINTF_LL "d"), (long long) seq);
  while (lw6sys_get_timestamp (sys_context) < end_timestamp && !(*(data->done)))
    {
      lw6p2p_node_poll (sys_context, data->node, &(data->progress));
      lw6sys_idle (sys_context);
      if (lw6p2p_node_is_dump_needed (sys_context, data->node))
	{
	  dump = lw6sys_str_random_words (sys_context, _TEST_NODE_API_DUMP_SIZE);
	  if (dump)
	    {
	      msg = lw6sys_new_sprintf (sys_context, "%" LW6SYS_PRINTF_LL "d %" LW6SYS_PRINTF_LL "x %s", (long long) seq, (long long) node_id, dump);
	      if (msg)
		{
		  len = strlen (msg);
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node2 sent dump draft message len=%d"), len);
		  if (len < _TEST_NODE_API_LEN_LIMIT)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node2 dump draft message is \"%s\""), msg);
		    }
		  lw6p2p_node_put_local_msg (sys_context, data->node, msg);
		  LW6SYS_FREE (sys_context, msg);
		}
	      msg =
		lw6sys_new_sprintf (sys_context, "%" LW6SYS_PRINTF_LL "d %"
				    LW6SYS_PRINTF_LL "x %s", (long long) seq, (long long) node_id, _TEST_NODE_API_MSG1_STR);
	      if (msg)
		{
		  len = strlen (msg);
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node2 sent draft message len=%d"), len);
		  if (len < _TEST_NODE_API_LEN_LIMIT)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node2 draft message is \"%s\""), msg);
		    }
		  lw6p2p_node_put_local_msg (sys_context, data->node, msg);
		  LW6SYS_FREE (sys_context, msg);
		}
	      seq++;		// fake we're going next round...
	      msg =
		lw6sys_new_sprintf (sys_context, "%" LW6SYS_PRINTF_LL "d %"
				    LW6SYS_PRINTF_LL "x %s", (long long) seq, (long long) node_id, _TEST_NODE_API_MSG2_STR);
	      if (msg)
		{
		  len = strlen (msg);
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node2 sent draft message len=%d"), len);
		  if (len < _TEST_NODE_API_LEN_LIMIT)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node2 draft message is \"%s\""), msg);
		    }
		  lw6p2p_node_put_local_msg (sys_context, data->node, msg);
		  LW6SYS_FREE (sys_context, msg);
		}
	      msg =
		lw6sys_new_sprintf (sys_context, "%" LW6SYS_PRINTF_LL "d %"
				    LW6SYS_PRINTF_LL "x %s", (long long) seq, (long long) node_id, _TEST_NODE_API_MSG3_STR);
	      if (msg)
		{
		  len = strlen (msg);
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node2 sent draft message len=%d"), len);
		  if (len < _TEST_NODE_API_LEN_LIMIT)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node2 draft message is \"%s\""), msg);
		    }
		  lw6p2p_node_put_local_msg (sys_context, data->node, msg);
		  LW6SYS_FREE (sys_context, msg);
		}

	      /*
	       * The last message we send is a long one. In real usage one
	       * can obviously send shorter messages, but the idea is that when
	       * testing, sending partial message will trigger the MISS message
	       * automatically. Else if the last message is a short one, MISS is
	       * never sent, just because, well, receiver can really not guess
	       * something is missing
	       */
	      tmp = lw6sys_str_random_word (sys_context, _TEST_NODE_API_FINAL_SIZE);
	      if (tmp)
		{
		  msg = lw6sys_new_sprintf (sys_context, "%" LW6SYS_PRINTF_LL "d %" LW6SYS_PRINTF_LL "x %s", (long long) seq, (long long) node_id, tmp);
		  LW6SYS_FREE (sys_context, tmp);
		}
	      if (msg)
		{
		  len = strlen (msg);
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node2 sent final draft message len=%d"), len);
		  if (len < _TEST_NODE_API_LEN_LIMIT)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node2 final draft message is \"%s\""), msg);
		    }
		  lw6p2p_node_put_local_msg (sys_context, data->node, msg);
		  LW6SYS_FREE (sys_context, msg);
		}

	      LW6SYS_FREE (sys_context, dump);
	      dump_sent = 1;
	    }
	}

      if (dump_sent)
	{
	  if (next_msgn_timestamp < lw6sys_get_timestamp (sys_context))
	    {
	      seq++;		// fake we're going next round...
	      msg =
		lw6sys_new_sprintf (sys_context, "%" LW6SYS_PRINTF_LL "d %"
				    LW6SYS_PRINTF_LL "x %s", (long long) seq, (long long) node_id, _TEST_NODE_API_MSGN_STR);
	      if (msg)
		{
		  len = strlen (msg);
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node2 sent draft message len=%d"), len);
		  if (len < _TEST_NODE_API_LEN_LIMIT)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node2 draft message is \"%s\""), msg);
		    }
		  lw6p2p_node_put_local_msg (sys_context, data->node, msg);
		  LW6SYS_FREE (sys_context, msg);
		}
	      next_msgn_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_NODE_API_MSGN_DELAY_MSEC;
	    }
	}
    }

  if (data->ret)
    {
      lw6p2p_node_disconnect (sys_context, data->node);
    }

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("end node api test 2 data->ret=%d"), data->ret);
}

static void
_test_node_api_node3_callback (lw6sys_context_t * sys_context, void *api_data)
{
  _test_node_api_data_t *data = (_test_node_api_data_t *) api_data;
  int64_t end_timestamp = 0LL;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("begin node api test 3"));

  data->ret = 1;

  end_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_NODE_API_DURATION_THREAD;
  while (lw6sys_get_timestamp (sys_context) < end_timestamp && !(*(data->done)))
    {
      lw6p2p_node_poll (sys_context, data->node, &(data->progress));
      lw6sys_idle (sys_context);
    }

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("end node api test 3 data->ret=%d"), data->ret);
}

static void
_test_node_api_node4_callback (lw6sys_context_t * sys_context, void *api_data)
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
  int join_ret = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("begin node api test 4"));

  begin_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_NODE_ACK_DELAY_MSEC;
  end_timestamp = begin_timestamp + _TEST_NODE_API_DURATION_THREAD;
  data->ret = 0;

  /*
   * This node acts as a client.
   */
  lw6sys_delay (sys_context, _TEST_NODE_API_DURATION_JOIN);
  join_ret = lw6p2p_node_client_join (sys_context, data->node, data->peer_id, _TEST_NODE_PUBLIC_URL2, &(data->progress));
  if (LW6SYS_TEST_ACK (join_ret))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("\"%s\" joined \"%s\""), ((_lw6p2p_node_t *) data->node)->public_url, _TEST_NODE_PUBLIC_URL2);

      while (lw6sys_get_timestamp (sys_context) < begin_timestamp)
	{
	  lw6p2p_node_poll (sys_context, data->node, &(data->progress));
	  lw6sys_idle (sys_context);
	}

      seq = lw6p2p_node_get_seq_max (sys_context, data->node);
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node4 before loop seq_max=%" LW6SYS_PRINTF_LL "d"), (long long) seq);
      while (lw6sys_get_timestamp (sys_context) < end_timestamp && !(*(data->done)))
	{
	  lw6p2p_node_poll (sys_context, data->node, &(data->progress));
	  lw6sys_idle (sys_context);

	  while ((msg = lw6p2p_node_get_next_reference_msg (sys_context, data->node, &(data->progress))) != NULL)
	    {
	      len = strlen (msg);
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node4 received reference message len=%d"), len);
	      if (len < _TEST_NODE_API_LEN_LIMIT)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node4 reference message is \"%s\""), msg);
		}
	      ++reference_received;
	      LW6SYS_FREE (sys_context, msg);
	    }
	  while ((msg = lw6p2p_node_get_next_draft_msg (sys_context, data->node, &(data->progress))) != NULL)
	    {
	      len = strlen (msg);
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node4 received draft message len=%d"), len);
	      if (len < _TEST_NODE_API_LEN_LIMIT)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node4 draft message is \"%s\""), msg);
		  ++short_draft_received;
		}
	      else
		{
		  ++long_draft_received;
		}
	      LW6SYS_FREE (sys_context, msg);
	    }
	  if (reference_received >= _TEST_NODE_API_NODE4_REFERENCE_RECEIVED &&
	      long_draft_received >= _TEST_NODE_API_NODE4_LONG_DRAFT_RECEIVED && short_draft_received >= _TEST_NODE_API_NODE4_SHORT_DRAFT_RECEIVED)
	    {
	      /*
	       * If there are enough messages, consider the test is done,
	       * this does not necessarly means it succeeded, even if there
	       * are good chances it did, but at least it avoids waiting
	       * uselessly for a long time on fast machines.
	       */
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node4, marking test as done"));
	      (*(data->done)) = 1;
	    }
	}
      seq = lw6p2p_node_get_seq_max (sys_context, data->node);
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("node4 after loop seq_max=%" LW6SYS_PRINTF_LL "d"), (long long) seq);
      /*
       * We don't test the draft received value for its exact value since
       * some extra NOP messages might have been sent
       */
      if (LW6SYS_TEST_ACK
	  (reference_received == _TEST_NODE_API_NODE4_REFERENCE_RECEIVED
	   && long_draft_received == _TEST_NODE_API_NODE4_LONG_DRAFT_RECEIVED && short_draft_received >= _TEST_NODE_API_NODE4_LONG_DRAFT_RECEIVED))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		      _x_
		      ("node4, looks like messages made it here, reference_received=%d, long_draft_received=%d, short_draft_received=%d"),
		      reference_received, long_draft_received, short_draft_received);
	  data->ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_
		      ("node4, mismatch in number of received messages reference_received=%d should be %d long_draft_received=%d should be %d short_draft_received=%d should be %d"),
		      reference_received,
		      _TEST_NODE_API_NODE4_REFERENCE_RECEIVED,
		      long_draft_received, _TEST_NODE_API_NODE4_LONG_DRAFT_RECEIVED, short_draft_received, _TEST_NODE_API_NODE4_SHORT_DRAFT_RECEIVED);
	}
      lw6p2p_node_disconnect (sys_context, data->node);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("\"%s\" couldn't join \"%s\""), ((_lw6p2p_node_t *) data->node)->public_url, _TEST_NODE_PUBLIC_URL2);
    }

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("end node api test 4 data->ret=%d"), data->ret);
}

static void
_test_node_api_node5_callback (lw6sys_context_t * sys_context, void *api_data)
{
  _test_node_api_data_t *data = (_test_node_api_data_t *) api_data;
  int64_t end_timestamp = 0LL;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("begin node api test 5"));

  end_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_NODE_API_DURATION_THREAD;

  data->ret = 1;

  while (lw6sys_get_timestamp (sys_context) < end_timestamp && !(*(data->done)))
    {
      lw6p2p_node_poll (sys_context, data->node, &(data->progress));
      lw6sys_idle (sys_context);
    }

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("end node api test 5 data->ret=%d"), data->ret);
}

static void
_test_node_api_node6_callback (lw6sys_context_t * sys_context, void *api_data)
{
  _test_node_api_data_t *data = (_test_node_api_data_t *) api_data;
  int64_t end_timestamp = 0LL;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("begin node api test 6"));

  end_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_NODE_API_DURATION_THREAD;

  data->ret = 1;

  while (lw6sys_get_timestamp (sys_context) < end_timestamp && !(*(data->done)))
    {
      lw6p2p_node_poll (sys_context, data->node, &(data->progress));
      lw6sys_idle (sys_context);
    }

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("end node api test 6 data->ret=%d"), data->ret);
}

static int
_api_with_backends (lw6sys_context_t * sys_context, char *cli_backends, char *srv_backends)
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

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing api with backends \"%s\" and \"%s\""), cli_backends, srv_backends);

  memset (&api_data1, 0, sizeof (_test_node_api_data_t));
  memset (&api_data2, 0, sizeof (_test_node_api_data_t));
  memset (&api_data3, 0, sizeof (_test_node_api_data_t));
  memset (&api_data4, 0, sizeof (_test_node_api_data_t));
  memset (&api_data5, 0, sizeof (_test_node_api_data_t));
  memset (&api_data6, 0, sizeof (_test_node_api_data_t));

  if (_init_nodes (sys_context, cli_backends, srv_backends, &db12, &db34, &db56, &node1, &node2, &node3, &node4, &node5, &node6))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("nodes initialized"));

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
      lw6sys_progress_default (sys_context, &(api_data1.progress), &(api_data1.progress_value));
      lw6sys_progress_default (sys_context, &(api_data2.progress), &(api_data2.progress_value));
      lw6sys_progress_default (sys_context, &(api_data3.progress), &(api_data3.progress_value));
      lw6sys_progress_default (sys_context, &(api_data4.progress), &(api_data4.progress_value));
      lw6sys_progress_default (sys_context, &(api_data5.progress), &(api_data5.progress_value));
      lw6sys_progress_default (sys_context, &(api_data6.progress), &(api_data6.progress_value));
      lw6sys_progress_begin (sys_context, &(api_data1.progress));
      lw6sys_progress_begin (sys_context, &(api_data2.progress));
      lw6sys_progress_begin (sys_context, &(api_data3.progress));
      lw6sys_progress_begin (sys_context, &(api_data4.progress));
      lw6sys_progress_begin (sys_context, &(api_data5.progress));
      lw6sys_progress_begin (sys_context, &(api_data6.progress));

      api_data4.peer_id = lw6p2p_node_get_id (sys_context, node2);

      thread1 = lw6sys_thread_create (sys_context, _test_node_api_node1_callback, NULL, (void *) &api_data1);
      thread2 = lw6sys_thread_create (sys_context, _test_node_api_node2_callback, NULL, (void *) &api_data2);
      thread3 = lw6sys_thread_create (sys_context, _test_node_api_node3_callback, NULL, (void *) &api_data3);
      thread4 = lw6sys_thread_create (sys_context, _test_node_api_node4_callback, NULL, (void *) &api_data4);
      thread5 = lw6sys_thread_create (sys_context, _test_node_api_node5_callback, NULL, (void *) &api_data5);
      thread6 = lw6sys_thread_create (sys_context, _test_node_api_node6_callback, NULL, (void *) &api_data6);

      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("threads created"));

      if (LW6SYS_TEST_ACK (thread1 && thread2 && thread3 && thread4 && thread5 && thread6))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("each node running in its own thread"));
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to start all threads"));
	  ret = 0;
	}
      if (thread1)
	{
	  lw6sys_thread_join (sys_context, thread1);
	}
      if (thread2)
	{
	  lw6sys_thread_join (sys_context, thread2);
	}
      if (thread3)
	{
	  lw6sys_thread_join (sys_context, thread3);
	}
      if (thread4)
	{
	  lw6sys_thread_join (sys_context, thread4);
	}
      if (thread5)
	{
	  lw6sys_thread_join (sys_context, thread5);
	}
      if (thread6)
	{
	  lw6sys_thread_join (sys_context, thread6);
	}
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("waiting for some time to let nodes handle disconnection"));
      end_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_NODE_API_DURATION_END;
      while (lw6sys_get_timestamp (sys_context) < end_timestamp)
	{
	  _poll_nodes (sys_context, node1, node2, node3, node4, node5, node6);
	}

      _quit_nodes (sys_context, db12, db34, db56, node1, node2, node3, node4, node5, node6);

      ret = ret && api_data1.ret && api_data2.ret && api_data3.ret && api_data4.ret && api_data5.ret && api_data6.ret;
      if (LW6SYS_TEST_ACK (ret))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("all nodes completed their tests successfully"));
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_
		      ("at least one node failed to complete its test, results are api_data1.ret=%d, api_data2.ret=%d, api_data3.ret=%d, api_data4.ret=%d, api_data5.ret=%d, api_data6.ret=%d"),
		      api_data1.ret, api_data2.ret, api_data3.ret, api_data4.ret, api_data5.ret, api_data6.ret);
	}
    }

  return ret;
}

/*
 * Testing node api
 */
static void
_test_node_api ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    ret = ret && _api_with_backends (sys_context, "tcp", "tcpd");
    ret = ret && _api_with_backends (sys_context, "udp", "udpd");
    _api_with_backends (sys_context, "http", "httpd");	// even if fails, keep going since http is optional
    ret = ret && _api_with_backends (sys_context, lw6cli_default_backends (sys_context), lw6srv_default_backends (sys_context));
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  int ret = CUE_SINIT_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  const int argc = _TEST_ARGC;
  const char *argv[] = { _TEST_ARGV0 };

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libp2p CUnit test suite"));

  if (lw6net_init (sys_context, argc, argv, _TEST_NET_LOG))
    {
      ret = CUE_SUCCESS;
    }

  return ret;
}

static int
_setup_quit ()
{
  int ret = CUE_SCLEAN_FAILED;
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libp2p CUnit test suite"));

  lw6net_quit (sys_context);
  ret = CUE_SUCCESS;

  return ret;
}

/**
 * lw6p2p_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libp2p module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6p2p_test_register (lw6sys_context_t * sys_context, int mode)
{
  int ret = 1;
  CU_Suite *suite = NULL;

  _test_data.sys_context = sys_context;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test_register (sys_context, mode);
      lw6glb_test_register (sys_context, mode);
      lw6cfg_test_register (sys_context, mode);
      lw6net_test_register (sys_context, mode);
      lw6nod_test_register (sys_context, mode);
      lw6cnx_test_register (sys_context, mode);
      lw6msg_test_register (sys_context, mode);
      lw6cli_test_register (sys_context, mode);
      lw6srv_test_register (sys_context, mode);
      lw6dat_test_register (sys_context, mode);
    }

  suite = CU_add_suite ("lw6p2p", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_db);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_entry);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_tentacle);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_node_init);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_node_oob);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_node_cmd);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_node_msg);
      /*
       * API test is really long and not always bullet-proof, until it's rock
       * stable it's only in full test.
       */
      if (mode & LW6SYS_TEST_MODE_FULL_TEST)
	{
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_node_api);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6p2p_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @p2p module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6p2p_test_run (lw6sys_context_t * sys_context, int mode)
{
  int ret = 0;

  _test_data.ret = 1;
  _test_data.sys_context = sys_context;

  if (lw6sys_cunit_run_tests (sys_context, mode))
    {
      ret = _test_data.ret;
    }

  return ret;
}
