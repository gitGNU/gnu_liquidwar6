/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

static char *
smob_id (char *type, int id)
{
  char *ret = NULL;

  ret = lw6sys_new_sprintf ("%s %d", type, id);

  return ret;
}

/*
 * Display smob
 */
static SCM
mark_dsp (SCM dsp)
{
  lw6_dsp_smob_t *dsp_smob = (lw6_dsp_smob_t *) SCM_SMOB_DATA (dsp);

  scm_gc_mark (dsp_smob->level);
  scm_gc_mark (dsp_smob->game_struct);
  scm_gc_mark (dsp_smob->game_state);
  scm_gc_mark (dsp_smob->pilot);

  return SCM_BOOL_F;
}

static size_t
free_dsp (SCM dsp)
{
  char *id = NULL;
  lw6_dsp_smob_t *dsp_smob = (lw6_dsp_smob_t *) SCM_SMOB_DATA (dsp);

  LW6_MUTEX_LOCK;

  lw6sys_log (LW6SYS_LOG_INFO, _("garbage collect dsp smob"));
  id = smob_id (SMOB_TYPE_DSP, dsp_smob->c_dsp->id);
  if (id)
    {
      if (lw6_global.dsp_smobs)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("request free dsp smob \"%s\""), id);
	  lw6sys_assoc_unset (lw6_global.dsp_smobs, id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("request free dsp smob \"%s\" but assoc is NULL"),
		      id);
	}
      LW6SYS_FREE (id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
print_dsp (SCM dsp, SCM port, scm_print_state * pstate)
{
  lw6dsp_backend_t *c_dsp = lw6_scm_to_dsp (dsp);
  char *repr = NULL;

  repr = lw6dsp_repr (c_dsp);

  scm_puts ("#<" SMOB_TYPE_DSP " ", port);
  if (repr)
    {
      scm_display (scm_makfrom0str (repr), port);
      LW6SYS_FREE (repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_dsp
 *
 * @c_dsp: the display object
 *
 * Creates an SCM 'dsp' object from C data.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_dsp (lw6dsp_backend_t * c_dsp)
{
  // c_dsp is supposed to have been initialized with lw6dsp_init()
  char *repr = NULL;
  char *id = NULL;
  lw6_dsp_smob_t *dsp_smob = NULL;

  scm_gc ();

  dsp_smob = (lw6_dsp_smob_t *) LW6SYS_CALLOC (sizeof (lw6_dsp_smob_t));
  if (dsp_smob)
    {
      dsp_smob->c_dsp = c_dsp;
      dsp_smob->level = SCM_BOOL_F;	// initialized later with update
      dsp_smob->game_struct = SCM_BOOL_F;	// initialized later with update
      dsp_smob->game_state = SCM_BOOL_F;	// initialized later with update
      dsp_smob->pilot = SCM_BOOL_F;	// initialized later with update
      id = smob_id (SMOB_TYPE_DSP, c_dsp->id);
      if (id)
	{
	  repr = lw6dsp_repr (c_dsp);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_INFO, _("creating dsp smob \"%s\""),
			  repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (&lw6_global.dsp_smobs, id, (void *) dsp_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (repr);
	    }
	  LW6SYS_FREE (id);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("unable to create dsp smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.dsp, dsp_smob);
}

/**
 * lw6_scm_to_dsp
 *
 * @dsp: the dsp to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'dsp' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6dsp_backend_t *
lw6_scm_to_dsp (SCM dsp)
{
  lw6dsp_backend_t *c_dsp;

  c_dsp = ((lw6_dsp_smob_t *) SCM_SMOB_DATA (dsp))->c_dsp;

  return c_dsp;
}

/**
 * lw6_free_dsp_smob
 *
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
lw6_free_dsp_smob (lw6_dsp_smob_t * dsp_smob)
{
  char *repr = NULL;

  repr = lw6dsp_repr (dsp_smob->c_dsp);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("freeing dsp smob \"%s\""), repr);
      LW6SYS_FREE (repr);
    }

  lw6dsp_quit (dsp_smob->c_dsp);
  lw6dsp_destroy_backend (dsp_smob->c_dsp);
  LW6SYS_FREE (dsp_smob);
}

/*
 * Sound smob
 */
static SCM
mark_snd (SCM snd)
{
  return SCM_BOOL_F;
}

static size_t
free_snd (SCM snd)
{
  char *id = NULL;
  lw6_snd_smob_t *snd_smob = (lw6_snd_smob_t *) SCM_SMOB_DATA (snd);

  LW6_MUTEX_LOCK;

  lw6sys_log (LW6SYS_LOG_INFO, _("garbage collect snd smob"));
  id = smob_id (SMOB_TYPE_SND, snd_smob->c_snd->id);
  if (id)
    {
      if (lw6_global.snd_smobs)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("request free snd smob \"%s\""), id);
	  lw6sys_assoc_unset (lw6_global.snd_smobs, id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("request free snd smob \"%s\" but assoc is NULL"),
		      id);
	}
      LW6SYS_FREE (id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
print_snd (SCM snd, SCM port, scm_print_state * pstate)
{
  lw6snd_backend_t *c_snd = lw6_scm_to_snd (snd);
  char *repr = NULL;

  repr = lw6snd_repr (c_snd);

  scm_puts ("#<" SMOB_TYPE_SND " ", port);
  if (repr)
    {
      scm_display (scm_makfrom0str (repr), port);
      LW6SYS_FREE (repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_snd
 *
 * @c_snd: the sound object
 *
 * Creates an SCM 'snd' object from C data.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_snd (lw6snd_backend_t * c_snd)
{
// c_snd is supposed to have been initialized with lw6snd_init()
  char *repr = NULL;
  char *id = NULL;
  lw6_snd_smob_t *snd_smob = NULL;

  scm_gc ();

  snd_smob = (lw6_snd_smob_t *) LW6SYS_CALLOC (sizeof (lw6_snd_smob_t));
  if (snd_smob)
    {
      snd_smob->c_snd = c_snd;
      id = smob_id (SMOB_TYPE_SND, c_snd->id);
      if (id)
	{
	  repr = lw6snd_repr (c_snd);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_INFO, _("creating snd smob \"%s\""),
			  repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (&lw6_global.snd_smobs, id, (void *) snd_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (repr);
	    }
	  LW6SYS_FREE (id);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("unable to create snd smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.snd, snd_smob);
}

/**
 * lw6_scm_to_snd
 *
 * @snd: the snd to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'snd' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6snd_backend_t *
lw6_scm_to_snd (SCM snd)
{
  lw6snd_backend_t *c_snd;

  c_snd = ((lw6_snd_smob_t *) SCM_SMOB_DATA (snd))->c_snd;

  return c_snd;
}

/**
 * lw6_free_snd_smob
 *
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
lw6_free_snd_smob (lw6_snd_smob_t * snd_smob)
{
  char *repr = NULL;

  repr = lw6snd_repr (snd_smob->c_snd);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("freeing snd smob \"%s\""), repr);
      LW6SYS_FREE (repr);
    }

  lw6snd_quit (snd_smob->c_snd);
  lw6snd_destroy_backend (snd_smob->c_snd);
  LW6SYS_FREE (snd_smob);
}

/*
 * Map smob
 */
static SCM
mark_map (SCM map)
{
  return SCM_BOOL_F;
}

static size_t
free_map (SCM map)
{
  char *id = NULL;
  lw6_map_smob_t *map_smob = (lw6_map_smob_t *) SCM_SMOB_DATA (map);

  LW6_MUTEX_LOCK;

  lw6sys_log (LW6SYS_LOG_INFO, _("garbage collect map smob"));
  id = smob_id (SMOB_TYPE_MAP, map_smob->c_map->id);
  if (id)
    {
      if (lw6_global.map_smobs)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("request free map smob \"%s\""), id);
	  lw6sys_assoc_unset (lw6_global.map_smobs, id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("request free map smob \"%s\" but assoc is NULL"),
		      id);
	}
      LW6SYS_FREE (id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
print_map (SCM map, SCM port, scm_print_state * pstate)
{
  lw6map_level_t *c_map = lw6_scm_to_map (map);
  char *repr = NULL;

  repr = lw6map_repr (c_map);

  scm_puts ("#<" SMOB_TYPE_MAP " ", port);
  if (repr)
    {
      scm_display (scm_makfrom0str (repr), port);
      LW6SYS_FREE (repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_map
 *
 * @c_map: the map object
 *
 * Creates an SCM 'map' object from C data.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_map (lw6map_level_t * c_map)
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

  map_smob = (lw6_map_smob_t *) LW6SYS_CALLOC (sizeof (lw6_map_smob_t));
  if (map_smob)
    {
      map_smob->c_map = c_map;
      id = smob_id (SMOB_TYPE_MAP, c_map->id);
      if (id)
	{
	  repr = lw6map_repr (c_map);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_INFO, _("creating map smob \"%s\""),
			  repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (&lw6_global.map_smobs, id, (void *) map_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (repr);
	    }
	  LW6SYS_FREE (id);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("unable to create map smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.map, map_smob);
}

/**
 * lw6_scm_to_map
 *
 * @map: the map to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'map' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6map_level_t *
lw6_scm_to_map (SCM map)
{
  lw6map_level_t *c_map;

  c_map = ((lw6_map_smob_t *) SCM_SMOB_DATA (map))->c_map;

  return c_map;
}

/**
 * lw6_free_map_smob
 *
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
lw6_free_map_smob (lw6_map_smob_t * map_smob)
{
  char *repr = NULL;

  repr = lw6map_repr (map_smob->c_map);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("freeing map smob \"%s\""), repr);
      LW6SYS_FREE (repr);
    }

  lw6map_free (map_smob->c_map);
  LW6SYS_FREE (map_smob);
}

/*
 * Menu smob
 */
static SCM
mark_menu (SCM menu)
{
  return SCM_BOOL_F;
}

static size_t
free_menu (SCM menu)
{
  char *id = NULL;
  lw6_menu_smob_t *menu_smob = (lw6_menu_smob_t *) SCM_SMOB_DATA (menu);

  LW6_MUTEX_LOCK;

  lw6sys_log (LW6SYS_LOG_INFO, _("garbage collect menu smob"));
  id = smob_id (SMOB_TYPE_MENU, menu_smob->c_menu->id);
  if (id)
    {
      if (lw6_global.menu_smobs)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("request free menu smob \"%s\""),
		      id);
	  lw6sys_assoc_unset (lw6_global.menu_smobs, id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("request free menu smob \"%s\" but assoc is NULL"),
		      id);
	}
      LW6SYS_FREE (id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
print_menu (SCM menu, SCM port, scm_print_state * pstate)
{
  lw6gui_menu_t *c_menu = lw6_scm_to_menu (menu);
  char *repr = NULL;

  repr = lw6gui_menu_repr (c_menu);

  scm_puts ("#<" SMOB_TYPE_MENU " ", port);
  if (repr)
    {
      scm_display (scm_makfrom0str (repr), port);
      LW6SYS_FREE (repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_menu
 *
 * @c_menu: the menu object
 *
 * Creates an SCM 'menu' object from C data.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_menu (lw6gui_menu_t * c_menu)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_menu_smob_t *menu_smob = NULL;

  scm_gc ();

  menu_smob = (lw6_menu_smob_t *) LW6SYS_CALLOC (sizeof (lw6_menu_smob_t));
  if (menu_smob)
    {
      menu_smob->c_menu = c_menu;
      id = smob_id (SMOB_TYPE_MENU, c_menu->id);
      if (id)
	{
	  repr = lw6gui_menu_repr (c_menu);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_INFO, _("creating menu smob \"%s\""),
			  repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (&lw6_global.menu_smobs, id,
				(void *) menu_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (repr);
	    }
	  LW6SYS_FREE (id);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("unable to create menu smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.menu, menu_smob);
}

/**
 * lw6_scm_to_menu
 *
 * @menu: the menu to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'menu' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6gui_menu_t *
lw6_scm_to_menu (SCM menu)
{
  lw6gui_menu_t *c_menu;

  c_menu = ((lw6_menu_smob_t *) SCM_SMOB_DATA (menu))->c_menu;

  return c_menu;
}

/**
 * lw6_free_menu_smob
 *
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
lw6_free_menu_smob (lw6_menu_smob_t * menu_smob)
{
  char *repr = NULL;

  repr = lw6gui_menu_repr (menu_smob->c_menu);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("freeing menu smob \"%s\""), repr);
      LW6SYS_FREE (repr);
    }

  lw6gui_menu_free (menu_smob->c_menu);
  LW6SYS_FREE (menu_smob);
}

/*
 * Game struct smob
 */
static SCM
mark_game_struct (SCM game_struct)
{
  lw6_game_struct_smob_t *game_struct_smob =
    (lw6_game_struct_smob_t *) SCM_SMOB_DATA (game_struct);

  scm_gc_mark (game_struct_smob->map);

  return SCM_BOOL_F;
}

static size_t
free_game_struct (SCM game_struct)
{
  char *id = NULL;
  lw6_game_struct_smob_t *game_struct_smob =
    (lw6_game_struct_smob_t *) SCM_SMOB_DATA (game_struct);

  LW6_MUTEX_LOCK;

  lw6sys_log (LW6SYS_LOG_INFO, _("garbage collect game struct smob"));
  id = smob_id (SMOB_TYPE_GAME_STRUCT, game_struct_smob->c_game_struct->id);
  if (id)
    {
      if (lw6_global.game_struct_smobs)
	{
	  lw6sys_log (LW6SYS_LOG_INFO,
		      _("request free game struct smob \"%s\""), id);
	  lw6sys_assoc_unset (lw6_global.game_struct_smobs, id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("request free game struct smob \"%s\" but assoc is NULL"),
		      id);
	}
      LW6SYS_FREE (id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
print_game_struct (SCM game_struct, SCM port, scm_print_state * pstate)
{
  lw6ker_game_struct_t *c_game_struct = lw6_scm_to_game_struct (game_struct);
  char *repr = NULL;

  repr = lw6ker_game_struct_repr (c_game_struct);

  scm_puts ("#<" SMOB_TYPE_GAME_STRUCT " ", port);
  if (repr)
    {
      scm_display (scm_makfrom0str (repr), port);
      LW6SYS_FREE (repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_game_struct
 *
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
lw6_make_scm_game_struct (lw6ker_game_struct_t * c_game_struct, SCM map)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_game_struct_smob_t *game_struct_smob = NULL;

  scm_gc ();

  game_struct_smob =
    (lw6_game_struct_smob_t *)
    LW6SYS_CALLOC (sizeof (lw6_game_struct_smob_t));
  if (game_struct_smob)
    {
      game_struct_smob->c_game_struct = c_game_struct;
      game_struct_smob->map = map;
      id = smob_id (SMOB_TYPE_GAME_STRUCT, c_game_struct->id);
      if (id)
	{
	  repr = lw6ker_game_struct_repr (c_game_struct);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _("creating game struct smob \"%s\""), repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (&lw6_global.game_struct_smobs, id,
				(void *) game_struct_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (repr);
	    }
	  LW6SYS_FREE (id);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("unable to create game struct smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.game_struct, game_struct_smob);
}

/**
 * lw6_scm_to_game_struct
 *
 * @game_struct: the game_struct to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'game_struct' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6ker_game_struct_t *
lw6_scm_to_game_struct (SCM game_struct)
{
  lw6ker_game_struct_t *c_game_struct;

  c_game_struct =
    ((lw6_game_struct_smob_t *) SCM_SMOB_DATA (game_struct))->c_game_struct;

  return c_game_struct;
}

/**
 * lw6_free_game_struct_smob
 *
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
lw6_free_game_struct_smob (lw6_game_struct_smob_t * game_struct_smob)
{
  char *repr = NULL;

  repr = lw6ker_game_struct_repr (game_struct_smob->c_game_struct);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("freeing game struct smob \"%s\""),
		  repr);
      LW6SYS_FREE (repr);
    }

  lw6ker_game_struct_free (game_struct_smob->c_game_struct);
  LW6SYS_FREE (game_struct_smob);
}

/*
 * Game state smob
 */
static SCM
mark_game_state (SCM game_state)
{
  lw6_game_state_smob_t *game_state_smob =
    (lw6_game_state_smob_t *) SCM_SMOB_DATA (game_state);

  scm_gc_mark (game_state_smob->game_struct);

  return SCM_BOOL_F;
}

static size_t
free_game_state (SCM game_state)
{
  char *id = NULL;
  lw6_game_state_smob_t *game_state_smob =
    (lw6_game_state_smob_t *) SCM_SMOB_DATA (game_state);

  LW6_MUTEX_LOCK;

  lw6sys_log (LW6SYS_LOG_INFO, _("garbage collect game state smob"));
  id = smob_id (SMOB_TYPE_GAME_STATE, game_state_smob->c_game_state->id);
  if (id)
    {
      if (lw6_global.game_state_smobs)
	{
	  lw6sys_log (LW6SYS_LOG_INFO,
		      _("request free game state smob \"%s\""), id);
	  lw6sys_assoc_unset (lw6_global.game_state_smobs, id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("request free game state smob \"%s\" but assoc is NULL"),
		      id);
	}
      LW6SYS_FREE (id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
print_game_state (SCM game_state, SCM port, scm_print_state * pstate)
{
  lw6ker_game_state_t *c_game_state = lw6_scm_to_game_state (game_state);
  char *repr = NULL;

  repr = lw6ker_game_state_repr (c_game_state);

  scm_puts ("#<" SMOB_TYPE_GAME_STATE " ", port);
  if (repr)
    {
      scm_display (scm_makfrom0str (repr), port);
      LW6SYS_FREE (repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_game_state
 *
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
lw6_make_scm_game_state (lw6ker_game_state_t * c_game_state, SCM game_struct)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_game_state_smob_t *game_state_smob = NULL;

  scm_gc ();

  game_state_smob =
    (lw6_game_state_smob_t *) LW6SYS_CALLOC (sizeof (lw6_game_state_smob_t));
  if (game_state_smob)
    {
      game_state_smob->c_game_state = c_game_state;
      game_state_smob->game_struct = game_struct;
      id = smob_id (SMOB_TYPE_GAME_STATE, c_game_state->id);
      if (id)
	{
	  repr = lw6ker_game_state_repr (c_game_state);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_INFO,
			  _("creating game state smob \"%s\""), repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (&lw6_global.game_state_smobs, id,
				(void *) game_state_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (repr);
	    }
	  LW6SYS_FREE (id);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("unable to create game state smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.game_state, game_state_smob);
}

/**
 * lw6_scm_to_game_state
 *
 * @game_state: the game_state to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'game_state' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6ker_game_state_t *
lw6_scm_to_game_state (SCM game_state)
{
  lw6ker_game_state_t *c_game_state;

  c_game_state =
    ((lw6_game_state_smob_t *) SCM_SMOB_DATA (game_state))->c_game_state;

  return c_game_state;
}

/**
 * lw6_free_game_state_smob
 *
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
lw6_free_game_state_smob (lw6_game_state_smob_t * game_state_smob)
{
  char *repr = NULL;

  repr = lw6ker_game_state_repr (game_state_smob->c_game_state);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("freeing game state smob \"%s\""), repr);
      LW6SYS_FREE (repr);
    }

  lw6ker_game_state_free (game_state_smob->c_game_state);
  LW6SYS_FREE (game_state_smob);
}

/*
 * Game pilot smob
 */
static SCM
mark_pilot (SCM pilot)
{
  return SCM_BOOL_F;
}

static size_t
free_pilot (SCM pilot)
{
  char *id = NULL;
  lw6_pilot_smob_t *pilot_smob = (lw6_pilot_smob_t *) SCM_SMOB_DATA (pilot);

  LW6_MUTEX_LOCK;

  lw6sys_log (LW6SYS_LOG_INFO, _("garbage collect pilot smob"));
  id = smob_id (SMOB_TYPE_PILOT, pilot_smob->c_pilot->id);
  if (id)
    {
      if (lw6_global.pilot_smobs)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("request free pilot smob \"%s\""),
		      id);
	  lw6sys_assoc_unset (lw6_global.pilot_smobs, id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("request free pilot smob \"%s\" but assoc is NULL"),
		      id);
	}
      LW6SYS_FREE (id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
print_pilot (SCM pilot, SCM port, scm_print_state * pstate)
{
  lw6pil_pilot_t *c_pilot = lw6_scm_to_pilot (pilot);
  char *repr = NULL;

  repr = lw6pil_pilot_repr (c_pilot);

  scm_puts ("#<" SMOB_TYPE_PILOT " ", port);
  if (repr)
    {
      scm_display (scm_makfrom0str (repr), port);
      LW6SYS_FREE (repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_pilot
 *
 * @c_pilot: the pilot object
 *
 * Creates an SCM 'pilot' object from C data.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_pilot (lw6pil_pilot_t * c_pilot)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_pilot_smob_t *pilot_smob = NULL;

  scm_gc ();

  pilot_smob = (lw6_pilot_smob_t *) LW6SYS_CALLOC (sizeof (lw6_pilot_smob_t));
  if (pilot_smob)
    {
      pilot_smob->c_pilot = c_pilot;
      id = smob_id (SMOB_TYPE_PILOT, c_pilot->id);
      if (id)
	{
	  repr = lw6pil_pilot_repr (c_pilot);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_INFO, _("creating pilot smob \"%s\""),
			  repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (&lw6_global.pilot_smobs, id,
				(void *) pilot_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (repr);
	    }
	  LW6SYS_FREE (id);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("unable to create pilot smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.pilot, pilot_smob);
}

/**
 * lw6_scm_to_pilot
 *
 * @pilot: the pilot to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'pilot' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6pil_pilot_t *
lw6_scm_to_pilot (SCM pilot)
{
  lw6pil_pilot_t *c_pilot;

  c_pilot = ((lw6_pilot_smob_t *) SCM_SMOB_DATA (pilot))->c_pilot;

  return c_pilot;
}

/**
 * lw6_free_pilot_smob
 *
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
lw6_free_pilot_smob (lw6_pilot_smob_t * pilot_smob)
{
  char *repr = NULL;

  repr = lw6pil_pilot_repr (pilot_smob->c_pilot);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("freeing pilot smob \"%s\""), repr);
      LW6SYS_FREE (repr);
    }

  lw6pil_pilot_free (pilot_smob->c_pilot);
  LW6SYS_FREE (pilot_smob);
}

/*
 * Game bot smob
 */
static SCM
mark_bot (SCM bot)
{
  lw6_bot_smob_t *bot_smob = (lw6_bot_smob_t *) SCM_SMOB_DATA (bot);

  scm_gc_mark (bot_smob->game_state);
  scm_gc_mark (bot_smob->pilot);

  return SCM_BOOL_F;
}

static size_t
free_bot (SCM bot)
{
  char *id = NULL;
  lw6_bot_smob_t *bot_smob = (lw6_bot_smob_t *) SCM_SMOB_DATA (bot);

  LW6_MUTEX_LOCK;

  lw6sys_log (LW6SYS_LOG_INFO, _("garbage collect bot smob"));
  id = smob_id (SMOB_TYPE_BOT, bot_smob->c_bot->id);
  if (id)
    {
      if (lw6_global.bot_smobs)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("request free bot smob \"%s\""), id);
	  lw6sys_assoc_unset (lw6_global.bot_smobs, id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("request free bot smob \"%s\" but assoc is NULL"),
		      id);
	}
      LW6SYS_FREE (id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
print_bot (SCM bot, SCM port, scm_print_state * pstate)
{
  lw6bot_backend_t *c_bot = lw6_scm_to_bot (bot);
  char *repr = NULL;

  repr = lw6bot_repr (c_bot);

  scm_puts ("#<" SMOB_TYPE_BOT " ", port);
  if (repr)
    {
      scm_display (scm_makfrom0str (repr), port);
      LW6SYS_FREE (repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_bot
 *
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
lw6_make_scm_bot (lw6bot_backend_t * c_bot, SCM game_state, SCM pilot)
{
// c_bot is supposed to have been initialized with lw6bot_init()
  char *repr = NULL;
  char *id = NULL;
  lw6_bot_smob_t *bot_smob = NULL;

  scm_gc ();

  bot_smob = (lw6_bot_smob_t *) LW6SYS_CALLOC (sizeof (lw6_bot_smob_t));
  if (bot_smob)
    {
      bot_smob->c_bot = c_bot;
      bot_smob->game_state = game_state;
      bot_smob->pilot = pilot;
      id = smob_id (SMOB_TYPE_BOT, c_bot->id);
      if (id)
	{
	  repr = lw6bot_repr (c_bot);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_INFO, _("creating bot smob \"%s\""),
			  repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (&lw6_global.bot_smobs, id, (void *) bot_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (repr);
	    }
	  LW6SYS_FREE (id);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("unable to create bot smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.bot, bot_smob);
}

/**
 * lw6_scm_to_bot
 *
 * @bot: the bot to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'bot' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6bot_backend_t *
lw6_scm_to_bot (SCM bot)
{
  lw6bot_backend_t *c_bot;

  c_bot = ((lw6_bot_smob_t *) SCM_SMOB_DATA (bot))->c_bot;

  return c_bot;
}

/**
 * lw6_free_bot_smob
 *
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
lw6_free_bot_smob (lw6_bot_smob_t * bot_smob)
{
  char *repr = NULL;

  repr = lw6bot_repr (bot_smob->c_bot);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("freeing bot smob \"%s\""), repr);
      LW6SYS_FREE (repr);
    }

  lw6bot_quit (bot_smob->c_bot);
  lw6bot_destroy_backend (bot_smob->c_bot);
  LW6SYS_FREE (bot_smob);
}

/*
 * Game look smob
 */
static SCM
mark_look (SCM look)
{
  return SCM_BOOL_F;
}

static size_t
free_look (SCM look)
{
  char *id = NULL;
  lw6_look_smob_t *look_smob = (lw6_look_smob_t *) SCM_SMOB_DATA (look);

  LW6_MUTEX_LOCK;

  lw6sys_log (LW6SYS_LOG_INFO, _("garbage collect look smob"));
  id = smob_id (SMOB_TYPE_LOOK, look_smob->c_look->id);
  if (id)
    {
      if (lw6_global.look_smobs)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("request free look smob \"%s\""),
		      id);
	  lw6sys_assoc_unset (lw6_global.look_smobs, id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("request free look smob \"%s\" but assoc is NULL"),
		      id);
	}
      LW6SYS_FREE (id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
print_look (SCM look, SCM port, scm_print_state * pstate)
{
  lw6gui_look_t *c_look = lw6_scm_to_look (look);
  char *repr = NULL;

  repr = lw6gui_look_repr (c_look);

  scm_puts ("#<" SMOB_TYPE_LOOK " ", port);
  if (repr)
    {
      scm_display (scm_makfrom0str (repr), port);
      LW6SYS_FREE (repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_look
 *
 * @c_look: the look object
 *
 * Creates an SCM 'look' object from C data.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_look (lw6gui_look_t * c_look)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_look_smob_t *look_smob = NULL;

  scm_gc ();

  look_smob = (lw6_look_smob_t *) LW6SYS_CALLOC (sizeof (lw6_look_smob_t));
  if (look_smob)
    {
      look_smob->c_look = c_look;
      id = smob_id (SMOB_TYPE_LOOK, c_look->id);
      if (id)
	{
	  repr = lw6gui_look_repr (c_look);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_INFO, _("creating look smob \"%s\""),
			  repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (&lw6_global.look_smobs, id,
				(void *) look_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (repr);
	    }
	  LW6SYS_FREE (id);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("unable to create look smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.look, look_smob);
}

/**
 * lw6_scm_to_look
 *
 * @look: the look to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'look' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6gui_look_t *
lw6_scm_to_look (SCM look)
{
  lw6gui_look_t *c_look;

  c_look = ((lw6_look_smob_t *) SCM_SMOB_DATA (look))->c_look;

  return c_look;
}

/**
 * lw6_free_look_smob
 *
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
lw6_free_look_smob (lw6_look_smob_t * look_smob)
{
  char *repr = NULL;

  repr = lw6gui_look_repr (look_smob->c_look);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("freeing look smob \"%s\""), repr);
      LW6SYS_FREE (repr);
    }

  lw6gui_look_free (look_smob->c_look);
  LW6SYS_FREE (look_smob);
}

/*
 * Loader smob
 */
static SCM
mark_loader (SCM loader)
{
  return SCM_BOOL_F;
}

static size_t
free_loader (SCM loader)
{
  char *id = NULL;
  lw6_loader_smob_t *loader_smob =
    (lw6_loader_smob_t *) SCM_SMOB_DATA (loader);

  LW6_MUTEX_LOCK;

  lw6sys_log (LW6SYS_LOG_INFO, _("garbage collect loader smob"));
  id = smob_id (SMOB_TYPE_LOADER, loader_smob->c_loader->id);
  if (id)
    {
      if (lw6_global.loader_smobs)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("request free loader smob \"%s\""),
		      id);
	  lw6sys_assoc_unset (lw6_global.loader_smobs, id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("request free loader smob \"%s\" but assoc is NULL"),
		      id);
	}
      LW6SYS_FREE (id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
print_loader (SCM loader, SCM port, scm_print_state * pstate)
{
  lw6tsk_loader_t *c_loader = lw6_scm_to_loader (loader);
  char *repr = NULL;

  repr = lw6tsk_loader_repr (c_loader);

  scm_puts ("#<" SMOB_TYPE_LOADER " ", port);
  if (repr)
    {
      scm_display (scm_makfrom0str (repr), port);
      LW6SYS_FREE (repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_loader
 *
 * @c_loader: the loader object
 *
 * Creates an SCM 'loader' object from C data.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_loader (lw6tsk_loader_t * c_loader)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_loader_smob_t *loader_smob = NULL;

  scm_gc ();

  loader_smob =
    (lw6_loader_smob_t *) LW6SYS_CALLOC (sizeof (lw6_loader_smob_t));
  if (loader_smob)
    {
      loader_smob->c_loader = c_loader;
      id = smob_id (SMOB_TYPE_LOADER, c_loader->id);
      if (id)
	{
	  repr = lw6tsk_loader_repr (c_loader);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_INFO, _("creating loader smob \"%s\""),
			  repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (&lw6_global.loader_smobs, id,
				(void *) loader_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (repr);
	    }
	  LW6SYS_FREE (id);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("unable to create loader smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.loader, loader_smob);
}

/**
 * lw6_scm_to_loader
 *
 * @loader: the loader to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'loader' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6tsk_loader_t *
lw6_scm_to_loader (SCM loader)
{
  lw6tsk_loader_t *c_loader;

  c_loader = ((lw6_loader_smob_t *) SCM_SMOB_DATA (loader))->c_loader;

  return c_loader;
}

/**
 * lw6_free_loader_smob
 *
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
lw6_free_loader_smob (lw6_loader_smob_t * loader_smob)
{
  char *repr = NULL;

  repr = lw6tsk_loader_repr (loader_smob->c_loader);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("freeing loader smob \"%s\""), repr);
      LW6SYS_FREE (repr);
    }

  lw6tsk_loader_free (loader_smob->c_loader);
  LW6SYS_FREE (loader_smob);
}

/*
 * Db smob
 */
static SCM
mark_db (SCM db)
{
  return SCM_BOOL_F;
}

static size_t
free_db (SCM db)
{
  char *id = NULL;
  lw6_db_smob_t *db_smob = (lw6_db_smob_t *) SCM_SMOB_DATA (db);

  LW6_MUTEX_LOCK;

  lw6sys_log (LW6SYS_LOG_INFO, _("garbage collect db smob"));
  id = smob_id (SMOB_TYPE_DB, db_smob->c_db->id);
  if (id)
    {
      if (lw6_global.db_smobs)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("request free db smob \"%s\""), id);
	  lw6sys_assoc_unset (lw6_global.db_smobs, id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("request free db smob \"%s\" but assoc is NULL"), id);
	}
      LW6SYS_FREE (id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
print_db (SCM db, SCM port, scm_print_state * pstate)
{
  lw6p2p_db_t *c_db = lw6_scm_to_db (db);
  char *repr = NULL;

  repr = lw6p2p_db_repr (c_db);

  scm_puts ("#<" SMOB_TYPE_DB " ", port);
  if (repr)
    {
      scm_display (scm_makfrom0str (repr), port);
      LW6SYS_FREE (repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_db
 *
 * @c_db: the database object
 *
 * Creates an SCM 'db' object from C data.
 *
 * Return value: the SCM object
 */
SCM
lw6_make_scm_db (lw6p2p_db_t * c_db)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_db_smob_t *db_smob = NULL;

  scm_gc ();

  db_smob = (lw6_db_smob_t *) LW6SYS_CALLOC (sizeof (lw6_db_smob_t));
  if (db_smob)
    {
      db_smob->c_db = c_db;
      id = smob_id (SMOB_TYPE_DB, c_db->id);
      if (id)
	{
	  repr = lw6p2p_db_repr (c_db);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_INFO, _("creating db smob \"%s\""),
			  repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (&lw6_global.db_smobs, id, (void *) db_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (repr);
	    }
	  LW6SYS_FREE (id);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("unable to create db smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.db, db_smob);
}

/**
 * lw6_scm_to_db
 *
 * @db: the db to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'db' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6p2p_db_t *
lw6_scm_to_db (SCM db)
{
  void *c_db;

  c_db = ((lw6_db_smob_t *) SCM_SMOB_DATA (db))->c_db;

  return c_db;
}

/**
 * lw6_free_db_smob
 *
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
lw6_free_db_smob (lw6_db_smob_t * db_smob)
{
  char *repr = NULL;

  repr = lw6p2p_db_repr (db_smob->c_db);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("freeing db smob \"%s\""), repr);
      LW6SYS_FREE (repr);
    }

  lw6p2p_db_close (db_smob->c_db);
  LW6SYS_FREE (db_smob);
}

/*
 * Node smob
 */
static SCM
mark_node (SCM node)
{
  lw6_node_smob_t *node_smob = (lw6_node_smob_t *) SCM_SMOB_DATA (node);

  scm_gc_mark (node_smob->db);

  return SCM_BOOL_F;
}

static size_t
free_node (SCM node)
{
  char *id = NULL;
  lw6_node_smob_t *node_smob = (lw6_node_smob_t *) SCM_SMOB_DATA (node);

  LW6_MUTEX_LOCK;

  lw6sys_log (LW6SYS_LOG_INFO, _("garbage collect node smob"));
  id = smob_id (SMOB_TYPE_NODE, node_smob->c_node->id);
  if (id)
    {
      if (lw6_global.node_smobs)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("request free node smob \"%s\""),
		      id);
	  lw6sys_assoc_unset (lw6_global.node_smobs, id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("request free node smob \"%s\" but assoc is NULL"),
		      id);
	}
      LW6SYS_FREE (id);
    }

  LW6_MUTEX_UNLOCK;

  return 0;
}

static int
print_node (SCM node, SCM port, scm_print_state * pstate)
{
  lw6p2p_node_t *c_node = lw6_scm_to_node (node);
  char *repr = NULL;

  repr = lw6p2p_node_repr (c_node);

  scm_puts ("#<" SMOB_TYPE_NODE " ", port);
  if (repr)
    {
      scm_display (scm_makfrom0str (repr), port);
      LW6SYS_FREE (repr);
    }
  scm_puts (">", port);

  return 1;
}

/**
 * lw6_make_scm_node
 *
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
lw6_make_scm_node (lw6p2p_node_t * c_node, SCM db)
{
  char *repr = NULL;
  char *id = NULL;
  lw6_node_smob_t *node_smob = NULL;

  scm_gc ();

  node_smob = (lw6_node_smob_t *) LW6SYS_CALLOC (sizeof (lw6_node_smob_t));
  if (node_smob)
    {
      node_smob->c_node = c_node;
      node_smob->db = db;

      id = smob_id (SMOB_TYPE_NODE, c_node->id);
      if (id)
	{
	  repr = lw6p2p_node_repr (c_node);
	  if (repr)
	    {
	      lw6sys_log (LW6SYS_LOG_INFO, _("creating node smob \"%s\""),
			  repr);
	      LW6_MUTEX_LOCK;
	      lw6sys_assoc_set (&lw6_global.node_smobs, id,
				(void *) node_smob);
	      LW6_MUTEX_UNLOCK;
	      LW6SYS_FREE (repr);
	    }
	  LW6SYS_FREE (id);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _("unable to create node smob, expect trouble"));
    }

  SCM_RETURN_NEWSMOB (lw6_global.smob_types.node, node_smob);
}

/**
 * lw6_scm_to_node
 *
 * @node: the node to convert (SCM object)
 *
 * Gets the internal C pointer corresponding to the
 * scheme 'node' object.
 *
 * Return value: a pointer, *not* a copy, must not be freed
 */
lw6p2p_node_t *
lw6_scm_to_node (SCM node)
{
  void *c_node;

  c_node = ((lw6_node_smob_t *) SCM_SMOB_DATA (node))->c_node;

  return c_node;
}

/**
 * lw6_free_node_smob
 *
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
lw6_free_node_smob (lw6_node_smob_t * node_smob)
{
  char *repr = NULL;

  repr = lw6p2p_node_repr (node_smob->c_node);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_INFO, _("freeing node smob \"%s\""), repr);
      LW6SYS_FREE (repr);
    }

  lw6p2p_node_free (node_smob->c_node);
  LW6SYS_FREE (node_smob);
}

/**
 * lw6_register_smobs
 *
 * Register all smobs to Guile.
 *
 * Return value: 1 on success, 0 if failed.
 */
int
lw6_register_smobs ()
{
  int ret = 1;

  lw6_global.smob_types.dsp =
    scm_make_smob_type (SMOB_TYPE_DSP, sizeof (lw6_dsp_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.dsp, mark_dsp);
  scm_set_smob_free (lw6_global.smob_types.dsp, free_dsp);
  scm_set_smob_print (lw6_global.smob_types.dsp, print_dsp);

  lw6_global.smob_types.snd =
    scm_make_smob_type (SMOB_TYPE_SND, sizeof (lw6_snd_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.snd, mark_snd);
  scm_set_smob_free (lw6_global.smob_types.snd, free_snd);
  scm_set_smob_print (lw6_global.smob_types.snd, print_snd);

  lw6_global.smob_types.map =
    scm_make_smob_type (SMOB_TYPE_MAP, sizeof (lw6_map_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.map, mark_map);
  scm_set_smob_free (lw6_global.smob_types.map, free_map);
  scm_set_smob_print (lw6_global.smob_types.map, print_map);

  lw6_global.smob_types.menu =
    scm_make_smob_type (SMOB_TYPE_MENU, sizeof (lw6_menu_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.menu, mark_menu);
  scm_set_smob_free (lw6_global.smob_types.menu, free_menu);
  scm_set_smob_print (lw6_global.smob_types.menu, print_menu);

  lw6_global.smob_types.game_struct =
    scm_make_smob_type (SMOB_TYPE_GAME_STRUCT,
			sizeof (lw6_game_struct_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.game_struct, mark_game_struct);
  scm_set_smob_free (lw6_global.smob_types.game_struct, free_game_struct);
  scm_set_smob_print (lw6_global.smob_types.game_struct, print_game_struct);

  lw6_global.smob_types.game_state =
    scm_make_smob_type (SMOB_TYPE_GAME_STATE, sizeof (lw6_game_state_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.game_state, mark_game_state);
  scm_set_smob_free (lw6_global.smob_types.game_state, free_game_state);
  scm_set_smob_print (lw6_global.smob_types.game_state, print_game_state);

  lw6_global.smob_types.pilot =
    scm_make_smob_type (SMOB_TYPE_PILOT, sizeof (lw6_pilot_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.pilot, mark_pilot);
  scm_set_smob_free (lw6_global.smob_types.pilot, free_pilot);
  scm_set_smob_print (lw6_global.smob_types.pilot, print_pilot);

  lw6_global.smob_types.bot =
    scm_make_smob_type (SMOB_TYPE_BOT, sizeof (lw6_bot_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.bot, mark_bot);
  scm_set_smob_free (lw6_global.smob_types.bot, free_bot);
  scm_set_smob_print (lw6_global.smob_types.bot, print_bot);

  lw6_global.smob_types.look =
    scm_make_smob_type (SMOB_TYPE_LOOK, sizeof (lw6_look_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.look, mark_look);
  scm_set_smob_free (lw6_global.smob_types.look, free_look);
  scm_set_smob_print (lw6_global.smob_types.look, print_look);

  lw6_global.smob_types.loader =
    scm_make_smob_type (SMOB_TYPE_LOADER, sizeof (lw6_loader_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.loader, mark_loader);
  scm_set_smob_free (lw6_global.smob_types.loader, free_loader);
  scm_set_smob_print (lw6_global.smob_types.loader, print_loader);

  lw6_global.smob_types.db =
    scm_make_smob_type (SMOB_TYPE_DB, sizeof (lw6_db_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.db, mark_db);
  scm_set_smob_free (lw6_global.smob_types.db, free_db);
  scm_set_smob_print (lw6_global.smob_types.db, print_db);

  lw6_global.smob_types.node =
    scm_make_smob_type (SMOB_TYPE_NODE, sizeof (lw6_node_smob_t));
  scm_set_smob_mark (lw6_global.smob_types.node, mark_node);
  scm_set_smob_free (lw6_global.smob_types.node, free_node);
  scm_set_smob_print (lw6_global.smob_types.node, print_node);

  return ret;
}
