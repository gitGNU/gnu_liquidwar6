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

#include "sim.h"
#include "sim-internal.h"

static int
_simulate (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6sim_results_t * results, lw6ker_game_struct_t * game_struct, int mask,
	   char *bot_backend)
{
  int ret = 0;
  lw6ker_game_state_t *game_state = NULL;
  int nb_cursors = 0;
  u_int64_t node_id;
  int cursors_colors[LW6MAP_MAX_NB_TEAMS];
  u_int16_t cursors_ids[LW6MAP_MAX_NB_TEAMS];
  lw6bot_backend_t *cursors_bots[LW6MAP_MAX_NB_TEAMS];
  lw6ker_cursor_t cursor;
  int i, j;
  int round, nb_rounds;
  lw6bot_seed_t seed;
  int x = 0, y = 0;
  lw6ker_score_array_t score_array;
  int bonus;

  memset (cursors_colors, 0, sizeof (cursors_colors));
  memset (cursors_ids, 0, sizeof (cursors_ids));
  memset (cursors_bots, 0, sizeof (cursors_bots));
  memset (&cursor, 0, sizeof (lw6ker_cursor_t));
  memset (&seed, 0, sizeof (lw6bot_seed_t));

  node_id = lw6sys_generate_id_64 (sys_context);
  nb_rounds = game_struct->rules.rounds_per_sec * game_struct->rules.total_time;

  game_state = lw6ker_game_state_new (sys_context, game_struct, NULL);
  if (game_state)
    {
      seed.game_state = game_state;
      seed.pilot = NULL;
      seed.dirty_read = 0;
      seed.param.speed = LW6MAP_TEAMS_DEFAULT_BOT_SPEED;
      seed.param.iq = LW6MAP_TEAMS_DEFAULT_BOT_IQ;

      ret = 1;

      lw6ker_game_state_register_node (sys_context, game_state, node_id);
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	{
	  if (lw6ker_team_mask_is_concerned (sys_context, i, mask))
	    {
	      cursors_colors[nb_cursors] = i;
	      cursors_ids[nb_cursors] = lw6sys_generate_id_16 (sys_context);
	      lw6ker_game_state_add_cursor (sys_context, game_state, node_id, cursors_ids[nb_cursors], i);
	      cursors_bots[nb_cursors] = lw6bot_create_backend (sys_context, argc, argv, bot_backend);
	      if (cursors_bots[nb_cursors])
		{
		  seed.param.cursor_id = cursors_ids[nb_cursors];
		  ret = ret && lw6bot_init (sys_context, cursors_bots[nb_cursors], &seed);
		}
	      nb_cursors++;
	    }
	}

      for (round = 0; round < nb_rounds; ++round)
	{
	  if (!(round % _LW6SIM_BOT_ROUNDS_PERIOD))
	    {
	      for (i = 0; i < nb_cursors; ++i)
		{
		  if (cursors_bots[i])
		    {
		      lw6bot_next_move (sys_context, cursors_bots[i], &x, &y);
		      lw6ker_cursor_reset (sys_context, &cursor);
		      cursor.pos.x = x;
		      cursor.pos.y = y;
		      cursor.node_id = node_id;
		      cursor.cursor_id = cursors_ids[i];
		      lw6ker_game_state_set_cursor (sys_context, game_state, &cursor);
		    }
		}
	    }

	  lw6ker_game_state_do_round (sys_context, game_state);
	}

      lw6ker_score_array_update (sys_context, &score_array, game_state);
      for (i = 0; i < score_array.nb_scores; ++i)
	{
	  j = score_array.scores[i].team_color;
	  if (lw6map_team_color_is_valid (j))
	    {
	      bonus = score_array.scores[i].consolidated_percent;
	      results->absolute[j] += bonus;
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bonus for team %d is %d, new absolute is %d"), j, bonus, results->absolute[j]);
	    }
	}

      for (i = 0; i < nb_cursors; ++i)
	{
	  if (cursors_bots[i])
	    {
	      lw6bot_quit (sys_context, cursors_bots[i]);
	      lw6bot_destroy_backend (sys_context, cursors_bots[i]);
	    }
	}

      lw6ker_game_state_free (sys_context, game_state);
    }

  return ret;
}

/**
 * lw6sim_simulate
 *
 * @sys_context: global system context
 * @argc: argc as passed to @main
 * @argv: argv as passed to @main
 * @results: out param, results of the simulation
 * @nb_teams: number of teams
 * @bot_backend: bot backend to use
 *
 * Runs a simulation of several battle/games on the default map using
 * different team settings. Will test teams up to the given number,
 * for instance if you give 3 as an argument, will run tests with teams
 * 0, 1 and 2 (that's to say a total of 3 teams).
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6sim_simulate (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6sim_results_t * results, int nb_teams, char *bot_backend)
{
  int ret = 0;
  int max_mask;
  int i;
  lw6map_level_t *level = NULL;
  lw6ker_game_struct_t *game_struct = NULL;

  level = lw6map_builtin_defaults (sys_context);
  level->param.rules.total_time = _LW6SIM_SIMULATION_TIME;
  if (level)
    {
      game_struct = lw6ker_game_struct_new (sys_context, level, NULL);
      if (game_struct)
	{
	  max_mask = _lw6sim_mask_get_max (sys_context, nb_teams);
	  lw6sim_results_zero (sys_context, results);
	  results->nb_teams = nb_teams;
	  ret = 1;
	  for (i = 0; i < max_mask; ++i)
	    {
	      if (_lw6sim_mask_is_valid (sys_context, i))
		{
		  if (ret)
		    {
		      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("simulating %d/%d"), i, max_mask);
		      ret = _simulate (sys_context, argc, argv, results, game_struct, i, bot_backend);
		    }
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("skipping simulation %d/%d, unvalid mask"), i, max_mask);
		}
	    }
	  lw6ker_game_struct_free (sys_context, game_struct);
	}
      lw6map_free (sys_context, level);
    }

  ret = lw6sim_results_update_percents (sys_context, results) && ret;

  return ret;
}

/**
 * lw6sim_simulate_basic
 *
 * @sys_context: global system context
 * @argc: argc as passed to @main
 * @argv: argv as passed to @main
 * @results: out param, results of the simulation
 *
 * Runs a simulation of several battle/games on the default map using
 * different team settings. Will test the most common colors only,
 * with the most popular bot.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6sim_simulate_basic (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6sim_results_t * results)
{
  return lw6sim_simulate (sys_context, argc, argv, results, LW6SIM_SIMULATE_BASIC_NB_TEAMS, LW6SIM_SIMULATE_BASIC_BOT_BACKEND);
}

/**
 * lw6sim_simulate_full
 *
 * @sys_context: global system context
 * @argc: argc as passed to @main
 * @argv: argv as passed to @main
 * @results: out param, results of the simulation
 *
 * Runs a simulation of several battle/games on the default map using
 * different team settings. Will test all colors,
 * with the most popular bot.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6sim_simulate_full (lw6sys_context_t * sys_context, int argc, const char *argv[], lw6sim_results_t * results)
{
  return lw6sim_simulate (sys_context, argc, argv, results, LW6SIM_SIMULATE_FULL_NB_TEAMS, LW6SIM_SIMULATE_FULL_BOT_BACKEND);
}
