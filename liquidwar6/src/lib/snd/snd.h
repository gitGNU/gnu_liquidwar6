
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

#ifndef LIQUIDWAR6SND_H
#define LIQUIDWAR6SND_H

#include "../sys/sys.h"
#include "../dyn/dyn.h"
#include "../cfg/cfg.h"

#define LW6SND_NB_FX 10
typedef enum lw6snd_fx_enum_e
{
  LW6SND_FX_SPLASH = 0,
  LW6SND_FX_VALID = 1,
  LW6SND_FX_SELECT = 2,
  LW6SND_FX_NO = 3,
  LW6SND_FX_START = 4,
  LW6SND_FX_DEATH = 5,
  LW6SND_FX_BELL = 6,
  LW6SND_FX_SCORE = 7,
  LW6SND_FX_HELLO = 8,
  LW6SND_FX_GOODBYE = 9
}
lw6snd_fx_enum_t;

/**
 * The snd backend is the first argument passed to any snd function,
 * it contains reference to all the functions which can be used
 * as well as a pointer on associated data. In OO, this would just
 * be an object, with members and methods, using polymorphism through
 * opaque pointers.
 */
typedef struct lw6snd_backend_s
{
  /// Handle on dynamic library (if it makes sense).
  lw6dyn_dl_handle_t *dl_handle;
  /**
   * Snd specific data, what is behind this pointer really
   * depends on the snd engine.
   */
  void *snd_context;
  /// The argc value passed to main.
  int argc;
  /// The argv value passed to main.
  const char **argv;
  /**
   * The id of the object, this is non-zero and unique within one run session,
   * incremented at each object creation.
   */
  u_int32_t id;

  /// Pointer on lw6snd_play_fx callback code.
  int (*play_fx) (lw6sys_context_t * sys_context, void *snd_context, int fx_id);
  /// Pointer on lw6snd_is_music_file callback code.
  int (*is_music_file) (lw6sys_context_t * sys_context, void *snd_context, char *music_file);
  /// Pointer on lw6snd_play_music_file callback code.
  int (*play_music_file) (lw6sys_context_t * sys_context, void *snd_context, char *music_file);
  /// Pointer on lw6snd_play_music_randomcallback code.
  int (*play_music_random) (lw6sys_context_t * sys_context, void *snd_context, char *music_path, char *music_filter, char *music_exclude);
  /// Pointer on lw6snd_stop_music callback code.
  void (*stop_music) (lw6sys_context_t * sys_context, void *snd_context);
  /// Pointer on lw6snd_init callback code.
  void *(*init) (lw6sys_context_t * sys_context, int argc, const char *argv[], float fx_volume, float water_volume, float music_volume);
  /// Pointer on lw6snd_set_fx_volume callback code.
  void (*set_fx_volume) (lw6sys_context_t * sys_context, void *snd_context, float volume);
  /// Pointer on lw6snd_set_water_volume callback code.
  void (*set_water_volume) (lw6sys_context_t * sys_context, void *snd_context, float volume);
  /// Pointer on lw6snd_set_music_volume callback code.
  void (*set_music_volume) (lw6sys_context_t * sys_context, void *snd_context, float volume);
  /// Pointer on lw6snd_poll callback code.
  void (*poll) (lw6sys_context_t * sys_context, void *snd_context);
  /// Pointer on lw6snd_quit callback code.
  void (*quit) (lw6sys_context_t * sys_context, void *snd_context);
  /// Pointer on lw6snd_repr callback code.
  char *(*repr) (lw6sys_context_t * sys_context, void *snd_context, u_int32_t id);
}
lw6snd_backend_t;

/* snd-api.c */
extern int lw6snd_play_fx (lw6sys_context_t * sys_context, lw6snd_backend_t * backend, int fx_id);
extern int lw6snd_is_music_file (lw6sys_context_t * sys_context, lw6snd_backend_t * backend, char *map_dir, char *music_path, char *music_file);
extern int lw6snd_play_music_file (lw6sys_context_t * sys_context, lw6snd_backend_t * backend, char *map_dir, char *music_path, char *music_file);
extern int lw6snd_play_music_random (lw6sys_context_t * sys_context, lw6snd_backend_t * backend, char *music_path, char *music_filter, char *music_exclude);
extern void lw6snd_stop_music (lw6sys_context_t * sys_context, lw6snd_backend_t * backend);
extern char *lw6snd_repr (lw6sys_context_t * sys_context, const lw6snd_backend_t * snd_backend);
extern int lw6snd_init (lw6sys_context_t * sys_context, lw6snd_backend_t * backend, float fx_volume, float water_volume, float music_volume);
extern void lw6snd_set_fx_volume (lw6sys_context_t * sys_context, lw6snd_backend_t * backend, float volume);
extern void lw6snd_set_water_volume (lw6sys_context_t * sys_context, lw6snd_backend_t * backend, float volume);
extern void lw6snd_set_music_volume (lw6sys_context_t * sys_context, lw6snd_backend_t * backend, float volume);
extern void lw6snd_poll (lw6sys_context_t * sys_context, lw6snd_backend_t * backend);
extern void lw6snd_quit (lw6sys_context_t * sys_context, lw6snd_backend_t * backend);
extern char *lw6snd_repr (lw6sys_context_t * sys_context, const lw6snd_backend_t * backend);

/* snd-register.c */
extern lw6sys_assoc_t *lw6snd_get_backends (lw6sys_context_t * sys_context, int argc, const char *argv[]);
extern lw6snd_backend_t *lw6snd_create_backend (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *name);
extern void lw6snd_destroy_backend (lw6sys_context_t * sys_context, lw6snd_backend_t * backend);

/* snd-test.c */
extern int lw6snd_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6snd_test_run (lw6sys_context_t * sys_context, int mode);

#endif
