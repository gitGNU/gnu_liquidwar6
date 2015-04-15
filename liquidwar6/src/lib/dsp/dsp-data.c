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

#include "dsp-internal.h"

_lw6dsp_data_t *
_lw6dsp_data_new (lw6sys_context_t * sys_context)
{
  _lw6dsp_data_t *ret = NULL;

  ret = (_lw6dsp_data_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6dsp_data_t));
  if (ret)
    {
      ret->instant_fps = 0.0f;
      ret->average_fps = 0.0f;
      ret->render_mutex = LW6SYS_MUTEX_CREATE (sys_context);
      if (!ret->render_mutex)
	{
	  LW6SYS_FREE (sys_context, ret);
	  ret = NULL;
	}
    }

  return ret;
}

void
_lw6dsp_data_free (lw6sys_context_t * sys_context, _lw6dsp_data_t * data)
{
  if (data->gfx_backend_name)
    {
      LW6SYS_FREE (sys_context, data->gfx_backend_name);
    }
  if (data->gfx_backend)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_
		  ("graphics backend is still defined but we're freeing data, the backend should have be closed before, int the display thread (join function)"));
    }
  if (data->render_mutex)
    {
      LW6SYS_MUTEX_DESTROY (sys_context, data->render_mutex);
    }
  _lw6dsp_param_clear (sys_context, &(data->param));
  LW6SYS_FREE (sys_context, data);
}
