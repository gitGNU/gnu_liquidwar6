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

#include "../sys/sys.h"
#include "dyn.h"
#include "dyn-internal.h"

const char *_LW6DYN_DEVEL_DEPTH_STRINGS[_LW6DYN_DEVEL_NB_DEPTHS] =
  { "", "../", "../../", "../../../" };

#define BACKEND_DEVEL_PATH_FORMAT "%ssrc/lib/%s/mod-%s/.libs/libmod_%s-%s." _LW6DYN_SUFFIX
#define BACKEND_SYSTEM_PATH_FORMAT "%s/%s/libmod_%s-%s." _LW6DYN_SUFFIX

static char *
make_devel_backend_path (const char *top_level_lib, const char *backend_name,
			 int depth)
{
  char *ret = NULL;

  if (depth >= 0 && depth < _LW6DYN_DEVEL_NB_DEPTHS)
    {
      ret =
	lw6sys_new_sprintf (BACKEND_DEVEL_PATH_FORMAT,
			    _LW6DYN_DEVEL_DEPTH_STRINGS[depth], top_level_lib,
			    backend_name, backend_name,
			    lw6sys_build_get_version ());
    }

  return ret;
}

static char *
make_system_backend_path (int argc, const char *argv[],
			  const char *top_level_lib, const char *backend_name)
{
  char *ret = NULL;
  char *mod_dir = NULL;

  mod_dir = lw6sys_get_mod_dir (argc, argv);
  if (mod_dir)
    {
      ret =
	lw6sys_new_sprintf (BACKEND_SYSTEM_PATH_FORMAT, mod_dir,
			    top_level_lib, backend_name,
			    lw6sys_build_get_version ());
      LW6SYS_FREE (mod_dir);
    }

  return ret;
}

/**
 * lw6dyn_path_find_backend:
 *
 * @argc: the number of command-line arguments as passed to @main
 * @arvg: an array of command-line arguments as passed to @main
 * @top_level_lib: the top-level library concerned, this means is it
 *   "cli", "gfx", "snd" or "srv". This will tell the function to search
 *   for the .so file in the correct subdirectory. Think of this as a
 *   category.
 * @backend_name: the actual name of the backend, this is the name of the
 *   .so file, between "libmod_" and ".so". For instance, to find
 *   "libmod_gl.so", the right argument is "gl1".
 *
 * Get the full path to a .so file corresponding to the given backend,
 * it is capable to search for system libraries installed after "make install"
 * but if not found, it will also search in the current build directory,
 * finding the .so files in hidden .libs subdirectories.
 *
 * Return value: the full path of the .so file, needs to be freed.
 */
char *
lw6dyn_path_find_backend (int argc, const char *argv[],
			  const char *top_level_lib, const char *backend_name)
{
  char *ret = NULL;
  char *system_backend_path = NULL;
  char *devel_backend_path = NULL;
  int depth;

  if (!ret)
    {
      system_backend_path =
	make_system_backend_path (argc, argv, top_level_lib, backend_name);
      if (system_backend_path)
	{
	  if (lw6sys_file_exists (system_backend_path))
	    {
	      ret = lw6sys_str_copy (system_backend_path);
	    }
	  else
	    {
	      for (depth = 0; depth < _LW6DYN_DEVEL_NB_DEPTHS && !ret;
		   ++depth)
		{
		  if (!ret)
		    {
		      devel_backend_path =
			make_devel_backend_path (top_level_lib, backend_name,
						 depth);
		      if (devel_backend_path)
			{
			  if (lw6sys_file_exists (devel_backend_path))
			    {
			      ret = lw6sys_str_copy (devel_backend_path);
			    }
			  LW6SYS_FREE (devel_backend_path);
			}
		    }
		}
	    }

	  if (!ret)
	    {
	      // module not found at all
	      lw6sys_log (LW6SYS_LOG_NOTICE,
			  _x_ ("couldn't find backend %s/%s in %s"),
			  top_level_lib, backend_name, system_backend_path);
	    }
	  LW6SYS_FREE (system_backend_path);
	}
    }

  return ret;
}
