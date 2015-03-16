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

#ifndef LIQUIDWAR6SND_OGG_INTERNAL_H
#define LIQUIDWAR6SND_OGG_INTERNAL_H

#include "../snd.h"

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#define _MOD_OGG_CHANNEL_WATER1 0
#define _MOD_OGG_CHANNEL_WATER2 1
#define _MOD_OGG_CHANNEL_FX0 2
#define _MOD_OGG_CHANNEL_GROUP_FX 1
#define _MOD_OGG_CHANNEL_GROUP_WATER 2

#define _MOD_OGG_NB_WATER 6
typedef enum _mod_ogg_water_enum_e
{
  _MOD_OGG_WATER1 = 0,
  _MOD_OGG_WATER2 = 1,
  _MOD_OGG_WATER3 = 2,
  _MOD_OGG_WATER4 = 3,
  _MOD_OGG_WATER5 = 4,
  _MOD_OGG_WATER6 = 5
}
_mod_ogg_water_enum_t;

typedef struct _mod_ogg_path_s
{
  char *data_dir;
}
_mod_ogg_path_t;

typedef struct _mod_ogg_mixer_s
{
  int nb_channels;
  int last_channel;
}
_mod_ogg_mixer_t;

typedef struct _mod_ogg_volume_s
{
  float fx;
  float water;
  float music;
}
_mod_ogg_volume_t;

typedef struct _mod_ogg_const_data_s
{
  int frequency;
  int bits;
  int channels;
  int chunksize;
  float water_pan1;
  float water_pan2;
  char *file_splash;
  char *file_valid;
  char *file_select;
  char *file_no;
  char *file_start;
  char *file_death;
  char *file_bell;
  char *file_score;
  char *file_hello;
  char *file_goodbye;
  char *file_water1;
  char *file_water2;
  char *file_water3;
  char *file_water4;
  char *file_water5;
  char *file_water6;
}
_mod_ogg_const_data_t;

typedef struct _mod_ogg_fx_s
{
  Mix_Chunk *fx[LW6SND_NB_FX];
}
_mod_ogg_fx_t;

typedef struct _mod_ogg_water_s
{
  Mix_Chunk *water[_MOD_OGG_NB_WATER];
}
_mod_ogg_water_t;

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
  _mod_ogg_mixer_t mixer;
  _mod_ogg_volume_t volume;
  _mod_ogg_const_data_t const_data;
  _mod_ogg_fx_t fx;
  _mod_ogg_water_t water;
  _mod_ogg_music_t music;
}
_mod_ogg_context_t;

/*
 * In const.c
 */
extern int _mod_ogg_load_consts (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context);
extern void _mod_ogg_unload_consts (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context);

/*
 * In fx.c
 */
extern int _mod_ogg_play_fx (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context, int fx_id);
extern int _mod_ogg_load_fx (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context);
extern void _mod_ogg_unload_fx (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context);

/*
 * In music.c
 */
extern int _mod_ogg_is_music_file (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context, char *music_file);
extern int _mod_ogg_play_music_file (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context, char *music_file);
extern int _mod_ogg_play_music_random (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context, char *music_path, char *music_filter,
				       char *music_exclude);
extern void _mod_ogg_stop_music (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context);

/*
 * In path.c
 */
extern int _mod_ogg_path_init (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context, int argc, const char *argv[]);
extern void _mod_ogg_path_quit (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context);

/*
 * In repr.c
 */
extern char *_mod_ogg_repr (lw6sys_context_t * sys_context, _mod_ogg_context_t * gfx_context, u_int32_t id);

/*
 * In setup.c
 */
extern _mod_ogg_context_t *_mod_ogg_init (lw6sys_context_t * sys_context, int argc, const char *argv[], float fx_volume, float water_volume,
					  float music_volume);
extern void _mod_ogg_poll (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context);
extern void _mod_ogg_quit (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context);

/*
 * In volume.c
 */
extern void _mod_ogg_set_fx_volume (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context, float volume);
extern void _mod_ogg_set_water_volume (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context, float volume);
extern void _mod_ogg_set_music_volume (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context, float volume);

/*
 * In water.c
 */
extern void _mod_ogg_poll_water (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context);
extern int _mod_ogg_load_water (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context);
extern void _mod_ogg_unload_water (lw6sys_context_t * sys_context, _mod_ogg_context_t * ogg_context);

#endif
