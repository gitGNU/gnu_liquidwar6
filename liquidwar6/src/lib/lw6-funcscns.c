/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013  Christian Mauduit <ufoot@ufoot.org>

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
_scm_lw6cns_support ()
{
  SCM ret = SCM_BOOL_F;
  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  ret = lw6cns_support ()? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/*
 * In backend.c
 */
static SCM
_scm_lw6cns_init ()
{
  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  if (!lw6_global.cns_initialized)
    {
      lw6cns_handler_install (lw6_cns_handler);
      lw6_global.cns_initialized = 1;
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6cns_quit ()
{
  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  if (lw6_global.cns_initialized)
    {
      lw6cns_handler_remove ();
      lw6_global.cns_initialized = 0;
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6cns_poll ()
{
  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  if (lw6_global.cns_initialized)
    {
      lw6cns_handler_poll ();
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

/**
 * lw6_register_funcs_cns
 *
 * Register the functions of the cns module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_cns ()
{
  int ret = 1;

  /*
   * In liquidwar6cns
   */
  /*
   * In support.c
   */
  ret = ret && lw6scm_c_define_gsubr (LW6DEF_C_LW6CNS_SUPPORT, 0, 0, 0,
				      (SCM (*)())_scm_lw6cns_support);
  /*
   * In handler.c
   */
  ret = ret && lw6scm_c_define_gsubr (LW6DEF_C_LW6CNS_INIT, 0, 0, 0,
				      (SCM (*)())_scm_lw6cns_init);
  ret = ret && lw6scm_c_define_gsubr (LW6DEF_C_LW6CNS_QUIT, 0, 0, 0,
				      (SCM (*)())_scm_lw6cns_quit);
  ret = ret && lw6scm_c_define_gsubr (LW6DEF_C_LW6CNS_POLL, 0, 0, 0,
				      (SCM (*)())_scm_lw6cns_poll);

  return ret;
}
