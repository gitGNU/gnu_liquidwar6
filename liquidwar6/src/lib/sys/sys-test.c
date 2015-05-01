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

#include "sys.h"
#include "sys-internal.h"

#define _TEST_KEYWORD_AS "--this-is_a_TEST-KeYWord"
#define _TEST_ARG_KEYWORD "my-option"
#define _TEST_ARG_ARGV_STRING "--my-option=my-value"
#define _TEST_CHECKSUM_STR_DATA "abc"
#define _TEST_CHECKSUM_STR_RESULT 0x2c17398c
#define _TEST_CHECKSUM_INT32_DATA 1234
#define _TEST_CHECKSUM_INT32_RESULT 0x5d3d512e
#define _TEST_CHECKSUM_INT64_DATA 0x1234567890abcdefLL
#define _TEST_CHECKSUM_INT64_RESULT 0x5fb58579
#define COLOR_ASCII "#12345678"
#define COLOR_8_R 0x12
#define COLOR_8_G 0x34
#define COLOR_8_B 0x56
#define COLOR_8_A 0x78
#ifdef LW6_MS_WINDOWS
#define PATH_WITH_SLASH "\\foo\\bar\\"
#define PATH_WITHOUT_SLASH "\\foo\\bar"
#define PATH_CONCAT "\\foo\\bar\\foo\\bar"
#define PATH_CONCAT_PARENT "\\foo\\bar\\foo"
#define PATH_RELATIVE "..\\..\\foo\\bar"
#define PATH_RELATIVE_UNPARENT "..\\foo\\bar"
#else // LW6_MS_WINDOWS
#define PATH_WITH_SLASH "/foo/bar/"
#define PATH_WITHOUT_SLASH "/foo/bar"
#define PATH_CONCAT "/foo/bar/foo/bar"
#define PATH_CONCAT_PARENT "/foo/bar/foo"
#define PATH_RELATIVE "../../foo/bar"
#define PATH_RELATIVE_UNPARENT "../foo/bar"
#endif // LW6_MS_WINDOWS
#define _TEST_CALLER_INFO_FILE "foo/bar/file.txt"
#define _TEST_CALLER_INFO_FILE_ONLY "file.txt"
#define _TEST_CALLER_INFO_LINE 42
#define _TEST_CALLER_INFO_FUNC "nop"
#define _TEST_LIST_STR1 "str1"
#define _TEST_LIST_STR2 "str2"
#define _TEST_LIST_STR3 "str3"
#define _TEST_LIST_STR4 "str4"
#define _TEST_LIST_STR5 "str5"
#define _TEST_ASSOC_KEY1 "1"
#define _TEST_ASSOC_KEY2 "2"
#define _TEST_ASSOC_KEY3 "3"
#define _TEST_ASSOC_KEY4 "4"
#define _TEST_ASSOC_KEY5 "5"
#define _TEST_LIST_FILTER_NB_ITEMS 25
#define _TEST_LIST_FILTER_RANGE 100
#define _TEST_HASH_SIZE 7
#define _TEST_CACHE_SIZE 7
#define _TEST_CACHE_DELAY_MSEC 500
#define _TEST_PATH_CWD "./"
#define _TEST_PATH_SPLIT "/foo/\\bar\\"
#define _TEST_DIR_LIST_FUNC_DATA "foo,bar"
#define _TEST_PATH_LIST_OTHER "."
#define _TEST_PATH_FILE_ONLY "/foo/bar.txt"
#define _FIND_IN_DIR_AND_PATH_DIR "/foo"
#define _FIND_IN_DIR_AND_PATH_PATH "/bar:/etc"
#define _FIND_IN_DIR_AND_PATH_FILE "hosts"
#define MALLOC_SIZE 1048576
#define _TEST_MUTEX_DELAY 100
#define THREAD_N 5
#define THREAD_SLEEP_MAIN 0.66f
#define THREAD_SLEEP_CALLBACK 0.2f
#define THREAD_TEXT "foo bar"
#define THREAD_STRESS_DURATION 3.0f
#define VTHREAD_N 3
#define _TEST_SLEEP_TIME 3
#define _TEST_DELAY_TIME 50
#define _TEST_RFC1123_SECONDS_FROM_NOW 100000
#define _TEST_READABLE_UPTIME_1 3689
#define _TEST_READABLE_UPTIME_2 1234567
#define _TEST_SLEEP_TIME_SHORT_STEP 0.001f
#define BLANK_STR "\t "
#define CLEANUP_FORMAT "<foo %c bar %c>"
#define CLEANUP_CONTROL '\n'
#define CLEANUP_ACCENT 233
#define STR_STARTS_WITH "abcdef"
#define STR_STARTS_WITH_OK "abc"
#define STR_STARTS_WITH_KO "def"
#define ASCII_STR "foo,bar"
#define _TEST_ARGC 4
#define _TEST_ARGV0 "test.out"
#ifdef LW6_MS_WINDOWS
#define _TEST_ARGV1 "--prefix=C:\\toto"
#define _TEST_ARGV2 "-map-dir=C:\\home\\user\\my-maps\\"
#else
#define _TEST_ARGV1 "--prefix=/toto"
#define _TEST_ARGV2 "-map-dir=/home/user/my-maps/"
#endif
#define _TEST_ARGV3 "THISisAbug!==--foobar=123---?\\"
#define _TEST_FILENAME "test.txt"
#define _TEST_CONTENT "This is a test"
#define _TEST_DUMP "This...\n...is a dump\n"
#define _TEST_HEXA_INT64 1
#define _TEST_HEXA_INT32 123456789
#define _TEST_HEXA_INT16 12345
#define _TEST_HEXA_INT8 123
#define _TEST_HEXA_FLOAT 2.0f
#define _TEST_HEXA_STR1 "abc"
#define _TEST_HEXA_STR2 "defghi"
#define _TEST_HEXA_WHD {1,2,3}
#define _TEST_HEXA_XYZ {4,5,6}
#define _TEST_HEXA_COLOR {25,50,75,100}
#define _TEST_KEYWORD LW6DEF_PREFIX
#ifdef LW6_MS_WINDOWS
#define _TEST_ENV_CONCAT1 "\\my\\path\\1"
#define _TEST_ENV_CONCAT2 "\\my\\path\\2"
#else
#define _TEST_ENV_CONCAT1 "/my/path/1"
#define _TEST_ENV_CONCAT2 "/my/path/2"
#endif
#define _TEST_ENV_SETENV_KEYWORD "LW6_MY_KEY"
#define _TEST_ENV_SETENV_VALUE "my-value"
#define _TEST_ENV_KEYWORD "my-key"
#ifdef LW6_MS_WINDOWS
#define _TEST_ENV_SPLIT "foo1\\foo2;foo3:bar1;bar2\\bar3"
#else
#define _TEST_ENV_SPLIT "foo1/foo2;foo3:bar1;bar2/bar3"
#endif
#define _ESCAPE_STR "this is a string with a simple quote ' a double quote \" a tab char \"\t\" and a backslash \"\\\" a lower and greater <> and a few allowed chars _-.,#:;?& OK"
#define _ESCAPE_STR_HTTP_URI "this%20is%20a%20string%20with%20a%20simple%20quote%20%27%20a%20double%20quote%20%22%20a%20tab%20char%20%22%22%20and%20a%20backslash%20%22%5C%22%20a%20lower%20and%20greater%20%3C%3E%20and%20a%20few%20allowed%20chars%20_-.,#:;?&%20OK"
#define _ESCAPE_STR_HTML_ATTRIBUTE "this is a string with a simple quote ' a double quote &quot; a tab char &quot; &quot; and a backslash &quot;\\&quot; a lower and greater &lt;&gt; and a few allowed chars _-.,#:;?&amp; OK"
#define _ESCAPE_STR_SQL_VALUE "this is a string with a simple quote '' a double quote \" a tab char \" \" and a backslash \"\\\" a lower and greater <> and a few allowed chars _-.,#:;?& OK"
#define _TEST_REFORMAT_STR1 "this is a short string"
#define _TEST_REFORMAT_STR2 "this is a very long string it should be cut in some places but wherever there is a very-very-very-very-very-very-very-very-very-very-very-very-very-very-very-very-very-very long word it won't be cut.\n\nIt should even handle newlines in some places but well, this is not absolutely necessary"
#define _TEST_REFORMAT_PREFIX1 "# "
#define _TEST_REFORMAT_COLUMNS1 25
#define _TEST_REFORMAT_PREFIX2 ""
#define _TEST_REFORMAT_COLUMNS2 10
#define _TEST_SPLIT_STR "/foo/bar,/foo/bar"
#define _TEST_SPLIT_CHAR 'o'
#define _TEST_STR_GLUE "-+-"
#define _TEST_STR_LOWER_UPPER "foObaR!"
#define _TEST_STR_TRUNCATE 5
#define _TEST_STR_TRUNCATE_MIDDLE_LEN 4
#define _TEST_STR_TRUNCATE_MIDDLE_STR "*"
#define _TEST_STR_LOWERED "foobar!"
#define _TEST_STR_UPPERED "FOOBAR!"
#define _TEST_STR_TRUNCATED "FOOBA"
#define _TEST_STR_MIDDLE_TRUNCATED "F*BA"
#define _TEST_STR_RANDOM_LEN 1000
#define _TEST_SERIALIZE 12345
#define _TEST_SORT_LENGTH 10
#define _TEST_SORT_INT {9,3,4,5,6,8,7,2,0,1};
#define _TEST_SORT_INT_MIN 0
#define _TEST_SORT_INT_MAX 9
#define _TEST_SORT_FLOAT {9.0,3.0,4.0,5.0,6.0,8.0,7.0,2.0,0.0,1.0};
#define _TEST_SORT_FLOAT_MIN 0.0
#define _TEST_SORT_FLOAT_MAX 9.0
#define _TEST_SORT_STR {"9","3","4","5","6","8","7","2","0","1"};
#define _TEST_SORT_STR_MIN "0"
#define _TEST_SORT_STR_MAX "9"
#define _TEST_SORT_TWIN {{9,0},{3,1},{4,2},{5,3},{6,4},{8,5},{7,6},{2,7},{0,8},{1,9}}
#define _TEST_SORT_TWIN_MIN {0,8}
#define _TEST_SORT_TWIN_MAX {9,0}
#define _TEST_SORT_TWIN_FUNC_DATA "this is a user func data"
#define _TEST_SIGNAL_LOOPS 25
#define _TEST_SIGNAL_TRAP_ERRORS 1
#define _TEST_CONVERT_INT 421
#define _TEST_CONVERT_LONG_LONG 421421421421421LL
#define _TEST_CONVERT_BOOL 1
#define _TEST_CONVERT_FLOAT 3.14f
#define _TEST_RANDOM_RANGE 1000
#define _TEST_RANDOM_FLOAT_MIN 100.0f
#define _TEST_RANDOM_FLOAT_MAX 101.0f
#define _TEST_PROCESS_DELAY 3
#define _TEST_PROCESS_CALLBACK1_STR "foo"
#define _TEST_PROCESS_CALLBACK2_STR "bar"
#define _TEST_PROGRESS_N 7
#define _TEST_PROGRESS_HERE 0.3
#define _TEST_HISTORY_MSG1 "1 too old"
#define _TEST_HISTORY_MSG2 "2 too old 2"
#define _TEST_HISTORY_MSG3 "3 oldest"
#define _TEST_HISTORY_MSG4 "4 message this is long and should be truncated message this is long and should be truncated message this is long and should be truncated message this is long and should be truncated message this is long and should be truncated message this is long and should be truncated"
#define _TEST_HISTORY_MSG5 "5 most recent"
#define _TEST_HISTORY_SLEEP 5.0
#define _TEST_HISTORY_TIMEOUT 2000
#define _TEST_SHAPE_W 640
#define _TEST_SHAPE_H 480
#define _TEST_SHAPE_D 3
#define _TEST_SHAPE_X 123
#define _TEST_SHAPE_Y 456
#define _TEST_SHAPE_Z 1
#define _TEST_SHAPE_MIN_W 10
#define _TEST_SHAPE_MIN_H 10
#define _TEST_SHAPE_MIN_D 1
#define _TEST_SHAPE_MAX_W 1000
#define _TEST_SHAPE_MAX_H 1000
#define _TEST_SHAPE_MAX_D 5
#define _TEST_MATH_POLY_WY1Y2S1_NB_STEPS 20
#define _TEST_MATH_POLY_WY1Y2S1_W 5.0f
#define _TEST_MATH_POLY_WY1Y2S1_Y1 1.0f
#define _TEST_MATH_POLY_WY1Y2S1_Y2 10.0f
#define _TEST_MATH_POLY_WY1Y2S1_NB_V1S 4
#define _TEST_MATH_POLY_WY1Y2S1_V1_MIN -3.0f
#define _TEST_MATH_POLY_WY1Y2S1_V1_MAX +3.0f
#define _TEST_MATH_ANGLE_XY_MIN -2
#define _TEST_MATH_ANGLE_XY_MAX 2
#define _TEST_MATH_HEARTBEAT_N 60
#define _TEST_MATH_HEARTBEAT_PERIOD 30
#define _TEST_MATH_HEARTBEAT_Y1 1.0f
#define _TEST_MATH_HEARTBEAT_Y2 1.5f
#define _TEST_MATH_BLINK_N 10
#define _TEST_MATH_BLINK_PERIOD 3
#define _TEST_MATH_LIN2LOG_LIN 10000
#define _TEST_MATH_LIN2LOG_LOG 200.0f
#define _TEST_MATH_LIN2LOG_BASE 100.0f
#define _TEST_URL_IP_1 "127.0.0.1"
#define _TEST_URL_PORT_1 1234
#define _TEST_URL_IP_2 "5.6.7.8"
#define _TEST_URL_PORT_2 80
#define _TEST_URL_CANONIZE_1 "http://www.toto.com/"
#define _TEST_URL_CANONIZE_2 "http://www.ToTo.com:81"
#define _TEST_URL_CANONIZE_3 "https://www.ToTo.com:443/tItI"
#define _TEST_URL_CANONIZE_4 "www.toto.com"
#define _TEST_URL_CANONIZE_5 ":123"
#define _TEST_URL_CANONIZE_6 ""
#define _TEST_URL_NOT_CANONIZED "ftp://truc"

#define _TEST_UTILS_IA 1
#define _TEST_UTILS_IB 2
#define _TEST_UTILS_FA 3.0f
#define _TEST_UTILS_FB 4.0f
#define _TEST_UTILS_LLA 10LL
#define _TEST_UTILS_LLB 20LL
#define _TEST_UTILS_DA 30.0f
#define _TEST_UTILS_DB 40.0f

#define _TEST_VERSION_COMPATIBLE_NB 4
#define _TEST_VERSION_COMPATIBLE_A1 "10.11.12"
#define _TEST_VERSION_COMPATIBLE_B1 "10.11.13"
#define _TEST_VERSION_COMPATIBLE_RET1 1
#define _TEST_VERSION_COMPATIBLE_A2 "10.11.12"
#define _TEST_VERSION_COMPATIBLE_B2 "10.12.13"
#define _TEST_VERSION_COMPATIBLE_RET2 0
#define _TEST_VERSION_COMPATIBLE_A3 "Alpha-2"
#define _TEST_VERSION_COMPATIBLE_B3 "ALPHA-2"
#define _TEST_VERSION_COMPATIBLE_RET3 1
#define _TEST_VERSION_COMPATIBLE_A4 "10.11.12"
#define _TEST_VERSION_COMPATIBLE_B4 "10.11"
#define _TEST_VERSION_COMPATIBLE_RET4 1

#define _TEST_LOG_MORE_DIALOG_TIMEOUT 10

typedef struct _lw6sys_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
} _lw6sys_test_data_t;

static _lw6sys_test_data_t _test_data = { 0, NULL };

/*
 * Testing functions in arg.c
 */
static void
_test_arg ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *value = NULL;
    const int argc = _TEST_ARGC;
    const char *argv[_TEST_ARGC] = { _TEST_ARGV0, _TEST_ARGV1, _TEST_ARGV2, _TEST_ARGV3 };

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("trying to match \"%s\""), _TEST_ARG_ARGV_STRING);
    if (LW6SYS_TEST_ACK (lw6sys_arg_match (sys_context, _TEST_ARG_KEYWORD, _TEST_ARG_ARGV_STRING)))
      {
	// does match!
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("command line arg \"%s\" does not match keyword \"%s\""), _TEST_ARG_ARGV_STRING, _TEST_ARG_KEYWORD);
	ret = 0;
      }

    if (LW6SYS_TEST_ACK (lw6sys_arg_exists (sys_context, argc, argv, _TEST_KEYWORD)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("keyword \"%s\" is present"), _TEST_KEYWORD);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("keyword \"%s\" not present"), _TEST_KEYWORD);
	ret = 0;
      }

    value = lw6sys_arg_get_value (sys_context, argc, argv, _TEST_KEYWORD);
    if (LW6SYS_TEST_ACK (value))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("value for \"%s\" is \"%s\""), _TEST_KEYWORD, value);
	LW6SYS_FREE (sys_context, value);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("no value for keyword \"%s\""), _TEST_KEYWORD);
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Used to test lw6sys_assoc_map.
 */
static void
assoc_map_func (lw6sys_context_t * sys_context, void *func_data, const char *key, void *value)
{
  int *ret = (int *) func_data;
  int *i = (int *) value;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("assoc item ret=%d, key=\"%s\", value=%d"), *ret, key, *i);
}

/*
 * Testing functions in assoc.c
 */
static void
_test_assoc ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;
  lw6sys_assoc_t *assoc;
  lw6sys_assoc_t *assoc_copy;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("new/delete on assoc"));
    assoc = lw6sys_assoc_new (sys_context, NULL);
    lw6sys_assoc_free (sys_context, assoc);
  }

  {
    int a = 3, b = 5, c = 7;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("set/unset/has_key/get/map on int assoc"));
    assoc = lw6sys_assoc_new (sys_context, NULL);
    lw6sys_assoc_set (sys_context, &assoc, "a", (void *) &a);
    lw6sys_assoc_set (sys_context, &assoc, "b", (void *) &b);
    lw6sys_assoc_set (sys_context, &assoc, "c", (void *) &a);
    lw6sys_assoc_set (sys_context, &assoc, "c", (void *) &c);
    lw6sys_assoc_unset (sys_context, assoc, "b");
    lw6sys_assoc_map (sys_context, assoc, &assoc_map_func, &ret);
    lw6sys_assoc_sort_and_map (sys_context, assoc, &assoc_map_func, &ret);
    ret = ret && lw6sys_assoc_has_key (sys_context, assoc, "a");
    ret = ret && !lw6sys_assoc_has_key (sys_context, assoc, "b");
    ret = ret && !lw6sys_assoc_has_key (sys_context, assoc, "this key does not exist");
    ret = ret && (*((int *) lw6sys_assoc_get (sys_context, assoc, "a")) == 3);
    ret = ret && ((int *) lw6sys_assoc_get (sys_context, assoc, "b") == NULL);
    ret = ret && (*((int *) lw6sys_assoc_get (sys_context, assoc, "c")) == 7);
    lw6sys_assoc_free (sys_context, assoc);
  }

  {
    char *str1, *str2, *str3;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing free_func callback and dup on string assoc"));
    assoc = lw6sys_assoc_new (sys_context, &lw6sys_free_callback);
    str1 = LW6SYS_MALLOC (sys_context, 5);
    strncpy (str1, _TEST_LIST_STR1, 5);
    lw6sys_assoc_set (sys_context, &assoc, _TEST_ASSOC_KEY1, (void *) str1);
    str2 = LW6SYS_MALLOC (sys_context, 5);
    strncpy (str2, _TEST_LIST_STR2, 5);
    lw6sys_assoc_set (sys_context, &assoc, _TEST_ASSOC_KEY2, (void *) str2);
    str3 = LW6SYS_MALLOC (sys_context, 5);
    strncpy (str3, _TEST_LIST_STR3, 5);
    lw6sys_assoc_set (sys_context, &assoc, _TEST_ASSOC_KEY3, (void *) str3);
    lw6sys_assoc_unset (sys_context, assoc, _TEST_ASSOC_KEY1);
    lw6sys_assoc_unset (sys_context, assoc, _TEST_ASSOC_KEY3);
    assoc_copy = lw6sys_assoc_dup (sys_context, assoc, (lw6sys_dup_func_t) lw6sys_str_copy);
    ret = ret && !lw6sys_assoc_has_key (sys_context, assoc_copy, _TEST_ASSOC_KEY1);
    ret = ret && lw6sys_assoc_has_key (sys_context, assoc_copy, _TEST_ASSOC_KEY2);
    ret = ret && !lw6sys_assoc_has_key (sys_context, assoc_copy, _TEST_ASSOC_KEY3);
    ret = ret && strcmp ((char *) lw6sys_assoc_get (sys_context, assoc_copy, _TEST_ASSOC_KEY2), _TEST_LIST_STR2) == 0;
    lw6sys_assoc_free (sys_context, assoc_copy);
    lw6sys_assoc_free (sys_context, assoc);
  }

  {
    int a = 3, b = 5;
    lw6sys_list_t *keys;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing keys on int assoc"));
    assoc = lw6sys_assoc_new (sys_context, NULL);
    lw6sys_assoc_set (sys_context, &assoc, "a", (void *) &a);
    lw6sys_assoc_set (sys_context, &assoc, "b", (void *) &b);
    lw6sys_assoc_set (sys_context, &assoc, "b", (void *) &b);
    keys = lw6sys_assoc_keys (sys_context, assoc);
    lw6sys_assoc_free (sys_context, assoc);
    ret = ret && lw6sys_list_length (sys_context, keys) == 2;
    lw6sys_list_free (sys_context, keys);
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Test functions in backtrace.c
 */
static void
_test_backtrace ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *bt = NULL;

    bt = lw6sys_backtrace (sys_context, 0, 0);
    if (LW6SYS_TEST_ACK (bt))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("backtrace(0,0)=\"%s\""), bt);
	LW6SYS_FREE (sys_context, bt);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to generate backtrace(0,0)"));
	ret = 0;
      }
    bt = lw6sys_backtrace (sys_context, 0, 1);
    if (LW6SYS_TEST_ACK (bt))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("backtrace(0,1)=\"%s\""), bt);
	LW6SYS_FREE (sys_context, bt);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to generate backtrace(0,1)"));
	ret = 0;
      }
    bt = lw6sys_backtrace (sys_context, 1, 1);
    if (LW6SYS_TEST_ACK (bt))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("backtrace(1,1)=\"%s\""), bt);
	LW6SYS_FREE (sys_context, bt);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to generate backtrace(1,1)"));
	ret = 0;
      }
    BT;
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Test functions in bazooka.c
 */
static void
_test_bazooka ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int bazooka_size = 0;
    int bazooka_eraser = 0;

    bazooka_size = lw6sys_get_memory_bazooka_size (sys_context);
    bazooka_eraser = lw6sys_get_memory_bazooka_eraser (sys_context);

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("bazooka_size=%d bazooka_eraser=%d, these are the defaults"), bazooka_size, bazooka_eraser);
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Test functions in build.c
 */
static void
_test_build ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("build version is \"%s\""), lw6sys_build_get_version ());

    lw6sys_build_log_all (sys_context);
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in cache.c
 */
static void
_test_cache ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;
  lw6sys_cache_t *cache;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("new/delete on cache"));
    cache = lw6sys_cache_new (sys_context, NULL, _TEST_CACHE_SIZE, _TEST_CACHE_DELAY_MSEC);
    lw6sys_cache_free (sys_context, cache);
  }

  {
    int a = 3, b = 5, c = 7;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("set/unset/has_key/get/map on int cache"));
    cache = lw6sys_cache_new (sys_context, NULL, _TEST_CACHE_SIZE, _TEST_CACHE_DELAY_MSEC);
    lw6sys_cache_set (sys_context, cache, "a", (void *) &a);
    lw6sys_cache_set (sys_context, cache, "b", (void *) &b);
    lw6sys_cache_set (sys_context, cache, "c", (void *) &a);
    lw6sys_cache_set (sys_context, cache, "c", (void *) &c);
    lw6sys_cache_unset (sys_context, cache, "b");
    ret = ret && lw6sys_cache_has_key (sys_context, cache, "a");
    ret = ret && !lw6sys_cache_has_key (sys_context, cache, "b");
    ret = ret && !lw6sys_cache_has_key (sys_context, cache, "this key does not exist");
    ret = ret && (*((int *) lw6sys_cache_get (sys_context, cache, "a")) == 3);
    ret = ret && ((int *) lw6sys_cache_get (sys_context, cache, "b") == NULL);
    ret = ret && (*((int *) lw6sys_cache_get (sys_context, cache, "c")) == 7);
    /*
     * Now we wait for some time and keys should disappear
     * as they've been set too long ago.
     */
    lw6sys_delay (sys_context, _TEST_CACHE_DELAY_MSEC);
    // wait even more
    lw6sys_snooze (sys_context);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing expiration"));
    ret = ret && (!lw6sys_cache_has_key (sys_context, cache, "a"));
    lw6sys_cache_free (sys_context, cache);
  }

  {
    char *str1, *str2, *str3;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing free_func callback on string cache"));
    cache = lw6sys_cache_new (sys_context, &lw6sys_free_callback, _TEST_CACHE_SIZE, _TEST_CACHE_DELAY_MSEC);
    str1 = LW6SYS_MALLOC (sys_context, 5);
    strncpy (str1, _TEST_LIST_STR1, 5);
    lw6sys_cache_set (sys_context, cache, _TEST_ASSOC_KEY1, (void *) str1);
    str2 = LW6SYS_MALLOC (sys_context, 5);
    strncpy (str2, _TEST_LIST_STR2, 5);
    lw6sys_cache_set (sys_context, cache, _TEST_ASSOC_KEY2, (void *) str2);
    str3 = LW6SYS_MALLOC (sys_context, 5);
    strncpy (str3, _TEST_LIST_STR3, 5);
    lw6sys_cache_set (sys_context, cache, _TEST_ASSOC_KEY3, (void *) str3);
    lw6sys_cache_unset (sys_context, cache, _TEST_ASSOC_KEY1);
    lw6sys_cache_unset (sys_context, cache, _TEST_ASSOC_KEY3);
    lw6sys_cache_free (sys_context, cache);
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in callerinfo.c
 */
static void
_test_callerinfo ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;
  _lw6sys_caller_info_t caller_info;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    _lw6sys_caller_info_clear (&caller_info);
    _lw6sys_caller_info_set (&caller_info, _TEST_CALLER_INFO_FILE, _TEST_CALLER_INFO_LINE, _TEST_CALLER_INFO_FUNC);

    if (LW6SYS_TEST_ACK
	(lw6sys_str_is_same (sys_context, caller_info.file, _TEST_CALLER_INFO_FILE_ONLY) && caller_info.line == _TEST_CALLER_INFO_LINE
	 && lw6sys_str_is_same (sys_context, caller_info.func, _TEST_CALLER_INFO_FUNC)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("caller info set to %s:%d %s()"), caller_info.file, caller_info.line, caller_info.func);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("caller info set error, got %s:%d %s()"), caller_info.file, caller_info.line, caller_info.func);
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in checksum.c
 */
static void
_test_checksum ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    u_int32_t checksum = 0;

    checksum = lw6sys_checksum_str (sys_context, _TEST_CHECKSUM_STR_DATA);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		_x_ ("checksum for \"%s\" is %08x (should be %08x)"), _TEST_CHECKSUM_STR_DATA, checksum, _TEST_CHECKSUM_STR_RESULT);
    if (!LW6SYS_TEST_ACK (checksum == _TEST_CHECKSUM_STR_RESULT))
      {
	ret = 0;
      }

    checksum = lw6sys_checksum_int32 (sys_context, _TEST_CHECKSUM_INT32_DATA);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		_x_ ("checksum for %d is %08x (should be %08x)"), _TEST_CHECKSUM_INT32_DATA, checksum, _TEST_CHECKSUM_INT32_RESULT);
    if (!LW6SYS_TEST_ACK (checksum == _TEST_CHECKSUM_INT32_RESULT))
      {
	ret = 0;
      }

    checksum = lw6sys_checksum_int64 (sys_context, _TEST_CHECKSUM_INT64_DATA);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		_x_ ("checksum for %lld is %08x (should be %08x)"), _TEST_CHECKSUM_INT64_DATA, checksum, _TEST_CHECKSUM_INT64_RESULT);
    if (!LW6SYS_TEST_ACK (checksum == _TEST_CHECKSUM_INT64_RESULT))
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
rgb_to_hsv_to_rgb (lw6sys_context_t * sys_context, char *label, lw6sys_color_8_t color_8)
{
  lw6sys_color_hsv_t color_hsv;
  lw6sys_color_8_t color_8_back;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%s init r=%d,g=%d,b=%d"), label, color_8.r, color_8.g, color_8.b);
  lw6sys_color_rgb_to_hsv (sys_context, &color_hsv, color_8);
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%s HSV convert h=%.2f,s=%.2f,v=%.2f"), label, color_hsv.h, color_hsv.s, color_hsv.v);
  color_8_back = lw6sys_color_hsv_to_rgb (sys_context, &color_hsv);
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%s RGB convert r=%d,g=%d,b=%d"), label, color_8_back.r, color_8_back.g, color_8_back.b);
}

static void
hsv_invert (lw6sys_context_t * sys_context, char *label, lw6sys_color_8_t color_8)
{
  lw6sys_color_hsv_t color_hsv;
  lw6sys_color_8_t color_8_back;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%s init r=%d,g=%d,b=%d"), label, color_8.r, color_8.g, color_8.b);
  lw6sys_color_rgb_to_hsv (sys_context, &color_hsv, color_8);
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%s HSV convert h=%.2f,s=%.2f,v=%.2f"), label, color_hsv.h, color_hsv.s, color_hsv.v);
  lw6sys_color_hsv_invert (sys_context, &color_hsv, 1, 1, 1);
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%s HSV invert h=%.2f,s=%.2f,v=%.2f"), label, color_hsv.h, color_hsv.s, color_hsv.v);
  color_8_back = lw6sys_color_hsv_to_rgb (sys_context, &color_hsv);
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%s RGB convert r=%d,g=%d,b=%d"), label, color_8_back.r, color_8_back.g, color_8_back.b);
}

/*
 * Testing functions in color.c
 */
static void
_test_color ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_color_8_t color_8;
    int32_t color_irgba;
    int32_t color_ibgra;
    int32_t color_iargb;
    int32_t color_iabgr;
    lw6sys_color_8_t color_8_from_irgba;
    lw6sys_color_8_t color_8_from_ibgra;
    lw6sys_color_8_t color_8_from_iargb;
    lw6sys_color_8_t color_8_from_iabgr;
    lw6sys_color_f_t color_f;
    lw6sys_color_8_t color_8_average[3];
    float distance;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("interpreting color \"%s\""), COLOR_ASCII);
    color_8 = lw6sys_color_a_to_8 (sys_context, COLOR_ASCII);
    if (!LW6SYS_TEST_ACK (color_8.r == COLOR_8_R && color_8.g == COLOR_8_G && color_8.b == COLOR_8_B && color_8.a == COLOR_8_A))
      {
	ret = 0;
      }

    color_irgba = lw6sys_color_8_to_irgba (color_8);
    color_ibgra = lw6sys_color_8_to_ibgra (color_8);
    color_iargb = lw6sys_color_8_to_iargb (color_8);
    color_iabgr = lw6sys_color_8_to_iabgr (color_8);
    color_8_from_irgba = lw6sys_color_irgba_to_8 (color_irgba);
    color_8_from_ibgra = lw6sys_color_ibgra_to_8 (color_ibgra);
    color_8_from_iargb = lw6sys_color_iargb_to_8 (color_iargb);
    color_8_from_iabgr = lw6sys_color_iabgr_to_8 (color_iabgr);
    if (LW6SYS_TEST_ACK
	(lw6sys_color_is_same (sys_context, color_8_from_irgba, color_8)
	 && lw6sys_color_is_same (sys_context, color_8_from_ibgra, color_8)
	 && lw6sys_color_is_same (sys_context, color_8_from_iargb, color_8) && lw6sys_color_is_same (sys_context, color_8_from_iabgr, color_8)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_
		    ("all 8-bit colors to 32-bit integer conversions seem to work color_irgba=%x color_ibgra=%x color_iargb=%x color_iabgr=%x"),
		    color_irgba, color_ibgra, color_iargb, color_iabgr);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_
		    ("8-bit colors to 32-bit integer conversions have a problem color_irgba=%x color_ibgra=%x color_iargb=%x color_iabgr=%x"),
		    color_irgba, color_ibgra, color_iargb, color_iabgr);
	color_irgba = lw6sys_color_8_to_irgba (color_8_from_irgba);
	color_ibgra = lw6sys_color_8_to_ibgra (color_8_from_ibgra);
	color_iargb = lw6sys_color_8_to_iargb (color_8_from_iargb);
	color_iabgr = lw6sys_color_8_to_iabgr (color_8_from_iabgr);
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_
		    ("re-conversion from 8-bit values gives color_irgba=%x color_ibgra=%x color_iargb=%x color_iabgr=%x"),
		    color_irgba, color_ibgra, color_iargb, color_iabgr);
	ret = 0;
      }

    lw6sys_color_8_to_f (&color_f, color_8);
    color_8 = lw6sys_color_f_to_8 (&color_f);

    rgb_to_hsv_to_rgb (sys_context, "white", LW6SYS_COLOR_8_WHITE);
    rgb_to_hsv_to_rgb (sys_context, "black", LW6SYS_COLOR_8_BLACK);
    rgb_to_hsv_to_rgb (sys_context, "red", LW6SYS_COLOR_8_RED);
    rgb_to_hsv_to_rgb (sys_context, "green", LW6SYS_COLOR_8_GREEN);
    rgb_to_hsv_to_rgb (sys_context, "blue", LW6SYS_COLOR_8_BLUE);
    rgb_to_hsv_to_rgb (sys_context, LW6DEF_TEST, color_8);

    hsv_invert (sys_context, "white", LW6SYS_COLOR_8_WHITE);
    hsv_invert (sys_context, "black", LW6SYS_COLOR_8_BLACK);
    hsv_invert (sys_context, "red", LW6SYS_COLOR_8_RED);
    hsv_invert (sys_context, "green", LW6SYS_COLOR_8_GREEN);
    hsv_invert (sys_context, "blue", LW6SYS_COLOR_8_BLUE);
    hsv_invert (sys_context, LW6DEF_TEST, color_8);

    color_8 = lw6sys_color_ponderate (sys_context, LW6SYS_COLOR_8_RED, LW6SYS_COLOR_8_BLUE, 0.5f);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("color between red & blue is r=%d,g=%d,b=%d"), color_8.r, color_8.g, color_8.b);

    color_8_average[0] = LW6SYS_COLOR_8_RED;
    color_8_average[1] = LW6SYS_COLOR_8_RED;
    color_8_average[2] = LW6SYS_COLOR_8_GREEN;
    color_8 = lw6sys_color_average (sys_context, 3, color_8_average);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("color average of red+red+green is r=%d,g=%d,b=%d"), color_8.r, color_8.g, color_8.b);
    distance = lw6sys_color_distance (sys_context, LW6SYS_COLOR_8_GREEN, LW6SYS_COLOR_8_BLUE);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("distance between green and blue is %.2f"), distance);

    if (LW6SYS_TEST_ACK (lw6sys_color_is_same (sys_context, color_8, color_8)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("color comparison failed"));
      }

    lw6sys_color_8_solid (&color_8);
    lw6sys_color_f_solid (&color_f);
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in convert.c
 */
static void
_test_convert ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *str;
    int i;
    int64_t l;
    float f;

    str = lw6sys_itoa (sys_context, _TEST_CONVERT_INT);
    if (LW6SYS_TEST_ACK (str))
      {
	i = lw6sys_atoi (sys_context, str);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("int=%d -> str=\"%s\" -> int=%d"), _TEST_CONVERT_INT, str, i);
	if (!LW6SYS_TEST_ACK (i == _TEST_CONVERT_INT))
	  {
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }

    str = lw6sys_lltoa (sys_context, _TEST_CONVERT_LONG_LONG);
    if (LW6SYS_TEST_ACK (str))
      {
	l = lw6sys_atoll (sys_context, str);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_ ("long long=%" LW6SYS_PRINTF_LL "d -> str=\"%s\" -> long long=%" LW6SYS_PRINTF_LL "d"), _TEST_CONVERT_LONG_LONG, str, (long long) l);
	if (!LW6SYS_TEST_ACK (l == _TEST_CONVERT_LONG_LONG))
	  {
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }

    str = lw6sys_btoa (sys_context, _TEST_CONVERT_BOOL);
    if (LW6SYS_TEST_ACK (str))
      {
	i = lw6sys_atob (sys_context, str);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("bool=%d -> str=\"%s\" -> bool=%d"), _TEST_CONVERT_BOOL, str, i);
	if (!LW6SYS_TEST_ACK (i == _TEST_CONVERT_BOOL))
	  {
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }

    str = lw6sys_ftoa (sys_context, _TEST_CONVERT_FLOAT);
    if (LW6SYS_TEST_ACK (str))
      {
	f = lw6sys_atof (sys_context, str);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("float=%f -> str=\"%s\" -> float=%f"), _TEST_CONVERT_FLOAT, str, f);
	if (f != _TEST_CONVERT_FLOAT)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("%f!=%f: while this is not 100%% blocking, floating point conversion on this computer is suspicious"), _TEST_CONVERT_FLOAT, f);
	  }
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Test functions in daemon.c
 */
static void
_test_daemon ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    const int argc = _TEST_ARGC;
    const char *argv[_TEST_ARGC] = { _TEST_ARGV0, _TEST_ARGV1, _TEST_ARGV2, _TEST_ARGV3 };
    char *pid_file = NULL;

    pid_file = lw6sys_daemon_pid_file (sys_context, argc, argv);
    if (LW6SYS_TEST_ACK (pid_file))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pid file=\"%s\""), pid_file);
	/*
	 * We don't test lw6sys_daemon_start for this would (obviously?)
	 * cause some problems to follow the rest of the test.
	 */
	lw6sys_daemon_stop (sys_context, pid_file);
	LW6SYS_FREE (sys_context, pid_file);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in dump.c
 */
static void
_test_dump ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *user_dir = NULL;

    user_dir = lw6sys_get_default_user_dir (sys_context);
    if (LW6SYS_TEST_ACK (user_dir))
      {
	if (!lw6sys_dir_exists (sys_context, user_dir))
	  {
	    lw6sys_create_dir (sys_context, user_dir);
	  }
	lw6sys_dump_clear (sys_context, user_dir);
	if (!LW6SYS_TEST_ACK (lw6sys_dump (sys_context, user_dir, _TEST_DUMP)))
	  {
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, user_dir);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in env.c
 */
static void
_test_env ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *value;
    char *concat;

    concat = lw6sys_env_concat (sys_context, _TEST_ENV_CONCAT1, _TEST_ENV_CONCAT2);
    if (LW6SYS_TEST_ACK (concat))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("ENV concat is \"%s\""), concat);
	LW6SYS_FREE (sys_context, concat);
      }
    else
      {
	ret = 0;
      }
    if (!LW6SYS_TEST_ACK (lw6sys_setenv (sys_context, _TEST_ENV_SETENV_KEYWORD, _TEST_ENV_SETENV_VALUE)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to set ENV \"%s\""), _TEST_ENV_SETENV_KEYWORD);
	ret = 0;
      }
    if (!LW6SYS_TEST_ACK (lw6sys_env_exists_prefixed (sys_context, _TEST_ENV_KEYWORD)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("ENV \"%s\" doesn't exist"), _TEST_ENV_KEYWORD);
	ret = 0;
      }
    value = lw6sys_getenv_prefixed (sys_context, _TEST_ENV_KEYWORD);
    if (LW6SYS_TEST_ACK (value))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("ENV value for \"%s\" is \"%s\""), _TEST_ENV_KEYWORD, value);
	LW6SYS_FREE (sys_context, value);
      }
    else
      {
	ret = 0;
      }
    value = lw6sys_get_username (sys_context);
    if (LW6SYS_TEST_ACK (value))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("username=\"%s\""), value);
	LW6SYS_FREE (sys_context, value);
      }
    else
      {
	ret = 0;
      }
    value = lw6sys_get_hostname (sys_context);
    if (LW6SYS_TEST_ACK (value))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("hostname=\"%s\""), value);
	LW6SYS_FREE (sys_context, value);
      }
    else
      {
	ret = 0;
      }
  }

  {
    lw6sys_list_t *splitted = NULL;
    char *data = NULL;

    splitted = lw6sys_env_split (sys_context, _TEST_ENV_SPLIT);
    if (LW6SYS_TEST_ACK (splitted))
      {
	lw6sys_list_free (sys_context, splitted);
      }
    else
      {
	ret = 0;
      }

    splitted = lw6sys_env_split (sys_context, _TEST_ENV_SPLIT);
    if (LW6SYS_TEST_ACK (splitted))
      {
	while (splitted && (data = (char *) lw6sys_lifo_pop (sys_context, &splitted)) != NULL)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("splitted env \"%s\" contains \"%s\""), _TEST_ENV_SPLIT, data);
	    LW6SYS_FREE (sys_context, data);
	  }
	//lw6sys_list_free(splitted);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in env.c
 */
static void
_test_escape ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *escaped = NULL;

    escaped = lw6sys_escape_http_uri (sys_context, _ESCAPE_STR);
    if (LW6SYS_TEST_ACK (escaped))
      {
	if (LW6SYS_TEST_ACK (!strcmp (_ESCAPE_STR_HTTP_URI, escaped)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("http uri escape for \"%s\" is \"%s\""), _ESCAPE_STR, escaped);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("http uri escape for \"%s\" is \"%s\" but should be \"%s\""), _ESCAPE_STR, escaped, _ESCAPE_STR_HTTP_URI);
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, escaped);
      }
    escaped = lw6sys_escape_html_attribute (sys_context, _ESCAPE_STR);
    if (LW6SYS_TEST_ACK (escaped))
      {
	if (LW6SYS_TEST_ACK (!strcmp (_ESCAPE_STR_HTML_ATTRIBUTE, escaped)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("http uri escape for \"%s\" is \"%s\""), _ESCAPE_STR, escaped);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("http uri escape for \"%s\" is \"%s\" but should be \"%s\""), _ESCAPE_STR, escaped, _ESCAPE_STR_HTML_ATTRIBUTE);
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, escaped);
      }
    escaped = lw6sys_escape_sql_value (sys_context, _ESCAPE_STR);
    if (LW6SYS_TEST_ACK (escaped))
      {
	if (LW6SYS_TEST_ACK (!strcmp (_ESCAPE_STR_SQL_VALUE, escaped)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("http uri escape for \"%s\" is \"%s\""), _ESCAPE_STR, escaped);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("http uri escape for \"%s\" is \"%s\" but should be \"%s\""), _ESCAPE_STR, escaped, _ESCAPE_STR_SQL_VALUE);
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, escaped);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in exec.c
 */
static int
_test_exec (int argc, const char *argv[], int mode)
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    /*
     * Not using LW6SYS_TEST_ACK here, threading/process issues
     */
    char *myself;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("executed_again=%d"), lw6sys_is_executed_again (sys_context, argc, argv));
    myself = lw6sys_exec_find_myself (sys_context, argc, argv);
    if (myself)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("found myself in \"%s\""), myself);
	if (mode)
	  {
	    if (lw6sys_exec_again (sys_context, argc, argv))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("exec again of \"%s\" successfull, means we're already \"executed again\""), myself);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("exec again of \"%s\" failed"), myself);
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("check mode, so not trying to exec again \"%s\""), myself);
	  }
	LW6SYS_FREE (sys_context, myself);
      }
    else
      {
	if (!mode)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("could not find myself, this is only check mode so no panic, but this is strange"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("could not find myself"));
	    ret = 0;
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END_NO_CUNIT;
  return ret;
}

/*
 * Testing functions in file.c
 */
static void
_test_file ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *content = NULL;
    char *user_dir = NULL;
    char *filename = NULL;
    int filesize = 0;
    const int argc = _TEST_ARGC;
    const char *argv[_TEST_ARGC] = { _TEST_ARGV0, _TEST_ARGV1, _TEST_ARGV2, _TEST_ARGV3 };

    user_dir = lw6sys_get_user_dir (sys_context, argc, argv);
    if (LW6SYS_TEST_ACK (user_dir))
      {
	if (!lw6sys_dir_exists (sys_context, user_dir))
	  {
	    lw6sys_create_dir (sys_context, user_dir);
	  }
	filename = lw6sys_path_concat (sys_context, user_dir, _TEST_FILENAME);
	if (LW6SYS_TEST_ACK (filename))
	  {
	    if (LW6SYS_TEST_ACK (lw6sys_write_file_content (sys_context, filename, _TEST_CONTENT)))
	      {
		content = lw6sys_read_file_content (sys_context, filename);
		if (LW6SYS_TEST_ACK (content && !strcmp (_TEST_CONTENT, content)))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("file \"%s\" contains \"%s\""), filename, content);
		    LW6SYS_FREE (sys_context, content);
		  }
		else
		  {
		    ret = 0;
		  }

		content = lw6sys_read_file_content_bin (sys_context, &filesize, filename);
		if (LW6SYS_TEST_ACK (content && !strcmp (_TEST_CONTENT, content) && strlen (content) == filesize))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("bin file \"%s\" has size %d and contains \"%s\""), filename, filesize, content);
		    LW6SYS_FREE (sys_context, content);
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
	    LW6SYS_FREE (sys_context, filename);
	  }
	LW6SYS_FREE (sys_context, user_dir);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in profiler.c
 */
static void
_test_profiler ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("Google Profiler support=%d"), lw6sys_profiler_check (sys_context, 0));
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in hash.c
 */
static void
_test_hash ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;
  lw6sys_hash_t *hash;
  lw6sys_hash_t *hash_copy;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("new/delete on hash"));
    hash = lw6sys_hash_new (sys_context, NULL, _TEST_HASH_SIZE);
    lw6sys_hash_free (sys_context, hash);
  }

  {
    int a = 3, b = 5, c = 7;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("set/unset/has_key/get/map on int hash"));
    hash = lw6sys_hash_new (sys_context, NULL, _TEST_HASH_SIZE);
    lw6sys_hash_set (sys_context, hash, "a", (void *) &a);
    lw6sys_hash_set (sys_context, hash, "b", (void *) &b);
    lw6sys_hash_set (sys_context, hash, "c", (void *) &a);
    lw6sys_hash_set (sys_context, hash, "c", (void *) &c);
    lw6sys_hash_unset (sys_context, hash, "b");
    lw6sys_hash_map (sys_context, hash, &assoc_map_func, &ret);
    lw6sys_hash_sort_and_map (sys_context, hash, &assoc_map_func, &ret);
    ret = ret && lw6sys_hash_has_key (sys_context, hash, "a");
    ret = ret && !lw6sys_hash_has_key (sys_context, hash, "b");
    ret = ret && !lw6sys_hash_has_key (sys_context, hash, "this key does not exist");
    ret = ret && (*((int *) lw6sys_hash_get (sys_context, hash, "a")) == 3);
    ret = ret && ((int *) lw6sys_hash_get (sys_context, hash, "b") == NULL);
    ret = ret && (*((int *) lw6sys_hash_get (sys_context, hash, "c")) == 7);
    lw6sys_hash_free (sys_context, hash);
  }

  {
    char *str1, *str2, *str3;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing free_func callback and dup on string hash"));
    hash = lw6sys_hash_new (sys_context, &lw6sys_free_callback, 7);
    str1 = LW6SYS_MALLOC (sys_context, 5);
    strncpy (str1, _TEST_LIST_STR1, 5);
    lw6sys_hash_set (sys_context, hash, _TEST_ASSOC_KEY1, (void *) str1);
    str2 = LW6SYS_MALLOC (sys_context, 5);
    strncpy (str2, _TEST_LIST_STR2, 5);
    lw6sys_hash_set (sys_context, hash, _TEST_ASSOC_KEY2, (void *) str2);
    str3 = LW6SYS_MALLOC (sys_context, 5);
    strncpy (str3, _TEST_LIST_STR3, 5);
    lw6sys_hash_set (sys_context, hash, _TEST_ASSOC_KEY3, (void *) str3);
    lw6sys_hash_unset (sys_context, hash, _TEST_ASSOC_KEY1);
    lw6sys_hash_unset (sys_context, hash, _TEST_ASSOC_KEY3);
    hash_copy = lw6sys_hash_dup (sys_context, hash, (lw6sys_dup_func_t) lw6sys_str_copy);
    ret = ret && !lw6sys_hash_has_key (sys_context, hash_copy, _TEST_ASSOC_KEY1);
    ret = ret && lw6sys_hash_has_key (sys_context, hash_copy, _TEST_ASSOC_KEY2);
    ret = ret && !lw6sys_hash_has_key (sys_context, hash_copy, _TEST_ASSOC_KEY3);
    ret = ret && strcmp ((char *) lw6sys_hash_get (sys_context, hash_copy, _TEST_ASSOC_KEY2), _TEST_LIST_STR2) == 0;
    lw6sys_hash_free (sys_context, hash_copy);
    lw6sys_hash_free (sys_context, hash);
  }

  {
    int a = 3, b = 5;
    lw6sys_list_t *keys;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing keys on int hash"));
    hash = lw6sys_hash_new (sys_context, NULL, _TEST_HASH_SIZE);
    lw6sys_hash_set (sys_context, hash, "a", (void *) &a);
    lw6sys_hash_set (sys_context, hash, "b", (void *) &b);
    lw6sys_hash_set (sys_context, hash, "b", (void *) &b);
    keys = lw6sys_hash_keys (sys_context, hash);
    lw6sys_hash_free (sys_context, hash);
    ret = ret && lw6sys_list_length (sys_context, keys) == 2;
    lw6sys_list_free (sys_context, keys);
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_hexa ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_hexa_serializer_t *hexa_serializer;
    char *hexa_string = NULL;
    int64_t _test_64;
    int32_t _test_32;
    int16_t _test_16;
    int8_t _test_8;
    float _test_float;
    char *_test_str = NULL;
    lw6sys_whd_t _test_whd = _TEST_HEXA_WHD;
    lw6sys_xyz_t _test_xyz = _TEST_HEXA_XYZ;
    lw6sys_color_8_t _test_color = _TEST_HEXA_COLOR;
    void *ptr = NULL;
    char *ptr_str = NULL;

    hexa_serializer = lw6sys_hexa_serializer_new (sys_context, "");
    if (LW6SYS_TEST_ACK (hexa_serializer))
      {
	lw6sys_hexa_serializer_rewind (sys_context, hexa_serializer);
	ret = ret && lw6sys_hexa_serializer_push_int64 (sys_context, hexa_serializer, _TEST_HEXA_INT64);
	ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, _TEST_HEXA_INT32);
	ret = ret && lw6sys_hexa_serializer_push_int16 (sys_context, hexa_serializer, _TEST_HEXA_INT16);
	ret = ret && lw6sys_hexa_serializer_push_int8 (sys_context, hexa_serializer, _TEST_HEXA_INT8);
	ret = ret && lw6sys_hexa_serializer_push_float (sys_context, hexa_serializer, _TEST_HEXA_FLOAT);
	ret = ret && lw6sys_hexa_serializer_push_str (sys_context, hexa_serializer, _TEST_HEXA_STR1);
	ret = ret && lw6sys_hexa_serializer_push_str (sys_context, hexa_serializer, _TEST_HEXA_STR2);
	ret = ret && lw6sys_hexa_serializer_push_whd (sys_context, hexa_serializer, _test_whd);
	ret = ret && lw6sys_hexa_serializer_push_xyz (sys_context, hexa_serializer, _test_xyz);
	ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, _test_color);
	if (LW6SYS_TEST_ACK (ret))
	  {
	    hexa_string = lw6sys_hexa_serializer_as_string (sys_context, hexa_serializer);
	    if (LW6SYS_TEST_ACK (hexa_string))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("hexa string is \"%s\""), hexa_string);
		LW6SYS_FREE (sys_context, hexa_string);
	      }

	    lw6sys_hexa_serializer_rewind (sys_context, hexa_serializer);

	    if (!LW6SYS_TEST_ACK (!lw6sys_hexa_serializer_eof (sys_context, hexa_serializer)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unexpected serializer EOF"));
		ret = 0;
	      }

	    if (LW6SYS_TEST_ACK (lw6sys_hexa_serializer_pop_int64 (sys_context, hexa_serializer, &_test_64)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("int64 loaded, value=%d"), (int) _test_64);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (LW6SYS_TEST_ACK (lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &_test_32)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("int32 loaded, value=%d"), (int) _test_32);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (LW6SYS_TEST_ACK (lw6sys_hexa_serializer_pop_int16 (sys_context, hexa_serializer, &_test_16)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("int16 loaded, value=%d"), (int) _test_16);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (LW6SYS_TEST_ACK (lw6sys_hexa_serializer_pop_int8 (sys_context, hexa_serializer, &_test_8)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("int8 loaded, value=%d"), (int) _test_8);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (LW6SYS_TEST_ACK (lw6sys_hexa_serializer_pop_float (sys_context, hexa_serializer, &_test_float)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("float loaded, value=%f"), _test_float);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (LW6SYS_TEST_ACK (lw6sys_hexa_serializer_pop_str (sys_context, hexa_serializer, &_test_str)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("str loaded, value=\"%s\""), _test_str);
		LW6SYS_FREE (sys_context, _test_str);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (LW6SYS_TEST_ACK (lw6sys_hexa_serializer_pop_str (sys_context, hexa_serializer, &_test_str)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("str loaded, value=\"%s\""), _test_str);
		LW6SYS_FREE (sys_context, _test_str);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (LW6SYS_TEST_ACK (lw6sys_hexa_serializer_pop_whd (sys_context, hexa_serializer, &_test_whd)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("whd loaded, value=%dx%dx%d"), (int) _test_whd.w, (int) _test_whd.h, (int) _test_whd.d);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (LW6SYS_TEST_ACK (lw6sys_hexa_serializer_pop_xyz (sys_context, hexa_serializer, &_test_xyz)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("xyz loaded, value=%d,%d,%d"), (int) _test_xyz.x, (int) _test_xyz.y, (int) _test_xyz.z);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (LW6SYS_TEST_ACK (lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &_test_color)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			    _x_ ("color loaded, value=#%02x%02x%02x%02x"), (int) _test_color.r, (int) _test_color.g, (int) _test_color.b, (int) _test_color.a);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (LW6SYS_TEST_ACK (lw6sys_hexa_serializer_eof (sys_context, hexa_serializer)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("serializer EOF"));
	      }
	    else
	      {
		ret = 0;
	      }
	  }

	ptr_str = lw6sys_hexa_ptr_to_str (sys_context, (void *) hexa_serializer);
	if (LW6SYS_TEST_ACK (ptr_str))
	  {
	    ptr = lw6sys_hexa_str_to_ptr (sys_context, ptr_str);
	    if (LW6SYS_TEST_ACK (ptr == (void *) hexa_serializer))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pointer %p string is \"%s\""), ptr, ptr_str);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("error converting pointer %p from string \"%s\""), ptr, ptr_str);
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, ptr_str);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("error converting pointer %p"), hexa_serializer);
	    ret = 0;
	  }
	lw6sys_hexa_serializer_free (sys_context, hexa_serializer);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in history.c
 */
static void
_test_history ()
{
  int ret = 0;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char **history;
    char **i;

    lw6sys_history_init (sys_context);
    lw6sys_history_register (sys_context, _TEST_HISTORY_MSG1);
    lw6sys_history_register (sys_context, _TEST_HISTORY_MSG2);
    lw6sys_sleep (sys_context, _TEST_HISTORY_SLEEP);
    lw6sys_history_register (sys_context, _TEST_HISTORY_MSG3);
    lw6sys_history_register (sys_context, _TEST_HISTORY_MSG4);
    lw6sys_history_register (sys_context, _TEST_HISTORY_MSG5);
    history = lw6sys_history_get (sys_context, _TEST_HISTORY_TIMEOUT);
    if (LW6SYS_TEST_ACK (history))
      {
	for (i = history; i[0]; ++i)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("history msg \"%s\""), i[0]);
	    ret = 1;
	  }
	lw6sys_history_free (sys_context, history);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in i18n.c
 */
static void
_test_i18n ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *utf8_str;
    int local_ret = 0;

    utf8_str = lw6sys_locale_to_utf8 (sys_context, ASCII_STR);
    if (LW6SYS_TEST_ACK (utf8_str))
      {
	if (LW6SYS_TEST_ACK (strcmp (utf8_str, ASCII_STR) == 0))
	  {
	    local_ret = 1;
	  }

	LW6SYS_FREE (sys_context, utf8_str);
      }

    ret = ret && local_ret;
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in id.c
 */
static void
_test_id ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    u_int16_t id_16 = 0;
    u_int32_t id_32 = 0;
    u_int64_t id_64 = 0;
    char *id_str = NULL;
    u_int64_t id_long = 0;

    id_16 = lw6sys_generate_id_16 (sys_context);
    id_str = lw6sys_id_ltoa (sys_context, id_16);
    if (LW6SYS_TEST_ACK (id_str))
      {
	id_long = lw6sys_id_atol (sys_context, id_str);
	if (LW6SYS_TEST_ACK (id_long == id_16))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("here's a random 16-bit id \"%s\""), id_str);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("erreur converting id, src=%" LW6SYS_PRINTF_LL "x, dst=%" LW6SYS_PRINTF_LL "x"), (unsigned long long) id_16, (long long) id_long);
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, id_str);
      }

    id_32 = lw6sys_generate_id_32 (sys_context);
    id_str = lw6sys_id_ltoa (sys_context, id_32);
    if (LW6SYS_TEST_ACK (id_str))
      {
	id_long = lw6sys_id_atol (sys_context, id_str);
	if (LW6SYS_TEST_ACK (id_long == id_32))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("here's a random 32-bit id \"%s\""), id_str);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("erreur converting id, src=%" LW6SYS_PRINTF_LL "x, dst=%" LW6SYS_PRINTF_LL "x"), (unsigned long long) id_32, (long long) id_long);
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, id_str);
      }

    id_64 = lw6sys_generate_id_64 (sys_context);
    id_str = lw6sys_id_ltoa (sys_context, id_64);
    if (LW6SYS_TEST_ACK (id_str))
      {
	id_long = lw6sys_id_atol (sys_context, id_str);
	if (LW6SYS_TEST_ACK (id_long == id_64))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("here's a random 64-bit id \"%s\""), id_str);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("erreur converting id, src=%" LW6SYS_PRINTF_LL "x, dst=%" LW6SYS_PRINTF_LL "x"), (unsigned long long) id_64, (long long) id_long);
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, id_str);
      }

  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in keyword.c
 */
static void
_test_keyword ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *str;

    str = lw6sys_keyword_as_key (sys_context, _TEST_KEYWORD_AS);
    if (LW6SYS_TEST_ACK (str))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("keyword as key is \"%s\""), str);
	LW6SYS_FREE (sys_context, str);
      }
    str = lw6sys_keyword_as_arg (sys_context, _TEST_KEYWORD_AS);
    if (LW6SYS_TEST_ACK (str))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("keyword as arg is \"%s\""), str);
	LW6SYS_FREE (sys_context, str);
      }
    str = lw6sys_keyword_as_env (sys_context, _TEST_KEYWORD_AS);
    if (LW6SYS_TEST_ACK (str))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("keyword as env is \"%s\""), str);
	LW6SYS_FREE (sys_context, str);
      }
    str = lw6sys_keyword_as_xml (sys_context, _TEST_KEYWORD_AS);
    if (LW6SYS_TEST_ACK (str))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("keyword as xml is \"%s\""), str);
	LW6SYS_FREE (sys_context, str);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Used to test lw6sys_list_map.
 */
static void
list_map_int_func (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  int *ret = (int *) func_data;
  int *i = (int *) data;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("list item ret=%d, i=%d"), *ret, *i);
}

/*
 * Used to test lw6sys_list_map.
 */
static void
list_map_str_func (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  int *ret = (int *) func_data;
  char *str = (char *) data;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("list item ret=%d, str=\"%s\""), *ret, str);
}

/*
 * Used to test lw6sys_list_map.
 */
static int
list_filter_func (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  int *limit = (int *) func_data;
  int *d = (int *) data;
  int ret = 0;

  ret = (*d < *limit);
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("list item limit=%d, d=%d, pass filter=%d"), *limit, *d, ret);

  return ret;
}

/*
 * Testing functions in list.c
 */
static void
_test_list ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;
  lw6sys_list_t *list;
  lw6sys_list_t *list_copy;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("new/delete/is_empty on list"));
    list = lw6sys_list_new (sys_context, NULL);
    ret = ret && lw6sys_list_is_empty (sys_context, list);
    lw6sys_list_free (sys_context, list);
  }

  {
    int a = 3, b = 5, c = 7;
    int i;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lifo push/pop on int list"));
    list = lw6sys_list_new (sys_context, NULL);
    lw6sys_lifo_push (sys_context, &list, (void *) &a);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lifo push %d"), a);
    lw6sys_lifo_push (sys_context, &list, (void *) &b);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lifo push %d"), b);
    lw6sys_lifo_push (sys_context, &list, (void *) &c);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lifo push %d"), c);
    lw6sys_list_map (sys_context, list, &list_map_int_func, &ret);
    ret = ret && lw6sys_list_length (sys_context, list) == 3;
    i = *((int *) lw6sys_lifo_pop (sys_context, &list));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lifo pop %d"), i);
    ret = ret && (i == c);
    i = *((int *) lw6sys_lifo_pop (sys_context, &list));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lifo pop %d"), i);
    ret = ret && (i == b);
    i = *((int *) lw6sys_lifo_pop (sys_context, &list));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lifo pop %d"), i);
    ret = ret && (i == a);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("free list"));
    lw6sys_list_free (sys_context, list);
  }

  {
    int a = 2, b = 4, c = 6, d = 8;
    int i;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo push/pop on int list"));
    list = lw6sys_list_new (sys_context, NULL);
    lw6sys_fifo_push (sys_context, &list, (void *) &a);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo push %d"), a);
    lw6sys_fifo_push (sys_context, &list, (void *) &b);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo push %d"), b);
    lw6sys_fifo_push (sys_context, &list, (void *) &c);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo push %d"), c);
    lw6sys_fifo_push (sys_context, &list, (void *) &d);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo push %d"), d);
    lw6sys_list_map (sys_context, list, &list_map_int_func, &ret);
    ret = ret && lw6sys_list_length (sys_context, list) == 4;
    i = *((int *) lw6sys_fifo_pop (sys_context, &list));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo pop %d"), i);
    ret = ret && (i == a);
    i = *((int *) lw6sys_fifo_pop (sys_context, &list));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo pop %d"), i);
    ret = ret && (i == b);
    i = *((int *) lw6sys_fifo_pop (sys_context, &list));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo pop %d"), i);
    ret = ret && (i == c);
    i = *((int *) lw6sys_fifo_pop (sys_context, &list));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo pop %d"), i);
    ret = ret && (i == d);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("free list"));
    lw6sys_list_free (sys_context, list);
  }

  {
    char *str;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_front/pop_front on string list"));
    list = lw6sys_list_new (sys_context, lw6sys_free_callback);
    lw6sys_list_push_front (sys_context, &list, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR1));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_front \"%s\""), _TEST_LIST_STR1);
    lw6sys_list_push_front (sys_context, &list, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR2));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_front \"%s\""), _TEST_LIST_STR2);
    lw6sys_list_push_front (sys_context, &list, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR3));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_front \"%s\""), _TEST_LIST_STR3);
    lw6sys_list_map (sys_context, list, &list_map_str_func, &ret);
    str = (char *) lw6sys_list_pop_front (sys_context, &list);
    if (LW6SYS_TEST_ACK (str))
      {
	ret = ret && (!strcmp (str, _TEST_LIST_STR3));
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_front \"%s\""), str);
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (str))
      {
	str = (char *) lw6sys_list_pop_front (sys_context, &list);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_front \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR2));
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (str))
      {
	str = (char *) lw6sys_list_pop_front (sys_context, &list);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_front \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR1));
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("free list"));
    lw6sys_list_free (sys_context, list);
  }

  {
    char *str;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_back/pop_front on string list"));
    list = lw6sys_list_new (sys_context, lw6sys_free_callback);
    lw6sys_list_push_back (sys_context, &list, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR1));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_back \"%s\""), _TEST_LIST_STR1);
    lw6sys_list_push_back (sys_context, &list, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR2));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_back \"%s\""), _TEST_LIST_STR2);
    lw6sys_list_push_back (sys_context, &list, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR3));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_back \"%s\""), _TEST_LIST_STR3);
    lw6sys_list_map (sys_context, list, &list_map_str_func, &ret);
    str = (char *) lw6sys_list_pop_front (sys_context, &list);
    if (LW6SYS_TEST_ACK (str))
      {
	ret = ret && (!strcmp (str, _TEST_LIST_STR1));
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_front \"%s\""), str);
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (str))
      {
	str = (char *) lw6sys_list_pop_front (sys_context, &list);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_front \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR2));
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (str))
      {
	str = (char *) lw6sys_list_pop_front (sys_context, &list);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_front \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR3));
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("free list"));
    lw6sys_list_free (sys_context, list);
  }

  {
    char *str;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_front/pop_back on string list"));
    list = lw6sys_list_new (sys_context, lw6sys_free_callback);
    lw6sys_list_push_front (sys_context, &list, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR1));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_front \"%s\""), _TEST_LIST_STR1);
    lw6sys_list_push_front (sys_context, &list, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR2));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_front \"%s\""), _TEST_LIST_STR2);
    lw6sys_list_push_front (sys_context, &list, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR3));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_front \"%s\""), _TEST_LIST_STR3);
    lw6sys_list_map (sys_context, list, &list_map_str_func, &ret);
    str = (char *) lw6sys_list_pop_back (sys_context, &list);
    if (LW6SYS_TEST_ACK (str))
      {
	ret = ret && (!strcmp (str, _TEST_LIST_STR1));
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_back \"%s\""), str);
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (str))
      {
	str = (char *) lw6sys_list_pop_back (sys_context, &list);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_back \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR2));
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (str))
      {
	str = (char *) lw6sys_list_pop_back (sys_context, &list);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_back \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR3));
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("free list"));
    lw6sys_list_free (sys_context, list);
  }

  {
    char *str;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_back/pop_back on string list"));
    list = lw6sys_list_new (sys_context, lw6sys_free_callback);
    lw6sys_list_push_back (sys_context, &list, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR1));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_back \"%s\""), _TEST_LIST_STR1);
    lw6sys_list_push_back (sys_context, &list, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR2));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_back \"%s\""), _TEST_LIST_STR2);
    lw6sys_list_push_back (sys_context, &list, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR3));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_back \"%s\""), _TEST_LIST_STR3);
    lw6sys_list_map (sys_context, list, &list_map_str_func, &ret);
    str = (char *) lw6sys_list_pop_back (sys_context, &list);
    if (LW6SYS_TEST_ACK (str))
      {
	ret = ret && (!strcmp (str, _TEST_LIST_STR3));
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_back \"%s\""), str);
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (str))
      {
	str = (char *) lw6sys_list_pop_back (sys_context, &list);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_back \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR2));
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (str))
      {
	str = (char *) lw6sys_list_pop_back (sys_context, &list);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_back \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR1));
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("free list"));
    lw6sys_list_free (sys_context, list);
  }

  {
    int a = 32, b = 64, c = 128, d = 256, e = 512;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("front/back push/pop on int list"));
    list = lw6sys_list_new (sys_context, NULL);
    lw6sys_list_push_front (sys_context, &list, (void *) &a);
    lw6sys_list_push_front (sys_context, &list, (void *) &b);
    lw6sys_list_push_back (sys_context, &list, (void *) &c);
    lw6sys_list_push_back (sys_context, &list, (void *) &d);
    lw6sys_list_push_back (sys_context, &list, (void *) &e);
    lw6sys_list_map (sys_context, list, &list_map_int_func, &ret);
    ret = ret && lw6sys_list_length (sys_context, list) == 5;
    ret = ret && (*((int *) lw6sys_list_pop_back (sys_context, &list)) == e);
    ret = ret && (*((int *) lw6sys_list_pop_back (sys_context, &list)) == d);
    ret = ret && (*((int *) lw6sys_list_pop_front (sys_context, &list)) == b);
    ret = ret && (*((int *) lw6sys_list_pop_front (sys_context, &list)) == a);
    ret = ret && (*((int *) lw6sys_list_pop_front (sys_context, &list)) == c);
    lw6sys_list_free (sys_context, list);
  }

  {
    char *str1, *str2, *str3, *str4;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing free_func callback and dup on string list"));
    list = lw6sys_list_new (sys_context, &lw6sys_free_callback);
    str1 = lw6sys_str_copy (sys_context, _TEST_LIST_STR1);
    lw6sys_lifo_push (sys_context, &list, (void *) str1);
    str2 = lw6sys_str_copy (sys_context, _TEST_LIST_STR2);
    lw6sys_fifo_push (sys_context, &list, (void *) str2);
    str3 = lw6sys_str_copy (sys_context, _TEST_LIST_STR3);
    lw6sys_lifo_push (sys_context, &list, (void *) str3);
    str4 = lw6sys_str_copy (sys_context, _TEST_LIST_STR4);
    lw6sys_fifo_push (sys_context, &list, (void *) str4);
    list_copy = lw6sys_list_dup (sys_context, list, (lw6sys_dup_func_t) lw6sys_str_copy);
    ret = ret && LW6SYS_TEST_ACK (lw6sys_list_length (sys_context, list_copy) == 4);
    lw6sys_list_free (sys_context, list_copy);
    lw6sys_list_free (sys_context, list);
  }

  {
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing final pop behavior"));

    list = lw6sys_list_new (sys_context, NULL);
    if (LW6SYS_TEST_ACK (list))
      {
	lw6sys_list_pop_front (sys_context, &list);
	if (!LW6SYS_TEST_ACK (!list))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("final pop front didn't free list"));
	    lw6sys_list_free (sys_context, list);
	    ret = 0;
	  }
      }

    list = lw6sys_list_new (sys_context, NULL);
    if (LW6SYS_TEST_ACK (list))
      {
	lw6sys_list_pop_back (sys_context, &list);
	if (!LW6SYS_TEST_ACK (!list))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("final pop back didn't free list"));
	    lw6sys_list_free (sys_context, list);
	    ret = 0;
	  }
      }
  }
  {
    int i = 0;
    int *d = NULL;
    int limit = 0;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing list filter"));
    list = lw6sys_list_new (sys_context, lw6sys_free_callback);
    if (LW6SYS_TEST_ACK (list))
      {
	for (i = 0; i < _TEST_LIST_FILTER_NB_ITEMS; ++i)
	  {
	    d = (int *) LW6SYS_MALLOC (sys_context, sizeof (int));
	    if (LW6SYS_TEST_ACK (d))
	      {
		(*d) = lw6sys_random (sys_context, _TEST_LIST_FILTER_RANGE);
		lw6sys_list_push_front (sys_context, &list, d);
	      }
	  }
	limit = (2 * _TEST_LIST_FILTER_RANGE) / 3;
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("1st filter limit=%d"), limit);
	lw6sys_list_filter (sys_context, &list, list_filter_func, &limit);
	limit = _TEST_LIST_FILTER_RANGE / 3;
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("2nd filter limit=%d"), limit);
	lw6sys_list_filter (sys_context, &list, list_filter_func, &limit);
	limit = -_TEST_LIST_FILTER_RANGE / 10;
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("3rd filter limit=%d"), limit);
	lw6sys_list_filter (sys_context, &list, list_filter_func, &limit);
	lw6sys_list_free (sys_context, list);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in listr.c
 */
static void
_test_list_r ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;
  lw6sys_list_r_t *list_r;
  lw6sys_list_r_t *list_r_copy;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("new/delete/is_empty on list_r"));
    list_r = lw6sys_list_r_new (sys_context, NULL);
    ret = ret && lw6sys_list_r_is_empty (sys_context, list_r);
    lw6sys_list_r_free (sys_context, list_r);
  }

  {
    int a = 3, b = 5, c = 7;
    int i;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lifo push/pop on int list_r"));
    list_r = lw6sys_list_r_new (sys_context, NULL);
    lw6sys_lifo_r_push (sys_context, list_r, (void *) &a);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lifo push %d"), a);
    lw6sys_lifo_r_push (sys_context, list_r, (void *) &b);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lifo push %d"), b);
    lw6sys_lifo_r_push (sys_context, list_r, (void *) &c);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lifo push %d"), c);
    lw6sys_list_r_map (sys_context, list_r, &list_map_int_func, &ret);
    ret = ret && lw6sys_list_r_length (sys_context, list_r) == 3;
    i = *((int *) lw6sys_lifo_r_pop (sys_context, list_r));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lifo pop %d"), i);
    ret = ret && (i == c);
    i = *((int *) lw6sys_lifo_r_pop (sys_context, list_r));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lifo pop %d"), i);
    ret = ret && (i == b);
    i = *((int *) lw6sys_lifo_r_pop (sys_context, list_r));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lifo pop %d"), i);
    ret = ret && (i == a);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("free list_r"));
    lw6sys_list_r_free (sys_context, list_r);
  }

  {
    int a = 2, b = 4, c = 6, d = 8;
    int i;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo push/pop on int list_r"));
    list_r = lw6sys_list_r_new (sys_context, NULL);
    lw6sys_fifo_r_push (sys_context, list_r, (void *) &a);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo push %d"), a);
    lw6sys_fifo_r_push (sys_context, list_r, (void *) &b);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo push %d"), b);
    lw6sys_fifo_r_push (sys_context, list_r, (void *) &c);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo push %d"), c);
    lw6sys_fifo_r_push (sys_context, list_r, (void *) &d);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo push %d"), d);
    lw6sys_list_r_map (sys_context, list_r, &list_map_int_func, &ret);
    ret = ret && lw6sys_list_r_length (sys_context, list_r) == 4;
    i = *((int *) lw6sys_fifo_r_pop (sys_context, list_r));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo pop %d"), i);
    ret = ret && (i == a);
    i = *((int *) lw6sys_fifo_r_pop (sys_context, list_r));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo pop %d"), i);
    ret = ret && (i == b);
    i = *((int *) lw6sys_fifo_r_pop (sys_context, list_r));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo pop %d"), i);
    ret = ret && (i == c);
    i = *((int *) lw6sys_fifo_r_pop (sys_context, list_r));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fifo pop %d"), i);
    ret = ret && (i == d);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("free list_r"));
    lw6sys_list_r_free (sys_context, list_r);
  }

  {
    char *str;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_front/pop_front on string list_r"));
    list_r = lw6sys_list_r_new (sys_context, lw6sys_free_callback);
    lw6sys_list_r_push_front (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR1));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_front \"%s\""), _TEST_LIST_STR1);
    lw6sys_list_r_push_front (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR2));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_front \"%s\""), _TEST_LIST_STR2);
    lw6sys_list_r_push_front (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR3));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_front \"%s\""), _TEST_LIST_STR3);
    lw6sys_list_r_map (sys_context, list_r, &list_map_str_func, &ret);
    str = (char *) lw6sys_list_r_pop_front (sys_context, list_r);
    if (LW6SYS_TEST_ACK (str))
      {
	ret = ret && (!strcmp (str, _TEST_LIST_STR3));
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_front \"%s\""), str);
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (str))
      {
	str = (char *) lw6sys_list_r_pop_front (sys_context, list_r);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_front \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR2));
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (str))
      {
	str = (char *) lw6sys_list_r_pop_front (sys_context, list_r);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_front \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR1));
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("free list_r"));
    lw6sys_list_r_free (sys_context, list_r);
  }

  {
    char *str;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_back/pop_front on string list_r"));
    list_r = lw6sys_list_r_new (sys_context, lw6sys_free_callback);
    lw6sys_list_r_push_back (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR1));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_back \"%s\""), _TEST_LIST_STR1);
    lw6sys_list_r_push_back (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR2));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_back \"%s\""), _TEST_LIST_STR2);
    lw6sys_list_r_push_back (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR3));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_back \"%s\""), _TEST_LIST_STR3);
    lw6sys_list_r_map (sys_context, list_r, &list_map_str_func, &ret);
    str = (char *) lw6sys_list_r_pop_front (sys_context, list_r);
    if (LW6SYS_TEST_ACK (str))
      {
	ret = ret && (!strcmp (str, _TEST_LIST_STR1));
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_front \"%s\""), str);
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (str))
      {
	str = (char *) lw6sys_list_r_pop_front (sys_context, list_r);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_front \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR2));
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (str))
      {
	str = (char *) lw6sys_list_r_pop_front (sys_context, list_r);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_front \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR3));
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("free list_r"));
    lw6sys_list_r_free (sys_context, list_r);
  }

  {
    char *str;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_front/pop_back on string list_r"));
    list_r = lw6sys_list_r_new (sys_context, lw6sys_free_callback);
    lw6sys_list_r_push_front (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR1));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_front \"%s\""), _TEST_LIST_STR1);
    lw6sys_list_r_push_front (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR2));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_front \"%s\""), _TEST_LIST_STR2);
    lw6sys_list_r_push_front (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR3));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_front \"%s\""), _TEST_LIST_STR3);
    lw6sys_list_r_map (sys_context, list_r, &list_map_str_func, &ret);
    str = (char *) lw6sys_list_r_pop_back (sys_context, list_r);
    if (LW6SYS_TEST_ACK (str))
      {
	ret = ret && (!strcmp (str, _TEST_LIST_STR1));
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_back \"%s\""), str);
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (str))
      {
	str = (char *) lw6sys_list_r_pop_back (sys_context, list_r);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_back \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR2));
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (str))
      {
	str = (char *) lw6sys_list_r_pop_back (sys_context, list_r);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_back \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR3));
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("free list_r"));
    lw6sys_list_r_free (sys_context, list_r);
  }

  {
    char *str;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_back/pop_back on string list_r"));
    list_r = lw6sys_list_r_new (sys_context, lw6sys_free_callback);
    lw6sys_list_r_push_back (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR1));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_back \"%s\""), _TEST_LIST_STR1);
    lw6sys_list_r_push_back (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR2));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_back \"%s\""), _TEST_LIST_STR2);
    lw6sys_list_r_push_back (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR3));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("push_back \"%s\""), _TEST_LIST_STR3);
    lw6sys_list_r_map (sys_context, list_r, &list_map_str_func, &ret);
    str = (char *) lw6sys_list_r_pop_back (sys_context, list_r);
    if (LW6SYS_TEST_ACK (str))
      {
	ret = ret && (!strcmp (str, _TEST_LIST_STR3));
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_back \"%s\""), str);
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (str))
      {
	str = (char *) lw6sys_list_r_pop_back (sys_context, list_r);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_back \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR2));
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (str))
      {
	str = (char *) lw6sys_list_r_pop_back (sys_context, list_r);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("pop_back \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR1));
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("free list_r"));
    lw6sys_list_r_free (sys_context, list_r);
  }

  {
    int a = 32, b = 64, c = 128, d = 256, e = 512;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("front/back push/pop on int list_r"));
    list_r = lw6sys_list_r_new (sys_context, NULL);
    lw6sys_list_r_push_front (sys_context, list_r, (void *) &a);
    lw6sys_list_r_push_front (sys_context, list_r, (void *) &b);
    lw6sys_list_r_push_back (sys_context, list_r, (void *) &c);
    lw6sys_list_r_push_back (sys_context, list_r, (void *) &d);
    lw6sys_list_r_push_back (sys_context, list_r, (void *) &e);
    lw6sys_list_r_map (sys_context, list_r, &list_map_int_func, &ret);
    ret = ret && lw6sys_list_r_length (sys_context, list_r) == 5;
    ret = ret && (*((int *) lw6sys_list_r_pop_back (sys_context, list_r)) == e);
    ret = ret && (*((int *) lw6sys_list_r_pop_back (sys_context, list_r)) == d);
    ret = ret && (*((int *) lw6sys_list_r_pop_front (sys_context, list_r)) == b);
    ret = ret && (*((int *) lw6sys_list_r_pop_front (sys_context, list_r)) == a);
    ret = ret && (*((int *) lw6sys_list_r_pop_front (sys_context, list_r)) == c);
    lw6sys_list_r_free (sys_context, list_r);
  }

  {
    char *str1, *str2, *str3, *str4;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing free_func callback and dup on string list_r"));
    list_r = lw6sys_list_r_new (sys_context, &lw6sys_free_callback);
    str1 = lw6sys_str_copy (sys_context, _TEST_LIST_STR1);
    lw6sys_lifo_r_push (sys_context, list_r, (void *) str1);
    str2 = lw6sys_str_copy (sys_context, _TEST_LIST_STR2);
    lw6sys_fifo_r_push (sys_context, list_r, (void *) str2);
    str3 = lw6sys_str_copy (sys_context, _TEST_LIST_STR3);
    lw6sys_lifo_r_push (sys_context, list_r, (void *) str3);
    str4 = lw6sys_str_copy (sys_context, _TEST_LIST_STR4);
    lw6sys_fifo_r_push (sys_context, list_r, (void *) str4);
    list_r_copy = lw6sys_list_r_dup (sys_context, list_r, (lw6sys_dup_func_t) lw6sys_str_copy);
    ret = ret && LW6SYS_TEST_ACK (lw6sys_list_r_length (sys_context, list_r_copy) == 4);
    lw6sys_list_r_free (sys_context, list_r_copy);
    lw6sys_list_r_free (sys_context, list_r);
  }

  {
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing final pop behavior"));

    list_r = lw6sys_list_r_new (sys_context, NULL);
    if (LW6SYS_TEST_ACK (list_r))
      {
	lw6sys_list_r_pop_front (sys_context, list_r);
	if (LW6SYS_TEST_ACK (list_r) && LW6SYS_TEST_ACK (lw6sys_list_r_length (sys_context, list_r) == 0))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("final pop front didn't free list_r"));
	    lw6sys_list_r_pop_front (sys_context, list_r);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("yet another pop front on list_r does not harm"));

	    lw6sys_list_r_free (sys_context, list_r);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("final pop front did free list_r"));
	    ret = 0;
	  }
      }

    list_r = lw6sys_list_r_new (sys_context, NULL);
    if (LW6SYS_TEST_ACK (list_r))
      {
	lw6sys_list_r_pop_back (sys_context, list_r);
	if (LW6SYS_TEST_ACK (list_r) && LW6SYS_TEST_ACK (lw6sys_list_r_length (sys_context, list_r) == 0))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("final pop back didn't free list_r"));
	    lw6sys_list_r_pop_back (sys_context, list_r);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("yet another pop back on list_r does not harm"));
	    lw6sys_list_r_free (sys_context, list_r);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("final pop back did free list_r"));
	    ret = 0;
	  }
      }
  }
  {
    int i = 0;
    int *d = NULL;
    int limit = 0;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing list_r filter"));
    list_r = lw6sys_list_r_new (sys_context, lw6sys_free_callback);
    if (LW6SYS_TEST_ACK (list_r))
      {
	for (i = 0; i < _TEST_LIST_FILTER_NB_ITEMS; ++i)
	  {
	    d = (int *) LW6SYS_MALLOC (sys_context, sizeof (int));
	    if (LW6SYS_TEST_ACK (d))
	      {
		(*d) = lw6sys_random (sys_context, _TEST_LIST_FILTER_RANGE);
		lw6sys_list_r_push_front (sys_context, list_r, d);
	      }
	  }
	limit = (2 * _TEST_LIST_FILTER_RANGE) / 3;
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("1st filter limit=%d"), limit);
	lw6sys_list_r_filter (sys_context, list_r, list_filter_func, &limit);
	limit = _TEST_LIST_FILTER_RANGE / 3;
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("2nd filter limit=%d"), limit);
	lw6sys_list_r_filter (sys_context, list_r, list_filter_func, &limit);
	limit = -_TEST_LIST_FILTER_RANGE / 10;
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("3rd filter limit=%d"), limit);
	lw6sys_list_r_filter (sys_context, list_r, list_filter_func, &limit);
	lw6sys_list_r_free (sys_context, list_r);
      }
  }

  {
    int list_len = 0;
    int list_r_len = 0;
    lw6sys_list_t *list = NULL;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("transfer on list_r"));
    list_r = lw6sys_list_r_new (sys_context, lw6sys_free_callback);
    lw6sys_list_r_push_front (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR1));
    lw6sys_list_r_push_front (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR2));
    lw6sys_list_r_push_front (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR3));
    list = lw6sys_list_r_transfer_to (sys_context, list_r);
    if (LW6SYS_TEST_ACK (list))
      {
	list_len = lw6sys_list_length (sys_context, list);
	list_r_len = lw6sys_list_r_length (sys_context, list_r);
	if (LW6SYS_TEST_ACK (list_len == 3) && LW6SYS_TEST_ACK (list_r_len == 0))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("transfer OK, sizes match list_len=%d list_r_len=%d"), list_len, list_r_len);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("transfer failed, sizes do not match list_len=%d list_r_len=%d"), list_len, list_r_len);
	    ret = 0;
	  }
	lw6sys_list_free (sys_context, list);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("transfer failed, list is NULL"));
	ret = 0;
      }
    lw6sys_list_r_free (sys_context, list_r);
  }

  {
    int list_len = 0;
    int list_r_len = 0;
    lw6sys_list_t *list = NULL;
    int i = 0;

    for (i = 0; i < 4; ++i)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("transfer on list_r step %d"), i);
	list = lw6sys_list_new (sys_context, lw6sys_free_callback);
	list_r = lw6sys_list_r_new (sys_context, lw6sys_free_callback);

	if (i < 1)
	  {
	    lw6sys_list_push_front (sys_context, &list, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR1));
	  }
	else
	  {
	    lw6sys_list_r_push_front (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR1));
	  }

	if (i < 2)
	  {
	    lw6sys_list_push_front (sys_context, &list, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR2));
	  }
	else
	  {
	    lw6sys_list_r_push_front (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR2));
	  }


	if (i < 3)
	  {
	    lw6sys_list_push_front (sys_context, &list, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR3));
	  }
	else
	  {
	    lw6sys_list_r_push_front (sys_context, list_r, (void *) lw6sys_str_copy (sys_context, _TEST_LIST_STR3));
	  }

	list_len = lw6sys_list_length (sys_context, list);
	list_r_len = lw6sys_list_r_length (sys_context, list_r);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("ready to transfer, sizes list_len=%d list_r_len=%d"), list_len, list_r_len);

	lw6sys_list_r_transfer_from (sys_context, list_r, &list);
	if (LW6SYS_TEST_ACK (list))
	  {
	    list_len = lw6sys_list_length (sys_context, list);
	    list_r_len = lw6sys_list_r_length (sys_context, list_r);
	    if (LW6SYS_TEST_ACK (list_len == 0) && LW6SYS_TEST_ACK (list_r_len == 3))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("transfer OK, sizes match list_len=%d list_r_len=%d"), list_len, list_r_len);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("transfer failed, sizes do not match list_len=%d list_r_len=%d"), list_len, list_r_len);
		ret = 0;
	      }
	    lw6sys_list_free (sys_context, list);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("transfer failed, list is NULL"));
	    ret = 0;
	  }
	lw6sys_list_r_free (sys_context, list_r);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in log.c
 */
static void
_test_log ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int old_log_level;
    int old_log_backtrace_mode;
    int log_level;
    int log_backtrace_mode;

    old_log_level = lw6sys_log_get_level (sys_context);
    old_log_backtrace_mode = lw6sys_log_get_backtrace_mode (sys_context);

    for (log_level = LW6SYS_LOG_ERROR_ID; log_level <= LW6SYS_LOG_DEBUG_ID; ++log_level)
      {
	for (log_backtrace_mode = LW6SYS_LOG_BACKTRACE_MODE_FUNC; log_backtrace_mode <= LW6SYS_LOG_BACKTRACE_MODE_FULL; ++log_backtrace_mode)
	  {
	    lw6sys_log_set_level (sys_context, log_level);
	    lw6sys_log_set_backtrace_mode (sys_context, log_backtrace_mode);

	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing log_notice level=%d backtrace_mode=%d"), log_level, log_backtrace_mode);
	    lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("testing log_info level=%d backtrace_mode=%d"), log_level, log_backtrace_mode);
	    // normally, this one does now show with defaults settings...
	    lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("testing log_debug level=%d backtrace_mode=%d"), log_level, log_backtrace_mode);
	  }
      }

    lw6sys_log_set_level (sys_context, old_log_level);
    lw6sys_log_set_backtrace_mode (sys_context, old_log_backtrace_mode);

  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing (more) functions in log.c
 */
static void
_test_log_more ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    /*
     * WARNING & ERROR tested only in full test mode
     */
    lw6sys_log_set_dialog_timeout (sys_context, _TEST_LOG_MORE_DIALOG_TIMEOUT);
    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("this is a /TEST/ warning"));
    lw6sys_log (sys_context, LW6SYS_LOG_ERROR, _x_ ("this is a /TEST/ error"));
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing (even more) functions in log.c
 */
static void
_test_log_even_more ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int console_state = 0;

    console_state = lw6sys_log_get_console_state (sys_context);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("console state is %d"), console_state);

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("now, testing console disabling, next displayed message should be about bar, not foo"));
    lw6sys_log_set_console_state (sys_context, 0);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("message about foo, should appear in log only"));
    lw6sys_log_set_console_state (sys_context, 1);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("message about bar, should appear in both log file and console"));

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("setting console state back to %d"), console_state);
    lw6sys_log_set_console_state (sys_context, console_state);

  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in math.c
 */
static void
_test_math ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    float y = 0.0f;
    float s = 0.0f;
    float x = 0.0f;
    float v1 = 0.0f;
    int ix, iy;
    float angle = 0.0f;
    int t, b;
    int lin_lin2log = 0;
    float log_lin2log = 0.0f;
    float deg = 0.0f;
    float rad = 0.0f;

    for (v1 = _TEST_MATH_POLY_WY1Y2S1_V1_MIN;
	 v1 <= _TEST_MATH_POLY_WY1Y2S1_V1_MAX; v1 += (_TEST_MATH_POLY_WY1Y2S1_V1_MAX - _TEST_MATH_POLY_WY1Y2S1_V1_MIN) / _TEST_MATH_POLY_WY1Y2S1_NB_V1S)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("wy1y2s1 v1=%f"), v1);
	for (x = 0.0f; x <= _TEST_MATH_POLY_WY1Y2S1_W; x += _TEST_MATH_POLY_WY1Y2S1_W / _TEST_MATH_POLY_WY1Y2S1_NB_STEPS)
	  {
	    lw6sys_math_poly_wy1y2s1 (sys_context, &y, &s, x, _TEST_MATH_POLY_WY1Y2S1_W, _TEST_MATH_POLY_WY1Y2S1_Y1, _TEST_MATH_POLY_WY1Y2S1_Y2, v1);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("wy1y2s1 x=%f y=%f s=%f"), x, y, s);
	  }
      }
    for (iy = _TEST_MATH_ANGLE_XY_MIN; iy <= _TEST_MATH_ANGLE_XY_MAX; ++iy)
      {
	for (ix = _TEST_MATH_ANGLE_XY_MIN; ix <= _TEST_MATH_ANGLE_XY_MAX; ++ix)
	  {
	    angle = lw6sys_math_angle_360 (sys_context, ix, iy);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("x=%d y=%d angle=%f"), ix, iy, angle);
	  }
      }

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		_x_ ("heartbeat period=%d y1=%f y2=%f"), _TEST_MATH_HEARTBEAT_PERIOD, _TEST_MATH_HEARTBEAT_Y1, _TEST_MATH_HEARTBEAT_Y2);
    for (t = 0; t < _TEST_MATH_HEARTBEAT_N; t++)
      {
	y = lw6sys_math_heartbeat (sys_context, t, _TEST_MATH_HEARTBEAT_PERIOD, _TEST_MATH_HEARTBEAT_Y1, _TEST_MATH_HEARTBEAT_Y2);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("heartbeat t=%d y=%f"), t, y);
      }

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("blink period=%d"), _TEST_MATH_BLINK_PERIOD);
    for (t = 0; t < _TEST_MATH_BLINK_N; t++)
      {
	b = lw6sys_math_blink (sys_context, t, _TEST_MATH_BLINK_PERIOD);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("blink t=%d b=%d"), t, b);
      }

    lin_lin2log = lw6sys_math_log2lin (sys_context, _TEST_MATH_LIN2LOG_LOG, _TEST_MATH_LIN2LOG_BASE);
    if (LW6SYS_TEST_ACK (lin_lin2log == _TEST_MATH_LIN2LOG_LIN))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("OK, logarithmic to linear conversion gives %f -> %d"), _TEST_MATH_LIN2LOG_LOG, lin_lin2log);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_ ("logarithmic to linear conversion gives %f -> %d, expected %d"), _TEST_MATH_LIN2LOG_LOG, lin_lin2log, _TEST_MATH_LIN2LOG_LIN);
	ret = 0;
      }
    log_lin2log = lw6sys_math_lin2log (sys_context, _TEST_MATH_LIN2LOG_LIN, _TEST_MATH_LIN2LOG_BASE);
    if (LW6SYS_TEST_ACK (roundf (log_lin2log) == roundf (_TEST_MATH_LIN2LOG_LOG)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("OK, logarithmic to linear conversion gives %d -> %f"), _TEST_MATH_LIN2LOG_LIN, log_lin2log);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_ ("logarithmic to linear conversion gives %d -> %f, expected %f"), _TEST_MATH_LIN2LOG_LIN, log_lin2log, _TEST_MATH_LIN2LOG_LOG);
	ret = 0;
      }
    deg = lw6sys_math_rad2deg (M_PI);
    if (!LW6SYS_TEST_ACK (abs (deg - 180.0f) < 0.001f))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("rad2deg %f -> %f, expected %f"), M_PI, deg, 180.0f);
	ret = 0;
      }
    rad = lw6sys_math_deg2rad (180.0f);
    if (!LW6SYS_TEST_ACK (abs (rad - M_PI) < 0.001f))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("deg2rad %f -> %f, expected %f"), 180.0f, rad, M_PI);
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in mem.c
 */
static void
_test_mem ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;
  char *ptr = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing MALLOC/FREE (%d bytes)"), MALLOC_SIZE);
  ptr = (char *) LW6SYS_MALLOC (sys_context, MALLOC_SIZE * sizeof (char));
  ret = ret && (ptr != NULL);
  LW6SYS_FREE (sys_context, ptr);
  // same tests without using macros, fixes link when LW6_OPTIMZE is defined
  ptr = (char *) lw6sys_malloc (sys_context, MALLOC_SIZE, __FILE__, __LINE__, __FUNCTION__);
  if (LW6SYS_TEST_ACK (ptr))
    {
      lw6sys_free (sys_context, ptr, __FILE__, __LINE__, __FUNCTION__);
    }
  ptr = (char *) lw6sys_calloc (sys_context, MALLOC_SIZE, __FILE__, __LINE__, __FUNCTION__);
  if (LW6SYS_TEST_ACK (ptr))
    {
      ptr = lw6sys_realloc (sys_context, ptr, MALLOC_SIZE + 1, __FILE__, __LINE__, __FUNCTION__);
      if (LW6SYS_TEST_ACK (ptr))
	{
	  lw6sys_free (sys_context, ptr, __FILE__, __LINE__, __FUNCTION__);
	}
    }
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d megs"), lw6sys_megabytes_available (sys_context));
  if (lw6sys_is_big_endian (sys_context))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("big endian"));
    }
  if (lw6sys_is_little_endian (sys_context))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("little endian"));
    }

  ret = ret && lw6sys_check_types_size (sys_context);

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in mutex.c
 */
static void
_test_mutex ()
{
  int ret = 0;
  lw6sys_context_t *sys_context = NULL;
  lw6sys_mutex_t *mutex = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i = 0;
    u_int64_t end_timestamp = 0;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing mutex functions"));
    mutex = LW6SYS_MUTEX_CREATE (sys_context);
    if (LW6SYS_TEST_ACK (mutex))
      {
	if (LW6SYS_TEST_ACK (LW6SYS_MUTEX_LOCK (sys_context, mutex)))
	  {
	    if (LW6SYS_TEST_ACK (!LW6SYS_MUTEX_TRYLOCK (sys_context, mutex)))
	      {
		if (LW6SYS_TEST_ACK (LW6SYS_MUTEX_UNLOCK (sys_context, mutex)))
		  {
		    ret = 1;
		  }
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("strange, trylock on locked mutex should return 0"));
	      }
	  }

	end_timestamp = lw6sys_get_timestamp (sys_context) + _TEST_MUTEX_DELAY;
	while (lw6sys_get_timestamp (sys_context) < end_timestamp)
	  {
	    if (LW6SYS_MUTEX_LOCK (sys_context, mutex))
	      {
		LW6SYS_MUTEX_UNLOCK (sys_context, mutex);
		i++;
	      }
	  }
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d locks per second"), (i * 1000) / _TEST_MUTEX_DELAY);
	LW6SYS_MUTEX_DESTROY (sys_context, mutex);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing options functions
 */
static void
_test_options ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *path;

    path = lw6sys_get_default_user_dir (sys_context);
    if (LW6SYS_TEST_ACK (path))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("default user dir is \"%s\""), path);
	LW6SYS_FREE (sys_context, path);
      }
    else
      {
	ret = 0;
      }

    path = lw6sys_get_default_prefix (sys_context);
    if (LW6SYS_TEST_ACK (path))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("default prefix is \"%s\""), path);
	LW6SYS_FREE (sys_context, path);
      }
    else
      {
	ret = 0;
      }

    lw6sys_options_log_defaults (sys_context);
  }

  {
    char *path;
    const int argc = _TEST_ARGC;
    const char *argv[_TEST_ARGC] = { _TEST_ARGV0, _TEST_ARGV1, _TEST_ARGV2, _TEST_ARGV3 };

    path = lw6sys_get_user_dir (sys_context, argc, argv);
    if (LW6SYS_TEST_ACK (path))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("user dir is \"%s\""), path);
	LW6SYS_FREE (sys_context, path);
      }
    else
      {
	ret = 0;
      }

    path = lw6sys_get_prefix (sys_context, argc, argv);
    if (LW6SYS_TEST_ACK (path))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("prefix is \"%s\""), path);
	LW6SYS_FREE (sys_context, path);
      }
    else
      {
	ret = 0;
      }

    lw6sys_options_log (sys_context, argc, argv);
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in nop.c
 */
static void
_test_nop ()
{
  int ret = 0;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    ret = lw6sys_true () && !lw6sys_false ();
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Used to test lw6sys_path_split
 */
static void
path_split_func (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  char *str = (char *) data;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("path part \"%s\""), str);
}

static int
_dir_list_filter (lw6sys_context_t * sys_context, void *func_data, char *file)
{
  int ret = 0;

  if (lw6sys_checksum_str (sys_context, file) % 2)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("keep \"%s\" (func_data=\"%s\")"), file, (char *) func_data);
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("discard \"%s\""), file);
    }

  return ret;
}

static void
_dir_list_print (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  char *file = (char *) data;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("entry \"%s\""), file);
}

/*
 * Testing functions in path.c
 */
static void
_test_path ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *path = NULL;
    char *path2 = NULL;
    int n = 0;
    int relative = 0;
    int cwd = 0;
    lw6sys_list_t *list = NULL;
    char *data_dir = NULL;
    char *needle = NULL;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("add/strip slash"));
    path = lw6sys_path_add_slash (sys_context, PATH_WITHOUT_SLASH);
    if (LW6SYS_TEST_ACK (path))
      {
	ret = ret && (strcmp (path, PATH_WITH_SLASH) == 0);
	LW6SYS_FREE (sys_context, path);
      }
    path = lw6sys_path_strip_slash (sys_context, PATH_WITH_SLASH);
    if (LW6SYS_TEST_ACK (path))
      {
	ret = ret && (strcmp (path, PATH_WITHOUT_SLASH) == 0);
	LW6SYS_FREE (sys_context, path);
      }
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("concat path"));
    path = lw6sys_path_concat (sys_context, PATH_WITH_SLASH, PATH_WITH_SLASH);
    if (LW6SYS_TEST_ACK (path))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("concatenated path = \"%s\""), path);
	ret = ret && (strcmp (path, PATH_CONCAT) == 0);
	LW6SYS_FREE (sys_context, path);
      }
    relative = lw6sys_path_is_relative (sys_context, PATH_RELATIVE);
    if (LW6SYS_TEST_ACK (relative))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("path \"%s\" is relative"), PATH_RELATIVE);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("path \"%s\" not considered relative"), PATH_RELATIVE);
	ret = 0;
      }
    cwd = lw6sys_path_is_cwd (sys_context, _TEST_PATH_CWD);
    if (LW6SYS_TEST_ACK (cwd))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("path \"%s\" is cwd"), _TEST_PATH_CWD);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("path \"%s\" not considered being cwd"), _TEST_PATH_CWD);
	ret = 0;
      }
    path = lw6sys_path_parent (sys_context, PATH_CONCAT);
    if (LW6SYS_TEST_ACK (path))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("parent path = \"%s\""), path);
	ret = ret && (strcmp (path, PATH_CONCAT_PARENT) == 0);
	LW6SYS_FREE (sys_context, path);
      }
    path = lw6sys_path_unparent (sys_context, PATH_RELATIVE);
    if (LW6SYS_TEST_ACK (path))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unparent path = \"%s\""), path);
	ret = ret && (strcmp (path, PATH_RELATIVE_UNPARENT) == 0);
	LW6SYS_FREE (sys_context, path);
      }
    path = lw6sys_path_unparent_no_malloc (sys_context, PATH_RELATIVE);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unparent path = \"%s\""), path);
    ret = ret && (strcmp (path, PATH_RELATIVE_UNPARENT) == 0);

    list = lw6sys_path_split (sys_context, _TEST_PATH_SPLIT);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("splitting \"%s\""), _TEST_PATH_SPLIT);
    if (LW6SYS_TEST_ACK (list))
      {
	lw6sys_list_map (sys_context, list, path_split_func, NULL);
	lw6sys_list_free (sys_context, list);
      }

    path = lw6sys_get_home (sys_context);
    if (LW6SYS_TEST_ACK (path))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("listing dir \"%s\""), path);
	list = lw6sys_dir_list (sys_context, path, _dir_list_filter, _TEST_DIR_LIST_FUNC_DATA, &n);
	if (LW6SYS_TEST_ACK (list))
	  {
	    lw6sys_list_map (sys_context, list, _dir_list_print, NULL);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d items found in \"%s\""), n, path);
	    lw6sys_list_free (sys_context, list);
	  }
	path2 = lw6sys_env_concat (sys_context, path, _TEST_PATH_LIST_OTHER);
	if (LW6SYS_TEST_ACK (path2))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("listing path \"%s\""), path2);
	    list = lw6sys_path_list (sys_context, path2, _dir_list_filter, _TEST_DIR_LIST_FUNC_DATA, &n);
	    if (LW6SYS_TEST_ACK (list))
	      {
		lw6sys_list_map (sys_context, list, _dir_list_print, NULL);
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d items found in \"%s\""), n, path2);
		lw6sys_list_free (sys_context, list);
	      }
	    LW6SYS_FREE (sys_context, path2);
	  }
	LW6SYS_FREE (sys_context, path);
      }
    path = lw6sys_find_in_dir_and_path (sys_context, _FIND_IN_DIR_AND_PATH_DIR, _FIND_IN_DIR_AND_PATH_PATH, _FIND_IN_DIR_AND_PATH_FILE);
    if (LW6SYS_TEST_ACK (path))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_
		    ("found \"%s\" with dir=\"%s\" path=\"%s\" file=\"%s\""),
		    path, _FIND_IN_DIR_AND_PATH_DIR, _FIND_IN_DIR_AND_PATH_PATH, _FIND_IN_DIR_AND_PATH_FILE);
	LW6SYS_FREE (sys_context, path);
      }
    path = lw6sys_path_file_only (sys_context, _TEST_PATH_FILE_ONLY);
    if (LW6SYS_TEST_ACK (path))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("file only for \"%s\" is \"%s\""), _TEST_PATH_FILE_ONLY, path);
	LW6SYS_FREE (sys_context, path);
      }

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("dir_exists"));
    data_dir = lw6sys_get_default_data_dir (sys_context);
    if (LW6SYS_TEST_ACK (data_dir))
      {
	needle = lw6sys_build_get_home_url ();
	if (lw6sys_dir_exists_with_readme (sys_context, data_dir))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("OK, \"%s\" exists"), data_dir);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_
			("directory \"%s\" does not exist, while this is strange, test is not considered as broken, but other tests in other modules are likely to fail"),
			data_dir);
	  }
	if (lw6sys_dir_exists_with_readme (sys_context, data_dir))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("OK, \"%s\" exists and has a README"), data_dir);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_
			("directory \"%s\" does not exist or does not have a README, while this is strange, test is not considered as broken, but other tests in other modules are likely to fail"),
			data_dir);
	  }
	if (lw6sys_dir_exists_with_readme_containing_text (sys_context, data_dir, needle))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("OK, \"%s\" exists and has a README, and this file contains \"%s\""), data_dir, needle);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_
			("directory \"%s\" does not exist or does not have a README, while this is strange, test is not considered as broken, but other tests in other modules are likely to fail (needle=\"%s\")"),
			data_dir, needle);
	  }
	LW6SYS_FREE (sys_context, data_dir);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_process_callback1 (lw6sys_context_t * sys_context, void *data)
{
  while (1)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("first process callback, will try and loop forever, data=\"%s\""), (char *) data);
      lw6sys_snooze (sys_context);
    }
}

static void
_process_callback2 (lw6sys_context_t * sys_context, void *data)
{
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("second process callback, will terminate now, data=\"%s\""), (char *) data);
}

/*
 * Testing functions in process.c
 */
static void
_test_process ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    u_int64_t pid1 = 0LL;
    u_int64_t pid2 = 0LL;

    if (lw6sys_process_is_fully_supported (sys_context))
      {
	pid1 = lw6sys_process_fork_and_call (sys_context, _process_callback1, _TEST_PROCESS_CALLBACK1_STR);
	if (pid1 > 0)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("first child %" LW6SYS_PRINTF_LL "d running"), (long long) pid1);
	    pid2 = lw6sys_process_fork_and_call (sys_context, _process_callback2, _TEST_PROCESS_CALLBACK2_STR);
	    if (pid2 > 0)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("second child %" LW6SYS_PRINTF_LL "d running"), (long long) pid2);
		lw6sys_sleep (sys_context, _TEST_PROCESS_DELAY);
		if (LW6SYS_TEST_ACK (lw6sys_process_kill_1_9 (sys_context, pid2)))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("second child %" LW6SYS_PRINTF_LL "d killed"), (long long) pid2);
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't kill second child %" LW6SYS_PRINTF_LL "d"), (long long) pid2);
		    ret = 0;
		  }
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't run second child"));
		ret = 0;
	      }
	    if (LW6SYS_TEST_ACK (lw6sys_process_kill_1_9 (sys_context, pid1)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("first child %" LW6SYS_PRINTF_LL "d killed"), (long long) pid1);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't kill first child %" LW6SYS_PRINTF_LL "d"), (long long) pid1);
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't run first child"));
	    ret = 0;
	  }
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_ ("platform does not fully support fork & kill, the game should still run, but some tests won't be performed"));
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in progress.c
 */
static void
_test_progress ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_progress_t progress;
    lw6sys_progress_t progress1;
    lw6sys_progress_t progress2;
    lw6sys_progress_t progress3;
    lw6sys_progress_t progress4;
    lw6sys_progress_t progress5;
    float value = 0.0f;
    int i = 0;

    lw6sys_progress_default (sys_context, &progress, &value);
    for (i = 0; i < _TEST_PROGRESS_N; ++i)
      {
	lw6sys_progress_update (sys_context, &progress, 0, _TEST_PROGRESS_N, i);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("progress=%0.2f"), *(progress.value));
      }
    lw6sys_progress_begin (sys_context, &progress);
    lw6sys_progress_half (sys_context, &progress);
    lw6sys_progress_end (sys_context, &progress);
    lw6sys_progress_split (sys_context, &progress1, &progress2, &progress);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		_x_ ("progress1 %0.2f/%0.2f progress2 %0.2f/%0.2f"), progress1.min, progress1.max, progress2.min, progress2.max);
    lw6sys_progress_split_here (sys_context, &progress1, &progress2, &progress, _TEST_PROGRESS_HERE);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		_x_ ("progress1 %0.2f/%0.2f progress2 %0.2f/%0.2f"), progress1.min, progress1.max, progress2.min, progress2.max);
    lw6sys_progress_split3 (sys_context, &progress1, &progress2, &progress3, &progress);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		_x_
		("progress1 %0.2f/%0.2f progress2 %0.2f/%0.2f progress3 %0.2f/%0.2f"),
		progress1.min, progress1.max, progress2.min, progress2.max, progress3.min, progress3.max);
    lw6sys_progress_split4 (sys_context, &progress1, &progress2, &progress3, &progress4, &progress);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		_x_
		("progress1 %0.2f/%0.2f progress2 %0.2f/%0.2f progress3 %0.2f/%0.2f progress4 %0.2f/%0.2f"),
		progress1.min, progress1.max, progress2.min, progress2.max, progress3.min, progress3.max, progress4.min, progress4.max);
    lw6sys_progress_split5 (sys_context, &progress1, &progress2, &progress3, &progress4, &progress5, &progress);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		_x_
		("progress1 %0.2f/%0.2f progress2 %0.2f/%0.2f progress3 %0.2f/%0.2f progress4 %0.2f/%0.2f progress5 %0.2f/%0.2f"),
		progress1.min, progress1.max, progress2.min, progress2.max,
		progress3.min, progress3.max, progress4.min, progress4.max, progress5.min, progress5.max);
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in random.c
 */
static void
_test_random ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i = 0;
    float f = 0.0f;

    i = lw6sys_random (sys_context, _TEST_RANDOM_RANGE);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("here's a random number between 0 and %d: %d"), _TEST_RANDOM_RANGE, i);
    f = lw6sys_random_float (sys_context, _TEST_RANDOM_FLOAT_MIN, _TEST_RANDOM_FLOAT_MAX);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("here's a random float number between %f and %f: %f"), _TEST_RANDOM_FLOAT_MIN, _TEST_RANDOM_FLOAT_MAX, f);
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in signal.c
 *
 * Not declared static so that backtrace is more meaningfull.
 */
int
_lw6sys_test_signal (lw6sys_context_t * sys_context)
{
  int ret = 1;
  int i;

  if (!LW6SYS_TEST_ACK (!lw6sys_signal_poll_quit (sys_context)))
    {
      ret = 0;
    }
  lw6sys_signal_custom (sys_context, _TEST_SIGNAL_TRAP_ERRORS);
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("our own SIGTERM and SIGHUP signals are in place, try typing CTRL+C for instance"));
  for (i = 0; i < _TEST_SIGNAL_LOOPS; ++i)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit=%d"), lw6sys_signal_poll_quit (sys_context));
      lw6sys_snooze (sys_context);
    }
  lw6sys_signal_send_quit (sys_context);
  if (!LW6SYS_TEST_ACK (lw6sys_signal_poll_quit (sys_context)))
    {
      ret = 0;
    }
  lw6sys_signal_term_handler (0);
  lw6sys_signal_int_handler (0);
  lw6sys_signal_hup_handler (0);
  lw6sys_signal_default (sys_context);

  return ret;
}

/*
 * Testing functions in signal.c
 */
static void
_test_signal ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    if (!_lw6sys_test_signal (sys_context))
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Not declared static so that backtrace is more meaningfull.
 */
int
_lw6sys_test_signal_more (lw6sys_context_t * sys_context)
{
  int ret = 1;

  lw6sys_signal_segv_handler (0);
  lw6sys_signal_fpe_handler (0);
  lw6sys_signal_default (sys_context);

  return ret;
}

/*
 * Testing (more) functions in signal.c
 */
static void
_test_signal_more ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    if (!LW6SYS_TEST_ACK (_lw6sys_test_signal_more (sys_context)))
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

typedef struct _twin_s
{
  int i1;
  int i2;
} _twin_t;

static int
_sort_twin_callback (lw6sys_context_t * sys_context, void *func_data, const void *ptr_a, const void *ptr_b)
{
  int ret = 0;
  const _twin_t *value_a = (const _twin_t *) ptr_a;
  const _twin_t *value_b = (const _twin_t *) ptr_b;

  if (value_a->i1 < value_b->i1)
    {
      ret = -1;
    }
  else if (value_a->i1 > value_b->i1)
    {
      ret = 1;
    }
  else
    {
      if (value_a->i2 < value_b->i2)
	{
	  ret = -1;
	}
      else if (value_a->i2 > value_b->i2)
	{
	  ret = 1;
	}
    }

  if (ret < 0)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("(%d,%d) < (%d,%d), func_data=\"%s\""), value_a->i1, value_a->i2, value_b->i1, value_b->i2,
		  (const char *) func_data);
    }
  else if (ret > 0)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("(%d,%d) > (%d,%d), func_data=\"%s\""), value_a->i1, value_a->i2, value_b->i1, value_b->i2,
		  (const char *) func_data);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("(%d,%d) == (%d,%d), func_data=\"%s\""), value_a->i1, value_a->i2, value_b->i1, value_b->i2,
		  (const char *) func_data);
    }

  return ret;
}

static int
_sort_twin_desc_callback (lw6sys_context_t * sys_context, void *func_data, const void *ptr_a, const void *ptr_b)
{
  return _sort_twin_callback (sys_context, func_data, ptr_b, ptr_a);
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
    int array[_TEST_SORT_LENGTH] = _TEST_SORT_INT;
    int i;

    list = lw6sys_list_new (sys_context, NULL);
    if (LW6SYS_TEST_ACK (list))
      {
	for (i = 0; i < _TEST_SORT_LENGTH; ++i)
	  {
	    lw6sys_lifo_push (sys_context, &list, (void *) (array + i));
	  }
	lw6sys_sort (sys_context, &list, lw6sys_sort_int_callback, NULL);
	if (LW6SYS_TEST_ACK (list))
	  {
	    if (LW6SYS_TEST_ACK ((*((int *) (list->data))) == _TEST_SORT_INT_MIN))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("min sorted value is %d"), _TEST_SORT_INT_MIN);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("min sorted value is %d and should be %d"), *((int *) (list->data)), _TEST_SORT_INT_MIN);
		ret = 0;
	      }
	  }
	lw6sys_sort (sys_context, &list, lw6sys_sort_int_desc_callback, NULL);
	if (LW6SYS_TEST_ACK (list))
	  {
	    if (LW6SYS_TEST_ACK ((*((int *) (list->data))) == _TEST_SORT_INT_MAX))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("max sorted value is %d"), _TEST_SORT_INT_MAX);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("max sorted value is %d and should be %d"), *((int *) (list->data)), _TEST_SORT_INT_MAX);
		ret = 0;
	      }
	  }
	if (LW6SYS_TEST_ACK (list))
	  {
	    lw6sys_list_free (sys_context, list);
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

  {
    lw6sys_list_t *list = NULL;
    float array[_TEST_SORT_LENGTH] = _TEST_SORT_FLOAT;
    int i;

    list = lw6sys_list_new (sys_context, NULL);
    if (LW6SYS_TEST_ACK (list))
      {
	for (i = 0; i < _TEST_SORT_LENGTH; ++i)
	  {
	    lw6sys_lifo_push (sys_context, &list, (void *) (array + i));
	  }
	lw6sys_sort (sys_context, &list, lw6sys_sort_float_callback, NULL);
	if (LW6SYS_TEST_ACK (list))
	  {
	    if (LW6SYS_TEST_ACK ((*((float *) (list->data))) == _TEST_SORT_FLOAT_MIN))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("min sorted value is %f"), _TEST_SORT_FLOAT_MIN);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("min sorted value is %f and should be %f"), *((float *) (list->data)), _TEST_SORT_FLOAT_MIN);
		ret = 0;
	      }
	  }
	lw6sys_sort (sys_context, &list, lw6sys_sort_float_desc_callback, NULL);
	if (LW6SYS_TEST_ACK (list))
	  {
	    if (LW6SYS_TEST_ACK ((*((float *) (list->data))) == _TEST_SORT_FLOAT_MAX))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("max sorted value is %f"), _TEST_SORT_FLOAT_MAX);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("max sorted value is %f and should be %f"), *((float *) (list->data)), _TEST_SORT_FLOAT_MAX);
		ret = 0;
	      }
	  }
	if (LW6SYS_TEST_ACK (list))
	  {
	    lw6sys_list_free (sys_context, list);
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

  {
    lw6sys_list_t *list = NULL;
    char *array[_TEST_SORT_LENGTH] = _TEST_SORT_STR;
    int i;

    list = lw6sys_list_new (sys_context, NULL);
    if (LW6SYS_TEST_ACK (list))
      {
	for (i = 0; i < _TEST_SORT_LENGTH; ++i)
	  {
	    lw6sys_lifo_push (sys_context, &list, (void *) array[i]);
	  }
	lw6sys_sort (sys_context, &list, lw6sys_sort_str_callback, NULL);
	if (LW6SYS_TEST_ACK (list))
	  {
	    if (LW6SYS_TEST_ACK (!strcmp ((char *) (list->data), _TEST_SORT_STR_MIN)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("min sorted value is \"%s\""), _TEST_SORT_STR_MIN);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_ ("min sorted value is \"%s\" and should be \"%s\""), (char *) (list->data), _TEST_SORT_STR_MIN);
		ret = 0;
	      }
	  }
	lw6sys_sort (sys_context, &list, lw6sys_sort_str_desc_callback, NULL);
	if (LW6SYS_TEST_ACK (list))
	  {
	    if (LW6SYS_TEST_ACK (!strcmp ((char *) (list->data), _TEST_SORT_STR_MAX)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("max sorted value is \"%s\""), _TEST_SORT_STR_MAX);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_ ("max sorted value is \"%s\" and should be \"%s\""), (char *) (list->data), _TEST_SORT_STR_MAX);
		ret = 0;
	      }
	  }
	if (LW6SYS_TEST_ACK (list))
	  {
	    lw6sys_list_free (sys_context, list);
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

  {
    lw6sys_list_t *list = NULL;
    _twin_t array[_TEST_SORT_LENGTH] = _TEST_SORT_TWIN;
    _twin_t array_min = _TEST_SORT_TWIN_MIN;
    _twin_t array_max = _TEST_SORT_TWIN_MAX;
    int i;

    list = lw6sys_list_new (sys_context, NULL);
    if (LW6SYS_TEST_ACK (list))
      {
	for (i = 0; i < _TEST_SORT_LENGTH; ++i)
	  {
	    lw6sys_lifo_push (sys_context, &list, (void *) (array + i));
	  }
	lw6sys_sort (sys_context, &list, _sort_twin_callback, _TEST_SORT_TWIN_FUNC_DATA);
	if (LW6SYS_TEST_ACK (list))
	  {
	    if (LW6SYS_TEST_ACK (!memcmp ((void *) (list->data), (void *) &array_min, sizeof (_twin_t))))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("min sorted value is (%d,%d)"), array_min.i1, array_min.i2);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_ ("min sorted value is (%d,%d) and should be (%d,%d)"), ((_twin_t *) (list->data))->i1, ((_twin_t *) (list->data))->i2,
			    array_min.i1, array_min.i2);
		ret = 0;
	      }
	  }
	lw6sys_sort (sys_context, &list, _sort_twin_desc_callback, _TEST_SORT_TWIN_FUNC_DATA);
	if (LW6SYS_TEST_ACK (list))
	  {
	    if (LW6SYS_TEST_ACK (!memcmp ((void *) (list->data), (void *) &array_max, sizeof (_twin_t))))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("max sorted value is (%d,%d)"), array_max.i1, array_max.i2);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_ ("max sorted value is (%d,%d) and should be (%d,%d)"), ((_twin_t *) (list->data))->i1, ((_twin_t *) (list->data))->i2,
			    array_max.i1, array_max.i2);
		ret = 0;
	      }
	  }
	if (LW6SYS_TEST_ACK (list))
	  {
	    lw6sys_list_free (sys_context, list);
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

/*
 * Testing functions in spinlock.c
 */
static void
_test_spinlock ()
{
  int ret = 0;
  lw6sys_context_t *sys_context = NULL;
  lw6sys_spinlock_t *spinlock = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing spinlock functions"));
    spinlock = lw6sys_spinlock_create (sys_context);
    if (LW6SYS_TEST_ACK (spinlock))
      {
	if (LW6SYS_TEST_ACK (lw6sys_spinlock_lock (sys_context, spinlock)))
	  {
	    if (!LW6SYS_TEST_ACK (!lw6sys_spinlock_trylock (sys_context, spinlock)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("strange, trylock on locked spinlock should return 0"));
	      }
	    else
	      {
		if (LW6SYS_TEST_ACK (lw6sys_spinlock_unlock (sys_context, spinlock)))
		  {
		    ret = 1;
		  }
	      }
	  }
	lw6sys_spinlock_destroy (sys_context, spinlock);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Used to test lw6sys_split...
 */
static void
str_split_func (lw6sys_context_t * sys_context, void *func_data, void *data)
{
  char *str = (char *) data;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("split part \"%s\""), str);
}

/*
 * Testing functions in str.c
 */
static void
_test_str ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *str;
    lw6sys_list_t *list;
    int is_bin = 0;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing is_blank on \"%s\""), BLANK_STR);
    ret = ret && lw6sys_str_is_blank (sys_context, BLANK_STR);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing is_null_or_empty"));
    ret = ret && lw6sys_str_is_null_or_empty (sys_context, NULL)
      && lw6sys_str_is_null_or_empty (sys_context, LW6SYS_STR_EMPTY) && !lw6sys_str_is_null_or_empty (sys_context, BLANK_STR);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing NULL shield %p"), lw6sys_str_empty_if_null (sys_context, NULL));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing cleanup"));
    str = lw6sys_new_sprintf (sys_context, CLEANUP_FORMAT, CLEANUP_CONTROL, CLEANUP_ACCENT);
    if (LW6SYS_TEST_ACK (str))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("before cleanup, string is \%s\""), str);
	lw6sys_str_cleanup (sys_context, str);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("after cleanup, string is \"%s\""), str);
	lw6sys_str_cleanup_ascii7 (sys_context, str);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("after cleanup (ascii-7), string is \"%s\""), str);
	LW6SYS_FREE (sys_context, str);
      }
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("testing is_same"));
    ret = ret && lw6sys_str_is_same (sys_context, BLANK_STR, BLANK_STR)
      && lw6sys_str_is_same (sys_context, NULL, NULL)
      && (!lw6sys_str_is_same (sys_context, NULL, BLANK_STR))
      && (!lw6sys_str_is_same (sys_context, BLANK_STR, NULL))
      && (!lw6sys_str_is_same (sys_context, BLANK_STR, _TEST_REFORMAT_STR1)) && (!lw6sys_str_is_same_no_case (sys_context, BLANK_STR, _TEST_REFORMAT_STR1));
    ret = ret
      && lw6sys_str_starts_with (sys_context, STR_STARTS_WITH,
				 STR_STARTS_WITH_OK)
      && !lw6sys_str_starts_with (sys_context, STR_STARTS_WITH,
				  STR_STARTS_WITH_KO)
      && lw6sys_str_starts_with_no_case (sys_context, STR_STARTS_WITH,
					 STR_STARTS_WITH_OK) && !lw6sys_str_starts_with_no_case (sys_context, STR_STARTS_WITH, STR_STARTS_WITH_KO);
    str = lw6sys_str_reformat (sys_context, _TEST_REFORMAT_STR1, _TEST_REFORMAT_PREFIX1, _TEST_REFORMAT_COLUMNS2);
    if (LW6SYS_TEST_ACK (str))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("reformatted string is \"%s\""), str);
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }

    str = lw6sys_str_reformat (sys_context, _TEST_REFORMAT_STR2, _TEST_REFORMAT_PREFIX2, _TEST_REFORMAT_COLUMNS2);
    if (LW6SYS_TEST_ACK (str))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("reformatted string is \"%s\""), str);
	LW6SYS_FREE (sys_context, str);
      }
    else
      {
	ret = 0;
      }

    str = lw6sys_str_copy (sys_context, _TEST_REFORMAT_STR1);
    if (LW6SYS_TEST_ACK (str))
      {
	lw6sys_str_reformat_this (sys_context, str, _TEST_REFORMAT_COLUMNS1);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("reformatted string is \"%s\""), str);
	LW6SYS_FREE (sys_context, str);
      }

    str = lw6sys_str_copy (sys_context, _TEST_REFORMAT_STR2);
    if (LW6SYS_TEST_ACK (str))
      {
	lw6sys_str_reformat_this (sys_context, str, _TEST_REFORMAT_COLUMNS2);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("reformatted string is \"%s\""), str);
	LW6SYS_FREE (sys_context, str);
      }

    list = lw6sys_str_split (sys_context, _TEST_SPLIT_STR, _TEST_SPLIT_CHAR);
    if (LW6SYS_TEST_ACK (list))
      {
	lw6sys_list_map (sys_context, list, str_split_func, NULL);
	str = lw6sys_str_join (sys_context, list, _TEST_STR_GLUE);
	if (LW6SYS_TEST_ACK (str))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("joined string is \"%s\""), str);
	    LW6SYS_FREE (sys_context, str);
	  }
	lw6sys_list_free (sys_context, list);
      }

    list = lw6sys_str_split_no_0 (sys_context, _TEST_SPLIT_STR, _TEST_SPLIT_CHAR);
    if (LW6SYS_TEST_ACK (list))
      {
	lw6sys_list_map (sys_context, list, str_split_func, NULL);
	str = lw6sys_str_join (sys_context, list, _TEST_STR_GLUE);
	if (LW6SYS_TEST_ACK (str))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("joined string is \"%s\""), str);
	    LW6SYS_FREE (sys_context, str);
	  }
	lw6sys_list_free (sys_context, list);
      }

    list = lw6sys_str_split_config_item (sys_context, _TEST_SPLIT_STR);
    if (LW6SYS_TEST_ACK (list))
      {
	lw6sys_list_map (sys_context, list, str_split_func, NULL);
	str = lw6sys_str_join (sys_context, list, _TEST_STR_GLUE);
	if (LW6SYS_TEST_ACK (str))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("joined string is \"%s\""), str);
	    LW6SYS_FREE (sys_context, str);
	  }
	lw6sys_list_free (sys_context, list);
      }

    str = lw6sys_str_copy (sys_context, _TEST_STR_LOWER_UPPER);
    if (LW6SYS_TEST_ACK (str))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("original string=\"%s\""), str);
	lw6sys_str_tolower (sys_context, str);
	if (LW6SYS_TEST_ACK (lw6sys_str_is_same (sys_context, str, _TEST_STR_LOWERED)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lowered string=\"%s\""), str);
	    lw6sys_str_toupper (sys_context, str);
	    if (LW6SYS_TEST_ACK (lw6sys_str_is_same (sys_context, str, _TEST_STR_UPPERED)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("uppered string=\"%s\""), str);
		lw6sys_str_truncate (sys_context, str, _TEST_STR_TRUNCATE);
		if (LW6SYS_TEST_ACK (lw6sys_str_is_same (sys_context, str, _TEST_STR_TRUNCATED)))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("truncated string=\"%s\""), str);
		    lw6sys_str_truncate_middle (sys_context, str, _TEST_STR_TRUNCATE_MIDDLE_LEN, _TEST_STR_TRUNCATE_MIDDLE_STR);
		    if (LW6SYS_TEST_ACK (lw6sys_str_is_same (sys_context, str, _TEST_STR_MIDDLE_TRUNCATED)))
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("middle truncated string=\"%s\""), str);
		      }
		  }
	      }
	  }
	LW6SYS_FREE (sys_context, str);
      }

    str = lw6sys_str_random (sys_context, _TEST_STR_RANDOM_LEN);
    if (LW6SYS_TEST_ACK (str))
      {
	is_bin = lw6sys_str_is_bin (sys_context, str, strlen (str));
	if (LW6SYS_TEST_ACK (is_bin))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("random string considered binary -> good!"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_
			("random string *not* considered binary, this is strange, test isn't reported as failed since an error can always happen and a monkey could random-type a shakespeare masterpice, but still..."));
	  }
	lw6sys_str_cleanup_ascii7 (sys_context, str);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("random string of %d chars (after ascii7 cleanup) \"%s\""), (int) strlen (str), str);
	LW6SYS_FREE (sys_context, str);
      }
    str = lw6sys_str_random_words (sys_context, _TEST_STR_RANDOM_LEN);
    if (LW6SYS_TEST_ACK (str))
      {
	is_bin = lw6sys_str_is_bin (sys_context, str, strlen (str));
	if (LW6SYS_TEST_ACK (!is_bin))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("random string not considered binary -> good!"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("random string (with alnum words) considered binary"));
	    ret = 0;
	  }
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("random string (with alnum words) of %d chars \"%s\""), (int) strlen (str), str);
	LW6SYS_FREE (sys_context, str);
      }
    str = lw6sys_str_random_word (sys_context, _TEST_STR_RANDOM_LEN);
    if (LW6SYS_TEST_ACK (str))
      {
	is_bin = lw6sys_str_is_bin (sys_context, str, strlen (str));
	if (LW6SYS_TEST_ACK (!is_bin))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("random string not considered binary -> good!"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("random string (with one single word) considered binary"));
	    ret = 0;
	  }
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("random string (with one single word) of %d chars \"%s\""), (int) strlen (str), str);
	LW6SYS_FREE (sys_context, str);
      }
    str = lw6sys_id_ltoa (sys_context, lw6sys_generate_id_64 (sys_context));
    if (LW6SYS_TEST_ACK (str))
      {
	is_bin = lw6sys_str_is_bin (sys_context, str, strlen (str));
	if (LW6SYS_TEST_ACK (!is_bin))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("string \"%s\" (64-bit ID) considered *not* binary -> good!"), str);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("string \"%s\" (64-bit ID) considered binary, this is wrong"), str);
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, str);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in sdl.c
 */
static void
_test_sdl ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("register/unregister SDL"));
    ret = ret && lw6sys_sdl_register (sys_context) && lw6sys_sdl_unregister (sys_context);
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in serial.c
 */
static void
_test_serial ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int64_t test64 = 0;
    int32_t test32 = 0;
    int16_t test16 = 0;
    unsigned char buffer[LW6SYS_SIZEOF_INT64];

    lw6sys_serialize_int64 (sys_context, buffer, _TEST_SERIALIZE);
    test64 = lw6sys_unserialize_int64 (sys_context, buffer);
    if (!LW6SYS_TEST_ACK (test64 == _TEST_SERIALIZE))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("serialize failed on 64-bit integer (%d/%d)"), (int) test64, _TEST_SERIALIZE);
	ret = 0;
      }

    lw6sys_serialize_int32 (sys_context, buffer, _TEST_SERIALIZE);
    test32 = lw6sys_unserialize_int32 (sys_context, buffer);
    if (!LW6SYS_TEST_ACK (test32 == _TEST_SERIALIZE))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("serialize failed on 32-bit integer (%d/%d)"), (int) test32, _TEST_SERIALIZE);
	ret = 0;
      }

    lw6sys_serialize_int16 (sys_context, buffer, _TEST_SERIALIZE);
    test16 = lw6sys_unserialize_int16 (sys_context, buffer);
    if (!LW6SYS_TEST_ACK (test16 == _TEST_SERIALIZE))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("serialize failed on 16-bit integer (%d/%d)"), (int) test16, _TEST_SERIALIZE);
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in shape.c
 */
static void
_test_shape ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_whd_t shape = { _TEST_SHAPE_W, _TEST_SHAPE_H, _TEST_SHAPE_D };
    lw6sys_whd_t shape2 = { _TEST_SHAPE_W + 1, _TEST_SHAPE_H + 1, _TEST_SHAPE_D + 1 };
    lw6sys_whd_t shape_min = { _TEST_SHAPE_MIN_W, _TEST_SHAPE_MIN_H, _TEST_SHAPE_MIN_D };
    lw6sys_whd_t shape_max = { _TEST_SHAPE_MAX_W, _TEST_SHAPE_MAX_H, _TEST_SHAPE_MAX_D };
    lw6sys_xyz_t pos = { _TEST_SHAPE_X, _TEST_SHAPE_Y, _TEST_SHAPE_Z };
    int volume = 0;
    int surface = 0;

    if (LW6SYS_TEST_ACK (lw6sys_shape_check_min_max_whd (sys_context, &shape, &shape_min, &shape_max)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_
		    ("shape %dx%dx%d respects constraints min=%dx%dx%d max=%dx%dx%d"),
		    (int) shape.w, (int) shape.h, (int) shape.d,
		    _TEST_SHAPE_MIN_W, _TEST_SHAPE_MIN_H, _TEST_SHAPE_MIN_D, _TEST_SHAPE_MAX_W, _TEST_SHAPE_MAX_H, _TEST_SHAPE_MAX_D);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (lw6sys_shape_check_pos (sys_context, &shape, &pos)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_ ("pos %d,%d,%d is inside shape %dx%dx%d"), (int) pos.x, (int) pos.y, (int) pos.z, (int) shape.w, (int) shape.h, (int) shape.d);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (lw6sys_shape_is_same (sys_context, &shape, &shape) && !lw6sys_shape_is_same (sys_context, &shape, &shape2)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_ ("shape %dx%dx%d reported different as %dx%dx%d"),
		    (int) shape.w, (int) shape.h, (int) shape.d, (int) shape2.w, (int) shape2.h, (int) shape2.d);
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (lw6sys_shape_is_same_xy (sys_context, &shape, &shape) && !lw6sys_shape_is_same_xy (sys_context, &shape, &shape2)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_ ("shape %dx%d reported different as %dx%d"), (int) shape.w, (int) shape.h, (int) shape2.w, (int) shape2.h);
      }
    else
      {
	ret = 0;
      }
    volume = lw6sys_shape_volume_whd (sys_context, &shape);
    if (LW6SYS_TEST_ACK (volume == shape.w * shape.h * shape.d))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("shape volume for %dx%dx%d is %d"), shape.w, shape.h, shape.d, volume);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_ ("shape volume for %dx%dx%d is %d but should be %d"), shape.w, shape.h, shape.d, volume, shape.w * shape.h * shape.d);
	ret = 0;
      }
    surface = lw6sys_shape_surface_wh (sys_context, &shape);
    if (LW6SYS_TEST_ACK (surface == shape.w * shape.h))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("shape surface for %dx%d is %d"), shape.w, shape.h, surface);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("shape surface for %dx%d is %d but should be %d"), shape.w, shape.h, surface, shape.w * shape.h);
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in stream.c
 */
static void
_test_stream ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    const int argc = _TEST_ARGC;
    const char *argv[_TEST_ARGC] = { _TEST_ARGV0, _TEST_ARGV1, _TEST_ARGV2, _TEST_ARGV3 };
    FILE *f = NULL;
    char *user_dir = NULL;
    char *filename = NULL;
    char *str = NULL;

    user_dir = lw6sys_get_user_dir (sys_context, argc, argv);
    if (LW6SYS_TEST_ACK (user_dir))
      {
	filename = lw6sys_path_concat (sys_context, user_dir, _TEST_FILENAME);
	if (LW6SYS_TEST_ACK (filename))
	  {
	    f = fopen (filename, "wb");
	    if (LW6SYS_TEST_ACK (f))
	      {
		lw6sys_stream_str_to_file (sys_context, f, _TEST_CONTENT);
		fclose (f);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to open \"%s\" for writing"), filename);
		ret = 0;
	      }
	    f = fopen (filename, "r");
	    if (LW6SYS_TEST_ACK (f))
	      {
		str = lw6sys_stream_file_to_str (sys_context, f);
		if (LW6SYS_TEST_ACK (str))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("read \"%s\", %d bytes"), filename, (int) strlen (str));
		    if (LW6SYS_TEST_ACK (lw6sys_str_is_same (sys_context, str, _TEST_CONTENT)))
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("content is fine \"%s\""), _TEST_CONTENT);
		      }
		    else
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("content is wrong it's \"%s\" and should be \"%s\""), str, _TEST_CONTENT);
			ret = 0;
		      }
		    LW6SYS_FREE (sys_context, str);
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to read \"%s\""), filename);
		    ret = 0;
		  }
		fclose (f);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to open \"%s\" for reading"), filename);
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, filename);
	  }
	LW6SYS_FREE (sys_context, user_dir);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
thread_func (lw6sys_context_t * sys_context, void *callback_data)
{
  int i = 0;
  char *text = NULL;

  text = (char *) callback_data;
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("thread_callback text=\"%s\""), text);
  for (i = 0; i < THREAD_N; ++i)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("thread_callback step %d"), i + 1);
      lw6sys_sleep (sys_context, THREAD_SLEEP_CALLBACK);
    }
}

static void
thread_join_nofree (lw6sys_context_t * sys_context, void *callback_data)
{
  char *text = NULL;

  text = (char *) callback_data;
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("thread_join text=\"%s\""), text);
}

static void
thread_join (lw6sys_context_t * sys_context, void *callback_data)
{
  thread_join_nofree (sys_context, callback_data);
  LW6SYS_FREE (sys_context, callback_data);
}

typedef struct thread_stress_data_s
{
  int run;
  int count;
  int ret;
} thread_stress_data_t;

static void
thread_stress_func (lw6sys_context_t * sys_context, void *callback_data)
{
  thread_stress_data_t *thread_stress_data = NULL;
  void *child;

  thread_stress_data = (thread_stress_data_t *) callback_data;

  while (thread_stress_data->run)
    {
      child = lw6sys_thread_create (sys_context, NULL, NULL, NULL);
      if (child)
	{
	  lw6sys_thread_join (sys_context, child);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create child"));
	  thread_stress_data->ret = 0;
	}
      thread_stress_data->count++;
    }
}

/*
 * Testing functions in thread.c
 */
static void
_test_thread ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_thread_handler_t *thread_handler = NULL;
    char *text = NULL;
    int i = 0;
    thread_stress_data_t thread_stress_data;

    ret = 1;
    text = lw6sys_str_copy (sys_context, THREAD_TEXT);
    if (LW6SYS_TEST_ACK (text))
      {
	thread_handler = lw6sys_thread_create (sys_context, &thread_func, &thread_join, (void *) text);
	if (LW6SYS_TEST_ACK (thread_handler))
	  {
	    for (i = 0; i < THREAD_N; ++i)
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			    _x_ ("thread_main step %d, callback_done=%d"), i + 1, lw6sys_thread_is_callback_done (sys_context, thread_handler));
		lw6sys_sleep (sys_context, THREAD_SLEEP_MAIN);
	      }

	    lw6sys_thread_join (sys_context, thread_handler);
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

    text = lw6sys_str_copy (sys_context, THREAD_TEXT);
    if (LW6SYS_TEST_ACK (text))
      {
	thread_handler = lw6sys_thread_create (sys_context, &thread_func, &thread_join, (void *) text);
	if (LW6SYS_TEST_ACK (thread_handler))
	  {
	    if (LW6SYS_TEST_ACK (lw6sys_thread_wait_callback_done (sys_context, thread_handler)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("wait_callback_done over"));
		if (LW6SYS_TEST_ACK (lw6sys_thread_is_callback_done (sys_context, thread_handler)))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("is_callback_done returns true, looks good"));
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("is_callback_done returns false when wait_callback_done is over"));
		    ret = 0;
		  }
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("wait_callback_done problem"));
		ret = 0;
	      }

	    lw6sys_thread_join (sys_context, thread_handler);
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
    if (LW6SYS_TEST_ACK (ret))
      {
	thread_stress_data.run = 1;
	thread_stress_data.count = 0;
	thread_stress_data.ret = 1;
	thread_handler = lw6sys_thread_create (sys_context, &thread_stress_func, NULL, &thread_stress_data);
	if (LW6SYS_TEST_ACK (thread_handler))
	  {
	    lw6sys_sleep (sys_context, THREAD_STRESS_DURATION);
	    thread_stress_data.run = 0;
	    lw6sys_thread_join (sys_context, thread_handler);
	    ret = ret && thread_stress_data.ret;
	    if (LW6SYS_TEST_ACK (ret))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			    _x_ ("%d threads (%0.1f/sec)"), thread_stress_data.count, (float) (((float) thread_stress_data.count) / THREAD_STRESS_DURATION));
	      }
	  }
	else
	  {
	    ret = 0;
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in time.c
 */
static void
_test_time ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i = 0;
    int nb_steps = 0;
    int64_t last_uptime;
    int64_t uptime = 0;
    int64_t timestamp = 0;
    int32_t cycle = 0;
    char *rfc1123 = NULL;
    char *clf = NULL;
    char *uptime_str = NULL;

    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("timestamp %" LW6SYS_PRINTF_LL "d"), (long long) lw6sys_get_timestamp (sys_context));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("uptime %" LW6SYS_PRINTF_LL "d"), (long long) lw6sys_get_uptime (sys_context));
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("cycle %d"), lw6sys_get_cycle (sys_context));
    lw6sys_idle (sys_context);
    lw6sys_timer_update (sys_context, &timestamp, &uptime, &cycle);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		_x_ ("after idle timestamp=%" LW6SYS_PRINTF_LL "d uptime=%" LW6SYS_PRINTF_LL "d cycle=%d"), (long long) timestamp, (long long) uptime, cycle);
    lw6sys_snooze (sys_context);
    lw6sys_timer_update (sys_context, &timestamp, &uptime, &cycle);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		_x_ ("after snooze timestamp=%" LW6SYS_PRINTF_LL "d uptime=%"
		     LW6SYS_PRINTF_LL "d cycle=%d"), (long long) timestamp, (long long) uptime, cycle);
    last_uptime = lw6sys_get_uptime (sys_context);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("sleep %d seconds"), _TEST_SLEEP_TIME);
    lw6sys_sleep (sys_context, _TEST_SLEEP_TIME);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("delay %d msec"), _TEST_DELAY_TIME);
    lw6sys_delay (sys_context, _TEST_DELAY_TIME);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lasted %" LW6SYS_PRINTF_LL "d ms"), (long long) (lw6sys_get_uptime (sys_context) - last_uptime));
    last_uptime = lw6sys_get_uptime (sys_context);
    nb_steps = _TEST_SLEEP_TIME / _TEST_SLEEP_TIME_SHORT_STEP;
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("sleep %d seconds using %d %0.6f steps"), _TEST_SLEEP_TIME, nb_steps, _TEST_SLEEP_TIME_SHORT_STEP);
    for (i = 0; i < nb_steps; ++i)
      {
	lw6sys_sleep (sys_context, _TEST_SLEEP_TIME_SHORT_STEP);
      }
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("lasted %" LW6SYS_PRINTF_LL "d ms"), (long long) (lw6sys_get_uptime (sys_context) - last_uptime));

    rfc1123 = lw6sys_date_rfc1123 (sys_context, 0);
    if (LW6SYS_TEST_ACK (rfc1123))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("rfc1123 date for now is \"%s\""), rfc1123);
	LW6SYS_FREE (sys_context, rfc1123);
      }
    rfc1123 = lw6sys_date_rfc1123 (sys_context, _TEST_RFC1123_SECONDS_FROM_NOW);
    if (LW6SYS_TEST_ACK (rfc1123))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("rfc1123 date in %d seconds is \"%s\""), _TEST_RFC1123_SECONDS_FROM_NOW, rfc1123);
	LW6SYS_FREE (sys_context, rfc1123);
      }
    clf = lw6sys_date_clf (sys_context);
    if (LW6SYS_TEST_ACK (clf))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("clf date is \"%s\""), clf);
	LW6SYS_FREE (sys_context, clf);
      }
    uptime_str = lw6sys_readable_uptime (sys_context, _TEST_READABLE_UPTIME_1);
    if (LW6SYS_TEST_ACK (uptime_str))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("readable uptime for %d is \"%s\""), _TEST_READABLE_UPTIME_1, uptime_str);
	LW6SYS_FREE (sys_context, uptime_str);
      }
    uptime_str = lw6sys_readable_uptime (sys_context, _TEST_READABLE_UPTIME_2);
    if (LW6SYS_TEST_ACK (uptime_str))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("readable uptime for %d is \"%s\""), _TEST_READABLE_UPTIME_2, uptime_str);
	LW6SYS_FREE (sys_context, uptime_str);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in url.c
 */
static void
_test_url ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *url;

    url = lw6sys_url_http_from_ip_port (sys_context, _TEST_URL_IP_1, _TEST_URL_PORT_1);
    if (LW6SYS_TEST_ACK (url))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("url from \"%s:%d\" is \"%s\""), _TEST_URL_IP_1, _TEST_URL_PORT_1, url);
	LW6SYS_FREE (sys_context, url);
      }
    else
      {
	ret = 0;
      }
    url = lw6sys_url_http_from_ip_port (sys_context, _TEST_URL_IP_2, _TEST_URL_PORT_2);
    if (LW6SYS_TEST_ACK (url))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("url from \"%s:%d\"is \"%s\""), _TEST_URL_IP_2, _TEST_URL_PORT_2, url);
	LW6SYS_FREE (sys_context, url);
      }
    else
      {
	ret = 0;
      }
    if (!LW6SYS_TEST_ACK (!lw6sys_url_is_canonized (sys_context, _TEST_URL_NOT_CANONIZED)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("url \"%s\" reported as canonized, but it isn't"), _TEST_URL_NOT_CANONIZED);
	ret = 0;
      }
    url = lw6sys_url_canonize (sys_context, _TEST_URL_CANONIZE_1);
    if (LW6SYS_TEST_ACK (url))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("canonical url for \"%s\" is \"%s\""), _TEST_URL_CANONIZE_1, url);
	if (!LW6SYS_TEST_ACK (lw6sys_url_is_canonized (sys_context, url)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("url \"%s\" not reported as canonized"), url);
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, url);
      }
    url = lw6sys_url_canonize (sys_context, _TEST_URL_CANONIZE_2);
    if (LW6SYS_TEST_ACK (url))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("canonical url for \"%s\" is \"%s\""), _TEST_URL_CANONIZE_2, url);
	if (!LW6SYS_TEST_ACK (lw6sys_url_is_canonized (sys_context, url)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("url \"%s\" not reported as canonized"), url);
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, url);
      }
    url = lw6sys_url_canonize (sys_context, _TEST_URL_CANONIZE_3);
    if (LW6SYS_TEST_ACK (url))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("canonical url for \"%s\" is \"%s\""), _TEST_URL_CANONIZE_3, url);
	if (!LW6SYS_TEST_ACK (lw6sys_url_is_canonized (sys_context, url)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("url \"%s\" not reported as canonized"), url);
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, url);
      }
    url = lw6sys_url_canonize (sys_context, _TEST_URL_CANONIZE_4);
    if (LW6SYS_TEST_ACK (url))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("canonical url for \"%s\" is \"%s\""), _TEST_URL_CANONIZE_4, url);
	if (!LW6SYS_TEST_ACK (lw6sys_url_is_canonized (sys_context, url)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("url \"%s\" not reported as canonized"), url);
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, url);
      }
    url = lw6sys_url_canonize (sys_context, _TEST_URL_CANONIZE_5);
    if (LW6SYS_TEST_ACK (url))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("canonical url for \"%s\" is \"%s\""), _TEST_URL_CANONIZE_5, url);
	if (!LW6SYS_TEST_ACK (lw6sys_url_is_canonized (sys_context, url)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("url \"%s\" not reported as canonized"), url);
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, url);
      }
    url = lw6sys_url_canonize (sys_context, _TEST_URL_CANONIZE_6);
    if (LW6SYS_TEST_ACK (url))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("canonical url for \"%s\" is \"%s\""), _TEST_URL_CANONIZE_6, url);
	if (!LW6SYS_TEST_ACK (lw6sys_url_is_canonized (sys_context, url)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("url \"%s\" not reported as canonized"), url);
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, url);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in version.c
 */
static void
_test_version ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    const char *version_a[_TEST_VERSION_COMPATIBLE_NB] = { _TEST_VERSION_COMPATIBLE_A1, _TEST_VERSION_COMPATIBLE_A2,
      _TEST_VERSION_COMPATIBLE_A3, _TEST_VERSION_COMPATIBLE_A4
    };
    const char *version_b[_TEST_VERSION_COMPATIBLE_NB] = { _TEST_VERSION_COMPATIBLE_B1, _TEST_VERSION_COMPATIBLE_B2,
      _TEST_VERSION_COMPATIBLE_B3, _TEST_VERSION_COMPATIBLE_B4
    };
    int version_ret[_TEST_VERSION_COMPATIBLE_NB] = { _TEST_VERSION_COMPATIBLE_RET1, _TEST_VERSION_COMPATIBLE_RET2,
      _TEST_VERSION_COMPATIBLE_RET3, _TEST_VERSION_COMPATIBLE_RET4
    };
    int i;

    for (i = 0; i < _TEST_VERSION_COMPATIBLE_NB; ++i)
      {
	if (LW6SYS_TEST_ACK (lw6sys_version_is_compatible (sys_context, version_a[i], version_b[i]) == version_ret[i]))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			_x_ ("OK, comparison of versions \"%s\" and \"%s\" returned %d"), version_a[i], version_b[i], version_ret[i]);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("Unexpected value, omparison of versions \"%s\" and \"%s\" returned %d"), version_a[i], version_b[i], version_ret[i]);
	    ret = 0;
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
vthread_func (lw6sys_context_t * sys_context, void *callback_data)
{
  int i;
  char *text = NULL;

  text = (char *) callback_data;
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("vthread_callback text=\"%s\""), text);

  lw6sys_idle (sys_context);
  if (!lw6sys_vthread_is_running (sys_context))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("vthread marked as not running, when it is"));
    }

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("1st vthread"));
  if (lw6sys_vthread_create (sys_context, &thread_func, &thread_join_nofree, callback_data))
    {
      for (i = 0; i < THREAD_N; ++i)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("1st vthread_main step %d"), i + 1);
	  lw6sys_sleep (sys_context, THREAD_SLEEP_MAIN);
	}

      lw6sys_vthread_join (sys_context);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't create 1st vthread"));
    }

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("2nd vthread"));
  if (lw6sys_vthread_create (sys_context, &thread_func, &thread_join_nofree, callback_data))
    {
      for (i = 0; i < THREAD_N; ++i)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("2nd vthread_main step %d"), i + 1);
	  lw6sys_sleep (sys_context, THREAD_SLEEP_MAIN);
	}

      lw6sys_vthread_join (sys_context);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't create 2nd vthread"));
    }
}

static void
vthread_join (lw6sys_context_t * sys_context, void *callback_data)
{
  char *text = NULL;

  text = (char *) callback_data;
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("vthread_join text=\"%s\""), text);
  LW6SYS_FREE (sys_context, callback_data);
}

static void
vthread_func_no_spawn (lw6sys_context_t * sys_context, void *callback_data)
{
  char *text = NULL;

  text = (char *) callback_data;
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("vthread_callback text=\"%s\", this callback does *NOT* fire a thread"), text);
}

/*
 * Testing functions in vthread.c
 */
static void
_test_vthread ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *text = NULL;
    int i = 0;

    ret = 1;

    if (!LW6SYS_TEST_ACK (!lw6sys_vthread_is_running (sys_context)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("vthread marked as running, when it's not"));
	ret = 0;
      }
    for (i = 0; i < VTHREAD_N; ++i)
      {
	text = lw6sys_str_copy (sys_context, THREAD_TEXT);
	if (LW6SYS_TEST_ACK (text))
	  {
	    if (LW6SYS_TEST_ACK (lw6sys_vthread_run (sys_context, &vthread_func, &vthread_join, (void *) text)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("done with vthread"));
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't run vthread"));
		ret = 0;
	      }
	  }
	else
	  {
	    ret = 0;
	  }

	text = lw6sys_str_copy (sys_context, THREAD_TEXT);
	if (LW6SYS_TEST_ACK (text))
	  {
	    if (LW6SYS_TEST_ACK (lw6sys_vthread_run (sys_context, &vthread_func_no_spawn, &vthread_join, (void *) text)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("done with vthread"));
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("can't run vthread"));
		ret = 0;
	      }
	  }
	else
	  {
	    ret = 0;
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing inline utils functions in sys.h
 */
static void
_test_utils ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int32_t ia = _TEST_UTILS_IA;
    int32_t ib = _TEST_UTILS_IB;
    int32_t imin = 0;
    int32_t imax = 0;
    float fa = _TEST_UTILS_FA;
    float fb = _TEST_UTILS_FB;
    float fmin = 0.0f;
    float fmax = 0.0f;
    int64_t lla = _TEST_UTILS_LLA;
    int64_t llb = _TEST_UTILS_LLB;
    int64_t llmin = 0LL;
    int64_t llmax = 0LL;
    double da = _TEST_UTILS_FA;
    double db = _TEST_UTILS_FB;
    double dmin = 0.0f;
    double dmax = 0.0f;

    imin = lw6sys_imin (ia, ib);
    imax = lw6sys_imax (ia, ib);
    if (LW6SYS_TEST_ACK (imin == ia && imax == ib))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("imin/imax OK, a=%d b=%d min=%d max=%d"), ia, ib, imin, imax);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("imax/imax failed, a=%d b=%d min=%d max=%d"), ia, ib, imin, imax);
	ret = 0;
      }

    fmin = lw6sys_fmin (fa, fb);
    fmax = lw6sys_fmax (fa, fb);
    if (LW6SYS_TEST_ACK (fmin == fa && fmax == fb))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("fmin/fmax OK, a=%f b=%f min=%f max=%f"), fa, fb, fmin, fmax);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("fmax/fmax failed, a=%f b=%f min=%f max=%f"), fa, fb, fmin, fmax);
	ret = 0;
      }

    llmin = lw6sys_llmin (lla, llb);
    llmax = lw6sys_llmax (lla, llb);
    if (LW6SYS_TEST_ACK (llmin == lla && llmax == llb))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_ ("llmin/llmax OK, a=%d b=%d min=%d max=%d"), (int32_t) lla, (int32_t) llb, (int32_t) llmin, (int32_t) llmax);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		    _x_ ("llmax/llmax failed, a=%d b=%d min=%d max=%d"), (int32_t) lla, (int32_t) llb, (int32_t) llmin, (int32_t) llmax);
	ret = 0;
      }

    dmin = lw6sys_dmin (da, db);
    dmax = lw6sys_dmax (da, db);
    if (LW6SYS_TEST_ACK (dmin == da && dmax == db))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("dmin/dmax OK, a=%f b=%f min=%f max=%f"), (float) da, (float) db, (float) dmin, (float) dmax);
      }
    else
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("dmax/dmax failed, a=%f b=%f min=%f max=%f"), (float) da, (float) db, (float) dmin, (float) dmax);
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libsys CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libsys CUnit test suite"));

  return CUE_SUCCESS;
}

/**
 * lw6sys_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libsys module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6sys_test_register (lw6sys_context_t * sys_context, int mode)
{
  int ret = 1;
  CU_Suite *suite = NULL;

  _test_data.sys_context = sys_context;

  suite = CU_add_suite ("lw6sys", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_arg);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_assoc);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_backtrace);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_bazooka);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_build);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_cache);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_callerinfo);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_checksum);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_color);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_convert);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_daemon);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_dump);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_env);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_escape);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_file);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_profiler);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_hash);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_hexa);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_history);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_i18n);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_id);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_keyword);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_list);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_list_r);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_log);
      if (mode & LW6SYS_TEST_MODE_FULL_TEST)
	{
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_log_more);
	}
      if (!(mode & LW6SYS_TEST_MODE_INTERACTIVE))
	{
	  /*
	   * We don't want garbage output when going interactive
	   */
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_log_even_more);
	}
      LW6SYS_CUNIT_ADD_TEST (suite, _test_math);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_mem);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_mutex);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_options);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_nop);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_path);
      if (!(mode & LW6SYS_TEST_MODE_INTERACTIVE))
	{
	  /*
	   * For some reason, forking can wreck the output,
	   * and NCurses gets lost.
	   */
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_process);
	}
      LW6SYS_CUNIT_ADD_TEST (suite, _test_progress);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_random);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_sdl);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_serial);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_shape);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_signal);
      if (mode & LW6SYS_TEST_MODE_FULL_TEST)
	{
	  LW6SYS_CUNIT_ADD_TEST (suite, _test_signal_more);
	}
      LW6SYS_CUNIT_ADD_TEST (suite, _test_sort);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_spinlock);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_str);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_stream);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_thread);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_time);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_url);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_utils);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_version);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_vthread);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6sys_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @sys module test suite, testing most (if not all...)
 * functions. Note that some tests perform file system operations
 * and might therefore fail on a read-only filesystem, or if
 * user permissions are not sufficient.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6sys_test_run (lw6sys_context_t * sys_context, int mode)
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

/**
 * lw6sys_test_exec
 *
 * @sys_context: global system context
 * @argc: number of args as passed to main
 * @argv: array of args as passed to main
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @sys module test which is specific to exec functions,
 * these ones require @argc and @argv to be correctly set so the
 * extra argument justifies putting it outside @lw6sys_test.
 * Additionnally, it's not fool proof... Moreover, it should be
 * run at the beginning of the program, running it afterwards could
 * give unpredictable results. So it's safer to use it outside the
 * CUnit standard mechanisms.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6sys_test_exec (lw6sys_context_t * sys_context, int argc, const char *argv[], int mode)
{
  int ret = 0;

  _test_data.ret = 1;
  _test_data.sys_context = sys_context;

  ret = _test_exec (argc, argv, mode);

  return ret;
}
