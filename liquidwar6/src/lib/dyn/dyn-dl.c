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

#include "dyn.h"
#include "dyn-internal.h"

static lt_dlhandle
_verbose_dlopen (lw6sys_context_t * sys_context, const char *file)
{
  lt_dlhandle ret = NULL;
  int nb_errs = 0;

  if ((nb_errs = lt_dlinit ()) > 0)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't initialize libltdl, %d errors"), nb_errs);
    }
  else
    {
      ret = lt_dlopen (file);
    }

  if (ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("load module \"%s\""), file);
    }
  else
    {
      if (lw6sys_file_exists (sys_context, file))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't open shared library \"%s\" lt_dlerror=\"%s\""), file, lt_dlerror ());
	}
      else
	{
	  /*
	   * File doesn't exist, different error message, and not
	   * as a warning, only an information.
	   */
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("shared library \"%s\" doesn't exist"), file);
	}
    }

  return ret;
}

static lw6dyn_dl_handle_t *
_dlopen_so (lw6sys_context_t * sys_context, const char *so_file, int is_backend)
{
  lw6dyn_dl_handle_t *ret = NULL;

  if (lw6sys_file_exists (sys_context, so_file))
    {
      /*
       * We really rely on calloc, for instance,
       * is_dlclose_safe must be 0 at init
       */
      ret = LW6SYS_CALLOC (sys_context, sizeof (lw6dyn_dl_handle_t));
      if (ret)
	{
	  ret->is_backend = is_backend;
	  ret->library_path = lw6sys_str_copy (sys_context, so_file);
	  if (ret->library_path)
	    {
	      ret->handle = _verbose_dlopen (sys_context, ret->library_path);
	      if (!ret->handle)
		{
		  LW6SYS_FREE (sys_context, ret->library_path);
		  LW6SYS_FREE (sys_context, ret);
		  ret = NULL;
		}
	    }
	  else
	    {
	      LW6SYS_FREE (sys_context, ret);
	      ret = NULL;
	    }
	}
    }

  return ret;
}

/**
 * lw6dyn_dlopen_backend_so:
 *
 * @sys_context: global system context
 * @backend_so: the .so file to open
 *
 * Opens a .so file directly, using a valid (full) path name.
 *
 * Return value: a handle to the module, once it's opened. You might still
 *   need to call a module specific @init function, but it's another story.
 */
lw6dyn_dl_handle_t *
lw6dyn_dlopen_backend_so (lw6sys_context_t * sys_context, const char *so_file)
{
  return _dlopen_so (sys_context, so_file, 1);
}

/**
 * lw6dyn_dlopen_shared_so:
 *
 * @sys_context: global system context
 * @shared_so: the .so file to open
 *
 * Opens a .so file directly, using a valid (full) path name.
 *
 * Return value: a handle to the shared code, once it's opened. You might still
 *   need to call a module specific @init function, but it's another story.
 */
lw6dyn_dl_handle_t *
lw6dyn_dlopen_shared_so (lw6sys_context_t * sys_context, const char *so_file)
{
  return _dlopen_so (sys_context, so_file, 0);
}

static int
_sym_exists (lw6sys_context_t * sys_context, lw6dyn_dl_handle_t * handle, const char *sym_format, const char *backend_name)
{
  int ret = 0;
  char *sym_str = NULL;

  sym_str = lw6sys_new_sprintf (sys_context, sym_format, backend_name);
  if (sym_str)
    {
      if (lw6dyn_dlsym (sys_context, handle, sym_str))
	{
	  ret = 1;
	}
      LW6SYS_FREE (sys_context, sym_str);
      sym_str = NULL;
    }

  return ret;
}

/**
 * lw6dyn_dlopen_backend:
 *
 * @sys_context: global system context
 * @argc: the number of command-line arguments as passed to @main
 * @arvg: an array of command-line arguments as passed to @main
 * @top_level_lib: the top-level library concerned, this means is it
 *   "cli", "gfx", "snd" or "srv". This will tell the function to search
 *   for the .so file in the correct subdirectory. Think of this as a
 *   category.
 * @backend: the actual name of the backend, this is the name of the
 *   .so file, between "libmod_" and ".so". For instance, to find
 *   "libmod_gl.so", the right argument is "gl1".
 *
 * Opens a .so file corresponding to the given backend,
 * it is capable to search for system libraries installed after "make install"
 * but if not found, it will also search in the current build directory,
 * finding the .so files in hidden .libs subdirectories.
 *
 * Return value: a handle to the module, once it's opened. You might still
 *   need to call a module specific @init function, but it's another story.
 */
lw6dyn_dl_handle_t *
lw6dyn_dlopen_backend (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *top_level_lib, const char *backend_name)
{
  lw6dyn_dl_handle_t *ret = NULL;
  char *so_file = NULL;
  char *get_pedigree_func_str = NULL;
  lw6sys_module_pedigree_t *(*get_pedigree_func) (lw6sys_context_t *) = NULL;
  lw6sys_module_pedigree_t *module_pedigree = NULL;
  char *create_backend_func_str = NULL;
  void *(*create_backend_func) (lw6sys_context_t *) = NULL;
  int ok = 0;
  static int first_load = 1;

  so_file = lw6dyn_path_find_backend (sys_context, argc, argv, top_level_lib, backend_name);
  if (so_file)
    {
      ret = lw6dyn_dlopen_backend_so (sys_context, so_file);

      if (ret)
	{
	  get_pedigree_func_str = lw6sys_new_sprintf (sys_context, LW6DYN_GET_PEDIGREE_FUNC_FORMAT, backend_name);
	  if (get_pedigree_func_str)
	    {
	      get_pedigree_func = lw6dyn_dlsym (sys_context, ret, get_pedigree_func_str);
	      if (get_pedigree_func)
		{
		  module_pedigree = (lw6sys_module_pedigree_t *) (get_pedigree_func) (sys_context);
		  if (module_pedigree)
		    {
		      if (module_pedigree->id
			  && module_pedigree->category
			  && module_pedigree->name
			  && module_pedigree->readme
			  && module_pedigree->version
			  && module_pedigree->copyright && module_pedigree->license && module_pedigree->date && module_pedigree->time)
			{
			  create_backend_func_str = lw6sys_new_sprintf (sys_context, LW6DYN_CREATE_BACKEND_FUNC_FORMAT, backend_name);
			  if (create_backend_func_str)
			    {
			      create_backend_func = lw6dyn_dlsym (sys_context, ret, create_backend_func_str);
			      if (create_backend_func)
				{
				  if (!strcmp (backend_name, module_pedigree->id))
				    {
				      if (!strcmp (top_level_lib, module_pedigree->category))
					{
					  if (lw6sys_version_is_compatible (sys_context, lw6sys_build_get_version (), module_pedigree->version))
					    {
					      if (_sym_exists (sys_context, ret, LW6DYN_IS_BACKEND_GPL_COMPATIBLE_SYM_FORMAT, backend_name))
						{
						  if (first_load)
						    {
						      /*
						       * the first time we
						       * load a .so file,
						       * log it to console
						       * just to make sure
						       * paths are right.
						       */
						      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _("loaded module \"%s\""), so_file);
						      first_load = 0;
						    }

						  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("module \"%s\" loaded, looks fine"), so_file);

						  ret->is_dlclose_safe =
						    _sym_exists (sys_context, ret, LW6DYN_IS_BACKEND_DLCLOSE_SAFE_SYM_FORMAT, backend_name);
						  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("is_dlclose_safe for \"%s\" is %d"), so_file,
							      ret->is_dlclose_safe);

						  /*
						   * Verbose dlopen did log the so name
						   */
						  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("id for \"%s\" is \"%s\""), so_file, module_pedigree->id);
						  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("category for \"%s\" is \"%s\""), so_file,
							      module_pedigree->category);
						  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("name for \"%s\" is \"%s\""), so_file, module_pedigree->name);
						  lw6sys_log (sys_context,
							      LW6SYS_LOG_INFO, _x_ ("readme for \"%s\" is \"%s\""), so_file, module_pedigree->readme);
						  lw6sys_log (sys_context,
							      LW6SYS_LOG_INFO, _x_ ("version for \"%s\" is \"%s\""), so_file, module_pedigree->version);
						  lw6sys_log (sys_context,
							      LW6SYS_LOG_INFO, _x_ ("copyright for \"%s\" is \"%s\""), so_file, module_pedigree->copyright);
						  lw6sys_log (sys_context,
							      LW6SYS_LOG_INFO, _x_ ("license for \"%s\" is \"%s\""), so_file, module_pedigree->license);
						  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("date for \"%s\" is \"%s\""), so_file, module_pedigree->date);
						  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("time for \"%s\" is \"%s\""), so_file, module_pedigree->time);
						  ok = 1;
						}
					      else
						{
						  lw6sys_log
						    (sys_context, LW6SYS_LOG_WARNING,
						     _x_
						     ("module mod_%s \"%s\" in \"%s\" is not GPL compatible"), backend_name, module_pedigree->name, so_file);
						}
					    }
					  else
					    {
					      lw6sys_log (sys_context,
							  LW6SYS_LOG_WARNING,
							  _x_
							  ("bad version for module mod_%s \"%s\" in \"%s\", module version is \"%s\" but main program version is \"%s\""),
							  backend_name, module_pedigree->name, so_file, module_pedigree->version, lw6sys_build_get_version ());
					    }
					}
				      else
					{
					  lw6sys_log (sys_context,
						      LW6SYS_LOG_WARNING,
						      _x_
						      ("bad category for module mod_%s \"%s\" in \"%s\", module category is \"%s\" but main program requested \"%s\""),
						      backend_name, module_pedigree->name, so_file, module_pedigree->category, top_level_lib);
					}
				    }
				  else
				    {
				      lw6sys_log (sys_context,
						  LW6SYS_LOG_WARNING,
						  _x_
						  ("bad id for module mod_%s \"%s\" in \"%s\", module id is \"%s\""),
						  backend_name, module_pedigree->name, so_file, module_pedigree->id);
				    }
				}
			      LW6SYS_FREE (sys_context, create_backend_func_str);
			    }
			}
		      else
			{
			  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad pedigree for mod_%s in \"%s\""), backend_name, so_file);
			}
		      LW6SYS_FREE (sys_context, module_pedigree);
		    }
		  else
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't get pedigree for mod_%s in \"%s\""), backend_name, so_file);
		    }
		}
	      LW6SYS_FREE (sys_context, get_pedigree_func_str);
	    }
	}
      LW6SYS_FREE (sys_context, so_file);
    }

  if (!ok)
    {
      if (ret)
	{
	  lw6dyn_dlclose_backend (sys_context, ret);
	  ret = NULL;
	}
    }

  return ret;
}

/**
 * lw6dyn_dlopen_shared:
 *
 * @sys_context: global system context
 * @argc: the number of command-line arguments as passed to @main
 * @arvg: an array of command-line arguments as passed to @main
 * @top_level_lib: the top-level library concerned, this means is it
 *   "cli", "gfx", "snd" or "srv". This will tell the function to search
 *   for the .so file in the correct subdirectory. Think of this as a
 *   category.
 * @shared: the actual name of the shared, this is the name of the
 *   .so file, between "libmod_" and ".so". For instance, to find
 *   "libmod_gl.so", the right argument is "gl1".
 *
 * Opens a .so file corresponding to the given shared code,
 * it is capable to search for system libraries installed after "make install"
 * but if not found, it will also search in the current build directory,
 * finding the .so files in hidden .libs subdirectories.
 *
 * Return value: a handle to the shared code, once it's opened. This is
 *   different from a real module, there's no real prototype, it just loads code.
 */
lw6dyn_dl_handle_t *
lw6dyn_dlopen_shared (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *top_level_lib, const char *shared_name)
{
  lw6dyn_dl_handle_t *ret = NULL;
  char *so_file = NULL;
  int ok = 0;

  so_file = lw6dyn_path_find_shared (sys_context, argc, argv, top_level_lib, shared_name);
  if (so_file)
    {
      ret = lw6dyn_dlopen_shared_so (sys_context, so_file);

      if (ret)
	{
	  ret->is_backend = 0;

	  if (_sym_exists (sys_context, ret, LW6DYN_IS_SHARED_GPL_COMPATIBLE_SYM_FORMAT, shared_name))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("shared code \"%s\" loaded, looks fine"), so_file);
	      ret->is_dlclose_safe = _sym_exists (sys_context, ret, LW6DYN_IS_SHARED_DLCLOSE_SAFE_SYM_FORMAT, shared_name);
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("is_dlclose_safe for \"%s\" is %d"), so_file, ret->is_dlclose_safe);
	      ok = 1;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("shared code shared_%s in \"%s\" is not GPL compatible"), shared_name, so_file);
	    }
	}
      LW6SYS_FREE (sys_context, so_file);
    }

  if (!ok)
    {
      if (ret)
	{
	  lw6dyn_dlclose_shared (sys_context, ret);
	  ret = NULL;
	}
    }

  return ret;
}

static int
_dlclose (lw6sys_context_t * sys_context, lw6dyn_dl_handle_t * handle)
{
  int nb_errs;
  int ret = 0;

  if (handle)
    {
      if (handle->is_dlclose_safe)
	{
	  /*
	   * Just for this log line, it's interesting to wrap
	   * the libtool lt_dlclose in our own function: when freeing
	   * a loaded shared library, we know *what* we're freeing,
	   * which can be pretty usefull when debugging.
	   */
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("unload \"%s\""), handle->library_path);
	  if (handle->handle)
	    {
	      if (!lt_dlclose (handle->handle))
		{
		  ret = 1;
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't unload shared library"));
		}
	      handle->handle = NULL;
	    }

	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("lt_dlclose done for \"%s\""), handle->library_path);

	  if ((nb_errs = lt_dlexit ()) > 0)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't exit libltdl, %d errors"), nb_errs);
	      ret = 0;
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("unload \"%s\" skipped, marked as not \"dlclose safe\""), handle->library_path);
	}
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("lt_dlexit done for \"%s\""), handle->library_path);

      LW6SYS_FREE (sys_context, handle->library_path);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL dl_handle"));
    }

  LW6SYS_FREE (sys_context, handle);

  return ret;
}

/**
 * lw6dyn_dlclose_backend:
 *
 * @sys_context: global system context
 * @handle: the backend to close.
 *
 * Closes an opened backend. Note that you must call any backend
 * specific clear, destroy, deinit, exit, function before.
 *
 * Return value: 1 if success, 0 on error.
 */
int
lw6dyn_dlclose_backend (lw6sys_context_t * sys_context, lw6dyn_dl_handle_t * handle)
{
  if (!(handle->is_backend))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("dlclosing \"%s\" as a backend but it's not one"), handle->library_path);
    }

  return _dlclose (sys_context, handle);
}

/**
 * lw6dyn_dlclose_shared:
 *
 * @sys_context: global system context
 * @handle: the shared code library to close.
 *
 * Closes an opened shared code library. Note that you must call any shared code library
 * specific clear, destroy, deinit, exit, function before.
 *
 * Return value: 1 if success, 0 on error.
 */
int
lw6dyn_dlclose_shared (lw6sys_context_t * sys_context, lw6dyn_dl_handle_t * handle)
{
  if (handle->is_backend)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("dlclosing \"%s\" as shared code but it's a backend"), handle->library_path);
    }

  return _dlclose (sys_context, handle);
}

/**
 * lw6dyn_dlsym:
 *
 * @sys_context: global system context
 * @handle: the backend concerned
 * @func_name: the function name, as a NULL terminated string
 *
 * Finds a C function in the given backend.
 *
 * Return value: a pointer to the function, NULL if not found.
 */
void *
lw6dyn_dlsym (lw6sys_context_t * sys_context, lw6dyn_dl_handle_t * handle, const char *func_name)
{
  void *ret;

  ret = lt_dlsym (handle->handle, func_name);
  if (ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("found symbol \"%s\" in library \"%s\""), func_name, handle->library_path);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("unable to find symbol \"%s\" in library \"%s\""), func_name, handle->library_path);
    }

  return ret;
}
