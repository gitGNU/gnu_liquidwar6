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

#define _TEST_ARGC 1
#define _TEST_ARGV0 "prog"
#define _TEST_DB_NAME "test.db"
#define _TEST_NODE_BIND_IP "0.0.0.0"
#define _TEST_NODE_BIND_PORT1 (LW6NET_DEFAULT_PORT + 11)
#define _TEST_NODE_BIND_PORT2 (LW6NET_DEFAULT_PORT + 12)
#define _TEST_NODE_BIND_PORT3 (LW6NET_DEFAULT_PORT + 13)
#define _TEST_NODE_NODE_ID1 0x1234123412341234LL
#define _TEST_NODE_NODE_ID2 0x2345234523452345LL
#define _TEST_NODE_NODE_ID3 0x3456345634563456LL
/*
 * following depends on LW6NET_DEFAULT_PORT
 */
#define _TEST_NODE_PUBLIC_URL1 "http://localhost:8067/"
#define _TEST_NODE_PUBLIC_URL2 "http://localhost:8068/"
#define _TEST_NODE_PUBLIC_URL3 "http://localhost:8069/"
#define _TEST_NODE_PASSWORD "toto"
#define _TEST_NODE_TITLE "This is a node"
#define _TEST_NODE_DESCRIPTION "This is the description of a node."
#define _TEST_NODE_BENCH 10

#define TEST_POLL_LOOPS 1000
#define TEST_POLL_DELAY 10

/* 
 * Testing db
 */
static int
_test_db ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int argc = _TEST_ARGC;
    char *argv[] = { _TEST_ARGV0 };
    lw6p2p_db_t *db = NULL;
    char *repr = NULL;

    lw6sys_log (LW6SYS_LOG_NOTICE,
		_
		("default database name is \"%s\" but we use \"%s\" for some tests"),
		lw6p2p_db_default_name (), _TEST_DB_NAME);

    db = lw6p2p_db_open (argc, argv, _TEST_DB_NAME);
    if (db)
      {
	repr = lw6p2p_db_repr (db);
	if (repr)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _("created db \"%s\""), repr);
	    LW6SYS_FREE (repr);
	  }
	lw6p2p_db_close (db);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("can't create db"));
	ret = 0;
      }

    if (lw6p2p_db_reset (argc, argv, _TEST_DB_NAME))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("deleted db"));
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("can't delete db"));
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
    int argc = _TEST_ARGC;
    char *argv[] = { _TEST_ARGV0 };
    lw6p2p_db_t *db = NULL;
    lw6p2p_node_t *node = NULL;
    char *repr = NULL;

    db = lw6p2p_db_open (argc, argv, lw6p2p_db_default_name ());
    if (db)
      {
	node =
	  lw6p2p_node_new (argc, argv, db, lw6cli_default_backends (),
			   lw6srv_default_backends (), _TEST_NODE_BIND_IP,
			   _TEST_NODE_BIND_PORT1, _TEST_NODE_NODE_ID1,
			   _TEST_NODE_PUBLIC_URL1, NULL, _TEST_NODE_TITLE,
			   _TEST_NODE_DESCRIPTION, _TEST_NODE_BENCH);
	if (node)
	  {
	    repr = lw6p2p_node_repr (node);
	    if (repr)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _("created node \"%s\""),
			    repr);
		LW6SYS_FREE (repr);
	      }
	    lw6p2p_node_close (node);
	    lw6p2p_node_close (node);	// yes, do it twice just to check
	    lw6p2p_node_free (node);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _("can't create node"));
	    ret = 0;
	  }
	lw6p2p_db_close (db);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("can't create db"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/* 
 * Initializes up to 3 nodes
 */
static int
_init_nodes (lw6p2p_db_t * db, void **node1, void **node2, void **node3)
{
  int argc = _TEST_ARGC;
  char *argv[] = { _TEST_ARGV0 };
  char *repr = NULL;
  int ret = 0;

  if (node1)
    {
      (*node1) =
	lw6p2p_node_new (argc, argv, db, lw6cli_default_backends (),
			 lw6srv_default_backends (), _TEST_NODE_BIND_IP,
			 _TEST_NODE_BIND_PORT1, _TEST_NODE_NODE_ID1,
			 _TEST_NODE_PUBLIC_URL1, _TEST_NODE_PASSWORD,
			 _TEST_NODE_TITLE, _TEST_NODE_DESCRIPTION,
			 _TEST_NODE_BENCH);
      if (*node1)
	{
	  repr = lw6p2p_node_repr (*node1);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_NOTICE, _("created node1 \"%s\""), repr);
	      LW6SYS_FREE (repr);
	    }
	}
    }

  if (node2)
    {
      (*node2) =
	lw6p2p_node_new (argc, argv, db, lw6cli_default_backends (),
			 lw6srv_default_backends (), _TEST_NODE_BIND_IP,
			 _TEST_NODE_BIND_PORT2, _TEST_NODE_NODE_ID2,
			 _TEST_NODE_PUBLIC_URL2, _TEST_NODE_PASSWORD,
			 _TEST_NODE_TITLE, _TEST_NODE_DESCRIPTION,
			 _TEST_NODE_BENCH);
      if (*node2)
	{
	  repr = lw6p2p_node_repr (*node2);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_NOTICE, _("created node2 \"%s\""), repr);
	      LW6SYS_FREE (repr);
	    }
	}
    }

  if (node3)
    {
      (*node3) =
	lw6p2p_node_new (argc, argv, db, lw6cli_default_backends (),
			 lw6srv_default_backends (), _TEST_NODE_BIND_IP,
			 _TEST_NODE_BIND_PORT3, _TEST_NODE_NODE_ID3,
			 _TEST_NODE_PUBLIC_URL3, _TEST_NODE_PASSWORD,
			 _TEST_NODE_TITLE, _TEST_NODE_DESCRIPTION,
			 _TEST_NODE_BENCH);
      if (*node3)
	{
	  repr = lw6p2p_node_repr (*node3);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_NOTICE, _("created node3 \"%s\""), repr);
	      LW6SYS_FREE (repr);
	    }
	}
    }

  if (((!node1) || (node1 && *node1))
      && ((!node2) || (node2 && *node2)) && ((!node3) || (node3 && *node3)))
    {
      ret = 1;
    }
  else
    {
      if (node1 && *node1)
	{
	  lw6p2p_node_free (*node1);
	}
      if (*node2 && *node2)
	{
	  lw6p2p_node_free (*node2);
	}
      if (*node3 && *node3)
	{
	  lw6p2p_node_free (*node3);
	}
    }

  return ret;
}

/* 
 * Testing node connection
 */
static int
_test_node_connect ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int argc = _TEST_ARGC;
    char *argv[] = { _TEST_ARGV0 };
    lw6p2p_db_t *db = NULL;
    int i;
    void *node1 = NULL;
    void *node2 = NULL;
    void *node3 = NULL;

    db = lw6p2p_db_open (argc, argv, lw6p2p_db_default_name ());
    if (db)
      {
	ret = 0;
	if (_init_nodes (db, &node1, &node2, &node3))
	  {
	    for (i = 0; i < TEST_POLL_LOOPS; ++i)
	      {
		lw6p2p_node_poll (node1);
		lw6p2p_node_poll (node2);
		lw6p2p_node_poll (node3);
		lw6sys_delay (TEST_POLL_DELAY);
	      }

	    ret = 1;

	    lw6p2p_node_close (node1);
	    lw6p2p_node_free (node1);
	    lw6p2p_node_close (node2);
	    lw6p2p_node_free (node2);
	    lw6p2p_node_close (node3);
	    lw6p2p_node_free (node3);
	  }
	lw6p2p_db_close (db);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("can't create db"));
	ret = 0;
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
  int argc = _TEST_ARGC;
  char *argv[] = { _TEST_ARGV0 };

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
      lw6cli_test (mode);
      lw6srv_test (mode);
    }

  if (lw6net_init (argc, argv))
    {
      ret = _test_db () && _test_node_init () && _test_node_connect ();
      lw6net_quit (argc, argv);
    }

  return ret;
}
