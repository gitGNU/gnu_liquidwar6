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

#include "dsp.h"

#ifdef LW6_MAC_OS_X
#include <SDL/SDL.h>
#endif

static int _ret = 1;
static int _mode = 0;

static void
_run (lw6sys_context_t * sys_context, void *data)
{
  if (CU_initialize_registry () == CUE_SUCCESS)
    {
      if (lw6dsp_test_register (sys_context, _mode))
	{
	  _ret = lw6dsp_test_run (sys_context, _mode);
	}
      CU_cleanup_registry ();
    }
}

/*
 * On Mac OS X, SDL/SDL.h will redefine main, override it, whatever,
 * anyway it needs it to be "int argc, char **argv" so *without* the
 * const attribute on argv. On the other hand, using const is nice
 * on other platforms since, in reality, it *is* const. So the
 * workarround is to have the #define below.
 */
#ifdef LW6_MAC_OS_X
int
main (int argc, char *argv[])
#else // LW6_MAC_OS_X
int
main (int argc, const char *argv[])
#endif				// LW6_MAC_OS_X
{
  int ret = 0;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_MAIN_BEGIN (sys_context);
  LW6HLP_MAIN_BEGIN (sys_context);

  lw6sys_log_clear (sys_context, NULL);
  _mode = lw6sys_arg_test_mode (sys_context, argc, (const char **) argv);
#ifdef LW6_MAC_OS_X		// LW6_MAC_OS_X
  lw6sys_vthread_run (sys_context, _run, NULL, NULL);
#else // LW6_MAC_OS_X
  _run (sys_context, NULL);
#endif // LW6_MAC_OS_X
  ret = _ret;

  LW6SYS_TEST_OUTPUT;

  LW6HLP_MAIN_END (sys_context);
  LW6SYS_MAIN_END (sys_context);

  return (!ret);
}
