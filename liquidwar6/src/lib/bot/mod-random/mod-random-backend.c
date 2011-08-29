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

#include "../bot.h"
#include "mod-random.h"
#include "mod-random-internal.h"

/*
 * http://www.gnu.org/prep/standards/html_node/Dynamic-Plug_002dIn-Interfaces.html
 */
void
mod_random_is_GPL_compatible ()
{
}

static void *
_init (int argc, char *argv[], lw6bot_data_t * data)
{
  _mod_random_context_t *random_context = _mod_random_init (argc, argv, data);

  return (void *) random_context;
}

static void
_quit (void *bot_context)
{
  _mod_random_context_t *random_context =
    (_mod_random_context_t *) bot_context;

  if (random_context)
    {
      _mod_random_quit (random_context);
    }
}

static int
_next_move (void *bot_context, int *x, int *y, lw6bot_data_t * data)
{
  int ret = 0;
  _mod_random_context_t *random_context =
    (_mod_random_context_t *) bot_context;

  if (random_context)
    {
      ret = _mod_random_next_move (random_context, x, y, data);
    }

  return ret;
}

static char *
_repr (void *bot_context, u_int32_t id)
{
  char *ret = NULL;
  _mod_random_context_t *random_context =
    (_mod_random_context_t *) bot_context;

  if (random_context)
    {
      ret = _mod_random_repr (random_context, id);
    }

  return ret;
}

lw6sys_module_pedigree_t *
mod_random_get_pedigree ()
{
  lw6sys_module_pedigree_t *module_pedigree = NULL;

  module_pedigree =
    (lw6sys_module_pedigree_t *)
    LW6SYS_CALLOC (sizeof (lw6sys_module_pedigree_t));
  if (module_pedigree)
    {
      module_pedigree->id = "random";
      module_pedigree->category = "bot";
      module_pedigree->name = _("Random");
      module_pedigree->readme =
	_x_
	("A totally random bot, cursor goes anywhere, whithout any logic.");
      module_pedigree->version = VERSION;
      module_pedigree->copyright =
	"Copyright (C)  2011  Christian Mauduit <ufoot@ufoot.org>";
      module_pedigree->license = "GPLv3+ (GNU GPL version 3 or later)";
      module_pedigree->date = __DATE__;
      module_pedigree->time = __TIME__;
    }

  return module_pedigree;
}

lw6bot_backend_t *
mod_random_create_backend ()
{
  lw6bot_backend_t *backend;

  backend = LW6SYS_MALLOC (sizeof (lw6bot_backend_t));
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
