/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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

#include "cli.h"

#ifdef LW6_ALLINONE
#include "mod-tcp/mod-tcp.h"
#include "mod-udp/mod-udp.h"
#include "mod-http/mod-http.h"
#endif

#ifdef MOD_HTTP
#define _CLI_DEFAULT_BACKENDS "tcp,udp,http"
#else
#define _CLI_DEFAULT_BACKENDS "tcp,udp"
#endif

/**
 * lw6cli_default_backends
 * 
 * Returns the list of the default cli backends.
 *
 * Return value: comma separated string, must not be freed.
 */
char *
lw6cli_default_backends ()
{
  return _CLI_DEFAULT_BACKENDS;
}

/**
 * lw6cli_get_backends
 *
 * @argc: argc, as passed to @main
 * @argv: argv, as passed to @main
 *
 * List available cli backends. The hash contains pairs with id and name
 * for each backend. The id is the technical key you can use to
 * load the backend, the name is something more readable you can display
 * in an interface. The backend objects themselves are not instanciated
 * by this (in fact, they are, but released on the fly) you need to
 * load and initialize them afterwards.
 *
 * Return value: hash containing id/name pairs.
 */
lw6sys_assoc_t *
lw6cli_get_backends (int argc, char *argv[])
{
  lw6sys_assoc_t *ret = NULL;
#ifdef LW6_ALLINONE
  lw6sys_module_pedigree_t *module_pedigree = NULL;

  ret = lw6sys_assoc_new (lw6sys_free_callback);
  if (ret)
    {
      module_pedigree = mod_tcp_get_pedigree ();
      if (module_pedigree)
	{
	  lw6sys_assoc_set (&ret, module_pedigree->id,
			    lw6sys_str_copy (module_pedigree->name));
	  LW6SYS_FREE (module_pedigree);
	}
      module_pedigree = mod_udp_get_pedigree ();
      if (module_pedigree)
	{
	  lw6sys_assoc_set (&ret, module_pedigree->id,
			    lw6sys_str_copy (module_pedigree->name));
	  LW6SYS_FREE (module_pedigree);
	}
#ifdef MOD_HTTP
      module_pedigree = mod_http_get_pedigree ();
      if (module_pedigree)
	{
	  lw6sys_assoc_set (&ret, module_pedigree->id,
			    lw6sys_str_copy (module_pedigree->name));
	  LW6SYS_FREE (module_pedigree);
	}
#endif
    }
#else
  ret = lw6dyn_list_backends (argc, argv, "cli");
#endif

  return ret;
}

/**
 * lw6cli_create_backend
 *
 * @argc: argc, as passed to @main
 * @argv: argv, as passed to @main
 * @name: string containing cli key, typically got from @lw6cli_get_backends
 *
 * Creates a cli backend, this is just about loading the dynamic
 * library if needed, and/or check cli engine is available, and
 * connect to it. It does not perform initialization.
 *
 * Return value: cli backend.
 */
lw6cli_backend_t *
lw6cli_create_backend (int argc, char *argv[], char *name)
{
  lw6cli_backend_t *backend = NULL;

#ifdef LW6_ALLINONE
  if (name && !strcmp (name, "tcp"))
    {
      backend = mod_tcp_create_backend ();
    }
  if (name && !strcmp (name, "udp"))
    {
      backend = mod_udp_create_backend ();
    }
#ifdef MOD_HTTP
  if (name && !strcmp (name, "http"))
    {
      backend = mod_http_create_backend ();
    }
#endif

  if (backend)
    {
      backend->argc = argc;
      backend->argv = argv;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("client backend \"%s\" does not exist"), name);
    }
#else
  lw6dyn_dl_handle_t *backend_handle = NULL;

  backend_handle = lw6dyn_dlopen_backend (argc, argv, "cli", name);

  if (backend_handle)
    {
      char *init_func_name;
      lw6cli_backend_t *(*init_func) ();

      init_func_name =
	lw6sys_new_sprintf (LW6DYN_CREATE_BACKEND_FUNC_FORMAT, name);
      if (init_func_name)
	{
	  init_func = lw6dyn_dlsym (backend_handle, init_func_name);
	  if (init_func)
	    {
	      backend = init_func ();
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _x_
			  ("unable to find function \"%s\" in client backend \"%s\""),
			  init_func_name, name);
	    }

	  LW6SYS_FREE (init_func_name);
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
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("unable to open client backend \"%s\""), name);
    }
#endif

  if (backend)
    {
      static u_int32_t seq_id = 0;

      backend->id = 0;
      while (!(backend->id))
	{
	  backend->id = ++seq_id;
	}

      backend->name = lw6sys_str_copy (name);
      if (!(backend->name))
	{
	  lw6cli_destroy_backend (backend);
	}
    }

  return backend;
}

/**
 * lw6cli_destroy_backend
 *
 * @backend: backend to destroy
 *
 * Destroys a cli backend.
 *
 * Return value: none.
 */
void
lw6cli_destroy_backend (lw6cli_backend_t * backend)
{
#ifndef LW6_ALLINONE
  lw6dyn_dlclose_backend (backend->dl_handle);
#endif
  if (backend->name)
    {
      LW6SYS_FREE (backend->name);
      backend->name = NULL;
    }
  LW6SYS_FREE (backend);
}
