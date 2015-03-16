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

#include "gl1-splash.h"
#include "gl1-splash-internal.h"

_mod_gl1_splash_context_t *
_mod_gl1_splash_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  _mod_gl1_splash_context_t *splash_context = NULL;

  splash_context = (_mod_gl1_splash_context_t *) LW6SYS_CALLOC (sys_context, sizeof (_mod_gl1_splash_context_t));
  if (splash_context)
    {
      if (_mod_gl1_splash_load_data (sys_context, utils_context, splash_context))
	{
	  // ok
	}
      else
	{
	  LW6SYS_FREE (sys_context, splash_context);
	  splash_context = NULL;
	}
    }

  return splash_context;
}

void *
mod_gl1_splash_init (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context)
{
  void *ret = NULL;
  if (utils_context)
    {
      ret = (void *) _mod_gl1_splash_init (sys_context, utils_context);
    }

  return ret;
}

void
_mod_gl1_splash_quit (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, _mod_gl1_splash_context_t * splash_context)
{
  if (utils_context && splash_context)
    {
      if (splash_context->text.shaded_text)
	{
	  mod_gl1_utils_shaded_text_free (sys_context, utils_context, splash_context->text.shaded_text);
	}
      _mod_gl1_splash_unload_data (sys_context, utils_context, splash_context);
      LW6SYS_FREE (sys_context, splash_context);
    }
}

void
mod_gl1_splash_quit (lw6sys_context_t * sys_context, mod_gl1_utils_context_t * utils_context, void *splash_context)
{
  _mod_gl1_splash_quit (sys_context, utils_context, (_mod_gl1_splash_context_t *) splash_context);
}
