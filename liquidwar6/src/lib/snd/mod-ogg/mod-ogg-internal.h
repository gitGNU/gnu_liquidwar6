/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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

#ifndef LIQUIDWAR6SND_OGG_INTERNAL_H
#define LIQUIDWAR6SND_OGG_INTERNAL_H

#include "../snd.h"

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

typedef struct _mod_ogg_path_s
{
  char *data_dir;
}
_mod_ogg_path_t;

typedef struct _mod_ogg_volume_s
{
  float sound;
  float music;
}
_mod_ogg_volume_t;

typedef struct _mod_ogg_const_data_s
{
  int frequency;
  int bits;
  int channels;
  int chunksize;
  char *file_splash;
  char *file_valid;
  char *file_select;
  char *file_start;
  char *file_death;
  char *file_bell;
  char *file_score;
  char *file_hello;
  char *file_goodbye;
}
_mod_ogg_const_data_t;

typedef struct _mod_ogg_sounds_s
{
  Mix_Chunk *sounds[LW6SND_NB_SOUNDS];
}
_mod_ogg_sounds_t;

typedef struct _mod_ogg_music_s
{
  Mix_Music *music;
  char *filter;
  char *exclude;
}
_mod_ogg_music_t;

typedef struct _mod_ogg_context_s
{
  _mod_ogg_path_t path;
  _mod_ogg_volume_t volume;
  _mod_ogg_const_data_t const_data;
  _mod_ogg_sounds_t sounds;
  _mod_ogg_music_t music;
}
_mod_ogg_context_t;

/*
 * In const.c
 */
extern int _mod_ogg_load_consts (_mod_ogg_context_t * ogg_context);
extern void _mod_ogg_unload_consts (_mod_ogg_context_t * ogg_context);

/*
 * In sound.c
 */
extern int _mod_ogg_play_sound (_mod_ogg_context_t * ogg_context,
				int sound_id);
extern int _mod_ogg_load_sounds (_mod_ogg_context_t * ogg_context);
extern void _mod_ogg_unload_sounds (_mod_ogg_context_t * ogg_context);

/*
 * In music.c
 */
extern int _mod_ogg_is_music_file (_mod_ogg_context_t *
				   ogg_context, char *music_file);
extern int _mod_ogg_play_music_file (_mod_ogg_context_t * ogg_context,
				     char *music_file);
extern int _mod_ogg_play_music_random (_mod_ogg_context_t * ogg_context,
				       char *music_path, char *music_filter,
				       char *music_exclude);
extern void _mod_ogg_stop_music (_mod_ogg_context_t * ogg_context);

/*
 * In path.c
 */
extern int _mod_ogg_path_init (_mod_ogg_context_t * ogg_context,
			       int argc, char *argv[]);
extern void _mod_ogg_path_quit (_mod_ogg_context_t * ogg_context);

/*
 * In repr.c
 */
extern char *_mod_ogg_repr (_mod_ogg_context_t * gfx_context, u_int32_t id);

/*
 * In setup.c
 */
extern _mod_ogg_context_t *_mod_ogg_init (int argc, char *argv[],
					  float sound_volume,
					  float music_volume);
extern void _mod_ogg_quit (_mod_ogg_context_t * ogg_context);

/*
 * In volume.c
 */
extern void _mod_ogg_set_sound_volume (_mod_ogg_context_t * ogg_context,
				       float volume);
extern void _mod_ogg_set_music_volume (_mod_ogg_context_t * ogg_context,
				       float volume);

#endif
