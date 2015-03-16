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

#include <string.h>
#include <time.h>

#include "liquidwar6.h"

/*
 * This structure holds all the global variables in LW6.
 * Why do we do this when we state everywhere we do *not*
 * want to use globals? Well, sometimes it just makes
 * sense to use globals. For instance argc and argv are
 * truely application-wide. Note that should we decide
 * to use no globals at all and pass everything as a
 * parameter, having it already in a struct is like
 * having done most of the job already...
 */
lw6_global_t lw6_global;

/**
 * lw6_init_global
 *
 * @sys_context: global system context
 * @argc: number of args as passed to main
 * @argv: array of strings as passed to main
 *
 * Initializes global values to their defaults.
 *
 * Return value: 1 on success, 0 if failed
 */
int
lw6_init_global (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  int ret = 0;
  lw6sys_list_t *funcs = NULL;

  memset (&lw6_global, 0, sizeof (lw6_global_t));

  pthread_mutex_init (&lw6_global.mutex, NULL);

  lw6_global.sys_context = sys_context;
  lw6_global.start_time = time (NULL);
  lw6_global.argc = argc;
  lw6_global.argv = argv;
  lw6_global.progress = 0.0f;

  LW6_MUTEX_LOCK;

  ret =
    ((lw6_global.dsp_smobs =
      lw6sys_assoc_new (sys_context,
			(lw6sys_free_func_t) lw6_free_dsp_smob)) != NULL)
    &&
    ((lw6_global.snd_smobs =
      lw6sys_assoc_new (sys_context,
			(lw6sys_free_func_t) lw6_free_snd_smob)) != NULL)
    &&
    ((lw6_global.map_smobs =
      lw6sys_assoc_new (sys_context,
			(lw6sys_free_func_t) lw6_free_map_smob)) != NULL)
    &&
    ((lw6_global.menu_smobs =
      lw6sys_assoc_new (sys_context,
			(lw6sys_free_func_t) lw6_free_menu_smob)) != NULL)
    &&
    ((lw6_global.game_struct_smobs =
      lw6sys_assoc_new (sys_context,
			(lw6sys_free_func_t) lw6_free_game_struct_smob)) !=
     NULL)
    &&
    ((lw6_global.game_state_smobs =
      lw6sys_assoc_new (sys_context,
			(lw6sys_free_func_t) lw6_free_game_state_smob)) != NULL)
    &&
    ((lw6_global.pilot_smobs =
      lw6sys_assoc_new (sys_context,
			(lw6sys_free_func_t) lw6_free_pilot_smob)) != NULL)
    &&
    ((lw6_global.bot_smobs =
      lw6sys_assoc_new (sys_context,
			(lw6sys_free_func_t) lw6_free_bot_smob)) != NULL)
    &&
    ((lw6_global.look_smobs =
      lw6sys_assoc_new (sys_context,
			(lw6sys_free_func_t) lw6_free_look_smob)) != NULL)
    &&
    ((lw6_global.loader_smobs =
      lw6sys_assoc_new (sys_context,
			(lw6sys_free_func_t) lw6_free_loader_smob)) != NULL)
    &&
    ((lw6_global.db_smobs =
      lw6sys_assoc_new (sys_context,
			(lw6sys_free_func_t) lw6_free_db_smob)) != NULL)
    &&
    ((lw6_global.node_smobs =
      lw6sys_assoc_new (sys_context,
			(lw6sys_free_func_t) lw6_free_node_smob)) != NULL)
    && ((lw6_global.jpeg_smobs = lw6sys_assoc_new (sys_context, (lw6sys_free_func_t) lw6_free_jpeg_smob)) != NULL);

  if (ret)
    {
      funcs = lw6hlp_list_funcs (sys_context);
      if (funcs)
	{
	  ret = ret && ((lw6_global.coverage = lw6scm_coverage_new (sys_context, funcs)) != NULL);
	  lw6sys_list_free (sys_context, funcs);
	}
      else
	{
	  ret = 0;
	}
    }

  LW6_MUTEX_UNLOCK;

  return ret;
}

static void
_quit_cfg (lw6sys_context_t * sys_context)
{
  if (lw6_global.cfg_initialized && (lw6_global.cfg_context != NULL))
    {
      lw6cfg_quit (sys_context, lw6_global.cfg_context);
    }
  lw6_global.cfg_context = NULL;
  lw6_global.cfg_initialized = 0;
}

static void
_quit_cns (lw6sys_context_t * sys_context)
{
  if (lw6_global.cns_initialized)
    {
      lw6cns_handler_remove (sys_context);
    }
  lw6_global.cns_initialized = 0;
}

static void
_quit_net (lw6sys_context_t * sys_context)
{
  if (lw6_global.net_initialized)
    {
      lw6net_quit (sys_context);
    }
  lw6_global.net_initialized = 0;
}

/**
 * lw6_quit_global
 *
 * @sys_context: global system context
 *
 * Frees global values. Will also garbage collect objects
 * in case Guile failed to do it perfectly (or we failed to
 * tell Guile how to do it). Important note: this function
 * can only be called once in a program, usually at the end.
 * If called twice, and typically, if *any* Guile code is run
 * after it, the risk is that Guile code does not find objects
 * that it should, or said differently, Guile might try to
 * manipulate stuff that has been deleted without its being
 * warned about it. In practice, libGC way of doing thing is
 * usually a good thing, since it will from time to time
 * keep objects in memory that shouldn't, so there's no point
 * in changing that, but as we are pedantic in LW6 about the
 * fact that everything at program exit must be perfectly
 * freed, the problem arises. So well, only call this once
 * at the end, else problem will show up, the usual form
 * is a segfault on the delete callback, as when Guile finally
 * runs its GC, the object does not exist any more.
 *
 * Return value: none.
 */
void
lw6_quit_global (lw6sys_context_t * sys_context)
{
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("final garbage collection"));

  if (sys_context != lw6_global.sys_context)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("global and local sys_context differ, sys_context=%p, lw6_global.sys_context=%p"), sys_context,
		  lw6_global.sys_context);
    }

  lw6scm_coverage_log (sys_context, lw6_global.coverage);
  lw6sys_hash_free (sys_context, lw6_global.coverage);
  lw6_global.coverage = NULL;

  LW6_MUTEX_LOCK;

  /*
   * Should the Guile GC be perfect, not one single object
   * would be freed here.
   */
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing remaining jpeg smobs"));
  lw6sys_assoc_free (sys_context, lw6_global.jpeg_smobs);
  lw6_global.jpeg_smobs = NULL;
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing remaining node smobs"));
  lw6sys_assoc_free (sys_context, lw6_global.node_smobs);
  lw6_global.node_smobs = NULL;
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing remaining db smobs"));
  lw6sys_assoc_free (sys_context, lw6_global.db_smobs);
  lw6_global.db_smobs = NULL;
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing remaining loader smobs"));
  lw6sys_assoc_free (sys_context, lw6_global.loader_smobs);
  lw6_global.loader_smobs = NULL;
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing remaining look smobs"));
  lw6sys_assoc_free (sys_context, lw6_global.look_smobs);
  lw6_global.look_smobs = NULL;
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing remaining bot smobs"));
  lw6sys_assoc_free (sys_context, lw6_global.bot_smobs);
  lw6_global.bot_smobs = NULL;
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing remaining pilot smobs"));
  lw6sys_assoc_free (sys_context, lw6_global.pilot_smobs);
  lw6_global.pilot_smobs = NULL;
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing remaining game_state smobs"));
  lw6sys_assoc_free (sys_context, lw6_global.game_state_smobs);
  lw6_global.game_state_smobs = NULL;
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing remaining game_struct smobs"));
  lw6sys_assoc_free (sys_context, lw6_global.game_struct_smobs);
  lw6_global.game_struct_smobs = NULL;
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing remaining menu smobs"));
  lw6sys_assoc_free (sys_context, lw6_global.menu_smobs);
  lw6_global.menu_smobs = NULL;
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing remaining map smobs"));
  lw6sys_assoc_free (sys_context, lw6_global.map_smobs);
  lw6_global.map_smobs = NULL;
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing remaining snd smobs"));
  lw6sys_assoc_free (sys_context, lw6_global.snd_smobs);
  lw6_global.snd_smobs = NULL;
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("freeing remaining dsp smobs"));
  lw6sys_assoc_free (sys_context, lw6_global.dsp_smobs);
  lw6_global.dsp_smobs = NULL;

  LW6_MUTEX_UNLOCK;

  _quit_cns (sys_context);
  _quit_net (sys_context);
  _quit_cfg (sys_context);

  pthread_mutex_destroy (&lw6_global.mutex);
  memset (&lw6_global, 0, sizeof (lw6_global_t));

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("all global stuff freed"));
}
