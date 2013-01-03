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
#endif

#include "hlp.h"

#define TEST_KEYWORD LW6DEF_PREFIX
#define _TEST_MAX_CREDITS 50

/*
 * Testing about
 */
static int
test_about ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    const char *help_string = NULL;
    lw6hlp_type_t type;
    const char *default_value;
    int min_value;
    int max_value;

    if (!lw6hlp_is_documented (TEST_KEYWORD))
      {
	ret = 0;
      }

    help_string = lw6hlp_about (NULL, NULL, NULL, NULL, TEST_KEYWORD);
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("help for \"%s\" is \"%s\""),
		TEST_KEYWORD, help_string);
    type = lw6hlp_get_type (TEST_KEYWORD);
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("type for \"%s\" is \"%d\""),
		TEST_KEYWORD, (int) type);
    default_value = lw6hlp_get_default_value (TEST_KEYWORD);
    if (default_value)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("default value for \"%s\" is \"%s\""), TEST_KEYWORD,
		    default_value);
      }
    min_value = lw6hlp_get_min_value (TEST_KEYWORD);
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("min value for \"%s\" is \"%d\""),
		TEST_KEYWORD, min_value);
    max_value = lw6hlp_get_max_value (TEST_KEYWORD);
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("max value for \"%s\" is \"%d\""),
		TEST_KEYWORD, max_value);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing credits
 */
static int
test_credits ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i;
    char *credits = NULL;

    for (i = 0; i < _TEST_MAX_CREDITS; ++i)
      {
	credits = lw6hlp_get_credits (i);
	if (credits)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("credit line %d: \"%s\""), i,
			credits);
	    LW6SYS_FREE (credits);
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
 * Testing print
 */
static int
test_print ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6sys_list_t *list;

    list = lw6hlp_list_quick ();
    if (list)
      {
	lw6hlp_print_keyword (&list, stdout);
	lw6sys_list_free (list);
      }
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_ ("now showing what \"--about=%s\" would look like"),
		TEST_KEYWORD);
    lw6hlp_print_about (TEST_KEYWORD, stdout);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6hlp_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @hlp module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6hlp_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
    }

  ret = test_about () && test_credits () && test_print ();

  return ret;
}
