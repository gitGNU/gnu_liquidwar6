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

#include "dsp.h"
#include "dsp-internal.h"

/**
 * lw6dsp_param_zero
 *
 * @sys_context: global system context
 * @param: the structure to initialize
 *
 * Fills a display param struct with zeros, this is mandatory
 * before any use. Think of it as a raw memset.
 *
 * Return value: none.
 */
void
lw6dsp_param_zero (lw6sys_context_t * sys_context, lw6dsp_param_t * param)
{
  memset (param, 0, sizeof (lw6dsp_param_t));
}

void
_lw6dsp_param_clear (lw6sys_context_t * sys_context, lw6dsp_param_t * param)
{
  if (param->menu)
    {
      lw6gui_menu_free (sys_context, param->menu);
    }
  if (param->look)
    {
      lw6gui_look_free (sys_context, param->look);
    }

  lw6dsp_param_zero (sys_context, param);
}

int
_lw6dsp_param_diff (lw6sys_context_t * sys_context, const _lw6dsp_data_t * data, const lw6dsp_param_t * param)
{
  int ret = 0;

  if (memcmp (&(data->param.misc), &(param->misc), sizeof (lw6dsp_misc_t)))
    {
      ret = ret | _LW6DSP_PARAM_DIFF_MISC;
    }

  if (!lw6gui_video_mode_is_same (sys_context, &(data->param.video_mode), &(param->video_mode)))
    {
      ret = ret | _LW6DSP_PARAM_DIFF_VIDEO_MODE;
    }

  if (!lw6gui_look_is_same (sys_context, data->param.look, param->look))
    {
      ret = ret | _LW6DSP_PARAM_DIFF_LOOK;
    }

  if (!lw6gui_menu_is_same (sys_context, data->param.menu, param->menu))
    {
      ret = ret | _LW6DSP_PARAM_DIFF_MENU;
    }

  if (data->param.level != param->level)
    {
      ret = ret | _LW6DSP_PARAM_DIFF_LEVEL;
    }

  if (data->param.game_struct != param->game_struct)
    {
      ret = ret | _LW6DSP_PARAM_DIFF_GAME_STRUCT;
    }

  if (data->param.game_state != param->game_state)
    {
      ret = ret | _LW6DSP_PARAM_DIFF_GAME_STATE;
    }

  if (data->param.pilot != param->pilot)
    {
      ret = ret | _LW6DSP_PARAM_DIFF_PILOT;
    }

  return ret;
}
