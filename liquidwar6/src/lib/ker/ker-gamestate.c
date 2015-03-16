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
#endif // HAVE_CONFIG_H

#include <string.h>

#include "ker.h"
#include "ker-internal.h"

/*
 * This is used to stamp game_structs as they are created.
 * Note that there's no race condition due to the fact that this
 * is global here, in fact even when 2 processes would share
 * this sequence id, it would not matter for they would then
 * try and identify the objects in their on per-process lists,
 * structures, Guile object, whatever they use.
 */
static volatile u_int32_t seq_id = 0;

/*
 * The sequence generation is done by a function since seq_id is
 * static and cannot be accessed from elsewhere, but we decided
 * to put some constateors outside this file (in ker-hexa.c for
 * instance).
 */
void
_lw6ker_game_state_set_id (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state)
{
  game_state->id = 0;
  while (!game_state->id)
    {
      game_state->id = ++seq_id;
    }
}

_lw6ker_game_state_t *
_lw6ker_game_state_new (lw6sys_context_t * sys_context, const _lw6ker_game_struct_t * game_struct, lw6sys_progress_t * progress)
{
  _lw6ker_game_state_t *ret = NULL;

  ret = (_lw6ker_game_state_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6ker_game_state_t));
  if (ret)
    {
      _lw6ker_game_state_set_id (sys_context, ret);
      ret->game_struct = game_struct;

      /*
       * We initialize total_rounds now, we will possibly increment
       * it later if we need to make the game longer because
       * there's only one team present.
       */
      ret->total_rounds = game_struct->rules.rounds_per_sec * game_struct->rules.total_time;

      _lw6ker_map_state_init (sys_context, &(ret->map_state), &(ret->game_struct->map_struct), &(ret->game_struct->rules), progress);
      _lw6ker_node_array_init (sys_context, &(ret->node_array));
    }

  return ret;
}

/**
 * lw6ker_game_state_new
 *
 * @sys_context: global system context
 * @game_struct: game_struct use to construct the object
 * @progress: progress indicator
 *
 * Creates a game state from a game struct. The game struct must be kept
 * (never freed) while game_state is in use.
 *
 * Return value: newly created object.
 */
lw6ker_game_state_t *
lw6ker_game_state_new (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct, lw6sys_progress_t * progress)
{
  return (lw6ker_game_state_t *) _lw6ker_game_state_new (sys_context, (const _lw6ker_game_struct_t *) game_struct, progress);
}

void
_lw6ker_game_state_free (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state)
{
  /*
   * IMPORTANT NOTE: it's important *not* to reference game_struct
   * in this free function. The idea is that game_struct can be
   * freed *before* game_state. Why is that is linked with arcane
   * garbage collection wizardry. Theorically the cleanest way would
   * be to make Guile aware that a given game_state requires a game_struct.
   * In a first approach, it's easier (but quirk and dirty, that's true)
   * not to care about this reference stuff, and simply manage
   * things so that whenever we *use* game_state, game_struct is still
   * alive. But we don't control what the garbage collector does,
   * and once it decides to purge objects (we have no more refs on
   * them that is) it can purge them in any order...
   */
  _lw6ker_map_state_clear (sys_context, &(game_state->map_state));

  LW6SYS_FREE (sys_context, game_state);
}

/**
 * lw6ker_game_state_free
 *
 * @sys_context: global system context
 * @game_state: the object to free
 *
 * Frees a game_state object, releases all required objects. At this stage
 * the map_struct must still be available.
 *
 * Return value: none
 */
void
lw6ker_game_state_free (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state)
{
  _lw6ker_game_state_free (sys_context, (_lw6ker_game_state_t *) game_state);
}

void
_lw6ker_game_state_point_to (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, const _lw6ker_game_struct_t * game_struct)
{
  int i;

  game_state->game_struct = game_struct;
  game_state->map_state.map_struct = &(game_struct->map_struct);
  game_state->map_state.armies.map_struct = &(game_struct->map_struct);
  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
    {
      game_state->map_state.teams[i].map_struct = &(game_struct->map_struct);
    }
}

/**
 * lw6ker_game_state_point_to
 *
 * @sys_context: global system context
 * @game_state: the game_state to modify
 * @game_struct: the game_struct to point to
 *
 * This can be used when one makes a copy (dup) of a game struct and for some
 * reason want the game_state to point on this new copy. Of course you should
 * make the game_state point to a game_struct that is identical to the one
 * that was used to construct the object in the first place. Use at your own
 * risk.
 *
 * Return value: none
 */
void
lw6ker_game_state_point_to (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, const lw6ker_game_struct_t * game_struct)
{
  _lw6ker_game_state_point_to (sys_context, (_lw6ker_game_state_t *) game_state, (const _lw6ker_game_struct_t *) game_struct);
}

int
_lw6ker_game_state_memory_footprint (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state)
{
  int ret = 0;

  // todo

  return ret;
}

/**
 * lw6ker_game_state_memory_footprint
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 *
 * Returns the approximative amount of memory taken by the object.
 *
 * Return value: number of bytes (approximation)
 */
int
lw6ker_game_state_memory_footprint (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state)
{
  return _lw6ker_game_state_memory_footprint (sys_context, (_lw6ker_game_state_t *) game_state);
}

char *
_lw6ker_game_state_repr (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  char *ret = NULL;

  if (game_state)
    {
      /*
       * do not use references to game_struct or level here,
       * function is called when freeing the object
       */
      ret =
	lw6sys_new_sprintf (sys_context,
			    "%u (%dx%dx%d, round %d, %d fighters)",
			    game_state->id, game_state->map_state.shape.w,
			    game_state->map_state.shape.h,
			    game_state->map_state.shape.d,
			    _lw6ker_game_state_get_rounds (sys_context, game_state), _lw6ker_game_state_get_nb_active_fighters (sys_context, game_state));
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't generate string id for NULL game_state"));
    }

  return ret;
}

/**
 * lw6ker_game_state_repr
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 *
 * Gives a readable representation of the object.
 *
 * Return value: newly allocated string, must be freed
 */
char *
lw6ker_game_state_repr (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return _lw6ker_game_state_repr (sys_context, (_lw6ker_game_state_t *) game_state);
}

int
_lw6ker_game_state_can_sync (lw6sys_context_t * sys_context, _lw6ker_game_state_t * dst, const _lw6ker_game_state_t * src)
{
  int ret = 0;

  /*
   * Sanity check, this function is designed to copy game states
   * which correspond to the same game struct. Any other use is
   * useless in LW6 context.
   */
  if (dst && src && dst->game_struct && src->game_struct)
    {
      if (dst == src)
	{
	  ret = 1;
	}
      else
	{
	  ret = _lw6ker_map_struct_lazy_compare (sys_context, &(dst->game_struct->map_struct), &(src->game_struct->map_struct));
	}
    }

  return ret;
}

/**
 * lw6ker_game_state_can_sync
 *
 * @sys_context: global system context
 * @dst: the destination game_state
 * @src: the source game_state
 *
 * Tells wether src and dst can be synced. This is not a fool proof function
 * but in most cases it will raise the error, use it to avoid blunders. It
 * just compares @dst and @src and tries to guess if they correspond to the
 * same logical objects.
 *
 * Return value: 1 if they are syncable, 0 if not.
 */
int
lw6ker_game_state_can_sync (lw6sys_context_t * sys_context, lw6ker_game_state_t * dst, const lw6ker_game_state_t * src)
{
  return _lw6ker_game_state_can_sync (sys_context, (_lw6ker_game_state_t *) dst, (const _lw6ker_game_state_t *) src);
}

int
_lw6ker_game_state_sync (lw6sys_context_t * sys_context, _lw6ker_game_state_t * dst, const _lw6ker_game_state_t * src)
{
  int ret = 0;

  /*
   * Sanity check, this function is designed to copy game states
   * which correspond to the same game struct. Any other use is
   * useless in LW6 context.
   */
  if (_lw6ker_game_state_can_sync (sys_context, dst, src))
    {
      if (dst == src)
	{
	  /*
	   * Special case where source and destination are the
	   * same, this can happen when testing, or when
	   * handling demo maps for instance, in that case
	   * we just do... nothing!
	   */
	  ret = 1;
	}
      else
	{
	  ret = _lw6ker_map_state_sync (sys_context, &dst->map_state, &src->map_state);
	  memcpy (&(dst->node_array), &(src->node_array), sizeof (_lw6ker_node_array_t));
	  memcpy (&(dst->global_history), &(src->global_history), sizeof (_lw6ker_history_t));
	  memcpy (&(dst->latest_history), &(src->latest_history), sizeof (_lw6ker_history_t));
	  dst->moves = src->moves;
	  dst->spreads = src->spreads;
	  dst->rounds = src->rounds;
	  dst->total_rounds = src->total_rounds;
	  dst->max_reached_teams = src->max_reached_teams;
	  dst->over = src->over;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("game_state_copy only works if dst and src point to the same game_struct"));
    }

  return ret;
}

/**
 * lw6ker_game_state_sync
 *
 * @sys_context: global system context
 * @dst: the destination game_state
 * @src: the source game_state
 *
 * Fundamental function, used to carbon copy a game state to another,
 * this is intensively used to keep too tracks of the game state, one
 * most-up-to-date but probably wrong, the one we use to display on the
 * screen, and one slightly outdated (or very outdated if network is
 * slow) but that we're sure of, something 100% bullet proof we can
 * rely on.
 *
 * Return value: 1 on success, 0 on error
 */
int
lw6ker_game_state_sync (lw6sys_context_t * sys_context, lw6ker_game_state_t * dst, const lw6ker_game_state_t * src)
{
  return _lw6ker_game_state_sync (sys_context, (_lw6ker_game_state_t *) dst, (const _lw6ker_game_state_t *) src);
}

_lw6ker_game_state_t *
_lw6ker_game_state_dup (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, lw6sys_progress_t * progress)
{
  _lw6ker_game_state_t *ret;

  ret = _lw6ker_game_state_new (sys_context, game_state->game_struct, progress);
  if (ret)
    {
      if (_lw6ker_game_state_sync (sys_context, ret, game_state))
	{
	  // ok
	}
      else
	{
	  _lw6ker_game_state_free (sys_context, ret);
	  ret = NULL;
	}
    }

  if (ret)
    {
      if (_lw6ker_game_state_checksum (sys_context, ret) == _lw6ker_game_state_checksum (sys_context, game_state))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("game_state dup %d->%d"), game_state->id, ret->id);
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("game_state checkum mismatch after dup"));
	}
    }

  return ret;
}

/**
 * lw6ker_game_state_dup
 *
 * @sys_context: global system context
 * @game_state: the game_state to copy
 * @progress: progress indicator
 *
 * Dups (copy) a game_state object. The newly created object points to
 * the same game_struct but is an independant copy, you can play
 * a whole different game on it. In practice this is often used
 * to create the game_state objects for anticipation in network games.
 *
 * Return value: newly created object
 */
lw6ker_game_state_t *
lw6ker_game_state_dup (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, lw6sys_progress_t * progress)
{
  return (lw6ker_game_state_t *) _lw6ker_game_state_dup (sys_context, (const _lw6ker_game_state_t *) game_state, progress);
}

void
_lw6ker_game_state_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, u_int32_t * checksum)
{
  _lw6ker_game_struct_update_checksum (sys_context, game_state->game_struct, checksum);
  _lw6ker_map_state_update_checksum (sys_context, &(game_state->map_state), checksum);
  _lw6ker_node_array_update_checksum (sys_context, &(game_state->node_array), checksum);
  _lw6ker_history_update_checksum (sys_context, &(game_state->global_history), checksum);
  _lw6ker_history_update_checksum (sys_context, &(game_state->latest_history), checksum);
  lw6sys_checksum_update_int32 (sys_context, checksum, game_state->moves);
  lw6sys_checksum_update_int32 (sys_context, checksum, game_state->spreads);
  lw6sys_checksum_update_int32 (sys_context, checksum, game_state->rounds);
  lw6sys_checksum_update_int32 (sys_context, checksum, game_state->total_rounds);
  lw6sys_checksum_update_int32 (sys_context, checksum, game_state->max_reached_teams);
  lw6sys_checksum_update_int32 (sys_context, checksum, game_state->over);
}

u_int32_t
_lw6ker_game_state_checksum (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  u_int32_t ret = 0;

  _lw6ker_game_state_update_checksum (sys_context, game_state, &ret);

  return ret;
}

/**
 * lw6ker_game_state_checksum
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 *
 * Calculates the checksum of a game_state, this can be very usefull
 * to make sure two states are identicall (prevent network errors and/or
 * cheating).
 *
 * Return value: 32-bit checksum
 */
u_int32_t
lw6ker_game_state_checksum (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return _lw6ker_game_state_checksum (sys_context, (const _lw6ker_game_state_t *) game_state);
}

/**
 * lw6ker_game_state_get_shape
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 * @shape: the shape (out param)
 *
 * Retrieves the shape (w*h*d)of the game_state.
 *
 * Return value: none.
 */
void
lw6ker_game_state_get_shape (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, lw6sys_whd_t * shape)
{
  (*shape) = ((_lw6ker_game_state_t *) game_state)->map_state.shape;
}

/**
 * lw6ker_game_state_get_w
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 *
 * Retrieves the width (shape.w) of the game_state.
 *
 * Return value: the width.
 */
int
lw6ker_game_state_get_w (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return ((_lw6ker_game_state_t *) game_state)->map_state.shape.w;
}

/**
 * lw6ker_game_state_get_h
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 *
 * Retrieves the height (shape.h) of the game_state.
 *
 * Return value: the height.
 */
int
lw6ker_game_state_get_h (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return ((_lw6ker_game_state_t *) game_state)->map_state.shape.h;
}

/**
 * lw6ker_game_state_get_d
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 *
 * Retrieves the depth (shape.d, AKA number of layers) of the game_state.
 *
 * Return value: the depth.
 */
int
lw6ker_game_state_get_d (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return ((_lw6ker_game_state_t *) game_state)->map_state.shape.d;
}

int
_lw6ker_game_state_register_node (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, u_int64_t node_id)
{
  int ret = 0;

  if (!_lw6ker_game_state_node_exists (sys_context, game_state, node_id))
    {
      ret = _lw6ker_node_array_enable (sys_context, &(game_state->node_array), node_id);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("node %" LW6SYS_PRINTF_LL "x already exists"), (long long) node_id);
    }

  return ret;
}

/**
 * lw6ker_game_state_register_node
 *
 * @sys_context: global system context
 * @game_state: the game_state to act on
 * @node_id: the id of the node to register
 *
 * Registers a node in the game, this must be done, else no action will
 * be allowed (such as adding a cursor or moving it). There's a limited number
 * of nodes allowed, and ids must be unique.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6ker_game_state_register_node (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, u_int64_t node_id)
{
  return _lw6ker_game_state_register_node (sys_context, (_lw6ker_game_state_t *) game_state, node_id);
}

int
_lw6ker_game_state_unregister_node (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, u_int64_t node_id)
{
  int ret = 0;
  int i;
  lw6ker_cursor_t *cursor;

  if (_lw6ker_game_state_node_exists (sys_context, game_state, node_id))
    {
      for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
	{
	  cursor = &(game_state->map_state.cursor_array.cursors[i]);
	  if (cursor->enabled && cursor->node_id == node_id)
	    {
	      _lw6ker_game_state_remove_cursor (sys_context, game_state, node_id, cursor->cursor_id);
	    }
	}
      ret = _lw6ker_node_array_disable (sys_context, &(game_state->node_array), node_id);
    }

  return ret;
}

/**
 * lw6ker_game_state_unregister_node
 *
 * @sys_context: global system context
 * @game_state: the game_state to act on
 * @node_id: the id of the node to register
 *
 * Unregisters a node in the game, this must be done when a node leaves the
 * game, it will free ressources and allow others to connect.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6ker_game_state_unregister_node (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, u_int64_t node_id)
{
  return _lw6ker_game_state_unregister_node (sys_context, (_lw6ker_game_state_t *) game_state, node_id);
}

int
_lw6ker_game_state_node_exists (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, u_int64_t node_id)
{
  int ret = 0;

  ret = (_lw6ker_node_array_get_ro (sys_context, &(game_state->node_array), node_id) != NULL);

  return ret;
}

/**
 * lw6ker_game_state_node_exists
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 * @node_id: the node to test
 *
 * Tells wether a node is present in a game.
 *
 * Return value: 1 if node is in game, 0 if not
 */
int
lw6ker_game_state_node_exists (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, u_int64_t node_id)
{
  return _lw6ker_game_state_node_exists (sys_context, (_lw6ker_game_state_t *) game_state, node_id);
}

int
_lw6ker_game_state_get_node_info (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, u_int16_t node_id, u_int32_t * last_command_round)
{
  int ret = 0;
  const _lw6ker_node_t *node = NULL;

  node = _lw6ker_node_array_get_ro (sys_context, &(game_state->node_array), node_id);
  if (node)
    {
      if (last_command_round)
	{
	  (*last_command_round) = node->last_command_round;
	}
      ret = 1;
    }

  return ret;

}

/**
 * lw6ker_game_state_get_node_info
 *
 * @sys_context: global system context
 * @game_state: game_state to query
 * @node_id: the node to get info about
 * @last_command_round: the last round for which a command was issued (out parameter)
 *
 * Queries information about a given node, mostly, what was the last round
 * we got a command.
 *
 * Return value: 1 on success, 0 on error.
 */
int
lw6ker_game_state_get_node_info (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, u_int16_t node_id, u_int32_t * last_command_round)
{
  return _lw6ker_game_state_get_node_info (sys_context, (_lw6ker_game_state_t *) game_state, node_id, last_command_round);
}

static int
_check_node_id (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, u_int64_t node_id)
{
  int ret = 0;

  if (_lw6ker_game_state_node_exists (sys_context, game_state, node_id))
    {
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("node %" LW6SYS_PRINTF_LL "x does not exist"), (long long) node_id);
    }

  return ret;
}

int
_lw6ker_game_state_add_cursor (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, u_int64_t node_id, u_int16_t cursor_id, int team_color)
{
  int ret = 0;
  const lw6map_rules_t *rules = NULL;
  int team_exists = 0;
  int real_team_color = LW6MAP_TEAM_COLOR_INVALID;
  int32_t x = 0;
  int32_t y = 0;

  if (_check_node_id (sys_context, game_state, node_id))
    {
      rules = &(game_state->game_struct->rules);
      if (!_lw6ker_game_state_cursor_exists (sys_context, game_state, cursor_id))
	{
	  team_exists = _lw6ker_game_state_team_exists (sys_context, game_state, team_color);
	  real_team_color = team_color;
	  if (team_exists)
	    {
	      if ((rules->color_conflict_mode == 1
		   &&
		   !_lw6ker_cursor_array_is_color_owned_by (sys_context, &
							    (game_state->map_state.cursor_array), node_id, team_color)) || rules->color_conflict_mode == 2)
		{
		  real_team_color = _lw6ker_map_state_get_free_team_color (sys_context, &(game_state->map_state));
		}
	    }

	  if (lw6map_team_color_is_valid (real_team_color))
	    {
	      if (lw6map_exp_is_team_color_allowed (sys_context, rules, team_color))
		{
		  if (real_team_color != team_color)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("color shift: real_team_color=%d team_color=%d"), real_team_color, team_color);
		    }
		  if (!_lw6ker_game_state_team_exists (sys_context, game_state, real_team_color))
		    {
		      _lw6ker_game_state_add_team (sys_context, game_state, node_id, real_team_color);
		    }
		  if (_lw6ker_game_state_team_exists (sys_context, game_state, real_team_color))
		    {
		      if (_lw6ker_cursor_get_start_xy
			  (sys_context, &x, &y, real_team_color,
			   rules->start_position_mode, _lw6ker_game_state_get_rounds (sys_context, game_state), &(game_state->map_state.shape), rules))
			{
			  ret = _lw6ker_cursor_array_enable (sys_context, &(game_state->map_state.cursor_array), node_id, cursor_id, real_team_color, x, y);
			}
		    }
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("unable to add team %d, game is full"), (int) team_color);
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("cursor %x already exists"), (int) cursor_id);
	}
    }

  return ret;
}

/**
 * lw6ker_game_state_add_cursor
 *
 * @sys_context: global system context
 * @game_state: the game_state to act upon
 * @node_id: the node issuing the command
 * @cursor_id: the id of the cursor to add
 * @team_color: the color we wish
 *
 * Adds a cursor in a game. Note that if there's already a cursor with
 * that id, it will fail, and the color is only the color we wish,
 * we might indeed be attributed another color on a successfull call.
 *
 * Return value: 1 on success, 0 on error.
 */
int
lw6ker_game_state_add_cursor (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, u_int64_t node_id, u_int16_t cursor_id, int team_color)
{
  return _lw6ker_game_state_add_cursor (sys_context, (_lw6ker_game_state_t *) game_state, node_id, cursor_id, team_color);
}

int
_lw6ker_game_state_remove_cursor (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, u_int64_t node_id, u_int16_t cursor_id)
{
  int ret = 0;
  int32_t nb_cursors = 0;
  int team_color = LW6MAP_TEAM_COLOR_INVALID;
  lw6ker_cursor_t cursor;

  if (_check_node_id (sys_context, game_state, node_id))
    {
      if (_lw6ker_game_state_cursor_exists (sys_context, game_state, cursor_id))
	{
	  _lw6ker_game_state_get_cursor (sys_context, game_state, &cursor, cursor_id);
	  team_color = cursor.team_color;
	  ret = _lw6ker_cursor_array_disable (sys_context, &(game_state->map_state.cursor_array), node_id, cursor_id);
	  if (_lw6ker_game_state_get_team_info (sys_context, game_state, team_color, &nb_cursors, NULL))
	    {
	      if (!nb_cursors)
		{
		  _lw6ker_game_state_remove_team (sys_context, game_state, node_id, team_color);
		}
	    }
	}
    }

  return ret;
}

/**
 * lw6ker_game_state_remove_cursor
 *
 * @sys_context: global system context
 * @game_state: the game_state to act upon
 * @node_id: the node issuing the command
 * @cursor_id: the id of the cursor to remove
 *
 * Removes a cursor from the game, corresponding teams will be removed
 * if needed.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6ker_game_state_remove_cursor (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, u_int64_t node_id, u_int16_t cursor_id)
{
  return _lw6ker_game_state_remove_cursor (sys_context, (_lw6ker_game_state_t *) game_state, node_id, cursor_id);
}

int
_lw6ker_game_state_cursor_exists (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, u_int16_t cursor_id)
{
  int ret = 0;
  const lw6ker_cursor_t *cursor = NULL;
  cursor = _lw6ker_cursor_array_get_ro (sys_context, &(game_state->map_state.cursor_array), cursor_id);
  if (cursor)
    {
      ret = 1;
    }

  return ret;
}

/**
 * lw6ker_game_state_cursor_exists
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 * @cursor_id: the cursor to test
 *
 * Tells wether a cursor is present in the game.
 *
 * Return value: 1 if cursor exists, 0 if not.
 */
int
lw6ker_game_state_cursor_exists (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, u_int16_t cursor_id)
{
  return _lw6ker_game_state_cursor_exists (sys_context, (const _lw6ker_game_state_t *) game_state, cursor_id);
}

int
_lw6ker_game_state_get_cursor (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, lw6ker_cursor_t * cursor, u_int16_t cursor_id)
{
  int ret = 0;
  const lw6ker_cursor_t *found_cursor = NULL;

  found_cursor = _lw6ker_cursor_array_get_ro (sys_context, &(game_state->map_state.cursor_array), cursor_id);
  if (cursor)
    {
      lw6ker_cursor_reset (sys_context, cursor);
    }
  if (found_cursor)
    {
      if (cursor)
	{
	  (*cursor) = (*found_cursor);
	}
      ret = 1;
    }

  return ret;
}

/**
 * lw6ker_game_state_get_cursor
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 * @cursor: the cursor data (out param)
 * @cursor_id: the cursor to query
 *
 * Get a pointer on a given cursor, pointer is read-only.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6ker_game_state_get_cursor (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, lw6ker_cursor_t * cursor, u_int16_t cursor_id)
{
  return _lw6ker_game_state_get_cursor (sys_context, (const _lw6ker_game_state_t *) game_state, cursor, cursor_id);
}

void
_lw6ker_game_state_get_cursor_by_index (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, lw6ker_cursor_t * cursor, int i)
{
  (*cursor) = game_state->map_state.cursor_array.cursors[i];
}

/**
 * lw6ker_game_state_get_cursor_by_index
 *
 * @sys_context: global system context
 * @game_state: the game state to query
 * @cursor: the cursor (out param)
 * @i: the index
 *
 * Gets the cursor information, using its index. This is usefull to walk
 * the whole cursor without knowing their ids. Pointer is read-only.
 *
 * Return value: none.
 */
void
lw6ker_game_state_get_cursor_by_index (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, lw6ker_cursor_t * cursor, int i)
{
  _lw6ker_game_state_get_cursor_by_index (sys_context, (const _lw6ker_game_state_t *) game_state, cursor, i);
}

int
_lw6ker_game_state_set_cursor (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, lw6ker_cursor_t * cursor)
{
  int ret = 0;
  int x, y, fire, fire2;

  if (_check_node_id (sys_context, game_state, cursor->node_id))
    {
      x = cursor->pos.x;
      y = cursor->pos.y;
      fire = cursor->fire;
      fire2 = cursor->fire2;
      lw6map_coords_fix_xy (&(game_state->game_struct->rules), &(game_state->map_state.shape), &x, &y);
      ret =
	_lw6ker_cursor_array_update (sys_context,
				     &(game_state->map_state.cursor_array),
				     cursor->node_id, cursor->cursor_id, x, y,
				     fire, fire2, 0, &(game_state->map_state.shape), &(game_state->game_struct->rules));
    }

  return ret;
}

/**
 * lw6ker_game_state_set_cursor
 *
 * @sys_context: global system context
 * @game_state: the game_state to act upon
 * @cursor: the cursor
 *
 * Sets a cursor, that is, changes its position, this is pretty much
 * anything we can do about a cursor except adding or removing it, just
 * because of Liquid War very simple rules. The passed pointer may be
 * freed after the call, only the @cursor_id, @node_id, @x, @y and @fire fields
 * are used, others are ignored. More precisely, the @enabled will be ignored,
 * it's not a valid way to add/remove teams.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6ker_game_state_set_cursor (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, lw6ker_cursor_t * cursor)
{
  return _lw6ker_game_state_set_cursor (sys_context, (_lw6ker_game_state_t *) game_state, cursor);
}

int
_lw6ker_game_state_add_team_internal (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, int team_color, int position_mode)
{
  int ret = 0;
  int32_t i;
  /*
   * Some values as INT64 to force casts to long long and
   * avoid integer capacity overflow.
   */
  int64_t total_fighters;
  int64_t total_fighters_to_remove;
  const lw6map_rules_t *rules = NULL;

  rules = &(game_state->game_struct->rules);
  if (!_lw6ker_game_state_team_exists (sys_context, game_state, team_color))
    {
      int32_t nb_fighters;
      lw6sys_xyz_t desired_center;
      int32_t x;
      int32_t y;

      _lw6ker_cursor_get_start_xy (sys_context, &x, &y, team_color,
				   position_mode, _lw6ker_game_state_get_rounds (sys_context, game_state), &(game_state->map_state.shape), rules);
      desired_center.x = x;
      desired_center.y = y;
      desired_center.z = 0;

      nb_fighters = lw6ker_per100_2 (game_state->map_state.map_struct->room_for_armies, rules->single_army_size);
      if (nb_fighters + game_state->map_state.armies.active_fighters > game_state->map_state.armies.max_fighters)
	{
	  /*
	   * OK there will be too many fighters, now the rule is:
	   * a new team gets 1/n of total allowed fighters.
	   */
	  nb_fighters = game_state->map_state.armies.max_fighters / (_lw6ker_map_state_get_nb_teams (sys_context, &(game_state->map_state)) + 1);
	}

      /*
       * Changing the number of fighter to incorporate the handicap
       * set in the configuration. Doing it here will do it both
       * on game startup and respawn.
       */
      nb_fighters = (nb_fighters * rules->team_profile_handicap[team_color]) / LW6MAP_RULES_DEFAULT_TEAM_PROFILE_HANDICAP;

      /*
       * Remove fighters if needed
       */
      if (nb_fighters + game_state->map_state.armies.active_fighters > game_state->map_state.armies.max_fighters)
	{
	  total_fighters = game_state->map_state.armies.active_fighters;
	  total_fighters_to_remove = nb_fighters - (game_state->map_state.armies.max_fighters - total_fighters);
	  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	    {
	      _lw6ker_map_state_remove_team_fighters (sys_context, &
						      (game_state->map_state),
						      i, (total_fighters_to_remove * game_state->map_state.armies.fighters_per_team[i]) / total_fighters);
	    }
	  /*
	   * Note that this value has probably changed since we last
	   * calculated it...
	   */
	  total_fighters = game_state->map_state.armies.active_fighters;
	  total_fighters_to_remove = nb_fighters - (game_state->map_state.armies.max_fighters - total_fighters);
	  _lw6ker_map_state_remove_fighters (sys_context, &(game_state->map_state), total_fighters_to_remove);
	}
      game_state->map_state.teams[team_color].offline = 0;
      /*
       * Do add people.
       */
      _lw6ker_map_state_populate_team (sys_context, &game_state->map_state, team_color, nb_fighters, desired_center, rules);
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("can't add team %d, it's already active"), team_color);
    }

  return ret;
}

int
_lw6ker_game_state_add_team (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, u_int64_t node_id, int team_color)
{
  int ret = 0;

  if (_check_node_id (sys_context, game_state, node_id))
    {
      ret = _lw6ker_game_state_add_team_internal (sys_context, game_state, team_color, game_state->game_struct->rules.start_position_mode);
      if (ret)
	{
	  /*
	   * Set frags to 0. If we're in a network game and someone
	   * re-uses a color, we don't want these frags to count.
	   * Note that the other function _lw6ker_game_state_add_team_internal
	   * does *not* touch the frag count.
	   */
	  game_state->map_state.armies.frags[team_color] = 0;
	}
    }

  return ret;
}

int
_lw6ker_game_state_remove_team_internal (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, int team_color)
{
  int ret = 0;

  if (game_state->map_state.teams[team_color].active)
    {
      int32_t nb_active_teams;
      /*
       * Some values as INT64 to force casts to long long and
       * avoid integer capacity overflow.
       */
      int64_t nb_active_fighters;
      int64_t new_average_fighters_per_team;
      int64_t new_active_fighters;
      int64_t single_average_fighters_per_team;
      int64_t total_fighters_to_distribute;
      int32_t i;

      nb_active_teams = _lw6ker_map_state_get_nb_teams (sys_context, &(game_state->map_state));
      nb_active_fighters = game_state->map_state.armies.active_fighters;

      if (nb_active_teams <= 1)
	{
	  /*
	   * Weird situation, not sure it should happen, except when in
	   * a spectator mode -> not one single fighter left!
	   */
	  _lw6ker_map_state_remove_fighters (sys_context, &(game_state->map_state), nb_active_fighters);
	}
      else
	{
	  new_average_fighters_per_team = nb_active_fighters / (nb_active_teams - 1);
	  single_average_fighters_per_team =
	    lw6ker_per100_2 (game_state->map_state.map_struct->room_for_armies, game_state->game_struct->rules.single_army_size);
	  if (new_average_fighters_per_team > single_average_fighters_per_team)
	    {
	      _lw6ker_map_state_remove_team_fighters (sys_context, &
						      (game_state->map_state),
						      team_color, nb_active_fighters - single_average_fighters_per_team * (nb_active_teams - 1));
	    }

	  /*
	   * At this stage we have suppressed any fighter that needed
	   * to be, that is there are not enough teams to reach the
	   * absolute limit for fighters in this map.
	   */
	  new_active_fighters = game_state->map_state.armies.active_fighters;
	  /*
	   * Now, second step, we redistribute the remaining fighters
	   * to other teams
	   */
	  total_fighters_to_distribute = game_state->map_state.armies.fighters_per_team[team_color];
	  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	    {
	      if (i != team_color)
		{
		  _lw6ker_map_state_redistribute_team (sys_context, &
						       (game_state->map_state),
						       i, team_color,
						       (game_state->map_state.armies.fighters_per_team[i] *
							total_fighters_to_distribute) / new_active_fighters, &(game_state->game_struct->rules));
		}
	    }
	  /*
	   * We might get a few fighters left due to rounding/approximations,
	   * we get rid of them.
	   */
	  while (game_state->map_state.armies.fighters_per_team[team_color] > 0)
	    {
	      for (i = 0; i < LW6MAP_MAX_NB_TEAMS && game_state->map_state.armies.fighters_per_team[team_color] > 0; ++i)
		{
		  if (game_state->map_state.teams[i].active)
		    {
		      _lw6ker_map_state_redistribute_team (sys_context, &(game_state->map_state), i, team_color, 1, &(game_state->game_struct->rules));
		    }
		}
	    }
	  _lw6ker_map_state_cancel_team (sys_context, &(game_state->map_state), team_color);
	}

      ret = 1;
    }

  return ret;
}

int
_lw6ker_game_state_remove_team (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, u_int64_t node_id, int team_color)
{
  int ret = 0;

  if (_check_node_id (sys_context, game_state, node_id))
    {
      ret = _lw6ker_game_state_remove_team_internal (sys_context, game_state, team_color);
    }

  return ret;
}

int
_lw6ker_game_state_team_exists (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, int team_color)
{
  int ret = 0;

  if (team_color >= 0 && team_color < LW6MAP_MAX_NB_TEAMS)
    {
      ret = game_state->map_state.teams[team_color].active;
    }

  return ret;
}

/**
 * lw6ker_game_state_team_exists
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 * @team_color: the team color to test
 *
 * Tells wether a team color is present in the game. Note that this is
 * different from cursor ids.
 *
 * Return value: 1 if team exists, 0 if not.
 */
int
lw6ker_game_state_team_exists (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int team_color)
{
  return _lw6ker_game_state_team_exists (sys_context, (const _lw6ker_game_state_t *) game_state, team_color);
}

int
_lw6ker_game_state_get_team_info (lw6sys_context_t * sys_context,
				  const _lw6ker_game_state_t * game_state, int team_color, int32_t * nb_cursors, int32_t * nb_fighters)
{
  int ret = 0;
  const _lw6ker_team_t *team;
  const _lw6ker_cursor_array_t *cursor_array;
  int i = 0;

  if (team_color >= 0 && team_color < LW6MAP_MAX_NB_TEAMS)
    {
      team = &(game_state->map_state.teams[team_color]);
      if (team->active)
	{
	  ret = 1;
	  if (nb_cursors)
	    {
	      (*nb_cursors) = 0;
	      cursor_array = &(game_state->map_state.cursor_array);
	      for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
		{
		  if (cursor_array->cursors[i].enabled && cursor_array->cursors[i].team_color == team_color)
		    {
		      (*nb_cursors)++;
		    }
		}
	    }
	  if (nb_fighters)
	    {
	      (*nb_fighters) = game_state->map_state.armies.fighters_per_team[team_color];
	    }
	}
    }

  if (!ret)
    {
      if (nb_cursors)
	{
	  (*nb_cursors) = 0;
	}
      if (nb_fighters)
	{
	  (*nb_fighters) = 0;
	}
    }

  return ret;
}

/**
 * lw6ker_game_state_get_team_info
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 * @team_color: the color to get informations about
 * @nb_cursors: number of cursors with this color (out param)
 * @nb_fighters: number of fighters with this color (out param)
 *
 * Gets informations about a given color. Indeed, a color can have several
 * cursors, and knowing how many fighters there are with a given color
 * is probably the most important things about a color.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6ker_game_state_get_team_info (lw6sys_context_t * sys_context,
				 const lw6ker_game_state_t * game_state, int team_color, int32_t * nb_cursors, int32_t * nb_fighters)
{
  return _lw6ker_game_state_get_team_info (sys_context, (const _lw6ker_game_state_t *) game_state, team_color, nb_cursors, nb_fighters);
}

int
_lw6ker_game_state_get_nb_teams (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  int ret = 0;

  ret = _lw6ker_map_state_get_nb_teams (sys_context, &(game_state->map_state));

  return ret;
}

/**
 * lw6ker_game_state_get_nb_teams
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 *
 * Tells how many teams there are in a game. This is different from the
 * cursors number, there can be more cursors than teams, because a team
 * can have several cursors.
 *
 * Return value: the number of teams.
 */
int
lw6ker_game_state_get_nb_teams (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return _lw6ker_game_state_get_nb_teams (sys_context, (const _lw6ker_game_state_t *) game_state);
}

void
_lw6ker_game_state_do_spread (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, u_int32_t team_mask)
{
  if (!game_state->over)
    {
      _lw6ker_map_state_spread_gradient (sys_context,
					 &(game_state->map_state),
					 &(game_state->game_struct->rules), game_state->game_struct->rules.spreads_per_round, team_mask);
    }
}

/**
 * lw6ker_game_state_do_spread
 *
 * @sys_context: global system context
 * @game_state: the game_state to act upon
 * @team_mask: a binary mask of which gradients (teams) must be spreaded
 *
 * Spreads the gradient, that is, calculates the potential of each point
 * on the map, ie the distance to the closest cursor. The binary mask
 * allows gradient to be spread for only some teams, this is usefull
 * in a multithreaded context, as gradients can be calculated separately.
 *
 * Return value: none
 */
void
lw6ker_game_state_do_spread (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, u_int32_t team_mask)
{
  _lw6ker_game_state_do_spread (sys_context, (_lw6ker_game_state_t *) game_state, team_mask);
}

void
_lw6ker_game_state_do_move (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state, u_int32_t team_mask)
{
  int rounds;

  rounds = _lw6ker_game_state_get_rounds (sys_context, game_state);

  if (!game_state->over)
    {
      _lw6ker_map_state_charge (sys_context, &(game_state->map_state), &(game_state->game_struct->rules), rounds);
      _lw6ker_map_state_process_fire (sys_context, &(game_state->map_state), &(game_state->game_struct->rules), rounds);
      _lw6ker_map_state_move_fighters (sys_context, &(game_state->map_state),
				       rounds,
				       lw6sys_checksum_int32
				       (sys_context, rounds) %
				       LW6KER_NB_PARITIES, &(game_state->game_struct->rules), game_state->game_struct->rules.moves_per_round, team_mask);
    }
}

/**
 * lw6ker_game_state_do_move
 *
 * @sys_context: global system context
 * @game_state: the game_state to act upon
 * @team_mask: a binary mask of which teams must be moved
 *
 * Moves the fighters, note that you must calculate the gradient from
 * time to time else they go to the wrong places. The @team_mask allows
 * the moving of only some given teams, but moving (for instance) even
 * teams then odd teams isn't the same as moving odd teams then even teams.
 * Whereas as far as gradient calculation is concerned, this could have been
 * true, you could have multithreaded that.
 *
 * Return value: none.
 */
void
lw6ker_game_state_do_move (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, u_int32_t team_mask)
{
  _lw6ker_game_state_do_move (sys_context, (_lw6ker_game_state_t *) game_state, team_mask);
}

void
_lw6ker_game_state_finish_round (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state)
{
  int total_rounds;
  int rounds;
  int nb_teams;
  int team_color;
  int32_t nb_cursors = 0;
  int32_t nb_fighters = 0;
  int32_t nb_fighters_to_delete = 0;
  int i;

  total_rounds = _lw6ker_game_state_get_total_rounds (sys_context, game_state);
  rounds = _lw6ker_game_state_get_rounds (sys_context, game_state);
  nb_teams = _lw6ker_game_state_get_nb_teams (sys_context, game_state);

  game_state->max_reached_teams = lw6sys_imax (game_state->max_reached_teams, nb_teams);

  for (team_color = 0; team_color < LW6MAP_MAX_NB_TEAMS; ++team_color)
    {
      _lw6ker_game_state_get_team_info (sys_context, game_state, team_color, &nb_cursors, &nb_fighters);
      if (nb_cursors == 0)
	{
	  if (game_state->map_state.teams[team_color].active)
	    {
	      /*
	       * This should be a rare case, how come there are no cursors for a team?
	       * Still, with an alliance system, it could theorically happen.
	       */
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("no cursors for team %d, removing it"), team_color);
	      _lw6ker_game_state_remove_team_internal (sys_context, game_state, team_color);
	    }
	}
      else
	{
	  if (nb_fighters == 0)
	    {
	      if (game_state->game_struct->rules.respawn_team)
		{
		  if (game_state->map_state.teams[team_color].respawn_round == 0)
		    {
		      /*
		       * We unset all weapons once someone looses to avoid
		       * artificial killing sprees on unfair settings.
		       */
		      _lw6ker_weapon_unset_all (sys_context, &(game_state->map_state));

		      _lw6ker_map_state_frag (sys_context,
					      &(game_state->map_state),
					      team_color,
					      game_state->game_struct->rules.frags_mode,
					      game_state->game_struct->rules.frags_to_distribute, game_state->game_struct->rules.frags_fade_out);

		      game_state->map_state.teams[team_color].respawn_round =
			rounds + (game_state->game_struct->rules.respawn_delay * game_state->game_struct->rules.rounds_per_sec);
		    }
		  if (game_state->map_state.teams[team_color].respawn_round <= rounds)
		    {
		      _lw6ker_game_state_remove_team_internal (sys_context, game_state, team_color);
		      game_state->map_state.teams[team_color].respawn_round = 0;
		      /*
		       * We don't delete more than half of the fighters but still,
		       * we do need to delete some.
		       */
		      nb_fighters_to_delete =
			lw6sys_imin (lw6ker_per100_2
				     (game_state->map_state.map_struct->room_for_armies,
				      game_state->game_struct->rules.single_army_size),
				     _lw6ker_game_state_get_nb_active_fighters (sys_context, game_state) / 2);
		      _lw6ker_map_state_remove_fighters (sys_context, &(game_state->map_state), nb_fighters_to_delete);
		      /*
		       * Cancelling then adding will have the effect to repopulate the team
		       */
		      _lw6ker_game_state_add_team_internal (sys_context, game_state, team_color, game_state->game_struct->rules.respawn_position_mode);
		    }
		}
	      else
		{
		  /*
		   * We unset all weapons once someone looses to avoid
		   * artificial killing sprees on unfair settings.
		   */
		  _lw6ker_weapon_unset_all (sys_context, &(game_state->map_state));
		  /*
		   * OK this is hell we do it manually but high-level functions
		   * all require a node-id...
		   */
		  _lw6ker_game_state_remove_team_internal (sys_context, game_state, team_color);
		  for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
		    {
		      if (game_state->map_state.cursor_array.cursors[i].team_color == team_color)
			{
			  _lw6ker_cursor_disable (sys_context, &(game_state->map_state.cursor_array.cursors[i]));
			  game_state->map_state.cursor_array.nb_cursors--;
			}
		    }
		}
	    }
	}
    }

  // First good reason to finish : time up
  if (rounds >= total_rounds)
    {
      game_state->over = 1;
    }

  // Second good reason to finish : in classical mode, only one left
  if ((!game_state->game_struct->rules.respawn_team) && (nb_teams <= 1) && (game_state->max_reached_teams >= 2))
    {
      game_state->over = 1;
    }

  if (!game_state->over)
    {
      _lw6ker_history_set (sys_context, &(game_state->global_history), &(game_state->map_state.armies), (rounds * LW6KER_HISTORY_SIZE) / total_rounds);
      _lw6ker_history_add (sys_context, &(game_state->latest_history), &(game_state->map_state.armies));
      game_state->moves += game_state->game_struct->rules.moves_per_round;
      game_state->spreads += game_state->game_struct->rules.spreads_per_round;
      game_state->rounds++;
      /*
       * If we're alone on the battlefield, there's no fight,
       * no point in having the clock ticking, so we make the
       * game longer until some other team comes. Technically,
       * this cause the "time left" to remain constant.
       */
      if (nb_teams <= 1)
	{
	  game_state->total_rounds++;
	}

      _lw6ker_game_state_checksum_log_if_needed (sys_context, game_state);
    }
}

/**
 * lw6ker_game_state_finish_round
 *
 * @sys_context: global system context
 * @game_state: the game_state to act upon
 *
 * Finishes a round, that is, vaccums various stuff, checks if some team
 * has lost, and so on. This is complementary to the spread and move
 * steps, it should be called at each round.
 *
 * Return value: none.
 */
void
lw6ker_game_state_finish_round (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state)
{
  _lw6ker_game_state_finish_round (sys_context, (_lw6ker_game_state_t *) game_state);
}

void
_lw6ker_game_state_do_round (lw6sys_context_t * sys_context, _lw6ker_game_state_t * game_state)
{
  /*
   * No joke about the implementation, here one really must
   * call do_spread do_move and finish_round and do nothing
   * else. This is because code elsewhere (especially in pil)
   * might assume that those 3 functions are equivalent to
   * do_round when they fiddle with threads and team_mask.
   */
  _lw6ker_game_state_do_spread (sys_context, game_state, LW6KER_TEAM_MASK_ALL);
  _lw6ker_game_state_do_move (sys_context, game_state, LW6KER_TEAM_MASK_ALL);
  _lw6ker_game_state_finish_round (sys_context, game_state);
}

/**
 * lw6ker_game_state_do_round
 *
 * @sys_context: global system context
 * @game_state: the game_state to act upon
 *
 * This is a fundamental function, it's called at each round,
 * it fires all the complex calculations in the game, the
 * real core algorithm. Every time this function is called,
 * the round is "over" and the game state is ready for the
 * next... round. It's equivalent to calling the spread, move
 * and finish functions.
 *
 * Return value: none.
 */
void
lw6ker_game_state_do_round (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state)
{
  _lw6ker_game_state_do_round (sys_context, (_lw6ker_game_state_t *) game_state);
}

u_int32_t
_lw6ker_game_state_get_moves (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  return (game_state->moves);
}

/**
 * lw6ker_game_state_get_moves
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 *
 * Returns the number of moves done on this game.
 *
 * Return value: number of moves.
 */
u_int32_t
lw6ker_game_state_get_moves (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return _lw6ker_game_state_get_moves (sys_context, (const _lw6ker_game_state_t *) game_state);
}

u_int32_t
_lw6ker_game_state_get_spreads (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  return (game_state->spreads);
}

/**
 * lw6ker_game_state_get_spreads
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 *
 * Returns the number of spreads done on this game.
 *
 * Return value: number of spreads.
 */
u_int32_t
lw6ker_game_state_get_spreads (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return _lw6ker_game_state_get_spreads (sys_context, (const _lw6ker_game_state_t *) game_state);
}

u_int32_t
_lw6ker_game_state_get_rounds (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  return (game_state->rounds);
}

/**
 * lw6ker_game_state_get_rounds
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 *
 * Returns the number of rounds done on this game.
 *
 * Return value: number of rounds.
 */
u_int32_t
lw6ker_game_state_get_rounds (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return _lw6ker_game_state_get_rounds (sys_context, (const _lw6ker_game_state_t *) game_state);
}

u_int32_t
_lw6ker_game_state_get_total_rounds (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  return (game_state->total_rounds);
}

/**
 * lw6ker_game_state_get_total_rounds
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 *
 * Returns the number of playable rounds in the game, that is the number
 * of rounds to be played if game goes up to the time limit. This is
 * a fixed number, if game slows down then time is stretched, but the
 * the exact maximum number of rounds is known at game start, and it is
 * the number returned by this function.
 *
 * Return value: number of rounds in the game
 */
u_int32_t
lw6ker_game_state_get_total_rounds (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return _lw6ker_game_state_get_total_rounds (sys_context, (const _lw6ker_game_state_t *) game_state);
}

int
_lw6ker_game_state_is_over (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  return game_state->over ? 1 : 0;
}

/**
 * lw6ker_game_state_is_over
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 *
 * Tells wether the game is over or not. The answer depends on time limit,
 * game rules, and of course what happened on the battlefield.
 *
 * Return value: 1 if over, 0 if not.
 */
int
lw6ker_game_state_is_over (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return _lw6ker_game_state_is_over (sys_context, (const _lw6ker_game_state_t *) game_state);
}

int
_lw6ker_game_state_did_cursor_win (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, u_int16_t cursor_id)
{
  int ret = 0;
  lw6ker_score_array_t score_array;
  int winner_team_color = LW6MAP_TEAM_COLOR_INVALID;
  lw6ker_cursor_t cursor;

  if (_lw6ker_game_state_is_over (sys_context, game_state))
    {
      if (_lw6ker_game_state_get_cursor (sys_context, game_state, &cursor, cursor_id))
	{
	  if (_lw6ker_score_array_update (sys_context, &score_array, game_state))
	    {
	      winner_team_color = score_array.scores[0].team_color;
	      if (winner_team_color == cursor.team_color)
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("cursor %x with color %d won"), (int) cursor_id, winner_team_color);
		  ret = 1;
		}
	    }
	}
    }

  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("cursor %x didn't win"), (int) cursor_id);
    }

  return ret;
}

/**
 * lw6ker_game_state_did_cursor_win
 *
 * @sys_context: global system context
 * @game_state: game_state to query
 * @cursor_id: the cursor to test
 *
 * Tells wether a cursor was the winner after a game is over.
 *
 * Return value: 1 if cursor is in winning team, 0 if not.
 */
int
lw6ker_game_state_did_cursor_win (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, u_int16_t cursor_id)
{
  return _lw6ker_game_state_did_cursor_win (sys_context, (_lw6ker_game_state_t *) game_state, cursor_id);
}

int
_lw6ker_game_state_get_winner (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, int excluded_team)
{
  int nb_active_fighters_max;
  int nb_active_fighters;
  int nb_teams;
  int i;
  int ret = LW6MAP_TEAM_COLOR_INVALID;

  nb_active_fighters_max = 0;
  nb_teams = _lw6ker_game_state_get_nb_teams (sys_context, game_state);
  for (i = 0; i < nb_teams; ++i)
    {
      nb_active_fighters = 0;
      _lw6ker_game_state_get_team_info (sys_context, game_state, i, NULL, &nb_active_fighters);
      if (nb_active_fighters > nb_active_fighters_max && nb_active_fighters > 0 && i != excluded_team)
	{
	  ret = i;
	  nb_active_fighters_max = nb_active_fighters;
	}
    }

  return ret;
}

/**
 * lw6ker_game_state_get_winner
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 * @excluded_team: a team to exclude
 *
 * Returns the winner, if you set excluded_team to something else
 * than a valid team number (for instance -1, but 0 is a valid team)
 * then this team will be excluded from search. This is usefull if
 * you want to find out who's the best positionned player while
 * excluding yourself, for instance if you're a bot.
 *
 * Return value: the winner team number, note that it can be
 * invalid (-1) if there's no winner (for example, there are no
 * teams on the map).
 */
int
lw6ker_game_state_get_winner (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int excluded_team)
{
  return _lw6ker_game_state_get_winner (sys_context, (const _lw6ker_game_state_t *) game_state, excluded_team);
}

int
_lw6ker_game_state_get_looser (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, int excluded_team)
{
  int nb_active_fighters_min;
  int nb_active_fighters;
  int nb_teams;
  int i;
  int ret = LW6MAP_TEAM_COLOR_INVALID;

  nb_active_fighters_min = _lw6ker_game_state_get_nb_active_fighters (sys_context, game_state);
  nb_teams = _lw6ker_game_state_get_nb_teams (sys_context, game_state);
  for (i = 0; i < nb_teams; ++i)
    {
      nb_active_fighters = 0;
      _lw6ker_game_state_get_team_info (sys_context, game_state, i, NULL, &nb_active_fighters);
      if (nb_active_fighters < nb_active_fighters_min && nb_active_fighters > 0 && i != excluded_team)
	{
	  ret = i;
	  nb_active_fighters_min = nb_active_fighters;
	}
    }

  return ret;
}

/**
 * lw6ker_game_state_get_looser
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 * @excluded_team: a team to exclude
 *
 * Returns the looser, if you set excluded_team to something else
 * than a valid team number (for instance -1, but 0 is a valid team)
 * then this team will be excluded from search. This is usefull if
 * you want to find out who's the worst positionned player while
 * excluding yourself, for instance if you're a bot.
 *
 * Return value: the looser team number, note that it can be
 * invalid (-1) if there's no looser (for example, there are no
 * teams on the map).
 */
int
lw6ker_game_state_get_looser (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int excluded_team)
{
  return _lw6ker_game_state_get_looser (sys_context, (const _lw6ker_game_state_t *) game_state, excluded_team);
}

int32_t
_lw6ker_game_state_get_nb_active_fighters (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  return (game_state->map_state.armies.active_fighters);
}

/**
 * lw6ker_game_state_get_nb_active_fighters
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 *
 * Gets the number of active fighters, this is relatively constant within the
 * game, it does not change when someone looses, but it can vary when a new
 * team arrives or disappears.
 *
 * Return value: number of fighters.
 */
int32_t
lw6ker_game_state_get_nb_active_fighters (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return _lw6ker_game_state_get_nb_active_fighters (sys_context, (const _lw6ker_game_state_t *) game_state);
}

int32_t
_lw6ker_game_state_get_time_elapsed (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  int32_t ret = 0;
  u_int32_t rounds = 0;

  rounds = _lw6ker_game_state_get_rounds (sys_context, game_state);
  ret = rounds / (u_int32_t) game_state->game_struct->rules.rounds_per_sec;

  return ret;
}

/**
 * lw6ker_game_state_get_time_elapsed
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 *
 * Returns the time elapsed, this is not the real time you'd time
 * with an atomic clock, rather the time that would have elapsed if
 * game had been run at its nominal speed. There can be a difference
 * if your computer is too slow, among other things.
 *
 * Return value: time elapsed, in seconds.
 */
int32_t
lw6ker_game_state_get_time_elapsed (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return _lw6ker_game_state_get_time_elapsed (sys_context, (const _lw6ker_game_state_t *) game_state);
}

int32_t
_lw6ker_game_state_get_time_left (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  int32_t ret = 0;

  ret =
    (_lw6ker_game_state_get_total_rounds (sys_context, game_state) -
     _lw6ker_game_state_get_rounds (sys_context, game_state)) / (u_int32_t) game_state->game_struct->rules.rounds_per_sec;

  return ret;
}

/**
 * lw6ker_game_state_get_time_left
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 *
 * Returns the time left, this is not the real time you'd time
 * with an atomic clock, rather the time that would theorically
 * be left is game was to be run at its nominal speed. There can
 * be a difference if your computer is too slow, among other things.
 * You shouldn't rely on this to know wether a game is over or not,
 * there's another dedicated function for that.
 *
 * Return value: time left, in seconds.
 */
int32_t
lw6ker_game_state_get_time_left (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return _lw6ker_game_state_get_time_left (sys_context, (const _lw6ker_game_state_t *) game_state);
}

int32_t
_lw6ker_game_state_get_global_history (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, int i, int team_id)
{
  return _lw6ker_history_get (sys_context, &(game_state->global_history), i, team_id);
}

/**
 * lw6ker_game_state_get_global_history
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 * @i: the index of the history point
 * @team_id: the team to query
 *
 * Returns the number of fighters at some point in the past (the lower i,
 * the oldest). The history scrolls automatically and erases itself at
 * some point, it's of constant length. This is the global, long term
 * history, reflects the whole game and could be used for an end-game
 * score screen.
 *
 * Return value: number of fighters at that time.
 */
int32_t
lw6ker_game_state_get_global_history (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int i, int team_id)
{
  return _lw6ker_game_state_get_global_history (sys_context, (const _lw6ker_game_state_t *) game_state, i, team_id);
}

int32_t
_lw6ker_game_state_get_latest_history (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state, int i, int team_id)
{
  return _lw6ker_history_get (sys_context, &(game_state->latest_history), i, team_id);
}

/**
 * lw6ker_game_state_get_latest_history
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 * @i: the index of the history point
 * @team_id: the team to query
 *
 * Returns the number of fighters at some point in the past (the lower i,
 * the oldest). The history scrolls automatically and erases itself at
 * some point, it's of constant length. This is the latest, short term
 * history, reflects the recent game evolutions and could be used to
 * display an in-game monitor.
 *
 * Return value: number of fighters at that time.
 */
int32_t
lw6ker_game_state_get_latest_history (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int i, int team_id)
{
  return _lw6ker_game_state_get_latest_history (sys_context, (const _lw6ker_game_state_t *) game_state, i, team_id);
}

int32_t
_lw6ker_game_state_get_global_history_max (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  return _lw6ker_history_get_max (sys_context, &(game_state->global_history));
}

/**
 * lw6ker_game_state_get_global_history_max
 *
 * @sys_context: global system context
 * @game_state: game_state to query
 *
 * Returns the maximum value, that is, the maximum number of fighters, all teams
 * combined, for this history. This can be used to scale charts.
 * This function for the global long term history.
 *
 * Return value: max number of fighters.
 */
int32_t
lw6ker_game_state_get_global_history_max (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return _lw6ker_game_state_get_global_history_max (sys_context, (const _lw6ker_game_state_t *) game_state);
}

int32_t
_lw6ker_game_state_get_latest_history_max (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  return _lw6ker_history_get_max (sys_context, &(game_state->latest_history));
}

/**
 * lw6ker_game_state_get_latest_history_max
 *
 * @sys_context: global system context
 * @game_state: game_state to query
 *
 * Returns the maximum value, that is, the maximum number of fighters, all teams
 * combined, for this history. This can be used to scale charts.
 * This function for the latest short term history.
 *
 * Return value: max number of fighters.
 */
int32_t
lw6ker_game_state_get_latest_history_max (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return _lw6ker_game_state_get_latest_history_max (sys_context, (const _lw6ker_game_state_t *) game_state);
}

/**
 * lw6ker_game_state_get_fighter_id
 *
 * @sys_context: global system context
 * @game_state: game_state to query
 * @x: x position
 * @y: y position
 * @z: z position
 *
 * Gets the id of a fighter in a given position. Previous versions of the
 * game used to have this declared inline static for speed, but the price
 * to pay in terms of maintainability was too high: too much stuff from the
 * ker module had to be kept public. This functions is very likely to be
 * called often when one wants to know what's happening on the battlefield,
 * to draw it, for instance. If there's no fighter, the id is negative,
 * any id equal or greater than 0 (returned by this function) is valid.
 *
 * Return value: the id of the fighter at that position.
 */
int32_t
lw6ker_game_state_get_fighter_id (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int32_t x, int32_t y, int32_t z)
{
  return _lw6ker_map_state_get_fighter_id (&(((const _lw6ker_game_state_t *) game_state)->map_state), x, y, z);
}

/**
 * lw6ker_game_state_get_fighter_rw_by_id
 *
 * @sys_context: global system context
 * @game_state: game_state to query
 * @fighter_id: the id of the fighter
 *
 * Gets a fighter by its id. Internally, all fighters are stored in an array
 * so it could be "safe" to get fighter with id 0 then walk the array.
 * Previous versions of the game used to have this public (the array),
 * it has been hidden since. Pointer is read/write.
 * Pointer is read/write.
 *
 * Return value: pointer to the fighter with the given id.
 */
lw6ker_fighter_t *
lw6ker_game_state_get_fighter_rw_by_id (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, int32_t fighter_id)
{
  return (&(((_lw6ker_game_state_t *) game_state)->map_state.armies.fighters[fighter_id]));
}

/**
 * lw6ker_game_state_get_fighter_rw_safe
 *
 * @sys_context: global system context
 * @game_state: game_state to query
 * @x: x position
 * @y: y position
 * @z: z position
 *
 * Gets a fighter by its position. This function will check for boundaries, if there's
 * no fighter in this place, it will return NULL, but nothing worse can happen. More
 * precisely, if the place is in a wall, it won't bug, unlike the non-bullet-proof
 * equivalent of this function.
 * Pointer is read/write.
 *
 * Return value: pointer to the fighter at this position, or NULL if none.
 */
lw6ker_fighter_t *
lw6ker_game_state_get_fighter_rw_safe (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, int32_t x, int32_t y, int32_t z)
{
  lw6ker_fighter_t *ret = NULL;
  int fighter_id;

  if (x >= 0
      && x < ((_lw6ker_game_state_t *) game_state)->map_state.shape.w
      && y >= 0 && y < ((_lw6ker_game_state_t *) game_state)->map_state.shape.h && z >= 0 && z < ((_lw6ker_game_state_t *) game_state)->map_state.shape.d)
    {
      fighter_id = lw6ker_game_state_get_fighter_id (sys_context, game_state, x, y, z);
      if (fighter_id >= 0)
	{
	  ret = lw6ker_game_state_get_fighter_rw_by_id (sys_context, game_state, fighter_id);
	}
    }

  return ret;
};

/**
 * lw6ker_game_state_get_fighter_rw_unsafe
 *
 * @sys_context: global system context
 * @game_state: game_state to query
 * @x: x position
 * @y: y position
 * @z: z position
 *
 * Gets a fighter by its position. This function will not check for boundaries, if there's
 * no fighter in this place, not only will it probably not return a valid value, but it will also
 * even segfault before that, trying to access non-existing structures in menory. So only
 * call this if you're sure there's a fighter here.
 * Pointer is read/write.
 *
 * Return value: pointer to the fighter at this position, or NULL if none.
 */
lw6ker_fighter_t *
lw6ker_game_state_get_fighter_rw_unsafe (lw6sys_context_t * sys_context, lw6ker_game_state_t * game_state, int32_t x, int32_t y, int32_t z)
{
  return _lw6ker_map_state_get_fighter_rw_unsafe (&(((_lw6ker_game_state_t *) game_state)->map_state), x, y, z);
}

/**
 * lw6ker_game_state_get_fighter_ro_by_id
 *
 * @sys_context: global system context
 * @game_state: game_state to query
 * @fighter_id: the id of the fighter
 *
 * Gets a fighter by its id. Internally, all fighters are stored in an array
 * so it could be "safe" to get fighter with id 0 then walk the array.
 * Previous versions of the game used to have this public (the array),
 * it has been hidden since. Pointer is read-only.
 * Pointer is read-only.
 *
 * Return value: pointer to the fighter with the given id.
 */
const lw6ker_fighter_t *
lw6ker_game_state_get_fighter_ro_by_id (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int32_t fighter_id)
{
  return (&(((const _lw6ker_game_state_t *) game_state)->map_state.armies.fighters[fighter_id]));
}

/**
 * lw6ker_game_state_get_fighter_ro_safe
 *
 * @sys_context: global system context
 * @game_state: game_state to query
 * @x: x position
 * @y: y position
 * @z: z position
 *
 * Gets a fighter by its position. This function will check for boundaries, if there's
 * no fighter in this place, it will return NULL, but nothing worse can happen. More
 * precisely, if the place is in a wall, it won't bug, unlike the non-bullet-proof
 * equivalent of this function.
 * Pointer is read-only.
 *
 * Return value: pointer to the fighter at this position, or NULL if none.
 */
const lw6ker_fighter_t *
lw6ker_game_state_get_fighter_ro_safe (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int32_t x, int32_t y, int32_t z)
{
  const lw6ker_fighter_t *ret = NULL;
  int fighter_id;

  if (x >= 0
      && x < ((_lw6ker_game_state_t *) game_state)->map_state.shape.w
      && y >= 0 && y < ((_lw6ker_game_state_t *) game_state)->map_state.shape.h && z >= 0 && z < ((_lw6ker_game_state_t *) game_state)->map_state.shape.d)
    {
      fighter_id = lw6ker_game_state_get_fighter_id (sys_context, game_state, x, y, z);
      if (fighter_id >= 0)
	{
	  ret = lw6ker_game_state_get_fighter_ro_by_id (sys_context, game_state, fighter_id);
	}
    }

  return ret;
};

/**
 * lw6ker_game_state_get_fighter_ro_unsafe
 *
 * @sys_context: global system context
 * @game_state: game_state to query
 * @x: x position
 * @y: y position
 * @z: z position
 *
 * Gets a fighter by its position. This function will not check for boundaries, if there's
 * no fighter in this place, not only will it probably not return a valid value, but it will also
 * even segfault before that, trying to access non-existing structures in menory. So only
 * call this if you're sure there's a fighter here.
 * Pointer is read-only.
 *
 * Return value: pointer to the fighter at this position, or NULL if none.
 */
const lw6ker_fighter_t *
lw6ker_game_state_get_fighter_ro_unsafe (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int32_t x, int32_t y, int32_t z)
{
  return _lw6ker_map_state_get_fighter_ro_unsafe (&(((const _lw6ker_game_state_t *) game_state)->map_state), x, y, z);
}

/**
 * lw6ker_game_state_get_zone_potential
 *
 * @sys_context: global system context
 * @game_state: the game_state to query
 * @zone_i: the zone index
 * @team_id: the team id (color)
 *
 * Gets the potential of a zone. In practice this is not needed to
 * make the game function, you need not call this to know how to move
 * fighters, however the information can be interesting for debugging.
 *
 * Return value: the potential
 */
int
lw6ker_game_state_get_zone_potential (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int i, int team_id)
{
  return ((_lw6ker_game_state_t *) game_state)->map_state.teams[team_id].gradient[i].potential;
}

/**
 * lw6ker_game_state_get_charge_per1000
 *
 * @sys_context: global system context
 * @game_state: game_state to query
 * @team_color: the team color to query
 *
 * Returns the charge ratio for a given team/color. A value of 100
 * means fire is enabled, more than 1000 means super-charge, under 100
 * means you have to wait.
 *
 * Return value: integer value.
 */
int
lw6ker_game_state_get_charge_per1000 (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int team_color)
{
  return _lw6ker_team_get_charge_per1000 (sys_context, &(((const _lw6ker_game_state_t *) game_state)->map_state.teams[team_color]));
}

/**
 * lw6ker_game_state_get_weapon_per1000_left
 *
 * @sys_context: global system context
 * @game_state: game_state to query
 * @team_color: the team color to query
 *
 * Returns how much of the weapon is yet to be consumed for a given
 * team. More than 1000 means extra time, 1000 is standard time to
 * be elapsed, 0 means it's over.
 *
 * Return value: integer value.
 */
int
lw6ker_game_state_get_weapon_per1000_left (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state, int team_color)
{
  return
    _lw6ker_map_state_get_weapon_per1000_left (sys_context, &
					       (((const _lw6ker_game_state_t
						  *) game_state)->map_state), lw6ker_game_state_get_rounds (sys_context, game_state), team_color);
}

/**
 * lw6ker_game_state_get_weapon_per1000_left
 *
 * @sys_context: global system context
 * @game_state: game_state to query
 * @team_color: the team color corresponding to last weapon (out param)
 * @weapon_id: the corresponding weapon_id (out param)
 * @per1000_left: how much of the weapon is yet to be spent (out param)
 *
 * Returns informations about the latest weapon, this is typically
 * for drawing purposes, just query this and you know if you need
 * to paint everything in red, green, whatever, as far as the default
 * backend is concerned. In case there's no weapon, well, parameters
 * are untouched. Pointers can be passed as NULL.
 *
 * Return value: 1 if found, 0 if not.
 */
int
lw6ker_game_state_get_latest_weapon (lw6sys_context_t * sys_context,
				     const lw6ker_game_state_t * game_state, int *team_color, int *weapon_id, int *per1000_left)
{
  return
    _lw6ker_weapon_get_latest_weapon (sys_context, &
				      (((const _lw6ker_game_state_t *)
					game_state)->map_state), lw6ker_game_state_get_rounds (sys_context, game_state), team_color, weapon_id, per1000_left);
}

int
_lw6ker_game_state_get_nb_colors (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  return _lw6ker_map_state_get_nb_teams (sys_context, &(game_state->map_state));
}

/**
 * lw6ker_game_state_get_nb_colors
 *
 * @sys_context: global system context
 * @game_state: game state to query
 *
 * Gives the max number of colors (AKA teams) that are present
 * in the game. This is just a simple utility/wrapper function
 * which is meant to be exported to Guile scripts.
 *
 * Return value: number of colors
 */
int
lw6ker_game_state_get_nb_colors (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return _lw6ker_game_state_get_nb_colors (sys_context, (const _lw6ker_game_state_t *) game_state);
}

int
_lw6ker_game_state_get_nb_cursors (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  return game_state->map_state.cursor_array.nb_cursors;
}

/**
 * lw6ker_game_state_get_nb_cursors
 *
 * @sys_context: global system context
 * @game_state: game state to query
 *
 * Gives the max number of cursors that are present
 * in the game. This is just a simple utility/wrapper function
 * which is meant to be exported to Guile scripts.
 *
 * Return value: number of cursors
 */
int
lw6ker_game_state_get_nb_cursors (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return _lw6ker_game_state_get_nb_cursors (sys_context, (const _lw6ker_game_state_t *) game_state);
}

int
_lw6ker_game_state_get_nb_nodes (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  return game_state->node_array.nb_nodes;
}

/**
 * lw6ker_game_state_get_nb_nodes
 *
 * @sys_context: global system context
 * @game_state: game state to query
 *
 * Gives the max number of nodes that are present
 * in the game. This is just a simple utility/wrapper function
 * which is meant to be exported to Guile scripts.
 *
 * Return value: number of nodes
 */
int
lw6ker_game_state_get_nb_nodes (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  return _lw6ker_game_state_get_nb_nodes (sys_context, (const _lw6ker_game_state_t *) game_state);
}
