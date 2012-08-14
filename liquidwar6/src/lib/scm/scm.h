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

#ifndef LIQUIDWAR6SCM_H
#define LIQUIDWAR6SCM_H

#include "../sys/sys.h"
#include "../hlp/hlp.h"

#include <libguile.h>

typedef SCM (*lw6scm_func_t) ();
typedef void *(*lw6scm_callback_t) (void *);

/* scm-funcname.c */
extern char *lw6scm_funcname_scm2c (const char *funcname);
extern char *lw6scm_funcname_c2scm (const char *funcname);

/* scm-coverage.c */
extern lw6sys_hash_t *lw6scm_coverage_new (lw6sys_list_t * funcs);
extern void lw6scm_coverage_call (lw6sys_hash_t * coverage, const char *func);
extern void lw6scm_coverage_log (lw6sys_hash_t * coverage);
extern int lw6scm_coverage_check (int *percent, lw6sys_hash_t * coverage,
				  lw6sys_list_t * funcs);
/* scm-utils.c */
extern char *lw6scm_utils_to_0str (SCM string);
extern SCM lw6scm_utils_to_scm_str_list (lw6sys_list_t * c_list);
extern SCM lw6scm_utils_to_scm_str_assoc (lw6sys_assoc_t * c_assoc);
extern lw6sys_list_t *lw6scm_utils_to_sys_str_list (SCM list);
extern lw6sys_assoc_t *lw6scm_utils_to_sys_str_assoc (SCM assoc);

/* scm-wrapper.c */
extern int lw6scm_c_define_gsubr (const char *name, int req, int opt, int rst,
				  lw6scm_func_t fcn);
extern int lw6scm_c_primitive_load (const char *filename);
extern void *lw6scm_with_guile (lw6scm_callback_t func, void *data);

/* scm-test.c */
extern int lw6scm_test (int mode);

#endif
