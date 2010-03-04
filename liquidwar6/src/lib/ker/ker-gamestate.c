/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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
static u_int32_t seq_id = 0;

lw6ker_game_state_t *
lw6ker_game_state_new (lw6ker_game_struct_t * game_struct,
		       lw6sys_progress_t * progress)
{
  lw6ker_game_state_t *ret = NULL;

  ret = (lw6ker_game_state_t *) LW6SYS_CALLOC (sizeof (lw6ker_game_state_t));
  if (ret)
    {
      ret->id = 0;
      while (!ret->id)
	{
	  ret->id = ++seq_id;
	}
      ret->game_struct = game_struct;

      _lw6ker_map_state_init (&(ret->map_state),
			      &(ret->game_struct->map_struct),
			      &(ret->game_struct->rules), progress);
      _lw6ker_server_array_init (&(ret->server_array));
    }

  return ret;
}

void
lw6ker_game_state_free (lw6ker_game_state_t * game_state)
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
  _lw6ker_map_state_clear (&(game_state->map_state));

  LW6SYS_FREE (game_state);
}

void
lw6ker_game_state_point_to (lw6ker_game_state_t * game_state,
			    lw6ker_game_struct_t * game_struct)
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

int
lw6ker_game_state_memory_footprint (lw6ker_game_state_t * game_state)
{
  int ret = 0;

  // todo

  return ret;
}

char *
lw6ker_game_state_repr (lw6ker_game_state_t * game_state)
{
  char *ret = NULL;

  if (game_state)
    {
      /*
       * do not use references to game_struct or level here,
       * function is called when freeing the object
       */
      ret =
	lw6sys_new_sprintf ("%u (%dx%dx%d, round %d, %d fighters)",
			    game_state->id,
			    game_state->map_state.shape.w,
			    game_state->map_state.shape.h,
			    game_state->map_state.shape.d,
			    lw6ker_game_state_get_rounds (game_state),
			    lw6ker_game_state_get_nb_active_fighters
			    (game_state));
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, "game_state",
		  _("can't generate string id for NULL game_state"));
    }

  return ret;
}

int
lw6ker_game_state_can_sync (lw6ker_game_state_t * dst,
			    lw6ker_game_state_t * src)
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
	  ret =
	    _lw6ker_map_struct_lazy_compare (&(dst->game_struct->map_struct),
					     &(src->game_struct->map_struct));
	}
    }

  return ret;
}

/*
 * Fundamental function, used to carbon copy a game state to another,
 * this is intensively used to keep too tracks of the game state, one
 * most-up-to-date but probably wrong, the one we use to display on the
 * screen, and one slightly outdated (or very outdated if network is
 * slow) but that we're sure of, something 100% bullet proof we can
 * rely on.
 */
int
lw6ker_game_state_sync (lw6ker_game_state_t * dst, lw6ker_game_state_t * src)
{
  int ret = 0;

  /*
   * Sanity check, this function is designed to copy game states
   * which correspond to the same game struct. Any other use is
   * useless in LW6 context.
   */
  if (lw6ker_game_state_can_sync (dst, src))
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
	  ret = _lw6ker_map_state_sync (&dst->map_state, &src->map_state);
	  memcpy (&(dst->server_array), &(src->server_array),
		  sizeof (lw6ker_server_array_t));
	  memcpy (&(dst->global_history), &(src->global_history),
		  sizeof (lw6ker_history_t));
	  memcpy (&(dst->latest_history), &(src->latest_history),
		  sizeof (lw6ker_history_t));
	  dst->moves = src->moves;
	  dst->spreads = src->spreads;
	  dst->rounds = src->rounds;
	  dst->max_reached_teams = src->max_reached_teams;
	  dst->over = src->over;
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _
		  ("game_state_copy only works if dst and src point to the same game_struct"));
    }

  return ret;
}

lw6ker_game_state_t *
lw6ker_game_state_dup (lw6ker_game_state_t * game_state,
		       lw6sys_progress_t * progress)
{
  lw6ker_game_state_t *ret;

  ret = lw6ker_game_state_new (game_state->game_struct, progress);
  if (ret)
    {
      if (lw6ker_game_state_sync (ret, game_state))
	{
	  // ok
	}
      else
	{
	  lw6ker_game_state_free (ret);
	  ret = NULL;
	}
    }

  if (ret)
    {
      if (lw6ker_game_state_checksum (ret) ==
	  lw6ker_game_state_checksum (game_state))
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("game_state dup %d->%d"),
		      game_state->id, ret->id);
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("game_state checkum mismatch after dup"));
	}
    }

  return ret;
}

void
_lw6ker_game_state_update_checksum (lw6ker_game_state_t *
				    game_state, u_int32_t * checksum)
{
  _lw6ker_game_struct_update_checksum (game_state->game_struct, checksum);
  _lw6ker_map_state_update_checksum (&(game_state->map_state), checksum);
  _lw6ker_server_array_update_checksum (&(game_state->server_array),
					checksum);
  _lw6ker_history_update_checksum (&(game_state->global_history), checksum);
  _lw6ker_history_update_checksum (&(game_state->latest_history), checksum);
  lw6sys_checksum_update_int32 (checksum, game_state->moves);
  lw6sys_checksum_update_int32 (checksum, game_state->spreads);
  lw6sys_checksum_update_int32 (checksum, game_state->rounds);
  lw6sys_checksum_update_int32 (checksum, game_state->max_reached_teams);
  lw6sys_checksum_update_int32 (checksum, game_state->over);
}

u_int32_t
lw6ker_game_state_checksum (lw6ker_game_state_t * game_state)
{
  u_int32_t ret = 0;

  _lw6ker_game_state_update_checksum (game_state, &ret);

  return ret;
}

int
lw6ker_game_state_register_server (lw6ker_game_state_t * game_state,
				   u_int64_t server_id)
{
  int ret = 0;

  if (!lw6ker_game_state_server_exists (game_state, server_id))
    {
      ret =
	lw6ker_server_array_enable (&(game_state->server_array), server_id);
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _("server %" LW6SYS_PRINTF_LL "x already exists"),
		  server_id);
    }

  return ret;
}

int
lw6ker_game_state_unregister_server (lw6ker_game_state_t * game_state,
				     u_int64_t server_id)
{
  int ret = 0;
  int i;
  lw6ker_cursor_t *cursor;

  if (lw6ker_game_state_server_exists (game_state, server_id))
    {
      for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
	{
	  cursor = &(game_state->map_state.cursor_array.cursors[i]);
	  if (cursor->enabled && cursor->server_id == server_id)
	    {
	      lw6ker_game_state_remove_cursor (game_state, server_id,
					       cursor->cursor_id);
	    }
	}
      ret =
	lw6ker_server_array_disable (&(game_state->server_array), server_id);
    }

  return ret;
}

int
lw6ker_game_state_server_exists (lw6ker_game_state_t * game_state,
				 u_int64_t server_id)
{
  int ret = 0;

  ret =
    (lw6ker_server_array_get (&(game_state->server_array), server_id) !=
     NULL);

  return ret;
}

int
lw6ker_game_state_get_server_info (lw6ker_game_state_t *
				   game_state, u_int16_t server_id,
				   u_int32_t * last_command_round)
{
  int ret = 0;
  lw6ker_server_t *server = NULL;

  server = lw6ker_server_array_get (&(game_state->server_array), server_id);
  if (server)
    {
      if (last_command_round)
	{
	  (*last_command_round) = server->last_command_round;
	}
      ret = 1;
    }

  return ret;

}

static int
check_server_id (lw6ker_game_state_t * game_state, u_int64_t server_id)
{
  int ret = 0;

  if (lw6ker_game_state_server_exists (game_state, server_id))
    {
      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _("server %" LW6SYS_PRINTF_LL "x does not exist"),
		  server_id);
    }

  return ret;
}

int
lw6ker_game_state_add_cursor (lw6ker_game_state_t * game_state,
			      u_int64_t server_id,
			      u_int16_t cursor_id, int team_color)
{
  int ret = 0;
  lw6map_rules_t *rules = NULL;
  int team_exists = 0;
  int real_team_color = LW6MAP_TEAM_COLOR_INVALID;
  int32_t x = 0;
  int32_t y = 0;

  if (check_server_id (game_state, server_id))
    {
      rules = &(game_state->game_struct->rules);
      if (!lw6ker_game_state_cursor_exists (game_state, cursor_id))
	{
	  team_exists =
	    lw6ker_game_state_team_exists (game_state, team_color);
	  real_team_color = team_color;
	  if (team_exists)
	    {
	      if ((rules->color_conflict_mode == 1
		   &&
		   !_lw6ker_cursor_array_is_color_owned_by (&
							    (game_state->
							     map_state.
							     cursor_array),
							    server_id,
							    team_color))
		  || rules->color_conflict_mode == 2)
		{
		  real_team_color =
		    lw6ker_map_state_get_free_team_color (&
							  (game_state->
							   map_state));
		}
	    }

	  if (real_team_color >= 0)
	    {
	      if (real_team_color != team_color)
		{
		  lw6sys_log (LW6SYS_LOG_DEBUG,
			      _
			      ("color shift: real_team_color=%d team_color=%d"),
			      real_team_color, team_color);
		}
	      if (!lw6ker_game_state_team_exists
		  (game_state, real_team_color))
		{
		  lw6ker_game_state_add_team (game_state, server_id,
					      real_team_color);
		}
	      if (lw6ker_game_state_team_exists (game_state, real_team_color))
		{
		  if (_lw6ker_cursor_get_start_xy
		      (&x, &y, real_team_color, 0, 0,
		       &(game_state->map_state.shape), rules))
		    {
		      ret =
			lw6ker_cursor_array_enable (&
						    (game_state->map_state.
						     cursor_array), server_id,
						    cursor_id,
						    real_team_color, x, y);
		    }
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_DEBUG,
			  _("unable to add team %d, game is full"),
			  (int) team_color);
	    }
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_DEBUG, _("cursor %x already exists"),
		      (int) cursor_id);
	}
    }

  return ret;
}

int
lw6ker_game_state_remove_cursor (lw6ker_game_state_t * game_state,
				 u_int64_t server_id, u_int16_t cursor_id)
{
  int ret = 0;
  int32_t nb_cursors = 0;
  int team_color = LW6MAP_TEAM_COLOR_INVALID;

  if (check_server_id (game_state, server_id))
    {
      if (lw6ker_game_state_cursor_exists (game_state, cursor_id))
	{
	  lw6ker_game_state_get_cursor_info (game_state, cursor_id, NULL,
					     NULL, &team_color, NULL, NULL);
	  ret =
	    lw6ker_cursor_array_disable (&
					 (game_state->map_state.cursor_array),
					 server_id, cursor_id);
	  if (lw6ker_game_state_get_team_info
	      (game_state, team_color, &nb_cursors, NULL))
	    {
	      if (!nb_cursors)
		{
		  lw6ker_game_state_remove_team (game_state, server_id,
						 team_color);
		}
	    }
	}
    }

  return ret;
}

int
lw6ker_game_state_cursor_exists (lw6ker_game_state_t * game_state,
				 u_int16_t cursor_id)
{
  int ret = 0;
  lw6ker_cursor_t *cursor = NULL;
  cursor =
    lw6ker_cursor_array_get (&(game_state->map_state.cursor_array),
			     cursor_id);
  if (cursor)
    {
      ret = 1;
    }

  return ret;
}

int
lw6ker_game_state_get_cursor_info (lw6ker_game_state_t *
				   game_state,
				   u_int16_t cursor_id,
				   u_int64_t * server_id,
				   char *letter,
				   int *team_color, int32_t * x, int32_t * y)
{
  int ret = 0;
  lw6ker_cursor_t *cursor = NULL;

  cursor =
    lw6ker_cursor_array_get (&(game_state->map_state.cursor_array),
			     cursor_id);
  if (cursor)
    {
      if (server_id)
	{
	  (*server_id) = cursor->server_id;
	}
      if (letter)
	{
	  (*letter) = cursor->letter;
	}
      if (team_color)
	{
	  (*team_color) = cursor->team_color;
	}
      if (x)
	{
	  (*x) = cursor->pos.x;
	}
      if (y)
	{
	  (*y) = cursor->pos.y;
	}
      ret = 1;
    }

  return ret;
}

int
lw6ker_game_state_set_cursor (lw6ker_game_state_t * game_state,
			      u_int64_t server_id,
			      u_int16_t cursor_id, int32_t x, int32_t y)
{
  int ret = 0;

  if (check_server_id (game_state, server_id))
    {
      lw6map_coords_fix_xy (&(game_state->game_struct->rules),
			    &(game_state->map_state.shape), &x, &y);
      ret =
	lw6ker_cursor_array_update (&(game_state->map_state.cursor_array),
				    server_id, cursor_id, x, y, 0,
				    &(game_state->map_state.shape),
				    &(game_state->game_struct->rules));
    }

  return ret;
}

int
_lw6ker_game_state_add_team (lw6ker_game_state_t * game_state, int team_color,
			     int random_place)
{
  int ret = 0;
  int32_t i;
  /*
   * Some values as INT64 to force casts to long long and
   * avoid integer capacity overflow.
   */
  int64_t total_fighters;
  int64_t total_fighters_to_remove;
  lw6map_rules_t *rules = NULL;

  rules = &(game_state->game_struct->rules);
  if (!lw6ker_game_state_team_exists (game_state, team_color))
    {
      int32_t nb_fighters;
      lw6sys_xyz_t desired_center;
      int32_t x;
      int32_t y;

      _lw6ker_cursor_get_start_xy (&x, &y, team_color, random_place,
				   lw6ker_game_state_get_rounds (game_state),
				   &(game_state->map_state.shape), rules);
      desired_center.x = x;
      desired_center.y = y;
      desired_center.z = 0;

      nb_fighters =
	lw6ker_per100_2 (game_state->map_state.map_struct->room_for_armies,
			 rules->single_army_size);
      if (nb_fighters + game_state->map_state.armies.active_fighters >
	  game_state->map_state.armies.max_fighters)
	{
	  /*
	   * OK there will be too many fighters, now the rule is:
	   * a new team gets 1/n of total allowed fighters.
	   */
	  nb_fighters =
	    game_state->map_state.armies.max_fighters /
	    (_lw6ker_map_state_get_nb_teams (&(game_state->map_state)) + 1);
	}
      /*
       * Remove fighters if needed
       */
      if (nb_fighters + game_state->map_state.armies.active_fighters >
	  game_state->map_state.armies.max_fighters)
	{
	  total_fighters = game_state->map_state.armies.active_fighters;
	  total_fighters_to_remove =
	    nb_fighters - (game_state->map_state.armies.max_fighters -
			   total_fighters);
	  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	    {
	      lw6ker_map_state_remove_team_fighters (&(game_state->map_state),
						     i,
						     (total_fighters_to_remove
						      *
						      game_state->
						      map_state.armies.
						      fighters_per_team[i]) /
						     total_fighters);
	    }
	  /*
	   * Note that this value has probably changed since we last
	   * calculated it...
	   */
	  total_fighters = game_state->map_state.armies.active_fighters;
	  total_fighters_to_remove =
	    nb_fighters - (game_state->map_state.armies.max_fighters -
			   total_fighters);
	  lw6ker_map_state_remove_fighters (&(game_state->map_state),
					    total_fighters_to_remove);
	}
      /*
       * Do add people.
       */
      lw6ker_map_state_populate_team (&game_state->map_state, team_color,
				      nb_fighters, desired_center, rules);
      ret = 1;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_DEBUG,
		  _("can't add team %d, it's already active"), team_color);
    }

  return ret;
}

int
lw6ker_game_state_add_team (lw6ker_game_state_t * game_state,
			    u_int64_t server_id, int team_color)
{
  int ret = 0;

  if (check_server_id (game_state, server_id))
    {
      ret = _lw6ker_game_state_add_team (game_state, team_color, 0);
      if (ret)
	{
	  /*
	   * Set frags to 0. If we're in a network game and someone
	   * re-uses a color, we don't want these frags to count.
	   * Note that the other function _lw6ker_game_state_add_team
	   * does *not* touch the frag count.
	   */
	  game_state->map_state.armies.frags[team_color] = 0;
	}
    }

  return ret;
}

int
_lw6ker_game_state_remove_team (lw6ker_game_state_t * game_state,
				int team_color)
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

      nb_active_teams =
	_lw6ker_map_state_get_nb_teams (&(game_state->map_state));
      nb_active_fighters = game_state->map_state.armies.active_fighters;

      if (nb_active_teams <= 1)
	{
	  /*
	   * Weird situation, not sure it should happen, except when in
	   * a spectator mode -> not one single fighter left!
	   */
	  lw6ker_map_state_remove_fighters (&(game_state->map_state),
					    nb_active_fighters);
	}
      else
	{
	  new_average_fighters_per_team =
	    nb_active_fighters / (nb_active_teams - 1);
	  single_average_fighters_per_team =
	    lw6ker_per100_2 (game_state->map_state.map_struct->
			     room_for_armies,
			     game_state->game_struct->rules.single_army_size);
	  if (new_average_fighters_per_team >
	      single_average_fighters_per_team)
	    {
	      lw6ker_map_state_remove_team_fighters (&(game_state->map_state),
						     team_color,
						     nb_active_fighters -
						     single_average_fighters_per_team
						     * (nb_active_teams - 1));
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
	  total_fighters_to_distribute =
	    game_state->map_state.armies.fighters_per_team[team_color];
	  for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	    {
	      if (i != team_color)
		{
		  lw6ker_map_state_redistribute_team (&
						      (game_state->map_state),
						      i, team_color,
						      (game_state->
						       map_state.armies.
						       fighters_per_team[i] *
						       total_fighters_to_distribute)
						      / new_active_fighters,
						      &(game_state->
							game_struct->rules));
		}
	    }
	  /*
	   * We might get a few fighters left due to rounding/approximations,
	   * we get rid of them.
	   */
	  while (game_state->map_state.armies.fighters_per_team[team_color] >
		 0)
	    {
	      for (i = 0;
		   i < LW6MAP_MAX_NB_TEAMS
		   && game_state->map_state.
		   armies.fighters_per_team[team_color] > 0; ++i)
		{
		  if (game_state->map_state.teams[i].active)
		    {
		      lw6ker_map_state_redistribute_team (&
							  (game_state->
							   map_state), i,
							  team_color, 1,
							  &(game_state->
							    game_struct->
							    rules));
		    }
		}
	    }

	  lw6ker_map_state_cancel_team (&(game_state->map_state), team_color);
	}

      ret = 1;
    }

  return ret;
}

int
lw6ker_game_state_remove_team (lw6ker_game_state_t * game_state,
			       u_int64_t server_id, int team_color)
{
  int ret = 0;

  if (check_server_id (game_state, server_id))
    {
      ret = _lw6ker_game_state_remove_team (game_state, team_color);
    }

  return ret;
}

int
lw6ker_game_state_team_exists (lw6ker_game_state_t * game_state,
			       int team_color)
{
  int ret = 0;

  if (team_color >= 0 && team_color < LW6MAP_MAX_NB_TEAMS)
    {
      ret = game_state->map_state.teams[team_color].active;
    }

  return ret;
}

int
lw6ker_game_state_get_team_info (lw6ker_game_state_t *
				 game_state, int team_color,
				 int32_t * nb_cursors, int32_t * nb_fighters)
{
  int ret = 0;
  lw6ker_team_t *team;
  lw6ker_cursor_array_t *cursor_array;
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
		  if (cursor_array->cursors[i].enabled
		      && cursor_array->cursors[i].team_color == team_color)
		    {
		      (*nb_cursors)++;
		    }
		}
	    }
	  if (nb_fighters)
	    {
	      (*nb_fighters) =
		game_state->map_state.armies.fighters_per_team[team_color];
	    }
	}
      else
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
    }

  return ret;
}

int
lw6ker_game_state_get_nb_teams (lw6ker_game_state_t * game_state)
{
  int ret = 0;

  ret = _lw6ker_map_state_get_nb_teams (&(game_state->map_state));

  return ret;
}

void
lw6ker_game_state_do_spread (lw6ker_game_state_t *
			     game_state, u_int32_t team_mask)
{
  if (!game_state->over)
    {
      lw6ker_map_state_spread_gradient (&(game_state->map_state),
					&(game_state->game_struct->rules),
					game_state->game_struct->
					rules.spreads_per_round, team_mask);
    }
}

void
lw6ker_game_state_do_move (lw6ker_game_state_t *
			   game_state, u_int32_t team_mask)
{
  if (!game_state->over)
    {
      lw6ker_map_state_move_fighters (&(game_state->map_state),
				      lw6sys_checksum_int32
				      (game_state->rounds) %
				      LW6KER_NB_PARITIES,
				      &(game_state->game_struct->rules),
				      game_state->game_struct->rules.
				      moves_per_round, team_mask);
    }
}

void
lw6ker_game_state_finish_round (lw6ker_game_state_t * game_state)
{
  int total_rounds;
  int nb_teams;
  int team_color;
  int32_t nb_cursors = 0;
  int32_t nb_fighters = 0;
  int32_t nb_fighters_to_delete = 0;
  int i;

  total_rounds = lw6ker_game_state_get_total_rounds (game_state);
  nb_teams = lw6ker_game_state_get_nb_teams (game_state);

  game_state->max_reached_teams =
    lw6sys_max (game_state->max_reached_teams, nb_teams);

  for (team_color = 0; team_color < LW6MAP_MAX_NB_TEAMS; ++team_color)
    {
      lw6ker_game_state_get_team_info (game_state, team_color, &nb_cursors,
				       &nb_fighters);
      if (nb_cursors == 0)
	{
	  /*
	   * This should be a rare case, how come there are no cursors for a team?
	   * Still, with an alliance system, it could theorically happen.
	   */
	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _("no cursors for team %d, removing it"), team_color);
	  _lw6ker_game_state_remove_team (game_state, team_color);
	}
      else
	{
	  if (nb_fighters == 0)
	    {
	      _lw6ker_game_state_remove_team (game_state, team_color);
	      if (game_state->game_struct->rules.respawn_team)
		{
		  /*
		   * We don't delete more than half of the fighters but still,
		   * we do need to delete some.
		   */
		  nb_fighters_to_delete =
		    lw6sys_min (lw6ker_per100_2
				(game_state->map_state.map_struct->
				 room_for_armies,
				 game_state->game_struct->rules.
				 single_army_size),
				lw6ker_game_state_get_nb_active_fighters
				(game_state) / 2);
		  lw6ker_map_state_remove_fighters (&(game_state->map_state),
						    nb_fighters_to_delete);
		  /*
		   * Cancelling then adding will have the effect to repopulate the team
		   */
		  _lw6ker_game_state_add_team (game_state, team_color,
					       game_state->game_struct->
					       rules.respawn_random_place);
		  game_state->map_state.armies.frags[team_color]--;
		}
	      else
		{
		  /*
		   * OK this is hell we do it manually but high-level functions
		   * all require a server-id...
		   */
		  for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
		    {
		      if (game_state->map_state.cursor_array.cursors[i].
			  team_color == team_color)
			{
			  lw6ker_cursor_disable (&
						 (game_state->map_state.
						  cursor_array.cursors[i]));
			  game_state->map_state.cursor_array.nb_cursors--;
			}
		    }
		}
	    }
	}
    }

  // First good reason to finish : time up
  if (game_state->rounds >= total_rounds)
    {
      game_state->over = 1;
    }

  // Second good reason to finish : in classical mode, only one left
  if ((!game_state->game_struct->rules.respawn_team) &&
      (nb_teams <= 1) && (game_state->max_reached_teams >= 2))
    {
      game_state->over = 1;
    }

  if (!game_state->over)
    {
      _lw6ker_history_set (&(game_state->global_history),
			   &(game_state->map_state.armies),
			   (game_state->rounds * LW6KER_HISTORY_SIZE) /
			   total_rounds);
      _lw6ker_history_add (&(game_state->latest_history),
			   &(game_state->map_state.armies));
      game_state->moves += game_state->game_struct->rules.moves_per_round;
      game_state->spreads += game_state->game_struct->rules.spreads_per_round;
      game_state->rounds++;
    }
}

/*
 * This is a fundamental function, it's called at each round,
 * it fires all the complex calculations in the game, the
 * real core algorithm. Every time this function is called,
 * the round is "over" and the game state is ready for the
 * next... round.
 */
void
lw6ker_game_state_do_round (lw6ker_game_state_t * game_state)
{
  /*
   * No joke about the implementation, here one really must
   * call do_spread do_move and finish_round and do nothing
   * else. This is because code elsewhere (especially in pil)
   * might assume that those 3 functions are equivalent to
   * do_round when they fiddle zith threads and team_mask.
   */
  lw6ker_game_state_do_spread (game_state, LW6KER_TEAM_MASK_ALL);
  lw6ker_game_state_do_move (game_state, LW6KER_TEAM_MASK_ALL);
  lw6ker_game_state_finish_round (game_state);
}

u_int32_t
lw6ker_game_state_get_moves (lw6ker_game_state_t * game_state)
{
  return (game_state->moves);
}

u_int32_t
lw6ker_game_state_get_spreads (lw6ker_game_state_t * game_state)
{
  return (game_state->spreads);
}

u_int32_t
lw6ker_game_state_get_rounds (lw6ker_game_state_t * game_state)
{
  return (game_state->rounds);
}

u_int32_t
lw6ker_game_state_get_total_rounds (lw6ker_game_state_t * game_state)
{
  return (game_state->game_struct->rules.rounds_per_sec *
	  game_state->game_struct->rules.total_time);
}

int
lw6ker_game_state_is_over (lw6ker_game_state_t * game_state)
{
  return game_state->over ? 1 : 0;
}

int32_t
lw6ker_game_state_get_nb_active_fighters (lw6ker_game_state_t * game_state)
{
  return (game_state->map_state.armies.active_fighters);
}

int32_t
lw6ker_game_state_get_time_elapsed (lw6ker_game_state_t * game_state)
{
  int32_t ret = 0;
  u_int32_t rounds = 0;

  rounds = lw6ker_game_state_get_rounds (game_state);
  ret = rounds / (u_int32_t) game_state->game_struct->rules.rounds_per_sec;

  return ret;
}

int32_t
lw6ker_game_state_get_time_left (lw6ker_game_state_t * game_state)
{
  int32_t ret = 0;

  ret =
    game_state->game_struct->rules.total_time -
    lw6ker_game_state_get_time_elapsed (game_state);

  return ret;
}

int32_t
lw6ker_game_state_get_global_history (lw6ker_game_state_t * game_state, int i,
				      int team_id)
{
  return _lw6ker_history_get (&(game_state->global_history), i, team_id);
}

int32_t
lw6ker_game_state_get_latest_history (lw6ker_game_state_t * game_state, int i,
				      int team_id)
{
  return _lw6ker_history_get (&(game_state->latest_history), i, team_id);
}

int32_t
lw6ker_game_state_get_global_history_max (lw6ker_game_state_t * game_state)
{
  return _lw6ker_history_get_max (&(game_state->global_history));
}

int32_t
lw6ker_game_state_get_latest_history_max (lw6ker_game_state_t * game_state)
{
  return _lw6ker_history_get_max (&(game_state->latest_history));
}
