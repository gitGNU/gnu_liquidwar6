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

#include <string.h>

#include "tsk.h"
#include "tsk-internal.h"

#define _LOADER_LOCK(SYS_CONTEXT) _loader_lock ((SYS_CONTEXT),__FILE__,__LINE__,__FUNCTION__,loader_data)
#define _LOADER_UNLOCK(SYS_CONTEXT) _loader_unlock ((SYS_CONTEXT),__FILE__,__LINE__,__FUNCTION__,loader_data)

static void
_loader_lock (lw6sys_context_t * sys_context, char *file, int line, const char *func, _lw6tsk_loader_data_t * loader_data)
{
  /*
   * Could put some log command here to debug
   */
  LW6SYS_MUTEX_LOCK (sys_context, loader_data->mutex);
  /*
   * Could put some log command here to debug
   */
}

static void
_loader_unlock (lw6sys_context_t * sys_context, char *file, int line, const char *func, _lw6tsk_loader_data_t * loader_data)
{
  /*
   * Could put some log command here to debug
   */
  LW6SYS_MUTEX_UNLOCK (sys_context, loader_data->mutex);
}

static void
_stage1_clear_request (lw6sys_context_t * sys_context, _lw6tsk_loader_stage1_t * stage1)
{
  if (stage1->map_path)
    {
      LW6SYS_FREE (sys_context, stage1->map_path);
      stage1->map_path = NULL;
    }
  if (stage1->relative_path)
    {
      LW6SYS_FREE (sys_context, stage1->relative_path);
      stage1->relative_path = NULL;
    }
  if (stage1->default_param)
    {
      lw6sys_assoc_free (sys_context, stage1->default_param);
      stage1->default_param = NULL;
    }
  if (stage1->forced_param)
    {
      lw6sys_assoc_free (sys_context, stage1->forced_param);
      stage1->forced_param = NULL;
    }
  if (stage1->seed)
    {
      LW6SYS_FREE (sys_context, stage1->seed);
      stage1->seed = NULL;
    }
  stage1->display_w = 0;
  stage1->display_h = 0;
}

static void
_stage1_clear_response (lw6sys_context_t * sys_context, _lw6tsk_loader_stage1_t * stage1)
{
  if (stage1->level)
    {
      lw6map_free (sys_context, stage1->level);
      stage1->level = NULL;
    }
}

static void
_stage1_clear (lw6sys_context_t * sys_context, _lw6tsk_loader_stage1_t * stage1)
{
  _stage1_clear_request (sys_context, stage1);
  _stage1_clear_response (sys_context, stage1);
  memset (stage1, 0, sizeof (_lw6tsk_loader_stage1_t));
}

static void
_stage2_clear_request (lw6sys_context_t * sys_context, _lw6tsk_loader_stage2_t * stage2)
{
  if (stage2->src)
    {
      lw6map_free (sys_context, stage2->src);
      stage2->src = NULL;
    }
}

static void
_stage2_clear_response (lw6sys_context_t * sys_context, _lw6tsk_loader_stage2_t * stage2)
{
  if (stage2->game_state)
    {
      lw6ker_game_state_free (sys_context, stage2->game_state);
      stage2->game_state = NULL;
    }
  if (stage2->game_struct)
    {
      lw6ker_game_struct_free (sys_context, stage2->game_struct);
      stage2->game_struct = NULL;
    }
  if (stage2->level)
    {
      lw6map_free (sys_context, stage2->level);
      stage2->level = NULL;
    }
}

static void
_stage2_clear (lw6sys_context_t * sys_context, _lw6tsk_loader_stage2_t * stage2)
{
  _stage2_clear_request (sys_context, stage2);
  _stage2_clear_response (sys_context, stage2);
  memset (stage2, 0, sizeof (_lw6tsk_loader_stage2_t));
}

static void
_clear (lw6sys_context_t * sys_context, _lw6tsk_loader_data_t * loader_data)
{
  _stage2_clear (sys_context, &(loader_data->stage2));
  _stage1_clear (sys_context, &(loader_data->stage1));
}

static void
_stage1 (lw6sys_context_t * sys_context, _lw6tsk_loader_data_t * loader_data)
{
  int request_number = 0;
  char *map_path = NULL;
  char *relative_path = NULL;
  lw6sys_assoc_t *default_param = NULL;
  lw6sys_assoc_t *forced_param = NULL;
  char *seed = NULL;
  lw6map_level_t *level = NULL;
  int display_w = 0;
  int display_h = 0;
  int map_w = 0;
  int map_h = 0;
  int bench_value = 0;
  int magic_number = 0;
  lw6map_level_t *src2 = NULL;
  lw6sys_progress_t progress;
  float dummy = 0.0f;
  int ok = 0;

  progress.value = loader_data->progress;
  if (!(progress.value))
    {
      progress.value = &dummy;
    }

  _LOADER_LOCK (sys_context);
  request_number = loader_data->request_number;

  /*
   * First case, load a map from disk.
   */
  if (loader_data->stage1.map_path && loader_data->stage1.relative_path)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
		  _x_ ("async load stage1 using ldr, request_number %d, level \"%s\""), request_number, loader_data->stage1.relative_path);

      (*(progress.value)) = _LW6TSK_LOADER_PROGRESS_STAGE1_BEGIN;
      map_path = lw6sys_str_copy (sys_context, loader_data->stage1.map_path);
      relative_path = lw6sys_str_copy (sys_context, loader_data->stage1.relative_path);
      default_param = lw6sys_assoc_dup (sys_context, loader_data->stage1.default_param, (lw6sys_dup_func_t) lw6sys_str_copy);
      forced_param = lw6sys_assoc_dup (sys_context, loader_data->stage1.forced_param, (lw6sys_dup_func_t) lw6sys_str_copy);
      display_w = loader_data->stage1.display_w;
      display_h = loader_data->stage1.display_h;
      bench_value = loader_data->stage1.bench_value;
      magic_number = loader_data->stage1.magic_number;

      _stage1_clear_request (sys_context, &(loader_data->stage1));
    }

  /*
   * Second case, generate a map
   */
  if (loader_data->stage1.seed)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("async load stage1 using gen, request_number %d, seed \"%s\""), request_number, loader_data->stage1.seed);

      (*(progress.value)) = _LW6TSK_LOADER_PROGRESS_STAGE1_BEGIN;
      seed = lw6gen_seed_normalize (sys_context, loader_data->stage1.seed);
      display_w = loader_data->stage1.display_w;
      display_h = loader_data->stage1.display_h;
      bench_value = loader_data->stage1.bench_value;
      magic_number = loader_data->stage1.magic_number;

      _stage1_clear_request (sys_context, &(loader_data->stage1));
    }
  _LOADER_UNLOCK (sys_context);

  if (map_path && relative_path)
    {
      progress.min = _LW6TSK_LOADER_PROGRESS_STAGE1_BEGIN_MAP;
      progress.max = _LW6TSK_LOADER_PROGRESS_STAGE1_END_MAP;
      lw6sys_progress_begin (sys_context, &progress);
      level =
	lw6ldr_read_relative (sys_context, map_path, relative_path, default_param,
			      forced_param, display_w, display_h, bench_value, magic_number, loader_data->user_dir, &progress);
      if (level)
	{
	  lw6sys_progress_end (sys_context, &progress);
	}
    }

  if (seed)
    {
      progress.min = _LW6TSK_LOADER_PROGRESS_STAGE1_BEGIN_MAP;
      progress.max = _LW6TSK_LOADER_PROGRESS_STAGE1_END_MAP;
      lw6sys_progress_begin (sys_context, &progress);
      lw6ldr_resampler_use_for_gen (sys_context, &map_w, &map_h, display_w, display_h, bench_value, magic_number);
      level = lw6gen_create_from_seed (sys_context, seed, map_w, map_h);
      if (level)
	{
	  lw6sys_progress_end (sys_context, &progress);
	}
    }

  if (map_path)
    {
      LW6SYS_FREE (sys_context, map_path);
      map_path = NULL;
    }
  if (relative_path)
    {
      LW6SYS_FREE (sys_context, relative_path);
      relative_path = NULL;
    }
  if (default_param)
    {
      lw6sys_assoc_free (sys_context, default_param);
      default_param = NULL;
    }
  if (forced_param)
    {
      lw6sys_assoc_free (sys_context, forced_param);
      forced_param = NULL;
    }
  if (seed)
    {
      LW6SYS_FREE (sys_context, seed);
      seed = NULL;
    }

  if (level)
    {
      char *repr = NULL;

      repr = lw6map_repr (sys_context, level);
      if (repr)
	{
	  progress.min = _LW6TSK_LOADER_PROGRESS_STAGE1_BEGIN_DUP;
	  progress.max = _LW6TSK_LOADER_PROGRESS_STAGE1_END_DUP;
	  lw6sys_progress_begin (sys_context, &progress);
	  src2 = lw6map_dup (sys_context, level, &progress);
	  _LOADER_LOCK (sys_context);
	  if (request_number == loader_data->request_number)
	    {
	      lw6sys_progress_end (sys_context, &progress);
	      _stage1_clear_response (sys_context, &(loader_data->stage1));
	      loader_data->stage1.level = level;
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("async load stage1 done, request_number %d, level \"%s\""), request_number, repr);
	      _stage2_clear_request (sys_context, &(loader_data->stage2));
	      loader_data->stage2.src = src2;
	      ok = 1;
	    }
	  else if (request_number < loader_data->request_number)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			  _x_ ("async load stage1 abort, request number %d < %d, level \"%s\""), request_number, loader_data->request_number, repr);
	      if (src2)
		{
		  lw6map_free (sys_context, src2);
		  src2 = NULL;
		}
	      lw6map_free (sys_context, level);
	      level = NULL;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			  _x_ ("async load stage1, inconsistent request number %d > %d, level \"%s\""), request_number, loader_data->request_number, repr);
	    }
	  _LOADER_UNLOCK (sys_context);
	  LW6SYS_FREE (sys_context, repr);
	  repr = NULL;
	}
    }
  if (ok)
    {
      (*(progress.value)) = _LW6TSK_LOADER_PROGRESS_STAGE1_END;
    }
}

static void
_stage2 (lw6sys_context_t * sys_context, _lw6tsk_loader_data_t * loader_data)
{
  int request_number = 0;
  lw6map_level_t *level = NULL;
  lw6ker_game_struct_t *game_struct = NULL;
  lw6ker_game_state_t *game_state = NULL;
  lw6sys_progress_t progress;
  float dummy = 0.0f;
  int ok = 0;

  progress.value = loader_data->progress;
  if (!(progress.value))
    {
      progress.value = &dummy;
    }

  _LOADER_LOCK (sys_context);
  request_number = loader_data->request_number;

  if (loader_data->stage2.src)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("async load stage2, request_number %d"), request_number);
      (*(progress.value)) = _LW6TSK_LOADER_PROGRESS_STAGE2_BEGIN;

      progress.min = _LW6TSK_LOADER_PROGRESS_STAGE2_BEGIN_MAP;
      progress.max = _LW6TSK_LOADER_PROGRESS_STAGE2_END_MAP;
      lw6sys_progress_begin (sys_context, &progress);
      level = lw6map_dup (sys_context, loader_data->stage2.src, &progress);
      lw6sys_progress_end (sys_context, &progress);

      _stage2_clear_request (sys_context, &(loader_data->stage2));
    }
  _LOADER_UNLOCK (sys_context);

  if (level)
    {
      progress.min = _LW6TSK_LOADER_PROGRESS_STAGE2_BEGIN_STRUCT;
      progress.max = _LW6TSK_LOADER_PROGRESS_STAGE2_END_STRUCT;
      lw6sys_progress_begin (sys_context, &progress);
      game_struct = lw6ker_game_struct_new (sys_context, level, &progress);
      if (game_struct)
	{
	  lw6sys_progress_end (sys_context, &progress);
	}
      else
	{
	  lw6map_free (sys_context, level);
	  level = NULL;
	}
    }
  if (level && game_struct)
    {
      progress.min = _LW6TSK_LOADER_PROGRESS_STAGE2_BEGIN_STATE;
      progress.max = _LW6TSK_LOADER_PROGRESS_STAGE2_END_STATE;
      lw6sys_progress_begin (sys_context, &progress);
      game_state = lw6ker_game_state_new (sys_context, game_struct, &progress);
      if (game_state)
	{
	  lw6sys_progress_end (sys_context, &progress);
	}
      else
	{
	  lw6ker_game_struct_free (sys_context, game_struct);
	  game_struct = NULL;
	  lw6map_free (sys_context, level);
	  level = NULL;
	}
    }

  if (level && game_struct && game_state)
    {
      char *repr = NULL;

      repr = lw6ker_game_state_repr (sys_context, game_state);
      if (repr)
	{
	  _LOADER_LOCK (sys_context);
	  if (request_number == loader_data->request_number)
	    {
	      _stage1_clear_response (sys_context, &(loader_data->stage1));
	      _stage2_clear_response (sys_context, &(loader_data->stage2));
	      loader_data->stage2.level = level;
	      loader_data->stage2.game_struct = game_struct;
	      loader_data->stage2.game_state = game_state;
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("async load stage2 done, request_number %d, game_state \"%s\""), request_number, repr);
	      ok = 1;
	    }
	  else if (request_number < loader_data->request_number)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO,
			  _x_ ("async load stage2 abort, request number %d < %d, game_state \"%s\""), request_number, loader_data->request_number, repr);
	      lw6ker_game_state_free (sys_context, game_state);
	      game_state = NULL;
	      lw6ker_game_struct_free (sys_context, game_struct);
	      game_struct = NULL;
	      lw6map_free (sys_context, level);
	      level = NULL;
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			  _x_
			  ("async load stage2, inconsistent request number %d > %d, game_state \"%s\""), request_number, loader_data->request_number, repr);
	    }
	  _LOADER_UNLOCK (sys_context);
	  LW6SYS_FREE (sys_context, repr);
	  repr = NULL;
	}
    }
  if (ok)
    {
      (*(progress.value)) = _LW6TSK_LOADER_PROGRESS_STAGE2_END;
    }
}

void
_lw6tsk_loader_poll (lw6sys_context_t * sys_context, _lw6tsk_loader_data_t * loader_data)
{
  loader_data->stage = 1;
  _stage1 (sys_context, loader_data);
  loader_data->stage = 2;
  _stage2 (sys_context, loader_data);
  loader_data->stage = 0;
}

/**
 * lw6tsk_loader_push_ldr
 *
 * @sys_context: global system context
 * @loader: loader object
 * @map_path: map-path config entry
 * @relative_path: relative map path
 * @default_param: default parameters to use for load
 * @forced_param: parameters to be forced and their values
 * @display_w: display width
 * @display_h: display height
 * @bench_value: bench value, reflecting computer CPU power
 * @magic_number: used to calibrate speed
 *
 * Pushes a load request to the loader. Will stop the current
 * load and push a new one. The request concerns a map which
 * should be loaded from a map directory on the filesystem.
 *
 * Return value: none.
 */
void
lw6tsk_loader_push_ldr (lw6sys_context_t * sys_context, lw6tsk_loader_t * loader, const char *map_path,
			const char *relative_path,
			lw6sys_assoc_t * default_param, lw6sys_assoc_t * forced_param, int display_w, int display_h, int bench_value, int magic_number)
{
  _lw6tsk_loader_data_t *loader_data;

  loader_data = (_lw6tsk_loader_data_t *) loader->data;

  _LOADER_LOCK (sys_context);

  loader_data->request_number++;
  _clear (sys_context, loader_data);
  loader_data->stage1.map_path = lw6sys_str_copy (sys_context, map_path);
  loader_data->stage1.relative_path = lw6sys_str_copy (sys_context, relative_path);
  loader_data->stage1.default_param = lw6sys_assoc_dup (sys_context, default_param, (lw6sys_dup_func_t) lw6sys_str_copy);
  loader_data->stage1.forced_param = lw6sys_assoc_dup (sys_context, forced_param, (lw6sys_dup_func_t) lw6sys_str_copy);
  loader_data->stage1.seed = NULL;
  loader_data->stage1.display_w = display_w;
  loader_data->stage1.display_h = display_h;
  loader_data->stage1.bench_value = bench_value;
  loader_data->stage1.magic_number = magic_number;

  _LOADER_UNLOCK (sys_context);
}

/**
 * lw6tsk_loader_push_gen
 *
 * @sys_context: global system context
 * @loader: loader object
 * @seed: seed string used to create the map
 * @display_w: display width
 * @display_h: display height
 * @bench_value: bench value, reflecting computer CPU power
 * @magic_number: used to calibrate speed
 *
 * Pushes a load request to the loader. Will stop the current
 * load and push a new one. The request is forwarded to the
 * pseudo-random map generation module.
 *
 * Return value: none.
 */
void
lw6tsk_loader_push_gen (lw6sys_context_t * sys_context, lw6tsk_loader_t * loader, const char *seed, int display_w, int display_h, int bench_value,
			int magic_number)
{
  _lw6tsk_loader_data_t *loader_data;

  loader_data = (_lw6tsk_loader_data_t *) loader->data;

  _LOADER_LOCK (sys_context);

  loader_data->request_number++;
  _clear (sys_context, loader_data);
  loader_data->stage1.map_path = NULL;
  loader_data->stage1.relative_path = NULL;
  loader_data->stage1.default_param = NULL;
  loader_data->stage1.forced_param = NULL;
  loader_data->stage1.seed = lw6gen_seed_normalize (sys_context, seed);
  loader_data->stage1.display_w = display_w;
  loader_data->stage1.display_h = display_h;
  loader_data->stage1.bench_value = bench_value;
  loader_data->stage1.magic_number = magic_number;

  _LOADER_UNLOCK (sys_context);
}

/**
 * lw6tsk_loader_pop
 *
 * @sys_context: global system context
 * @level: loaded level (out param)
 * @game_struct: loaded struct (out param)
 * @game_state: loaded state (out param)
 * @bench_value: the bench_value used (out param)
 * @loader: loader object
 *
 * Pops data from the loader, will allocate everything dynamically.
 * Function can either return just level or level and game struct and
 * game state (3 of them together). It's safe to use the received
 * level, display it right away, then wait for the rest. If things
 * are loaded fast enough, you just receive everything at once.
 *
 * Return value: 1 if some data, 0 if none.
 */
int
lw6tsk_loader_pop (lw6sys_context_t * sys_context, lw6map_level_t ** level, lw6ker_game_struct_t ** game_struct, lw6ker_game_state_t ** game_state,
		   int *bench_value, lw6tsk_loader_t * loader)
{
  int ret = 0;
  _lw6tsk_loader_data_t *loader_data;

  loader_data = (_lw6tsk_loader_data_t *) loader->data;

  _LOADER_LOCK (sys_context);

  if (level)
    {
      (*level) = NULL;
    }
  if (game_struct)
    {
      (*game_struct) = NULL;
    }
  if (game_state)
    {
      (*game_state) = NULL;
    }
  if (bench_value)
    {
      (*bench_value) = 0;
    }

  if (level)
    {
      if (loader_data->stage1.level)
	{
	  (*level) = loader_data->stage1.level;
	  loader_data->stage1.level = NULL;	// do not free!
	  ret = 1;
	}
    }
  if (level && game_struct && game_state)
    {
      if (loader_data->stage2.level && loader_data->stage2.game_struct && loader_data->stage2.game_state && (!(*level)))
	{
	  (*level) = loader_data->stage2.level;
	  (*game_struct) = loader_data->stage2.game_struct;
	  (*game_state) = loader_data->stage2.game_state;
	  loader_data->stage2.level = NULL;	// do not free!
	  loader_data->stage2.game_struct = NULL;	// do not free!
	  loader_data->stage2.game_state = NULL;	// do not free!
	  if (bench_value)
	    {
	      (*bench_value) = loader_data->stage1.bench_value;
	    }
	  ret = 1;
	}
    }

  _LOADER_UNLOCK (sys_context);

  return ret;
}

/*
 * This is used to stamp task loaders as they are created.
 * Note that there's no race condition due to the fact that this
 * is global here, in fact even when 2 processes would share
 * this sequence id, it would not matter for they would then
 * try and identify the objects in their on per-process lists,
 * structures, Guile object, whatever they use.
 */
static volatile u_int32_t seq_id = 0;

static void
_loader_data_free (lw6sys_context_t * sys_context, _lw6tsk_loader_data_t * loader_data)
{
  if (loader_data->mutex)
    {
      _LOADER_LOCK (sys_context);

      _clear (sys_context, loader_data);

      _LOADER_UNLOCK (sys_context);
      LW6SYS_MUTEX_DESTROY (sys_context, loader_data->mutex);
    }
  if (loader_data->user_dir)
    {
      LW6SYS_FREE (sys_context, loader_data->user_dir);
    }
  LW6SYS_FREE (sys_context, loader_data);
}

static void
_loader_callback (lw6sys_context_t * sys_context, void *data)
{
  _lw6tsk_loader_data_t *loader_data;

  loader_data = (_lw6tsk_loader_data_t *) data;

  while (!loader_data->stop)
    {
      _lw6tsk_loader_poll (sys_context, loader_data);
      lw6sys_sleep (sys_context, loader_data->sleep);
    }
}

static void
_loader_join (lw6sys_context_t * sys_context, void *data)
{
  _lw6tsk_loader_data_t *loader_data;

  loader_data = (_lw6tsk_loader_data_t *) data;

  _loader_data_free (sys_context, loader_data);
}

/**
 * lw6tsk_loader_new
 *
 * @sys_context: global system context
 * @sleep: how many seconds to wait between every poll
 * @user_dir: user directory
 * @progress: progress indicator to use
 *
 * Creates a new loader. This object is used to do some reputed
 * slow calculus in the background, in a separated thread. Typical example
 * is map loading. This is a high-level objects which encapsulates threads
 * and other wizardry.
 *
 * Return value: a pointer to the loader, NULL if failed.
 */
lw6tsk_loader_t *
lw6tsk_loader_new (lw6sys_context_t * sys_context, float sleep, char *user_dir, volatile float *progress)
{
  lw6tsk_loader_t *loader = NULL;
  _lw6tsk_loader_data_t *loader_data = NULL;

  loader = (lw6tsk_loader_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6tsk_loader_t));
  if (loader)
    {
      loader->id = 0;
      while (!loader->id)
	{
	  loader->id = ++seq_id;
	}
      loader_data = (_lw6tsk_loader_data_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6tsk_loader_data_t));
      loader->data = loader_data;
      if (loader->data)
	{
	  loader_data->sleep = sleep;
	  loader_data->user_dir = lw6sys_str_copy (sys_context, user_dir);
	  loader_data->progress = progress;
	  loader_data->mutex = LW6SYS_MUTEX_CREATE (sys_context);
	  if (loader_data->mutex)
	    {
	      loader->thread = lw6sys_thread_create (sys_context, _loader_callback, _loader_join, loader->data);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to create mutex for loader"));
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to allocate memory for loader data"));
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to allocate memory for loader"));
    }

  if (loader && (!loader->thread || !loader->data || (loader_data && !loader_data->user_dir)))
    {
      if (loader->thread)
	{
	  lw6sys_thread_join (sys_context, loader->thread);
	}
      if (loader_data)
	{
	  _loader_data_free (sys_context, loader_data);
	}
      LW6SYS_FREE (sys_context, loader);
      loader = NULL;
    }

  return loader;
}

/**
 * lw6tsk_loader_free
 *
 * @sys_context: global system context
 * @loader: the loader to free.
 *
 * Deletes a loader. Will automatically stop the child thread,
 * free data, and so on.
 *
 * Return value: none.
 */
void
lw6tsk_loader_free (lw6sys_context_t * sys_context, lw6tsk_loader_t * loader)
{
  _lw6tsk_loader_data_t *loader_data;

  loader_data = (_lw6tsk_loader_data_t *) loader->data;
  loader_data->stop = 1;
  lw6sys_thread_join (sys_context, loader->thread);
  // no need to free loader_data, done by join
  LW6SYS_FREE (sys_context, loader);
}

/**
 * lw6tsk_loader_repr
 *
 * @sys_context: global system context
 * @loader: the loader to represent.
 *
 * Creates a string which briefly describes the loader.
 *
 * Return value: a dynamically allocated pointer, must be freed.
 */
char *
lw6tsk_loader_repr (lw6sys_context_t * sys_context, const lw6tsk_loader_t * loader)
{
  char *ret = NULL;
  _lw6tsk_loader_data_t *loader_data;
  float progress = 0.0f;

  if (loader)
    {
      loader_data = (_lw6tsk_loader_data_t *) loader->data;
      if (loader_data->progress)
	{
	  progress = *(loader_data->progress);
	}
      ret =
	lw6sys_new_sprintf (sys_context, _x_ ("%u request_number=%d stage=%d progress=%0.2f"), loader->id, loader_data->request_number, loader_data->stage,
			    progress);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't generate string id for NULL loader"));
    }

  return ret;
}

/**
 * lw6tsk_loader_get_stage
 *
 * @sys_context: global system context
 * @loader: the loader to query.
 *
 * Returns the current stage of the loader.
 *
 * Return value: 0 if idle, 1 if loading the map from disk, 2 if build
 *               dynamic stuff such as game_state.
 */
int
lw6tsk_loader_get_stage (lw6sys_context_t * sys_context, lw6tsk_loader_t * loader)
{
  int ret = 0;
  _lw6tsk_loader_data_t *loader_data;

  loader_data = (_lw6tsk_loader_data_t *) loader->data;
  ret = loader_data->stage;

  return ret;
}
