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

void
_release_dsp_quit (void *func_data, char *key, void *value)
{
  lw6_dsp_smob_t *dsp_smob;
  char *repr = NULL;

  dsp_smob = (lw6_dsp_smob_t *) value;
  if (dsp_smob && dsp_smob->c_dsp)
    {
      repr = lw6dsp_repr (dsp_smob->c_dsp);
      if (repr)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("release dsp \"%s\""), repr);
	  LW6SYS_FREE (repr);
	}
      lw6dsp_quit (dsp_smob->c_dsp);
    }
}

void
_release_snd_quit (void *func_data, char *key, void *value)
{
  lw6_snd_smob_t *snd_smob;
  char *repr = NULL;

  snd_smob = (lw6_snd_smob_t *) value;
  if (snd_smob && snd_smob->c_snd)
    {
      repr = lw6snd_repr (snd_smob->c_snd);
      if (repr)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("release snd \"%s\""), repr);
	  LW6SYS_FREE (repr);
	}
      lw6snd_quit (snd_smob->c_snd);
    }
}


/**
 * lw6_release
 *
 * Functions which will call @quit(), @free(), @destroy() on whatever
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
lw6_release ()
{
  if (lw6_global.dsp_smobs)
    {
      lw6sys_assoc_map (lw6_global.dsp_smobs, _release_dsp_quit, NULL);
    }
  if (lw6_global.snd_smobs)
    {
      lw6sys_assoc_map (lw6_global.snd_smobs, _release_snd_quit, NULL);
    }
}

void
_exit_send_quit (void *func_data, char *key, void *value)
{
  lw6_dsp_smob_t *dsp_smob;
  int *sent;
  char *repr = NULL;

  dsp_smob = (lw6_dsp_smob_t *) value;
  sent = (int *) func_data;
  if (dsp_smob && dsp_smob->c_dsp)
    {
      repr = lw6dsp_repr (dsp_smob->c_dsp);
      if (repr)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("send quit to \"%s\""), repr);
	  LW6SYS_FREE (repr);
	}
      lw6gui_input_send_quit (dsp_smob->c_dsp->input);
      if (sent)
	{
	  (*sent) = 1;
	}
    }
}

/**
 * lw6_exit
 *
 * Will exit the program. If there are some display objects alive,
 * that is if there's a display activated, it will find them and
 * send a quit event, giving a chance to the to exit cleanly.
 * If not, it will just call @exit() directly.
 *
 * Return value: none
 */
void
lw6_exit ()
{
  int sent = 0;

  printf ("\n");

  if (lw6_global.dsp_smobs)
    {
      lw6sys_assoc_map (lw6_global.dsp_smobs, _exit_send_quit, &sent);
    }

  if (!sent)
    {
      exit (0);
    }
}
