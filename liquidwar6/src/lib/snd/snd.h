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

#ifndef LIQUIDWAR6SND_H
#define LIQUIDWAR6SND_H

#include "../sys/sys.h"
#include "../dyn/dyn.h"
#include "../cfg/cfg.h"

#define LW6SND_NB_FX 9
typedef enum lw6snd_fx_enum_e
{
  LW6SND_FX_SPLASH = 0,
  LW6SND_FX_VALID = 1,
  LW6SND_FX_SELECT = 2,
  LW6SND_FX_START = 3,
  LW6SND_FX_DEATH = 4,
  LW6SND_FX_BELL = 5,
  LW6SND_FX_SCORE = 6,
  LW6SND_FX_HELLO = 7,
  LW6SND_FX_GOODBYE = 8
}
lw6snd_fx_enum_t;

#define LW6SND_NB_WATER 6

typedef struct lw6snd_backend_s
{
  lw6dyn_dl_handle_t *dl_handle;
  void *snd_context;
  int argc;
  char **argv;
  u_int32_t id;

  int (*play_fx) (void *snd_context, int fx_id);
  int (*is_music_file) (void *snd_context, char *music_file);
  int (*play_music_file) (void *snd_context, char *music_file);
  int (*play_music_random) (void *snd_context, char *music_path,
			    char *music_filter, char *music_exclude);
  void (*stop_music) (void *snd_context);
  void *(*init) (int argc, char *argv[], float fx_volume,
		 float music_volume);
  void (*set_fx_volume) (void *snd_context, float volume);
  void (*set_music_volume) (void *snd_context, float volume);
  void (*quit) (void *snd_context);
  char *(*repr) (void *snd_context, u_int32_t id);
}
lw6snd_backend_t;

/*
 * In api.c
 */
extern int lw6snd_play_fx (lw6snd_backend_t * backend, int fx_id);
extern int lw6snd_is_music_file (lw6snd_backend_t * backend,
				 char *map_dir,
				 char *music_path, char *music_file);
extern int lw6snd_play_music_file (lw6snd_backend_t * backend,
				   char *map_dir,
				   char *music_path, char *music_file);
extern int lw6snd_play_music_random (lw6snd_backend_t * backend,
				     char *music_path, char *music_filter,
				     char *music_exclude);
extern void lw6snd_stop_music (lw6snd_backend_t * backend);
extern char *lw6snd_repr (lw6snd_backend_t * snd_backend);
extern int lw6snd_init (lw6snd_backend_t * backend, float fx_volume,
			float music_volume);
extern void lw6snd_set_fx_volume (lw6snd_backend_t * backend,
				     float volume);
extern void lw6snd_set_music_volume (lw6snd_backend_t * backend,
				     float volume);
extern void lw6snd_quit (lw6snd_backend_t * backend);
extern char *lw6snd_repr (lw6snd_backend_t * backend);

/*
 * In register.c
 */
extern lw6sys_assoc_t *lw6snd_get_backends (int argc, char *argv[]);
extern lw6snd_backend_t *lw6snd_create_backend (int argc, char *argv[],
						char *name);
extern void lw6snd_destroy_backend (lw6snd_backend_t * backend);

/* snd-test.c */
extern int lw6snd_test (int mode);

#endif
