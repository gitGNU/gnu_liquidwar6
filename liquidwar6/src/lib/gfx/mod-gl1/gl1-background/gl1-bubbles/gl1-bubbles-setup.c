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

#include <time.h>

#include "../../../gfx.h"
#include "gl1-bubbles.h"
#include "gl1-bubbles-internal.h"

/*
 * Low-level SDL initialisation.
 */
_mod_gl1_background_bubbles_context_t *
_mod_gl1_background_bubbles_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  _mod_gl1_background_bubbles_context_t *bubbles_context = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("init background/bubbles"));

  bubbles_context = (_mod_gl1_background_bubbles_context_t *) LW6SYS_CALLOC (sys_context, sizeof (_mod_gl1_background_bubbles_context_t));
  if (!bubbles_context)
    {
      /*
       * No use to continue if this basic malloc fails...
       */
      exit (1);
    }

  if (!_mod_gl1_background_bubbles_load_data (sys_context, utils_context, bubbles_context))
    {
      LW6SYS_FREE (sys_context, bubbles_context);
      bubbles_context = NULL;
    }

  return bubbles_context;
}

void *
mod_gl1_background_bubbles_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  return (void *) _mod_gl1_background_bubbles_init (sys_context, utils_context);
}

/*
 * Ends-up all SDL stuff.
 */
void
_mod_gl1_background_bubbles_quit (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context,
				  _mod_gl1_background_bubbles_context_t * bubbles_context)
{
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("quit background/bubbles"));

  _mod_gl1_background_bubbles_unload_data (sys_context, utils_context, bubbles_context);
  LW6SYS_FREE (sys_context, bubbles_context);
}

void
mod_gl1_background_bubbles_quit (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *bubbles_context)
{
  _mod_gl1_background_bubbles_quit (sys_context, utils_context, (_mod_gl1_background_bubbles_context_t *) bubbles_context);
}
