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
#endif

#include <CUnit/CUnit.h>

#include "cnx.h"

#define _TEST_LOCAL_URL "http://ufoot.org:8056/"
#define _TEST_REMOTE_URL "http://ufoot.hd.free.fr:8056/"
#define _TEST_REMOTE_IP "127.0.0.1"
#define _TEST_REMOTE_PORT 8888
#define _TEST_PASSWORD "toto"
#define _TEST_LOCAL_ID 0x1234123412341234LL
#define _TEST_REMOTE_ID 0x2345234523452345LL
#define _TEST_DNS_OK 1
#define _TEST_NETWORK_RELIABILITY 100
#define _TEST_NEXT_FOO_DELAY 5000
#define _TEST_PASSWORD_SEED "http://"
#define _TEST_PASSWORD1 "abc"
#define _TEST_PASSWORD2 "XY"
#define _TEST_PASSWORD_CHECKSUM "a838475e316f4b3e3fed"
#define _TEST_TICKET_TABLE_HASH_SIZE 11
#define _TEST_TICKET_TABLE_ID1 "1234123412341234"
#define _TEST_TICKET_TABLE_ID2 "2345234523452345"
#define _TEST_TICKET_ACK_DELAY_MSEC 3000

#define _TEST_PACKET_LOGICAL_TICKET_SIG_1 0x12341234
#define _TEST_PACKET_PHYSICAL_TICKET_SIG_1 0x23452345
#define _TEST_PACKET_LOGICAL_FROM_ID_1 0x2345234523452345LL
#define _TEST_PACKET_LOGICAL_TO_ID_1 0x3456345634563456LL
#define _TEST_PACKET_MSG_1 "toto"
#define _TEST_PACKET_CHECKSUM_1 0xbfad51f5
#define _TEST_PACKET_LOGICAL_TICKET_SIG_2 0x12341234
#define _TEST_PACKET_PHYSICAL_TICKET_SIG_2 0x23452345
#define _TEST_PACKET_LOGICAL_FROM_ID_2 0x2345234523452345LL
#define _TEST_PACKET_LOGICAL_TO_ID_2 0x3456345634563456LL
#define _TEST_PACKET_MSG_2 "titi"
#define _TEST_PACKET_CHECKSUM_2 0xa1f8f755
#define _TEST_PACKET_COMPARE +1

typedef struct _lw6cnx_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
} _lw6cnx_test_data_t;

static _lw6cnx_test_data_t _test_data = { 0, NULL };

/*
 * Testing functions in connection.c
 */
static void
_test_connection ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6cnx_connection_t *cnx = NULL;
    int64_t now = 0;

    now = lw6sys_get_timestamp (sys_context);
    cnx =
      lw6cnx_connection_new (sys_context, _TEST_LOCAL_URL, _TEST_REMOTE_URL,
			     _TEST_REMOTE_IP, _TEST_REMOTE_PORT, _TEST_PASSWORD, _TEST_LOCAL_ID, _TEST_REMOTE_ID, _TEST_DNS_OK, _TEST_NETWORK_RELIABILITY);
    if (LW6SYS_TEST_ACK (cnx))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("cnx_connection object creation works"));
	if (LW6SYS_TEST_ACK (lw6cnx_connection_should_send_foo (sys_context, cnx, now)))
	  {
	    lw6cnx_connection_init_foo_bar_key (sys_context, cnx, now, _TEST_NEXT_FOO_DELAY);
	    if (LW6SYS_TEST_ACK (!lw6cnx_connection_should_send_foo (sys_context, cnx, now)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("cnx_connection foo scheduling works"));
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_ ("cnx_connection object returns true when queried if foo message should be sent, when key has just been initialized"));
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("new cnx_connection object returns false when queried if foo message should be sent"));
	    ret = 0;
	  }

	if (LW6SYS_TEST_ACK (lw6cnx_connection_lock_send (sys_context, cnx)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("acquired send mutex"));
	    lw6cnx_connection_unlock_send (sys_context, cnx);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to acquire send mutex"));
	    ret = 0;
	  }

	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("connection filter returns %d"), lw6cnx_connection_reliability_filter (sys_context, cnx));

	lw6cnx_connection_free (sys_context, cnx);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("cnx_connection object creation failed"));
	ret = 0;
      }

    /*
     * Trying with some NULL fields
     */
    cnx =
      lw6cnx_connection_new (sys_context, _TEST_LOCAL_URL, _TEST_REMOTE_URL,
			     _TEST_REMOTE_IP, _TEST_REMOTE_PORT, NULL, _TEST_LOCAL_ID, _TEST_REMOTE_ID, _TEST_DNS_OK, _TEST_NETWORK_RELIABILITY);
    if (LW6SYS_TEST_ACK (cnx))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("cnx_connection object creation works, even with \"some\" NULLs"));
	lw6cnx_connection_free (sys_context, cnx);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("cnx_connection object creation failed"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in packet.c
 */
static void
_test_packet ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6cnx_packet_t *packet_1 = NULL;
    lw6cnx_packet_t *packet_2 = NULL;
    u_int32_t checksum_1 = 0;
    u_int32_t checksum_2 = 0;

    packet_1 =
      lw6cnx_packet_new (sys_context, _TEST_PACKET_LOGICAL_TICKET_SIG_1,
			 _TEST_PACKET_PHYSICAL_TICKET_SIG_1, _TEST_PACKET_LOGICAL_FROM_ID_1, _TEST_PACKET_LOGICAL_TO_ID_1, _TEST_PACKET_MSG_1);
    if (LW6SYS_TEST_ACK (packet_1))
      {
	packet_2 =
	  lw6cnx_packet_new (sys_context, _TEST_PACKET_LOGICAL_TICKET_SIG_2,
			     _TEST_PACKET_PHYSICAL_TICKET_SIG_2, _TEST_PACKET_LOGICAL_FROM_ID_2, _TEST_PACKET_LOGICAL_TO_ID_2, _TEST_PACKET_MSG_2);
	if (LW6SYS_TEST_ACK (packet_2))
	  {
	    checksum_1 = lw6cnx_packet_checksum (sys_context, packet_1);
	    checksum_2 = lw6cnx_packet_checksum (sys_context, packet_2);
	    if (LW6SYS_TEST_ACK (checksum_1 == _TEST_PACKET_CHECKSUM_1 && checksum_2 == _TEST_PACKET_CHECKSUM_2))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("packet checksums OK (%08x,%08x)"), checksum_1, checksum_2);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_
			    ("packet checksums mismatch (%08x,%08x) vs (%08x,%08x)"),
			    checksum_1, checksum_2, _TEST_PACKET_CHECKSUM_1, _TEST_PACKET_CHECKSUM_2);
		ret = 0;
	      }
	    if (LW6SYS_TEST_ACK (lw6cnx_packet_compare (sys_context, packet_1, packet_2) == _TEST_PACKET_COMPARE))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("packet compare works as expected"));
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("packet compare does not work as expected"));
		ret = 0;
	      }
	    lw6cnx_packet_free (sys_context, packet_2);
	  }
	else
	  {
	    ret = 0;
	  }
	lw6cnx_packet_free (sys_context, packet_1);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in password.c
 */
static void
_test_password ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *checksum = NULL;

    checksum = lw6cnx_password_checksum (sys_context, NULL, NULL);
    if (LW6SYS_TEST_ACK (checksum))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("checksum for password NULL is \"%s\""), checksum);
	LW6SYS_FREE (sys_context, checksum);
      }
    else
      {
	ret = 0;
      }
    checksum = lw6cnx_password_checksum (sys_context, NULL, "");
    if (LW6SYS_TEST_ACK (checksum))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("checksum for empty password is \"%s\""), checksum);
	LW6SYS_FREE (sys_context, checksum);
      }
    else
      {
	ret = 0;
      }
    checksum = lw6cnx_password_checksum (sys_context, _TEST_PASSWORD_SEED, "");
    if (LW6SYS_TEST_ACK (checksum))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("checksum for empty password with seed \"%s\" is \"%s\""), _TEST_PASSWORD_SEED, checksum);
	LW6SYS_FREE (sys_context, checksum);
      }
    else
      {
	ret = 0;
      }
    checksum = lw6cnx_password_checksum (sys_context, _TEST_PASSWORD_SEED, _TEST_PASSWORD1);
    if (LW6SYS_TEST_ACK (checksum))
      {
	if (LW6SYS_TEST_ACK (lw6sys_str_is_same (sys_context, checksum, _TEST_PASSWORD_CHECKSUM)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("checksum for password \"%s\" is \"%s\""), _TEST_PASSWORD1, checksum);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("checksum for password \"%s\" is \"%s\", should be \"%s\""), _TEST_PASSWORD1, checksum, _TEST_PASSWORD_CHECKSUM);
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK (lw6cnx_password_verify (sys_context, _TEST_PASSWORD_SEED, _TEST_PASSWORD1, _TEST_PASSWORD1)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("same password test works"));
	  }
	else
	  {
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK (lw6cnx_password_verify (sys_context, _TEST_PASSWORD_SEED, _TEST_PASSWORD1, checksum)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("same password test works using checksum"));
	  }
	else
	  {
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK (lw6cnx_password_verify (sys_context, _TEST_PASSWORD_SEED, NULL, _TEST_PASSWORD2)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("same password test works when it's NULL here"));
	  }
	else
	  {
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK (!lw6cnx_password_verify (sys_context, _TEST_PASSWORD_SEED, _TEST_PASSWORD1, _TEST_PASSWORD2)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("same password test detects wrong passwords"));
	  }
	else
	  {
	    ret = 0;
	  }
	if (LW6SYS_TEST_ACK (!lw6cnx_password_verify (sys_context, NULL, _TEST_PASSWORD1, _TEST_PASSWORD2)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("same password test detects wrong seed"));
	  }
	else
	  {
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, checksum);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in tickettable.c
 */
static void
_test_ticket_table ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6cnx_ticket_table_t ticket_table;
    u_int32_t ticket = 0;

    lw6cnx_ticket_table_zero (sys_context, &ticket_table);
    if (LW6SYS_TEST_ACK (lw6cnx_ticket_table_init (sys_context, &ticket_table, _TEST_TICKET_TABLE_HASH_SIZE)))
      {
	ticket = lw6cnx_ticket_table_get_recv (sys_context, &ticket_table, _TEST_TICKET_TABLE_ID1);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("ticket generated %08x"), ticket);
	ticket = lw6cnx_ticket_table_get_recv (sys_context, &ticket_table, _TEST_TICKET_TABLE_ID2);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("ticket generated %08x"), ticket);
	if (LW6SYS_TEST_ACK (!lw6cnx_ticket_table_was_recv_exchanged (sys_context, &ticket_table, _TEST_TICKET_TABLE_ID1)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("recv ticket reported as *not* exchanged, this is right"));
	    lw6cnx_ticket_table_ack_recv (sys_context, &ticket_table, _TEST_TICKET_TABLE_ID1, _TEST_TICKET_ACK_DELAY_MSEC);

	    if (LW6SYS_TEST_ACK (!lw6cnx_ticket_table_was_recv_exchanged (sys_context, &ticket_table, _TEST_TICKET_TABLE_ID1)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			    _x_ ("recv ticket still *not* reported as exchanged, this is right, we need to wait for %d msec"), _TEST_TICKET_ACK_DELAY_MSEC);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("recv ticket already reported as exchanged, this is wrong"));
		ret = 0;
	      }

	    lw6sys_delay (sys_context, _TEST_TICKET_ACK_DELAY_MSEC + 1);

	    if (LW6SYS_TEST_ACK (lw6cnx_ticket_table_was_recv_exchanged (sys_context, &ticket_table, _TEST_TICKET_TABLE_ID1)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("recv ticket now reported as exchanged, this is right"));
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("recv ticket now reported as *not* exchanged, this is wrong"));
		ret = 0;
	      }

	    lw6cnx_ticket_table_ack_recv (sys_context, &ticket_table, _TEST_TICKET_TABLE_ID1, _TEST_TICKET_ACK_DELAY_MSEC);

	    if (LW6SYS_TEST_ACK (lw6cnx_ticket_table_was_recv_exchanged (sys_context, &ticket_table, _TEST_TICKET_TABLE_ID1)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("recv ticket still reported as exchanged after double ack, this is right"));
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("recv ticket now reported as *not* exchanged after double ack, this is wrong"));
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("recv ticket reported as exchanged, this is wrong"));
	    ret = 0;
	  }

	if (LW6SYS_TEST_ACK (!lw6cnx_ticket_table_get_send (sys_context, &ticket_table, _TEST_TICKET_TABLE_ID1)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("send ticket does not exist yet, this is right"));
	    lw6cnx_ticket_table_set_send (sys_context, &ticket_table, _TEST_TICKET_TABLE_ID1, lw6sys_generate_id_32 (sys_context));
	    if (LW6SYS_TEST_ACK (lw6cnx_ticket_table_get_send (sys_context, &ticket_table, _TEST_TICKET_TABLE_ID1)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("send ticket now exists, this is right"));
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("send ticket still does not exist, this is wrong"));
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("send ticket does exist, this is wrong"));
	    ret = 0;
	  }
	lw6cnx_ticket_table_clear (sys_context, &ticket_table);
      }
  }
  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libcnx CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libcnx CUnit test suite"));

  return CUE_SUCCESS;
}

/**
 * lw6cnx_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libcnx module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6cnx_test_register (lw6sys_context_t * sys_context, int mode)
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
    }

  suite = CU_add_suite ("lw6cnx", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_connection);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_packet);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_password);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_ticket_table);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6cnx_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @cnx module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6cnx_test_run (lw6sys_context_t * sys_context, int mode)
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
