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

#include "gfx.h"
#ifdef LW6_ALLINONE
#ifdef MOD_GL1
#include "mod-gl1/mod-gl1.h"
#else // MOD_GL1
#ifdef MOD_GLES2
#include "mod-gles2/mod-gles2.h"
#else // MOD_GLES2
#ifdef MOD_SOFT
#include "mod-soft/mod-soft.h"
#else // MOD_SOFT
#ifdef MOD_CACA
#include "mod-caca/mod-caca.h"
#endif // MOD_CACA
#endif // MOD_SOFT
#endif // MOD_GLES2
#endif // MOD_GL1
#endif // LW6_ALLINONE

/**
 * lw6gfx_get_backends
 *
 * @sys_context: global system context
 * @argc: argc, as passed to @main
 * @argv: argv, as passed to @main
 *
 * List available gfx backends. The hash contains pairs with id and name
 * for each backend. The id is the technical key you can use to
 * load the backend, the name is something more readable you can display
 * in an interface. The backend objects themselves are not instanciated
 * by this (in fact, they are, but released on the fly) you need to
 * load and initialize them afterwards.
 *
 * Return value: hash containing id/name pairs.
 */
lw6sys_assoc_t *
lw6gfx_get_backends (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  lw6sys_assoc_t *ret = NULL;

#ifdef LW6_ALLINONE
  lw6sys_module_pedigree_t *module_pedigree = NULL;

  ret = lw6sys_assoc_new (sys_context, lw6sys_free_callback);
  if (ret)
    {
#ifdef MOD_GL1
      module_pedigree = mod_gl1_get_pedigree (sys_context);
      if (module_pedigree)
	{
	  lw6sys_assoc_set (sys_context, &ret, module_pedigree->id, lw6sys_str_copy (sys_context, module_pedigree->name));
	  LW6SYS_FREE (sys_context, module_pedigree);
	}
#else // MOD_GL1
#ifdef MOD_GLES2
      module_pedigree = mod_gles2_get_pedigree (sys_context);
      if (module_pedigree)
	{
	  lw6sys_assoc_set (sys_context, &ret, module_pedigree->id, lw6sys_str_copy (sys_context, module_pedigree->name));
	  LW6SYS_FREE (sys_context, module_pedigree);
	}

#else // MOD_GLES2
#ifdef MOD_SOFT
      module_pedigree = mod_soft_get_pedigree (sys_context);
      if (module_pedigree)
	{
	  lw6sys_assoc_set (sys_context, &ret, module_pedigree->id, lw6sys_str_copy (sys_context, module_pedigree->name));
	  LW6SYS_FREE (sys_context, module_pedigree);
	}
#else // MOD_SOFT
#ifdef MOD_CACA
      module_pedigree = mod_caca_get_pedigree (sys_context);
      if (module_pedigree)
	{
	  lw6sys_assoc_set (sys_context, &ret, module_pedigree->id, lw6sys_str_copy (sys_context, module_pedigree->name));
	  LW6SYS_FREE (sys_context, module_pedigree);
	}
#endif // MOD_CACA
#endif // MOD_SOFT
#endif // MOD_GLES2
#endif // MOD_GL1
    }
#else // LW6_ALLINONE
  ret = lw6dyn_list_backends (sys_context, argc, argv, "gfx");
#endif // LW6_ALLINONE

  return ret;
}

/**
 * lw6gfx_create_backend
 *
 * @sys_context: global system context
 * @argc: argc, as passed to @main
 * @argv: argv, as passed to @main
 * @name: string containing gfx key, typically got from @lw6gfx_get_backends
 *
 * Creates a gfx backend, this is just about loading the dynamic
 * library if needed, and/or check gfx engine is available, and
 * connect to it. It does not perform initialization.
 *
 * Return value: gfx backend.
 */
lw6gfx_backend_t *
lw6gfx_create_backend (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *name)
{
  lw6gfx_backend_t *backend = NULL;
#ifdef LW6_ALLINONE
#ifdef MOD_GL1
  if (name && !strcmp (name, "gl1"))
    {
      backend = mod_gl1_create_backend (sys_context);
    }
#else // MOD_GL1
#ifdef MOD_GLES2
  if (name && !strcmp (name, "gles2"))
    {
      backend = mod_gles2_create_backend (sys_context);
    }
#else // MOD_GLES2
#ifdef MOD_SOFT
  if (name && !strcmp (name, "soft"))
    {
      backend = mod_soft_create_backend (sys_context);
    }
#else // MOD_SOFT
#ifdef MOD_CACA
  if (name && !strcmp (name, "caca"))
    {
      backend = mod_caca_create_backend (sys_context);
    }
#endif // MOD_CACA
#endif // MOD_SOFT
#endif // MOD_GLES2
#endif // MOD_GL1

  if (backend)
    {
      backend->call_lock = LW6SYS_MUTEX_CREATE (sys_context);
      if (backend->call_lock)
	{
	  backend->argc = argc;
	  backend->argv = argv;
	}
      else
	{
	  lw6gfx_destroy_backend (sys_context, backend);
	  backend = NULL;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("graphical backend \"%s\" does not exist"), name);
    }
#else // LW6_ALLINONE
  lw6dyn_dl_handle_t *backend_handle = NULL;

  backend_handle = lw6dyn_dlopen_backend (sys_context, argc, argv, "gfx", name);

  if (backend_handle)
    {
      char *init_func_name;
      lw6gfx_backend_t *(*init_func) (void *);

      init_func_name = lw6sys_new_sprintf (sys_context, LW6DYN_CREATE_BACKEND_FUNC_FORMAT, name);
      if (init_func_name)
	{
	  init_func = lw6dyn_dlsym (sys_context, backend_handle, init_func_name);
	  if (init_func)
	    {
	      backend = init_func (sys_context);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to find function \"%s\" in gfx backend \"%s\""), init_func_name, name);
	    }

	  LW6SYS_FREE (sys_context, init_func_name);
	}
    }


  if (backend && backend_handle)
    {
      backend->call_lock = LW6SYS_MUTEX_CREATE (sys_context);
      if (backend->call_lock)
	{
	  backend->dl_handle = backend_handle;
	  backend->argc = argc;
	  backend->argv = argv;
	}
      else
	{
	  lw6gfx_destroy_backend (sys_context, backend);
	  backend = NULL;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to open gfx backend \"%s\""), name);
    }
#endif // LW6_ALLINONE

  if (backend)
    {
      static volatile u_int32_t seq_id = 0;

      backend->id = 0;
      while (!(backend->id))
	{
	  backend->id = ++seq_id;
	}
    }

  return backend;
}

/**
 * lw6gfx_destroy_backend
 *
 * @sys_context: global system context
 * @backend: gfx backend to destroy
 *
 * Frees the ressources associated to a gfx, which must have been
 * properly uninitialized before.
 *
 * Return value: none.
 */
void
lw6gfx_destroy_backend (lw6sys_context_t * sys_context, lw6gfx_backend_t * backend)
{
#ifndef LW6_ALLINONE
  if (backend->dl_handle)
    {
      lw6dyn_dlclose_backend (sys_context, backend->dl_handle);
    }
#endif
  if (backend->call_lock)
    {
      LW6SYS_MUTEX_DESTROY (sys_context, backend->call_lock);
    }
  LW6SYS_FREE (sys_context, backend);
}
