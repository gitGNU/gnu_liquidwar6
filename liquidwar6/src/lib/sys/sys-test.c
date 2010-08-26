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

#include "sys.h"

#define TEST_KEYWORD_AS "--this-is_a_TEST-KeYWord"
#define TEST_ARG_KEYWORD "my-option"
#define TEST_ARG_ARGV_STRING "--my-option=my-value"
#define TEST_CHECKSUM_STR_DATA "abc"
#define TEST_CHECKSUM_STR_RESULT 0x2c17398c
#define TEST_CHECKSUM_INT32_DATA 1234
#define TEST_CHECKSUM_INT32_RESULT 0x5d3d512e
#define TEST_CHECKSUM_INT64_DATA 0x1234567890abcdefLL
#define TEST_CHECKSUM_INT64_RESULT 0x5fb58579
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
#else
#define PATH_WITH_SLASH "/foo/bar/"
#define PATH_WITHOUT_SLASH "/foo/bar"
#define PATH_CONCAT "/foo/bar/foo/bar"
#define PATH_CONCAT_PARENT "/foo/bar/foo"
#define PATH_RELATIVE "../../foo/bar"
#define PATH_RELATIVE_UNPARENT "../foo/bar"
#endif
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
#define TEST_PATH_CWD "./"
#define TEST_PATH_SPLIT "/foo/\\bar\\"
#define _TEST_DIR_LIST_FUNC_DATA "foo,bar"
#define _TEST_PATH_LIST_OTHER "."
#define _FIND_IN_DIR_AND_PATH_DIR "/foo"
#define _FIND_IN_DIR_AND_PATH_PATH "/bar:/etc"
#define _FIND_IN_DIR_AND_PATH_FILE "hosts"
#define MALLOC_SIZE 1048576
#define THREAD_N 5
#define THREAD_SLEEP_MAIN 0.66f
#define THREAD_SLEEP_CALLBACK 0.2f
#define THREAD_TEXT "foo bar"
#define THREAD_STRESS_DURATION 3.0f
#define VTHREAD_N 3
#define TEST_SLEEP_TIME 3
#define TEST_DELAY_TIME 50
#define TEST_RFC1123_SECONDS_FROM_NOW 100000
#define TEST_READABLE_UPTIME_1 3689
#define TEST_READABLE_UPTIME_2 1234567
#define TEST_SLEEP_TIME_SHORT_STEP 0.001f
#define BLANK_STR "\t "
#define CLEANUP_FORMAT "<foo %c bar %c>"
#define CLEANUP_CONTROL '\n'
#define CLEANUP_ACCENT 233
#define STR_STARTS_WITH "abcdef"
#define STR_STARTS_WITH_OK "abc"
#define STR_STARTS_WITH_KO "def"
#define ASCII_STR "foo,bar"
#define TEST_ARGC 4
#define TEST_ARGV0 "test.out"
#ifdef LW6_MS_WINDOWS
#define TEST_ARGV1 "--prefix=C:\\toto"
#define TEST_ARGV2 "-map-dir=C:\\home\\user\\my-maps\\"
#else
#define TEST_ARGV1 "--prefix=/toto"
#define TEST_ARGV2 "-map-dir=/home/user/my-maps/"
#endif
#define TEST_ARGV3 "THISisAbug!==--foobar=123---?\\"
#define TEST_FILENAME "test.txt"
#define TEST_CONTENT "This is a test"
#define TEST_DUMP "This...\n...is a dump\n"
#define TEST_HEXA_INT64 1
#define TEST_HEXA_INT32 123456789
#define TEST_HEXA_INT16 12345
#define TEST_HEXA_INT8 123
#define TEST_HEXA_FLOAT 2.0f
#define TEST_HEXA_STR1 "abc"
#define TEST_HEXA_STR2 "defghi"
#define TEST_HEXA_WHD {1,2,3}
#define TEST_HEXA_XYZ {4,5,6}
#define TEST_HEXA_COLOR {25,50,75,100}
#define TEST_KEYWORD LW6DEF_PREFIX
#ifdef LW6_MS_WINDOWS
#define TEST_ENV_CONCAT1 "\\my\\path\\1"
#define TEST_ENV_CONCAT2 "\\my\\path\\2"
#else
#define TEST_ENV_CONCAT1 "/my/path/1"
#define TEST_ENV_CONCAT2 "/my/path/2"
#endif
#define TEST_ENV_SETENV_KEYWORD "LW6_MY_KEY"
#define TEST_ENV_SETENV_VALUE "my-value"
#define TEST_ENV_KEYWORD "my-key"
#ifdef LW6_MS_WINDOWS
#define TEST_ENV_SPLIT "foo1\\foo2;foo3:bar1;bar2\\bar3"
#else
#define TEST_ENV_SPLIT "foo1/foo2;foo3:bar1;bar2/bar3"
#endif
#define _ESCAPE_STR "this is a string with a simple quote ' a double quote \" a tab char \"\t\" and a backslash \"\\\" a lower and greater <> and a few allowed chars _-.,#:;?& OK"
#define _ESCAPE_STR_HTTP_URI "this%20is%20a%20string%20with%20a%20simple%20quote%20%27%20a%20double%20quote%20%22%20a%20tab%20char%20%22%22%20and%20a%20backslash%20%22%5C%22%20a%20lower%20and%20greater%20%3C%3E%20and%20a%20few%20allowed%20chars%20_-.,#:;?&%20OK"
#define _ESCAPE_STR_HTML_ATTRIBUTE "this is a string with a simple quote ' a double quote &quot; a tab char &quot; &quot; and a backslash &quot;\\&quot; a lower and greater &lt;&gt; and a few allowed chars _-.,#:;?&amp; OK"
#define _ESCAPE_STR_SQL_VALUE "this is a string with a simple quote '' a double quote \" a tab char \" \" and a backslash \"\\\" a lower and greater <> and a few allowed chars _-.,#:;?& OK"
#define TEST_REFORMAT_STR1 "this is a short string"
#define TEST_REFORMAT_STR2 "this is a very long string it should be cut in some places but wherever there is a very-very-very-very-very-very-very-very-very-very-very-very-very-very-very-very-very-very long word it won't be cut.\n\nIt should even handle newlines in some places but well, this is not absolutely necessary"
#define TEST_REFORMAT_PREFIX "# "
#define TEST_REFORMAT_COLUMNS 40
#define TEST_SPLIT_STR "/foo/bar,/foo/bar"
#define TEST_SPLIT_CHAR 'o'
#define _TEST_STR_LOWER_UPPER "foObaR!"
#define _TEST_STR_TRUNCATE 5
#define TEST_SERIALIZE 12345
#define TEST_SORT_LENGTH 10
#define TEST_SORT_INT {9,3,4,5,6,8,7,2,0,1};
#define TEST_SORT_INT_MIN 0
#define TEST_SORT_INT_MAX 9
#define TEST_SORT_FLOAT {9.0,3.0,4.0,5.0,6.0,8.0,7.0,2.0,0.0,1.0};
#define TEST_SORT_FLOAT_MIN 0.0
#define TEST_SORT_FLOAT_MAX 9.0
#define TEST_SORT_STR {"9","3","4","5","6","8","7","2","0","1"};
#define TEST_SORT_STR_MIN "0"
#define TEST_SORT_STR_MAX "9"
#define _TEST_SIGNAL_LOOPS 25
#define TEST_CONVERT_INT 421
#define TEST_CONVERT_BOOL 1
#define TEST_CONVERT_FLOAT 3.14f
#define TEST_RANDOM_RANGE 1000
#define TEST_RANDOM_FLOAT_MIN 100.0f
#define TEST_RANDOM_FLOAT_MAX 101.0f
#define TEST_PROGRESS_N 7
#define TEST_PROGRESS_HERE 0.3
#define TEST_HISTORY_MSG1 "1 too old"
#define TEST_HISTORY_MSG2 "2 too old 2"
#define TEST_HISTORY_MSG3 "3 oldest"
#define TEST_HISTORY_MSG4 "4 message this is long and should be truncated message this is long and should be truncated message this is long and should be truncated message this is long and should be truncated message this is long and should be truncated message this is long and should be truncated"
#define TEST_HISTORY_MSG5 "5 most recent"
#define TEST_HISTORY_SLEEP 5.0
#define TEST_HISTORY_TIMEOUT 2000
#define TEST_SHAPE_W 640
#define TEST_SHAPE_H 480
#define TEST_SHAPE_D 3
#define TEST_SHAPE_X 123
#define TEST_SHAPE_Y 456
#define TEST_SHAPE_Z 1
#define TEST_SHAPE_MIN_WH 10
#define TEST_SHAPE_MAX_WH 1000
#define TEST_SHAPE_MAX_D 5
#define TEST_MATH_POLY_WY1Y2S1_NB_STEPS 20
#define TEST_MATH_POLY_WY1Y2S1_W 5.0f
#define TEST_MATH_POLY_WY1Y2S1_Y1 1.0f
#define TEST_MATH_POLY_WY1Y2S1_Y2 10.0f
#define TEST_MATH_POLY_WY1Y2S1_NB_V1S 4
#define TEST_MATH_POLY_WY1Y2S1_V1_MIN -3.0f
#define TEST_MATH_POLY_WY1Y2S1_V1_MAX +3.0f
#define TEST_MATH_ANGLE_XY_MIN -2
#define TEST_MATH_ANGLE_XY_MAX 2
#define TEST_MATH_HEARTBEAT_N 60
#define TEST_MATH_HEARTBEAT_PERIOD 30
#define TEST_MATH_HEARTBEAT_Y1 1.0f
#define TEST_MATH_HEARTBEAT_Y2 1.5f
#define TEST_MATH_BLINK_N 10
#define TEST_MATH_BLINK_PERIOD 3
#define TEST_URL_IP_1 "127.0.0.1"
#define TEST_URL_PORT_1 1234
#define TEST_URL_IP_2 "5.6.7.8"
#define TEST_URL_PORT_2 80
#define TEST_URL_CANONIZE_1 "http://www.toto.com/"
#define TEST_URL_CANONIZE_2 "http://www.ToTo.com:81"
#define TEST_URL_CANONIZE_3 "https://www.ToTo.com:443/tItI"
#define TEST_URL_CANONIZE_4 "www.toto.com"
#define TEST_URL_CANONIZE_5 ":123"
#define TEST_URL_CANONIZE_6 ""
#define TEST_URL_NOT_CANONIZED "ftp://truc"
#define TEST_PASSWORD_SEED "http://"
#define TEST_PASSWORD1 "abc"
#define TEST_PASSWORD2 "XY"

/*
 * Testing functions in arg.c
 */
static int
test_arg ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *value = NULL;
    int argc = TEST_ARGC;
    char *argv[TEST_ARGC] =
      { TEST_ARGV0, TEST_ARGV1, TEST_ARGV2, TEST_ARGV3 };

    lw6sys_log (LW6SYS_LOG_NOTICE, _("trying to match \"%s\""),
		TEST_ARG_ARGV_STRING);
    if (lw6sys_arg_match (TEST_ARG_KEYWORD, TEST_ARG_ARGV_STRING))
      {
	// does match!
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _
		    ("command line arg \"%s\" does not match keyword \"%s\""),
		    TEST_ARG_ARGV_STRING, TEST_ARG_KEYWORD);
	ret = 0;
      }

    if (lw6sys_arg_exists (argc, argv, TEST_KEYWORD))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("keyword \"%s\" is present"),
		    TEST_KEYWORD);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("keyword \"%s\" not present"),
		    TEST_KEYWORD);
	ret = 0;
      }

    value = lw6sys_arg_get_value (argc, argv, TEST_KEYWORD);
    if (value)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("value for \"%s\" is \"%s\""),
		    TEST_KEYWORD, value);
	LW6SYS_FREE (value);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("no value for keyword \"%s\""),
		    TEST_KEYWORD);
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Used to test lw6sys_assoc_map.
 */
static void
assoc_map_func (void *func_data, char *key, void *value)
{
  int *ret = (int *) func_data;
  int *i = (int *) value;

  lw6sys_log (LW6SYS_LOG_NOTICE,
	      _("assoc item ret=%d, key=\"%s\", value=%d"), *ret, key, *i);
}

/*
 * Testing functions in assoc.c
 */
static int
test_assoc ()
{
  int ret = 1;
  lw6sys_assoc_t *assoc;
  lw6sys_assoc_t *assoc_copy;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (LW6SYS_LOG_NOTICE, _("new/delete on assoc"));
    assoc = lw6sys_assoc_new (NULL);
    lw6sys_assoc_free (assoc);
  }

  {
    int a = 3, b = 5, c = 7;

    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("set/unset/has_key/get/map on int assoc"));
    assoc = lw6sys_assoc_new (NULL);
    lw6sys_assoc_set (&assoc, "a", (void *) &a);
    lw6sys_assoc_set (&assoc, "b", (void *) &b);
    lw6sys_assoc_set (&assoc, "c", (void *) &a);
    lw6sys_assoc_set (&assoc, "c", (void *) &c);
    lw6sys_assoc_unset (assoc, "b");
    lw6sys_assoc_map (assoc, &assoc_map_func, &ret);
    lw6sys_assoc_sort_and_map (assoc, &assoc_map_func, &ret);
    ret = ret && lw6sys_assoc_has_key (assoc, "a");
    ret = ret && !lw6sys_assoc_has_key (assoc, "b");
    ret = ret && !lw6sys_assoc_has_key (assoc, "this key does not exist");
    ret = ret && (*((int *) lw6sys_assoc_get (assoc, "a")) == 3);
    ret = ret && ((int *) lw6sys_assoc_get (assoc, "b") == NULL);
    ret = ret && (*((int *) lw6sys_assoc_get (assoc, "c")) == 7);
    lw6sys_assoc_free (assoc);
  }

  {
    char *str1, *str2, *str3;

    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("testing free_func callback and dup on string assoc"));
    assoc = lw6sys_assoc_new (&lw6sys_free_callback);
    str1 = LW6SYS_MALLOC (5);
    strncpy (str1, _TEST_LIST_STR1, 5);
    lw6sys_assoc_set (&assoc, _TEST_ASSOC_KEY1, (void *) str1);
    str2 = LW6SYS_MALLOC (5);
    strncpy (str2, _TEST_LIST_STR2, 5);
    lw6sys_assoc_set (&assoc, _TEST_ASSOC_KEY2, (void *) str2);
    str3 = LW6SYS_MALLOC (5);
    strncpy (str3, _TEST_LIST_STR3, 5);
    lw6sys_assoc_set (&assoc, _TEST_ASSOC_KEY3, (void *) str3);
    lw6sys_assoc_unset (assoc, _TEST_ASSOC_KEY1);
    lw6sys_assoc_unset (assoc, _TEST_ASSOC_KEY3);
    assoc_copy =
      lw6sys_assoc_dup (assoc, (lw6sys_dup_func_t) lw6sys_str_copy);
    ret = ret && !lw6sys_assoc_has_key (assoc_copy, _TEST_ASSOC_KEY1);
    ret = ret && lw6sys_assoc_has_key (assoc_copy, _TEST_ASSOC_KEY2);
    ret = ret && !lw6sys_assoc_has_key (assoc_copy, _TEST_ASSOC_KEY3);
    ret = ret
      && strcmp ((char *) lw6sys_assoc_get (assoc_copy, _TEST_ASSOC_KEY2),
		 _TEST_LIST_STR2) == 0;
    lw6sys_assoc_free (assoc_copy);
    lw6sys_assoc_free (assoc);
  }

  {
    int a = 3, b = 5;
    lw6sys_list_t *keys;

    lw6sys_log (LW6SYS_LOG_NOTICE, _("testing keys on int assoc"));
    assoc = lw6sys_assoc_new (NULL);
    lw6sys_assoc_set (&assoc, "a", (void *) &a);
    lw6sys_assoc_set (&assoc, "b", (void *) &b);
    lw6sys_assoc_set (&assoc, "b", (void *) &b);
    keys = lw6sys_assoc_keys (assoc);
    lw6sys_assoc_free (assoc);
    ret = ret && lw6sys_list_length (keys) == 2;
    lw6sys_list_free (keys);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Test functions in build.c
 */
static int
test_build ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (LW6SYS_LOG_NOTICE, _("build version is \"%s\""),
		lw6sys_build_get_version ());

    lw6sys_build_log_all ();
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in checksum.c
 */
static int
test_checksum ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    u_int32_t checksum = 0;

    checksum = lw6sys_checksum_str (TEST_CHECKSUM_STR_DATA);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("checksum for \"%s\" is %08x (should be %08x)"),
		TEST_CHECKSUM_STR_DATA, checksum, TEST_CHECKSUM_STR_RESULT);
    if (checksum != TEST_CHECKSUM_STR_RESULT)
      {
	ret = 0;
      }

    checksum = lw6sys_checksum_int32 (TEST_CHECKSUM_INT32_DATA);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("checksum for %d is %08x (should be %08x)"),
		TEST_CHECKSUM_INT32_DATA, checksum,
		TEST_CHECKSUM_INT32_RESULT);
    if (checksum != TEST_CHECKSUM_INT32_RESULT)
      {
	ret = 0;
      }

    checksum = lw6sys_checksum_int64 (TEST_CHECKSUM_INT64_DATA);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("checksum for %lld is %08x (should be %08x)"),
		TEST_CHECKSUM_INT64_DATA, checksum,
		TEST_CHECKSUM_INT64_RESULT);
    if (checksum != TEST_CHECKSUM_INT64_RESULT)
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static void
rgb_to_hsv_to_rgb (char *label, lw6sys_color_8_t color_8)
{
  lw6sys_color_hsv_t color_hsv;
  lw6sys_color_8_t color_8_back;

  lw6sys_log (LW6SYS_LOG_NOTICE, _("%s init r=%d,g=%d,b=%d"), label,
	      color_8.r, color_8.g, color_8.b);
  lw6sys_color_rgb_to_hsv (&color_hsv, color_8);
  lw6sys_log (LW6SYS_LOG_NOTICE, _("%s HSV convert h=%.2f,s=%.2f,v=%.2f"),
	      label, color_hsv.h, color_hsv.s, color_hsv.v);
  color_8_back = lw6sys_color_hsv_to_rgb (&color_hsv);
  lw6sys_log (LW6SYS_LOG_NOTICE, _("%s RGB convert r=%d,g=%d,b=%d"), label,
	      color_8_back.r, color_8_back.g, color_8_back.b);
}

/*
 * Testing functions in color.c
 */
static int
test_color ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_color_8_t color_8;
    lw6sys_color_f_t color_f;
    lw6sys_color_8_t color_8_average[3];
    float distance;

    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("interpreting color \"%s\""), COLOR_ASCII);
    color_8 = lw6sys_color_a_to_8 (COLOR_ASCII);
    if (color_8.r != COLOR_8_R || color_8.g != COLOR_8_G
	|| color_8.b != COLOR_8_B || color_8.a != COLOR_8_A)
      {
	ret = 0;
      }

    lw6sys_color_8_to_f (&color_f, color_8);
    color_8 = lw6sys_color_f_to_8 (&color_f);

    rgb_to_hsv_to_rgb ("white", LW6SYS_COLOR_8_WHITE);
    rgb_to_hsv_to_rgb ("black", LW6SYS_COLOR_8_BLACK);
    rgb_to_hsv_to_rgb ("red", LW6SYS_COLOR_8_RED);
    rgb_to_hsv_to_rgb ("green", LW6SYS_COLOR_8_GREEN);
    rgb_to_hsv_to_rgb ("blue", LW6SYS_COLOR_8_BLUE);
    rgb_to_hsv_to_rgb (LW6DEF_TEST, color_8);

    color_8 =
      lw6sys_color_ponderate (LW6SYS_COLOR_8_RED, LW6SYS_COLOR_8_BLUE, 0.5f);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("color between red & blue is r=%d,g=%d,b=%d"), color_8.r,
		color_8.g, color_8.b);

    color_8_average[0] = LW6SYS_COLOR_8_RED;
    color_8_average[1] = LW6SYS_COLOR_8_RED;
    color_8_average[2] = LW6SYS_COLOR_8_GREEN;
    color_8 = lw6sys_color_average (3, color_8_average);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("color average of red+red+green is r=%d,g=%d,b=%d"),
		color_8.r, color_8.g, color_8.b);
    distance =
      lw6sys_color_distance (LW6SYS_COLOR_8_GREEN, LW6SYS_COLOR_8_BLUE);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("distance between green and blue is %.2f"), distance);

    if (!lw6sys_color_is_same (color_8, color_8))
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("color comparison failed"));
      }

    lw6sys_color_8_solid (&color_8);
    lw6sys_color_f_solid (&color_f);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in convert.c
 */
static int
test_convert ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *str;
    int i;
    float f;

    str = lw6sys_itoa (TEST_CONVERT_INT);
    if (str)
      {
	i = lw6sys_atoi (str);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("int=%d -> str=\"%s\" -> int=%d"),
		    TEST_CONVERT_INT, str, i);
	if (i != TEST_CONVERT_INT)
	  {
	    ret = 0;
	  }
	LW6SYS_FREE (str);
      }
    else
      {
	ret = 0;
      }

    str = lw6sys_btoa (TEST_CONVERT_BOOL);
    if (str)
      {
	i = lw6sys_atob (str);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("bool=%d -> str=\"%s\" -> bool=%d"),
		    TEST_CONVERT_BOOL, str, i);
	if (i != TEST_CONVERT_BOOL)
	  {
	    ret = 0;
	  }
	LW6SYS_FREE (str);
      }
    else
      {
	ret = 0;
      }

    str = lw6sys_ftoa (TEST_CONVERT_FLOAT);
    if (str)
      {
	f = lw6sys_atof (str);
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("float=%f -> str=\"%s\" -> float=%f"),
		    TEST_CONVERT_FLOAT, str, f);
	if (f != TEST_CONVERT_FLOAT)
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_
			("%f!=%f: while this is not 100% blocking, floating point conversion on this computer is suspicious"),
			TEST_CONVERT_FLOAT, f);
	  }
	LW6SYS_FREE (str);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Test functions in daemon.c
 */
static int
test_daemon ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int argc = TEST_ARGC;
    char *argv[TEST_ARGC] =
      { TEST_ARGV0, TEST_ARGV1, TEST_ARGV2, TEST_ARGV3 };
    char *pid_file = NULL;

    pid_file = lw6sys_daemon_pid_file (argc, argv);
    if (pid_file)
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("pid file=\"%s\""), pid_file);
	/*
	 * We don't test lw6sys_daemon_start for this would (obviously?)
	 * cause some problems to follow the rest of the test.
	 */
	lw6sys_daemon_stop (pid_file);
	LW6SYS_FREE (pid_file);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in dump.c
 */
static int
test_dump ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *user_dir = NULL;

    user_dir = lw6sys_get_default_user_dir ();
    if (user_dir)
      {
	if (!lw6sys_dir_exists (user_dir))
	  {
	    lw6sys_create_dir (user_dir);
	  }
	lw6sys_dump_clear (user_dir);
	if (!lw6sys_dump (user_dir, TEST_DUMP))
	  {
	    ret = 0;
	  }
	LW6SYS_FREE (user_dir);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in env.c
 */
static int
test_env ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *value;
    char *concat;

    concat = lw6sys_env_concat (TEST_ENV_CONCAT1, TEST_ENV_CONCAT2);
    if (concat)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("ENV concat is \"%s\""), concat);
	LW6SYS_FREE (concat);
      }
    else
      {
	ret = 0;
      }
    if (!lw6sys_setenv (TEST_ENV_SETENV_KEYWORD, TEST_ENV_SETENV_VALUE))
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("unable to set ENV \"%s\""),
		    TEST_ENV_SETENV_KEYWORD);
	ret = 0;
      }
    if (!lw6sys_env_exists_prefixed (TEST_ENV_KEYWORD))
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("ENV \"%s\" doesn't exist"),
		    TEST_ENV_KEYWORD);
	ret = 0;
      }
    value = lw6sys_getenv_prefixed (TEST_ENV_KEYWORD);
    if (value)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("ENV value for \"%s\" is \"%s\""),
		    TEST_ENV_KEYWORD, value);
	LW6SYS_FREE (value);
      }
    else
      {
	ret = 0;
      }
    value = lw6sys_get_username ();
    if (value)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("username=\"%s\""), value);
	LW6SYS_FREE (value);
      }
    else
      {
	ret = 0;
      }
    value = lw6sys_get_hostname ();
    if (value)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("hostname=\"%s\""), value);
	LW6SYS_FREE (value);
      }
    else
      {
	ret = 0;
      }
  }

  {
    lw6sys_list_t *splitted = NULL;
    char *data = NULL;

    splitted = lw6sys_env_split (TEST_ENV_SPLIT);
    if (splitted)
      {
	lw6sys_list_free (splitted);
      }
    else
      {
	ret = 0;
      }

    splitted = lw6sys_env_split (TEST_ENV_SPLIT);
    if (splitted)
      {
	while (splitted
	       && (data = (char *) lw6sys_lifo_pop (&splitted)) != NULL)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_("splitted env \"%s\" contains \"%s\""),
			TEST_ENV_SPLIT, data);
	    LW6SYS_FREE (data);
	  }
	//lw6sys_list_free(splitted);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in env.c
 */
static int
test_escape ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *escaped = NULL;

    escaped = lw6sys_escape_http_uri (_ESCAPE_STR);
    if (escaped)
      {
	if (!strcmp (_ESCAPE_STR_HTTP_URI, escaped))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_("http uri escape for \"%s\" is \"%s\""),
			_ESCAPE_STR, escaped);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_
			("http uri escape for \"%s\" is \"%s\" but should be \"%s\""),
			_ESCAPE_STR, escaped, _ESCAPE_STR_HTTP_URI);
	    ret = 0;
	  }
	LW6SYS_FREE (escaped);
      }
    escaped = lw6sys_escape_html_attribute (_ESCAPE_STR);
    if (escaped)
      {
	if (!strcmp (_ESCAPE_STR_HTML_ATTRIBUTE, escaped))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_("http uri escape for \"%s\" is \"%s\""),
			_ESCAPE_STR, escaped);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_
			("http uri escape for \"%s\" is \"%s\" but should be \"%s\""),
			_ESCAPE_STR, escaped, _ESCAPE_STR_HTML_ATTRIBUTE);
	    ret = 0;
	  }
	LW6SYS_FREE (escaped);
      }
    escaped = lw6sys_escape_sql_value (_ESCAPE_STR);
    if (escaped)
      {
	if (!strcmp (_ESCAPE_STR_SQL_VALUE, escaped))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_("http uri escape for \"%s\" is \"%s\""),
			_ESCAPE_STR, escaped);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_
			("http uri escape for \"%s\" is \"%s\" but should be \"%s\""),
			_ESCAPE_STR, escaped, _ESCAPE_STR_SQL_VALUE);
	    ret = 0;
	  }
	LW6SYS_FREE (escaped);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in exec.c
 */
static int
test_exec (int argc, char *argv[], int mode)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *myself;

    lw6sys_log (LW6SYS_LOG_NOTICE, _("executed_again=%d"),
		lw6sys_is_executed_again (argc, argv));
    myself = lw6sys_exec_find_myself (argc, argv);
    if (myself)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("found myself in \"%s\""), myself);
	if (mode)
	  {
	    if (lw6sys_exec_again (argc, argv))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _
			    ("exec again of \"%s\" successfull, means we're already \"executed again\""),
			    myself);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _("exec again of \"%s\" failed"), myself);
		ret = 0;
	      }
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_("check mode, so not trying to exec again \"%s\""),
			myself);
	  }
	LW6SYS_FREE (myself);
      }
    else
      {
	if (mode)
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _("could not find myself"));
	    ret = 0;
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_
			("could not find myself, this is only check mode so no panic, but this is strange"));
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in file.c
 */
static int
test_file ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *content = NULL;
    char *user_dir = NULL;
    char *filename = NULL;
    int filesize = 0;
    int argc = TEST_ARGC;
    char *argv[TEST_ARGC] =
      { TEST_ARGV0, TEST_ARGV1, TEST_ARGV2, TEST_ARGV3 };

    user_dir = lw6sys_get_user_dir (argc, argv);
    if (user_dir)
      {
	if (!lw6sys_dir_exists (user_dir))
	  {
	    lw6sys_create_dir (user_dir);
	  }
	filename = lw6sys_path_concat (user_dir, TEST_FILENAME);
	if (filename)
	  {
	    if (lw6sys_write_file_content (filename, TEST_CONTENT))
	      {
		content = lw6sys_read_file_content (filename);
		if (content && !strcmp (TEST_CONTENT, content))
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_("file \"%s\" contains \"%s\""),
				filename, content);
		    LW6SYS_FREE (content);
		  }
		else
		  {
		    ret = 0;
		  }

		content = lw6sys_read_file_content_bin (&filesize, filename);
		if (content && !strcmp (TEST_CONTENT, content)
		    && strlen (content) == filesize)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_
				("bin file \"%s\" has size %d and contains \"%s\""),
				filename, filesize, content);
		    LW6SYS_FREE (content);
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
	    LW6SYS_FREE (filename);
	  }
	LW6SYS_FREE (user_dir);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in profiler.c
 */
static int
test_profiler ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (LW6SYS_LOG_NOTICE, _("Google Profiler support=%d"),
		lw6sys_profiler_check (0));
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in hash.c
 */
static int
test_hash ()
{
  int ret = 1;
  lw6sys_hash_t *hash;
  lw6sys_hash_t *hash_copy;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (LW6SYS_LOG_NOTICE, _("new/delete on hash"));
    hash = lw6sys_hash_new (NULL, 7);
    lw6sys_hash_free (hash);
  }

  {
    int a = 3, b = 5, c = 7;

    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("set/unset/has_key/get/map on int hash"));
    hash = lw6sys_hash_new (NULL, 7);
    lw6sys_hash_set (hash, "a", (void *) &a);
    lw6sys_hash_set (hash, "b", (void *) &b);
    lw6sys_hash_set (hash, "c", (void *) &a);
    lw6sys_hash_set (hash, "c", (void *) &c);
    lw6sys_hash_unset (hash, "b");
    lw6sys_hash_map (hash, &assoc_map_func, &ret);
    lw6sys_hash_sort_and_map (hash, &assoc_map_func, &ret);
    ret = ret && lw6sys_hash_has_key (hash, "a");
    ret = ret && !lw6sys_hash_has_key (hash, "b");
    ret = ret && !lw6sys_hash_has_key (hash, "this key does not exist");
    ret = ret && (*((int *) lw6sys_hash_get (hash, "a")) == 3);
    ret = ret && ((int *) lw6sys_hash_get (hash, "b") == NULL);
    ret = ret && (*((int *) lw6sys_hash_get (hash, "c")) == 7);
    lw6sys_hash_free (hash);
  }

  {
    char *str1, *str2, *str3;

    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("testing free_func callback and dup on string hash"));
    hash = lw6sys_hash_new (&lw6sys_free_callback, 7);
    str1 = LW6SYS_MALLOC (5);
    strncpy (str1, _TEST_LIST_STR1, 5);
    lw6sys_hash_set (hash, _TEST_ASSOC_KEY1, (void *) str1);
    str2 = LW6SYS_MALLOC (5);
    strncpy (str2, _TEST_LIST_STR2, 5);
    lw6sys_hash_set (hash, _TEST_ASSOC_KEY2, (void *) str2);
    str3 = LW6SYS_MALLOC (5);
    strncpy (str3, _TEST_LIST_STR3, 5);
    lw6sys_hash_set (hash, _TEST_ASSOC_KEY3, (void *) str3);
    lw6sys_hash_unset (hash, _TEST_ASSOC_KEY1);
    lw6sys_hash_unset (hash, _TEST_ASSOC_KEY3);
    hash_copy = lw6sys_hash_dup (hash, (lw6sys_dup_func_t) lw6sys_str_copy);
    ret = ret && !lw6sys_hash_has_key (hash_copy, _TEST_ASSOC_KEY1);
    ret = ret && lw6sys_hash_has_key (hash_copy, _TEST_ASSOC_KEY2);
    ret = ret && !lw6sys_hash_has_key (hash_copy, _TEST_ASSOC_KEY3);
    ret = ret
      && strcmp ((char *) lw6sys_hash_get (hash_copy, _TEST_ASSOC_KEY2),
		 _TEST_LIST_STR2) == 0;
    lw6sys_hash_free (hash_copy);
    lw6sys_hash_free (hash);
  }

  {
    int a = 3, b = 5;
    lw6sys_list_t *keys;

    lw6sys_log (LW6SYS_LOG_NOTICE, _("testing keys on int hash"));
    hash = lw6sys_hash_new (NULL, 7);
    lw6sys_hash_set (hash, "a", (void *) &a);
    lw6sys_hash_set (hash, "b", (void *) &b);
    lw6sys_hash_set (hash, "b", (void *) &b);
    keys = lw6sys_hash_keys (hash);
    lw6sys_hash_free (hash);
    ret = ret && lw6sys_list_length (keys) == 2;
    lw6sys_list_free (keys);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_hexa ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_hexa_serializer_t *hexa_serializer;
    char *hexa_string = NULL;
    int64_t test_64;
    int32_t test_32;
    int16_t test_16;
    int8_t test_8;
    float test_float;
    char *test_str = NULL;
    lw6sys_whd_t test_whd = TEST_HEXA_WHD;
    lw6sys_xyz_t test_xyz = TEST_HEXA_XYZ;
    lw6sys_color_8_t test_color = TEST_HEXA_COLOR;
    void *ptr = NULL;
    char *ptr_str = NULL;

    hexa_serializer = lw6sys_hexa_serializer_new ("");
    if (hexa_serializer)
      {
	lw6sys_hexa_serializer_rewind (hexa_serializer);
	ret = ret
	  && lw6sys_hexa_serializer_push_int64 (hexa_serializer,
						TEST_HEXA_INT64);
	ret = ret
	  && lw6sys_hexa_serializer_push_int32 (hexa_serializer,
						TEST_HEXA_INT32);
	ret = ret
	  && lw6sys_hexa_serializer_push_int16 (hexa_serializer,
						TEST_HEXA_INT16);
	ret = ret
	  && lw6sys_hexa_serializer_push_int8 (hexa_serializer,
					       TEST_HEXA_INT8);
	ret = ret
	  && lw6sys_hexa_serializer_push_float (hexa_serializer,
						TEST_HEXA_FLOAT);
	ret = ret
	  && lw6sys_hexa_serializer_push_str (hexa_serializer,
					      TEST_HEXA_STR1);
	ret = ret
	  && lw6sys_hexa_serializer_push_str (hexa_serializer,
					      TEST_HEXA_STR2);
	ret = ret
	  && lw6sys_hexa_serializer_push_whd (hexa_serializer, test_whd);
	ret = ret
	  && lw6sys_hexa_serializer_push_xyz (hexa_serializer, test_xyz);
	ret = ret
	  && lw6sys_hexa_serializer_push_color (hexa_serializer, test_color);
	if (ret)
	  {
	    hexa_string = lw6sys_hexa_serializer_as_string (hexa_serializer);
	    if (hexa_string)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _("hexa string is \"%s\""),
			    hexa_string);
		LW6SYS_FREE (hexa_string);
	      }

	    lw6sys_hexa_serializer_rewind (hexa_serializer);

	    if (lw6sys_hexa_serializer_eof (hexa_serializer))
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _("unexpected serializer EOF"));
		ret = 0;
	      }

	    if (lw6sys_hexa_serializer_pop_int64 (hexa_serializer, &test_64))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _("int64 loaded, value=%d"),
			    (int) test_64);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (lw6sys_hexa_serializer_pop_int32 (hexa_serializer, &test_32))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _("int32 loaded, value=%d"),
			    (int) test_32);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (lw6sys_hexa_serializer_pop_int16 (hexa_serializer, &test_16))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _("int16 loaded, value=%d"),
			    (int) test_16);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (lw6sys_hexa_serializer_pop_int8 (hexa_serializer, &test_8))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _("int8 loaded, value=%d"),
			    (int) test_8);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (lw6sys_hexa_serializer_pop_float
		(hexa_serializer, &test_float))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _("float loaded, value=%f"),
			    test_float);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (lw6sys_hexa_serializer_pop_str (hexa_serializer, &test_str))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _("str loaded, value=\"%s\""),
			    test_str);
		LW6SYS_FREE (test_str);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (lw6sys_hexa_serializer_pop_str (hexa_serializer, &test_str))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _("str loaded, value=\"%s\""),
			    test_str);
		LW6SYS_FREE (test_str);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (lw6sys_hexa_serializer_pop_whd (hexa_serializer, &test_whd))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _("whd loaded, value=%dx%dx%d"), (int) test_whd.w,
			    (int) test_whd.h, (int) test_whd.d);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (lw6sys_hexa_serializer_pop_xyz (hexa_serializer, &test_xyz))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _("xyz loaded, value=%d,%d,%d"), (int) test_xyz.x,
			    (int) test_xyz.y, (int) test_xyz.z);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (lw6sys_hexa_serializer_pop_color
		(hexa_serializer, &test_color))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _("color loaded, value=#%02x%02x%02x%02x"),
			    (int) test_color.r, (int) test_color.g,
			    (int) test_color.b, (int) test_color.a);
	      }
	    else
	      {
		ret = 0;
	      }
	    if (lw6sys_hexa_serializer_eof (hexa_serializer))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _("serializer EOF"));
	      }
	    else
	      {
		ret = 0;
	      }
	  }

	ptr_str = lw6sys_hexa_ptr_to_str ((void *) hexa_serializer);
	if (ptr_str)
	  {
	    ptr = lw6sys_hexa_str_to_ptr (ptr_str);
	    if (ptr == (void *) hexa_serializer)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _("pointer %p string is \"%s\""), ptr, ptr_str);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _
			    ("error converting pointer %p from string \"%s\""),
			    ptr, ptr_str);
		ret = 0;
	      }
	    LW6SYS_FREE (ptr_str);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING, _("error converting pointer %p"),
			hexa_serializer);
	    ret = 0;
	  }
	lw6sys_hexa_serializer_free (hexa_serializer);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in history.c
 */
static int
test_history ()
{
  int ret = 0;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char **history;
    char **i;

    lw6sys_history_init ();
    lw6sys_history_register (TEST_HISTORY_MSG1);
    lw6sys_history_register (TEST_HISTORY_MSG2);
    lw6sys_sleep (TEST_HISTORY_SLEEP);
    lw6sys_history_register (TEST_HISTORY_MSG3);
    lw6sys_history_register (TEST_HISTORY_MSG4);
    lw6sys_history_register (TEST_HISTORY_MSG5);
    history = lw6sys_history_get (TEST_HISTORY_TIMEOUT);
    if (history)
      {
	for (i = history; i[0]; ++i)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _("history msg \"%s\""), i[0]);
	    ret = 1;
	  }
	lw6sys_history_free (history);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in i18n.c
 */
static int
test_i18n ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *utf8_str;
    int local_ret = 0;

    utf8_str = lw6sys_locale_to_utf8 (ASCII_STR);
    if (utf8_str)
      {
	if (strcmp (utf8_str, ASCII_STR) == 0)
	  {
	    local_ret = 1;
	  }

	LW6SYS_FREE (utf8_str);
      }

    ret = ret && local_ret;
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in id.c
 */
static int
test_id ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    u_int16_t id_16 = 0;
    u_int32_t id_32 = 0;
    u_int64_t id_64 = 0;
    char *id_str = NULL;
    u_int64_t id_long = 0;

    id_16 = lw6sys_generate_id_16 ();
    id_str = lw6sys_id_ltoa (id_16);
    if (id_str)
      {
	id_long = lw6sys_id_atol (id_str);
	if (id_long == id_16)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_("here's a random 16-bit id \"%s\""), id_str);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_("erreur converting id, src=%" LW6SYS_PRINTF_LL
			  "x, dst=%" LW6SYS_PRINTF_LL "x"), (u_int64_t) id_16,
			id_long);
	    ret = 0;
	  }
	LW6SYS_FREE (id_str);
      }

    id_32 = lw6sys_generate_id_32 ();
    id_str = lw6sys_id_ltoa (id_32);
    if (id_str)
      {
	id_long = lw6sys_id_atol (id_str);
	if (id_long == id_32)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_("here's a random 32-bit id \"%s\""), id_str);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_("erreur converting id, src=%" LW6SYS_PRINTF_LL
			  "x, dst=%" LW6SYS_PRINTF_LL "x"), (u_int64_t) id_32,
			id_long);
	    ret = 0;
	  }
	LW6SYS_FREE (id_str);
      }

    id_64 = lw6sys_generate_id_64 ();
    id_str = lw6sys_id_ltoa (id_64);
    if (id_str)
      {
	id_long = lw6sys_id_atol (id_str);
	if (id_long == id_64)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_("here's a random 64-bit id \"%s\""), id_str);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_("erreur converting id, src=%" LW6SYS_PRINTF_LL
			  "x, dst=%" LW6SYS_PRINTF_LL "x"), (u_int64_t) id_64,
			id_long);
	    ret = 0;
	  }
	LW6SYS_FREE (id_str);
      }

  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in keyword.c
 */
static int
test_keyword ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *str;

    str = lw6sys_keyword_as_key (TEST_KEYWORD_AS);
    if (str)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("keyword as key is \"%s\""), str);
	LW6SYS_FREE (str);
      }
    str = lw6sys_keyword_as_arg (TEST_KEYWORD_AS);
    if (str)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("keyword as arg is \"%s\""), str);
	LW6SYS_FREE (str);
      }
    str = lw6sys_keyword_as_env (TEST_KEYWORD_AS);
    if (str)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("keyword as env is \"%s\""), str);
	LW6SYS_FREE (str);
      }
    str = lw6sys_keyword_as_xml (TEST_KEYWORD_AS);
    if (str)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("keyword as xml is \"%s\""), str);
	LW6SYS_FREE (str);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Used to test lw6sys_list_map.
 */
static void
list_map_int_func (void *func_data, void *data)
{
  int *ret = (int *) func_data;
  int *i = (int *) data;

  lw6sys_log (LW6SYS_LOG_NOTICE, _("list item ret=%d, i=%d"), *ret, *i);
}

/*
 * Used to test lw6sys_list_map.
 */
static void
list_map_str_func (void *func_data, void *data)
{
  int *ret = (int *) func_data;
  char *str = (char *) data;

  lw6sys_log (LW6SYS_LOG_NOTICE, _("list item ret=%d, str=\"%s\""), *ret,
	      str);
}

/*
 * Used to test lw6sys_list_map.
 */
static int
list_filter_func (void *func_data, void *data)
{
  int *limit = (int *) func_data;
  int *d = (int *) data;
  int ret = 0;

  ret = (*d < *limit);
  lw6sys_log (LW6SYS_LOG_NOTICE,
	      _("list item limit=%d, d=%d, pass filter=%d"), *limit, *d, ret);

  return ret;
}

/*
 * Testing functions in list.c
 */
static int
test_list ()
{
  int ret = 1;
  lw6sys_list_t *list;
  lw6sys_list_t *list_copy;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (LW6SYS_LOG_NOTICE, _("new/delete/is_empty on list"));
    list = lw6sys_list_new (NULL);
    ret = ret && lw6sys_list_is_empty (list);
    lw6sys_list_free (list);
  }

  {
    int a = 3, b = 5, c = 7;
    int i;

    lw6sys_log (LW6SYS_LOG_NOTICE, _("lifo push/pop on int list"));
    list = lw6sys_list_new (NULL);
    lw6sys_lifo_push (&list, (void *) &a);
    lw6sys_log (LW6SYS_LOG_NOTICE, _("lifo push %d"), a);
    lw6sys_lifo_push (&list, (void *) &b);
    lw6sys_log (LW6SYS_LOG_NOTICE, _("lifo push %d"), b);
    lw6sys_lifo_push (&list, (void *) &c);
    lw6sys_log (LW6SYS_LOG_NOTICE, _("lifo push %d"), c);
    lw6sys_list_map (list, &list_map_int_func, &ret);
    ret = ret && lw6sys_list_length (list) == 3;
    i = *((int *) lw6sys_lifo_pop (&list));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("lifo pop %d"), i);
    ret = ret && (i == c);
    i = *((int *) lw6sys_lifo_pop (&list));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("lifo pop %d"), i);
    ret = ret && (i == b);
    i = *((int *) lw6sys_lifo_pop (&list));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("lifo pop %d"), i);
    ret = ret && (i == a);
    lw6sys_log (LW6SYS_LOG_NOTICE, _("free list"));
    lw6sys_list_free (list);
  }

  {
    int a = 2, b = 4, c = 6, d = 8;
    int i;

    lw6sys_log (LW6SYS_LOG_NOTICE, _("fifo push/pop on int list"));
    list = lw6sys_list_new (NULL);
    lw6sys_fifo_push (&list, (void *) &a);
    lw6sys_log (LW6SYS_LOG_NOTICE, _("fifo push %d"), a);
    lw6sys_fifo_push (&list, (void *) &b);
    lw6sys_log (LW6SYS_LOG_NOTICE, _("fifo push %d"), b);
    lw6sys_fifo_push (&list, (void *) &c);
    lw6sys_log (LW6SYS_LOG_NOTICE, _("fifo push %d"), c);
    lw6sys_fifo_push (&list, (void *) &d);
    lw6sys_log (LW6SYS_LOG_NOTICE, _("fifo push %d"), d);
    lw6sys_list_map (list, &list_map_int_func, &ret);
    ret = ret && lw6sys_list_length (list) == 4;
    i = *((int *) lw6sys_fifo_pop (&list));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("fifo pop %d"), i);
    ret = ret && (i == a);
    i = *((int *) lw6sys_fifo_pop (&list));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("fifo pop %d"), i);
    ret = ret && (i == b);
    i = *((int *) lw6sys_fifo_pop (&list));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("fifo pop %d"), i);
    ret = ret && (i == c);
    i = *((int *) lw6sys_fifo_pop (&list));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("fifo pop %d"), i);
    ret = ret && (i == d);
    lw6sys_log (LW6SYS_LOG_NOTICE, _("free list"));
    lw6sys_list_free (list);
  }

  {
    char *str;

    lw6sys_log (LW6SYS_LOG_NOTICE, _("push_front/pop_front on string list"));
    list = lw6sys_list_new (lw6sys_free_callback);
    lw6sys_list_push_front (&list,
			    (void *) lw6sys_str_copy (_TEST_LIST_STR1));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("push_front \"%s\""), _TEST_LIST_STR1);
    lw6sys_list_push_front (&list,
			    (void *) lw6sys_str_copy (_TEST_LIST_STR2));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("push_front \"%s\""), _TEST_LIST_STR2);
    lw6sys_list_push_front (&list,
			    (void *) lw6sys_str_copy (_TEST_LIST_STR3));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("push_front \"%s\""), _TEST_LIST_STR3);
    lw6sys_list_map (list, &list_map_str_func, &ret);
    str = (char *) lw6sys_list_pop_front (&list);
    if (str)
      {
	ret = ret && (!strcmp (str, _TEST_LIST_STR3));
	lw6sys_log (LW6SYS_LOG_NOTICE, _("pop_front \"%s\""), str);
	LW6SYS_FREE (str);
      }
    else
      {
	ret = 0;
      }
    if (str)
      {
	str = (char *) lw6sys_list_pop_front (&list);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("pop_front \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR2));
	LW6SYS_FREE (str);
      }
    else
      {
	ret = 0;
      }
    if (str)
      {
	str = (char *) lw6sys_list_pop_front (&list);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("pop_front \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR1));
	LW6SYS_FREE (str);
      }
    else
      {
	ret = 0;
      }
    lw6sys_log (LW6SYS_LOG_NOTICE, _("free list"));
    lw6sys_list_free (list);
  }

  {
    char *str;

    lw6sys_log (LW6SYS_LOG_NOTICE, _("push_back/pop_front on string list"));
    list = lw6sys_list_new (lw6sys_free_callback);
    lw6sys_list_push_back (&list, (void *) lw6sys_str_copy (_TEST_LIST_STR1));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("push_back \"%s\""), _TEST_LIST_STR1);
    lw6sys_list_push_back (&list, (void *) lw6sys_str_copy (_TEST_LIST_STR2));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("push_back \"%s\""), _TEST_LIST_STR2);
    lw6sys_list_push_back (&list, (void *) lw6sys_str_copy (_TEST_LIST_STR3));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("push_back \"%s\""), _TEST_LIST_STR3);
    lw6sys_list_map (list, &list_map_str_func, &ret);
    str = (char *) lw6sys_list_pop_front (&list);
    if (str)
      {
	ret = ret && (!strcmp (str, _TEST_LIST_STR1));
	lw6sys_log (LW6SYS_LOG_NOTICE, _("pop_front \"%s\""), str);
	LW6SYS_FREE (str);
      }
    else
      {
	ret = 0;
      }
    if (str)
      {
	str = (char *) lw6sys_list_pop_front (&list);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("pop_front \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR2));
	LW6SYS_FREE (str);
      }
    else
      {
	ret = 0;
      }
    if (str)
      {
	str = (char *) lw6sys_list_pop_front (&list);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("pop_front \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR3));
	LW6SYS_FREE (str);
      }
    else
      {
	ret = 0;
      }
    lw6sys_log (LW6SYS_LOG_NOTICE, _("free list"));
    lw6sys_list_free (list);
  }

  {
    char *str;

    lw6sys_log (LW6SYS_LOG_NOTICE, _("push_front/pop_back on string list"));
    list = lw6sys_list_new (lw6sys_free_callback);
    lw6sys_list_push_front (&list,
			    (void *) lw6sys_str_copy (_TEST_LIST_STR1));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("push_front \"%s\""), _TEST_LIST_STR1);
    lw6sys_list_push_front (&list,
			    (void *) lw6sys_str_copy (_TEST_LIST_STR2));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("push_front \"%s\""), _TEST_LIST_STR2);
    lw6sys_list_push_front (&list,
			    (void *) lw6sys_str_copy (_TEST_LIST_STR3));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("push_front \"%s\""), _TEST_LIST_STR3);
    lw6sys_list_map (list, &list_map_str_func, &ret);
    str = (char *) lw6sys_list_pop_back (&list);
    if (str)
      {
	ret = ret && (!strcmp (str, _TEST_LIST_STR1));
	lw6sys_log (LW6SYS_LOG_NOTICE, _("pop_back \"%s\""), str);
	LW6SYS_FREE (str);
      }
    else
      {
	ret = 0;
      }
    if (str)
      {
	str = (char *) lw6sys_list_pop_back (&list);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("pop_back \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR2));
	LW6SYS_FREE (str);
      }
    else
      {
	ret = 0;
      }
    if (str)
      {
	str = (char *) lw6sys_list_pop_back (&list);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("pop_back \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR3));
	LW6SYS_FREE (str);
      }
    else
      {
	ret = 0;
      }
    lw6sys_log (LW6SYS_LOG_NOTICE, _("free list"));
    lw6sys_list_free (list);
  }

  {
    char *str;

    lw6sys_log (LW6SYS_LOG_NOTICE, _("push_back/pop_back on string list"));
    list = lw6sys_list_new (lw6sys_free_callback);
    lw6sys_list_push_back (&list, (void *) lw6sys_str_copy (_TEST_LIST_STR1));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("push_back \"%s\""), _TEST_LIST_STR1);
    lw6sys_list_push_back (&list, (void *) lw6sys_str_copy (_TEST_LIST_STR2));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("push_back \"%s\""), _TEST_LIST_STR2);
    lw6sys_list_push_back (&list, (void *) lw6sys_str_copy (_TEST_LIST_STR3));
    lw6sys_log (LW6SYS_LOG_NOTICE, _("push_back \"%s\""), _TEST_LIST_STR3);
    lw6sys_list_map (list, &list_map_str_func, &ret);
    str = (char *) lw6sys_list_pop_back (&list);
    if (str)
      {
	ret = ret && (!strcmp (str, _TEST_LIST_STR3));
	lw6sys_log (LW6SYS_LOG_NOTICE, _("pop_back \"%s\""), str);
	LW6SYS_FREE (str);
      }
    else
      {
	ret = 0;
      }
    if (str)
      {
	str = (char *) lw6sys_list_pop_back (&list);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("pop_back \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR2));
	LW6SYS_FREE (str);
      }
    else
      {
	ret = 0;
      }
    if (str)
      {
	str = (char *) lw6sys_list_pop_back (&list);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("pop_back \"%s\""), str);
	ret = ret && (!strcmp (str, _TEST_LIST_STR1));
	LW6SYS_FREE (str);
      }
    else
      {
	ret = 0;
      }
    lw6sys_log (LW6SYS_LOG_NOTICE, _("free list"));
    lw6sys_list_free (list);
  }

  {
    int a = 32, b = 64, c = 128, d = 256, e = 512;

    lw6sys_log (LW6SYS_LOG_NOTICE, _("front/back push/pop on int list"));
    list = lw6sys_list_new (NULL);
    lw6sys_list_push_front (&list, (void *) &a);
    lw6sys_list_push_front (&list, (void *) &b);
    lw6sys_list_push_back (&list, (void *) &c);
    lw6sys_list_push_back (&list, (void *) &d);
    lw6sys_list_push_back (&list, (void *) &e);
    lw6sys_list_map (list, &list_map_int_func, &ret);
    ret = ret && lw6sys_list_length (list) == 5;
    ret = ret && (*((int *) lw6sys_list_pop_back (&list)) == e);
    ret = ret && (*((int *) lw6sys_list_pop_back (&list)) == d);
    ret = ret && (*((int *) lw6sys_list_pop_front (&list)) == b);
    ret = ret && (*((int *) lw6sys_list_pop_front (&list)) == a);
    ret = ret && (*((int *) lw6sys_list_pop_front (&list)) == c);
    lw6sys_list_free (list);
  }

  {
    char *str1, *str2, *str3, *str4;

    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("testing free_func callback and dup on string list"));
    list = lw6sys_list_new (&lw6sys_free_callback);
    str1 = lw6sys_str_copy (_TEST_LIST_STR1);
    lw6sys_lifo_push (&list, (void *) str1);
    str2 = lw6sys_str_copy (_TEST_LIST_STR2);
    lw6sys_fifo_push (&list, (void *) str2);
    str3 = lw6sys_str_copy (_TEST_LIST_STR3);
    lw6sys_lifo_push (&list, (void *) str3);
    str4 = lw6sys_str_copy (_TEST_LIST_STR4);
    lw6sys_fifo_push (&list, (void *) str4);
    list_copy = lw6sys_list_dup (list, (lw6sys_dup_func_t) lw6sys_str_copy);
    ret = ret && lw6sys_list_length (list_copy) == 4;
    lw6sys_list_free (list_copy);
    lw6sys_list_free (list);
  }

  {
    lw6sys_log (LW6SYS_LOG_NOTICE, _("testing final pop behavior"));

    list = lw6sys_list_new (NULL);
    if (list)
      {
	lw6sys_list_pop_front (&list);
	if (list)
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_("final pop front didn't free list"));
	    lw6sys_list_free (list);
	    ret = 0;
	  }
      }

    list = lw6sys_list_new (NULL);
    if (list)
      {
	lw6sys_list_pop_back (&list);
	if (list)
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_("final pop back didn't free list"));
	    lw6sys_list_free (list);
	    ret = 0;
	  }
      }
  }
  {
    int i = 0;
    int *d = NULL;
    int limit = 0;

    lw6sys_log (LW6SYS_LOG_NOTICE, _("testing list filter"));
    list = lw6sys_list_new (lw6sys_free_callback);
    if (list)
      {
	for (i = 0; i < _TEST_LIST_FILTER_NB_ITEMS; ++i)
	  {
	    d = (int *) LW6SYS_MALLOC (sizeof (int));
	    if (d)
	      {
		(*d) = lw6sys_random (_TEST_LIST_FILTER_RANGE);
		lw6sys_list_push_front (&list, d);
	      }
	  }
	limit = (2 * _TEST_LIST_FILTER_RANGE) / 3;
	lw6sys_log (LW6SYS_LOG_NOTICE, _("1st filter limit=%d"), limit);
	lw6sys_list_filter (&list, list_filter_func, &limit);
	limit = _TEST_LIST_FILTER_RANGE / 3;
	lw6sys_log (LW6SYS_LOG_NOTICE, _("2nd filter limit=%d"), limit);
	lw6sys_list_filter (&list, list_filter_func, &limit);
	limit = -_TEST_LIST_FILTER_RANGE / 10;
	lw6sys_log (LW6SYS_LOG_NOTICE, _("3rd filter limit=%d"), limit);
	lw6sys_list_filter (&list, list_filter_func, &limit);
	lw6sys_list_free (list);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in log.c
 */
static int
test_log (int mode)
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int log_level;

    log_level = lw6sys_log_get_level ();
    lw6sys_log_set_level (log_level);
    if (mode)
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("this is a /TEST/ warning"));
	lw6sys_log (LW6SYS_LOG_ERROR, _("this is a /TEST/ error"));
      }
    else
      {
	/*
	 * We do not test WARNING & ERROR to avoid confusion...
	 */
      }
    lw6sys_log (LW6SYS_LOG_NOTICE, _("testing log_notice %s=%d"),
		"log_level=%d", log_level);
    lw6sys_log (LW6SYS_LOG_INFO, _("testing log_info %s=%d"), "log_level=%d",
		log_level);
    // normally, this one does now show with defaults settings...
    lw6sys_log (LW6SYS_LOG_DEBUG, _("testing log_debug %s=%d"),
		"log_level=%d", log_level);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in math.c
 */
static int
test_math ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    float y = 0.0f;
    float s = 0.0f;
    float x = 0.0f;
    float v1 = 0.0f;
    int ix, iy;
    float angle = 0.0f;
    int t, b;

    for (v1 = TEST_MATH_POLY_WY1Y2S1_V1_MIN;
	 v1 <= TEST_MATH_POLY_WY1Y2S1_V1_MAX;
	 v1 +=
	 (TEST_MATH_POLY_WY1Y2S1_V1_MAX -
	  TEST_MATH_POLY_WY1Y2S1_V1_MIN) / TEST_MATH_POLY_WY1Y2S1_NB_V1S)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("wy1y2s1 v1=%f"), v1);
	for (x = 0.0f; x <= TEST_MATH_POLY_WY1Y2S1_W;
	     x += TEST_MATH_POLY_WY1Y2S1_W / TEST_MATH_POLY_WY1Y2S1_NB_STEPS)
	  {
	    lw6sys_math_poly_wy1y2s1 (&y, &s, x, TEST_MATH_POLY_WY1Y2S1_W,
				      TEST_MATH_POLY_WY1Y2S1_Y1,
				      TEST_MATH_POLY_WY1Y2S1_Y2, v1);
	    lw6sys_log (LW6SYS_LOG_NOTICE, _("wy1y2s1 x=%f y=%f s=%f"), x, y,
			s);
	  }
      }
    for (iy = TEST_MATH_ANGLE_XY_MIN; iy <= TEST_MATH_ANGLE_XY_MAX; ++iy)
      {
	for (ix = TEST_MATH_ANGLE_XY_MIN; ix <= TEST_MATH_ANGLE_XY_MAX; ++ix)
	  {
	    angle = lw6sys_math_angle_360 (ix, iy);
	    lw6sys_log (LW6SYS_LOG_NOTICE, _("x=%d y=%d angle=%f"), ix, iy,
			angle);
	  }
      }

    lw6sys_log (LW6SYS_LOG_NOTICE, _("heartbeat period=%d y1=%f y2=%f"),
		TEST_MATH_HEARTBEAT_PERIOD, TEST_MATH_HEARTBEAT_Y1,
		TEST_MATH_HEARTBEAT_Y2);
    for (t = 0; t < TEST_MATH_HEARTBEAT_N; t++)
      {
	y =
	  lw6sys_math_heartbeat (t, TEST_MATH_HEARTBEAT_PERIOD,
				 TEST_MATH_HEARTBEAT_Y1,
				 TEST_MATH_HEARTBEAT_Y2);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("heartbeat t=%d y=%f"), t, y);
      }

    lw6sys_log (LW6SYS_LOG_NOTICE, _("blink period=%d"),
		TEST_MATH_BLINK_PERIOD);
    for (t = 0; t < TEST_MATH_BLINK_N; t++)
      {
	b = lw6sys_math_blink (t, TEST_MATH_BLINK_PERIOD);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("blink t=%d b=%d"), t, b);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in mem.c
 */
static int
test_mem ()
{
  int ret = 1;
  char *ptr = NULL;
  LW6SYS_TEST_FUNCTION_BEGIN;

  lw6sys_log (LW6SYS_LOG_NOTICE,
	      _("testing MALLOC/FREE (%d bytes)"), MALLOC_SIZE);
  ptr = (char *) LW6SYS_MALLOC (MALLOC_SIZE * sizeof (char));
  ret = ret && (ptr != NULL);
  LW6SYS_FREE (ptr);
  // same tests without using macros, fixes link when LW6_OPTIMZE is defined
  ptr = (char *) lw6sys_malloc (MALLOC_SIZE, __FILE__, __LINE__);
  if (ptr)
    {
      lw6sys_free (ptr, __FILE__, __LINE__);
    }
  ptr = (char *) lw6sys_calloc (MALLOC_SIZE, __FILE__, __LINE__);
  if (ptr)
    {
      ptr = lw6sys_realloc (ptr, MALLOC_SIZE + 1, __FILE__, __LINE__);
      if (ptr)
	{
	  lw6sys_free (ptr, __FILE__, __LINE__);
	}
    }
  lw6sys_log (LW6SYS_LOG_NOTICE, _("%d megs"), lw6sys_megabytes_available ());
  if (lw6sys_is_big_endian ())
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _("big endian"));
    }
  if (lw6sys_is_little_endian ())
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _("little endian"));
    }

  ret = ret && lw6sys_check_types_size ();

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in mutex.c
 */
static int
test_mutex ()
{
  int ret = 0;
  void *mutex = NULL;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (LW6SYS_LOG_NOTICE, _("testing mutex functions"));
    mutex = lw6sys_mutex_create ();
    if (mutex)
      {
	if (lw6sys_mutex_lock (mutex))
	  {
	    if (lw6sys_mutex_trylock (mutex))
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _
			    ("strange, trylock on locked mutex should return 0"));
	      }
	    else
	      {
		if (lw6sys_mutex_unlock (mutex))
		  {
		    ret = 1;
		  }
	      }
	  }
	lw6sys_mutex_destroy (mutex);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing options functions
 */
static int
test_options ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *path;

    path = lw6sys_get_default_user_dir ();
    if (path)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("default user dir is \"%s\""), path);
	LW6SYS_FREE (path);
      }
    else
      {
	ret = 0;
      }

    path = lw6sys_get_default_prefix ();
    if (path)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("default prefix is \"%s\""), path);
	LW6SYS_FREE (path);
      }
    else
      {
	ret = 0;
      }

    lw6sys_options_log_defaults ();
  }

  {
    char *path;
    int argc = TEST_ARGC;
    char *argv[TEST_ARGC] =
      { TEST_ARGV0, TEST_ARGV1, TEST_ARGV2, TEST_ARGV3 };

    path = lw6sys_get_user_dir (argc, argv);
    if (path)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("user dir is \"%s\""), path);
	LW6SYS_FREE (path);
      }
    else
      {
	ret = 0;
      }

    path = lw6sys_get_prefix (argc, argv);
    if (path)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("prefix is \"%s\""), path);
	LW6SYS_FREE (path);
      }
    else
      {
	ret = 0;
      }

    lw6sys_options_log (argc, argv);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in nop.c
 */
static int
test_nop ()
{
  int ret = 0;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    ret = lw6sys_true () && !lw6sys_false ();
  }

  LW6SYS_TEST_FUNCTION_END return ret;
}

/*
 * Used to test lw6sys_path_split
 */
static void
path_split_func (void *func_data, void *data)
{
  char *str = (char *) data;

  lw6sys_log (LW6SYS_LOG_NOTICE, _("path part \"%s\""), str);
}

static int
_dir_list_filter (void *func_data, char *file)
{
  int ret = 0;

  if (lw6sys_checksum_str (file) % 2)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _("keep \"%s\" (func_data=\"%s\")"),
		  file, func_data);
      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _("discard \"%s\""), file);
    }

  return ret;
}

static void
_dir_list_print (void *func_data, void *data)
{
  char *file = (char *) data;

  lw6sys_log (LW6SYS_LOG_NOTICE, _("entry \"%s\""), file);
}

/*
 * Testing functions in password.c
 */
static int
test_password ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *checksum = NULL;

    checksum = lw6sys_password_checksum (NULL, NULL);
    if (checksum)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("checksum for password NULL is \"%s\""), checksum);
	LW6SYS_FREE (checksum);
      }
    else
      {
	ret = 0;
      }
    checksum = lw6sys_password_checksum (NULL, "");
    if (checksum)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("checksum for empty password is \"%s\""), checksum);
	LW6SYS_FREE (checksum);
      }
    else
      {
	ret = 0;
      }
    checksum = lw6sys_password_checksum (TEST_PASSWORD_SEED, "");
    if (checksum)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _
		    ("checksum for empty password with seed \"%s\" is \"%s\""),
		    TEST_PASSWORD_SEED, checksum);
	LW6SYS_FREE (checksum);
      }
    else
      {
	ret = 0;
      }
    checksum = lw6sys_password_checksum (TEST_PASSWORD_SEED, TEST_PASSWORD1);
    if (checksum)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("checksum for password \"%s\" is \"%s\""),
		    TEST_PASSWORD1, checksum);
	if (lw6sys_password_verify
	    (TEST_PASSWORD_SEED, TEST_PASSWORD1, TEST_PASSWORD1))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _("same password test works"));
	  }
	else
	  {
	    ret = 0;
	  }
	if (lw6sys_password_verify
	    (TEST_PASSWORD_SEED, TEST_PASSWORD1, checksum))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_("same password test works using checksum"));
	  }
	else
	  {
	    ret = 0;
	  }
	if (lw6sys_password_verify (TEST_PASSWORD_SEED, NULL, TEST_PASSWORD2))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_("same password test works when it's NULL here"));
	  }
	else
	  {
	    ret = 0;
	  }
	if (!lw6sys_password_verify
	    (TEST_PASSWORD_SEED, TEST_PASSWORD1, TEST_PASSWORD2))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_("same password test detects wrong passwords"));
	  }
	else
	  {
	    ret = 0;
	  }
	if (!lw6sys_password_verify (NULL, TEST_PASSWORD1, TEST_PASSWORD2))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_("same password test detects wrong seed"));
	  }
	else
	  {
	    ret = 0;
	  }
	LW6SYS_FREE (checksum);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in path.c
 */
static int
test_path ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *path = NULL;
    char *path2 = NULL;
    int n = 0;
    int relative = 0;
    int cwd = 0;
    lw6sys_list_t *list = NULL;

    lw6sys_log (LW6SYS_LOG_NOTICE, _("add/strip slash"));
    path = lw6sys_path_add_slash (PATH_WITHOUT_SLASH);
    if (path)
      {
	ret = ret && (strcmp (path, PATH_WITH_SLASH) == 0);
	LW6SYS_FREE (path);
      }
    path = lw6sys_path_strip_slash (PATH_WITH_SLASH);
    if (path)
      {
	ret = ret && (strcmp (path, PATH_WITHOUT_SLASH) == 0);
	LW6SYS_FREE (path);
      }
    lw6sys_log (LW6SYS_LOG_NOTICE, _("concat path"));
    path = lw6sys_path_concat (PATH_WITH_SLASH, PATH_WITH_SLASH);
    if (path)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("concatenated path = \"%s\""), path);
	ret = ret && (strcmp (path, PATH_CONCAT) == 0);
	LW6SYS_FREE (path);
      }
    relative = lw6sys_path_is_relative (PATH_RELATIVE);
    if (relative)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("path \"%s\" is relative"), PATH_RELATIVE);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _("path \"%s\" not considered relative"), PATH_RELATIVE);
	ret = 0;
      }
    cwd = lw6sys_path_is_cwd (TEST_PATH_CWD);
    if (cwd)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("path \"%s\" is cwd"), TEST_PATH_CWD);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _("path \"%s\" not considered being cwd"), TEST_PATH_CWD);
	ret = 0;
      }
    path = lw6sys_path_parent (PATH_CONCAT);
    if (path)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("parent path = \"%s\""), path);
	ret = ret && (strcmp (path, PATH_CONCAT_PARENT) == 0);
	LW6SYS_FREE (path);
      }
    path = lw6sys_path_unparent (PATH_RELATIVE);
    if (path)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("unparent path = \"%s\""), path);
	ret = ret && (strcmp (path, PATH_RELATIVE_UNPARENT) == 0);
	LW6SYS_FREE (path);
      }
    path = lw6sys_path_unparent_no_malloc (PATH_RELATIVE);
    lw6sys_log (LW6SYS_LOG_NOTICE, _("unparent path = \"%s\""), path);
    ret = ret && (strcmp (path, PATH_RELATIVE_UNPARENT) == 0);

    list = lw6sys_path_split (TEST_PATH_SPLIT);
    lw6sys_log (LW6SYS_LOG_NOTICE, _("splitting \"%s\""), TEST_PATH_SPLIT);
    if (list)
      {
	lw6sys_list_map (list, path_split_func, NULL);
	lw6sys_list_free (list);
      }

    path = lw6sys_get_home ();
    if (path)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("listing dir \"%s\""), path);
	list =
	  lw6sys_dir_list (path, _dir_list_filter, _TEST_DIR_LIST_FUNC_DATA,
			   &n);
	if (list)
	  {
	    lw6sys_list_map (list, _dir_list_print, NULL);
	    lw6sys_log (LW6SYS_LOG_NOTICE, _("%d items found in \"%s\""), n,
			path);
	    lw6sys_list_free (list);
	  }
	path2 = lw6sys_env_concat (path, _TEST_PATH_LIST_OTHER);
	if (path2)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _("listing path \"%s\""), path2);
	    list =
	      lw6sys_path_list (path2, _dir_list_filter,
				_TEST_DIR_LIST_FUNC_DATA, &n);
	    if (list)
	      {
		lw6sys_list_map (list, _dir_list_print, NULL);
		lw6sys_log (LW6SYS_LOG_NOTICE, _("%d items found in \"%s\""),
			    n, path2);
		lw6sys_list_free (list);
	      }
	    LW6SYS_FREE (path2);
	  }
	LW6SYS_FREE (path);
      }
    path =
      lw6sys_find_in_dir_and_path (_FIND_IN_DIR_AND_PATH_DIR,
				   _FIND_IN_DIR_AND_PATH_PATH,
				   _FIND_IN_DIR_AND_PATH_FILE);
    if (path)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("found \"%s\" with dir=\"%s\" path=\"%s\" file=\"%s\""),
		    path, _FIND_IN_DIR_AND_PATH_DIR,
		    _FIND_IN_DIR_AND_PATH_PATH, _FIND_IN_DIR_AND_PATH_FILE);
	LW6SYS_FREE (path);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in progress.c
 */
static int
test_progress ()
{
  int ret = 1;
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

    lw6sys_progress_default (&progress, &value);
    for (i = 0; i < TEST_PROGRESS_N; ++i)
      {
	lw6sys_progress_update (&progress, 0, TEST_PROGRESS_N, i);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("progress=%0.2f"),
		    *(progress.value));
      }
    lw6sys_progress_begin (&progress);
    lw6sys_progress_half (&progress);
    lw6sys_progress_end (&progress);
    lw6sys_progress_split (&progress1, &progress2, &progress);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("progress1 %0.2f/%0.2f progress2 %0.2f/%0.2f"),
		progress1.min, progress1.max, progress2.min, progress2.max);
    lw6sys_progress_split_here (&progress1, &progress2, &progress,
				TEST_PROGRESS_HERE);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("progress1 %0.2f/%0.2f progress2 %0.2f/%0.2f"),
		progress1.min, progress1.max, progress2.min, progress2.max);
    lw6sys_progress_split3 (&progress1, &progress2, &progress3, &progress);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_
		("progress1 %0.2f/%0.2f progress2 %0.2f/%0.2f progress3 %0.2f/%0.2f"),
		progress1.min, progress1.max, progress2.min, progress2.max,
		progress3.min, progress3.max);
    lw6sys_progress_split4 (&progress1, &progress2, &progress3, &progress4,
			    &progress);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_
		("progress1 %0.2f/%0.2f progress2 %0.2f/%0.2f progress3 %0.2f/%0.2f progress4 %0.2f/%0.2f"),
		progress1.min, progress1.max, progress2.min, progress2.max,
		progress3.min, progress3.max, progress4.min, progress4.max);
    lw6sys_progress_split5 (&progress1, &progress2, &progress3, &progress4,
			    &progress5, &progress);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_
		("progress1 %0.2f/%0.2f progress2 %0.2f/%0.2f progress3 %0.2f/%0.2f progress4 %0.2f/%0.2f progress5 %0.2f/%0.2f"),
		progress1.min, progress1.max, progress2.min, progress2.max,
		progress3.min, progress3.max, progress4.min, progress4.max,
		progress5.min, progress5.max);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in random.c
 */
static int
test_random ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i = 0;
    float f = 0.0f;

    i = lw6sys_random (TEST_RANDOM_RANGE);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("here's a random number between 0 and %d: %d"),
		TEST_RANDOM_RANGE, i);
    f = lw6sys_random_float (TEST_RANDOM_FLOAT_MIN, TEST_RANDOM_FLOAT_MAX);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("here's a random float number between %f and %f: %f"),
		TEST_RANDOM_FLOAT_MIN, TEST_RANDOM_FLOAT_MAX, f);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in signal.c
 */
static int
test_signal ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i;

    if (lw6sys_signal_poll_quit ())
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("quit!=0, strange..."));
      }
    lw6sys_signal_custom ();
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_
		("our own SIGTERM and SIGHUP signals are in place, try typing CTRL+C for instance"));
    for (i = 0; i < _TEST_SIGNAL_LOOPS; ++i)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("quit=%d"),
		    lw6sys_signal_poll_quit ());
	lw6sys_snooze ();
      }
    lw6sys_signal_send_quit ();
    if (!lw6sys_signal_poll_quit ())
      {
	ret = 0;
      }
    lw6sys_signal_term_handler (0);
    lw6sys_signal_int_handler (0);
    lw6sys_signal_hup_handler (0);
    lw6sys_signal_default ();
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in sort.c
 */
static int
test_sort ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_list_t *list = NULL;
    int array[TEST_SORT_LENGTH] = TEST_SORT_INT;
    int i;

    list = lw6sys_list_new (NULL);
    if (list)
      {
	for (i = 0; i < TEST_SORT_LENGTH; ++i)
	  {
	    lw6sys_lifo_push (&list, (void *) (array + i));
	  }
	lw6sys_sort (&list, lw6sys_sort_int_callback);
	if (list)
	  {
	    if ((*((int *) (list->data))) == TEST_SORT_INT_MIN)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _("min sorted value is %d"),
			    TEST_SORT_INT_MIN);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _("min sorted value is %d and should be %d"),
			    *((int *) (list->data)), TEST_SORT_INT_MIN);
		ret = 0;
	      }
	  }
	lw6sys_sort (&list, lw6sys_sort_int_desc_callback);
	if (list)
	  {
	    if ((*((int *) (list->data))) == TEST_SORT_INT_MAX)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _("max sorted value is %d"),
			    TEST_SORT_INT_MAX);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _("max sorted value is %d and should be %d"),
			    *((int *) (list->data)), TEST_SORT_INT_MAX);
		ret = 0;
	      }
	  }
	if (list)
	  {
	    lw6sys_list_free (list);
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
    float array[TEST_SORT_LENGTH] = TEST_SORT_FLOAT;
    int i;

    list = lw6sys_list_new (NULL);
    if (list)
      {
	for (i = 0; i < TEST_SORT_LENGTH; ++i)
	  {
	    lw6sys_lifo_push (&list, (void *) (array + i));
	  }
	lw6sys_sort (&list, lw6sys_sort_float_callback);
	if (list)
	  {
	    if ((*((float *) (list->data))) == TEST_SORT_FLOAT_MIN)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _("min sorted value is %f"),
			    TEST_SORT_FLOAT_MIN);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _("min sorted value is %f and should be %f"),
			    *((float *) (list->data)), TEST_SORT_FLOAT_MIN);
		ret = 0;
	      }
	  }
	lw6sys_sort (&list, lw6sys_sort_float_desc_callback);
	if (list)
	  {
	    if ((*((float *) (list->data))) == TEST_SORT_FLOAT_MAX)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _("max sorted value is %f"),
			    TEST_SORT_FLOAT_MAX);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _("max sorted value is %f and should be %f"),
			    *((float *) (list->data)), TEST_SORT_FLOAT_MAX);
		ret = 0;
	      }
	  }
	if (list)
	  {
	    lw6sys_list_free (list);
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
    char *array[TEST_SORT_LENGTH] = TEST_SORT_STR;
    int i;

    list = lw6sys_list_new (NULL);
    if (list)
      {
	for (i = 0; i < TEST_SORT_LENGTH; ++i)
	  {
	    lw6sys_lifo_push (&list, (void *) array[i]);
	  }
	lw6sys_sort (&list, lw6sys_sort_str_callback);
	if (list)
	  {
	    if (!strcmp ((char *) (list->data), TEST_SORT_STR_MIN))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _("min sorted value is \"%s\""),
			    TEST_SORT_STR_MIN);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _
			    ("min sorted value is \"%s\" and should be \"%s\""),
			    (char *) (list->data), TEST_SORT_STR_MIN);
		ret = 0;
	      }
	  }
	lw6sys_sort (&list, lw6sys_sort_str_desc_callback);
	if (list)
	  {
	    if (!strcmp ((char *) (list->data), TEST_SORT_STR_MAX))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _("max sorted value is \"%s\""),
			    TEST_SORT_STR_MAX);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _
			    ("max sorted value is \"%s\" and should be \"%s\""),
			    (char *) (list->data), TEST_SORT_STR_MAX);
		ret = 0;
	      }
	  }
	if (list)
	  {
	    lw6sys_list_free (list);
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
  return ret;
}

/*
 * Testing functions in spinlock.c
 */
static int
test_spinlock ()
{
  int ret = 0;
  void *spinlock = NULL;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (LW6SYS_LOG_NOTICE, _("testing spinlock functions"));
    spinlock = lw6sys_spinlock_create ();
    if (spinlock)
      {
	if (lw6sys_spinlock_lock (spinlock))
	  {
	    if (lw6sys_spinlock_trylock (spinlock))
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _
			    ("strange, trylock on locked spinlock should return 0"));
	      }
	    else
	      {
		if (lw6sys_spinlock_unlock (spinlock))
		  {
		    ret = 1;
		  }
	      }
	  }
	lw6sys_spinlock_destroy (spinlock);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Used to test lw6sys_split...
 */
static void
str_split_func (void *func_data, void *data)
{
  char *str = (char *) data;

  lw6sys_log (LW6SYS_LOG_NOTICE, _("split part \"%s\""), str);
}

/*
 * Testing functions in str.c
 */
static int
test_str ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *str;
    lw6sys_list_t *list;

    lw6sys_log (LW6SYS_LOG_NOTICE, _("testing is_blank on \"%s\""),
		BLANK_STR);
    ret = ret && lw6sys_str_is_blank (BLANK_STR);
    lw6sys_log (LW6SYS_LOG_NOTICE, _("testing cleanup"));
    str =
      lw6sys_new_sprintf (CLEANUP_FORMAT, CLEANUP_CONTROL, CLEANUP_ACCENT);
    if (str)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("before cleanup, string is \%s\""),
		    str);
	lw6sys_str_cleanup (str);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("after cleanup, string is \"%s\""),
		    str);
	lw6sys_str_cleanup_ascii7 (str);
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("after cleanup (ascii-7), string is \"%s\""), str);
	LW6SYS_FREE (str);
      }
    lw6sys_log (LW6SYS_LOG_NOTICE, _("testing is_same"));
    ret = ret && lw6sys_str_is_same (BLANK_STR, BLANK_STR)
      && lw6sys_str_is_same (NULL, NULL)
      && (!lw6sys_str_is_same (NULL, BLANK_STR))
      && (!lw6sys_str_is_same (BLANK_STR, NULL))
      && (!lw6sys_str_is_same (BLANK_STR, TEST_REFORMAT_STR1))
      && (!lw6sys_str_is_same_no_case (BLANK_STR, TEST_REFORMAT_STR1));
    ret = ret && lw6sys_str_starts_with (STR_STARTS_WITH, STR_STARTS_WITH_OK)
      && !lw6sys_str_starts_with (STR_STARTS_WITH, STR_STARTS_WITH_KO)
      && lw6sys_str_starts_with_no_case (STR_STARTS_WITH, STR_STARTS_WITH_OK)
      && !lw6sys_str_starts_with_no_case (STR_STARTS_WITH,
					  STR_STARTS_WITH_KO);
    str =
      lw6sys_str_reformat (TEST_REFORMAT_STR1, TEST_REFORMAT_PREFIX,
			   TEST_REFORMAT_COLUMNS);
    if (str)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("reformatted string is \"%s\""),
		    str);
	LW6SYS_FREE (str);
      }
    else
      {
	ret = 0;
      }

    str =
      lw6sys_str_reformat (TEST_REFORMAT_STR2, TEST_REFORMAT_PREFIX,
			   TEST_REFORMAT_COLUMNS);
    if (str)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("reformatted string is \"%s\""),
		    str);
	LW6SYS_FREE (str);
      }
    else
      {
	ret = 0;
      }

    list = lw6sys_str_split (TEST_SPLIT_STR, TEST_SPLIT_CHAR);
    if (list)
      {
	lw6sys_list_map (list, str_split_func, NULL);
	lw6sys_list_free (list);
      }

    list = lw6sys_str_split_no_0 (TEST_SPLIT_STR, TEST_SPLIT_CHAR);
    if (list)
      {
	lw6sys_list_map (list, str_split_func, NULL);
	lw6sys_list_free (list);
      }

    list = lw6sys_str_split_config_item (TEST_SPLIT_STR);
    if (list)
      {
	lw6sys_list_map (list, str_split_func, NULL);
	lw6sys_list_free (list);
      }

    str = lw6sys_str_copy (_TEST_STR_LOWER_UPPER);
    if (str)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("original string=\"%s\""), str);
	lw6sys_str_tolower (str);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("lowered string=\"%s\""), str);
	lw6sys_str_toupper (str);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("uppered string=\"%s\""), str);
	lw6sys_str_truncate (str, _TEST_STR_TRUNCATE);
	lw6sys_log (LW6SYS_LOG_NOTICE, _("truncated string=\"%s\""), str);
	LW6SYS_FREE (str);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in sdl.c
 */
static int
test_sdl ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_log (LW6SYS_LOG_NOTICE, _("register/unregister SDL"));
    ret = ret && lw6sys_sdl_register () && lw6sys_sdl_unregister ();
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in serial.c
 */
static int
test_serial ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int64_t test64 = 0;
    int32_t test32 = 0;
    int16_t test16 = 0;
    unsigned char buffer[LW6SYS_SIZEOF_INT64];

    lw6sys_serialize_int64 (buffer, TEST_SERIALIZE);
    test64 = lw6sys_unserialize_int64 (buffer);
    if (test64 != TEST_SERIALIZE)
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _("serialize failed on 64-bit integer (%d/%d)"),
		    (int) test64, TEST_SERIALIZE);
	ret = 0;
      }

    lw6sys_serialize_int32 (buffer, TEST_SERIALIZE);
    test32 = lw6sys_unserialize_int32 (buffer);
    if (test32 != TEST_SERIALIZE)
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _("serialize failed on 32-bit integer (%d/%d)"),
		    (int) test32, TEST_SERIALIZE);
	ret = 0;
      }

    lw6sys_serialize_int16 (buffer, TEST_SERIALIZE);
    test16 = lw6sys_unserialize_int16 (buffer);
    if (test16 != TEST_SERIALIZE)
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _("serialize failed on 16-bit integer (%d/%d)"),
		    (int) test16, TEST_SERIALIZE);
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in shape.c
 */
static int
test_shape ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_whd_t shape = { TEST_SHAPE_W, TEST_SHAPE_H, TEST_SHAPE_D };
    lw6sys_whd_t shape2 =
      { TEST_SHAPE_W + 1, TEST_SHAPE_H + 1, TEST_SHAPE_D + 1 };
    lw6sys_xyz_t pos = { TEST_SHAPE_X, TEST_SHAPE_Y, TEST_SHAPE_Z };

    if (lw6sys_shape_check_min_max_whd
	(&shape, TEST_SHAPE_MIN_WH, TEST_SHAPE_MAX_WH, TEST_SHAPE_MAX_D))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("shape %dx%dx%d respects [%d-%d-%d] constraints"),
		    (int) shape.w, (int) shape.h, (int) shape.d,
		    TEST_SHAPE_MIN_WH, TEST_SHAPE_MAX_WH, TEST_SHAPE_MAX_D);
      }
    else
      {
	ret = 0;
      }
    if (lw6sys_shape_check_pos (&shape, &pos))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("pos %d,%d,%d is inside shape %dx%dx%d"), (int) pos.x,
		    (int) pos.y, (int) pos.z, (int) shape.w, (int) shape.h,
		    (int) shape.d);
      }
    else
      {
	ret = 0;
      }
    if (lw6sys_shape_is_same (&shape, &shape)
	&& !lw6sys_shape_is_same (&shape, &shape2))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("shape %dx%dx%d reported different as %dx%dx%d"),
		    (int) shape.w, (int) shape.h, (int) shape.d,
		    (int) shape2.w, (int) shape2.h, (int) shape2.d);
      }
    else
      {
	ret = 0;
      }
    if (lw6sys_shape_is_same_xy (&shape, &shape)
	&& !lw6sys_shape_is_same_xy (&shape, &shape2))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("shape %dx%d reported different as %dx%d"),
		    (int) shape.w, (int) shape.h,
		    (int) shape2.w, (int) shape2.h);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static void
thread_func (void *callback_data)
{
  int i = 0;
  char *text = NULL;

  text = (char *) callback_data;
  lw6sys_log (LW6SYS_LOG_NOTICE, _("thread_callback text=\"%s\""), text);
  for (i = 0; i < THREAD_N; ++i)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _("thread_callback step %d"), i + 1);
      lw6sys_sleep (THREAD_SLEEP_CALLBACK);
    }
}

static void
thread_join_nofree (void *callback_data)
{
  char *text = NULL;

  text = (char *) callback_data;
  lw6sys_log (LW6SYS_LOG_NOTICE, _("thread_join text=\"%s\""), text);
}

static void
thread_join (void *callback_data)
{
  thread_join_nofree (callback_data);
  LW6SYS_FREE (callback_data);
}

typedef struct thread_stress_data_s
{
  int run;
  int count;
  int ret;
} thread_stress_data_t;

static void
thread_stress_func (void *callback_data)
{
  thread_stress_data_t *thread_stress_data = NULL;
  void *child;

  thread_stress_data = (thread_stress_data_t *) callback_data;

  while (thread_stress_data->run)
    {
      child = lw6sys_thread_create (NULL, NULL, NULL);
      if (child)
	{
	  lw6sys_thread_join (child);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _("unable to create child"));
	  thread_stress_data->ret = 0;
	}
      thread_stress_data->count++;
    }
}

/*
 * Testing functions in thread.c
 */
static int
test_thread ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    void *thread_handler = NULL;
    char *text = lw6sys_str_copy (THREAD_TEXT);
    int i = 0;
    thread_stress_data_t thread_stress_data;

    ret = 1;
    if (text)
      {
	thread_handler =
	  lw6sys_thread_create (&thread_func, &thread_join, (void *) text);
	if (thread_handler)
	  {
	    for (i = 0; i < THREAD_N; ++i)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _("thread_main step %d, callback_done=%d"), i + 1,
			    lw6sys_thread_is_callback_done (thread_handler));
		lw6sys_sleep (THREAD_SLEEP_MAIN);
	      }

	    lw6sys_thread_join (thread_handler);
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
    if (ret)
      {
	thread_stress_data.run = 1;
	thread_stress_data.count = 0;
	thread_stress_data.ret = 1;
	thread_handler =
	  lw6sys_thread_create (&thread_stress_func, NULL,
				&thread_stress_data);
	if (thread_handler)
	  {
	    lw6sys_sleep (THREAD_STRESS_DURATION);
	    thread_stress_data.run = 0;
	    lw6sys_thread_join (thread_handler);
	    ret = ret && thread_stress_data.ret;
	    if (ret)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _("%d threads (%0.1f/sec)"),
			    thread_stress_data.count,
			    (float) (((float) thread_stress_data.count) /
				     THREAD_STRESS_DURATION));
	      }
	  }
	else
	  {
	    ret = 0;
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in time.c
 */
static int
test_time ()
{
  int ret = 1;
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

    lw6sys_log (LW6SYS_LOG_NOTICE, _("timestamp %" LW6SYS_PRINTF_LL "d"),
		lw6sys_get_timestamp ());
    lw6sys_log (LW6SYS_LOG_NOTICE, _("uptime %" LW6SYS_PRINTF_LL "d"),
		lw6sys_get_uptime ());
    lw6sys_log (LW6SYS_LOG_NOTICE, _("cycle %d"), lw6sys_get_cycle ());
    lw6sys_idle ();
    lw6sys_timer_update (&timestamp, &uptime, &cycle);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("after idle timestamp=%" LW6SYS_PRINTF_LL "d uptime=%"
		  LW6SYS_PRINTF_LL "d cycle=%d"), timestamp, uptime, cycle);
    lw6sys_snooze ();
    lw6sys_timer_update (&timestamp, &uptime, &cycle);
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_("after snooze timestamp=%" LW6SYS_PRINTF_LL "d uptime=%"
		  LW6SYS_PRINTF_LL "d cycle=%d"), timestamp, uptime, cycle);
    last_uptime = lw6sys_get_uptime ();
    lw6sys_log (LW6SYS_LOG_NOTICE, _("sleep %d seconds"), TEST_SLEEP_TIME);
    lw6sys_sleep (TEST_SLEEP_TIME);
    lw6sys_log (LW6SYS_LOG_NOTICE, _("delay %d msec"), TEST_DELAY_TIME);
    lw6sys_delay (TEST_DELAY_TIME);
    lw6sys_log (LW6SYS_LOG_NOTICE, _("lasted %" LW6SYS_PRINTF_LL "d ms"),
		lw6sys_get_uptime () - last_uptime);
    last_uptime = lw6sys_get_uptime ();
    nb_steps = TEST_SLEEP_TIME / TEST_SLEEP_TIME_SHORT_STEP;
    lw6sys_log (LW6SYS_LOG_NOTICE, _("sleep %d seconds using %d %0.6f steps"),
		TEST_SLEEP_TIME, nb_steps, TEST_SLEEP_TIME_SHORT_STEP);
    for (i = 0; i < nb_steps; ++i)
      {
	lw6sys_sleep (TEST_SLEEP_TIME_SHORT_STEP);
      }
    lw6sys_log (LW6SYS_LOG_NOTICE, _("lasted %" LW6SYS_PRINTF_LL "d ms"),
		lw6sys_get_uptime () - last_uptime);

    rfc1123 = lw6sys_date_rfc1123 (0);
    if (rfc1123)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("rfc1123 date for now is \"%s\""),
		    rfc1123);
	LW6SYS_FREE (rfc1123);
      }
    rfc1123 = lw6sys_date_rfc1123 (TEST_RFC1123_SECONDS_FROM_NOW);
    if (rfc1123)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("rfc1123 date in %d seconds is \"%s\""),
		    TEST_RFC1123_SECONDS_FROM_NOW, rfc1123);
	LW6SYS_FREE (rfc1123);
      }
    clf = lw6sys_date_clf ();
    if (clf)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("clf date is \"%s\""), clf);
	LW6SYS_FREE (clf);
      }
    uptime_str = lw6sys_readable_uptime (TEST_READABLE_UPTIME_1);
    if (uptime_str)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("readable uptime for %d is \"%s\""),
		    TEST_READABLE_UPTIME_1, uptime_str);
	LW6SYS_FREE (uptime_str);
      }
    uptime_str = lw6sys_readable_uptime (TEST_READABLE_UPTIME_2);
    if (uptime_str)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("readable uptime for %d is \"%s\""),
		    TEST_READABLE_UPTIME_2, uptime_str);
	LW6SYS_FREE (uptime_str);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in url.c
 */
static int
test_url ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *url;

    url = lw6sys_url_http_from_ip_port (TEST_URL_IP_1, TEST_URL_PORT_1);
    if (url)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("url from \"%s:%d\" is \"%s\""),
		    TEST_URL_IP_1, TEST_URL_PORT_1, url);
	LW6SYS_FREE (url);
      }
    else
      {
	ret = 0;
      }
    url = lw6sys_url_http_from_ip_port (TEST_URL_IP_2, TEST_URL_PORT_2);
    if (url)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("url from \"%s:%d\"is \"%s\""),
		    TEST_URL_IP_2, TEST_URL_PORT_2, url);
	LW6SYS_FREE (url);
      }
    else
      {
	ret = 0;
      }
    if (lw6sys_url_is_canonized (TEST_URL_NOT_CANONIZED))
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _("url \"%s\" reported as canonized, but it isn't"),
		    TEST_URL_NOT_CANONIZED);
	ret = 0;
      }
    url = lw6sys_url_canonize (TEST_URL_CANONIZE_1);
    if (url)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("canonical url for \"%s\" is \"%s\""),
		    TEST_URL_CANONIZE_1, url);
	if (!lw6sys_url_is_canonized (url))
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_("url \"%s\" not reported as canonized"), url);
	    ret = 0;
	  }
	LW6SYS_FREE (url);
      }
    url = lw6sys_url_canonize (TEST_URL_CANONIZE_2);
    if (url)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("canonical url for \"%s\" is \"%s\""),
		    TEST_URL_CANONIZE_2, url);
	if (!lw6sys_url_is_canonized (url))
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_("url \"%s\" not reported as canonized"), url);
	    ret = 0;
	  }
	LW6SYS_FREE (url);
      }
    url = lw6sys_url_canonize (TEST_URL_CANONIZE_3);
    if (url)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("canonical url for \"%s\" is \"%s\""),
		    TEST_URL_CANONIZE_3, url);
	if (!lw6sys_url_is_canonized (url))
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_("url \"%s\" not reported as canonized"), url);
	    ret = 0;
	  }
	LW6SYS_FREE (url);
      }
    url = lw6sys_url_canonize (TEST_URL_CANONIZE_4);
    if (url)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("canonical url for \"%s\" is \"%s\""),
		    TEST_URL_CANONIZE_4, url);
	if (!lw6sys_url_is_canonized (url))
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_("url \"%s\" not reported as canonized"), url);
	    ret = 0;
	  }
	LW6SYS_FREE (url);
      }
    url = lw6sys_url_canonize (TEST_URL_CANONIZE_5);
    if (url)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("canonical url for \"%s\" is \"%s\""),
		    TEST_URL_CANONIZE_5, url);
	if (!lw6sys_url_is_canonized (url))
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_("url \"%s\" not reported as canonized"), url);
	    ret = 0;
	  }
	LW6SYS_FREE (url);
      }
    url = lw6sys_url_canonize (TEST_URL_CANONIZE_6);
    if (url)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("canonical url for \"%s\" is \"%s\""),
		    TEST_URL_CANONIZE_6, url);
	if (!lw6sys_url_is_canonized (url))
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_("url \"%s\" not reported as canonized"), url);
	    ret = 0;
	  }
	LW6SYS_FREE (url);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static void
vthread_func (void *callback_data)
{
  int i;
  char *text = NULL;

  text = (char *) callback_data;
  lw6sys_log (LW6SYS_LOG_NOTICE, _("vthread_callback text=\"%s\""), text);

  lw6sys_idle ();
  if (!lw6sys_vthread_is_running ())
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("vthread marked as not running, when it is"));
    }

  if (lw6sys_vthread_create
      (&thread_func, &thread_join_nofree, callback_data))
    {
      for (i = 0; i < THREAD_N; ++i)
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE, _("vthread_main step %d"), i + 1);
	  lw6sys_sleep (THREAD_SLEEP_MAIN);
	}

      lw6sys_vthread_join ();
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _("can't create thread"));
    }
}

static void
vthread_join (void *callback_data)
{
  char *text = NULL;

  text = (char *) callback_data;
  lw6sys_log (LW6SYS_LOG_NOTICE, _("vthread_join text=\"%s\""), text);
  LW6SYS_FREE (callback_data);
}

/*
 * Testing functions in vthread.c
 */
static int
test_vthread ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *text = NULL;
    int i = 0;

    ret = 1;

    if (lw6sys_vthread_is_running ())
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _("vthread marked as running, when it's not"));
	ret = 0;
      }
    for (i = 0; i < VTHREAD_N; ++i)
      {
	text = lw6sys_str_copy (THREAD_TEXT);
	if (text)
	  {
	    if (lw6sys_vthread_run
		(&vthread_func, &vthread_join, (void *) text))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _("done with vthread"));
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _("can't run vthread"));
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
  return ret;
}

/**
 * lw6sys_test_exec
 *
 * @argc: number of args as passed to main
 * @argv: array of args as passed to main
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @sys module test suite which is specific to exec functions,
 * these ones require @argc and @argv to be correctly set so the
 * extra argument justifies putting it outside @lw6sys_test.
 * Additionnally, it's not fool proof...
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6sys_test_exec (int argc, char *argv[], int mode)
{
  int ret = 0;

  ret = test_exec (argc, argv, mode);

  return ret;
}

/**
 * lw6sys_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @sys module test suite, testing most (if not all...)
 * functions. Note that some tests perform file system operations
 * and might therefore fail on a read-only filesystem, or if
 * user permissions are not sufficient.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6sys_test (int mode)
{
  int ret = 0;

  ret = test_arg () && test_assoc () && test_build () && test_checksum ()
    && test_color () && test_convert () && test_daemon () && test_dump ()
    && test_env () && test_escape () && test_file () && test_profiler ()
    && test_hash () && test_hexa () && test_history () && test_i18n ()
    && test_id () && test_keyword () && test_list () && test_log (mode)
    && test_math () && test_mem () && test_mutex () && test_options ()
    && test_nop () && test_password () && test_path () && test_progress ()
    && test_random () && test_sdl () && test_serial () && test_shape ()
    && test_signal () && test_sort () && test_spinlock () && test_str ()
    && test_thread () && test_time () && test_url () && test_vthread ();

  return ret;
}
