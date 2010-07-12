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

#include <unistd.h>

#include "sys.h"
#include "sys-internal.h"

#define CPUPROFILE "CPUPROFILE"
#define HEAPPROFILE "HEAPPROFILE"

static int
_cpu_profiler_check (int verbose)
{
  int ret = 0;
  char *cpuprofile = NULL;

  cpuprofile = lw6sys_getenv (CPUPROFILE);
  if (cpuprofile)
    {
#ifdef LW6_PROFILER
      if (verbose)
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE,
		      _("using Google CPU Profiler with CPUPROFILE=\"%s\""),
		      cpuprofile);
	}
      ret = 1;
#else
      if (verbose)
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("CPUPROFILE=\"%s\" but this binaray does not seem to support Google Profiler"),
		      cpuprofile);
	}
#endif
      LW6SYS_FREE (cpuprofile);
    }
  else
    {
#ifdef LW6_PROFILER
      if (verbose)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _("Google CPU Profiler supported but turned off"));
	}
#else
      if (verbose)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("no Google CPU Profiler support"));
	}
#endif
    }

  return ret;
}

static int
_heap_profiler_check (int verbose)
{
  int ret = 0;
  char *heapprofile = NULL;

  heapprofile = lw6sys_getenv (HEAPPROFILE);
  if (heapprofile)
    {
#ifdef LW6_PROFILER
      if (verbose)
	{
	  lw6sys_log (LW6SYS_LOG_NOTICE,
		      _("using Google HEAP Profiler with HEAPPROFILE=\"%s\""),
		      heapprofile);
	}
      ret = 1;
#else
      if (verbose)
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("HEAPPROFILE=\"%s\" but this binaray does not seem to support Google Profiler"),
		      heapprofile);
	}
#endif
      LW6SYS_FREE (heapprofile);
    }
  else
    {
#ifdef LW6_PROFILER
      if (verbose)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _("Google HEAP Profiler supported but turned off"));
	}
#else
      if (verbose)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("no Google HEAP Profiler support"));
	}
#endif
    }

  return ret;
}

/**
 * lw6sys_check_profiler
 *
 * @verbose: wether to display informations on the console
 *
 * Checks wether Google Profiler support has been built, and if
 * it's set, outputs the log file. If CPUPROFILE is defined but
 * binary has no support for it, will display a warning message.
 *
 * Return value: 1 if google profile enabled and activated, 0 if not
 */
int
lw6sys_profiler_check (int verbose)
{
  int ret = 1;

  ret = _cpu_profiler_check (verbose) && ret;
  ret = _heap_profiler_check (verbose) && ret;

  return ret;
}
