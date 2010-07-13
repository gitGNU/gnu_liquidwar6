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
 * argc: number of args as passed to main
 * argv: array of strings as passed to main
 *
 * Initializes global values to their defaults.
 *
 * Return value: 1 on success, 0 if failed
 */
int
lw6_init_global (int argc, char *argv[])
{
  int ret = 0;

  memset (&lw6_global, 0, sizeof (lw6_global_t));

  pthread_mutex_init (&lw6_global.mutex, NULL);

  lw6_global.start_time = time (NULL);
  lw6_global.argc = argc;
  lw6_global.argv = argv;
  lw6_global.progress = 0.0f;

  LW6_MUTEX_LOCK;

  ret =
    ((lw6_global.dsp_smobs =
      lw6sys_assoc_new ((void (*)(void *)) lw6_free_dsp_smob)) != NULL) &&
    ((lw6_global.snd_smobs =
      lw6sys_assoc_new ((void (*)(void *)) lw6_free_snd_smob)) != NULL) &&
    ((lw6_global.map_smobs =
      lw6sys_assoc_new ((void (*)(void *)) lw6_free_map_smob)) != NULL) &&
    ((lw6_global.menu_smobs =
      lw6sys_assoc_new ((void (*)(void *)) lw6_free_menu_smob)) != NULL) &&
    ((lw6_global.game_struct_smobs =
      lw6sys_assoc_new ((void (*)(void *)) lw6_free_game_struct_smob)) !=
     NULL)
    &&
    ((lw6_global.game_state_smobs =
      lw6sys_assoc_new ((void (*)(void *)) lw6_free_game_state_smob)) != NULL)
    &&
    ((lw6_global.pilot_smobs =
      lw6sys_assoc_new ((void (*)(void *)) lw6_free_pilot_smob)) != NULL)
    &&
    ((lw6_global.bot_smobs =
      lw6sys_assoc_new ((void (*)(void *)) lw6_free_bot_smob)) != NULL)
    &&
    ((lw6_global.look_smobs =
      lw6sys_assoc_new ((void (*)(void *)) lw6_free_look_smob)) != NULL)
    &&
    ((lw6_global.loader_smobs =
      lw6sys_assoc_new ((void (*)(void *)) lw6_free_loader_smob)) != NULL) &&
    ((lw6_global.db_smobs =
      lw6sys_assoc_new ((void (*)(void *)) lw6_free_db_smob)) != NULL) &&
    ((lw6_global.node_smobs =
      lw6sys_assoc_new ((void (*)(void *)) lw6_free_node_smob)) != NULL);

  LW6_MUTEX_UNLOCK;

  return ret;
}

static void
quit_cfg ()
{
  if (lw6_global.cfg_initialized && (lw6_global.cfg_context != NULL))
    {
      lw6cfg_quit (lw6_global.cfg_context);
    }
  lw6_global.cfg_context = NULL;
  lw6_global.cfg_initialized = 0;
}

static void
quit_cns ()
{
  if (lw6_global.cns_initialized)
    {
      lw6cns_handler_remove ();
    }
  lw6_global.cns_initialized = 0;
}

static void
quit_net ()
{
  if (lw6_global.net_initialized)
    {
      lw6net_quit ();
    }
  lw6_global.net_initialized = 0;
}

/**
 * lw6_quit_global
 * 
 * argc: number of args as passed to main
 * argv: array of strings as passed to main
 *
 * Frees global values. Will also garbage collect objects
 * in case Guile failed to do it perfectly (or we failed to
 * tell Guile how to do it).
 *
 * Return value: none.
 */
void
lw6_quit_global ()
{
  lw6sys_log (LW6SYS_LOG_INFO, _("final garbage collection"));

  LW6_MUTEX_LOCK;

  /*
   * Should the Guile GC be perfect, not one single object
   * would be freed here.
   */
  lw6sys_assoc_free (lw6_global.node_smobs);
  lw6_global.node_smobs = NULL;
  lw6sys_assoc_free (lw6_global.db_smobs);
  lw6_global.db_smobs = NULL;
  lw6sys_assoc_free (lw6_global.loader_smobs);
  lw6_global.loader_smobs = NULL;
  lw6sys_assoc_free (lw6_global.look_smobs);
  lw6_global.look_smobs = NULL;
  lw6sys_assoc_free (lw6_global.bot_smobs);
  lw6_global.bot_smobs = NULL;
  lw6sys_assoc_free (lw6_global.pilot_smobs);
  lw6_global.pilot_smobs = NULL;
  lw6sys_assoc_free (lw6_global.game_state_smobs);
  lw6_global.game_state_smobs = NULL;
  lw6sys_assoc_free (lw6_global.game_struct_smobs);
  lw6_global.game_struct_smobs = NULL;
  lw6sys_assoc_free (lw6_global.menu_smobs);
  lw6_global.menu_smobs = NULL;
  lw6sys_assoc_free (lw6_global.map_smobs);
  lw6_global.map_smobs = NULL;
  lw6sys_assoc_free (lw6_global.snd_smobs);
  lw6_global.snd_smobs = NULL;
  lw6sys_assoc_free (lw6_global.dsp_smobs);
  lw6_global.dsp_smobs = NULL;

  LW6_MUTEX_UNLOCK;

  quit_cns ();
  quit_net ();
  quit_cfg ();

  pthread_mutex_destroy (&lw6_global.mutex);

  memset (&lw6_global, 0, sizeof (lw6_global_t));
}
