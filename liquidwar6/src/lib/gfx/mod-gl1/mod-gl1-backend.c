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

#include "../gfx.h"
#include "mod-gl1.h"
#include "mod-gl1-internal.h"

/**
 * mod_gl1_is_GPL_compatible
 *
 * Defined to tell mod_gl1 is compatible with GNU General Public License
 * Of course it is. This function does nothing, but the fact it's declared
 * makes its GPL compatibility obvious. Having this declared is required.
 *
 * Return value: none
 */
void
mod_gl1_is_GPL_compatible ()
{
/*
 * http://www.gnu.org/prep/standards/html_node/Dynamic-Plug_002dIn-Interfaces.html
 */
}

static void *
_init (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6gui_video_mode_t * video_mode, lw6gui_resize_callback_func_t resize_callback)
{
  _mod_gl1_context_t *mod_gl1_context = _mod_gl1_init (sys_context, argc, argv, video_mode, resize_callback);

  return (void *) mod_gl1_context;
}

static void
_quit (lw6sys_context_t * sys_context, void *gfx_context)
{
  _mod_gl1_context_t *mod_gl1_context = (_mod_gl1_context_t *) gfx_context;

  if (mod_gl1_context)
    {
      _mod_gl1_quit (sys_context, mod_gl1_context);
    }
}

static char *
_repr (lw6sys_context_t * sys_context, void *gfx_context, u_int32_t id)
{
  char *ret = NULL;
  _mod_gl1_context_t *mod_gl1_context = (_mod_gl1_context_t *) gfx_context;

  if (mod_gl1_context)
    {
      ret = _mod_gl1_repr (sys_context, mod_gl1_context, id);
    }

  return ret;
}

static int
_set_video_mode (lw6sys_context_t * sys_context, void *gfx_context, lw6gui_video_mode_t * video_mode)
{
  int ret = 0;
  _mod_gl1_context_t *mod_gl1_context = (_mod_gl1_context_t *) gfx_context;

  if (mod_gl1_context)
    {
      ret = mod_gl1_utils_set_video_mode (sys_context, &(mod_gl1_context->utils_context), video_mode);
    }

  return ret;
}

static int
_get_video_mode (lw6sys_context_t * sys_context, void *gfx_context, lw6gui_video_mode_t * video_mode)
{
  int ret = 0;
  _mod_gl1_context_t *mod_gl1_context = (_mod_gl1_context_t *) gfx_context;

  if (mod_gl1_context)
    {
      ret = mod_gl1_utils_get_video_mode (sys_context, &(mod_gl1_context->utils_context), video_mode);
    }

  return ret;
}

static int
_get_fullscreen_modes (lw6sys_context_t * sys_context, void *gfx_context, lw6gui_fullscreen_modes_t * modes)
{
  int ret = 0;
  _mod_gl1_context_t *mod_gl1_context = (_mod_gl1_context_t *) gfx_context;

  if (mod_gl1_context)
    {
      ret = mod_gl1_utils_get_fullscreen_modes (sys_context, &(mod_gl1_context->utils_context), modes);
    }

  return ret;
}

static lw6gui_input_t *
_pump_events (lw6sys_context_t * sys_context, void *gfx_context)
{
  lw6gui_input_t *ret = NULL;
  _mod_gl1_context_t *mod_gl1_context = (_mod_gl1_context_t *) gfx_context;

  if (mod_gl1_context)
    {
      _lw6gfx_sdl_timer_update (sys_context, &(mod_gl1_context->utils_context.sdl_context));
      ret =
	_lw6gfx_sdl_pump_events (sys_context, &(mod_gl1_context->utils_context.sdl_context), mod_gl1_utils_pump_event_callback,
				 (void *) &(mod_gl1_context->utils_context));
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
  _mod_gl1_context_t *mod_gl1_context = (_mod_gl1_context_t *) gfx_context;
  lw6gui_look_t *look2 = NULL;

  if (mod_gl1_context)
    {
      _lw6gfx_sdl_timer_update (sys_context, &(mod_gl1_context->utils_context.sdl_context));
      mod_gl1_context->utils_context.counter.nb_frames++;

      /*
       * This call to smoothers_update shouldn't be needed anymore when
       * drawable will be fully handled.
       */
      mod_gl1_utils_smoothers_update (sys_context, &(mod_gl1_context->utils_context));	// to be fixed
      _mod_gl1_set_backends (sys_context, mod_gl1_context, look);
      mod_gl1_utils_cache_update (sys_context, &(mod_gl1_context->utils_context), look);

      mod_gl1_utils_prepare_buffer (sys_context, &(mod_gl1_context->utils_context), look);

      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display mask=%d"), mask);
      if (mask & LW6GUI_DISPLAY_SPLASH)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display step=splash"));
	  mod_gl1_splash_display (sys_context, &(mod_gl1_context->utils_context), mod_gl1_context->splash_context);
	  look2 = lw6gui_look_dup (sys_context, look);
	  if (look2)
	    {
	      mod_gl1_splash_patch_system_color (sys_context, &(mod_gl1_context->utils_context), mod_gl1_context->splash_context,
						 &(look2->style.color_set.system_color));
	    }
	}
      else
	{
	  /*
	   * We display background/view/hud/menu only if splash isn't
	   * activated, splash is really "instead of the rest"
	   */
	  if (mask & LW6GUI_DISPLAY_BACKGROUND)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display step=background"));
	      mod_gl1_background_display_background (sys_context, &(mod_gl1_context->utils_context), mod_gl1_context->background_backend, look);
	    }
	  if ((mask & LW6GUI_DISPLAY_PREVIEW) && level && !game_state)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display step=preview"));
	      mod_gl1_view_display_preview (sys_context, &(mod_gl1_context->utils_context), mod_gl1_context->view_backend, look, level);
	    }
	  if ((mask & LW6GUI_DISPLAY_MAP) && game_state)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display step=map"));
	      mod_gl1_view_display_map (sys_context, &(mod_gl1_context->utils_context), mod_gl1_context->view_backend, look, game_state, local_cursors);
	    }
	  if ((mask & LW6GUI_DISPLAY_FIGHTERS) && game_state)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display step=fighters"));
	      mod_gl1_view_display_fighters (sys_context, &(mod_gl1_context->utils_context), mod_gl1_context->view_backend, look, game_state, local_cursors);
	    }
	  if ((mask & LW6GUI_DISPLAY_DEBUG_ZONES) && game_struct)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display step=zones"));
	      mod_gl1_utils_display_zones (sys_context, &(mod_gl1_context->utils_context), look, game_struct);
	    }
	  if ((mask & LW6GUI_DISPLAY_DEBUG_GRADIENT) && game_state)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display step=gradient"));
	      mod_gl1_utils_display_gradient (sys_context, &(mod_gl1_context->utils_context), look, game_state, debug_team_id, debug_layer_id);
	    }
	  if ((mask & LW6GUI_DISPLAY_CURSORS) && game_state)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display step=cursors"));
	      mod_gl1_view_display_cursors (sys_context, &(mod_gl1_context->utils_context), mod_gl1_context->view_backend, look, game_state, local_cursors);
	    }
	  if ((mask & LW6GUI_DISPLAY_HUD) && game_state)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display step=hud"));
	      mod_gl1_hud_display_hud (sys_context, &(mod_gl1_context->utils_context), mod_gl1_context->hud_backend, look, game_state, local_cursors);
	    }
	  if ((mask & LW6GUI_DISPLAY_SCORE) && game_state)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display step=score"));
	      mod_gl1_hud_display_score (sys_context, &(mod_gl1_context->utils_context), mod_gl1_context->hud_backend, look, game_state, local_cursors);
	    }
	  if ((mask & LW6GUI_DISPLAY_MENU) && menu)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display step=menu"));
	      mod_gl1_menu_display_menu (sys_context, &(mod_gl1_context->utils_context), mod_gl1_context->menu_backend, look, menu);
	    }
	  if ((mask & LW6GUI_DISPLAY_PROGRESS))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display step=progress"));
	      mod_gl1_menu_display_progress (sys_context, &(mod_gl1_context->utils_context), mod_gl1_context->menu_backend, look, progress);
	    }
	  if ((mask & LW6GUI_DISPLAY_META) && menu)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display step=meta"));
	      mod_gl1_menu_display_meta (sys_context, &(mod_gl1_context->utils_context), mod_gl1_context->menu_backend, look, menu);
	    }
	}
      if ((mask & LW6GUI_DISPLAY_LOG) && log_list)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display step=log"));
	  mod_gl1_utils_display_log (sys_context, &(mod_gl1_context->utils_context), look2 ? look2 : look, log_list);
	}
      if (mask & LW6GUI_DISPLAY_FPS)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display step=fps"));
	  mod_gl1_utils_display_fps (sys_context, &(mod_gl1_context->utils_context), look2 ? look2 : look, fps);
	}
      if ((mask & LW6GUI_DISPLAY_MPS) && game_struct)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display step=mps"));
	  mod_gl1_utils_display_mps (sys_context, &(mod_gl1_context->utils_context),
				     look2 ? look2 : look, mps, game_struct->rules.rounds_per_sec * game_struct->rules.moves_per_round);
	}
      if (mask & LW6GUI_DISPLAY_URL)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display step=url"));
	  mod_gl1_utils_display_url (sys_context, &(mod_gl1_context->utils_context), look2 ? look2 : look, lw6sys_build_get_home_url ());
	}
      mod_gl1_utils_show_mouse (sys_context, &(mod_gl1_context->utils_context), mask & LW6GUI_DISPLAY_MOUSE, 0);

      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("display swap"));

      if (capture)
	{
	  mod_gl1_utils_capture2disk (sys_context, &(mod_gl1_context->utils_context));
	}
      if (gfx_debug)
	{
	  mod_gl1_utils_bitmap_hash_dump2disk (sys_context, &(mod_gl1_context->utils_context), 0);
	}
      mod_gl1_utils_swap_buffers (sys_context, &(mod_gl1_context->utils_context));

      if (look2)
	{
	  lw6gui_look_free (sys_context, look2);
	  look2 = NULL;
	}
      ret = 1;
    }

  return ret;
}

/**
 * mod_gl1_get_pedigree
 *
 * @sys_context: global system context
 *
 * Returns the pedigree for mod-gl1, giving details about the module,
 * including name, description, licence, date/time of compilation.
 *
 * Return value: dynamically allocated object.
 */
lw6sys_module_pedigree_t *
mod_gl1_get_pedigree (lw6sys_context_t * sys_context)
{
  lw6sys_module_pedigree_t *module_pedigree = NULL;

  module_pedigree = (lw6sys_module_pedigree_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6sys_module_pedigree_t));
  if (module_pedigree)
    {
      module_pedigree->id = "gl1";
      module_pedigree->category = "gfx";
      module_pedigree->name = _("OpenGL v1 / SDL");
      module_pedigree->readme =
	_x_
	("This is the most accomplished graphics backend for Liquid War 6. It uses OpenGL v1 hardware acceleration if available, and uses SDL as a technical wrapper.");
      module_pedigree->version = VERSION;
      module_pedigree->copyright = "Copyright (C)  2015  Christian Mauduit <ufoot@ufoot.org>";
      module_pedigree->license = "GPLv3+ (GNU GPL version 3 or later)";
      module_pedigree->date = __DATE__;
      module_pedigree->time = __TIME__;
    }

  return module_pedigree;
}

/**
 * mod_gl1_create_backend
 *
 * @sys_context: global system context
 *
 * Creates a mod-gl1 backend.
 *
 * Return value: backend pointer.
 */
lw6gfx_backend_t *
mod_gl1_create_backend (lw6sys_context_t * sys_context)
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
