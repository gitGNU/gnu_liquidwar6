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
 * In liquidwar6net
 */

/*
 * In setup.c
 */
static SCM
_scm_lw6net_init (SCM net_log)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_net_log = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_BOOLP (net_log), net_log, SCM_ARG1, __FUNCTION__);

  c_net_log = SCM_NFALSEP (net_log);
  if (lw6net_init (sys_context, lw6_global.argc, lw6_global.argv, c_net_log))
    {
      lw6_global.net_initialized = 1;
      ret = SCM_BOOL_F;
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6net_quit ()
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  if (lw6_global.net_initialized)
    {
      lw6net_quit (sys_context);
    }
  lw6_global.net_initialized = 0;

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

/**
 * lw6_register_funcs_net
 *
 * @sys_context: global system context
 *
 * Register the functions of the net module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_net (lw6sys_context_t * sys_context)
{
  int ret = 1;

  /*
   * In liquidwar6net
   */
  /*
   * In setup.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6NET_INIT, 1, 0, 0, (SCM (*)())_scm_lw6net_init);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6NET_QUIT, 0, 0, 0, (SCM (*)())_scm_lw6net_quit);

  return ret;
}
