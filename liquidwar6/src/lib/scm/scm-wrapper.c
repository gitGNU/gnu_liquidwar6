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

#include "scm.h"
#include "scm-internal.h"

#ifdef LW6_GUILE2
#ifdef HAVE_GC_GC_H
#include <gc/gc.h>
#endif // HAVE_GC_GC_H
#endif // LW6_GUILE2


void *
_lw6scm_wrapper_callback_func (void *data)
{
  void *ret = NULL;
  _lw6scm_wrapper_callback_data_t *callback_data = (_lw6scm_wrapper_callback_data_t *) data;

  if (data)
    {
      lw6sys_context_t *sys_context = callback_data->sys_context;
      lw6scm_callback_t orig_func = callback_data->func;
      void *orig_data = callback_data->data;

      if (sys_context)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("running function %p(%p) in Guile context, inside callback"), orig_func, orig_data);

	  if (orig_func)
	    {
	      ret = orig_func (sys_context, orig_data);
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("function %p(%p) in Guile context returned %p"), orig_func, orig_data, ret);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("function is NULL, executing nothing in Guile context"));
	    }
	}
    }

  return ret;
}

/**
 * lw6scm_c_define_gsubr
 *
 * @sys_context: global system context
 * @name: name of the function when called from guile
 * @req: required parameters
 * @opt: optional parameters
 * @rst: ? should RTFM to find that out
 * @fcn: the function itself (pointer on the C executable code)
 *
 * Wrapper on @scm_c_define_gsubr, one of the value of this function is that
 * it does check wether it's documented before registering it. So if you
 * try to register something not documented, it will fire a warning, which
 * is a very nice code-quality tool.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6scm_c_define_gsubr (lw6sys_context_t * sys_context, const char *name, int req, int opt, int rst, lw6scm_func_t fcn)
{
  int ret = 0;

  if (lw6hlp_is_documented (sys_context, name))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("registering Guile function \"%s\""), name);
      scm_c_define_gsubr ((const char *) name, req, opt, rst, fcn);
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("Guile function \"%s\" not documented, won't register it"), name);
    }

  return ret;
}

/**
 * lw6scm_c_primitive_load
 *
 * @sys_context: global system context
 * @filename: file to execute
 *
 * Loads and executes a script. Will add a log message while doing it.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6scm_c_primitive_load (lw6sys_context_t * sys_context, const char *filename)
{
  int ret = 0;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("loading \"%s\""), filename);

  if (lw6sys_file_exists (sys_context, filename))
    {
      scm_c_primitive_load ((const char *) filename);
      // todo, check that it does really work
      ret = 1;
    }
  else
    {
      /*
       * Well, actually, in the calling code, the
       * check for file existence is also done
       * so this is just "in case".
       */
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to read script file \"%s\""), filename);
    }

  return ret;
}

/**
 * lw6scm_with_guile
 *
 * @sys_context: global system context
 * @func: callback to use
 * @data: data to pass to callback
 *
 * Initializes Guile and calls function within it.
 *
 * Return value: callback return value.
 */
void *
lw6scm_with_guile (lw6sys_context_t * sys_context, lw6scm_callback_t func, void *data)
{
  _lw6scm_wrapper_callback_data_t callback_data;

  memset (&callback_data, 0, sizeof (callback_data));
  callback_data.sys_context = sys_context;
  callback_data.func = func;
  callback_data.data = data;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("running function %p(%p) in Guile context, outside callback"), func, data);
#ifdef LW6_GUILE2
  /*
   * When using Guile2, looks like Valgrind reports a bunch of non-initialized
   * stuff, eg:
   *
   * ==32242== Conditional jump or move depends on uninitialised value(s)
   * * ==32242==    at 0x51CD744: GC_push_all_eager (in /usr/lib/libgc.so.1.0.3)
   * ==32242==    by 0x51D60BD: GC_with_callee_saves_pushed (in /usr/lib/libgc.so.1.0.3)
   * ==32242==    by 0x51CEDEE: GC_push_roots (in /usr/lib/libgc.so.1.0.3)
   * ==32242==    by 0x51CE1FA: GC_mark_some (in /usr/lib/libgc.so.1.0.3)
   * ==32242==    by 0x51C5239: GC_stopped_mark (in /usr/lib/libgc.so.1.0.3)
   * ==32242==    by 0x51C590E: GC_try_to_collect_inner (in /usr/lib/libgc.so.1.0.3)
   * ==32242==    by 0x51D00AF: GC_init_inner (in /usr/lib/libgc.so.1.0.3)
   * ==32242==    by 0x4EA7C12: ??? (in /usr/lib/libguile-2.0.so.22.4.0)
   * ==32242==    by 0x4EB6D27: ??? (in /usr/lib/libguile-2.0.so.22.4.0)
   * ==32242==    by 0x4F0980F: ??? (in /usr/lib/libguile-2.0.so.22.4.0)
   * ==32242==    by 0x4F09846: ??? (in /usr/lib/libguile-2.0.so.22.4.0)
   * ==32242==    by 0x51CFCC7: GC_call_with_stack_base (in /usr/lib/libgc.so.1.0.3)
   * ==32242==
   * ==32242== Use of uninitialised value of size 8
   * ==32242==    at 0x51CD52D: GC_mark_and_push_stack (in /usr/lib/libgc.so.1.0.3)
   * ==32242==    by 0x51CD74E: GC_push_all_eager (in /usr/lib/libgc.so.1.0.3)
   * ==32242==    by 0x51D60BD: GC_with_callee_saves_pushed (in /usr/lib/libgc.so.1.0.3)
   * ==32242==    by 0x51CEDEE: GC_push_roots (in /usr/lib/libgc.so.1.0.3)
   * ==32242==    by 0x51CE1FA: GC_mark_some (in /usr/lib/libgc.so.1.0.3)
   * ==32242==    by 0x51C5239: GC_stopped_mark (in /usr/lib/libgc.so.1.0.3)
   * ==32242==    by 0x51C590E: GC_try_to_collect_inner (in /usr/lib/libgc.so.1.0.3)
   * ==32242==    by 0x51D00AF: GC_init_inner (in /usr/lib/libgc.so.1.0.3)
   * ==32242==    by 0x4EA7C12: ??? (in /usr/lib/libguile-2.0.so.22.4.0)
   * ==32242==    by 0x4EB6D27: ??? (in /usr/lib/libguile-2.0.so.22.4.0)
   * ==32242==    by 0x4F0980F: ??? (in /usr/lib/libguile-2.0.so.22.4.0)
   * ==32242==    by 0x4F09846: ??? (in /usr/lib/libguile-2.0.so.22.4.0)
   *
   * So what we could do is just force a call to GC_INIT, just in case.
   * This is only for Guile2, Guile1 does not use GC. But anyway calling
   * GC_INIT() here will cause a segfault on the later call within Guile...
   * So my for now, we just do nothing, maybe it's just the Valgrind context
   * that is broken, since Valgrind itself fiddles with threads.
   *
   * See http://thread.gmane.org/gmane.lisp.guile.bugs/5340
   */
#endif // LW6_GUILE2
  return scm_with_guile (_lw6scm_wrapper_callback_func, (void *) &callback_data);
}
