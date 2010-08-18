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

#define _TEST_BASE64_STR "this is a string"

/*
 * Testing functions in base64.c
 */
static int
test_base64 ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *base64_str = NULL;
    char *str = NULL;

    base64_str = lw6glb_base64_encode_str (_TEST_BASE64_STR);
    if (base64_str)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("base64 encoded version of \"%s\" is \"%s\""),
		    _TEST_BASE64_STR, base64_str);
	str = lw6glb_base64_decode_str (base64_str);
	if (str)
	  {
	    if (lw6sys_str_is_same (str, _TEST_BASE64_STR))
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _
			    ("base64 string \"%s\" correctly decoded to \"%s\""),
			    base64_str, str);
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _
			    ("base64 string \"%s\" decoded to \"%s\" but shoudl be \"%s\""),
			    base64_str, str, _TEST_BASE64_STR);
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
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _("unable to encode \"%s\" in base64"), _TEST_BASE64_STR);
	ret = 0;
      }

    str = lw6glb_base64_decode_str (_TEST_BASE64_STR);
    if (str)
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _
		    ("base64 routine was able to decode \"%s\" into \"%s\", this is wrong"),
		    _TEST_BASE64_STR, str);
	ret = 0;
	LW6SYS_FREE (str);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("unable to decode \"%s\", this is normal"),
		    _TEST_BASE64_STR);
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

  ret = test_base64 ();

  return ret;
}
