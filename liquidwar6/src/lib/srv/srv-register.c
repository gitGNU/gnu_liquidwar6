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

#include <string.h>

#include "srv.h"
#ifdef LW6_ALLINONE
#include "mod-tcpd/mod-tcpd.h"
#include "mod-udpd/mod-udpd.h"
#include "mod-httpd/mod-httpd.h"
#endif

#define _SRV_DEFAULT_BACKENDS "tcpd,udpd,httpd"

/**
 * lw6srv_default_backends
 *
 * @sys_context: global system context
 *
 * Returns the list of the default srv backends.
 *
 * Return value: comma separated string, must not be freed.
 */
char *
lw6srv_default_backends (lw6sys_context_t * sys_context)
{
  return _SRV_DEFAULT_BACKENDS;
}

/**
 * lw6srv_get_backends
 *
 * @sys_context: global system context
 * @argc: argc, as passed to @main
 * @argv: argv, as passed to @main
 *
 * List available srv backends. The hash contains pairs with id and name
 * for each backend. The id is the technical key you can use to
 * load the backend, the name is something more readable you can display
 * in an interface. The backend objects themselves are not instanciated
 * by this (in fact, they are, but released on the fly) you need to
 * load and initialize them afterwards.
 *
 * Return value: hash containing id/name pairs.
 */
lw6sys_assoc_t *
lw6srv_get_backends (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  lw6sys_assoc_t *ret = NULL;

#ifdef LW6_ALLINONE
  lw6sys_module_pedigree_t *module_pedigree = NULL;

  ret = lw6sys_assoc_new (sys_context, lw6sys_free_callback);
  if (ret)
    {
      module_pedigree = mod_tcpd_get_pedigree (sys_context);
      if (module_pedigree)
	{
	  lw6sys_assoc_set (sys_context, &ret, module_pedigree->id, lw6sys_str_copy (sys_context, module_pedigree->name));
	  LW6SYS_FREE (sys_context, module_pedigree);
	}
      module_pedigree = mod_udpd_get_pedigree (sys_context);
      if (module_pedigree)
	{
	  lw6sys_assoc_set (sys_context, &ret, module_pedigree->id, lw6sys_str_copy (sys_context, module_pedigree->name));
	  LW6SYS_FREE (sys_context, module_pedigree);
	}
      module_pedigree = mod_httpd_get_pedigree (sys_context);
      if (module_pedigree)
	{
	  lw6sys_assoc_set (sys_context, &ret, module_pedigree->id, lw6sys_str_copy (sys_context, module_pedigree->name));
	  LW6SYS_FREE (sys_context, module_pedigree);
	}
    }
#else // LW6_ALLINONE
  ret = lw6dyn_list_backends (sys_context, argc, argv, "srv");
#endif // LW6_ALLINONE

  return ret;
}

/**
 * lw6srv_create_backend
 *
 * @sys_context: global system context
 * @argc: argc, as passed to @main
 * @argv: argv, as passed to @main
 * @name: string containing srv key, typically got from @lw6srv_get_backends
 *
 * Creates a srv backend, this is just about loading the dynamic
 * library if needed, and/or check srv engine is available, and
 * connect to it. It does not perform initialization.
 *
 * Return value: srv backend.
 */
lw6srv_backend_t *
lw6srv_create_backend (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *name)
{
  lw6srv_backend_t *backend = NULL;
#ifdef LW6_ALLINONE
  if (name && !strcmp (name, "tcpd"))
    {
      backend = mod_tcpd_create_backend (sys_context);
    }
  if (name && !strcmp (name, "udpd"))
    {
      backend = mod_udpd_create_backend (sys_context);
    }
  if (name && !strcmp (name, "httpd"))
    {
      backend = mod_httpd_create_backend (sys_context);
    }

  if (backend)
    {
      backend->argc = argc;
      backend->argv = argv;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("server backend \"%s\" does not exist"), name);
    }
#else // LW6_ALLINONE
  lw6dyn_dl_handle_t *backend_handle = NULL;

  backend_handle = lw6dyn_dlopen_backend (sys_context, argc, argv, "srv", name);

  if (backend_handle)
    {
      char *init_func_name;
      lw6srv_backend_t *(*init_func) (lw6sys_context_t *);

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
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to find function \"%s\" in server backend \"%s\""), init_func_name, name);
	    }

	  LW6SYS_FREE (sys_context, init_func_name);
	}
    }


  if (backend && backend_handle)
    {
      backend->dl_handle = backend_handle;
      backend->argc = argc;
      backend->argv = argv;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to open server backend \"%s\""), name);
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

      backend->name = lw6sys_str_copy (sys_context, name);
      if (!(backend->name))
	{
	  lw6srv_destroy_backend (sys_context, backend);
	}
    }

  return backend;
}

/**
 * lw6srv_destroy_backend
 *
 * @sys_context: global system context
 * @backend: backend to destroy
 *
 * Destroys a srv backend.
 *
 * Return value: none.
 */
void
lw6srv_destroy_backend (lw6sys_context_t * sys_context, lw6srv_backend_t * backend)
{
#ifndef LW6_ALLINONE
  lw6dyn_dlclose_backend (sys_context, backend->dl_handle);
#endif // LW6_ALLINONE
  if (backend->name)
    {
      LW6SYS_FREE (sys_context, backend->name);
      backend->name = NULL;
    }
  LW6SYS_FREE (sys_context, backend);
}
