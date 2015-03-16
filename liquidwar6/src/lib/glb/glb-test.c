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

#include "glb.h"

#define _TEST_BASE64_STR_1 "this is a string!"
#define _TEST_BASE64_STR_2 ""
#define _TEST_BASE64_STR_3 "this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string"
#define _TEST_BASE64_STR_4_LEN 100
#define _TEST_BASE64_PREFIX "TOTO=0"
#define _TEST_SHA1_LEN 1000
#define _TEST_SHA1_KEY "toto"
#define _TEST_SHA1_CHECK 0xd4a35ebb

typedef struct _lw6glb_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
} _lw6glb_test_data_t;

static _lw6glb_test_data_t _test_data = { 0, NULL };

static int
_test_base64_ok (lw6sys_context_t * sys_context, char *_test_str, int log_all)
{
  int ret = 1;
  char *base64_str = NULL;
  char *str = NULL;

  base64_str = lw6glb_base64_encode_str (sys_context, _test_str);
  if (base64_str)
    {
      if (log_all)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("base64 encoded version of \"%s\" is \"%s\""), _test_str, base64_str);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("base64 encoding, result \"%s\""), base64_str);
	}
      str = lw6glb_base64_decode_str (sys_context, base64_str);
      if (str)
	{
	  if (lw6sys_str_is_same (sys_context, str, _test_str))
	    {
	      if (log_all)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("base64 string \"%s\" correctly decoded to \"%s\""), base64_str, str);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("base64 string \"%s\" correctly decoded"), base64_str);
		}
	    }
	  else
	    {
	      if (log_all)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			      _x_ ("base64 string \"%s\" decoded to \"%s\" but should be \"%s\""), base64_str, str, _test_str);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("base64 string decoded but result is different"));
		}
	      ret = 0;
	    }
	  LW6SYS_FREE (sys_context, str);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to decode \"%s\" from base64"), base64_str);
	  ret = 0;
	}
      LW6SYS_FREE (sys_context, base64_str);
    }
  else
    {
      if (log_all)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to encode \"%s\" in base64"), _test_str);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to encode in base64"));
	}
      ret = 0;
    }

  base64_str = lw6glb_base64_encode_str_prefix (sys_context, _test_str, _TEST_BASE64_PREFIX);
  if (base64_str)
    {
      if (log_all)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		      _x_ ("base64 encoded version of \"%s\" is \"%s\" (prefix=\"%s\")"), _test_str, base64_str, _TEST_BASE64_PREFIX);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("base64 encoding result is \"%s\" (prefix=\"%s\")"), base64_str, _TEST_BASE64_PREFIX);
	}
      str = lw6glb_base64_decode_str_prefix (sys_context, base64_str, _TEST_BASE64_PREFIX);
      if (str)
	{
	  if (lw6sys_str_is_same (sys_context, str, _test_str))
	    {
	      if (log_all)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			      _x_ ("base64 string \"%s\" correctly decoded to \"%s\" (prefix=\"%s\")"), base64_str, str, _TEST_BASE64_PREFIX);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("base64 string \"%s\" correctly decoded (prefix=\"%s\")"), base64_str, _TEST_BASE64_PREFIX);
		}
	    }
	  else
	    {
	      if (log_all)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			      _x_
			      ("base64 string \"%s\" decoded to \"%s\" but should be \"%s\" (prefix=\"%s\")"),
			      base64_str, str, _test_str, _TEST_BASE64_PREFIX);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("base64 string decoded but result is different (prefix=\"%s\")"), _TEST_BASE64_PREFIX);
		}
	      ret = 0;
	    }
	  LW6SYS_FREE (sys_context, str);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to decode \"%s\" from base64 (prefix=\"%s\")"), base64_str, _TEST_BASE64_PREFIX);
	  ret = 0;
	}
      LW6SYS_FREE (sys_context, base64_str);
    }
  else
    {
      if (log_all)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to encode \"%s\" in base64 (prefix=\"%s\")"), _test_str, _TEST_BASE64_PREFIX);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to encode in base64 (prefix=\"%s\")"), _TEST_BASE64_PREFIX);
	}
      ret = 0;
    }

  return ret;
}

static int
_test_base64_ko (lw6sys_context_t * sys_context, char *_test_str, int log_all)
{
  int ret = 1;
  char *str = NULL;

  str = lw6glb_base64_decode_str (sys_context, _test_str);
  if (str)
    {
      if (strlen (str) > 0)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		      _x_
		      ("base64 routine was able to decode \"%s\", this is in theory wrong, but we just keep going to check if nothing real bad happen (buffer overflow, core dump...)"),
		      _test_str);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("base64 routine decoded \"\" to itself"));
	}
      LW6SYS_FREE (sys_context, str);
    }
  else
    {
      if (log_all)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to decode \"%s\", this is normal"), _test_str);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("unable to decode arbitrary non base64-like string, this is normal"));
	}
    }
  return ret;
}

/*
 * Testing functions in base64.c
 */
static void
_test_base64 ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *random_str = NULL;

    random_str = lw6sys_str_random (sys_context, _TEST_BASE64_STR_4_LEN);
    ret = _test_base64_ok (sys_context, _TEST_BASE64_STR_1, 1) && ret;
    ret = _test_base64_ok (sys_context, _TEST_BASE64_STR_2, 1) && ret;
    ret = _test_base64_ok (sys_context, _TEST_BASE64_STR_3, 1) && ret;
    if (random_str)
      {
	ret = _test_base64_ok (sys_context, random_str, 0) && ret;
      }
    ret = _test_base64_ko (sys_context, _TEST_BASE64_STR_1, 1) && ret;
    ret = _test_base64_ko (sys_context, _TEST_BASE64_STR_2, 1) && ret;
    ret = _test_base64_ko (sys_context, _TEST_BASE64_STR_3, 1) && ret;
    if (random_str)
      {
	ret = _test_base64_ko (sys_context, random_str, 0) && ret;
	LW6SYS_FREE (sys_context, random_str);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing functions in sha1.c
 */
static void
_test_sha1 ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *random_str = NULL;
    char *sha1_80 = NULL;
    u_int32_t sha1_32 = 0;

    random_str = lw6sys_str_random (sys_context, _TEST_SHA1_LEN);
    if (random_str)
      {
	sha1_80 = lw6glb_sha1_hmac_80_str (sys_context, NULL, random_str);
	sha1_32 = lw6glb_sha1_hmac_32_str (sys_context, NULL, random_str);
	if (sha1_80)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("sha1_80=%s sha1_32=%08x"), sha1_80, sha1_32);
	    LW6SYS_FREE (sys_context, sha1_80);
	  }
	else
	  {
	    ret = 0;
	  }
	sha1_80 = lw6glb_sha1_hmac_80_str (sys_context, _TEST_SHA1_KEY, random_str);
	sha1_32 = lw6glb_sha1_hmac_32_str (sys_context, _TEST_SHA1_KEY, random_str);
	if (sha1_80)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("sha1_80=%s sha1_32=%08x"), sha1_80, sha1_32);
	    LW6SYS_FREE (sys_context, sha1_80);
	  }
	else
	  {
	    ret = 0;
	  }
	sha1_80 = lw6glb_sha1_hmac_80_str (sys_context, _TEST_SHA1_KEY, _TEST_SHA1_KEY);
	sha1_32 = lw6glb_sha1_hmac_32_str (sys_context, _TEST_SHA1_KEY, _TEST_SHA1_KEY);
	if (sha1_80)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("sha1_80=%s sha1_32=%08x"), sha1_80, sha1_32);
	    LW6SYS_FREE (sys_context, sha1_80);
	  }
	else
	  {
	    ret = 0;
	  }
	if (sha1_32 == _TEST_SHA1_CHECK)
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("sha1 returned the right value on latest test"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			_x_ ("sha1 for \"%s\" returned %08x, should have been %08x"), _TEST_SHA1_KEY, sha1_32, _TEST_SHA1_CHECK);
	    ret = 0;
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

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libglb CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libglb CUnit test suite"));

  return CUE_SUCCESS;
}

/**
 * lw6glb_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libglb module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6glb_test_register (lw6sys_context_t * sys_context, int mode)
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
    }

  suite = CU_add_suite ("lw6glb", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_base64);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_sha1);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6map_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @glb module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6glb_test_run (lw6sys_context_t * sys_context, int mode)
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
