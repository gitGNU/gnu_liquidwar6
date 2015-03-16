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

#define SMOB_TYPE_DSP "lw6dsp-backend"
#define SMOB_TYPE_SND "lw6snd-backend"
#define SMOB_TYPE_MAP "lw6map-level"
#define SMOB_TYPE_MENU "lw6gui-menu"
#define SMOB_TYPE_GAME_STRUCT "lw6ker-game-struct"
#define SMOB_TYPE_GAME_STATE "lw6ker-game-state"
#define SMOB_TYPE_PILOT "lw6pil-pilot"
#define SMOB_TYPE_BOT "lw6bot-backend"
#define SMOB_TYPE_LOOK "lw6gui-look"
#define SMOB_TYPE_LOADER "lw6tsk-loader"
#define SMOB_TYPE_DB "lw6p2p-db"
#define SMOB_TYPE_NODE "lw6p2p-node"
#define SMOB_TYPE_JPEG "lw6img-jpeg"

static char *
_smob_id (lw6sys_context_t * sys_context, char *type, int id)
{
  char *ret = NULL;

  ret = lw6sys_new_sprintf (sys_context, "%s %d", type, id);

  return ret;
}

/*
 * Display smob
 */
static SCM
_mark_dsp (SCM dsp)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6_dsp_smob_t *dsp_smob = (lw6_dsp_smob_t *) SCM_SMOB_DATA (dsp);

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mark dsp smob"));

  scm_gc_mark (dsp_smob->level);
  scm_gc_mark (dsp_smob->game_struct);
  scm_gc_mark (dsp_smob->game_state);
  scm_gc_mark (dsp_smob->pilot);

  return SCM_BOOL_F;
}

static size_t
_free_dsp (SCM dsp)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *id = NULL;
  lw6_dsp_smob_t *dsp_smob = (lw6_dsp_smob_t *) SCM_SMOB_DATA (dsp);

  LW6_MUTEX_LOCK;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("garbage collect dsp smob"));
  id = _smob_id (sys_context, SMOB_TYPE_DSP, dsp_smob->c_dsp->id);
  if (id)
    {
      if (lw6_global.dsp_smobs)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("request free dsp smob \"%s\""), id);
	  lw6sys_assoc_unset (sys_context, lw6_global.dsp_smobs, id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("request free dsp smob \"%s\" but assoc is NULL"), id);
	}
      LW6SYS_FREE (sys_context, id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
_print_dsp (SCM dsp, SCM port, scm_print_state * pstate)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6dsp_backend_t *c_dsp = lw6_scm_to_dsp (sys_context, dsp);
  char *repr = NULL;

  repr = lw6dsp_repr (sys_context, c_dsp);

  scm_puts ("#<" SMOB_TYPE_DSP " ", port);
  if (repr)
    {
      scm_display (scm_from_locale_string (repr), port);
      LW6SYS_FREE (sys_context, repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_dsp
 *
 * @sys_context: global system context
 * @c_dsp: the display object
 *
 * Creates an SCM 'dsp' object from C data.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_dsp (lw6sys_context_t * sys_context, lw6dsp_backend_t * c_dsp)
{
  // c_dsp is supposed to have been initialized with lw6dsp_init()
  char *repr = NULL;
  char *id = NULL;
  lw6_dsp_smob_t *dsp_smob = NULL;

  scm_gc ();

  dsp_smob = (lw6_dsp_smob_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6_dsp_smob_t));
  if (dsp_smob)
    {
      dsp_smob->c_dsp = c_dsp;
      dsp_smob->level = SCM_BOOL_F;	// initialized later with update
      dsp_smob->game_struct = SCM_BOOL_F;	// initialized later with update
      dsp_smob->game_state = SCM_BOOL_F;	// initialized later with update
      dsp_smob->pilot = SCM_BOOL_F;	// initialized later with update
      id = _smob_id (sys_context, SMOB_TYPE_DSP, c_dsp->id);
      if (id)
	{
	  repr = lw6dsp_repr (sys_context, c_dsp);
	  if (repr)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("creating dsp smob \"%s\""), repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (sys_context, &lw6_global.dsp_smobs, id, (void *) dsp_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (sys_context, repr);
	    }
	  LW6SYS_FREE (sys_context, id);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create dsp smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.dsp, dsp_smob);
}

/**
 * lw6_scm_to_dsp
 *
 * @sys_context: global system context
 * @dsp: the dsp to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'dsp' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6dsp_backend_t *
lw6_scm_to_dsp (lw6sys_context_t * sys_context, SCM dsp)
{
  lw6dsp_backend_t *c_dsp;

  c_dsp = ((lw6_dsp_smob_t *) SCM_SMOB_DATA (dsp))->c_dsp;

  return c_dsp;
}

/**
 * lw6_free_dsp_smob
 *
 * @sys_context: global system context
 * @dsp_smob: the smob to free
 *
 * Frees a dsp smob, we need a special function to do
 * that as structures like assoc hold pointers to these
 * objects and therefore need a proper callback when being
 * destroyed.
 *
 * Return value: none
 */
void
lw6_free_dsp_smob (lw6sys_context_t * sys_context, lw6_dsp_smob_t * dsp_smob)
{
  char *repr = NULL;

  repr = lw6dsp_repr (sys_context, dsp_smob->c_dsp);
  if (repr)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing dsp smob \"%s\""), repr);
      LW6SYS_FREE (sys_context, repr);
    }

  lw6dsp_quit (sys_context, dsp_smob->c_dsp);
  lw6dsp_destroy_backend (sys_context, dsp_smob->c_dsp);
  LW6SYS_FREE (sys_context, dsp_smob);
}

/*
 * Sound smob
 */
static SCM
_mark_snd (SCM snd)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mark snd smob"));

  return SCM_BOOL_F;
}

static size_t
_free_snd (SCM snd)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *id = NULL;
  lw6_snd_smob_t *snd_smob = (lw6_snd_smob_t *) SCM_SMOB_DATA (snd);

  LW6_MUTEX_LOCK;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("garbage collect snd smob"));
  id = _smob_id (sys_context, SMOB_TYPE_SND, snd_smob->c_snd->id);
  if (id)
    {
      if (lw6_global.snd_smobs)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("request free snd smob \"%s\""), id);
	  lw6sys_assoc_unset (sys_context, lw6_global.snd_smobs, id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("request free snd smob \"%s\" but assoc is NULL"), id);
	}
      LW6SYS_FREE (sys_context, id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
_print_snd (SCM snd, SCM port, scm_print_state * pstate)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6snd_backend_t *c_snd = lw6_scm_to_snd (sys_context, snd);
  char *repr = NULL;

  repr = lw6snd_repr (sys_context, c_snd);

  scm_puts ("#<" SMOB_TYPE_SND " ", port);
  if (repr)
    {
      scm_display (scm_from_locale_string (repr), port);
      LW6SYS_FREE (sys_context, repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_snd
 *
 * @sys_context: global system context
 * @c_snd: the sound object
 *
 * Creates an SCM 'snd' object from C data.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_snd (lw6sys_context_t * sys_context, lw6snd_backend_t * c_snd)
{
// c_snd is supposed to have been initialized with lw6snd_init()
  char *repr = NULL;
  char *id = NULL;
  lw6_snd_smob_t *snd_smob = NULL;

  scm_gc ();

  snd_smob = (lw6_snd_smob_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6_snd_smob_t));
  if (snd_smob)
    {
      snd_smob->c_snd = c_snd;
      id = _smob_id (sys_context, SMOB_TYPE_SND, c_snd->id);
      if (id)
	{
	  repr = lw6snd_repr (sys_context, c_snd);
	  if (repr)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("creating snd smob \"%s\""), repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (sys_context, &lw6_global.snd_smobs, id, (void *) snd_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (sys_context, repr);
	    }
	  LW6SYS_FREE (sys_context, id);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create snd smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.snd, snd_smob);
}

/**
 * lw6_scm_to_snd
 *
 * @sys_context: global system context
 * @snd: the snd to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'snd' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6snd_backend_t *
lw6_scm_to_snd (lw6sys_context_t * sys_context, SCM snd)
{
  lw6snd_backend_t *c_snd;

  c_snd = ((lw6_snd_smob_t *) SCM_SMOB_DATA (snd))->c_snd;

  return c_snd;
}

/**
 * lw6_free_snd_smob
 *
 * @sys_context: global system context
 * @snd_smob: the smob to free
 *
 * Frees a snd smob, we need a special function to do
 * that as structures like assoc hold pointers to these
 * objects and therefore need a proper callback when being
 * destroyed.
 *
 * Return value: none
 */
void
lw6_free_snd_smob (lw6sys_context_t * sys_context, lw6_snd_smob_t * snd_smob)
{
  char *repr = NULL;

  repr = lw6snd_repr (sys_context, snd_smob->c_snd);
  if (repr)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing snd smob \"%s\""), repr);
      LW6SYS_FREE (sys_context, repr);
    }

  lw6snd_quit (sys_context, snd_smob->c_snd);
  lw6snd_destroy_backend (sys_context, snd_smob->c_snd);
  LW6SYS_FREE (sys_context, snd_smob);
}

/*
 * Map smob
 */
static SCM
_mark_map (SCM map)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mark map smob"));

  return SCM_BOOL_F;
}

static size_t
_free_map (SCM map)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *id = NULL;
  lw6_map_smob_t *map_smob = (lw6_map_smob_t *) SCM_SMOB_DATA (map);

  LW6_MUTEX_LOCK;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("garbage collect map smob"));
  id = _smob_id (sys_context, SMOB_TYPE_MAP, map_smob->c_map->id);
  if (id)
    {
      if (lw6_global.map_smobs)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("request free map smob \"%s\""), id);
	  lw6sys_assoc_unset (sys_context, lw6_global.map_smobs, id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("request free map smob \"%s\" but assoc is NULL"), id);
	}
      LW6SYS_FREE (sys_context, id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
_print_map (SCM map, SCM port, scm_print_state * pstate)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6map_level_t *c_map = lw6_scm_to_map (sys_context, map);
  char *repr = NULL;

  repr = lw6map_repr (sys_context, c_map);

  scm_puts ("#<" SMOB_TYPE_MAP " ", port);
  if (repr)
    {
      scm_display (scm_from_locale_string (repr), port);
      LW6SYS_FREE (sys_context, repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_map
 *
 * @sys_context: global system context
 * @c_map: the map object
 *
 * Creates an SCM 'map' object from C data.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_map (lw6sys_context_t * sys_context, lw6map_level_t * c_map)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_map_smob_t *map_smob = NULL;

  /*
   * Calling the garbage collector, the idea is that when we
   * create a map object, we're doing something rare, and
   * we're not playing, so there's no harm in garbage collecting.
   * Additionnally, this will free memory for new map processing.
   * Ideally we would garbage collect just after affecting the
   * new map, or just before allocating memory for it. Putting
   * a garbage collection here does not harm anyway.
   */
  scm_gc ();

  map_smob = (lw6_map_smob_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6_map_smob_t));
  if (map_smob)
    {
      map_smob->c_map = c_map;
      id = _smob_id (sys_context, SMOB_TYPE_MAP, c_map->id);
      if (id)
	{
	  repr = lw6map_repr (sys_context, c_map);
	  if (repr)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("creating map smob \"%s\""), repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (sys_context, &lw6_global.map_smobs, id, (void *) map_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (sys_context, repr);
	    }
	  LW6SYS_FREE (sys_context, id);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create map smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.map, map_smob);
}

/**
 * lw6_scm_to_map
 *
 * @sys_context: global system context
 * @map: the map to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'map' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6map_level_t *
lw6_scm_to_map (lw6sys_context_t * sys_context, SCM map)
{
  lw6map_level_t *c_map;

  c_map = ((lw6_map_smob_t *) SCM_SMOB_DATA (map))->c_map;

  return c_map;
}

/**
 * lw6_free_map_smob
 *
 * @sys_context: global system context
 * @map_smob: the smob to free
 *
 * Frees a map smob, we need a special function to do
 * that as structures like assoc hold pointers to these
 * objects and therefore need a proper callback when being
 * destroyed.
 *
 * Return value: none
 */
void
lw6_free_map_smob (lw6sys_context_t * sys_context, lw6_map_smob_t * map_smob)
{
  char *repr = NULL;

  repr = lw6map_repr (sys_context, map_smob->c_map);
  if (repr)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing map smob \"%s\""), repr);
      LW6SYS_FREE (sys_context, repr);
    }

  lw6map_free (sys_context, map_smob->c_map);
  LW6SYS_FREE (sys_context, map_smob);
}

/*
 * Menu smob
 */
static SCM
_mark_menu (SCM menu)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mark menu smob"));

  return SCM_BOOL_F;
}

static size_t
_free_menu (SCM menu)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *id = NULL;
  lw6_menu_smob_t *menu_smob = (lw6_menu_smob_t *) SCM_SMOB_DATA (menu);

  LW6_MUTEX_LOCK;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("garbage collect menu smob"));
  id = _smob_id (sys_context, SMOB_TYPE_MENU, menu_smob->c_menu->id);
  if (id)
    {
      if (lw6_global.menu_smobs)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("request free menu smob \"%s\""), id);
	  lw6sys_assoc_unset (sys_context, lw6_global.menu_smobs, id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("request free menu smob \"%s\" but assoc is NULL"), id);
	}
      LW6SYS_FREE (sys_context, id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
_print_menu (SCM menu, SCM port, scm_print_state * pstate)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_menu_t *c_menu = lw6_scm_to_menu (sys_context, menu);
  char *repr = NULL;

  repr = lw6gui_menu_repr (sys_context, c_menu);

  scm_puts ("#<" SMOB_TYPE_MENU " ", port);
  if (repr)
    {
      scm_display (scm_from_locale_string (repr), port);
      LW6SYS_FREE (sys_context, repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_menu
 *
 * @sys_context: global system context
 * @c_menu: the menu object
 *
 * Creates an SCM 'menu' object from C data.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_menu (lw6sys_context_t * sys_context, lw6gui_menu_t * c_menu)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_menu_smob_t *menu_smob = NULL;

  scm_gc ();

  menu_smob = (lw6_menu_smob_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6_menu_smob_t));
  if (menu_smob)
    {
      menu_smob->c_menu = c_menu;
      id = _smob_id (sys_context, SMOB_TYPE_MENU, c_menu->id);
      if (id)
	{
	  repr = lw6gui_menu_repr (sys_context, c_menu);
	  if (repr)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("creating menu smob \"%s\""), repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (sys_context, &lw6_global.menu_smobs, id, (void *) menu_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (sys_context, repr);
	    }
	  LW6SYS_FREE (sys_context, id);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create menu smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.menu, menu_smob);
}

/**
 * lw6_scm_to_menu
 *
 * @sys_context: global system context
 * @menu: the menu to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'menu' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6gui_menu_t *
lw6_scm_to_menu (lw6sys_context_t * sys_context, SCM menu)
{
  lw6gui_menu_t *c_menu;

  c_menu = ((lw6_menu_smob_t *) SCM_SMOB_DATA (menu))->c_menu;

  return c_menu;
}

/**
 * lw6_free_menu_smob
 *
 * @sys_context: global system context
 * @menu_smob: the smob to free
 *
 * Frees a menu smob, we need a special function to do
 * that as structures like assoc hold pointers to these
 * objects and therefore need a proper callback when being
 * destroyed.
 *
 * Return value: none
 */
void
lw6_free_menu_smob (lw6sys_context_t * sys_context, lw6_menu_smob_t * menu_smob)
{
  char *repr = NULL;

  repr = lw6gui_menu_repr (sys_context, menu_smob->c_menu);
  if (repr)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing menu smob \"%s\""), repr);
      LW6SYS_FREE (sys_context, repr);
    }

  lw6gui_menu_free (sys_context, menu_smob->c_menu);
  LW6SYS_FREE (sys_context, menu_smob);
}

/*
 * Game struct smob
 */
static SCM
_mark_game_struct (SCM game_struct)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6_game_struct_smob_t *game_struct_smob = (lw6_game_struct_smob_t *) SCM_SMOB_DATA (game_struct);

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mark game struct smob"));

  scm_gc_mark (game_struct_smob->map);

  return SCM_BOOL_F;
}

static size_t
_free_game_struct (SCM game_struct)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *id = NULL;
  lw6_game_struct_smob_t *game_struct_smob = (lw6_game_struct_smob_t *) SCM_SMOB_DATA (game_struct);

  LW6_MUTEX_LOCK;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("garbage collect game struct smob"));
  id = _smob_id (sys_context, SMOB_TYPE_GAME_STRUCT, game_struct_smob->c_game_struct->id);
  if (id)
    {
      if (lw6_global.game_struct_smobs)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("request free game struct smob \"%s\""), id);
	  lw6sys_assoc_unset (sys_context, lw6_global.game_struct_smobs, id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("request free game struct smob \"%s\" but assoc is NULL"), id);
	}
      LW6SYS_FREE (sys_context, id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
_print_game_struct (SCM game_struct, SCM port, scm_print_state * pstate)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6ker_game_struct_t *c_game_struct = lw6_scm_to_game_struct (sys_context, game_struct);
  char *repr = NULL;

  repr = lw6ker_game_struct_repr (sys_context, c_game_struct);

  scm_puts ("#<" SMOB_TYPE_GAME_STRUCT " ", port);
  if (repr)
    {
      scm_display (scm_from_locale_string (repr), port);
      LW6SYS_FREE (sys_context, repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_game_struct
 *
 * @sys_context: global system context
 * @c_game_struct: the game struct object
 * @map: the map (SCM object) referenced
 *
 * Creates an SCM 'game-struct' object from C data.
 * Passing the map object enables the garbage collector
 * not to free the map until the game struct is freed.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_game_struct (lw6sys_context_t * sys_context, lw6ker_game_struct_t * c_game_struct, SCM map)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_game_struct_smob_t *game_struct_smob = NULL;

  scm_gc ();

  game_struct_smob = (lw6_game_struct_smob_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6_game_struct_smob_t));
  if (game_struct_smob)
    {
      game_struct_smob->c_game_struct = c_game_struct;
      game_struct_smob->map = map;
      id = _smob_id (sys_context, SMOB_TYPE_GAME_STRUCT, c_game_struct->id);
      if (id)
	{
	  repr = lw6ker_game_struct_repr (sys_context, c_game_struct);
	  if (repr)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("creating game struct smob \"%s\""), repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (sys_context, &lw6_global.game_struct_smobs, id, (void *) game_struct_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (sys_context, repr);
	    }
	  LW6SYS_FREE (sys_context, id);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create game struct smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.game_struct, game_struct_smob);
}

/**
 * lw6_scm_to_game_struct
 *
 * @sys_context: global system context
 * @game_struct: the game_struct to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'game_struct' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6ker_game_struct_t *
lw6_scm_to_game_struct (lw6sys_context_t * sys_context, SCM game_struct)
{
  lw6ker_game_struct_t *c_game_struct;

  c_game_struct = ((lw6_game_struct_smob_t *) SCM_SMOB_DATA (game_struct))->c_game_struct;

  return c_game_struct;
}

/**
 * lw6_free_game_struct_smob
 *
 * @sys_context: global system context
 * @game_struct_smob: the smob to free
 *
 * Frees a game_struct smob, we need a special function to do
 * that as structures like assoc hold pointers to these
 * objects and therefore need a proper callback when being
 * destroyed.
 *
 * Return value: none
 */
void
lw6_free_game_struct_smob (lw6sys_context_t * sys_context, lw6_game_struct_smob_t * game_struct_smob)
{
  char *repr = NULL;

  repr = lw6ker_game_struct_repr (sys_context, game_struct_smob->c_game_struct);
  if (repr)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing game struct smob \"%s\""), repr);
      LW6SYS_FREE (sys_context, repr);
    }

  lw6ker_game_struct_free (sys_context, game_struct_smob->c_game_struct);
  LW6SYS_FREE (sys_context, game_struct_smob);
}

/*
 * Game state smob
 */
static SCM
_mark_game_state (SCM game_state)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6_game_state_smob_t *game_state_smob = (lw6_game_state_smob_t *) SCM_SMOB_DATA (game_state);

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mark game state smob"));

  scm_gc_mark (game_state_smob->game_struct);

  return SCM_BOOL_F;
}

static size_t
_free_game_state (SCM game_state)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *id = NULL;
  lw6_game_state_smob_t *game_state_smob = (lw6_game_state_smob_t *) SCM_SMOB_DATA (game_state);

  LW6_MUTEX_LOCK;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("garbage collect game state smob"));
  id = _smob_id (sys_context, SMOB_TYPE_GAME_STATE, game_state_smob->c_game_state->id);
  if (id)
    {
      if (lw6_global.game_state_smobs)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("request free game state smob \"%s\""), id);
	  lw6sys_assoc_unset (sys_context, lw6_global.game_state_smobs, id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("request free game state smob \"%s\" but assoc is NULL"), id);
	}
      LW6SYS_FREE (sys_context, id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
_print_game_state (SCM game_state, SCM port, scm_print_state * pstate)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6ker_game_state_t *c_game_state = lw6_scm_to_game_state (sys_context, game_state);
  char *repr = NULL;

  repr = lw6ker_game_state_repr (sys_context, c_game_state);

  scm_puts ("#<" SMOB_TYPE_GAME_STATE " ", port);
  if (repr)
    {
      scm_display (scm_from_locale_string (repr), port);
      LW6SYS_FREE (sys_context, repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_game_state
 *
 * @sys_context: global system context
 * @c_game_state: the game state object
 * @game_struct: the game struct (SCM object) referenced
 *
 * Creates an SCM 'game_state' object from C data.
 * Passing game_struct enables the garbage collector not
 * to free the game_struct until the game_state is freed.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_game_state (lw6sys_context_t * sys_context, lw6ker_game_state_t * c_game_state, SCM game_struct)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_game_state_smob_t *game_state_smob = NULL;

  scm_gc ();

  game_state_smob = (lw6_game_state_smob_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6_game_state_smob_t));
  if (game_state_smob)
    {
      game_state_smob->c_game_state = c_game_state;
      game_state_smob->game_struct = game_struct;
      id = _smob_id (sys_context, SMOB_TYPE_GAME_STATE, c_game_state->id);
      if (id)
	{
	  repr = lw6ker_game_state_repr (sys_context, c_game_state);
	  if (repr)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("creating game state smob \"%s\""), repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (sys_context, &lw6_global.game_state_smobs, id, (void *) game_state_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (sys_context, repr);
	    }
	  LW6SYS_FREE (sys_context, id);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create game state smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.game_state, game_state_smob);
}

/**
 * lw6_scm_to_game_state
 *
 * @sys_context: global system context
 * @game_state: the game_state to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'game_state' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6ker_game_state_t *
lw6_scm_to_game_state (lw6sys_context_t * sys_context, SCM game_state)
{
  lw6ker_game_state_t *c_game_state;

  c_game_state = ((lw6_game_state_smob_t *) SCM_SMOB_DATA (game_state))->c_game_state;

  return c_game_state;
}

/**
 * lw6_free_game_state_smob
 *
 * @sys_context: global system context
 * @game_state_smob: the smob to free
 *
 * Frees a game_state smob, we need a special function to do
 * that as structures like assoc hold pointers to these
 * objects and therefore need a proper callback when being
 * destroyed.
 *
 * Return value: none
 */
void
lw6_free_game_state_smob (lw6sys_context_t * sys_context, lw6_game_state_smob_t * game_state_smob)
{
  char *repr = NULL;

  repr = lw6ker_game_state_repr (sys_context, game_state_smob->c_game_state);
  if (repr)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing game state smob \"%s\""), repr);
      LW6SYS_FREE (sys_context, repr);
    }

  lw6ker_game_state_free (sys_context, game_state_smob->c_game_state);
  LW6SYS_FREE (sys_context, game_state_smob);
}

/*
 * Game pilot smob
 */
static SCM
_mark_pilot (SCM pilot)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mark pilot smob"));

  return SCM_BOOL_F;
}

static size_t
_free_pilot (SCM pilot)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *id = NULL;
  lw6_pilot_smob_t *pilot_smob = (lw6_pilot_smob_t *) SCM_SMOB_DATA (pilot);

  LW6_MUTEX_LOCK;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("garbage collect pilot smob"));
  id = _smob_id (sys_context, SMOB_TYPE_PILOT, pilot_smob->c_pilot->id);
  if (id)
    {
      if (lw6_global.pilot_smobs)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("request free pilot smob \"%s\""), id);
	  lw6sys_assoc_unset (sys_context, lw6_global.pilot_smobs, id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("request free pilot smob \"%s\" but assoc is NULL"), id);
	}
      LW6SYS_FREE (sys_context, id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
_print_pilot (SCM pilot, SCM port, scm_print_state * pstate)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6pil_pilot_t *c_pilot = lw6_scm_to_pilot (sys_context, pilot);
  char *repr = NULL;

  repr = lw6pil_pilot_repr (sys_context, c_pilot);

  scm_puts ("#<" SMOB_TYPE_PILOT " ", port);
  if (repr)
    {
      scm_display (scm_from_locale_string (repr), port);
      LW6SYS_FREE (sys_context, repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_pilot
 *
 * @sys_context: global system context
 * @c_pilot: the pilot object
 *
 * Creates an SCM 'pilot' object from C data.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_pilot (lw6sys_context_t * sys_context, lw6pil_pilot_t * c_pilot)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_pilot_smob_t *pilot_smob = NULL;

  scm_gc ();

  pilot_smob = (lw6_pilot_smob_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6_pilot_smob_t));
  if (pilot_smob)
    {
      pilot_smob->c_pilot = c_pilot;
      id = _smob_id (sys_context, SMOB_TYPE_PILOT, c_pilot->id);
      if (id)
	{
	  repr = lw6pil_pilot_repr (sys_context, c_pilot);
	  if (repr)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("creating pilot smob \"%s\""), repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (sys_context, &lw6_global.pilot_smobs, id, (void *) pilot_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (sys_context, repr);
	    }
	  LW6SYS_FREE (sys_context, id);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create pilot smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.pilot, pilot_smob);
}

/**
 * lw6_scm_to_pilot
 *
 * @sys_context: global system context
 * @pilot: the pilot to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'pilot' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6pil_pilot_t *
lw6_scm_to_pilot (lw6sys_context_t * sys_context, SCM pilot)
{
  lw6pil_pilot_t *c_pilot;

  c_pilot = ((lw6_pilot_smob_t *) SCM_SMOB_DATA (pilot))->c_pilot;

  return c_pilot;
}

/**
 * lw6_free_pilot_smob
 *
 * @sys_context: global system context
 * @pilot_smob: the smob to free
 *
 * Frees a pilot smob, we need a special function to do
 * that as structures like assoc hold pointers to these
 * objects and therefore need a proper callback when being
 * destroyed.
 *
 * Return value: none
 */
void
lw6_free_pilot_smob (lw6sys_context_t * sys_context, lw6_pilot_smob_t * pilot_smob)
{
  char *repr = NULL;

  repr = lw6pil_pilot_repr (sys_context, pilot_smob->c_pilot);
  if (repr)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing pilot smob \"%s\""), repr);
      LW6SYS_FREE (sys_context, repr);
    }

  lw6pil_pilot_free (sys_context, pilot_smob->c_pilot);
  LW6SYS_FREE (sys_context, pilot_smob);
}

/*
 * Game bot smob
 */
static SCM
_mark_bot (SCM bot)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6_bot_smob_t *bot_smob = (lw6_bot_smob_t *) SCM_SMOB_DATA (bot);

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mark bot smob"));

  scm_gc_mark (bot_smob->game_state);
  scm_gc_mark (bot_smob->pilot);

  return SCM_BOOL_F;
}

static size_t
_free_bot (SCM bot)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *id = NULL;
  lw6_bot_smob_t *bot_smob = (lw6_bot_smob_t *) SCM_SMOB_DATA (bot);

  LW6_MUTEX_LOCK;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("garbage collect bot smob"));
  id = _smob_id (sys_context, SMOB_TYPE_BOT, bot_smob->c_bot->id);
  if (id)
    {
      if (lw6_global.bot_smobs)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("request free bot smob \"%s\""), id);
	  lw6sys_assoc_unset (sys_context, lw6_global.bot_smobs, id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("request free bot smob \"%s\" but assoc is NULL"), id);
	}
      LW6SYS_FREE (sys_context, id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
_print_bot (SCM bot, SCM port, scm_print_state * pstate)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6bot_backend_t *c_bot = lw6_scm_to_bot (sys_context, bot);
  char *repr = NULL;

  repr = lw6bot_repr (sys_context, c_bot);

  scm_puts ("#<" SMOB_TYPE_BOT " ", port);
  if (repr)
    {
      scm_display (scm_from_locale_string (repr), port);
      LW6SYS_FREE (sys_context, repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_bot
 *
 * @sys_context: global system context
 * @c_bot: the bot object
 * @game_state: the game state
 * @pilot: the pilot
 *
 * Creates an SCM 'bot' object from C data.
 * Passing game_state and pilot enables the garbage collector
 * not the free them until bot is freed.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_bot (lw6sys_context_t * sys_context, lw6bot_backend_t * c_bot, SCM game_state, SCM pilot)
{
// c_bot is supposed to have been initialized with lw6bot_init()
  char *repr = NULL;
  char *id = NULL;
  lw6_bot_smob_t *bot_smob = NULL;

  scm_gc ();

  bot_smob = (lw6_bot_smob_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6_bot_smob_t));
  if (bot_smob)
    {
      bot_smob->c_bot = c_bot;
      bot_smob->game_state = game_state;
      bot_smob->pilot = pilot;
      id = _smob_id (sys_context, SMOB_TYPE_BOT, c_bot->id);
      if (id)
	{
	  repr = lw6bot_repr (sys_context, c_bot);
	  if (repr)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("creating bot smob \"%s\""), repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (sys_context, &lw6_global.bot_smobs, id, (void *) bot_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (sys_context, repr);
	    }
	  LW6SYS_FREE (sys_context, id);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create bot smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.bot, bot_smob);
}

/**
 * lw6_scm_to_bot
 *
 * @sys_context: global system context
 * @bot: the bot to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'bot' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6bot_backend_t *
lw6_scm_to_bot (lw6sys_context_t * sys_context, SCM bot)
{
  lw6bot_backend_t *c_bot;

  c_bot = ((lw6_bot_smob_t *) SCM_SMOB_DATA (bot))->c_bot;

  return c_bot;
}

/**
 * lw6_free_bot_smob
 *
 * @sys_context: global system context
 * @bot_smob: the smob to free
 *
 * Frees a bot smob, we need a special function to do
 * that as structures like assoc hold pointers to these
 * objects and therefore need a proper callback when being
 * destroyed.
 *
 * Return value: none
 */
void
lw6_free_bot_smob (lw6sys_context_t * sys_context, lw6_bot_smob_t * bot_smob)
{
  char *repr = NULL;

  repr = lw6bot_repr (sys_context, bot_smob->c_bot);
  if (repr)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing bot smob \"%s\""), repr);
      LW6SYS_FREE (sys_context, repr);
    }

  lw6bot_quit (sys_context, bot_smob->c_bot);
  lw6bot_destroy_backend (sys_context, bot_smob->c_bot);
  LW6SYS_FREE (sys_context, bot_smob);
}

/*
 * Game look smob
 */
static SCM
_mark_look (SCM look)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mark look smob"));

  return SCM_BOOL_F;
}

static size_t
_free_look (SCM look)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *id = NULL;
  lw6_look_smob_t *look_smob = (lw6_look_smob_t *) SCM_SMOB_DATA (look);

  LW6_MUTEX_LOCK;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("garbage collect look smob"));
  id = _smob_id (sys_context, SMOB_TYPE_LOOK, look_smob->c_look->id);
  if (id)
    {
      if (lw6_global.look_smobs)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("request free look smob \"%s\""), id);
	  lw6sys_assoc_unset (sys_context, lw6_global.look_smobs, id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("request free look smob \"%s\" but assoc is NULL"), id);
	}
      LW6SYS_FREE (sys_context, id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
_print_look (SCM look, SCM port, scm_print_state * pstate)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6gui_look_t *c_look = lw6_scm_to_look (sys_context, look);
  char *repr = NULL;

  repr = lw6gui_look_repr (sys_context, c_look);

  scm_puts ("#<" SMOB_TYPE_LOOK " ", port);
  if (repr)
    {
      scm_display (scm_from_locale_string (repr), port);
      LW6SYS_FREE (sys_context, repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_look
 *
 * @sys_context: global system context
 * @c_look: the look object
 *
 * Creates an SCM 'look' object from C data.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_look (lw6sys_context_t * sys_context, lw6gui_look_t * c_look)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_look_smob_t *look_smob = NULL;

  scm_gc ();

  look_smob = (lw6_look_smob_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6_look_smob_t));
  if (look_smob)
    {
      look_smob->c_look = c_look;
      id = _smob_id (sys_context, SMOB_TYPE_LOOK, c_look->id);
      if (id)
	{
	  repr = lw6gui_look_repr (sys_context, c_look);
	  if (repr)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("creating look smob \"%s\""), repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (sys_context, &lw6_global.look_smobs, id, (void *) look_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (sys_context, repr);
	    }
	  LW6SYS_FREE (sys_context, id);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create look smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.look, look_smob);
}

/**
 * lw6_scm_to_look
 *
 * @sys_context: global system context
 * @look: the look to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'look' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6gui_look_t *
lw6_scm_to_look (lw6sys_context_t * sys_context, SCM look)
{
  lw6gui_look_t *c_look;

  c_look = ((lw6_look_smob_t *) SCM_SMOB_DATA (look))->c_look;

  return c_look;
}

/**
 * lw6_free_look_smob
 *
 * @sys_context: global system context
 * @look_smob: the smob to free
 *
 * Frees a look smob, we need a special function to do
 * that as structures like assoc hold pointers to these
 * objects and therefore need a proper callback when being
 * destroyed.
 *
 * Return value: none
 */
void
lw6_free_look_smob (lw6sys_context_t * sys_context, lw6_look_smob_t * look_smob)
{
  char *repr = NULL;

  repr = lw6gui_look_repr (sys_context, look_smob->c_look);
  if (repr)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing look smob \"%s\""), repr);
      LW6SYS_FREE (sys_context, repr);
    }

  lw6gui_look_free (sys_context, look_smob->c_look);
  LW6SYS_FREE (sys_context, look_smob);
}

/*
 * Loader smob
 */
static SCM
_mark_loader (SCM loader)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mark loader smob"));

  return SCM_BOOL_F;
}

static size_t
_free_loader (SCM loader)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *id = NULL;
  lw6_loader_smob_t *loader_smob = (lw6_loader_smob_t *) SCM_SMOB_DATA (loader);

  LW6_MUTEX_LOCK;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("garbage collect loader smob"));
  id = _smob_id (sys_context, SMOB_TYPE_LOADER, loader_smob->c_loader->id);
  if (id)
    {
      if (lw6_global.loader_smobs)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("request free loader smob \"%s\""), id);
	  lw6sys_assoc_unset (sys_context, lw6_global.loader_smobs, id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("request free loader smob \"%s\" but assoc is NULL"), id);
	}
      LW6SYS_FREE (sys_context, id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
_print_loader (SCM loader, SCM port, scm_print_state * pstate)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6tsk_loader_t *c_loader = lw6_scm_to_loader (sys_context, loader);
  char *repr = NULL;

  repr = lw6tsk_loader_repr (sys_context, c_loader);

  scm_puts ("#<" SMOB_TYPE_LOADER " ", port);
  if (repr)
    {
      scm_display (scm_from_locale_string (repr), port);
      LW6SYS_FREE (sys_context, repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_loader
 *
 * @sys_context: global system context
 * @c_loader: the loader object
 *
 * Creates an SCM 'loader' object from C data.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_loader (lw6sys_context_t * sys_context, lw6tsk_loader_t * c_loader)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_loader_smob_t *loader_smob = NULL;

  scm_gc ();

  loader_smob = (lw6_loader_smob_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6_loader_smob_t));
  if (loader_smob)
    {
      loader_smob->c_loader = c_loader;
      id = _smob_id (sys_context, SMOB_TYPE_LOADER, c_loader->id);
      if (id)
	{
	  repr = lw6tsk_loader_repr (sys_context, c_loader);
	  if (repr)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("creating loader smob \"%s\""), repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (sys_context, &lw6_global.loader_smobs, id, (void *) loader_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (sys_context, repr);
	    }
	  LW6SYS_FREE (sys_context, id);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create loader smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.loader, loader_smob);
}

/**
 * lw6_scm_to_loader
 *
 * @sys_context: global system context
 * @loader: the loader to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'loader' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6tsk_loader_t *
lw6_scm_to_loader (lw6sys_context_t * sys_context, SCM loader)
{
  lw6tsk_loader_t *c_loader;

  c_loader = ((lw6_loader_smob_t *) SCM_SMOB_DATA (loader))->c_loader;

  return c_loader;
}

/**
 * lw6_free_loader_smob
 *
 * @sys_context: global system context
 * @loader_smob: the smob to free
 *
 * Frees a loader smob, we need a special function to do
 * that as structures like assoc hold pointers to these
 * objects and therefore need a proper callback when being
 * destroyed.
 *
 * Return value: none
 */
void
lw6_free_loader_smob (lw6sys_context_t * sys_context, lw6_loader_smob_t * loader_smob)
{
  char *repr = NULL;

  repr = lw6tsk_loader_repr (sys_context, loader_smob->c_loader);
  if (repr)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing loader smob \"%s\""), repr);
      LW6SYS_FREE (sys_context, repr);
    }

  lw6tsk_loader_free (sys_context, loader_smob->c_loader);
  LW6SYS_FREE (sys_context, loader_smob);
}

/*
 * Db smob
 */
static SCM
_mark_db (SCM db)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mark db smob"));

  return SCM_BOOL_F;
}

static size_t
_free_db (SCM db)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *id = NULL;
  lw6_db_smob_t *db_smob = (lw6_db_smob_t *) SCM_SMOB_DATA (db);

  LW6_MUTEX_LOCK;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("garbage collect db smob"));
  id = _smob_id (sys_context, SMOB_TYPE_DB, db_smob->c_db->id);
  if (id)
    {
      if (lw6_global.db_smobs)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("request free db smob \"%s\""), id);
	  lw6sys_assoc_unset (sys_context, lw6_global.db_smobs, id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("request free db smob \"%s\" but assoc is NULL"), id);
	}
      LW6SYS_FREE (sys_context, id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
_print_db (SCM db, SCM port, scm_print_state * pstate)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_db_t *c_db = lw6_scm_to_db (sys_context, db);
  char *repr = NULL;

  repr = lw6p2p_db_repr (sys_context, c_db);

  scm_puts ("#<" SMOB_TYPE_DB " ", port);
  if (repr)
    {
      scm_display (scm_from_locale_string (repr), port);
      LW6SYS_FREE (sys_context, repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_db
 *
 * @sys_context: global system context
 * @c_db: the database object
 *
 * Creates an SCM 'db' object from C data.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_db (lw6sys_context_t * sys_context, lw6p2p_db_t * c_db)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_db_smob_t *db_smob = NULL;

  scm_gc ();

  db_smob = (lw6_db_smob_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6_db_smob_t));
  if (db_smob)
    {
      db_smob->c_db = c_db;
      id = _smob_id (sys_context, SMOB_TYPE_DB, c_db->id);
      if (id)
	{
	  repr = lw6p2p_db_repr (sys_context, c_db);
	  if (repr)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("creating db smob \"%s\""), repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (sys_context, &lw6_global.db_smobs, id, (void *) db_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (sys_context, repr);
	    }
	  LW6SYS_FREE (sys_context, id);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create db smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.db, db_smob);
}

/**
 * lw6_scm_to_db
 *
 * @sys_context: global system context
 * @db: the db to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'db' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6p2p_db_t *
lw6_scm_to_db (lw6sys_context_t * sys_context, SCM db)
{
  void *c_db;

  c_db = ((lw6_db_smob_t *) SCM_SMOB_DATA (db))->c_db;

  return c_db;
}

/**
 * lw6_free_db_smob
 *
 * @sys_context: global system context
 * @db_smob: the smob to free
 *
 * Frees a db smob, we need a special function to do
 * that as structures like assoc hold pointers to these
 * objects and therefore need a proper callback when being
 * destroyed.
 *
 * Return value: none
 */
void
lw6_free_db_smob (lw6sys_context_t * sys_context, lw6_db_smob_t * db_smob)
{
  char *repr = NULL;

  repr = lw6p2p_db_repr (sys_context, db_smob->c_db);
  if (repr)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing db smob \"%s\""), repr);
      LW6SYS_FREE (sys_context, repr);
    }

  lw6p2p_db_close (sys_context, db_smob->c_db);
  LW6SYS_FREE (sys_context, db_smob);
}

/*
 * Node smob
 */
static SCM
_mark_node (SCM node)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6_node_smob_t *node_smob = (lw6_node_smob_t *) SCM_SMOB_DATA (node);

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mark node smob"));

  scm_gc_mark (node_smob->db);

  return SCM_BOOL_F;
}

static size_t
_free_node (SCM node)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *id = NULL;
  lw6_node_smob_t *node_smob = (lw6_node_smob_t *) SCM_SMOB_DATA (node);

  LW6_MUTEX_LOCK;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("garbage collect node smob"));
  id = _smob_id (sys_context, SMOB_TYPE_NODE, node_smob->c_node->id);
  if (id)
    {
      if (lw6_global.node_smobs)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("request free node smob \"%s\""), id);
	  lw6sys_assoc_unset (sys_context, lw6_global.node_smobs, id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("request free node smob \"%s\" but assoc is NULL"), id);
	}
      LW6SYS_FREE (sys_context, id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
_print_node (SCM node, SCM port, scm_print_state * pstate)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6p2p_node_t *c_node = lw6_scm_to_node (sys_context, node);
  char *repr = NULL;

  repr = lw6p2p_node_repr (sys_context, c_node);

  scm_puts ("#<" SMOB_TYPE_NODE " ", port);
  if (repr)
    {
      scm_display (scm_from_locale_string (repr), port);
      LW6SYS_FREE (sys_context, repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_node
 *
 * @sys_context: global system context
 * @c_node: the node object
 * @db: the db (SCM object) referenced
 *
 * Creates an SCM 'node' object from C data.
 * Passing db enables the garbage collector not to
 * free db until node is freed.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_node (lw6sys_context_t * sys_context, lw6p2p_node_t * c_node, SCM db)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_node_smob_t *node_smob = NULL;

  scm_gc ();

  node_smob = (lw6_node_smob_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6_node_smob_t));
  if (node_smob)
    {
      node_smob->c_node = c_node;
      node_smob->db = db;

      id = _smob_id (sys_context, SMOB_TYPE_NODE, c_node->id);
      if (id)
	{
	  repr = lw6p2p_node_repr (sys_context, c_node);
	  if (repr)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("creating node smob \"%s\""), repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (sys_context, &lw6_global.node_smobs, id, (void *) node_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (sys_context, repr);
	    }
	  LW6SYS_FREE (sys_context, id);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create node smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.node, node_smob);
}

/**
 * lw6_scm_to_node
 *
 * @sys_context: global system context
 * @node: the node to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'node' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6p2p_node_t *
lw6_scm_to_node (lw6sys_context_t * sys_context, SCM node)
{
  void *c_node;

  c_node = ((lw6_node_smob_t *) SCM_SMOB_DATA (node))->c_node;

  return c_node;
}

/**
 * lw6_free_node_smob
 *
 * @sys_context: global system context
 * @node_smob: the smob to free
 *
 * Frees a node smob, we need a special function to do
 * that as structures like assoc hold pointers to these
 * objects and therefore need a proper callback when being
 * destroyed.
 *
 * Return value: none
 */
void
lw6_free_node_smob (lw6sys_context_t * sys_context, lw6_node_smob_t * node_smob)
{
  char *repr = NULL;

  repr = lw6p2p_node_repr (sys_context, node_smob->c_node);
  if (repr)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing node smob \"%s\""), repr);
      LW6SYS_FREE (sys_context, repr);
    }

  lw6p2p_node_free (sys_context, node_smob->c_node);
  LW6SYS_FREE (sys_context, node_smob);
}

/*
 * Jpeg smob
 */
static SCM
_mark_jpeg (SCM jpeg)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("mark jpeg smob"));

  return SCM_BOOL_F;
}

static size_t
_free_jpeg (SCM jpeg)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  char *id = NULL;
  lw6_jpeg_smob_t *jpeg_smob = (lw6_jpeg_smob_t *) SCM_SMOB_DATA (jpeg);

  LW6_MUTEX_LOCK;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("garbage collect jpeg smob"));
  id = _smob_id (sys_context, SMOB_TYPE_JPEG, jpeg_smob->c_jpeg->id);
  if (id)
    {
      if (lw6_global.jpeg_smobs)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("request free jpeg smob \"%s\""), id);
	  lw6sys_assoc_unset (sys_context, lw6_global.jpeg_smobs, id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("request free jpeg smob \"%s\" but assoc is NULL"), id);
	}
      LW6SYS_FREE (sys_context, id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
_print_jpeg (SCM jpeg, SCM port, scm_print_state * pstate)
{
  lw6sys_context_t *sys_context = lw6_global.sys_context;
  lw6img_jpeg_t *c_jpeg = lw6_scm_to_jpeg (sys_context, jpeg);
  char *repr = NULL;

  repr = lw6img_repr (sys_context, c_jpeg);

  scm_puts ("#<" SMOB_TYPE_JPEG " ", port);
  if (repr)
    {
      scm_display (scm_from_locale_string (repr), port);
      LW6SYS_FREE (sys_context, repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_jpeg
 *
 * @sys_context: global system context
 * @c_jpeg: the database object
 *
 * Creates an SCM 'jpeg' object from C data.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_jpeg (lw6sys_context_t * sys_context, lw6img_jpeg_t * c_jpeg)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_jpeg_smob_t *jpeg_smob = NULL;

  scm_gc ();

  jpeg_smob = (lw6_jpeg_smob_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6_jpeg_smob_t));
  if (jpeg_smob)
    {
      jpeg_smob->c_jpeg = c_jpeg;
      id = _smob_id (sys_context, SMOB_TYPE_JPEG, c_jpeg->id);
      if (id)
	{
	  repr = lw6img_repr (sys_context, c_jpeg);
	  if (repr)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("creating jpeg smob \"%s\""), repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (sys_context, &lw6_global.jpeg_smobs, id, (void *) jpeg_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (sys_context, repr);
	    }
	  LW6SYS_FREE (sys_context, id);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create jpeg smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.jpeg, jpeg_smob);
}

/**
 * lw6_scm_to_jpeg
 *
 * @sys_context: global system context
 * @jpeg: the jpeg to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'jpeg' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6img_jpeg_t *
lw6_scm_to_jpeg (lw6sys_context_t * sys_context, SCM jpeg)
{
  void *c_jpeg;

  c_jpeg = ((lw6_jpeg_smob_t *) SCM_SMOB_DATA (jpeg))->c_jpeg;

  return c_jpeg;
}

/**
 * lw6_free_jpeg_smob
 *
 * @sys_context: global system context
 * @jpeg_smob: the smob to free
 *
 * Frees a jpeg smob, we need a special function to do
 * that as structures like assoc hold pointers to these
 * objects and therefore need a proper callback when being
 * destroyed.
 *
 * Return value: none
 */
void
lw6_free_jpeg_smob (lw6sys_context_t * sys_context, lw6_jpeg_smob_t * jpeg_smob)
{
  char *repr = NULL;

  repr = lw6img_repr (sys_context, jpeg_smob->c_jpeg);
  if (repr)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing jpeg smob \"%s\""), repr);
      LW6SYS_FREE (sys_context, repr);
    }

  lw6img_screenshot_free (sys_context, jpeg_smob->c_jpeg);
  LW6SYS_FREE (sys_context, jpeg_smob);
}

/**
 * lw6_register_smobs
 *
 * @sys_context: global system context
 *
 * Register all smobs to Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_smobs (lw6sys_context_t * sys_context)
{
  int ret = 1;

  lw6_global.smob_types.dsp = scm_make_smob_type (SMOB_TYPE_DSP, sizeof (lw6_dsp_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.dsp, _mark_dsp);
  scm_set_smob_free (lw6_global.smob_types.dsp, _free_dsp);
  scm_set_smob_print (lw6_global.smob_types.dsp, _print_dsp);

  lw6_global.smob_types.snd = scm_make_smob_type (SMOB_TYPE_SND, sizeof (lw6_snd_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.snd, _mark_snd);
  scm_set_smob_free (lw6_global.smob_types.snd, _free_snd);
  scm_set_smob_print (lw6_global.smob_types.snd, _print_snd);

  lw6_global.smob_types.map = scm_make_smob_type (SMOB_TYPE_MAP, sizeof (lw6_map_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.map, _mark_map);
  scm_set_smob_free (lw6_global.smob_types.map, _free_map);
  scm_set_smob_print (lw6_global.smob_types.map, _print_map);

  lw6_global.smob_types.menu = scm_make_smob_type (SMOB_TYPE_MENU, sizeof (lw6_menu_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.menu, _mark_menu);
  scm_set_smob_free (lw6_global.smob_types.menu, _free_menu);
  scm_set_smob_print (lw6_global.smob_types.menu, _print_menu);

  lw6_global.smob_types.game_struct = scm_make_smob_type (SMOB_TYPE_GAME_STRUCT, sizeof (lw6_game_struct_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.game_struct, _mark_game_struct);
  scm_set_smob_free (lw6_global.smob_types.game_struct, _free_game_struct);
  scm_set_smob_print (lw6_global.smob_types.game_struct, _print_game_struct);

  lw6_global.smob_types.game_state = scm_make_smob_type (SMOB_TYPE_GAME_STATE, sizeof (lw6_game_state_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.game_state, _mark_game_state);
  scm_set_smob_free (lw6_global.smob_types.game_state, _free_game_state);
  scm_set_smob_print (lw6_global.smob_types.game_state, _print_game_state);

  lw6_global.smob_types.pilot = scm_make_smob_type (SMOB_TYPE_PILOT, sizeof (lw6_pilot_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.pilot, _mark_pilot);
  scm_set_smob_free (lw6_global.smob_types.pilot, _free_pilot);
  scm_set_smob_print (lw6_global.smob_types.pilot, _print_pilot);

  lw6_global.smob_types.bot = scm_make_smob_type (SMOB_TYPE_BOT, sizeof (lw6_bot_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.bot, _mark_bot);
  scm_set_smob_free (lw6_global.smob_types.bot, _free_bot);
  scm_set_smob_print (lw6_global.smob_types.bot, _print_bot);

  lw6_global.smob_types.look = scm_make_smob_type (SMOB_TYPE_LOOK, sizeof (lw6_look_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.look, _mark_look);
  scm_set_smob_free (lw6_global.smob_types.look, _free_look);
  scm_set_smob_print (lw6_global.smob_types.look, _print_look);

  lw6_global.smob_types.loader = scm_make_smob_type (SMOB_TYPE_LOADER, sizeof (lw6_loader_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.loader, _mark_loader);
  scm_set_smob_free (lw6_global.smob_types.loader, _free_loader);
  scm_set_smob_print (lw6_global.smob_types.loader, _print_loader);

  lw6_global.smob_types.db = scm_make_smob_type (SMOB_TYPE_DB, sizeof (lw6_db_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.db, _mark_db);
  scm_set_smob_free (lw6_global.smob_types.db, _free_db);
  scm_set_smob_print (lw6_global.smob_types.db, _print_db);

  lw6_global.smob_types.node = scm_make_smob_type (SMOB_TYPE_NODE, sizeof (lw6_node_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.node, _mark_node);
  scm_set_smob_free (lw6_global.smob_types.node, _free_node);
  scm_set_smob_print (lw6_global.smob_types.node, _print_node);

  lw6_global.smob_types.jpeg = scm_make_smob_type (SMOB_TYPE_JPEG, sizeof (lw6_jpeg_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.jpeg, _mark_jpeg);
  scm_set_smob_free (lw6_global.smob_types.jpeg, _free_jpeg);
  scm_set_smob_print (lw6_global.smob_types.jpeg, _print_jpeg);

  return ret;
}
