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

#include "glb.h"

#define _TEST_BASE64_STR_1 "this is a string!"
#define _TEST_BASE64_STR_2 ""
#define _TEST_BASE64_STR_3 "this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string, this is a LONG... string"
#define _TEST_BASE64_STR_4_LEN 100
#define _TEST_BASE64_PREFIX "TOTO=0"
#define _TEST_SHA1_LEN 1000
#define _TEST_SHA1_KEY "toto"
#define _TEST_SHA1_CHECK 0xd4a35ebb

static int
_test_base64_ok (char *test_str, int log_all)
{
  int ret = 1;
  char *base64_str = NULL;
  char *str = NULL;

  base64_str = lw6glb_base64_encode_str (test_str);
  if (base64_str)
    {
      if (log_all)
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE,
		      _("base64 encoded version of \"%s\" is \"%s\""),
		      test_str, base64_str);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE,
		      _("base64 encoding, result \"%s\""), base64_str);
	}
      str = lw6glb_base64_decode_str (base64_str);
      if (str)
	{
	  if (lw6sys_str_is_same (str, test_str))
	    {
	      if (log_all)
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _
			      ("base64 string \"%s\" correctly decoded to \"%s\""),
			      base64_str, str);
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _
			      ("base64 string \"%s\" correctly decoded"),
			      base64_str);
		}
	    }
	  else
	    {
	      if (log_all)
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _
			      ("base64 string \"%s\" decoded to \"%s\" but should be \"%s\""),
			      base64_str, str, test_str);
		}
	      ret = 0;
	    }
	  LW6SYS_FREE (str);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("unable to decode \"%s\" from base64"), base64_str);
	  ret = 0;
	}
      LW6SYS_FREE (base64_str);
    }
  else
    {
      if (log_all)
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("unable to encode \"%s\" in base64"), test_str);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING, _("unable to encode in base64"));
	}
      ret = 0;
    }

  base64_str =
    lw6glb_base64_encode_str_prefix (test_str, _TEST_BASE64_PREFIX);
  if (base64_str)
    {
      if (log_all)
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE,
		      _
		      ("base64 encoded version of \"%s\" is \"%s\" (prefix=\"%s\")"),
		      test_str, base64_str, _TEST_BASE64_PREFIX);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE,
		      _
		      ("base64 encoding result is \"%s\" (prefix=\"%s\")"),
		      base64_str, _TEST_BASE64_PREFIX);
	}
      str = lw6glb_base64_decode_str_prefix (base64_str, _TEST_BASE64_PREFIX);
      if (str)
	{
	  if (lw6sys_str_is_same (str, test_str))
	    {
	      if (log_all)
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _
			      ("base64 string \"%s\" correctly decoded to \"%s\" (prefix=\"%s\")"),
			      base64_str, str, _TEST_BASE64_PREFIX);
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _
			      ("base64 string \"%s\" correctly decoded (prefix=\"%s\")"),
			      base64_str, _TEST_BASE64_PREFIX);
		}
	    }
	  else
	    {
	      if (log_all)
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _
			      ("base64 string \"%s\" decoded to \"%s\" but should be \"%s\" (prefix=\"%s\")"),
			      base64_str, str, test_str, _TEST_BASE64_PREFIX);
		}
	      ret = 0;
	    }
	  LW6SYS_FREE (str);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("unable to decode \"%s\" from base64 (prefix=\"%s\")"),
		      base64_str, _TEST_BASE64_PREFIX);
	  ret = 0;
	}
      LW6SYS_FREE (base64_str);
    }
  else
    {
      if (log_all)
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("unable to encode \"%s\" in base64 (prefix=\"%s\")"),
		      test_str, _TEST_BASE64_PREFIX);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("unable to encode in base64 (prefix=\"%s\")"),
		      _TEST_BASE64_PREFIX);
	}
      ret = 0;
    }

  return ret;
}

static int
_test_base64_ko (char *test_str, int log_all)
{
  int ret = 1;
  char *str = NULL;

  str = lw6glb_base64_decode_str (test_str);
  if (str)
    {
      if (strlen (str) > 0)
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE,
		      _
		      ("base64 routine was able to decode \"%s\", this is in theory wrong, but we just keep going to check if nothing real bad happen (buffer overflow, core dump...)"),
		      test_str);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE,
		      _("base64 routine decoded \"\" to itself"));
	}
      LW6SYS_FREE (str);
    }
  else
    {
      if (log_all)
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE,
		      _("unable to decode \"%s\", this is normal"), test_str);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE,
		      _
		      ("unable to decode arbitrary non base64-like string, this is normal"));
	}
    }
  return ret;
}

/*
 * Testing functions in base64.c
 */
static int
test_base64 ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *random_str = NULL;

    random_str = lw6sys_str_random (_TEST_BASE64_STR_4_LEN);
    ret = _test_base64_ok (_TEST_BASE64_STR_1, 1) && ret;
    ret = _test_base64_ok (_TEST_BASE64_STR_2, 1) && ret;
    ret = _test_base64_ok (_TEST_BASE64_STR_3, 1) && ret;
    if (random_str)
      {
	ret = _test_base64_ok (random_str, 0) && ret;
      }
    ret = _test_base64_ko (_TEST_BASE64_STR_1, 1) && ret;
    ret = _test_base64_ko (_TEST_BASE64_STR_2, 1) && ret;
    ret = _test_base64_ko (_TEST_BASE64_STR_3, 1) && ret;
    if (random_str)
      {
	ret = _test_base64_ko (random_str, 0) && ret;
	LW6SYS_FREE (random_str);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in sha1.c
 */
static int
test_sha1 ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *random_str = NULL;
    char *sha1_80 = NULL;
    u_int32_t sha1_32 = 0;

    random_str = lw6sys_str_random (_TEST_SHA1_LEN);
    if (random_str)
      {
	sha1_80 = lw6glb_sha1_hmac_80_str (NULL, random_str);
	sha1_32 = lw6glb_sha1_hmac_32_str (NULL, random_str);
	if (sha1_80)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _("sha1_80=%s sha1_32=%08x"),
			sha1_80, sha1_32);
	    LW6SYS_FREE (sha1_80);
	  }
	else
	  {
	    ret = 0;
	  }
	sha1_80 = lw6glb_sha1_hmac_80_str (_TEST_SHA1_KEY, random_str);
	sha1_32 = lw6glb_sha1_hmac_32_str (_TEST_SHA1_KEY, random_str);
	if (sha1_80)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _("sha1_80=%s sha1_32=%08x"),
			sha1_80, sha1_32);
	    LW6SYS_FREE (sha1_80);
	  }
	else
	  {
	    ret = 0;
	  }
	sha1_80 = lw6glb_sha1_hmac_80_str (_TEST_SHA1_KEY, _TEST_SHA1_KEY);
	sha1_32 = lw6glb_sha1_hmac_32_str (_TEST_SHA1_KEY, _TEST_SHA1_KEY);
	if (sha1_80)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _("sha1_80=%s sha1_32=%08x"),
			sha1_80, sha1_32);
	    LW6SYS_FREE (sha1_80);
	  }
	else
	  {
	    ret = 0;
	  }
	if (sha1_32 == _TEST_SHA1_CHECK)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_("sha1 returned the right value on latest test"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_
			("sha1 for \"%s\" returned %08x, should have been %08x"),
			_TEST_SHA1_KEY, sha1_32, _TEST_SHA1_CHECK);
	    ret = 0;
	  }
	LW6SYS_FREE (random_str);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6glb_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @glb module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6glb_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
    }

  ret = test_base64 () && test_sha1 ();

  return ret;
}
