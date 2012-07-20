/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

#include "liquidwar6.h"

/*
 * For a GNU gettext-like behavior of scheme code.
 */
static SCM
_scm_gettext (SCM string)
{
  char *c_string;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (string), string, SCM_ARG1, __FUNCTION__);

  c_string = lw6scm_utils_to_0str (string);
  if (c_string)
    {
      ret = scm_from_locale_string (gettext (c_string));
      LW6SYS_FREE (c_string);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/*
 * Global control functions
 */
static SCM
_scm_lw6_release ()
{
  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  lw6_release ();

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6_exit ()
{
  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  lw6_exit ();

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6_set_ret (SCM ret)
{
  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_BOOLP (ret), ret, SCM_ARG1, __FUNCTION__);

  lw6_set_ret (SCM_NFALSEP (ret));

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6_get_ret ()
{
  SCM ret = SCM_UNDEFINED;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (lw6_global.coverage, __FUNCTION__);

  ret = lw6_get_ret ()? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/**
 * lw6_register_funcs
 *
 * Register all the functions, make them callable from Guile.
 * This is a very simple yet long and very usefull function,
 * without it Guile has no knowledge of what LW6 is.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs ()
{
  int ret = 1;

  ret = ret
    && lw6scm_c_define_gsubr ("C-GETTEXT", 1, 0, 0, (SCM (*)())_scm_gettext);
  /*
   * This one uses scm_define_gsubr and not the binding
   * lw6scm_c_define_gsubr for _ is not documented due
   * to internal parsing limitations.
   */
  scm_c_define_gsubr ("_", 1, 0, 0, (SCM (*)())_scm_gettext);

  ret = ret && lw6_register_funcs_sys ();
  ret = ret && lw6_register_funcs_hlp ();
  ret = ret && lw6_register_funcs_cfg ();
  ret = ret && lw6_register_funcs_gui ();
  ret = ret && lw6_register_funcs_dsp ();
  ret = ret && lw6_register_funcs_ldr ();
  ret = ret && lw6_register_funcs_map ();
  ret = ret && lw6_register_funcs_ker ();
  ret = ret && lw6_register_funcs_pil ();
  ret = ret && lw6_register_funcs_snd ();
  ret = ret && lw6_register_funcs_cns ();
  ret = ret && lw6_register_funcs_tsk ();
  ret = ret && lw6_register_funcs_net ();
  ret = ret && lw6_register_funcs_cli ();
  ret = ret && lw6_register_funcs_srv ();
  ret = ret && lw6_register_funcs_p2p ();
  ret = ret && lw6_register_funcs_bot ();

  /*
   * Global control funcs
   */
  ret = ret && lw6scm_c_define_gsubr (LW6DEF_C_LW6_RELEASE, 0, 0, 0,
				      (SCM (*)())_scm_lw6_release);
  ret = ret && lw6scm_c_define_gsubr (LW6DEF_C_LW6_EXIT, 0, 0, 0,
				      (SCM (*)())_scm_lw6_exit);
  ret = ret && lw6scm_c_define_gsubr (LW6DEF_C_LW6_SET_RET, 1, 0, 0,
				      (SCM (*)())_scm_lw6_set_ret);
  ret = ret && lw6scm_c_define_gsubr (LW6DEF_C_LW6_GET_RET, 0, 0, 0,
				      (SCM (*)())_scm_lw6_get_ret);

  return ret;
}

/**
 * lw6_cns_handler
 *
 * c_line: the line typed by the user
 *
 * This function will be called every time a message
 * is typed on the console. It runs the given line in
 * the current Guile environment.
 *
 * Return value: none
 */
extern void
lw6_cns_handler (char *c_line)
{
  SCM line;
  SCM func_symbol;
  SCM func;

  if (c_line)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("interpreting console input \"%s\""),
		  c_line);

      lw6cns_history_add_if_needed (c_line);
      line = scm_from_locale_string (c_line);

      func_symbol = scm_c_lookup ("lw6-console-try-catch");
      func = scm_variable_ref (func_symbol);
      scm_call_1 (func, line);
      free (c_line);		// exceptionnally, don't use LW6SYS_FREE
    }
  else
    {
      printf (_x_ ("(quit) or CTRL_C to quit"));
      printf ("\n");
    }
}
