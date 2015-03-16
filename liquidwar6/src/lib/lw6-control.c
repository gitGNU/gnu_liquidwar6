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

void
_release_dsp_quit (lw6sys_context_t * sys_context, void *func_data, const char *key, void *value)
{
  lw6_dsp_smob_t *dsp_smob;
  char *repr = NULL;

  dsp_smob = (lw6_dsp_smob_t *) value;
  if (dsp_smob && dsp_smob->c_dsp)
    {
      repr = lw6dsp_repr (sys_context, dsp_smob->c_dsp);
      if (repr)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("release dsp \"%s\""), repr);
	  LW6SYS_FREE (sys_context, repr);
	}
      lw6dsp_quit (sys_context, dsp_smob->c_dsp);
    }
}

void
_release_snd_quit (lw6sys_context_t * sys_context, void *func_data, const char *key, void *value)
{
  lw6_snd_smob_t *snd_smob;
  char *repr = NULL;

  snd_smob = (lw6_snd_smob_t *) value;
  if (snd_smob && snd_smob->c_snd)
    {
      repr = lw6snd_repr (sys_context, snd_smob->c_snd);
      if (repr)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("release snd \"%s\""), repr);
	  LW6SYS_FREE (sys_context, repr);
	}
      lw6snd_quit (sys_context, snd_smob->c_snd);
    }
}

void
_release_node_close (lw6sys_context_t * sys_context, void *func_data, const char *key, void *value)
{
  lw6_node_smob_t *node_smob;
  char *repr = NULL;

  node_smob = (lw6_node_smob_t *) value;
  if (node_smob && node_smob->c_node)
    {
      repr = lw6p2p_node_repr (sys_context, node_smob->c_node);
      if (repr)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("release node \"%s\""), repr);
	  LW6SYS_FREE (sys_context, repr);
	}
      lw6p2p_node_close (sys_context, node_smob->c_node);
    }
}


/**
 * lw6_release
 *
 * @sys_context: global system context
 *
 * Functions which will call @quit, @free, @destroy on whatever
 * smob object that has threads and/or requires hardware ressources.
 * This is to be called before the Guile interpreter ends. This is because
 * when it garbage collects objects at the end of the program, it has
 * no idea of what order to use when freeing objects. So if an object
 * which uses another one in a thread is freed after the other is freed,
 * you get a (rather unexplainabled if not warned) segfault.
 *
 * Return value: none
 */
void
lw6_release (lw6sys_context_t * sys_context)
{
  if (lw6_global.dsp_smobs)
    {
      lw6sys_assoc_map (sys_context, lw6_global.dsp_smobs, _release_dsp_quit, NULL);
    }
  if (lw6_global.snd_smobs)
    {
      lw6sys_assoc_map (sys_context, lw6_global.snd_smobs, _release_snd_quit, NULL);
    }
  if (lw6_global.node_smobs)
    {
      lw6sys_assoc_map (sys_context, lw6_global.node_smobs, _release_node_close, NULL);
    }
}

/**
 * lw6_exit
 *
 * @sys_context: global system context
 *
 * Sends a quit message and displays a newline.
 *
 * Return value: none
 */
void
lw6_exit (lw6sys_context_t * sys_context)
{
  printf ("\n");
  lw6sys_signal_send_quit (sys_context);
}

/**
 * lw6_set_ret
 *
 * @sys_context: global system context
 * @ret: return value to set, 1 for success, 0 for failure
 *
 * Sets the ret value for the script.
 *
 * Return value: none
 */
void
lw6_set_ret (lw6sys_context_t * sys_context, int ret)
{
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("set script ret=%d"), ret);
  lw6_global.ret = ret ? 1 : 0;
}

/**
 * lw6_get_ret
 *
 * @sys_context: global system context
 *
 * Get the ret value for the script.
 *
 * Return value: 1 if success, 0 if not.
 */
int
lw6_get_ret (lw6sys_context_t * sys_context)
{
  return (lw6_global.ret ? 1 : 0);
}
