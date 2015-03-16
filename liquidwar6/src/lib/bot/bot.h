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

#ifndef LIQUIDWAR6BOT_H
#define LIQUIDWAR6BOT_H

#include "../sys/sys.h"
#include "../map/map.h"
#include "../ker/ker.h"
#include "../pil/pil.h"
#include "../dyn/dyn.h"

/**
 * Parameters usable by a bot engine. Those are the
 * stable, fixed parameters passed at bot creation, they
 * don't change during the bot life.
 */
typedef struct lw6bot_param_s
{
  /**
   * Speed of the bot, this is a value between 0.0f and 1.0f,
   * 1 means normal speed, 0 is as slow as possible. Values
   * over 1 will make the bot act/move faster than usual.
   */
  float speed;
  /**
   * IQ is supposed to reflect the cleverness of the bot.
   * The default is 100 (this value is basically a percentage),
   * 0 means just so stupid, and a high value, for instance
   * 200, means very clever.
   */
  int iq;
  /// The cursor ID, which is a 16-bit non-null integer.
  u_int16_t cursor_id;
} lw6bot_param_t;

/**
 * Data used by a bot, those are essentially stable data
 * passed as an argument plus changing data, that is,
 * the game state.
 */
typedef struct lw6bot_data_s
{
  /**
   * Game state the bot will have to base its reflexion upon.
   * This not need be always the same game state, the pointer
   * might change, but it should always refer to the same
   * logical game, that is, at least, same struct.
   */
  const lw6ker_game_state_t *game_state;
  /// Constant parameters passed to the bot at creation.
  lw6bot_param_t param;
} lw6bot_data_t;

/**
 * Parameters passed at bot creation, the only use for this
 * is to simplify the protoype of the init function.
 */
typedef struct lw6bot_seed_s
{
  /**
   * Game state, that is, the level used, the fighters on it,
   * the other cursors positions, and so on.
   */
  const lw6ker_game_state_t *game_state;
  /**
   * This can be NULL, it's a pilot object which can be used
   * in some case, when, for instance, in dirty read mode,
   * we want to read the level on the fly without syncing.
   */
  lw6pil_pilot_t *pilot;
  /// The dirty read mode (between 0 and 2).
  int dirty_read;
  /// Parameters given to the bot at creation.
  lw6bot_param_t param;
} lw6bot_seed_t;

/**
 * The bot backend is the first argument passed to any bot function,
 * it contains reference to all the functions which can be used
 * as well as a pointer on associated data. In OO, this would just
 * be an object, with members and methods, using polymorphism through
 * opaque pointers.
 */
typedef struct lw6bot_backend_s
{
  /// Handle on dynamic library (if it makes sense).
  lw6dyn_dl_handle_t *dl_handle;
  /**
   * Bot specific data, what is behind this pointer really
   * depends on the bot engine.
   */
  void *bot_context;
  /// The argc value passed to main.
  int argc;
  /// The argv value passed to main.
  const char **argv;
  /**
   * The id of the object, this is non-zero and unique within one run session,
   * incremented at each object creation.
   */
  u_int32_t id;
  /// Parameters passed at initialization.
  lw6bot_seed_t seed;

  /// Pointer on lw6bot_init callback code.
  void *(*init) (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6bot_data_t * data);
  /// Pointer on lw6bot_context callback code.
  void (*quit) (lw6sys_context_t * sys_context, void *bot_context);
  /// Pointer on lw6bot_next_move callback code.
  int (*next_move) (lw6sys_context_t * sys_context, void *bot_context, int *x, int *y, lw6bot_data_t * data);
  /// Pointer on lw6bot_repr callback code.
  char *(*repr) (lw6sys_context_t * sys_context, void *bot_context, u_int32_t id);
}
lw6bot_backend_t;

/* bot-api.c */
extern int lw6bot_init (lw6sys_context_t * sys_context, lw6bot_backend_t * backend, lw6bot_seed_t * seed);
extern void lw6bot_quit (lw6sys_context_t * sys_context, lw6bot_backend_t * backend);
extern int lw6bot_next_move (lw6sys_context_t * sys_context, lw6bot_backend_t * backend, int *x, int *y);
extern char *lw6bot_repr (lw6sys_context_t * sys_context, const lw6bot_backend_t * backend);

/* bot-register.c */
extern lw6sys_assoc_t *lw6bot_get_backends (lw6sys_context_t * sys_context, int argc, const char *argv[]);
extern lw6bot_backend_t *lw6bot_create_backend (lw6sys_context_t * sys_context, int argc, const char *argv[], const char *name);
extern void lw6bot_destroy_backend (lw6sys_context_t * sys_context, lw6bot_backend_t * backend);

/* bot-test.c */
extern int lw6bot_test_register (lw6sys_context_t * sys_context, int mode);
extern int lw6bot_test_run (lw6sys_context_t * sys_context, int mode);

#endif // LIQUIDWAR6BOT_H
