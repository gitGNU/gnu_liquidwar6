
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

#include "ldr.h"

/**
 * lw6ldr_process_non_run_options:
 *
 * @sys_context: global system context
 * @argc: the number of command-line args, as passed to main
 * @argv: an array of strings containing command-line args, as passed to main
 * @run_game: a pointer to a boolean which will contain true (1) if the
 *   game must be launched, or false (0) if the option is such that game
 *   must be skipped. Example: --copyright, --help, ...
 *
 * Will interpret the command-line arguments, and trap those who are
 * related to xml files, this is usefull when building the game,
 * we want to have an extra binary to do this without being linked
 * to SDL, for instance.
 *
 * Return value: non-zero if success, 0 if error. The error can be, for
 *   instance, the test suite returning "no, tests were not OK".
 */
int
lw6ldr_process_non_run_options (lw6sys_context_t * sys_context, int argc, const char *argv[], int *run_game)
{
  int i;
  int ret = 1;

  /*
   * It' s important to call lw6hlp_process_non_run_options, it will
   * check for bad options, and globally, we rely on it,
   * this function just superseds it and adds the XML handling.
   */
  ret = lw6hlp_process_non_run_options (sys_context, argc, argv, run_game);
  if (ret && (*run_game))
    {
      for (i = 1; i < argc; ++i)
	{
	  if (lw6sys_arg_match (sys_context, LW6DEF_EXAMPLE_RULES_XML, argv[i]))
	    {
	      lw6ldr_print_example_rules_xml (sys_context, stdout);
	      (*run_game) = 0;
	    }
	  else if (lw6sys_arg_match (sys_context, LW6DEF_EXAMPLE_HINTS_XML, argv[i]))
	    {
	      lw6ldr_print_example_hints_xml (sys_context, stdout);
	      (*run_game) = 0;
	    }
	  else if (lw6sys_arg_match (sys_context, LW6DEF_EXAMPLE_STYLE_XML, argv[i]))
	    {
	      lw6ldr_print_example_style_xml (sys_context, stdout);
	      (*run_game) = 0;
	    }
	  else if (lw6sys_arg_match (sys_context, LW6DEF_EXAMPLE_TEAMS_XML, argv[i]))
	    {
	      lw6ldr_print_example_teams_xml (sys_context, stdout);
	      (*run_game) = 0;
	    }
	}
    }

  return ret;
}
