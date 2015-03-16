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

#include "liquidwar6.h"

static SCM
_mouse_get_state (lw6sys_context_t * sys_context, lw6dsp_backend_t * c_dsp)
{
  SCM ret = SCM_BOOL_F;

  ret = scm_cons
    (scm_cons
     (scm_from_locale_string ("screen-pos-x"),
      scm_from_int (c_dsp->input->mouse.screen_pointer.pos_x)),
     (scm_cons
      (scm_cons
       (scm_from_locale_string ("screen-pos-y"),
	scm_from_int (c_dsp->input->mouse.screen_pointer.pos_y)),
       (scm_cons
	(scm_cons
	 (scm_from_locale_string ("screen-speed-x"),
	  scm_from_int (c_dsp->input->mouse.screen_pointer.speed_x)),
	 (scm_cons
	  (scm_cons
	   (scm_from_locale_string ("screen-speed-y"),
	    scm_from_int (c_dsp->input->mouse.screen_pointer.speed_y)),
	   (scm_cons
	    (scm_cons
	     (scm_from_locale_string ("map-pos-x"),
	      scm_from_int (c_dsp->input->mouse.map_pointer.pos_x)),
	     (scm_cons
	      (scm_cons
	       (scm_from_locale_string ("map-pos-y"),
		scm_from_int (c_dsp->input->mouse.map_pointer.pos_y)),
	       scm_cons
	       (scm_cons
		(scm_from_locale_string ("map-speed-x"),
		 scm_from_int (c_dsp->input->mouse.map_pointer.speed_x)),
		(scm_cons
		 (scm_cons
		  (scm_from_locale_string ("map-speed-y"),
		   scm_from_int (c_dsp->input->mouse.map_pointer.speed_y)),
		  (scm_cons
		   (scm_cons
		    (scm_from_locale_string ("menu-position"),
		     scm_from_int (c_dsp->input->mouse.menu_position)),
		    (scm_cons
		     (scm_cons
		      (scm_from_locale_string ("menu-scroll"),
		       scm_from_int (c_dsp->input->mouse.menu_scroll)),
		      (scm_cons
		       (scm_cons (scm_from_locale_string ("menu-esc"), c_dsp->input->mouse.menu_esc ? SCM_BOOL_T : SCM_BOOL_F), SCM_LIST0))))))))))))))))))));

  return ret;
}

/*
 * In liquidwar6gui
 */
static SCM
_scm_lw6gui_menu_new (SCM title, SCM help, SCM popup, SCM esc, SCM enable_esc)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  char *c_title = NULL;
  char *c_help = NULL;
  char *c_popup = NULL;
  char *c_esc = NULL;
  int c_enable_esc = 0;
  lw6gui_menu_t *c_menu = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  if (SCM_NFALSEP (title))
    {
      SCM_ASSERT (scm_is_string (title), title, SCM_ARG1, __FUNCTION__);
    }
  if (SCM_NFALSEP (help))
    {
      SCM_ASSERT (scm_is_string (help), help, SCM_ARG2, __FUNCTION__);
    }
  if (SCM_NFALSEP (popup))
    {
      SCM_ASSERT (scm_is_string (popup), popup, SCM_ARG3, __FUNCTION__);
    }
  SCM_ASSERT (scm_is_string (esc), esc, SCM_ARG4, __FUNCTION__);
  SCM_ASSERT (SCM_BOOLP (enable_esc), enable_esc, SCM_ARG5, __FUNCTION__);

  if (SCM_NFALSEP (title))
    {
      c_title = lw6scm_utils_to_0str (sys_context, title);
    }
  else
    {
      c_title = lw6sys_str_copy (sys_context, LW6SYS_STR_EMPTY);
    }
  if (c_title)
    {
      if (SCM_NFALSEP (help))
	{
	  c_help = lw6scm_utils_to_0str (sys_context, help);
	}
      else
	{
	  c_help = lw6sys_str_copy (sys_context, LW6SYS_STR_EMPTY);
	}
      if (c_help)
	{
	  if (SCM_NFALSEP (popup))
	    {
	      c_popup = lw6scm_utils_to_0str (sys_context, popup);
	    }
	  else
	    {
	      c_popup = lw6sys_str_copy (sys_context, LW6SYS_STR_EMPTY);
	    }
	  if (c_popup)
	    {
	      c_esc = lw6scm_utils_to_0str (sys_context, esc);
	      if (c_esc)
		{
		  c_enable_esc = SCM_NFALSEP (enable_esc);

		  c_menu = lw6gui_menu_new (sys_context, c_title, c_help, c_popup, c_esc, c_enable_esc);
		  if (c_menu)
		    {
		      ret = lw6_make_scm_menu (sys_context, c_menu);
		    }
		  LW6SYS_FREE (sys_context, c_esc);
		}
	      LW6SYS_FREE (sys_context, c_popup);
	    }
	  LW6SYS_FREE (sys_context, c_help);
	}
      LW6SYS_FREE (sys_context, c_title);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_menu_append (SCM menu, SCM menuitem)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *c_label = NULL;
  char *c_tooltip = NULL;
  int c_value = 0;
  int c_enabled = 0;
  int c_selected = 0;
  int c_colored = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.menu, menu), menu, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_CONSP (menuitem) || menuitem == SCM_EOL, menuitem, SCM_ARG2, __FUNCTION__);

  c_label = lw6scm_utils_to_0str (sys_context, scm_assoc_ref (menuitem, scm_from_locale_string ("label")));
  if (c_label)
    {
      c_tooltip = lw6scm_utils_to_0str (sys_context, scm_assoc_ref (menuitem, scm_from_locale_string ("tooltip")));
      if (c_tooltip)
	{
	  lw6gui_menu_t *c_menu;

	  c_value = scm_to_int (scm_assoc_ref (menuitem, scm_from_locale_string ("value")));
	  c_enabled = scm_to_bool (scm_assoc_ref (menuitem, scm_from_locale_string ("enabled")));
	  c_selected = scm_to_bool (scm_assoc_ref (menuitem, scm_from_locale_string ("selected")));
	  c_colored = scm_to_bool (scm_assoc_ref (menuitem, scm_from_locale_string ("colored")));

	  c_menu = lw6_scm_to_menu (sys_context, menu);

	  ret =
	    scm_from_int (lw6gui_menu_append_for_id_use
			  (sys_context, c_menu, c_label, c_tooltip, c_value, c_enabled, c_selected, c_colored, lw6sys_get_timestamp (sys_context)));

	  LW6SYS_FREE (sys_context, c_tooltip);
	}
      LW6SYS_FREE (sys_context, c_label);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_menu_remove (SCM menu, SCM position)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_menu_t *c_menu = NULL;
  int c_position = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.menu, menu), menu, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (position), position, SCM_ARG2, __FUNCTION__);

  c_menu = lw6_scm_to_menu (sys_context, menu);
  c_position = scm_to_int (position);

  ret = lw6gui_menu_remove (sys_context, c_menu, c_position, lw6sys_get_timestamp (sys_context)) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_menu_remove_all (SCM menu, SCM position)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_menu_t *c_menu = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.menu, menu), menu, SCM_ARG1, __FUNCTION__);

  c_menu = lw6_scm_to_menu (sys_context, menu);
  ret = lw6gui_menu_remove_all (sys_context, c_menu, lw6sys_get_timestamp (sys_context)) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_menu_sync (SCM menu, SCM menuitem)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  int c_id = 0;
  char *c_label = NULL;
  char *c_tooltip = NULL;
  int c_value = 0;
  int c_enabled = 0;
  int c_selected = 0;
  int c_colored = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.menu, menu), menu, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_CONSP (menuitem) || menuitem == SCM_EOL, menuitem, SCM_ARG2, __FUNCTION__);

  c_label = lw6scm_utils_to_0str (sys_context, scm_assoc_ref (menuitem, scm_from_locale_string ("label")));
  if (c_label)
    {
      c_tooltip = lw6scm_utils_to_0str (sys_context, scm_assoc_ref (menuitem, scm_from_locale_string ("tooltip")));
      if (c_tooltip)
	{
	  lw6gui_menu_t *c_menu;

	  c_id = scm_to_int (scm_assoc_ref (menuitem, scm_from_locale_string ("id")));
	  c_value = scm_to_int (scm_assoc_ref (menuitem, scm_from_locale_string ("value")));
	  c_enabled = scm_to_bool (scm_assoc_ref (menuitem, scm_from_locale_string ("enabled")));
	  c_selected = scm_to_bool (scm_assoc_ref (menuitem, scm_from_locale_string ("selected")));
	  c_colored = scm_to_bool (scm_assoc_ref (menuitem, scm_from_locale_string ("colored")));

	  c_menu = lw6_scm_to_menu (sys_context, menu);

	  lw6gui_menu_sync_using_id (sys_context, c_menu, c_id, c_label, c_tooltip, c_value, c_enabled, c_selected, c_colored,
				     lw6sys_get_timestamp (sys_context));

	  LW6SYS_FREE (sys_context, c_tooltip);
	}
      LW6SYS_FREE (sys_context, c_label);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_menu_select (SCM menu, SCM position, SCM allow_scroll)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_menu_t *c_menu = NULL;
  int c_position = 0;
  int c_allow_scroll = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.menu, menu), menu, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (position), position, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (SCM_BOOLP (allow_scroll), allow_scroll, SCM_ARG3, __FUNCTION__);

  c_menu = lw6_scm_to_menu (sys_context, menu);
  c_position = scm_to_int (position);
  c_allow_scroll = SCM_NFALSEP (allow_scroll);

  ret = lw6gui_menu_select (sys_context, c_menu, c_position, c_allow_scroll, lw6sys_get_timestamp (sys_context)) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_menu_select_esc (SCM menu, SCM state)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_menu_t *c_menu = NULL;
  int c_state = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.menu, menu), menu, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_BOOLP (state), state, SCM_ARG2, __FUNCTION__);

  c_menu = lw6_scm_to_menu (sys_context, menu);
  c_state = SCM_NFALSEP (state);

  lw6gui_menu_select_esc (sys_context, c_menu, c_state, lw6sys_get_timestamp (sys_context));

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6gui_menu_enable_esc (SCM menu, SCM state)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_menu_t *c_menu = NULL;
  int c_state = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.menu, menu), menu, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_BOOLP (state), state, SCM_ARG2, __FUNCTION__);

  c_menu = lw6_scm_to_menu (sys_context, menu);
  c_state = SCM_NFALSEP (state);

  lw6gui_menu_enable_esc (sys_context, c_menu, c_state, lw6sys_get_timestamp (sys_context));

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6gui_menu_scroll_up (SCM menu)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_menu_t *c_menu = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.menu, menu), menu, SCM_ARG1, __FUNCTION__);

  c_menu = lw6_scm_to_menu (sys_context, menu);

  ret = lw6gui_menu_scroll_up (sys_context, c_menu) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_menu_scroll_down (SCM menu)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_menu_t *c_menu = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.menu, menu), menu, SCM_ARG1, __FUNCTION__);

  c_menu = lw6_scm_to_menu (sys_context, menu);

  ret = lw6gui_menu_scroll_down (sys_context, c_menu) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_menu_set_breadcrumbs (SCM menu, SCM breadcrumbs)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_menu_t *c_menu = NULL;
  lw6sys_list_t *c_breadcrumbs = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.menu, menu), menu, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (SCM_CONSP (breadcrumbs) || breadcrumbs == SCM_EOL, breadcrumbs, SCM_ARG2, __FUNCTION__);

  c_menu = lw6_scm_to_menu (sys_context, menu);
  c_breadcrumbs = lw6scm_utils_to_sys_str_list (sys_context, breadcrumbs);
  if (c_breadcrumbs)
    {
      lw6gui_menu_set_breadcrumbs (sys_context, c_menu, c_breadcrumbs);
      lw6sys_list_free (sys_context, c_breadcrumbs);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_menu_close_popup (SCM menu)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_menu_t *c_menu = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.menu, menu), menu, SCM_ARG1, __FUNCTION__);
  c_menu = lw6_scm_to_menu (sys_context, menu);
  lw6gui_menu_close_popup (sys_context, c_menu);

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_BOOL_F;
}

static SCM
_scm_lw6gui_menu_has_popup (SCM menu)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_menu_t *c_menu = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.menu, menu), menu, SCM_ARG1, __FUNCTION__);
  c_menu = lw6_scm_to_menu (sys_context, menu);
  ret = lw6gui_menu_has_popup (sys_context, c_menu) ? SCM_BOOL_T : SCM_BOOL_F;

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_default_look ()
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_look_t *c_look = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  c_look = lw6gui_look_new (sys_context, NULL);
  if (c_look)
    {
      ret = lw6_make_scm_look (sys_context, c_look);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_look_set (SCM look, SCM key, SCM value)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_look_t *c_look = NULL;
  char *c_key = NULL;
  char *c_value = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.look, look), look, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (key), key, SCM_ARG2, __FUNCTION__);
  SCM_ASSERT (scm_is_string (value), value, SCM_ARG3, __FUNCTION__);

  c_look = lw6_scm_to_look (sys_context, look);
  if (c_look)
    {
      c_key = lw6scm_utils_to_0str (sys_context, key);
      if (c_key)
	{
	  c_value = lw6scm_utils_to_0str (sys_context, value);
	  if (c_value)
	    {
	      ret = lw6gui_look_set (sys_context, c_look, c_key, c_value) ? SCM_BOOL_T : SCM_BOOL_F;
	      LW6SYS_FREE (sys_context, c_value);
	    }
	  LW6SYS_FREE (sys_context, c_key);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_look_get (SCM look, SCM key)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_look_t *c_look = NULL;
  char *c_key = NULL;
  char *c_value = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.look, look), look, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_string (key), key, SCM_ARG2, __FUNCTION__);

  c_look = lw6_scm_to_look (sys_context, look);
  if (c_look)
    {
      c_key = lw6scm_utils_to_0str (sys_context, key);
      if (c_key)
	{
	  c_value = lw6gui_look_get (sys_context, c_look, c_key);
	  if (c_value)
	    {
	      ret = scm_from_locale_string (c_value);
	      LW6SYS_FREE (sys_context, c_value);
	    }
	  LW6SYS_FREE (sys_context, c_key);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_look_zoom_in (SCM look, SCM zoom_step)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_look_t *c_look = NULL;
  float c_zoom_step = 0.0f;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.look, look), look, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_number (zoom_step), zoom_step, SCM_ARG2, __FUNCTION__);

  c_look = lw6_scm_to_look (sys_context, look);
  if (c_look)
    {
      c_zoom_step = scm_to_double (zoom_step);
      if (lw6gui_look_zoom_in (sys_context, c_look, c_zoom_step))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_look_zoom_out (SCM look, SCM zoom_step)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_look_t *c_look = NULL;
  float c_zoom_step = 0.0f;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.look, look), look, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_number (zoom_step), zoom_step, SCM_ARG2, __FUNCTION__);

  c_look = lw6_scm_to_look (sys_context, look);
  if (c_look)
    {
      c_zoom_step = scm_to_double (zoom_step);
      if (lw6gui_look_zoom_out (sys_context, c_look, c_zoom_step))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_input_reset (SCM dsp)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      lw6gui_input_reset (sys_context, c_dsp->input);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return SCM_UNDEFINED;
}

static SCM
_scm_lw6gui_mouse_poll_move (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_mouse_poll_move (sys_context, &(c_dsp->input->mouse), NULL, NULL))
	{
	  ret = _mouse_get_state (sys_context, c_dsp);
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_mouse_get_state (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      ret = _mouse_get_state (sys_context, c_dsp);
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_keyboard_is_pressed (SCM dsp, SCM keysym)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  SCM ret = SCM_BOOL_F;
  lw6dsp_backend_t *c_dsp = NULL;
  int c_keysym = 0;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);
  SCM_ASSERT (scm_is_integer (keysym), keysym, SCM_ARG2, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      c_keysym = scm_to_int (keysym);

      ret = lw6gui_keyboard_is_pressed (sys_context, &(c_dsp->input->keyboard), c_keysym) ? SCM_BOOL_T : SCM_BOOL_F;
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_keyboard_get_move_pad (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_move_pad_t move_pad;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      lw6gui_keyboard_get_move_pad (sys_context, &(c_dsp->input->keyboard), &move_pad);
      ret = scm_list_4 (scm_cons
			(scm_from_locale_string ("up"),
			 move_pad.up ? SCM_BOOL_T : SCM_BOOL_F),
			scm_cons
			(scm_from_locale_string ("down"),
			 move_pad.down ? SCM_BOOL_T : SCM_BOOL_F),
			scm_cons
			(scm_from_locale_string ("left"),
			 move_pad.left ? SCM_BOOL_T : SCM_BOOL_F), scm_cons (scm_from_locale_string ("right"), move_pad.right ? SCM_BOOL_T : SCM_BOOL_F));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick1_get_move_pad (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_move_pad_t move_pad;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      lw6gui_joystick_get_move_pad (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK1_ID]), &move_pad);
      ret =
	scm_list_4 (scm_cons
		    (scm_from_locale_string ("up"),
		     move_pad.up ? SCM_BOOL_T : SCM_BOOL_F),
		    scm_cons (scm_from_locale_string ("down"),
			      move_pad.down ? SCM_BOOL_T : SCM_BOOL_F),
		    scm_cons (scm_from_locale_string ("left"),
			      move_pad.left ? SCM_BOOL_T : SCM_BOOL_F), scm_cons (scm_from_locale_string ("right"), move_pad.right ? SCM_BOOL_T : SCM_BOOL_F));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick2_get_move_pad (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;
  lw6gui_move_pad_t move_pad;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      lw6gui_joystick_get_move_pad (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK2_ID]), &move_pad);
      ret =
	scm_list_4 (scm_cons
		    (scm_from_locale_string ("up"),
		     move_pad.up ? SCM_BOOL_T : SCM_BOOL_F),
		    scm_cons (scm_from_locale_string ("down"),
			      move_pad.down ? SCM_BOOL_T : SCM_BOOL_F),
		    scm_cons (scm_from_locale_string ("left"),
			      move_pad.left ? SCM_BOOL_T : SCM_BOOL_F), scm_cons (scm_from_locale_string ("right"), move_pad.right ? SCM_BOOL_T : SCM_BOOL_F));
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_mouse_pop_button_left (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->mouse.button_left)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_mouse_pop_button_right (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->mouse.button_right)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_mouse_pop_button_middle (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->mouse.button_middle)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_mouse_pop_simple_click (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_simple_click (sys_context, &(c_dsp->input->mouse.button_left))
	  || lw6gui_button_pop_simple_click (sys_context, &(c_dsp->input->mouse.button_right))
	  || lw6gui_button_pop_simple_click (sys_context, &(c_dsp->input->mouse.button_middle)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_mouse_pop_double_click (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_double_click (sys_context, &(c_dsp->input->mouse.button_left))
	  || lw6gui_button_pop_double_click (sys_context, &(c_dsp->input->mouse.button_right))
	  || lw6gui_button_pop_double_click (sys_context, &(c_dsp->input->mouse.button_middle)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_mouse_pop_triple_click (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_triple_click (sys_context, &(c_dsp->input->mouse.button_left))
	  || lw6gui_button_pop_triple_click (sys_context, &(c_dsp->input->mouse.button_right))
	  || lw6gui_button_pop_triple_click (sys_context, &(c_dsp->input->mouse.button_middle)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_mouse_pop_wheel_up (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->mouse.wheel_up)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_mouse_pop_wheel_down (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->mouse.wheel_down)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_keyboard_pop_arrow_up (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->keyboard.arrow_up)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_keyboard_pop_arrow_down (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->keyboard.arrow_down)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_keyboard_pop_arrow_left (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->keyboard.arrow_left)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_keyboard_pop_arrow_right (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->keyboard.arrow_right)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_keyboard_pop_key_enter (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->keyboard.key_enter)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_keyboard_pop_key_esc (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->keyboard.key_esc)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_keyboard_pop_key_ctrl (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->keyboard.key_ctrl)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_keyboard_pop_key_alt (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->keyboard.key_alt)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_keyboard_pop_key_pgup (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->keyboard.key_pgup)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_keyboard_pop_key_pgdown (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->keyboard.key_pgdown)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick1_pop_pad_up (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK1_ID].pad_up)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick1_pop_pad_down (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK1_ID].pad_down)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick1_pop_pad_left (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK1_ID].pad_left)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick1_pop_pad_right (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK1_ID].pad_right)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick1_pop_button_a (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK1_ID].button_a)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick1_pop_button_b (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK1_ID].button_b)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick1_pop_button_c (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK1_ID].button_c)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick1_pop_button_d (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK1_ID].button_d)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick1_pop_button_e (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK1_ID].button_e)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick1_pop_button_f (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK1_ID].button_f)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick2_pop_pad_up (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK2_ID].pad_up)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick2_pop_pad_down (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK2_ID].pad_down)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick2_pop_pad_left (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK2_ID].pad_left)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick2_pop_pad_right (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK2_ID].pad_right)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick2_pop_button_a (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK2_ID].button_a)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick2_pop_button_b (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK2_ID].button_b)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick2_pop_button_c (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK2_ID].button_c)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick2_pop_button_d (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK2_ID].button_d)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick2_pop_button_e (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK2_ID].button_e)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

static SCM
_scm_lw6gui_joystick2_pop_button_f (SCM dsp)
{
  SCM ret = SCM_BOOL_F;
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = NULL;

  LW6SYS_SCRIPT_FUNCTION_BEGIN;
  lw6scm_coverage_call (sys_context, lw6_global.coverage, __FUNCTION__);

  SCM_ASSERT (SCM_SMOB_PREDICATE (lw6_global.smob_types.dsp, dsp), dsp, SCM_ARG1, __FUNCTION__);

  c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  if (c_dsp)
    {
      if (lw6gui_button_pop_press (sys_context, &(c_dsp->input->joysticks[LW6GUI_JOYSTICK2_ID].button_f)))
	{
	  ret = SCM_BOOL_T;
	}
    }

  LW6SYS_SCRIPT_FUNCTION_END;

  return ret;
}

/**
 * lw6_register_funcs_gui
 *
 * @sys_context: global system context
 *
 * Register the functions of the gui module, make them callable from Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_funcs_gui (lw6sys_context_t * sys_context)
{
  int ret = 1;

  /*
   * In liquidwar6gui
   */
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MENU_NEW, 5, 0, 0, (SCM (*)())_scm_lw6gui_menu_new);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MENU_APPEND, 2, 0, 0, (SCM (*)())_scm_lw6gui_menu_append);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MENU_REMOVE, 2, 0, 0, (SCM (*)())_scm_lw6gui_menu_remove);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MENU_REMOVE_ALL, 1, 0, 0, (SCM (*)())_scm_lw6gui_menu_remove_all);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MENU_SYNC, 2, 0, 0, (SCM (*)())_scm_lw6gui_menu_sync);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MENU_SELECT, 3, 0, 0, (SCM (*)())_scm_lw6gui_menu_select);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MENU_SELECT_ESC, 2, 0, 0, (SCM (*)())_scm_lw6gui_menu_select_esc);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MENU_ENABLE_ESC, 2, 0, 0, (SCM (*)())_scm_lw6gui_menu_enable_esc);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MENU_SCROLL_UP, 1, 0, 0, (SCM (*)())_scm_lw6gui_menu_scroll_up);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MENU_SCROLL_DOWN, 1, 0, 0, (SCM (*)())_scm_lw6gui_menu_scroll_down);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MENU_SET_BREADCRUMBS, 2, 0, 0, (SCM (*)())_scm_lw6gui_menu_set_breadcrumbs);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MENU_CLOSE_POPUP, 1, 0, 0, (SCM (*)())_scm_lw6gui_menu_close_popup);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MENU_HAS_POPUP, 1, 0, 0, (SCM (*)())_scm_lw6gui_menu_has_popup);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_DEFAULT_LOOK, 0, 0, 0, (SCM (*)())_scm_lw6gui_default_look);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_LOOK_SET, 3, 0, 0, (SCM (*)())_scm_lw6gui_look_set);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_LOOK_GET, 2, 0, 0, (SCM (*)())_scm_lw6gui_look_get);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_LOOK_ZOOM_IN, 2, 0, 0, (SCM (*)())_scm_lw6gui_look_zoom_in);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_LOOK_ZOOM_OUT, 2, 0, 0, (SCM (*)())_scm_lw6gui_look_zoom_out);

  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_INPUT_RESET, 1, 0, 0, (SCM (*)())_scm_lw6gui_input_reset);

  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MOUSE_POLL_MOVE, 1, 0, 0, (SCM (*)())_scm_lw6gui_mouse_poll_move);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MOUSE_GET_STATE, 1, 0, 0, (SCM (*)())_scm_lw6gui_mouse_get_state);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_KEYBOARD_IS_PRESSED, 1, 0, 0, (SCM (*)())_scm_lw6gui_keyboard_is_pressed);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_KEYBOARD_GET_MOVE_PAD, 1, 0, 0, (SCM (*)())_scm_lw6gui_keyboard_get_move_pad);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK1_GET_MOVE_PAD, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick1_get_move_pad);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK2_GET_MOVE_PAD, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick2_get_move_pad);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MOUSE_POP_BUTTON_LEFT, 1, 0, 0, (SCM (*)())_scm_lw6gui_mouse_pop_button_left);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MOUSE_POP_BUTTON_RIGHT, 1, 0, 0, (SCM (*)())_scm_lw6gui_mouse_pop_button_right);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MOUSE_POP_BUTTON_MIDDLE, 1, 0, 0, (SCM (*)())_scm_lw6gui_mouse_pop_button_middle);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MOUSE_POP_SIMPLE_CLICK, 1, 0, 0, (SCM (*)())_scm_lw6gui_mouse_pop_simple_click);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MOUSE_POP_DOUBLE_CLICK, 1, 0, 0, (SCM (*)())_scm_lw6gui_mouse_pop_double_click);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MOUSE_POP_TRIPLE_CLICK, 1, 0, 0, (SCM (*)())_scm_lw6gui_mouse_pop_triple_click);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MOUSE_POP_WHEEL_UP, 1, 0, 0, (SCM (*)())_scm_lw6gui_mouse_pop_wheel_up);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_MOUSE_POP_WHEEL_DOWN, 1, 0, 0, (SCM (*)())_scm_lw6gui_mouse_pop_wheel_down);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_KEYBOARD_POP_ARROW_UP, 1, 0, 0, (SCM (*)())_scm_lw6gui_keyboard_pop_arrow_up);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_KEYBOARD_POP_ARROW_DOWN, 1, 0, 0, (SCM (*)())_scm_lw6gui_keyboard_pop_arrow_down);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_KEYBOARD_POP_ARROW_LEFT, 1, 0, 0, (SCM (*)())_scm_lw6gui_keyboard_pop_arrow_left);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_KEYBOARD_POP_ARROW_RIGHT, 1, 0, 0, (SCM (*)())_scm_lw6gui_keyboard_pop_arrow_right);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_KEYBOARD_POP_KEY_ENTER, 1, 0, 0, (SCM (*)())_scm_lw6gui_keyboard_pop_key_enter);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_KEYBOARD_POP_KEY_ESC, 1, 0, 0, (SCM (*)())_scm_lw6gui_keyboard_pop_key_esc);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_KEYBOARD_POP_KEY_CTRL, 1, 0, 0, (SCM (*)())_scm_lw6gui_keyboard_pop_key_ctrl);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_KEYBOARD_POP_KEY_ALT, 1, 0, 0, (SCM (*)())_scm_lw6gui_keyboard_pop_key_alt);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_KEYBOARD_POP_KEY_PGUP, 1, 0, 0, (SCM (*)())_scm_lw6gui_keyboard_pop_key_pgup);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_KEYBOARD_POP_KEY_PGDOWN, 1, 0, 0, (SCM (*)())_scm_lw6gui_keyboard_pop_key_pgdown);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK1_POP_PAD_UP, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick1_pop_pad_up);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK1_POP_PAD_DOWN, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick1_pop_pad_down);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK1_POP_PAD_LEFT, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick1_pop_pad_left);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK1_POP_PAD_RIGHT, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick1_pop_pad_right);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK1_POP_BUTTON_A, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick1_pop_button_a);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK1_POP_BUTTON_B, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick1_pop_button_b);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK1_POP_BUTTON_C, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick1_pop_button_c);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK1_POP_BUTTON_D, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick1_pop_button_d);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK1_POP_BUTTON_E, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick1_pop_button_e);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK1_POP_BUTTON_F, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick1_pop_button_f);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK2_POP_PAD_UP, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick2_pop_pad_up);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK2_POP_PAD_DOWN, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick2_pop_pad_down);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK2_POP_PAD_LEFT, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick2_pop_pad_left);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK2_POP_PAD_RIGHT, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick2_pop_pad_right);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK2_POP_BUTTON_A, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick2_pop_button_a);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK2_POP_BUTTON_B, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick2_pop_button_b);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK2_POP_BUTTON_C, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick2_pop_button_c);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK2_POP_BUTTON_D, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick2_pop_button_d);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK2_POP_BUTTON_E, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick2_pop_button_e);
  ret = ret && lw6scm_c_define_gsubr (sys_context, LW6DEF_C_LW6GUI_JOYSTICK2_POP_BUTTON_F, 1, 0, 0, (SCM (*)())_scm_lw6gui_joystick2_pop_button_f);

  return ret;
}
