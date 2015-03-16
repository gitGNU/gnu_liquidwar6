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

#ifndef LIQUIDWAR6CLI_HTTP_H
#define LIQUIDWAR6CLI_HTTP_H

#include "../cli.h"

#define MOD_HTTP_BACKEND_ID "http"

/* mod-http-backend.c */
extern lw6sys_module_pedigree_t *mod_http_get_pedigree (lw6sys_context_t * sys_context);
extern lw6cli_backend_t *mod_http_create_backend (lw6sys_context_t * sys_context);
extern void mod_http_is_GPL_compatible ();
extern void mod_http_is_dlclose_safe ();

#endif // LIQUIDWAR6CLI_HTTP_H
