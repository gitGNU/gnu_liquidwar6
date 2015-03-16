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

#include "msg.h"

#define _TEST_IP_1 "123.123.123.123"
#define _TEST_IP_2 "1.2.3.4"
#define _TEST_IP_3 "10.10.10.10"
#define _TEST_PORT 10000
#define _TEST_PROGRAM "beewar7"
#define _TEST_VERSION "777"
#define _TEST_CODENAME "laia"
#define _TEST_STAMP 2
#define _TEST_ID 0x1212323242425252LL
#define _TEST_ID_1 0x1234123412341234LL
#define _TEST_ID_2 0x2345234523452345LL
#define _TEST_ID_3 0x3456345634563456LL
#define _TEST_URL "http://192.168.20.20:8000/"
#define _TEST_TITLE "This is not a sentence"
#define _TEST_DESCRIPTION "This is not an explanation about what this is."
#define _TEST_PASSWORD "toto"
#define _TEST_PASSWORD_KO "titi"
#define _TEST_BENCH 10
#define _TEST_OPEN_RELAY 1
#define _TEST_UPTIME 60
#define _TEST_IDLE_SCREENSHOT_SIZE 5
#define _TEST_IDLE_SCREENSHOT_DATA "1234"
#define _TEST_REQUEST_COMMAND "INFO"
#define _TEST_REQUEST_REMOTE_URL "http://ufoot.hd.free.fr/"
#define _TEST_REQUEST_PASSWORD "toto"
#define _TEST_REQUEST_LOCAL_URL "http://ufoot.org/"
#define _TEST_KEY_VALUE_KO " \t"
#define _TEST_KEY_VALUE_OK_1 "KEY value"
#define _TEST_KEY_VALUE_OK_2 "  KEY2HASNOVALUE"
#define _TEST_KEY_VALUE_OK_3 "KEY3looks-STRANGE\t= value 3 even\t stranger but it does work"
#define _TEST_DEFAULT_KEY_OK "KEY"
#define _TEST_DEFAULT_KEY_KO "???"
#define _TEST_DEFAULT_VALUE_STR "abc"
#define _TEST_DEFAULT_VALUE_INT 123
#define _TEST_ENVELOPE_PHYSICAL_TICKET_SIG 0xabcd0123
#define _TEST_ENVELOPE_LOGICAL_TICKET_SIG 0xabcd0123
#define _TEST_ENVELOPE_PHYSICAL_FROM_ID 0x1234123412341234LL
#define _TEST_ENVELOPE_PHYSICAL_FROM_ID_KO 0x1234123412341235LL
#define _TEST_ENVELOPE_PHYSICAL_TO_ID 0x2345234523452345LL
#define _TEST_ENVELOPE_PHYSICAL_TO_ID_KO 0x2345234523452346LL
#define _TEST_ENVELOPE_LOGICAL_FROM_ID 0x3456345634563456LL
#define _TEST_ENVELOPE_LOGICAL_TO_ID 0x4567456745674567LL
/*
 * It's important to truncate to show up the problem, should
 * the message after the envelope be inconsistent, we still
 * want the envelope to validate. However, too short causes
 * error, more specifically the snapshot generator uses long
 * version names that add extra bytes, this value used to
 * be 100 but 100 is too short for that matter.
 */
#define _TEST_ENVELOPE_TRUNCATE_LEN 120
#define _TEST_META_LOGICAL_FROM 0x1234123412341234LL
#define _TEST_META_NODE_ID_1 0x1234123412341234LL
#define _TEST_META_NODE_ID_2 0x2345234523452345LL
#define _TEST_META_SERIAL_0_1 0
#define _TEST_META_SERIAL_0_2A 3
#define _TEST_META_SERIAL_0_2B 7
#define _TEST_META_SEQ_0_1 100000LL
#define _TEST_META_SEQ_0_2A 300000LL
#define _TEST_META_SEQ_0_2B 700000LL
#define _TEST_META_STR "3456345634563456 0 1 4567456745674567 1000 1000000000000"
#define _TEST_WORD_STR_OK "\"this was quoted\" in a phrase with words"
#define _TEST_WORD_STR_KO " "
#define _TEST_WORD_X_STR_OK "/parent/child/"
#define _TEST_WORD_X_STR_KO "/ / /"
#define _TEST_WORD_BASE64_ITEM " spaces /slashes/"
#define _TEST_WORD_BASE64_OK "%s just before this was base64"
#define _TEST_WORD_BASE64_KO "j*k$lmjk %s I said BASE64 first!"
#define _TEST_WORD_INT_32_OK "0 is a number"
#define _TEST_WORD_INT_32_KO "\n"
#define _TEST_WORD_INT_32_GE0_OK "0 is greater or equal to 0"
#define _TEST_WORD_INT_32_GE0_KO "-1 is lower than 0"
#define _TEST_WORD_INT_32_GT0_OK "1 is greater than 0"
#define _TEST_WORD_INT_32_GT0_KO "zero can't be parsed as a number so will fail"
#define _TEST_WORD_INT_64_OK "1000000000000 is a number"
#define _TEST_WORD_INT_64_KO "\n"
#define _TEST_WORD_INT_64_GE0_OK "0 is greater or equal to 0"
#define _TEST_WORD_INT_64_GE0_KO "-1 is lower than 0"
#define _TEST_WORD_INT_64_GT0_OK "1 is greater than 0"
#define _TEST_WORD_INT_64_GT0_KO "zero can't be parsed as a number so will fail"
#define _TEST_WORD_ID_16_OK "12ab foo bar"
#define _TEST_WORD_ID_16_KO "12345 is too long"
#define _TEST_WORD_ID_32_OK "12345678"
#define _TEST_WORD_ID_32_KO "abcd1234 is not an id because abcd1234 is not a valid id (subtle, it's too big)"
#define _TEST_WORD_ID_64_OK " \t1234123456785678 has spaces at the beginning but this ain't a problem"
#define _TEST_WORD_ID_64_KO "!6789#'8678 did you ever hope that garbage could make it through?"
#define _TEST_Z_MSG_1 "too short to be compressed"
#define _TEST_Z_MSG_2 ""
#define _TEST_Z_MSG_3 "this should be compressed, this should be compressed, this should be compressed, this should be compressed, this should be compressed, this should be compressed, this should be compressed, this should be compressed, this should be compressed, this should be compressed..."
#define _TEST_Z_MSG_4_LEN 200
#define _TEST_Z_LIMIT 30
#define _TEST_SORT_NB 20
#define _TEST_SORT_RANGE 100000000
#define _TEST_SORT_LEN 50
#define _TEST_TICKET1 0x1234567812345678LL
#define _TEST_TICKET2 0x2345678912345678LL
#define _TEST_TICKET1_SIG 0xd46d60b7
#define _TEST_TICKET2_SIG 0xf8e16b04
#define _TEST_TICKET_FROM_ID 0x1234123412341234LL
#define _TEST_TICKET_TO_ID 0X2345234523452345LL
#define _TEST_TICKET_MSG "hello world"
#define _TEST_DATA_SERIAL 421
#define _TEST_DATA_I 3
#define _TEST_DATA_N 1000
#define _TEST_DATA_SEQ 1000000123456LL
#define _TEST_DATA_KER_MSG "HELLO WORLD"
#define _TEST_META_SERIAL 123
#define _TEST_META_I 2
#define _TEST_META_N 500
#define _TEST_META_SEQ 1000000654321LL
#define _TEST_META_ARRAY_ITEM_NODE_ID_1 0x1111111111111111LL
#define _TEST_META_ARRAY_ITEM_NODE_ID_2 0x2222222222222222LL
#define _TEST_META_ARRAY_ITEM_NODE_ID_3 0x3333333333333333LL
#define _TEST_META_ARRAY_ITEM_SERIAL_0_1 2
#define _TEST_META_ARRAY_ITEM_SERIAL_0_2 4
#define _TEST_META_ARRAY_ITEM_SERIAL_0_3 6
#define _TEST_META_ARRAY_ITEM_SEQ_0_1 2000000000001LL
#define _TEST_META_ARRAY_ITEM_SEQ_0_2 4000000000002LL
#define _TEST_META_ARRAY_ITEM_SEQ_0_3 6000000000003LL
#define _TEST_SEQ_BASE 1000000000000000LL
#define _TEST_SEQ_RANGE 1000000000
#define _TEST_SERIAL_BASE 10000000
#define _TEST_SERIAL_RANGE 1000
#define _TEST_MISS_ID_FROM 0x1234123412341234LL
#define _TEST_MISS_ID_TO 0x2345234523452345LL
#define _TEST_MISS_SERIAL_MIN 10
#define _TEST_MISS_SERIAL_MAX 100
#define _TEST_MSG_SERIAL_RANGE 1000000000

typedef struct _lw6msg_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
} _lw6msg_test_data_t;

static _lw6msg_test_data_t _test_data = { 0, NULL };

/*
 * Testing functions in cmd.c
 */
static void
_test_cmd ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6nod_info_t *info = NULL;
    lw6nod_info_t *analysed_info = NULL;
    char *msg = NULL;
    u_int64_t ticket = 0;
    u_int32_t key = 0;
    int64_t seq = 0LL;
    int serial = 0;
    u_int64_t analysed_ticket = 0;
    u_int32_t analysed_key = 0;
    int64_t analysed_seq = 0LL;
    int analysed_serial = 0LL;
    char *remote_url = NULL;
    int data_serial = 0;
    int data_i = 0;
    int data_n = 0;
    int64_t data_seq = 0;
    char *data_ker_msg = NULL;
    int meta_serial = 0;
    int meta_i = 0;
    int meta_n = 0;
    int64_t meta_seq = 0LL;
    lw6msg_meta_array_t meta_array_src;
    lw6msg_meta_array_t meta_array_dst;
    u_int64_t miss_id_from = 0LL;
    u_int64_t miss_id_to = 0LL;
    int miss_serial_min = 0;
    int miss_serial_max = 0;

    info =
      lw6nod_info_new (sys_context, _TEST_PROGRAM, _TEST_VERSION, _TEST_CODENAME,
		       _TEST_STAMP, _TEST_ID, _TEST_URL, _TEST_TITLE,
		       _TEST_DESCRIPTION, _TEST_PASSWORD, _TEST_BENCH, _TEST_OPEN_RELAY, _TEST_UPTIME, _TEST_IDLE_SCREENSHOT_SIZE, _TEST_IDLE_SCREENSHOT_DATA);
    if (info)
      {
	msg = lw6msg_cmd_generate_hello (sys_context, info);
	if (msg)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("hello command is \"%s\""), msg);
	    if (lw6msg_cmd_analyse_hello (sys_context, &analysed_info, msg))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("hello command analysed (node url=\"%s\")"), analysed_info->const_info.ref_info.url);
		lw6nod_info_free (sys_context, analysed_info);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to analyze \"%s\""), msg);
		ret = 0;
	      }
	    remote_url = lw6msg_cmd_guess_from_url (sys_context, msg);
	    if (remote_url)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("could guess url \"%s\" from \"%s\""), remote_url, msg);
		LW6SYS_FREE (sys_context, remote_url);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to guess url from \"%s\""), msg);
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, msg);
	  }

	ticket = lw6sys_generate_id_64 (sys_context);
	msg = lw6msg_cmd_generate_ticket (sys_context, info, ticket);
	if (msg)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("ticket command is \"%s\""), msg);
	    if (lw6msg_cmd_analyse_ticket (sys_context, &analysed_info, &analysed_ticket, msg))
	      {
		if (ticket == analysed_ticket)
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("ticket command analysed (ticket=%" LW6SYS_PRINTF_LL "x)"), (long long) ticket);
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				_x_
				("ticket command analysed but ticket is wrong (%"
				 LW6SYS_PRINTF_LL "x and should be %" LW6SYS_PRINTF_LL "x)"), (long long) analysed_ticket, (long long) ticket);
		    ret = 0;
		  }
		lw6nod_info_free (sys_context, analysed_info);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to analyze \"%s\""), msg);
		ret = 0;
	      }
	    remote_url = lw6msg_cmd_guess_from_url (sys_context, msg);
	    if (remote_url)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("could guess url \"%s\" from \"%s\""), remote_url, msg);
		LW6SYS_FREE (sys_context, remote_url);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to guess url from \"%s\""), msg);
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, msg);
	  }

	key = lw6sys_generate_id_32 (sys_context);
	serial = lw6sys_random (sys_context, _TEST_MSG_SERIAL_RANGE);
	msg = lw6msg_cmd_generate_foo (sys_context, info, key, serial);
	if (msg)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("foo command is \"%s\""), msg);
	    if (lw6msg_cmd_analyse_foo (sys_context, &analysed_info, &analysed_key, &analysed_serial, msg))
	      {
		if (key == analysed_key && serial == analysed_serial)
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("foo command analysed (key=%x serial=%d)"), key, serial);
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				_x_
				("foo command analysed but something is wrong (key=%x and should be %x, serial=%d and should be %d)"),
				analysed_key, key, analysed_serial, serial);
		    ret = 0;
		  }
		lw6nod_info_free (sys_context, analysed_info);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to analyze \"%s\""), msg);
		ret = 0;
	      }
	    remote_url = lw6msg_cmd_guess_from_url (sys_context, msg);
	    if (remote_url)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("could guess url \"%s\" from \"%s\""), remote_url, msg);
		LW6SYS_FREE (sys_context, remote_url);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to guess url from \"%s\""), msg);
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, msg);
	  }

	key = lw6sys_generate_id_32 (sys_context);
	serial = lw6sys_random (sys_context, _TEST_MSG_SERIAL_RANGE);
	msg = lw6msg_cmd_generate_bar (sys_context, info, key, serial);
	if (msg)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("bar command is \"%s\""), msg);
	    if (lw6msg_cmd_analyse_bar (sys_context, &analysed_info, &analysed_key, &analysed_serial, msg))
	      {
		if (key == analysed_key)
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("bar command analysed (key=%x serial=%d)"), key, serial);
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				_x_
				("bar command analysed but something is wrong (key=%x and should be %x, serial=%d and should be %d)"),
				analysed_key, key, analysed_serial, serial);
		    ret = 0;
		  }
		lw6nod_info_free (sys_context, analysed_info);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to analyze \"%s\""), msg);
		ret = 0;
	      }
	    remote_url = lw6msg_cmd_guess_from_url (sys_context, msg);
	    if (remote_url)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("could guess url \"%s\" from \"%s\""), remote_url, msg);
		LW6SYS_FREE (sys_context, remote_url);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to guess url from \"%s\""), msg);
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, msg);
	  }

	seq = _TEST_SEQ_BASE + lw6sys_random (sys_context, _TEST_SEQ_RANGE);
	serial = _TEST_SERIAL_BASE + lw6sys_random (sys_context, _TEST_SERIAL_RANGE);
	msg = lw6msg_cmd_generate_join (sys_context, info, seq, serial);
	if (msg)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("join command is \"%s\""), msg);
	    if (lw6msg_cmd_analyse_join (sys_context, &analysed_info, NULL, &analysed_seq, &analysed_serial, msg))
	      {
		if (seq == analysed_seq && serial == analysed_serial)
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("join command analysed (seq=%" LW6SYS_PRINTF_LL "d serial=%d)"), (long long) seq, serial);
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				_x_
				("join command analysed but seq and/or serial is wrong (seq=%"
				 LW6SYS_PRINTF_LL "d and should be %"
				 LW6SYS_PRINTF_LL "d serial=%d and should be %d)"), (long long) analysed_seq, (long long) seq, analysed_serial, serial);
		    ret = 0;
		  }
		lw6nod_info_free (sys_context, analysed_info);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to analyze \"%s\""), msg);
		ret = 0;
	      }
	    remote_url = lw6msg_cmd_guess_from_url (sys_context, msg);
	    if (remote_url)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("could guess url \"%s\" from \"%s\""), remote_url, msg);
		LW6SYS_FREE (sys_context, remote_url);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to guess url from \"%s\""), msg);
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, msg);
	  }

	msg = lw6msg_cmd_generate_goodbye (sys_context, info);
	if (msg)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("goodbye command is \"%s\""), msg);
	    if (lw6msg_cmd_analyse_goodbye (sys_context, &analysed_info, msg))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("goodbye command analysed (node url=\"%s\")"), analysed_info->const_info.ref_info.url);
		lw6nod_info_free (sys_context, analysed_info);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to analyze \"%s\""), msg);
		ret = 0;
	      }
	    remote_url = lw6msg_cmd_guess_from_url (sys_context, msg);
	    if (remote_url)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("could guess url \"%s\" from \"%s\""), remote_url, msg);
		LW6SYS_FREE (sys_context, remote_url);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to guess url from \"%s\""), msg);
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, msg);
	  }

	msg = lw6msg_cmd_generate_data (sys_context, _TEST_DATA_SERIAL, _TEST_DATA_I, _TEST_DATA_N, _TEST_DATA_SEQ, _TEST_DATA_KER_MSG);
	if (msg)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("data command is \"%s\""), msg);
	    if (lw6msg_cmd_analyse_data (sys_context, &data_serial, &data_i, &data_n, &data_seq, &data_ker_msg, msg))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("data command analysed (node ker_msg=\"%s\")"), data_ker_msg);
		LW6SYS_FREE (sys_context, data_ker_msg);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to analyze \"%s\""), msg);
		ret = 0;
	      }
	    remote_url = lw6msg_cmd_guess_from_url (sys_context, msg);
	    if (remote_url)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("could guess url \"%s\" from \"%s\", this is wrong"), remote_url, msg);
		LW6SYS_FREE (sys_context, remote_url);
		ret = 0;
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to guess url from \"%s\", this is right"), msg);
	      }
	    LW6SYS_FREE (sys_context, msg);
	  }

	lw6msg_meta_array_zero (sys_context, &meta_array_src);
	lw6msg_meta_array_zero (sys_context, &meta_array_dst);
	meta_array_src.logical_from = _TEST_META_LOGICAL_FROM;
	lw6msg_meta_array_set (sys_context, &meta_array_src, _TEST_META_ARRAY_ITEM_NODE_ID_1, _TEST_META_ARRAY_ITEM_SERIAL_0_1, _TEST_META_ARRAY_ITEM_SEQ_0_1);
	lw6msg_meta_array_set (sys_context, &meta_array_src, _TEST_META_ARRAY_ITEM_NODE_ID_2, _TEST_META_ARRAY_ITEM_SERIAL_0_2, _TEST_META_ARRAY_ITEM_SEQ_0_2);
	lw6msg_meta_array_set (sys_context, &meta_array_src, _TEST_META_ARRAY_ITEM_NODE_ID_3, _TEST_META_ARRAY_ITEM_SERIAL_0_3, _TEST_META_ARRAY_ITEM_SEQ_0_3);
	msg = lw6msg_cmd_generate_meta (sys_context, _TEST_META_SERIAL, _TEST_META_I, _TEST_META_N, _TEST_META_SEQ, &meta_array_src);
	if (msg)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("meta command is \"%s\""), msg);
	    if (lw6msg_cmd_analyse_meta (sys_context, &meta_serial, &meta_i, &meta_n, &meta_seq, &meta_array_dst, msg))
	      {
		if (!memcmp (&meta_array_src, &meta_array_dst, sizeof (lw6msg_meta_array_t)))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("meta command parsed, dst == src, this is fine"));
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("meta command not parsed, dst != src, should be the same"));
		  }
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to analyze \"%s\""), msg);
		ret = 0;
	      }
	    remote_url = lw6msg_cmd_guess_from_url (sys_context, msg);
	    if (remote_url)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("could guess url \"%s\" from \"%s\", this is wrong"), remote_url, msg);
		LW6SYS_FREE (sys_context, remote_url);
		ret = 0;
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to guess url from \"%s\", this is right"), msg);
	      }
	    LW6SYS_FREE (sys_context, msg);
	  }

	msg = lw6msg_cmd_generate_miss (sys_context, _TEST_MISS_ID_FROM, _TEST_MISS_ID_TO, _TEST_MISS_SERIAL_MIN, _TEST_MISS_SERIAL_MAX);
	if (msg)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("miss command is \"%s\""), msg);
	    if (lw6msg_cmd_analyse_miss (sys_context, &miss_id_from, &miss_id_to, &miss_serial_min, &miss_serial_max, msg))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			    _x_
			    ("miss command analysed id_from=%"
			     LW6SYS_PRINTF_LL "x id_to=%" LW6SYS_PRINTF_LL
			     "x serial_min=%d serial_max=%d"), (long long) miss_id_from, (long long) miss_id_to, miss_serial_min, miss_serial_max);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to analyze \"%s\""), msg);
		ret = 0;
	      }
	    remote_url = lw6msg_cmd_guess_from_url (sys_context, msg);
	    if (remote_url)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("could guess url \"%s\" from \"%s\", this is wrong"), remote_url, msg);
		LW6SYS_FREE (sys_context, remote_url);
		ret = 0;
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to guess url from \"%s\", this is right"), msg);
	      }
	    LW6SYS_FREE (sys_context, msg);
	  }

	lw6nod_info_free (sys_context, info);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_do_test_envelope (lw6sys_context_t * sys_context, lw6msg_envelope_mode_t mode)
{
  int ret = 1;
  lw6nod_info_t *info = NULL;
  char *msg = NULL;
  char *envelope = NULL;
  char *password_checksum = NULL;
  char *received_msg = NULL;
  u_int32_t received_physical_ticket_sig = 0;
  u_int32_t received_logical_ticket_sig = 0;
  u_int64_t received_physical_from_id = 0;
  u_int64_t received_physical_to_id = 0;
  u_int64_t received_logical_from_id = 0;
  u_int64_t received_logical_to_id = 0;
  char *received_physical_from_url = NULL;

  info =
    lw6nod_info_new (sys_context, _TEST_PROGRAM, _TEST_VERSION, _TEST_CODENAME,
		     _TEST_STAMP, _TEST_ID, _TEST_URL, _TEST_TITLE,
		     _TEST_DESCRIPTION, _TEST_PASSWORD, _TEST_BENCH, _TEST_OPEN_RELAY, _TEST_UPTIME, _TEST_IDLE_SCREENSHOT_SIZE, _TEST_IDLE_SCREENSHOT_DATA);
  if (info)
    {
      password_checksum = lw6cnx_password_checksum (sys_context, _TEST_URL, _TEST_PASSWORD);
      if (password_checksum)
	{
	  msg = lw6msg_cmd_generate_hello (sys_context, info);
	  if (msg)
	    {
	      envelope =
		lw6msg_envelope_generate (sys_context, mode, lw6sys_build_get_version (),
					  password_checksum,
					  _TEST_ENVELOPE_PHYSICAL_TICKET_SIG,
					  _TEST_ENVELOPE_LOGICAL_TICKET_SIG,
					  _TEST_ENVELOPE_PHYSICAL_FROM_ID,
					  _TEST_ENVELOPE_PHYSICAL_TO_ID, _TEST_ENVELOPE_LOGICAL_FROM_ID, _TEST_ENVELOPE_LOGICAL_TO_ID, msg);
	      if (envelope)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("envelope generated \"%s\""), envelope);
		  if (lw6msg_envelope_analyse
		      (sys_context, envelope, mode, _TEST_URL, _TEST_PASSWORD,
		       _TEST_ENVELOPE_PHYSICAL_FROM_ID,
		       _TEST_ENVELOPE_PHYSICAL_TO_ID, &received_msg,
		       &received_physical_ticket_sig,
		       &received_logical_ticket_sig,
		       &received_physical_from_id, &received_physical_to_id, &received_logical_from_id, &received_logical_to_id, &received_physical_from_url))
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("envelope \"%s\" parsed"), envelope);
		      if (received_msg)
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("message is \"%s\""), received_msg);
			  LW6SYS_FREE (sys_context, received_msg);
			}
		      if (received_physical_from_url)
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("guessed \"from url\" \"%s\""), received_physical_from_url);
			  LW6SYS_FREE (sys_context, received_physical_from_url);
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to analyse envelope \"%s\""), envelope);
		      ret = 0;
		    }
		  lw6sys_str_truncate (sys_context, envelope, _TEST_ENVELOPE_TRUNCATE_LEN);
		  if (lw6msg_envelope_analyse
		      (sys_context, envelope, mode, _TEST_URL, _TEST_PASSWORD,
		       _TEST_ENVELOPE_PHYSICAL_FROM_ID,
		       _TEST_ENVELOPE_PHYSICAL_TO_ID, &received_msg,
		       &received_physical_ticket_sig,
		       &received_logical_ticket_sig,
		       &received_physical_from_id, &received_physical_to_id, &received_logical_from_id, &received_logical_to_id, &received_physical_from_url))
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("truncated envelope \"%s\" parsed"), envelope);
		      if (received_msg)
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("message is \"%s\""), received_msg);
			  LW6SYS_FREE (sys_context, received_msg);
			}
		      if (received_physical_from_url)
			{
			  LW6SYS_FREE (sys_context, received_physical_from_url);
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to analyse envelope \"%s\""), envelope);
		      ret = 0;
		    }

		  if (lw6msg_envelope_analyse
		      (sys_context, envelope, mode, _TEST_URL, _TEST_PASSWORD_KO,
		       _TEST_ENVELOPE_PHYSICAL_FROM_ID,
		       _TEST_ENVELOPE_PHYSICAL_TO_ID, &received_msg,
		       &received_physical_ticket_sig,
		       &received_logical_ticket_sig,
		       &received_physical_from_id, &received_physical_to_id, &received_logical_from_id, &received_logical_to_id, &received_physical_from_url))
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("message parsed even with wrong password"));
		      ret = 0;
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("password checking works"));
		    }
		  if (lw6msg_envelope_analyse
		      (sys_context, envelope, mode, _TEST_URL, _TEST_PASSWORD,
		       _TEST_ENVELOPE_PHYSICAL_FROM_ID_KO,
		       _TEST_ENVELOPE_PHYSICAL_TO_ID, &received_msg,
		       &received_physical_ticket_sig,
		       &received_logical_ticket_sig,
		       &received_physical_from_id, &received_physical_to_id, &received_logical_from_id, &received_logical_to_id, &received_physical_from_url))
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("message parsed even with wrong \"from id\""));
		      ret = 0;
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("\"from id\" checking works"));
		    }
		  if (lw6msg_envelope_analyse
		      (sys_context, envelope, mode, _TEST_URL, _TEST_PASSWORD,
		       _TEST_ENVELOPE_PHYSICAL_FROM_ID,
		       _TEST_ENVELOPE_PHYSICAL_TO_ID_KO, &received_msg,
		       &received_physical_ticket_sig,
		       &received_logical_ticket_sig,
		       &received_physical_from_id, &received_physical_to_id, &received_logical_from_id, &received_logical_to_id, &received_physical_from_url))
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("message parsed even with wrong \"to id\""));
		      ret = 0;
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("\"to id\" checking works"));
		    }

		  LW6SYS_FREE (sys_context, envelope);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to generate envelope"));
		}
	      LW6SYS_FREE (sys_context, msg);
	    }

	  /*
	   * Testing with physical == logical, password=NULL,ticket=0
	   */
	  msg = lw6msg_cmd_generate_hello (sys_context, info);
	  if (msg)
	    {
	      envelope =
		lw6msg_envelope_generate (sys_context, mode, lw6sys_build_get_version (),
					  LW6MSG_UNDEF,
					  0,
					  0,
					  _TEST_ENVELOPE_PHYSICAL_FROM_ID,
					  _TEST_ENVELOPE_PHYSICAL_TO_ID, _TEST_ENVELOPE_PHYSICAL_FROM_ID, _TEST_ENVELOPE_PHYSICAL_TO_ID, msg);
	      if (envelope)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("envelope generated \"%s\""), envelope);
		  if (lw6msg_envelope_analyse
		      (sys_context, envelope, mode, _TEST_URL, NULL,
		       _TEST_ENVELOPE_PHYSICAL_FROM_ID,
		       _TEST_ENVELOPE_PHYSICAL_TO_ID, &received_msg,
		       &received_physical_ticket_sig,
		       &received_logical_ticket_sig,
		       &received_physical_from_id, &received_physical_to_id, &received_logical_from_id, &received_logical_to_id, &received_physical_from_url))
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("envelope \"%s\" parsed"), envelope);
		      if (received_msg)
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("message is \"%s\""), received_msg);
			  LW6SYS_FREE (sys_context, received_msg);
			}
		      if (received_physical_from_url)
			{
			  LW6SYS_FREE (sys_context, received_physical_from_url);
			}
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to analyse envelope \"%s\""), envelope);
		      ret = 0;
		    }
		  LW6SYS_FREE (sys_context, envelope);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to generate envelope"));
		}
	      LW6SYS_FREE (sys_context, msg);
	    }
	  LW6SYS_FREE (sys_context, password_checksum);
	}
      lw6nod_info_free (sys_context, info);
    }

  return ret;
}

/*
 * Testing functions in envelope.c
 */
static void
_test_envelope ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    ret = ret && _do_test_envelope (sys_context, LW6MSG_ENVELOPE_MODE_TELNET);
    ret = ret && _do_test_envelope (sys_context, LW6MSG_ENVELOPE_MODE_URL);
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in meta.c
 */
static void
_test_meta ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6msg_meta_array_t meta_array;
    int i = 0;
    char *str = NULL;

    lw6msg_meta_array_zero (sys_context, &meta_array);
    if (lw6msg_meta_array_exists (sys_context, &meta_array, _TEST_META_NODE_ID_1))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("could find id in zeroed array, this is wrong"));
	ret = 0;
      }
    if (!lw6msg_meta_array_set (sys_context, &meta_array, _TEST_META_NODE_ID_1, _TEST_META_SERIAL_0_1, _TEST_META_SEQ_0_1))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to set node_id %" LW6SYS_PRINTF_LL "x"), (long long) _TEST_META_NODE_ID_1);
	ret = 0;
      }
    if (!lw6msg_meta_array_set (sys_context, &meta_array, _TEST_META_NODE_ID_2, _TEST_META_SERIAL_0_2A, _TEST_META_SEQ_0_2A))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to set node_id %" LW6SYS_PRINTF_LL "x (stage A)"), (long long) _TEST_META_NODE_ID_2);
	ret = 0;
      }
    if (!lw6msg_meta_array_set (sys_context, &meta_array, _TEST_META_NODE_ID_2, _TEST_META_SERIAL_0_2B, _TEST_META_SEQ_0_2B))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to set node_id %" LW6SYS_PRINTF_LL "x (stage B)"), (long long) _TEST_META_NODE_ID_2);
	ret = 0;
      }
    if (!lw6msg_meta_array_exists (sys_context, &meta_array, _TEST_META_NODE_ID_1))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_ ("couldn't find id %" LW6SYS_PRINTF_LL "x in array, it should be here"), (long long) _TEST_META_NODE_ID_1);
	ret = 0;
      }
    i = lw6msg_meta_array_exists (sys_context, &meta_array, _TEST_META_NODE_ID_2);
    if (i >= 0)
      {
	if (meta_array.items[i].node_id == _TEST_META_NODE_ID_2 &&
	    meta_array.items[i].serial_0 == _TEST_META_SERIAL_0_2B && meta_array.items[i].seq_0 == _TEST_META_SEQ_0_2B)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			_x_ ("OK, found node_id=%" LW6SYS_PRINTF_LL
			     "x serial_0=%d seq_0=%" LW6SYS_PRINTF_LL "d"), (long long) _TEST_META_NODE_ID_2, _TEST_META_SERIAL_0_2B, _TEST_META_SEQ_0_2B);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("could not find node_id=%" LW6SYS_PRINTF_LL
			     "x serial_0=%d seq_0=%" LW6SYS_PRINTF_LL "d"), (long long) _TEST_META_NODE_ID_2, _TEST_META_SERIAL_0_2B, _TEST_META_SEQ_0_2B);
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_ ("couldn't find id %" LW6SYS_PRINTF_LL "x in array, it should be here"), (long long) _TEST_META_NODE_ID_2);
	ret = 0;
      }
    if (!lw6msg_meta_array_unset (sys_context, &meta_array, _TEST_META_NODE_ID_1))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't remove %" LW6SYS_PRINTF_LL "x from array"), (long long) _TEST_META_NODE_ID_1);
	ret = 0;
      }
    if (lw6msg_meta_array_exists (sys_context, &meta_array, _TEST_META_NODE_ID_1))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("could find id %" LW6SYS_PRINTF_LL "x after removing it"), _TEST_META_NODE_ID_1);
	ret = 0;
      }
    lw6msg_meta_array_zero (sys_context, &meta_array);
    if (lw6msg_meta_str2array (sys_context, &meta_array, _TEST_META_STR))
      {
	str = lw6msg_meta_array2str (sys_context, &meta_array);
	if (str)
	  {
	    if (lw6sys_str_is_same (sys_context, str, _TEST_META_STR))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("OK, str2array and array2str give the same result \"%s\""), str);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_ ("problem, str2array and array2str give different results src=\"%s\" dst=\"%s\""), _TEST_META_STR, str);
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, str);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("lw6msg_meta_array2str failed"));
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("lw6msg_meta_str2array failed"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in oob.c
 */
static void
_test_oob ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6nod_info_t *info = NULL;
    char *oob = NULL;
    lw6sys_list_t *list = NULL;
    char *url = NULL;
    lw6nod_info_t *verified_node = NULL;
    char *pong_url = NULL;
    char *request = NULL;
    int syntax_ok = 0;
    char *command = NULL;
    int password_ok = 0;
    char *remote_url = NULL;

    info =
      lw6nod_info_new (sys_context, _TEST_PROGRAM, _TEST_VERSION, _TEST_CODENAME,
		       _TEST_STAMP, _TEST_ID, _TEST_URL, _TEST_TITLE,
		       _TEST_DESCRIPTION, _TEST_PASSWORD, _TEST_BENCH, _TEST_OPEN_RELAY, _TEST_UPTIME, _TEST_IDLE_SCREENSHOT_SIZE, _TEST_IDLE_SCREENSHOT_DATA);
    if (info)
      {
	oob = lw6msg_oob_generate_info (sys_context, info);
	if (oob)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("standard oob INFO is \"%s\""), oob);
	    LW6SYS_FREE (sys_context, oob);
	  }
	oob = lw6msg_oob_generate_list (sys_context, info);
	if (oob)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("standard (empty) oob LIST is \"%s\""), oob);
	    LW6SYS_FREE (sys_context, oob);
	  }

	oob = lw6msg_oob_generate_pong (sys_context, info);
	if (oob)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("standard oob PONG is \"%s\""), oob);
	    pong_url = lw6msg_oob_analyse_pong (sys_context, oob);
	    if (pong_url)
	      {
		if (lw6sys_str_is_same (sys_context, pong_url, _TEST_URL))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("got url \"%s\" from PONG"), pong_url);
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("PONG message parsed but read \"%s\" instead of \"%s\""), pong_url, _TEST_URL);
		    ret = 0;
		  }
		LW6SYS_FREE (sys_context, pong_url);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to parse PONG message \"%s\""), oob);
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, oob);
	  }

	list = lw6nod_info_new_verified_nodes (sys_context);
	if (list)
	  {
	    url = lw6sys_url_http_from_ip_port (sys_context, _TEST_IP_1, _TEST_PORT);
	    if (url)
	      {
		verified_node =
		  lw6nod_info_new (sys_context, _TEST_PROGRAM, _TEST_VERSION,
				   _TEST_CODENAME, _TEST_STAMP, _TEST_ID_1,
				   url, _TEST_TITLE, _TEST_DESCRIPTION,
				   _TEST_PASSWORD, _TEST_BENCH, _TEST_OPEN_RELAY, _TEST_UPTIME, _TEST_IDLE_SCREENSHOT_SIZE, _TEST_IDLE_SCREENSHOT_DATA);
		if (verified_node && list)
		  {
		    lw6sys_list_push_front (sys_context, &list, verified_node);
		  }
		LW6SYS_FREE (sys_context, url);
	      }
	    url = lw6sys_url_http_from_ip_port (sys_context, _TEST_IP_2, _TEST_PORT);
	    if (url)
	      {
		verified_node =
		  lw6nod_info_new (sys_context, _TEST_PROGRAM, _TEST_VERSION,
				   _TEST_CODENAME, _TEST_STAMP, _TEST_ID_2,
				   url, _TEST_TITLE, _TEST_DESCRIPTION,
				   _TEST_PASSWORD, _TEST_BENCH, _TEST_OPEN_RELAY, _TEST_UPTIME, _TEST_IDLE_SCREENSHOT_SIZE, _TEST_IDLE_SCREENSHOT_DATA);
		if (verified_node && list)
		  {
		    lw6sys_list_push_front (sys_context, &list, verified_node);
		  }
		LW6SYS_FREE (sys_context, url);
	      }
	    url = lw6sys_url_http_from_ip_port (sys_context, _TEST_IP_3, _TEST_PORT);
	    if (url)
	      {
		verified_node =
		  lw6nod_info_new (sys_context, _TEST_PROGRAM, _TEST_VERSION,
				   _TEST_CODENAME, _TEST_STAMP, _TEST_ID_3,
				   url, _TEST_TITLE, _TEST_DESCRIPTION,
				   _TEST_PASSWORD, _TEST_BENCH, _TEST_OPEN_RELAY, _TEST_UPTIME, _TEST_IDLE_SCREENSHOT_SIZE, _TEST_IDLE_SCREENSHOT_DATA);
		if (verified_node && list)
		  {
		    lw6sys_list_push_front (sys_context, &list, verified_node);
		  }
		LW6SYS_FREE (sys_context, url);
	      }

	    if (list)
	      {
		lw6nod_info_set_verified_nodes (sys_context, info, list);
	      }

	    oob = lw6msg_oob_generate_list (sys_context, info);
	    if (oob)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("standard (populated) oob LIST is \"%s\""), oob);
		LW6SYS_FREE (sys_context, oob);
	      }
	  }

	request = lw6msg_oob_generate_request (sys_context, _TEST_REQUEST_COMMAND, _TEST_REQUEST_REMOTE_URL, _TEST_REQUEST_PASSWORD, _TEST_REQUEST_LOCAL_URL);
	if (request)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			_x_
			("request with command=\"%s\" password=\"%s\" url=\"%s\" is \"%s\""),
			_TEST_REQUEST_COMMAND, _TEST_REQUEST_PASSWORD, _TEST_REQUEST_LOCAL_URL, request);
	    if (lw6msg_oob_analyse_request
		(sys_context, &syntax_ok, &command, &password_ok, &remote_url, request, _TEST_REQUEST_REMOTE_URL, _TEST_REQUEST_PASSWORD))
	      {
		if (remote_url)
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
				_x_ ("request \"%s\" analysed syntax_ok=%d password_ok=%d remote_url=\"%s\""), request, syntax_ok, password_ok, remote_url);
		    LW6SYS_FREE (sys_context, remote_url);
		  }
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_ ("request \"%s\" could not be analysed syntax_ok=%d password_ok=%d"), request, syntax_ok, password_ok);
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, request);
	  }
	request = lw6msg_oob_generate_request (sys_context, _TEST_REQUEST_COMMAND, _TEST_REQUEST_REMOTE_URL, _TEST_REQUEST_PASSWORD, NULL);
	if (request)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			_x_ ("request with command=\"%s\" password=\"%s\" url=NULL is \"%s\""), _TEST_REQUEST_COMMAND, _TEST_REQUEST_PASSWORD, request);
	    if (lw6msg_oob_analyse_request
		(sys_context, &syntax_ok, &command, &password_ok, &remote_url, request, _TEST_REQUEST_REMOTE_URL, _TEST_REQUEST_PASSWORD))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("request \"%s\" analysed syntax_ok=%d password_ok=%d"), request, syntax_ok, password_ok);
		if (remote_url)
		  {
		    LW6SYS_FREE (sys_context, remote_url);
		  }
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_ ("request \"%s\" could not be analysed syntax_ok=%d password_ok=%d"), request, syntax_ok, password_ok);
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, request);
	  }
	request = lw6msg_oob_generate_request (sys_context, _TEST_REQUEST_COMMAND, NULL, NULL, _TEST_REQUEST_LOCAL_URL);
	if (request)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			_x_ ("request with command=\"%s\" password=NULL url=\"%s\" is \"%s\""), _TEST_REQUEST_COMMAND, _TEST_REQUEST_LOCAL_URL, request);
	    if (lw6msg_oob_analyse_request (sys_context, &syntax_ok, &command, &password_ok, &remote_url, request, NULL, NULL))
	      {
		if (remote_url)
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
				_x_ ("request \"%s\" analysed syntax_ok=%d password_ok=%d remote_url=\"%s\""), request, syntax_ok, password_ok, remote_url);
		    LW6SYS_FREE (sys_context, remote_url);
		  }
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_ ("request \"%s\" could not be analysed syntax_ok=%d password_ok=%d"), request, syntax_ok, password_ok);
		ret = 0;
	      }
	    if (lw6msg_oob_analyse_request
		(sys_context, &syntax_ok, &command, &password_ok, &remote_url, request, _TEST_REQUEST_REMOTE_URL, _TEST_REQUEST_PASSWORD))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("request \"%s\" is reported OK but password is wrong"), request);
		if (remote_url)
		  {
		    LW6SYS_FREE (sys_context, remote_url);
		  }
		ret = 0;
	      }
	    else
	      {
		if (password_ok)
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				_x_ ("request \"%s\" is reported KO with password OK, this is not logic, password *is* wrong"), request);
		    ret = 0;
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("password checking is OK"));
		  }
	      }
	    LW6SYS_FREE (sys_context, request);
	  }
	request = lw6msg_oob_generate_request (sys_context, _TEST_REQUEST_COMMAND, NULL, NULL, NULL);
	if (request)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("request with command=\"%s\" password=NULL url=NULL is \"%s\""), _TEST_REQUEST_COMMAND, request);
	    if (lw6msg_oob_analyse_request (sys_context, &syntax_ok, &command, &password_ok, &remote_url, request, NULL, NULL))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("request \"%s\" analysed syntax_ok=%d password_ok=%d"), request, syntax_ok, password_ok);
		if (remote_url)
		  {
		    LW6SYS_FREE (sys_context, remote_url);
		  }
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_ ("request \"%s\" could not be analysed syntax_ok=%d password_ok=%d"), request, syntax_ok, password_ok);
		ret = 0;
	      }
	    if (lw6msg_oob_analyse_request
		(sys_context, &syntax_ok, &command, &password_ok, &remote_url, request, _TEST_REQUEST_REMOTE_URL, _TEST_REQUEST_PASSWORD))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("request \"%s\" is reported OK but password is wrong"), request);
		if (remote_url)
		  {
		    LW6SYS_FREE (sys_context, remote_url);
		  }
		ret = 0;
	      }
	    else
	      {
		if (password_ok)
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				_x_ ("request \"%s\" is reported KO with password OK, this is not logic, password *is* wrong"), request);
		    ret = 0;
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("password checking is OK"));
		  }
	      }
	    LW6SYS_FREE (sys_context, request);
	  }

	lw6nod_info_free (sys_context, info);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in sort.c
 */
static void
_test_sort ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_list_t *list = NULL;
    int i = 0;
    int64_t seq = 0LL, last_seq = 0LL;
    char *str = NULL;
    char *tmp = NULL;

    list = lw6sys_list_new (sys_context, lw6sys_free_callback);
    if (list)
      {
	for (i = 0; i < _TEST_SORT_NB; ++i)
	  {
	    seq = ((int64_t) lw6sys_random (sys_context, _TEST_SORT_RANGE) + 1) * ((int64_t) lw6sys_random (sys_context, _TEST_SORT_RANGE) + 1);
	    tmp = lw6sys_str_random_words (sys_context, _TEST_SORT_LEN);
	    if (tmp)
	      {
		str = lw6sys_new_sprintf (sys_context, "%" LW6SYS_PRINTF_LL "d %s", (long long) seq, tmp);
		if (str)
		  {
		    lw6sys_list_push_front (sys_context, &list, str);
		  }
		else
		  {
		    ret = 0;
		  }
		LW6SYS_FREE (sys_context, tmp);
	      }
	    else
	      {
		ret = 0;
	      }
	  }
	lw6sys_sort (sys_context, &list, lw6msg_sort_str_by_seq_callback, NULL);
	while ((str = lw6sys_list_pop_front (sys_context, &list)) != NULL)
	  {
	    if (lw6msg_word_first_int_64 (sys_context, &seq, NULL, str))
	      {
		if (seq > 0LL && seq >= last_seq)
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("sorted str by seq \"%s\""), str);
		    last_seq = seq;
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				_x_ ("sort problem last_seq=%"
				     LW6SYS_PRINTF_LL "d > seq=%" LW6SYS_PRINTF_LL "d \"%s\""), (long long) last_seq, (long long) seq, str);
		    ret = 0;
		  }
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to parse string \"%s\""), str);
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, str);
	  }
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in ticket.c
 */
static void
_test_ticket ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    u_int32_t ticket1_sig = 0;
    u_int32_t ticket2_sig = 0;

    ticket1_sig = lw6msg_ticket_calc_sig (sys_context, _TEST_TICKET1, _TEST_TICKET_FROM_ID, _TEST_TICKET_TO_ID, _TEST_TICKET_MSG);
    if (ticket1_sig == _TEST_TICKET1_SIG)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("ticket_sig for %" LW6SYS_PRINTF_LL "x is %08x"), (long long) _TEST_TICKET1, ticket1_sig);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_ ("ticket_sig for %" LW6SYS_PRINTF_LL "x is %08x and should be %08x"), (long long) _TEST_TICKET1, ticket1_sig, _TEST_TICKET1_SIG);
	ret = 0;
      }

    ticket2_sig = lw6msg_ticket_calc_sig (sys_context, _TEST_TICKET2, _TEST_TICKET_FROM_ID, _TEST_TICKET_TO_ID, _TEST_TICKET_MSG);
    if (ticket2_sig == _TEST_TICKET2_SIG)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("ticket_sig for %" LW6SYS_PRINTF_LL "x is %08x"), (long long) _TEST_TICKET2, ticket2_sig);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_ ("ticket_sig for %" LW6SYS_PRINTF_LL "x is %08x and should be %08x"), (long long) _TEST_TICKET2, ticket2_sig, _TEST_TICKET2_SIG);
	ret = 0;
      }

    if (lw6msg_ticket_check_sig (sys_context, _TEST_TICKET1, _TEST_TICKET_FROM_ID, _TEST_TICKET_TO_ID, _TEST_TICKET_MSG, ticket1_sig))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("ticket_sig check works when same"));
	if (!lw6msg_ticket_check_sig
	    (sys_context, _TEST_TICKET1 + 1, _TEST_TICKET_FROM_ID, _TEST_TICKET_TO_ID,
	     _TEST_TICKET_MSG, ticket1_sig) &&
	    !lw6msg_ticket_check_sig
	    (sys_context, _TEST_TICKET1, _TEST_TICKET_FROM_ID + 1, _TEST_TICKET_TO_ID,
	     _TEST_TICKET_MSG, ticket1_sig) && !lw6msg_ticket_check_sig
	    (sys_context, _TEST_TICKET1, _TEST_TICKET_FROM_ID, _TEST_TICKET_TO_ID + 1, _TEST_TICKET_MSG, ticket1_sig))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("ticket_sig check works when different"));
	  }
	else
	  {
	    ret = 0;
	  }
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_key_value_assoc_callback (lw6sys_context_t * sys_context, void *func_data, const char *key, void *value)
{
  char *value_str = (char *) value;
  int *count = (int *) func_data;

  (*count)++;
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("count=%d found key=\"%s\" value=\"%s\""), *count, key, value_str);
}

/*
 * Testing functions in utils.c
 */
static void
_test_utils ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *key = NULL;
    char *value = NULL;
    lw6sys_assoc_t *assoc = NULL;
    int count = 0;
    char *value_str = NULL;
    int value_int = 0;

    if (lw6msg_utils_parse_key_value_to_ptr (sys_context, &key, &value, _TEST_KEY_VALUE_OK_1))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("line \"%s\" parsed key=\"%s\" value=\"%s\""), _TEST_KEY_VALUE_OK_1, key, value);
	LW6SYS_FREE (sys_context, key);
	LW6SYS_FREE (sys_context, value);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to parse \"%s\""), _TEST_KEY_VALUE_OK_1);
	ret = 0;
      }
    if (lw6msg_utils_parse_key_value_to_ptr (sys_context, &key, &value, _TEST_KEY_VALUE_KO))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_ ("line \"%s\" parsed key=\"%s\" value=\"%s\" but this should have returned NULL"), _TEST_KEY_VALUE_KO, key, value);
	LW6SYS_FREE (sys_context, key);
	LW6SYS_FREE (sys_context, value);
	ret = 0;
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to parse \"%s\", this is normal"), _TEST_KEY_VALUE_KO);
      }
    assoc = lw6sys_assoc_new (sys_context, lw6sys_free_callback);
    if (assoc)
      {
	lw6msg_utils_parse_key_value_to_assoc (sys_context, &assoc, _TEST_KEY_VALUE_OK_1);
	lw6msg_utils_parse_key_value_to_assoc (sys_context, &assoc, _TEST_KEY_VALUE_OK_2);
	lw6msg_utils_parse_key_value_to_assoc (sys_context, &assoc, _TEST_KEY_VALUE_OK_3);
	lw6msg_utils_parse_key_value_to_assoc (sys_context, &assoc, _TEST_KEY_VALUE_KO);
	count = 0;
	lw6sys_assoc_map (sys_context, assoc, _key_value_assoc_callback, &count);
	ret = ret && (count == 3);

	value_str = lw6msg_utils_get_assoc_str_with_default (sys_context, assoc, _TEST_DEFAULT_KEY_OK, _TEST_DEFAULT_VALUE_STR);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("value for \"%s\" is \"%s\""), _TEST_DEFAULT_KEY_OK, value_str);
	value_str = lw6msg_utils_get_assoc_str_with_default (sys_context, assoc, _TEST_DEFAULT_KEY_KO, _TEST_DEFAULT_VALUE_STR);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("value for \"%s\" is \"%s\""), _TEST_DEFAULT_KEY_KO, value_str);
	value_int = lw6msg_utils_get_assoc_int_with_default (sys_context, assoc, _TEST_DEFAULT_KEY_OK, _TEST_DEFAULT_VALUE_INT);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("value for \"%s\" is %d"), _TEST_DEFAULT_KEY_OK, value_int);
	value_int = lw6msg_utils_get_assoc_int_with_default (sys_context, assoc, _TEST_DEFAULT_KEY_KO, _TEST_DEFAULT_VALUE_INT);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("value for \"%s\" is %d"), _TEST_DEFAULT_KEY_KO, value_int);

	lw6sys_assoc_free (sys_context, assoc);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in word.c
 */
static void
_test_word ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6msg_word_t word;
    char *next = NULL;
    int i = 0;
    int64_t ll = 0LL;
    u_int16_t id_16 = 0;
    u_int32_t id_32 = 0;
    u_int64_t id_64 = 0;
    char *base64_item = NULL;
    char *base64_str = NULL;

    if (lw6msg_word_first (sys_context, &word, &next, _TEST_WORD_STR_OK))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("parsed str=\"%s\", next=\"%s\""), word.buf, next);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to parse \"%s\""), _TEST_WORD_STR_OK);
	ret = 0;
      }
    if (lw6msg_word_first (sys_context, &word, &next, _TEST_WORD_STR_KO))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("parsed str=\"%s\" from \"%s\", this is wrong"), word.buf, _TEST_WORD_STR_KO);
	ret = 0;
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to parse \"%s\", that's right"), _TEST_WORD_STR_KO);
      }

    if (lw6msg_word_first_x (sys_context, &word, &next, _TEST_WORD_X_STR_OK))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("parsed str=\"%s\", next=\"%s\""), word.buf, next);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to parse \"%s\""), _TEST_WORD_X_STR_OK);
	ret = 0;
      }
    if (lw6msg_word_first_x (sys_context, &word, &next, _TEST_WORD_X_STR_KO))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("parsed str=\"%s\" from \"%s\", this is wrong"), word.buf, _TEST_WORD_X_STR_KO);
	ret = 0;
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to parse \"%s\", that's right"), _TEST_WORD_X_STR_KO);
      }

    base64_item = lw6glb_base64_encode_str (sys_context, _TEST_WORD_BASE64_ITEM);
    if (base64_item)
      {
	base64_str = lw6sys_new_sprintf (sys_context, _TEST_WORD_BASE64_OK, base64_item);
	if (base64_str)
	  {
	    if (lw6msg_word_first_base64 (sys_context, &word, &next, base64_str))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("parsed base64 str=\"%s\", next=\"%s\""), word.buf, next);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to parse base 64 from \"%s\""), base64_str);
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, base64_str);
	  }
	base64_str = lw6sys_new_sprintf (sys_context, _TEST_WORD_BASE64_KO, base64_item);
	if (base64_str)
	  {
	    if (lw6msg_word_first_base64 (sys_context, &word, &next, base64_str))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("parsed str=\"%s\" from \"%s\", this is wrong"), word.buf, base64_str);
		ret = 0;
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to parse base64 from \"%s\", that's right"), base64_str);
	      }
	    LW6SYS_FREE (sys_context, base64_str);
	  }
	LW6SYS_FREE (sys_context, base64_item);
      }

    if (lw6msg_word_first_int_32 (sys_context, &i, &next, _TEST_WORD_INT_32_OK))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("parsed int32=%d, next=\"%s\""), i, next);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to parse int32 from \"%s\""), _TEST_WORD_INT_32_OK);
	ret = 0;
      }
    if (lw6msg_word_first_int_32 (sys_context, &i, &next, _TEST_WORD_INT_32_KO))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("parsed int32=%d from \"%s\", this is wrong"), i, _TEST_WORD_INT_32_KO);
	ret = 0;
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to parse \"%s\", that's right"), _TEST_WORD_INT_32_KO);
      }

    if (lw6msg_word_first_int_32_ge0 (sys_context, &i, &next, _TEST_WORD_INT_32_GE0_OK))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("parsed int32=%d (>0), next=\"%s\""), i, next);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to parse int32 (>0) from \"%s\""), _TEST_WORD_INT_32_GE0_OK);
	ret = 0;
      }
    if (lw6msg_word_first_int_32_ge0 (sys_context, &i, &next, _TEST_WORD_INT_32_GE0_KO))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("parsed int32=%d from \"%s\", this is wrong"), i, _TEST_WORD_INT_32_GE0_KO);
	ret = 0;
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to parse int32 from \"%s\", that's right"), _TEST_WORD_INT_32_GE0_KO);
      }

    if (lw6msg_word_first_int_32_gt0 (sys_context, &i, &next, _TEST_WORD_INT_32_GT0_OK))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("parsed int32=%d (>0), next=\"%s\""), i, next);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to parse int32 (>0) from \"%s\""), _TEST_WORD_INT_32_GT0_OK);
	ret = 0;
      }
    if (lw6msg_word_first_int_32_gt0 (sys_context, &i, &next, _TEST_WORD_INT_32_GT0_KO))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("parsed int32=%d from \"%s\", this is wrong"), i, _TEST_WORD_INT_32_GT0_KO);
	ret = 0;
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to parse int32 from \"%s\", that's right"), _TEST_WORD_INT_32_GT0_KO);
      }

    if (lw6msg_word_first_int_64 (sys_context, &ll, &next, _TEST_WORD_INT_64_OK))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("parsed int64=%" LW6SYS_PRINTF_LL "d, next=\"%s\""), (long long) ll, next);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to parse int64 from \"%s\""), _TEST_WORD_INT_64_OK);
	ret = 0;
      }
    if (lw6msg_word_first_int_64 (sys_context, &ll, &next, _TEST_WORD_INT_64_KO))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_ ("parsed int64=%" LW6SYS_PRINTF_LL "d from \"%s\", this is wrong"), (long long) ll, _TEST_WORD_INT_64_KO);
	ret = 0;
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to parse \"%s\", that's right"), _TEST_WORD_INT_64_KO);
      }

    if (lw6msg_word_first_int_64_ge0 (sys_context, &ll, &next, _TEST_WORD_INT_64_GE0_OK))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("parsed int64=%" LW6SYS_PRINTF_LL "d (>0), next=\"%s\""), (long long) ll, next);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to parse int64 (>0) from \"%s\""), _TEST_WORD_INT_64_GE0_OK);
	ret = 0;
      }
    if (lw6msg_word_first_int_64_ge0 (sys_context, &ll, &next, _TEST_WORD_INT_64_GE0_KO))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_ ("parsed int64=%" LW6SYS_PRINTF_LL "d from \"%s\", this is wrong"), (long long) ll, _TEST_WORD_INT_64_GE0_KO);
	ret = 0;
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to parse int64 from \"%s\", that's right"), _TEST_WORD_INT_64_GE0_KO);
      }

    if (lw6msg_word_first_int_64_gt0 (sys_context, &ll, &next, _TEST_WORD_INT_64_GT0_OK))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("parsed int64=%" LW6SYS_PRINTF_LL "d (>0), next=\"%s\""), (long long) ll, next);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to parse int64 (>0) from \"%s\""), _TEST_WORD_INT_64_GT0_OK);
	ret = 0;
      }
    if (lw6msg_word_first_int_64_gt0 (sys_context, &ll, &next, _TEST_WORD_INT_64_GT0_KO))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_ ("parsed int64=%" LW6SYS_PRINTF_LL "d from \"%s\", this is wrong"), (long long) ll, _TEST_WORD_INT_64_GT0_KO);
	ret = 0;
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to parse int64 from \"%s\", that's right"), _TEST_WORD_INT_64_GT0_KO);
      }

    if (lw6msg_word_first_id_16 (sys_context, &id_16, &next, _TEST_WORD_ID_16_OK))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("parsed id_16 from \"%s\", next=\"%s\""), _TEST_WORD_ID_16_OK, next);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to parse id_16 from \"%s\""), _TEST_WORD_ID_16_OK);
	ret = 0;
      }
    if (lw6msg_word_first_id_16 (sys_context, &id_16, &next, _TEST_WORD_ID_16_KO))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("parsed id_16 from \"%s\", this is wrong"), _TEST_WORD_ID_16_KO);
	ret = 0;
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to parse id_16 from \"%s\", that's right"), _TEST_WORD_ID_16_KO);
      }

    if (lw6msg_word_first_id_32 (sys_context, &id_32, &next, _TEST_WORD_ID_32_OK))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("parsed id_32 from \"%s\", next=\"%s\""), _TEST_WORD_ID_32_OK, next);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to parse id_32 from \"%s\""), _TEST_WORD_ID_32_OK);
	ret = 0;
      }
    if (lw6msg_word_first_id_32 (sys_context, &id_32, &next, _TEST_WORD_ID_32_KO))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("parsed id_32 from \"%s\", this is wrong"), _TEST_WORD_ID_32_KO);
	ret = 0;
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to parse id_32 from \"%s\", that's right"), _TEST_WORD_ID_32_KO);
      }

    if (lw6msg_word_first_id_64 (sys_context, &id_64, &next, _TEST_WORD_ID_64_OK))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("parsed id_64 from \"%s\", next=\"%s\""), _TEST_WORD_ID_64_OK, next);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to parse id_64 from \"%s\""), _TEST_WORD_ID_64_OK);
	ret = 0;
      }
    if (lw6msg_word_first_id_64 (sys_context, &id_64, &next, _TEST_WORD_ID_64_KO))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("parsed id_64 from \"%s\", this is wrong"), _TEST_WORD_ID_64_KO);
	ret = 0;
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to parse id_64 from \"%s\", that's right"), _TEST_WORD_ID_64_KO);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_test_z_ok (lw6sys_context_t * sys_context, char *_test_str, int log_all)
{
  int ret = 1;
  char *encoded_string = NULL;
  char *decoded_string = NULL;

  encoded_string = lw6msg_z_encode (sys_context, _test_str, _TEST_Z_LIMIT);
  if (encoded_string)
    {
      if (log_all)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("z-encode of \"%s\" gives \"%s\""), _test_str, encoded_string);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("z-encode gives \"%s\""), encoded_string);
	}
      decoded_string = lw6msg_z_decode (sys_context, encoded_string);
      if (decoded_string)
	{
	  if (lw6sys_str_is_same (sys_context, decoded_string, _test_str))
	    {
	      if (log_all)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("successuflly z-decoded \"%s\" to \"%s\""), encoded_string, decoded_string);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("successuflly z-decoded \"%s\""), encoded_string);
		}
	    }
	  else
	    {
	      if (log_all)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			      _x_ ("z-decoded \"%s\" but it's \"%s\", different from \"%s\""), encoded_string, decoded_string, _test_str);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("z-decoded \"%s\" but strings are different"), encoded_string);
		}
	      ret = 0;
	    }
	  LW6SYS_FREE (sys_context, decoded_string);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to z-decode \"%s\""), encoded_string);
	  ret = 0;
	}
      LW6SYS_FREE (sys_context, encoded_string);
    }
  else
    {
      if (log_all)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to z-encode \"%s\""), _test_str);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to z-encode"));
	}
      ret = 0;
    }

  return ret;
}

static int
_test_z_ko (lw6sys_context_t * sys_context, char *_test_str, int log_all)
{
  int ret = 1;
  char *decoded_string = NULL;

  decoded_string = lw6msg_z_decode (sys_context, _test_str);
  if (decoded_string)
    {
      if (log_all)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("z-decoded \"%s\" -> \"%s\", this is wrong!"), _test_str, decoded_string);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("z-decoded a non z-encoded string, this is wrong!"));
	}
      ret = 0;
      LW6SYS_FREE (sys_context, decoded_string);
    }
  else
    {
      if (log_all)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to z-decode \"%s\", that's right"), _test_str);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to z-decode a non z-encode string, that's right"));
	}
    }

  return ret;
}

/*
 * Testing functions in z.c
 */
static void
_test_z ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *random_str = NULL;
    char *z_random_str = NULL;

    random_str = lw6sys_str_random (sys_context, _TEST_Z_MSG_4_LEN);
    ret = _test_z_ok (sys_context, _TEST_Z_MSG_1, 1) && ret;
    ret = _test_z_ok (sys_context, _TEST_Z_MSG_2, 1) && ret;
    ret = _test_z_ok (sys_context, _TEST_Z_MSG_3, 1) && ret;

    if (random_str)
      {
	ret = _test_z_ok (sys_context, random_str, 0) && ret;
      }

    ret = _test_z_ko (sys_context, LW6MSG_Z_PREFIX _TEST_Z_MSG_1, 1) && ret;
    ret = _test_z_ko (sys_context, LW6MSG_Z_PREFIX _TEST_Z_MSG_2, 1) && ret;
    ret = _test_z_ko (sys_context, LW6MSG_Z_PREFIX _TEST_Z_MSG_3, 1) && ret;

    if (random_str)
      {
	z_random_str = lw6sys_str_concat (sys_context, LW6MSG_Z_PREFIX, random_str);
	if (z_random_str)
	  {
	    ret = _test_z_ko (sys_context, z_random_str, 0) && ret;
	    LW6SYS_FREE (sys_context, z_random_str);
	  }
	LW6SYS_FREE (sys_context, random_str);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libmsg CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libmsg CUnit test suite"));

  return CUE_SUCCESS;
}

/**
 * lw6msg_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libmsg module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6msg_test_register (lw6sys_context_t * sys_context, int mode)
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
      lw6nod_test_register (sys_context, mode);
      lw6cnx_test_register (sys_context, mode);
    }

  suite = CU_add_suite ("lw6msg", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_cmd);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_envelope);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_meta);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_oob);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_sort);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_ticket);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_utils);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_word);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_z);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6msg_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @msg module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6msg_test_run (lw6sys_context_t * sys_context, int mode)
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
