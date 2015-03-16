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

#include "../gfx.h"
#include "mod-caca.h"
#include "mod-caca-internal.h"

/**
 * mod_caca_is_GPL_compatible
 *
 * Defined to tell mod_caca is compatible with GNU General Public License
 * Of course it is. This function does nothing, but the fact it's declared
 * makes its GPL compatibility obvious. Having this declared is required.
 *
 * Return value: none
 */
void
mod_caca_is_GPL_compatible ()
{
/*
 * http://www.gnu.org/prep/standards/html_node/Dynamic-Plug_002dIn-Interfaces.html
 */
}

static void *
_init (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6gui_video_mode_t * video_mode, lw6gui_resize_callback_func_t resize_callback)
{
  _mod_caca_context_t *mod_caca_context = _mod_caca_init (sys_context, argc, argv, video_mode, resize_callback);

  return (void *) mod_caca_context;
}

static void
_quit (lw6sys_context_t * sys_context, void *gfx_context)
{
  _mod_caca_context_t *mod_caca_context = (_mod_caca_context_t *) gfx_context;

  if (mod_caca_context)
    {
      _mod_caca_quit (sys_context, mod_caca_context);
    }
}

static char *
_repr (lw6sys_context_t * sys_context, void *gfx_context, u_int32_t id)
{
  char *ret = NULL;
  _mod_caca_context_t *mod_caca_context = (_mod_caca_context_t *) gfx_context;

  if (mod_caca_context)
    {
      ret = _mod_caca_repr (sys_context, mod_caca_context, id);
    }

  return ret;
}

static int
_set_video_mode (lw6sys_context_t * sys_context, void *gfx_context, lw6gui_video_mode_t * video_mode)
{
  int ret = 0;
  _mod_caca_context_t *mod_caca_context = (_mod_caca_context_t *) gfx_context;

  if (mod_caca_context)
    {
      ret = _mod_caca_set_video_mode (sys_context, mod_caca_context, video_mode);
    }

  return ret;
}

static int
_get_video_mode (lw6sys_context_t * sys_context, void *gfx_context, lw6gui_video_mode_t * video_mode)
{
  int ret = 0;
  _mod_caca_context_t *mod_caca_context = (_mod_caca_context_t *) gfx_context;

  if (mod_caca_context)
    {
      ret = _mod_caca_get_video_mode (sys_context, mod_caca_context, video_mode);
    }

  return ret;
}

static int
_get_fullscreen_modes (lw6sys_context_t * sys_context, void *gfx_context, lw6gui_fullscreen_modes_t * modes)
{
  int ret = 0;
  _mod_caca_context_t *mod_caca_context = (_mod_caca_context_t *) gfx_context;

  if (mod_caca_context)
    {
      ret = _mod_caca_get_fullscreen_modes (sys_context, mod_caca_context, modes);
    }

  return ret;
}

static lw6gui_input_t *
_pump_events (lw6sys_context_t * sys_context, void *gfx_context)
{
  lw6gui_input_t *ret = NULL;
  _mod_caca_context_t *mod_caca_context = (_mod_caca_context_t *) gfx_context;

  if (mod_caca_context)
    {
      ret = _mod_caca_pump_events (sys_context, mod_caca_context);
    }

  return ret;
}

static int
_display (lw6sys_context_t * sys_context, void *gfx_context, int mask, const lw6gui_look_t * look,
	  const lw6map_level_t * level,
	  const lw6ker_game_struct_t * game_struct,
	  const lw6ker_game_state_t * game_state,
	  lw6pil_local_cursors_t * local_cursors,
	  lw6gui_menu_t * menu, float progress, float fps, float mps, const char **log_list, int capture, int gfx_debug, int debug_team_id, int debug_layer_id)
{
  int ret = 0;
  _mod_caca_context_t *mod_caca_context = (_mod_caca_context_t *) gfx_context;

  if (mod_caca_context)
    {
      ret =
	_mod_caca_display (sys_context, mod_caca_context, mask, look, level, game_struct,
			   game_state, local_cursors, menu, progress, fps, mps, log_list, capture, gfx_debug, debug_team_id, debug_layer_id);
    }

  return ret;
}

/**
 * mod_caca_get_pedigree
 *
 * @sys_context: global system context
 *
 * Returns the pedigree for mod-caca, giving details about the module,
 * including name, description, licence, date/time of compilation.
 *
 * Return value: dynamically allocated object.
 */
lw6sys_module_pedigree_t *
mod_caca_get_pedigree (lw6sys_context_t * sys_context)
{
  lw6sys_module_pedigree_t *module_pedigree = NULL;

  module_pedigree = (lw6sys_module_pedigree_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6sys_module_pedigree_t));
  if (module_pedigree)
    {
      module_pedigree->id = "caca";
      module_pedigree->category = "gfx";
      module_pedigree->name = _("libcaca");
      module_pedigree->readme = _x_ ("ASCII-art based backend, using libcaca from Caca Labs.");
      module_pedigree->version = VERSION;
      module_pedigree->copyright = "Copyright (C)  2015  Christian Mauduit <ufoot@ufoot.org>";
      module_pedigree->license = "GPLv3+ (GNU GPL version 3 or later)";
      module_pedigree->date = __DATE__;
      module_pedigree->time = __TIME__;
    }

  return module_pedigree;
}

/**
 * mod_caca_create_backend
 *
 * @sys_context: global system context
 *
 * Creates a mod-caca backend.
 *
 * Return value: backend pointer.
 */
lw6gfx_backend_t *
mod_caca_create_backend (lw6sys_context_t * sys_context)
{
  lw6gfx_backend_t *backend;

  backend = LW6SYS_CALLOC (sys_context, sizeof (lw6gfx_backend_t));
  if (backend)
    {
      backend->init = _init;
      backend->quit = _quit;
      backend->repr = _repr;
      backend->set_video_mode = _set_video_mode;
      backend->get_video_mode = _get_video_mode;
      backend->get_fullscreen_modes = _get_fullscreen_modes;
      backend->pump_events = _pump_events;
      backend->display = _display;
    }

  return backend;
}
