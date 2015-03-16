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

#ifndef LIQUIDWAR6SND_CSOUND_INTERNAL_H
#define LIQUIDWAR6SND_CSOUND_INTERNAL_H

#include "../snd.h"

typedef struct _mod_csound_path_s
{
  char *data_dir;
}
_mod_csound_path_t;

typedef struct _mod_csound_volume_s
{
  float fx;
  float water;
  float music;
}
_mod_csound_volume_t;

typedef struct _mod_csound_context_s
{
  _mod_csound_path_t path;
  _mod_csound_volume_t volume;
}
_mod_csound_context_t;

/*
 * In fx.c
 */
extern int _mod_csound_play_fx (lw6sys_context_t * sys_context, _mod_csound_context_t * csound_context, int fx_id);

/*
 * In music.c
 */
extern int _mod_csound_is_music_file (lw6sys_context_t * sys_context, _mod_csound_context_t * csound_context, char *music_file);
extern int _mod_csound_play_music_file (lw6sys_context_t * sys_context, _mod_csound_context_t * csound_context, char *music_file);
extern int _mod_csound_play_music_random (lw6sys_context_t * sys_context, _mod_csound_context_t * csound_context, char *music_path, char *music_filter,
					  char *music_exclude);
extern void _mod_csound_stop_music (lw6sys_context_t * sys_context, _mod_csound_context_t * csound_context);

/*
 * In path.c
 */
extern int _mod_csound_path_init (lw6sys_context_t * sys_context, _mod_csound_context_t * csound_context, int argc, const char *argv[]);
extern void _mod_csound_path_quit (lw6sys_context_t * sys_context, _mod_csound_context_t * csound_context);

/*
 * In repr.c
 */
extern char *_mod_csound_repr (lw6sys_context_t * sys_context, _mod_csound_context_t * csound_context, u_int32_t id);

/*
 * In setup.c
 */
extern _mod_csound_context_t *_mod_csound_init (lw6sys_context_t * sys_context, int argc, const char *argv[], float fx_volume, float water_volume,
						float music_volume);
extern void _mod_csound_poll (lw6sys_context_t * sys_context, _mod_csound_context_t * csound_context);
extern void _mod_csound_quit (lw6sys_context_t * sys_context, _mod_csound_context_t * csound_context);

/*
 * In volume.c
 */
extern void _mod_csound_set_fx_volume (lw6sys_context_t * sys_context, _mod_csound_context_t * csound_context, float volume);
extern void _mod_csound_set_water_volume (lw6sys_context_t * sys_context, _mod_csound_context_t * csound_context, float volume);
extern void _mod_csound_set_music_volume (lw6sys_context_t * sys_context, _mod_csound_context_t * csound_context, float volume);

/*
 * In water.c
 */
extern int _mod_csound_poll_water (lw6sys_context_t * sys_context, _mod_csound_context_t * csound_context);


#endif
