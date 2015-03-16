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
#endif

#include "../sys/sys.h"
#include "dyn.h"
#include "dyn-internal.h"

const char *_LW6DYN_DEVEL_DEPTH_STRINGS[_LW6DYN_DEVEL_NB_DEPTHS] = { "", "../", "../../", "../../../" };

/*
 * Path used to find loadable modules, this is normalize for
 * readability, the two variants, one with src/ and the other
 * without it, are used, respectively, for development and
 * real execution contexts.
 */
#define _BACKEND_DEVEL_PATH_FORMAT "%ssrc/lib/%s/mod-%s/.libs/libmod_%s-%s." _LW6DYN_SUFFIX
#define _BACKEND_SYSTEM_PATH_FORMAT "%s/%s/libmod_%s-%s." _LW6DYN_SUFFIX

/*
 * Path for shared code for modules differ from standard module
 * paths, to avoid loading them by mistake when listing dirs.
 */
#define _SHARED_DEVEL_PATH_FORMAT "%ssrc/lib/%s/shared-%s/.libs/libshared_%s-%s." _LW6DYN_SUFFIX
#define _SHARED_SYSTEM_PATH_FORMAT "%s/%s/libshared_%s-%s." _LW6DYN_SUFFIX

#define _SHARED_NEEDLE "shared"

static char *
_make_devel_path (lw6sys_context_t * sys_context, const char *top_level_lib, const char *backend_name, int depth, const char *devel_path_format)
{
  char *ret = NULL;

  if (depth >= 0 && depth < _LW6DYN_DEVEL_NB_DEPTHS)
    {
      ret =
	lw6sys_new_sprintf (sys_context, devel_path_format,
			    _LW6DYN_DEVEL_DEPTH_STRINGS[depth], top_level_lib, backend_name, backend_name, lw6sys_build_get_version ());
    }

  return ret;
}

static char *
_make_system_path (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *top_level_lib, const char *backend_name,
		   const char *system_path_format)
{
  char *ret = NULL;
  char *mod_dir = NULL;

  mod_dir = lw6sys_get_mod_dir (sys_context, argc, argv);
  if (mod_dir)
    {
      ret = lw6sys_new_sprintf (sys_context, system_path_format, mod_dir, top_level_lib, backend_name, lw6sys_build_get_version ());
      LW6SYS_FREE (sys_context, mod_dir);
    }

  return ret;
}

static char *
_path_find (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *top_level_lib, const char *backend_name, const char *devel_path_format,
	    const char *system_path_format)
{
  char *ret = NULL;
  char *system_path = NULL;
  char *devel_path = NULL;
  int depth;

  if (!ret)
    {
      system_path = _make_system_path (sys_context, argc, argv, top_level_lib, backend_name, system_path_format);
      if (system_path)
	{
	  if (lw6sys_file_exists (sys_context, system_path))
	    {
	      ret = lw6sys_str_copy (sys_context, system_path);
	    }
	  else
	    {
	      for (depth = 0; depth < _LW6DYN_DEVEL_NB_DEPTHS && !ret; ++depth)
		{
		  if (!ret)
		    {
		      devel_path = _make_devel_path (sys_context, top_level_lib, backend_name, depth, devel_path_format);
		      if (devel_path)
			{
			  if (lw6sys_file_exists (sys_context, devel_path))
			    {
			      ret = lw6sys_str_copy (sys_context, devel_path);
			    }
			  LW6SYS_FREE (sys_context, devel_path);
			}
		    }
		}
	    }

	  // module not found at all
	  /*
	   * We distinguish the case of a module or a shared code
	   * bundle to display more explicit messages, this is a NOTICE
	   * so it can be seen quite often and this kind of problem
	   * requires very good feedback for it's tricky to fix.
	   */
	  if (strstr (system_path_format, _SHARED_NEEDLE))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("couldn't find shared code for %s/%s in %s"), top_level_lib, backend_name, system_path);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("couldn't find backend for %s/%s in %s"), top_level_lib, backend_name, system_path);
	    }

	  LW6SYS_FREE (sys_context, system_path);
	}
    }

  return ret;
}

/**
 * lw6dyn_path_find_backend:
 *
 * @sys_context: global system context
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
lw6dyn_path_find_backend (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *top_level_lib, const char *backend_name)
{
  char *ret = NULL;

  ret = _path_find (sys_context, argc, argv, top_level_lib, backend_name, _BACKEND_DEVEL_PATH_FORMAT, _BACKEND_SYSTEM_PATH_FORMAT);

  return ret;
}

/**
 * lw6dyn_path_find_shared:
 *
 * @sys_context: global system context
 * @argc: the number of command-line arguments as passed to @main
 * @arvg: an array of command-line arguments as passed to @main
 * @top_level_lib: the top-level library concerned, this means is it
 *   "cli", "gfx", "snd" or "srv". This will tell the function to search
 *   for the .so file in the correct subdirectory. Think of this as a
 *   category.
 * @shared_name: the actual name of the shared code, this is the name of the
 *   .so file, between "libshared_" and ".so". For instance, to find
 *   "libshared_sdl.so", the right argument is "sdl".
 *
 * Get the full path to a .so file corresponding to the given shared code entry,
 * it is capable to search for system libraries installed after "make install"
 * but if not found, it will also search in the current build directory,
 * finding the .so files in hidden .libs subdirectories.
 * This is different from the standard module loader, since it will search
 * for .so files with a slightly different name. The idea is to distinguish
 * modules that are truely loadable and shared code that can't be used
 * standalone and can't either be stuffed in the main binary since it refers
 * to external dynamic library which will only be loaded at runtime.
 *
 * Return value: the full path of the .so file, needs to be freed.
 */
char *
lw6dyn_path_find_shared (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *top_level_lib, const char *shared_name)
{
  char *ret = NULL;

  ret = _path_find (sys_context, argc, argv, top_level_lib, shared_name, _SHARED_DEVEL_PATH_FORMAT, _SHARED_SYSTEM_PATH_FORMAT);

  return ret;
}
