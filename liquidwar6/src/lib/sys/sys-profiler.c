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

#include <unistd.h>

#include "sys.h"
#include "sys-internal.h"

#define CPUPROFILE "CPUPROFILE"
#define HEAPPROFILE "HEAPPROFILE"

static void
_cleanup_first_char (lw6sys_context_t * sys_context, char *env)
{
  /*
   * For some reason google-perftools fiddle
   * with env vars and sign bit is set, wrecking
   * log information. So we set in to 0 to clean
   * the log, letting Google use whatever they
   * want, here we use a copy of the value.
   */
  (*env) = (*env) & 0x7f;
}

static int
_cpu_profiler_check (lw6sys_context_t * sys_context, int verbose)
{
  int ret = 0;
  char *cpuprofile = NULL;

  cpuprofile = lw6sys_getenv (sys_context, CPUPROFILE);
  if (cpuprofile)
    {
      _cleanup_first_char (sys_context, cpuprofile);
#ifdef LW6_PROFILER
      if (verbose)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("using Google CPU Profiler with CPUPROFILE=\"%s\""), cpuprofile);
	}
      ret = 1;
#else
      if (verbose)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("CPUPROFILE=\"%s\" but this binaray does not seem to support Google Profiler"), cpuprofile);
	}
#endif
      LW6SYS_FREE (sys_context, cpuprofile);
    }
  else
    {
#ifdef LW6_PROFILER
      if (verbose)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("Google CPU Profiler supported but turned off"));
	}
#else
      if (verbose)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("no Google CPU Profiler support"));
	}
#endif
    }

  return ret;
}

static int
_heap_profiler_check (lw6sys_context_t * sys_context, int verbose)
{
  int ret = 0;
  char *heapprofile = NULL;

  heapprofile = lw6sys_getenv (sys_context, HEAPPROFILE);
  if (heapprofile)
    {
      _cleanup_first_char (sys_context, heapprofile);
#ifdef LW6_PROFILER
      if (verbose)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("using Google HEAP Profiler with HEAPPROFILE=\"%s\""), heapprofile);
	}
      ret = 1;
#else
      if (verbose)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("HEAPPROFILE=\"%s\" but this binaray does not seem to support Google Profiler"), heapprofile);
	}
#endif
      LW6SYS_FREE (sys_context, heapprofile);
    }
  else
    {
#ifdef LW6_PROFILER
      if (verbose)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("Google HEAP Profiler supported but turned off"));
	}
#else
      if (verbose)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("no Google HEAP Profiler support"));
	}
#endif
    }

  return ret;
}

/**
 * lw6sys_check_profiler
 *
 * @sys_context: global system context
 * @verbose: wether to display informations on the console
 *
 * Checks wether Google Profiler support has been built, and if
 * it's set, outputs the log file. If CPUPROFILE is defined but
 * binary has no support for it, will display a warning message.
 *
 * Return value: 1 if google profile enabled and activated, 0 if not
 */
int
lw6sys_profiler_check (lw6sys_context_t * sys_context, int verbose)
{
  int ret = 1;

  ret = _cpu_profiler_check (sys_context, verbose) && ret;
  ret = _heap_profiler_check (sys_context, verbose) && ret;

  return ret;
}
