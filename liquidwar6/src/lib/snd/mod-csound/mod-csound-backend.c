/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013  Christian Mauduit <ufoot@ufoot.org>

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

#include "../snd.h"
#include "mod-csound.h"
#include "mod-csound-internal.h"

/**
 * mod_csound_is_GPL_compatible
 *
 * Defined to tell mod_csound is compatible with GNU General Public License
 * Of course it is. This function does nothing, but the fact it's declared
 * makes its GPL compatibility obvious. Having this declared is required.
 *
 * Return value: none
 */
void
mod_csound_is_GPL_compatible ()
{
/*
 * http://www.gnu.org/prep/standards/html_node/Dynamic-Plug_002dIn-Interfaces.html
 */
}

/**
 * mod_csound_is_dlclose_safe
 *
 * Defined to tell mod_csound has no dlclose issue, once can safely call
 * lt_dlclose on it when done with it, without risking any segfault.
 * Some other LW6 modules/shared libraries do have this problem.
 *
 * Return value: none
 */
void
mod_csound_is_dlclose_safe ()
{
}

static int
_play_fx (void *snd_context, int fx_id)
{
  int ret = 0;

  _mod_csound_context_t *csound_context =
    (_mod_csound_context_t *) snd_context;

  if (csound_context)
    {
      ret = _mod_csound_play_fx (csound_context, fx_id);
    }

  return ret;
}

static int
_is_music_file (void *snd_context, char *music_file)
{
  int ret = 0;

  _mod_csound_context_t *mod_csound_context =
    (_mod_csound_context_t *) snd_context;

  if (mod_csound_context)
    {
      ret = _mod_csound_is_music_file (mod_csound_context, music_file);
    }

  return ret;
}

static int
_play_music_file (void *snd_context, char *music_file)
{
  int ret = 0;

  _mod_csound_context_t *csound_context =
    (_mod_csound_context_t *) snd_context;

  if (csound_context)
    {
      ret = _mod_csound_play_music_file (csound_context, music_file);
    }

  return ret;
}

static int
_play_music_random (void *snd_context, char *music_path, char *music_filter,
		    char *music_exclude)
{
  int ret = 0;

  _mod_csound_context_t *mod_csound_context =
    (_mod_csound_context_t *) snd_context;

  if (mod_csound_context)
    {
      ret =
	_mod_csound_play_music_random (mod_csound_context, music_path,
				       music_filter, music_exclude);
    }

  return ret;
}

static void
_stop_music (void *snd_context)
{
  _mod_csound_context_t *mod_csound_context =
    (_mod_csound_context_t *) snd_context;

  if (mod_csound_context)
    {
      _mod_csound_stop_music (mod_csound_context);
    }
}

static void *
_init (int argc, const char *argv[], float fx_volume, float water_volume,
       float music_volume)
{
  _mod_csound_context_t *csound_context =
    _mod_csound_init (argc, argv, fx_volume, water_volume, music_volume);

  return (void *) csound_context;
}

static void
_set_fx_volume (void *snd_context, float volume)
{
  _mod_csound_context_t *csound_context =
    (_mod_csound_context_t *) snd_context;

  if (csound_context)
    {
      _mod_csound_set_fx_volume (csound_context, volume);
    }
}

static void
_set_water_volume (void *snd_context, float volume)
{
  _mod_csound_context_t *csound_context =
    (_mod_csound_context_t *) snd_context;

  if (csound_context)
    {
      _mod_csound_set_water_volume (csound_context, volume);
    }
}

static void
_set_music_volume (void *snd_context, float volume)
{
  _mod_csound_context_t *csound_context =
    (_mod_csound_context_t *) snd_context;

  if (csound_context)
    {
      _mod_csound_set_music_volume (csound_context, volume);
    }
}

static void
_poll (void *snd_context)
{
  _mod_csound_context_t *csound_context =
    (_mod_csound_context_t *) snd_context;

  if (csound_context)
    {
      _mod_csound_poll (csound_context);
    }
}

static void
_quit (void *snd_context)
{
  _mod_csound_context_t *csound_context =
    (_mod_csound_context_t *) snd_context;

  if (csound_context)
    {
      _mod_csound_quit (csound_context);
    }
}

static char *
_repr (void *snd_context, u_int32_t id)
{
  char *ret = NULL;
  _mod_csound_context_t *mod_csound_context =
    (_mod_csound_context_t *) snd_context;

  if (mod_csound_context)
    {
      ret = _mod_csound_repr (mod_csound_context, id);
    }

  return ret;
}

lw6sys_module_pedigree_t *
mod_csound_get_pedigree ()
{
  lw6sys_module_pedigree_t *module_pedigree = NULL;

  module_pedigree =
    (lw6sys_module_pedigree_t *)
    LW6SYS_CALLOC (sizeof (lw6sys_module_pedigree_t));
  if (module_pedigree)
    {
      module_pedigree->id = "csound";
      module_pedigree->category = "snd";
      module_pedigree->name = _("Csound");
      module_pedigree->readme =
	_x_
	("This sound backend uses Csound to produce on-the-fly music and sound effects. Not impemented yet.");
      module_pedigree->version = VERSION;
      module_pedigree->copyright =
	"Copyright (C)  2013  Christian Mauduit <ufoot@ufoot.org>";
      module_pedigree->license = "GPLv3+ (GNU GPL version 3 or later)";
      module_pedigree->date = __DATE__;
      module_pedigree->time = __TIME__;
    }

  return module_pedigree;
}

lw6snd_backend_t *
mod_csound_create_backend ()
{
  lw6snd_backend_t *backend;

  backend = LW6SYS_MALLOC (sizeof (lw6snd_backend_t));
  if (backend)
    {
      memset (backend, 0, sizeof (lw6snd_backend_t));

      backend->play_fx = _play_fx;
      backend->is_music_file = _is_music_file;
      backend->play_music_file = _play_music_file;
      backend->play_music_random = _play_music_random;
      backend->stop_music = _stop_music;
      backend->init = _init;
      backend->set_fx_volume = _set_fx_volume;
      backend->set_water_volume = _set_water_volume;
      backend->set_music_volume = _set_music_volume;
      backend->poll = _poll;
      backend->quit = _quit;
      backend->repr = _repr;
    }

  return backend;
}
