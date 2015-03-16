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

#include "../snd.h"
#include "mod-ogg.h"
#include "mod-ogg-internal.h"

/**
 * mod_ogg_is_GPL_compatible
 *
 * Defined to tell mod_ogg is compatible with GNU General Public License
 * Of course it is. This function does nothing, but the fact it's declared
 * makes its GPL compatibility obvious. Having this declared is required.
 *
 * Return value: none
 */
void
mod_ogg_is_GPL_compatible ()
{
/*
 * http://www.gnu.org/prep/standards/html_node/Dynamic-Plug_002dIn-Interfaces.html
 */
}

/**
 * mod_ogg_is_dlclose_safe
 *
 * Defined to tell mod_ogg has no dlclose issue, once can safely call
 * lt_dlclose on it when done with it, without risking any segfault.
 * Some other LW6 modules/shared libraries do have this problem.
 *
 * Return value: none
 */
void
mod_ogg_is_dlclose_safe ()
{
}

static int
_play_fx (lw6sys_context_t * sys_context, void *snd_context, int fx_id)
{
  int ret = 0;

  _mod_ogg_context_t *mod_ogg_context = (_mod_ogg_context_t *) snd_context;

  if (mod_ogg_context)
    {
      ret = _mod_ogg_play_fx (sys_context, mod_ogg_context, fx_id);
    }

  return ret;
}

static int
_is_music_file (lw6sys_context_t * sys_context, void *snd_context, char *music_file)
{
  int ret = 0;

  _mod_ogg_context_t *mod_ogg_context = (_mod_ogg_context_t *) snd_context;

  if (mod_ogg_context)
    {
      ret = _mod_ogg_is_music_file (sys_context, mod_ogg_context, music_file);
    }

  return ret;
}

static int
_play_music_file (lw6sys_context_t * sys_context, void *snd_context, char *music_file)
{
  int ret = 0;

  _mod_ogg_context_t *mod_ogg_context = (_mod_ogg_context_t *) snd_context;

  if (mod_ogg_context)
    {
      ret = _mod_ogg_play_music_file (sys_context, mod_ogg_context, music_file);
    }

  return ret;
}

static int
_play_music_random (lw6sys_context_t * sys_context, void *snd_context, char *music_path, char *music_filter, char *music_exclude)
{
  int ret = 0;

  _mod_ogg_context_t *mod_ogg_context = (_mod_ogg_context_t *) snd_context;

  if (mod_ogg_context)
    {
      ret = _mod_ogg_play_music_random (sys_context, mod_ogg_context, music_path, music_filter, music_exclude);
    }

  return ret;
}

static void
_stop_music (lw6sys_context_t * sys_context, void *snd_context)
{
  _mod_ogg_context_t *mod_ogg_context = (_mod_ogg_context_t *) snd_context;

  if (mod_ogg_context)
    {
      _mod_ogg_stop_music (sys_context, mod_ogg_context);
    }
}

static void *
_init (lw6sys_context_t * sys_context, int argc, const char *argv[], float fx_volume, float water_volume, float music_volume)
{
  _mod_ogg_context_t *mod_ogg_context = _mod_ogg_init (sys_context, argc, argv, fx_volume, water_volume, music_volume);

  return (void *) mod_ogg_context;
}

static void
_set_fx_volume (lw6sys_context_t * sys_context, void *snd_context, float volume)
{
  _mod_ogg_context_t *mod_ogg_context = (_mod_ogg_context_t *) snd_context;

  if (mod_ogg_context)
    {
      _mod_ogg_set_fx_volume (sys_context, mod_ogg_context, volume);
    }
}

static void
_set_water_volume (lw6sys_context_t * sys_context, void *snd_context, float volume)
{
  _mod_ogg_context_t *mod_ogg_context = (_mod_ogg_context_t *) snd_context;

  if (mod_ogg_context)
    {
      _mod_ogg_set_water_volume (sys_context, mod_ogg_context, volume);
    }
}

static void
_set_music_volume (lw6sys_context_t * sys_context, void *snd_context, float volume)
{
  _mod_ogg_context_t *mod_ogg_context = (_mod_ogg_context_t *) snd_context;

  if (mod_ogg_context)
    {
      _mod_ogg_set_music_volume (sys_context, mod_ogg_context, volume);
    }
}

static void
_poll (lw6sys_context_t * sys_context, void *snd_context)
{
  _mod_ogg_context_t *mod_ogg_context = (_mod_ogg_context_t *) snd_context;

  if (mod_ogg_context)
    {
      _mod_ogg_poll (sys_context, mod_ogg_context);
    }
}

static void
_quit (lw6sys_context_t * sys_context, void *snd_context)
{
  _mod_ogg_context_t *mod_ogg_context = (_mod_ogg_context_t *) snd_context;

  if (mod_ogg_context)
    {
      _mod_ogg_quit (sys_context, mod_ogg_context);
    }
}

static char *
_repr (lw6sys_context_t * sys_context, void *snd_context, u_int32_t id)
{
  char *ret = NULL;
  _mod_ogg_context_t *mod_ogg_context = (_mod_ogg_context_t *) snd_context;

  if (mod_ogg_context)
    {
      ret = _mod_ogg_repr (sys_context, mod_ogg_context, id);
    }

  return ret;
}

/**
 * mod_ogg_get_pedigree
 *
 * @sys_context: global system context
 *
 * Returns the pedigree for mod-ogg, giving details about the module,
 * including name, description, licence, date/time of compilation.
 *
 * Return value: dynamically allocated object.
 */
lw6sys_module_pedigree_t *
mod_ogg_get_pedigree (lw6sys_context_t * sys_context)
{
  lw6sys_module_pedigree_t *module_pedigree = NULL;

  module_pedigree = (lw6sys_module_pedigree_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6sys_module_pedigree_t));
  if (module_pedigree)
    {
      module_pedigree->id = "ogg";
      module_pedigree->category = "snd";
      module_pedigree->name = _("Ogg Vorbis");
      module_pedigree->readme =
	_x_ ("This sound backend uses Ogg Vorbis (through SDL) to handle sound effects and music. This is the only working sound backend for now.");
      module_pedigree->version = VERSION;
      module_pedigree->copyright = "Copyright (C)  2015  Christian Mauduit <ufoot@ufoot.org>";
      module_pedigree->license = "GPLv3+ (GNU GPL version 3 or later)";
      module_pedigree->date = __DATE__;
      module_pedigree->time = __TIME__;
    }

  return module_pedigree;
}

/**
 * mod_ogg_create_backend
 *
 * @sys_context: global system context
 *
 * Creates a mod-ogg backend.
 *
 * Return value: backend pointer.
 */
lw6snd_backend_t *
mod_ogg_create_backend (lw6sys_context_t * sys_context)
{
  lw6snd_backend_t *backend;

  backend = LW6SYS_MALLOC (sys_context, sizeof (lw6snd_backend_t));
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
