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

#ifndef LIQUIDWAR6DYN_H
#define LIQUIDWAR6DYN_H

#include <ltdl.h>

#include "../sys/sys.h"

#define LW6DYN_GET_PEDIGREE_FUNC_FORMAT "mod_%s_get_pedigree"
#define LW6DYN_CREATE_BACKEND_FUNC_FORMAT "mod_%s_create_backend"
#define LW6DYN_IS_BACKEND_GPL_COMPATIBLE_SYM_FORMAT "mod_%s_is_GPL_compatible"
#define LW6DYN_IS_SHARED_GPL_COMPATIBLE_SYM_FORMAT "shared_%s_is_GPL_compatible"
#define LW6DYN_IS_BACKEND_DLCLOSE_SAFE_SYM_FORMAT "mod_%s_is_dlclose_safe"
#define LW6DYN_IS_SHARED_DLCLOSE_SAFE_SYM_FORMAT "shared_%s_is_dlclose_safe"

/**
 * Handle on dynamic library. Well, actually, ltdl does already
 * provide something like, so why use our own wrapper? It happens
 * storing the library path, that's to say what .so files it comes
 * from, is usefull for debugging, so this structure bundles those
 * two informations together.
 */
typedef struct lw6dyn_dl_handle_s
{
  /// Libtool handler.
  lt_dlhandle handle;
  /**
   * Path to .so file containing the code, or whatever file
   * is relevant on the current platform, the idea is to keep
   * track of where the library comes from.
   */
  char *library_path;
  /**
   * True (1) if the handle is a backend or false (0) if it's
   * just some shared code.
   */
  int is_backend;
  /**
   * True (1) if one can safely call dlclose on this backend.
   * Set to false (0) if low level dlclose must be skipped.
   * For some reason, some (external) libraries really do not
   * behave well when unloaded on the fly, even if we stop threads
   * using them and don't use them anymore. The workarround
   * is to have this flag defined, to skip the internal close
   * to dlclose. LW will still free the memory, but won't call
   * libtool dlclose for real. Libtool keeps track of this internally
   * and won't reload it on next call, maintain reference counts etc.
   * so there's no real harm. Except it just looks ugly not to be
   * able to truely unload the module.
   */
  int is_dlclose_safe;
}
lw6dyn_dl_handle_t;

/* dyn-dl.c */
extern lw6dyn_dl_handle_t *lw6dyn_dlopen_backend_so (lw6sys_context_t * sys_context, const char *so_file);
extern lw6dyn_dl_handle_t *lw6dyn_dlopen_shared_so (lw6sys_context_t * sys_context, const char *so_file);
extern lw6dyn_dl_handle_t *lw6dyn_dlopen_backend (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *top_level_lib,
						  const char *backend_name);
extern lw6dyn_dl_handle_t *lw6dyn_dlopen_shared (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *top_level_lib,
						 const char *shared_name);
extern int lw6dyn_dlclose_backend (lw6sys_context_t * sys_context, lw6dyn_dl_handle_t * handle);
extern int lw6dyn_dlclose_shared (lw6sys_context_t * sys_context, lw6dyn_dl_handle_t * handle);
extern void *lw6dyn_dlsym (lw6sys_context_t * sys_context, lw6dyn_dl_handle_t * handle, const char *func_name);

/* dyn-list.c */
extern lw6sys_assoc_t *lw6dyn_list_backends (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *top_level_lib);

/* dyn-path.c */
extern char *lw6dyn_path_find_backend (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *top_level_lib, const char *backend_name);
extern char *lw6dyn_path_find_shared (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *top_level_lib, const char *backend_name);

/* dyn-test.c */
extern int lw6dyn_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6dyn_test_run (lw6sys_context_t * sys_context, int mode);

#endif
