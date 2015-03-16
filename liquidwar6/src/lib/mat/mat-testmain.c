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

#include "mat.h"

int
main (int argc, const char *argv[])
{
  int ret = 0;
  lw6sys_context_t *sys_context = NULL;
  int mode = 0;

  LW6SYS_MAIN_BEGIN (sys_context);

  lw6sys_log_clear (sys_context, NULL);
  mode = lw6sys_arg_test_mode (sys_context, argc, argv);

  if (CU_initialize_registry () == CUE_SUCCESS)
    {
      if (lw6mat_test_register (sys_context, mode))
	{
	  ret = lw6mat_test_run (sys_context, mode);
	}
      CU_cleanup_registry ();
    }

  LW6SYS_TEST_OUTPUT;

  LW6SYS_MAIN_END (sys_context);

  return (!ret);
}
