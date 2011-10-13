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

#include "../gfx.h"
#include "mod-gl.h"
#include "mod-gl-internal.h"

/*
 * http://www.gnu.org/prep/standards/html_node/Dynamic-Plug_002dIn-Interfaces.html
 */
void
mod_gl_is_GPL_compatible ()
{
}

static void *
_init (int argc, char *argv[], lw6gui_video_mode_t * video_mode,
       lw6gui_resize_callback_func_t resize_callback)
{
  _mod_gl_context_t *mod_gl_context =
    _mod_gl_init (argc, argv, video_mode, resize_callback);

  return (void *) mod_gl_context;
}

static void
_quit (void *gfx_context)
{
  _mod_gl_context_t *mod_gl_context = (_mod_gl_context_t *) gfx_context;

  if (mod_gl_context)
    {
      _mod_gl_quit (mod_gl_context);
    }
}

static char *
_repr (void *gfx_context, u_int32_t id)
{
  char *ret = NULL;
  _mod_gl_context_t *mod_gl_context = (_mod_gl_context_t *) gfx_context;

  if (mod_gl_context)
    {
      ret = _mod_gl_repr (mod_gl_context, id);
    }

  return ret;
}

static int
_set_video_mode (void *gfx_context, lw6gui_video_mode_t * video_mode)
{
  int ret = 0;
  _mod_gl_context_t *mod_gl_context = (_mod_gl_context_t *) gfx_context;

  if (mod_gl_context)
    {
      ret =
	mod_gl_utils_set_video_mode (&(mod_gl_context->utils_context),
				     video_mode);
    }

  return ret;
}

static int
_get_video_mode (void *gfx_context, lw6gui_video_mode_t * video_mode)
{
  int ret = 0;
  _mod_gl_context_t *mod_gl_context = (_mod_gl_context_t *) gfx_context;

  if (mod_gl_context)
    {
      ret =
	mod_gl_utils_get_video_mode (&(mod_gl_context->utils_context),
				     video_mode);
    }

  return ret;
}

static int
_get_fullscreen_modes (void *gfx_context, lw6gui_fullscreen_modes_t * modes)
{
  int ret = 0;
  _mod_gl_context_t *mod_gl_context = (_mod_gl_context_t *) gfx_context;

  if (mod_gl_context)
    {
      ret =
	mod_gl_utils_get_fullscreen_modes (&(mod_gl_context->utils_context),
					   modes);
    }

  return ret;
}

static lw6gui_input_t *
_pump_events (void *gfx_context)
{
  lw6gui_input_t *ret = NULL;
  _mod_gl_context_t *mod_gl_context = (_mod_gl_context_t *) gfx_context;

  if (mod_gl_context)
    {
      mod_gl_utils_timer_update (&(mod_gl_context->utils_context));
      ret = mod_gl_utils_pump_events (&(mod_gl_context->utils_context));
    }

  return ret;
}

static int
_display (void *gfx_context, int mask, lw6gui_look_t * look,
	  lw6map_level_t * level,
	  lw6ker_game_struct_t * game_struct,
	  lw6ker_game_state_t * game_state,
	  lw6pil_local_cursors_t * local_cursors,
	  lw6gui_menu_t * menu,
	  float progress,
	  float fps, float mps, char **log_list, int capture, int gfx_debug,
	  int debug_team_id, int debug_layer_id)
{
  int ret = 0;
  _mod_gl_context_t *mod_gl_context = (_mod_gl_context_t *) gfx_context;

  if (mod_gl_context)
    {
      mod_gl_utils_timer_update (&(mod_gl_context->utils_context));
      mod_gl_context->utils_context.counter.nb_frames++;

      /*
       * This call to smoothers_update shouldn't be needed anymore when
       * drawable will be fully handled.
       */
      mod_gl_utils_smoothers_update (&(mod_gl_context->utils_context));	// to be fixed

      _mod_gl_set_backends (mod_gl_context, look);

      mod_gl_utils_prepare_buffer (&(mod_gl_context->utils_context), look);

      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display mask=%d"), mask);
      if (mask & LW6GUI_DISPLAY_SPLASH)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display step=splash"));
	  mod_gl_splash_display (&(mod_gl_context->utils_context),
				 mod_gl_context->splash_context);
	  mod_gl_splash_patch_system_color (&(mod_gl_context->utils_context),
					    mod_gl_context->splash_context,
					    &(look->style.color_set.
					      system_color));
	}
      else
	{
	  /*
	   * We display background/view/hud/menu only if splash isn't
	   * activated, splash is really "instead of the rest"
	   */
	  if (mask & LW6GUI_DISPLAY_BACKGROUND)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display step=background"));
	      mod_gl_background_display_background (&
						    (mod_gl_context->
						     utils_context),
						    mod_gl_context->
						    background_backend, look);
	    }
	  if ((mask & LW6GUI_DISPLAY_PREVIEW) && level && !game_state)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display step=preview"));
	      mod_gl_view_display_preview (&(mod_gl_context->utils_context),
					   mod_gl_context->view_backend, look,
					   level);
	    }
	  if ((mask & LW6GUI_DISPLAY_MAP) && game_state)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display step=map"));
	      mod_gl_view_display_map (&(mod_gl_context->utils_context),
				       mod_gl_context->view_backend, look,
				       game_state, local_cursors);
	    }
	  if ((mask & LW6GUI_DISPLAY_FIGHTERS) && game_state)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display step=fighters"));
	      mod_gl_view_display_fighters (&(mod_gl_context->utils_context),
					    mod_gl_context->view_backend,
					    look, game_state, local_cursors);
	    }
	  if ((mask & LW6GUI_DISPLAY_DEBUG_ZONES) && game_struct)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display step=zones"));
	      mod_gl_utils_display_zones (&(mod_gl_context->utils_context),
					  look, game_struct);
	    }
	  if ((mask & LW6GUI_DISPLAY_DEBUG_GRADIENT) && game_state)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display step=gradient"));
	      mod_gl_utils_display_gradient (&(mod_gl_context->utils_context),
					     look, game_state, debug_team_id,
					     debug_layer_id);
	    }
	  if ((mask & LW6GUI_DISPLAY_CURSORS) && game_state)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display step=cursors"));
	      mod_gl_view_display_cursors (&(mod_gl_context->utils_context),
					   mod_gl_context->view_backend, look,
					   game_state, local_cursors);
	    }
	  if ((mask & LW6GUI_DISPLAY_HUD) && game_state)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display step=hud"));
	      mod_gl_hud_display_hud (&(mod_gl_context->utils_context),
				      mod_gl_context->hud_backend, look,
				      game_state, local_cursors);
	    }
	  if ((mask & LW6GUI_DISPLAY_SCORE) && game_state)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display step=score"));
	      mod_gl_hud_display_score (&(mod_gl_context->utils_context),
					mod_gl_context->hud_backend, look,
					game_state, local_cursors);
	    }
	  if ((mask & LW6GUI_DISPLAY_MENU) && menu)
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display step=menu"));
	      mod_gl_menu_display_menu (&(mod_gl_context->utils_context),
					mod_gl_context->menu_backend, look,
					menu);
	    }
	  if ((mask & LW6GUI_DISPLAY_PROGRESS))
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display step=progress"));
	      mod_gl_menu_display_progress (&(mod_gl_context->utils_context),
					    mod_gl_context->menu_backend,
					    look, progress);
	    }
	}
      if ((mask & LW6GUI_DISPLAY_LOG) && log_list)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display step=log"));
	  mod_gl_utils_display_log (&(mod_gl_context->utils_context), look,
				    log_list);
	}
      if (mask & LW6GUI_DISPLAY_FPS)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display step=fps"));
	  mod_gl_utils_display_fps (&(mod_gl_context->utils_context), look,
				    fps);
	}
      if ((mask & LW6GUI_DISPLAY_MPS) && game_struct)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display step=mps"));
	  mod_gl_utils_display_mps (&(mod_gl_context->utils_context), look,
				    mps,
				    game_struct->rules.rounds_per_sec *
				    game_struct->rules.moves_per_round);
	}
      if (mask & LW6GUI_DISPLAY_URL)
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display step=url"));
	  mod_gl_utils_display_url (&(mod_gl_context->utils_context),
				    look, lw6sys_build_get_home_url ());
	}
      mod_gl_utils_show_mouse (&(mod_gl_context->utils_context),
			       mask & LW6GUI_DISPLAY_MOUSE, 0);

      lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("display swap"));

      if (capture)
	{
	  mod_gl_utils_capture2disk (&(mod_gl_context->utils_context));
	}
      if (gfx_debug)
	{
	  mod_gl_utils_bitmap_hash_dump2disk (&
					      (mod_gl_context->utils_context),
					      0);
	}
      mod_gl_utils_swap_buffers (&(mod_gl_context->utils_context));

      ret = 1;
    }

  return ret;
}

lw6sys_module_pedigree_t *
mod_gl_get_pedigree ()
{
  lw6sys_module_pedigree_t *module_pedigree = NULL;

  module_pedigree =
    (lw6sys_module_pedigree_t *)
    LW6SYS_CALLOC (sizeof (lw6sys_module_pedigree_t));
  if (module_pedigree)
    {
      module_pedigree->id = "gl";
      module_pedigree->category = "gfx";
      module_pedigree->name = _("OpenGL/SDL");
      module_pedigree->readme =
	_x_
	("This is the most accomplished graphics backend for Liquid War 6. It uses OpenGL hardware acceleration if available, and uses SDL as a technical wrapper.");
      module_pedigree->version = VERSION;
      module_pedigree->copyright =
	"Copyright (C)  2011  Christian Mauduit <ufoot@ufoot.org>";
      module_pedigree->license = "GPLv3+ (GNU GPL version 3 or later)";
      module_pedigree->date = __DATE__;
      module_pedigree->time = __TIME__;
    }

  return module_pedigree;
}

lw6gfx_backend_t *
mod_gl_create_backend ()
{
  lw6gfx_backend_t *backend;

  backend = LW6SYS_CALLOC (sizeof (lw6gfx_backend_t));
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
