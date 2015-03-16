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

#include "hlp.h"

int
main (int argc, const char *argv[])
{
  int ret = 0;
  lw6sys_context_t *sys_context = NULL;
  int run_game = 1;

  LW6SYS_MAIN_BEGIN (sys_context);
  LW6HLP_MAIN_BEGIN (sys_context);

  ret = lw6hlp_process_non_run_options (sys_context, argc, argv, &run_game);
  if (ret && run_game)
    {
      if (argc > 1 && argv[0])
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("program \"%s\" is a stripped down version of \"%s\", it can't handle all options"), argv[0], lw6sys_build_get_package_tarname ());
	}
      else if (argc == 1 && argv[0])
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_ERROR,
		      _x_
		      ("program \"%s\" is a stripped down version of \"%s\", you need to pass it some args"), argv[0], lw6sys_build_get_package_tarname ());
	}
    }

  lw6sys_clear_memory_bazooka (sys_context);

  LW6HLP_MAIN_END (sys_context);
  LW6SYS_MAIN_END (sys_context);

  return (!ret);
}
