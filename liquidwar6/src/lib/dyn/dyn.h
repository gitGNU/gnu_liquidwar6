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

#ifndef LIQUIDWAR6DYN_H
#define LIQUIDWAR6DYN_H

#include <ltdl.h>

#include "../sys/sys.h"

#define LW6DYN_GET_PEDIGREE_FUNC_FORMAT "mod_%s_get_pedigree"
#define LW6DYN_CREATE_BACKEND_FUNC_FORMAT "mod_%s_create_backend"
#define LW6DYN_IS_GPL_COMPATIBLE_SYM_FORMAT "mod_%s_is_GPL_compatible"

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
}
lw6dyn_dl_handle_t;

/* dyn-dl.c */
extern lw6dyn_dl_handle_t *lw6dyn_dlopen_backend_so (const char *so_file);
extern lw6dyn_dl_handle_t *lw6dyn_dlopen_backend (int argc,
						  const char *argv[],
						  const char *top_level_lib,
						  const char *backend_name);
extern int lw6dyn_dlclose_backend (lw6dyn_dl_handle_t * handle);
extern void *lw6dyn_dlsym (lw6dyn_dl_handle_t * handle,
			   const char *func_name);

/* dyn-list.c */
extern lw6sys_assoc_t *lw6dyn_list_backends (int argc, const char *argv[],
					     const char *top_level_lib);

/* dyn-path.c */
extern char *lw6dyn_path_find_backend (int argc, const char *argv[],
				       const char *top_level_lib,
				       const char *backend_name);

/* dyn-test.c */
extern int lw6dyn_test (int mode);

#endif
