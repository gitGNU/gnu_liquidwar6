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

#include "dat.h"

#define _TEST_LINE_TEXT_SHORT "this is a short text"
#define _TEST_LINE_TEXT_LONG "this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text, this is a long text..."
#define _TEST_MATRIX_LINE_0_SERIAL 123
/*
 * Testing functions in line.c
 */
static int
test_line ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6dat_line_t line;
    char *text = NULL;

    lw6dat_line_zero (&line);
    if (lw6dat_line_get_text (&line))
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _("get text returned something on a zeroed line"));
	ret = 0;
      }
    lw6dat_line_set_text (&line, _TEST_LINE_TEXT_SHORT);
    text = lw6dat_line_get_text (&line);
    if (text)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("line text is \"%s\""), text);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("unable to get line text"));
      }
    lw6dat_line_set_text (&line, _TEST_LINE_TEXT_LONG);
    text = lw6dat_line_get_text (&line);
    if (text)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("line text is \"%s\""), text);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("unable to get line text"));
      }
    lw6dat_line_set_text (&line, _TEST_LINE_TEXT_SHORT);
    text = lw6dat_line_get_text (&line);
    if (text)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("line text is \"%s\""), text);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("unable to get line text"));
      }
    lw6dat_line_set_text (&line, _TEST_LINE_TEXT_LONG);
    text = lw6dat_line_get_text (&line);
    if (text)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE, _("line text is \"%s\""), text);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("unable to get line text"));
      }
    lw6dat_line_clear (&line);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in matrix.c
 */
static int
test_matrix ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6dat_matrix_t *matrix;

    matrix = lw6dat_matrix_new (_TEST_MATRIX_LINE_0_SERIAL);
    if (matrix)
      {
	lw6dat_matrix_free (matrix);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("unable to create matrix"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing functions in warehouse.c
 */
static int
test_warehouse ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6dat_warehouse_t *warehouse = NULL;

    warehouse = lw6dat_warehouse_new ();
    if (warehouse)
      {
	lw6dat_warehouse_free (warehouse);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING, _("unable to create warehouse"));
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6dat_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @dat module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6dat_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
    }

  ret = test_line () && test_matrix () && test_warehouse ();

  return ret;
}
