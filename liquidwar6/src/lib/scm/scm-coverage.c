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

#include "scm.h"

typedef struct _coverage_check_s
{
  int *ret;
  lw6sys_hash_t *coverage;
  int n;
} _coverage_check_t;

/**
 * lw6scm_coverage_new
 *
 * @sys_context: global system context
 * @funcs: list of functions, used as an input to size the hash correctly
 *
 * Creates a new coverage hash, this is a simple hash containing pointers
 * to integers. At startup hash is empty, whenever a scheme user function is
 * called, if the entry exists it's incremented, else it's created with an
 * initial value of 1 (one call).
 *
 * Return value: newly allocated hash
 */
lw6sys_hash_t *
lw6scm_coverage_new (lw6sys_context_t * sys_context, lw6sys_list_t * funcs)
{
  lw6sys_hash_t *ret = NULL;
  int funcs_size = 0;
  int ret_size = 0;

  funcs_size = lw6sys_list_length (sys_context, funcs);
  ret_size = (funcs_size) / 2 + 1;
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("init coverage hash with %d slots for a max of %d funcs"), ret_size, funcs_size);
  ret = lw6sys_hash_new (sys_context, lw6sys_free_callback, ret_size);

  return ret;
}

/**
 * lw6scm_coverage_call
 *
 * Registers a call on a given function.
 *
 * @sys_context: global system context
 * @coverage: the hash to use to store the update
 * @func: the name of the function (its callable scheme name)
 *
 * Return value: none.
 */
void
lw6scm_coverage_call (lw6sys_context_t * sys_context, lw6sys_hash_t * coverage, const char *func)
{
  void *value = NULL;
  int *calls;

  if (coverage)
    {
      value = lw6sys_hash_get (sys_context, coverage, func);
      if (value)
	{
	  calls = (int *) value;
	  ++(*calls);
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("calling scm function \"%s\", #%d"), func, (*calls));
	}
      else
	{
	  calls = (int *) LW6SYS_MALLOC (sys_context, sizeof (int));
	  if (calls)
	    {
	      (*calls) = 1;
	      lw6sys_hash_set (sys_context, coverage, func, calls);
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("calling scm function \"%s\", 1st call"), func);
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to register function call on NULL coverage"));
    }
}

static void
_coverage_log_callback (lw6sys_context_t * sys_context, void *func_data, const char *key, void *value)
{
  int calls = 0;

  if (value)
    {
      calls = *((int *) value);
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("script function \"%s\" called %d times"), key, calls);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("script function \"%s\" has NULL coverage entry"), key);
    }
}

/**
 * lw6scm_coverage_log
 *
 * Logs the information about which function has been called, and how many times.
 * This is only about scheme functions.
 *
 * @sys_context: global system context
 * @coverage: the hash containing the call data
 *
 * Return value: none
 */
void
lw6scm_coverage_log (lw6sys_context_t * sys_context, lw6sys_hash_t * coverage)
{
  if (coverage)
    {
      lw6sys_hash_map (sys_context, coverage, _coverage_log_callback, NULL);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to log NULL coverage"));
    }
}

static void
_coverage_check_callback (lw6sys_context_t * sys_context, void *func_data, void *value)
{
  const char *key = (const char *) value;
  _coverage_check_t *cc = (_coverage_check_t *) func_data;
  char *c_name = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("checking script function \"%s\""), key);
  c_name = lw6scm_funcname_scm2c (sys_context, key);
  if (c_name)
    {
      if (lw6sys_hash_has_key (sys_context, cc->coverage, c_name))
	{
	  ++(cc->n);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("no call to \"%s\""), key);
	  *(cc->ret) = 0;
	}
      LW6SYS_FREE (sys_context, c_name);
    }
}

/**
 * lw6scm_coverage_check
 *
 * @sys_context: global system context
 * @percent: if not NULL, will contain the percentage of coverage
 * @coverage: object to query, containing coverage information
 * @funcs: list of functions to check
 *
 * Checks wether the script code coverage is fine, that is, wether all functions
 * are called at least once. For each function listed (as a string) in funcs, it will
 * look in coverage and check wether the entry exists and was called. Note that this
 * function assumes that in c functions are defined with _scm_my_function (prefixed
 * with _scm_ and using underscore) while in scm functions are defined with c-my-function
 * (prefixed with c- and using minus).
 *
 * Return value: 1 if OK, 0 if KO.
 */
int
lw6scm_coverage_check (lw6sys_context_t * sys_context, int *percent, lw6sys_hash_t * coverage, lw6sys_list_t * funcs)
{
  int ret = 0;
  int funcs_size = 0;
  _coverage_check_t cc;

  if (coverage)
    {
      ret = 1;
      memset (&cc, 0, sizeof (_coverage_check_t));
      funcs_size = lw6sys_list_length (sys_context, funcs);
      cc.ret = &ret;
      cc.coverage = coverage;

      lw6sys_list_map (sys_context, funcs, _coverage_check_callback, (void *) &cc);
      if (percent != NULL && funcs_size > 0)
	{
	  (*percent) = (100 * cc.n) / funcs_size;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to check NULL coverage"));
    }

  return ret;
}
