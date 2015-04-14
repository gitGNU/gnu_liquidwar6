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
 * In liquidwar6sys
 */

/*
 * In bazooka.c
 */
static SCM
_scm_lw6sys_set_memory_bazooka_size (SCM size)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_size;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_integer (size), size, SCM_ARG1, __FUNCTION__);

  c_size = scm_to_int (size);

  ret = lw6sys_set_memory_bazooka_size (sys_context, c_size) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_memory_bazooka_size ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_int (lw6sys_get_memory_bazooka_size (sys_context));

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_set_memory_bazooka_eraser (SCM state)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_state;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_bool (state), state, SCM_ARG1, __FUNCTION__);

  c_state = scm_to_bool (state);

  ret = lw6sys_set_memory_bazooka_eraser (sys_context, c_state) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_memory_bazooka_eraser ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = lw6sys_get_memory_bazooka_eraser (sys_context) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/*
 * build.c
 */
static SCM
_scm_lw6sys_build_get_package_tarname ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_package_tarname ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_package_name ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_package_name ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_package_string ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_package_string ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_package_id ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_package_id ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_version ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_version ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_codename ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_codename ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_version_base ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_version_base ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_version_major ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_version_major ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_version_minor ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_version_minor ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_stamp ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_stamp ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_md5sum ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_md5sum ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_copyright ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_copyright ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_license ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_license ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_home_url ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_home_url ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_bugs_url ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_bugs_url ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_configure_args ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_configure_args ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_gcc_version ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_gcc_version ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_cflags ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_cflags ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_ldflags ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_ldflags ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_hostname ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_hostname ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_date ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_date ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_time ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_time ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_host_cpu ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_host_cpu ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_endianness ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_endianness (sys_context));

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_pointer_size ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_int (lw6sys_build_get_pointer_size ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_is_x86 ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_int (lw6sys_build_is_x86 ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_is_amd64 ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_int (lw6sys_build_is_amd64 ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_host_os ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_host_os ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_is_gnu ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = lw6sys_build_is_gnu (sys_context) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_is_unix ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = lw6sys_build_is_unix (sys_context) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_is_ms_windows ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = lw6sys_build_is_ms_windows (sys_context) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_is_mac_os_x ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = lw6sys_build_is_mac_os_x (sys_context) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_is_gp2x ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = lw6sys_build_is_gp2x (sys_context) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_top_srcdir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_top_srcdir (sys_context));

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_abs_srcdir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_abs_srcdir ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_prefix ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_prefix ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_datadir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_datadir ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_libdir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_libdir ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_includedir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_includedir ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_localedir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_localedir ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_docdir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_docdir ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_console ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_console ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_gtk ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_gtk ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_mod_gl1 ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_mod_gl1 ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_mod_gles2 ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_mod_gles2 ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_mod_soft ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_mod_soft ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_mod_caca ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_mod_caca ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_mod_csound ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_mod_csound ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_mod_ogg ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_mod_ogg ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_mod_http ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_mod_http ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_openmp ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_openmp ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_optimize ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_optimize ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_allinone ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_allinone ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_fullstatic ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_fullstatic ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_paranoid ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_paranoid ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_gprof ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_gprof ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_instrument ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_instrument ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_profiler ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_profiler ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_gcov ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_gcov ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_enable_valgrind ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_locale_string (lw6sys_build_get_enable_valgrind ());

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_build_get_bin_id ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_int (lw6sys_build_get_bin_id (sys_context));

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/*
 * in debug.c
 */

static SCM
_scm_lw6sys_debug_get ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = lw6sys_debug_get (sys_context) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_debug_set (SCM mode)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_mode;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_bool (mode), mode, SCM_ARG1, __FUNCTION__);

  c_mode = scm_to_bool (mode);

  lw6sys_debug_set (sys_context, c_mode);

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

/*
 * In dump.c
 */

static SCM
_scm_lw6sys_dump (SCM content)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_content;
  char *user_dir = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (content), content, SCM_ARG1, __FUNCTION__);

  c_content = lw6scm_utils_to_0str (sys_context, content);
  if (c_content)
    {
      user_dir = lw6cfg_unified_get_user_dir (sys_context, lw6_global.argc, lw6_global.argv);
      if (user_dir)
	{
	  lw6sys_dump (sys_context, user_dir, c_content);
	  LW6SYS_FREE (sys_context, user_dir);
	}
      LW6SYS_FREE (sys_context, c_content);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6sys_dump_clear ()
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *user_dir = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  user_dir = lw6cfg_unified_get_user_dir (sys_context, lw6_global.argc, lw6_global.argv);
  if (user_dir)
    {
      lw6sys_dump_clear (sys_context, user_dir);
      LW6SYS_FREE (sys_context, user_dir);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

/*
 * In env.c
 */
static SCM
_scm_lw6sys_get_username ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_username (sys_context);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_hostname ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_hostname (sys_context);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_getenv (SCM key)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_key = NULL;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (key), key, SCM_ARG1, __FUNCTION__);

  c_key = lw6scm_utils_to_0str (sys_context, key);
  if (c_key)
    {
      buf = lw6sys_getenv (sys_context, c_key);
      if (buf)
	{
	  ret = scm_from_locale_string (buf);
	  LW6SYS_FREE (sys_context, buf);
	}
      LW6SYS_FREE (sys_context, c_key);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_getenv_prefixed (SCM keyword)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_keyword = NULL;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (keyword), keyword, SCM_ARG1, __FUNCTION__);

  c_keyword = lw6scm_utils_to_0str (sys_context, keyword);
  if (c_keyword)
    {
      buf = lw6sys_getenv_prefixed (sys_context, c_keyword);
      if (buf)
	{
	  ret = scm_from_locale_string (buf);
	  LW6SYS_FREE (sys_context, buf);
	}
      LW6SYS_FREE (sys_context, c_keyword);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/*
 * In id.c
 */
static SCM
_scm_lw6sys_generate_id_16 ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_ret = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_ret = lw6sys_id_ltoa (sys_context, lw6sys_generate_id_16 (sys_context));
  if (c_ret)
    {
      ret = scm_from_locale_string (c_ret);
      LW6SYS_FREE (sys_context, c_ret);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_generate_id_32 ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_ret = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_ret = lw6sys_id_ltoa (sys_context, lw6sys_generate_id_32 (sys_context));
  if (c_ret)
    {
      ret = scm_from_locale_string (c_ret);
      LW6SYS_FREE (sys_context, c_ret);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_generate_id_64 ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_ret = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_ret = lw6sys_id_ltoa (sys_context, lw6sys_generate_id_64 (sys_context));
  if (c_ret)
    {
      ret = scm_from_locale_string (c_ret);
      LW6SYS_FREE (sys_context, c_ret);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/*
 * In log.c
 */
static SCM
_scm_lw6sys_log (SCM level, SCM message)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_level;
  char *c_message;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_integer (level), level, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (message), message, SCM_ARG2, __FUNCTION__);

  c_level = scm_to_int (level);
  c_message = lw6scm_utils_to_0str (sys_context, message);
  if (c_message)
    {
      lw6sys_log (sys_context, c_level, __FILE__, __LINE__, "%s", c_message);
      LW6SYS_FREE (sys_context, c_message);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6sys_log_set_dialog_timeout (SCM dialog_timeout)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_dialog_timeout = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_integer (dialog_timeout), dialog_timeout, SCM_ARG1, __FUNCTION__);

  c_dialog_timeout = scm_to_int (dialog_timeout);
  ret = lw6sys_log_set_dialog_timeout (sys_context, c_dialog_timeout) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_log_get_level ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_int (lw6sys_log_get_level (sys_context));

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_log_set_level (SCM level)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_level;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_integer (level), level, SCM_ARG1, __FUNCTION__);

  c_level = scm_to_int (level);
  lw6sys_log_set_level (sys_context, c_level);

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6sys_log_get_backtrace_mode ()
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  SCM ret = SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_int (lw6sys_log_get_backtrace_mode (sys_context));

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_log_set_backtrace_mode (SCM backtrace_mode)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_backtrace_mode;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_integer (backtrace_mode), backtrace_mode, SCM_ARG1, __FUNCTION__);

  c_backtrace_mode = scm_to_int (backtrace_mode);
  lw6sys_log_set_backtrace_mode (sys_context, c_backtrace_mode);

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

/*
 * In mem.c
 */
static SCM
_scm_lw6sys_megabytes_available ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_int (lw6sys_megabytes_available (sys_context));

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/*
 * In openmp.c
 */
static SCM
_scm_lw6sys_openmp_get_num_procs ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_int (lw6sys_openmp_get_num_procs (sys_context));

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/*
 * In option.c
 */
static SCM
_scm_lw6sys_get_default_user_dir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_default_user_dir (sys_context);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_default_config_file ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_default_config_file (sys_context);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_default_log_file ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_default_log_file (sys_context);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_default_prefix ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_default_prefix (sys_context);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_default_mod_dir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_default_mod_dir (sys_context);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_default_data_dir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_default_data_dir (sys_context);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_default_music_dir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_default_music_dir (sys_context);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_default_music_path ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_default_music_path (sys_context);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_default_map_dir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_default_map_dir (sys_context);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_default_map_path ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_default_map_path (sys_context);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_default_script_file ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_default_script_file (sys_context);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_cwd ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_cwd (sys_context);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_run_dir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_run_dir (sys_context, lw6_global.argc, lw6_global.argv);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_user_dir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_user_dir (sys_context, lw6_global.argc, lw6_global.argv);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_config_file ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_config_file (sys_context, lw6_global.argc, lw6_global.argv);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_log_file ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_log_file (sys_context, lw6_global.argc, lw6_global.argv);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_prefix ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_prefix (sys_context, lw6_global.argc, lw6_global.argv);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_mod_dir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_mod_dir (sys_context, lw6_global.argc, lw6_global.argv);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_data_dir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_data_dir (sys_context, lw6_global.argc, lw6_global.argv);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_music_dir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_music_dir (sys_context, lw6_global.argc, lw6_global.argv);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_music_path ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_music_path (sys_context, lw6_global.argc, lw6_global.argv);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_map_dir ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_map_dir (sys_context, lw6_global.argc, lw6_global.argv);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_map_path ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_map_path (sys_context, lw6_global.argc, lw6_global.argv);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_script_file ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *buf = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  buf = lw6sys_get_script_file (sys_context, lw6_global.argc, lw6_global.argv);
  if (buf)
    {
      ret = scm_from_locale_string (buf);
      LW6SYS_FREE (sys_context, buf);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/*
 * In path.c
 */
static SCM
_scm_lw6sys_path_concat (SCM path1, SCM path2)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_path1 = NULL;
  char *c_path2 = NULL;
  char *c_ret = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (path1), path1, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (path2), path2, SCM_ARG2, __FUNCTION__);

  c_path1 = lw6scm_utils_to_0str (sys_context, path1);
  if (c_path1)
    {
      c_path2 = lw6scm_utils_to_0str (sys_context, path2);
      if (c_path2)
	{
	  c_ret = lw6sys_path_concat (sys_context, c_path1, c_path2);
	  if (c_ret)
	    {
	      ret = scm_from_locale_string (c_ret);
	      LW6SYS_FREE (sys_context, c_ret);
	    }
	  LW6SYS_FREE (sys_context, c_path2);
	}
      LW6SYS_FREE (sys_context, c_path1);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_path_file_only (SCM path)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_path = NULL;
  char *c_ret = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (path), path, SCM_ARG1, __FUNCTION__);

  c_path = lw6scm_utils_to_0str (sys_context, path);
  if (c_path)
    {
      c_ret = lw6sys_path_file_only (sys_context, c_path);
      if (c_ret)
	{
	  ret = scm_from_locale_string (c_ret);
	  LW6SYS_FREE (sys_context, c_ret);
	}
      LW6SYS_FREE (sys_context, c_path);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_path_parent (SCM path)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_path = NULL;
  char *c_ret = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (path), path, SCM_ARG1, __FUNCTION__);

  c_path = lw6scm_utils_to_0str (sys_context, path);
  if (c_path)
    {
      c_ret = lw6sys_path_parent (sys_context, c_path);
      if (c_ret)
	{
	  ret = scm_from_locale_string (c_ret);
	  LW6SYS_FREE (sys_context, c_ret);
	}
      LW6SYS_FREE (sys_context, c_path);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_path_split (SCM path)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_path = NULL;
  lw6sys_list_t *c_ret = NULL;
  lw6sys_list_t *tmp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (path), path, SCM_ARG1, __FUNCTION__);

  c_path = lw6scm_utils_to_0str (sys_context, path);
  if (c_path)
    {
      ret = SCM_LIST0;
      c_ret = lw6sys_path_split (sys_context, c_path);
      if (c_ret)
	{
	  for (tmp = c_ret; tmp != NULL; tmp = (lw6sys_list_t *) tmp->next_item)
	    {
	      if (tmp->data)
		{
		  ret = scm_cons (scm_from_locale_string ((char *) tmp->data), ret);
		}
	    }
	  lw6sys_list_free (sys_context, c_ret);
	  ret = scm_reverse (ret);
	}
      LW6SYS_FREE (sys_context, c_path);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/*
 * In signal.c
 */
static SCM
_scm_lw6sys_signal_custom (SCM trap_errors)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_trap_errors;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_BOOLP (trap_errors), trap_errors, SCM_ARG1, __FUNCTION__);

  c_trap_errors = SCM_NFALSEP (trap_errors);
  lw6sys_signal_custom (sys_context, c_trap_errors);

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6sys_signal_default ()
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  lw6sys_signal_default (sys_context);

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6sys_signal_send_quit ()
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  lw6sys_signal_send_quit (sys_context);

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6sys_signal_poll_quit ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = lw6sys_signal_poll_quit (sys_context) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/*
 * In time.c
 */
static SCM
_scm_lw6sys_get_timestamp ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_long_long (lw6sys_get_timestamp (sys_context));

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_uptime ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_long_long (lw6sys_get_uptime (sys_context));

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_get_cycle ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  ret = scm_from_int (lw6sys_get_cycle (sys_context));

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6sys_sleep (SCM seconds)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  float c_seconds = 0.0f;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_number (seconds), seconds, SCM_ARG1, __FUNCTION__);

  c_seconds = scm_to_double (seconds);

  lw6sys_sleep (sys_context, c_seconds);

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6sys_delay (SCM msec)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_msec = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_number (msec), msec, SCM_ARG1, __FUNCTION__);

  c_msec = scm_to_int (msec);

  lw6sys_delay (sys_context, c_msec);

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6sys_idle ()
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  lw6sys_idle (sys_context);

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6sys_snooze ()
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  lw6sys_snooze (sys_context);

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

/*
 * In url.c
 */
static SCM
_scm_lw6sys_url_canonize (SCM url)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  SCM ret = SCM_BOOL_F;
  char *c_url = NULL;
  char *c_ret = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (scm_is_string (url), url, SCM_ARG1, __FUNCTION__);

  c_url = lw6scm_utils_to_0str (sys_context, url);
  if (c_url)
    {
      c_ret = lw6sys_url_canonize (sys_context, c_url);
      if (c_ret)
	{
	  ret = scm_from_locale_string (c_ret);
	  LW6SYS_FREE (sys_context, c_ret);
	}
      LW6SYS_FREE (sys_context, c_url);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/**
 * lw6_register_funcs_sys
 *
 * @sys_context: global system context
 *
 * Register the functions of the sys module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_sys (lw6sys_context_t * sys_context)
{
  int ret = 1;

  /*
   * In liquidwar6sys
   */
  /*
   * In bazooka.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_SET_MEMORY_BAZOOKA_SIZE, 1, 0, 0, (SCM (*)())_scm_lw6sys_set_memory_bazooka_size);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_MEMORY_BAZOOKA_SIZE, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_memory_bazooka_size);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_SET_MEMORY_BAZOOKA_ERASER, 1, 0, 0, (SCM (*)())_scm_lw6sys_set_memory_bazooka_eraser);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_MEMORY_BAZOOKA_ERASER, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_memory_bazooka_eraser);
  /*
   * In build.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_PACKAGE_TARNAME, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_package_tarname);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_PACKAGE_NAME, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_package_name);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_PACKAGE_STRING, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_package_string);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_PACKAGE_ID, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_package_id);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_VERSION, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_version);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_CODENAME, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_codename);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_VERSION_BASE, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_version_base);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_VERSION_MAJOR, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_version_major);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_VERSION_MINOR, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_version_minor);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_STAMP, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_stamp);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_MD5SUM, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_md5sum);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_COPYRIGHT, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_copyright);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_LICENSE, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_license);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_HOME_URL, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_home_url);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_BUGS_URL, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_bugs_url);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_CONFIGURE_ARGS, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_configure_args);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_GCC_VERSION, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_gcc_version);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_CFLAGS, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_cflags);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_LDFLAGS, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_ldflags);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_HOSTNAME, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_hostname);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_DATE, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_date);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_TIME, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_time);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_HOST_CPU, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_host_cpu);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENDIANNESS, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_endianness);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_POINTER_SIZE, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_pointer_size);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_IS_X86, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_is_x86);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_IS_AMD64, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_is_amd64);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_HOST_OS, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_host_os);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_IS_GNU, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_is_gnu);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_IS_UNIX, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_is_unix);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_IS_MS_WINDOWS, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_is_ms_windows);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_IS_MAC_OS_X, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_is_mac_os_x);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_IS_GP2X, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_is_gp2x);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_TOP_SRCDIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_top_srcdir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ABS_SRCDIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_abs_srcdir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_PREFIX, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_prefix);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_DATADIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_datadir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_LIBDIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_libdir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_INCLUDEDIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_includedir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_LOCALEDIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_localedir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_DOCDIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_docdir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_CONSOLE, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_console);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_GTK, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_gtk);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_MOD_GL1, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_mod_gl1);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_MOD_GLES2, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_mod_gles2);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_MOD_SOFT, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_mod_soft);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_MOD_CACA, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_mod_caca);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_MOD_CSOUND, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_mod_csound);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_MOD_OGG, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_mod_ogg);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_MOD_HTTP, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_mod_http);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_OPENMP, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_openmp);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_OPTIMIZE, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_optimize);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_ALLINONE, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_allinone);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_FULLSTATIC, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_fullstatic);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_PARANOID, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_paranoid);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_GPROF, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_gprof);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_INSTRUMENT, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_instrument);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_PROFILER, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_profiler);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_GCOV, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_gcov);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_ENABLE_VALGRIND, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_enable_valgrind);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_BUILD_GET_BIN_ID, 0, 0, 0, (SCM (*)())_scm_lw6sys_build_get_bin_id);
  /*
   * In debug.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_DEBUG_GET, 0, 0, 0, (SCM (*)())_scm_lw6sys_debug_get);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_DEBUG_SET, 1, 0, 0, (SCM (*)())_scm_lw6sys_debug_set);
  /*
   * In dump.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_DUMP, 1, 0, 0, (SCM (*)())_scm_lw6sys_dump);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_DUMP_CLEAR, 0, 0, 0, (SCM (*)())_scm_lw6sys_dump_clear);
  /*
   * In env.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_USERNAME, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_username);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_HOSTNAME, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_hostname);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GETENV, 1, 0, 0, (SCM (*)())_scm_lw6sys_getenv);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GETENV_PREFIXED, 1, 0, 0, (SCM (*)())_scm_lw6sys_getenv_prefixed);

  /*
   * In id.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GENERATE_ID_16, 0, 0, 0, (SCM (*)())_scm_lw6sys_generate_id_16);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GENERATE_ID_32, 0, 0, 0, (SCM (*)())_scm_lw6sys_generate_id_32);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GENERATE_ID_64, 0, 0, 0, (SCM (*)())_scm_lw6sys_generate_id_64);

  /*
   * In log.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_LOG, 2, 0, 0, (SCM (*)())_scm_lw6sys_log);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_LOG_SET_DIALOG_TIMEOUT, 1, 0, 0, (SCM (*)())_scm_lw6sys_log_set_dialog_timeout);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_LOG_GET_LEVEL, 0, 0, 0, (SCM (*)())_scm_lw6sys_log_get_level);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_LOG_SET_LEVEL, 1, 0, 0, (SCM (*)())_scm_lw6sys_log_set_level);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_LOG_GET_BACKTRACE_MODE, 0, 0, 0, (SCM (*)())_scm_lw6sys_log_get_backtrace_mode);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_LOG_SET_BACKTRACE_MODE, 1, 0, 0, (SCM (*)())_scm_lw6sys_log_set_backtrace_mode);
  /*
   * in mem.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_MEGABYTES_AVAILABLE, 0, 0, 0, (SCM (*)())_scm_lw6sys_megabytes_available);
  /*
   * in openmp.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_OPENMP_GET_NUM_PROCS, 0, 0, 0, (SCM (*)())_scm_lw6sys_openmp_get_num_procs);
  /*
   * In options.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_DEFAULT_USER_DIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_default_user_dir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_DEFAULT_CONFIG_FILE, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_default_config_file);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_DEFAULT_LOG_FILE, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_default_log_file);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_DEFAULT_PREFIX, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_default_prefix);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_DEFAULT_MOD_DIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_default_mod_dir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_DEFAULT_DATA_DIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_default_data_dir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_DEFAULT_MUSIC_DIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_default_music_dir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_DEFAULT_MUSIC_PATH, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_default_music_path);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_DEFAULT_MAP_DIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_default_map_dir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_DEFAULT_MAP_PATH, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_default_map_path);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_DEFAULT_SCRIPT_FILE, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_default_script_file);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_CWD, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_cwd);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_RUN_DIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_run_dir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_USER_DIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_user_dir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_CONFIG_FILE, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_config_file);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_LOG_FILE, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_log_file);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_PREFIX, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_prefix);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_MOD_DIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_mod_dir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_DATA_DIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_data_dir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_MUSIC_DIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_music_dir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_MUSIC_PATH, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_music_path);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_MAP_DIR, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_map_dir);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_MAP_PATH, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_map_path);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_SCRIPT_FILE, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_script_file);
  /*
   * In path.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_PATH_CONCAT, 2, 0, 0, (SCM (*)())_scm_lw6sys_path_concat);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_PATH_FILE_ONLY, 1, 0, 0, (SCM (*)())_scm_lw6sys_path_file_only);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_PATH_PARENT, 1, 0, 0, (SCM (*)())_scm_lw6sys_path_parent);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_PATH_SPLIT, 1, 0, 0, (SCM (*)())_scm_lw6sys_path_split);
  /*
   * In signal.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_SIGNAL_CUSTOM, 1, 0, 0, (SCM (*)())_scm_lw6sys_signal_custom);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_SIGNAL_DEFAULT, 0, 0, 0, (SCM (*)())_scm_lw6sys_signal_default);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_SIGNAL_SEND_QUIT, 0, 0, 0, (SCM (*)())_scm_lw6sys_signal_send_quit);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_SIGNAL_POLL_QUIT, 0, 0, 0, (SCM (*)())_scm_lw6sys_signal_poll_quit);
  /*
   * In time.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_TIMESTAMP, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_timestamp);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_UPTIME, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_uptime);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_GET_CYCLE, 0, 0, 0, (SCM (*)())_scm_lw6sys_get_cycle);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_SLEEP, 1, 0, 0, (SCM (*)())_scm_lw6sys_sleep);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_DELAY, 1, 0, 0, (SCM (*)())_scm_lw6sys_delay);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_IDLE, 0, 0, 0, (SCM (*)())_scm_lw6sys_idle);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_SNOOZE, 0, 0, 0, (SCM (*)())_scm_lw6sys_snooze);
  /*
   * In url.c
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6SYS_URL_CANONIZE, 1, 0, 0, (SCM (*)())_scm_lw6sys_url_canonize);

  return ret;
}
