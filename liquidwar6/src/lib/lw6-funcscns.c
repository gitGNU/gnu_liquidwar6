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

#include "liquidwar6.h"

/*
 * In liquidwar6cns
 */

/*
 * In support.c
 */
static SCM
_scm_lw6cns_console_support ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = lw6cns_console_support (sys_context) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6cns_term_support ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = lw6cns_term_support (sys_context) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/*
 * In backend.c
 */
static SCM
_scm_lw6cns_init ()
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  if (!lw6_global.cns_initialized)
    {
      lw6cns_handler_install (sys_context, lw6_cns_handler);
      lw6_global.cns_initialized = 1;
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6cns_quit ()
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  if (lw6_global.cns_initialized)
    {
      lw6cns_handler_remove (sys_context);
      lw6_global.cns_initialized = 0;
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6cns_poll ()
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  if (lw6_global.cns_initialized)
    {
      lw6cns_handler_poll (sys_context);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

/**
 * lw6_register_funcs_cns
 *
 * @sys_context: global system context
 *
 * Register the functions of the cns module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_cns (lw6sys_context_t * sys_context)
{
  int ret = 1;

  /*
   * In liquidwar6cns
   */
  /*
   * In support.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6CNS_CONSOLE_SUPPORT, 0, 0, 0, (SCM (*)())_scm_lw6cns_console_support);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6CNS_TERM_SUPPORT, 0, 0, 0, (SCM (*)())_scm_lw6cns_term_support);
  /*
   * In handler.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6CNS_INIT, 0, 0, 0, (SCM (*)())_scm_lw6cns_init);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6CNS_QUIT, 0, 0, 0, (SCM (*)())_scm_lw6cns_quit);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6CNS_POLL, 0, 0, 0, (SCM (*)())_scm_lw6cns_poll);

  return ret;
}
