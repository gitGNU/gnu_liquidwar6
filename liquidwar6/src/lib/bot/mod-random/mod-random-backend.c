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
#endif // HAVE_CONFIG_H

#include "../bot.h"
#include "mod-random.h"
#include "mod-random-internal.h"

/**
 * mod_random_is_GPL_compatible
 *
 * Defined to tell mod_random is compatible with GNU General Public License.
 * Of course it is. This function does nothing, but the fact it's declared
 * makes its GPL compatibility obvious. Having this declared is required.
 *
 * Return value: none
 */
void
mod_random_is_GPL_compatible ()
{
/*
 * http://www.gnu.org/prep/standards/html_node/Dynamic-Plug_002dIn-Interfaces.html
 */
}

/**
 * mod_random_is_dlclose_safe
 *
 * Defined to tell mod_random has no dlclose issue, once can safely call
 * lt_dlclose on it when done with it, without risking any segfault.
 * Some other LW6 modules/shared libraries do have this problem.
 *
 * Return value: none
 */
void
mod_random_is_dlclose_safe ()
{
}

static void *
_init (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6bot_data_t * data)
{
  _mod_random_context_t *random_context = _mod_random_init (sys_context, argc, argv, data);

  return (void *) random_context;
}

static void
_quit (lw6sys_context_t * sys_context, void *bot_context)
{
  _mod_random_context_t *random_context = (_mod_random_context_t *) bot_context;

  if (random_context)
    {
      _mod_random_quit (sys_context, random_context);
    }
}

static int
_next_move (lw6sys_context_t * sys_context, void *bot_context, int *x, int *y, lw6bot_data_t * data)
{
  int ret = 0;
  _mod_random_context_t *random_context = (_mod_random_context_t *) bot_context;

  if (random_context)
    {
      ret = _mod_random_next_move (sys_context, random_context, x, y, data);
    }

  return ret;
}

static char *
_repr (lw6sys_context_t * sys_context, void *bot_context, u_int32_t id)
{
  char *ret = NULL;
  _mod_random_context_t *random_context = (_mod_random_context_t *) bot_context;

  if (random_context)
    {
      ret = _mod_random_repr (sys_context, random_context, id);
    }

  return ret;
}

/**
 * mod_random_get_pedigree
 *
 * @sys_context: global system context
 *
 * Returns the pedigree for mod-random, giving details about the module,
 * including name, description, licence, date/time of compilation.
 *
 * Return value: dynamically allocated object.
 */
lw6sys_module_pedigree_t *
mod_random_get_pedigree (lw6sys_context_t * sys_context)
{
  lw6sys_module_pedigree_t *module_pedigree = NULL;

  module_pedigree = (lw6sys_module_pedigree_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6sys_module_pedigree_t));
  if (module_pedigree)
    {
      module_pedigree->id = "random";
      module_pedigree->category = "bot";
      module_pedigree->name = _("Random");
      module_pedigree->readme = _x_ ("A totally random bot, cursor goes anywhere, whithout any logic.");
      module_pedigree->version = VERSION;
      module_pedigree->copyright = "Copyright (C)  2015  Christian Mauduit <ufoot@ufoot.org>";
      module_pedigree->license = "GPLv3+ (GNU GPL version 3 or later)";
      module_pedigree->date = __DATE__;
      module_pedigree->time = __TIME__;
    }

  return module_pedigree;
}

/**
 * mod_random_create_backend
 *
 * @sys_context: global system context
 *
 * Creates a mod-random backend.
 *
 * Return value: backend pointer.
 */
lw6bot_backend_t *
mod_random_create_backend (lw6sys_context_t * sys_context)
{
  lw6bot_backend_t *backend;

  backend = LW6SYS_MALLOC (sys_context, sizeof (lw6bot_backend_t));
  if (backend)
    {
      memset (backend, 0, sizeof (lw6bot_backend_t));

      backend->init = _init;
      backend->quit = _quit;
      backend->next_move = _next_move;
      backend->repr = _repr;
    }

  return backend;
}
