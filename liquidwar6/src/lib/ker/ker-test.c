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

#include <CUnit/CUnit.h>

#include "ker.h"

/*
 * The checksums below will change any time the core algorithm
 * is changed. They are *very* important, the need to be respected.
 */
#define _TEST_GAME_STRUCT_CHECKSUM 0x7c3cf9f5
#define _TEST_GAME_STRUCT_HEXA_CHECKSUM 0xd9561e32
#define _TEST_GAME_STATE_CHECKSUM 0xe5d5b129
#define _TEST_GAME_STATE_HEXA_CHECKSUM 0x26e1cf90
#define _TEST_GAME_STATE_POPULATE_CHECKSUM 0x7629629f
#define _TEST_GAME_STATE_ALGORITHM_CHECKSUM 0xd3bca262

#define _TEST_MAP_WIDTH 72
#define _TEST_MAP_HEIGHT 24
#define _TEST_MAP_NB_LAYERS 5
#define _TEST_MAP_NOISE_PERCENT 20
#define _TEST_NB_ROUNDS 100
#define _TEST_NODE_ID 0x1234123412341234LL
#define _TEST_CURSOR1_ID 0x1234
#define _TEST_CURSOR2_ID 0x2345
#define _TEST_CURSOR3_ID 0x3456
#define _TEST_COLOR1 LW6MAP_TEAM_COLOR_RED
#define _TEST_COLOR2 LW6MAP_TEAM_COLOR_GREEN
#define _TEST_COLOR3 LW6MAP_TEAM_COLOR_BLUE
#define _TEST_TEAM_MASK_SEED 123
#define _TEST_HISTORY_TEAM 0
#define _TEST_CHECKSUM_LOG_INTERVAL 2

typedef struct _lw6ker_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
} _lw6ker_test_data_t;

static _lw6ker_test_data_t _test_data = { 0, NULL };

static void
_print_game_struct_repr (lw6sys_context_t * sys_context, const lw6ker_game_struct_t * game_struct)
{
  char *repr = NULL;
  lw6sys_whd_t shape;
  int x, y, z;
  int nb_zones;
  int max_zone_size;
  int is_fg, is_bg;
  lw6sys_xyz_t zone_pos;
  int zone_size;
  int zone_id;
  lw6sys_xyz_t here = { 0, 0, 0 };
  lw6sys_xyz_t there = { 0, 0, 0 };

  lw6ker_game_struct_get_shape (sys_context, game_struct, &shape);
  x = lw6sys_random (sys_context, lw6ker_game_struct_get_w (sys_context, game_struct));
  y = lw6sys_random (sys_context, lw6ker_game_struct_get_h (sys_context, game_struct));
  z = lw6sys_random (sys_context, lw6ker_game_struct_get_d (sys_context, game_struct));
  is_fg = lw6ker_game_struct_is_fg (sys_context, game_struct, x, y, z);
  is_bg = lw6ker_game_struct_is_bg (sys_context, game_struct, x, y, z);
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d,%d,%d is_fg=%d is_bg=%d"), x, y, z, is_fg, is_bg);

  lw6ker_game_struct_get_zones_info (sys_context, game_struct, &nb_zones, &max_zone_size);
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("nb_zones=%d max_zone_size=%d"), nb_zones, max_zone_size);
  zone_id = lw6ker_game_struct_get_zone_id (sys_context, game_struct, shape.w / 2, shape.h / 2, shape.d / 2);
  lw6ker_game_struct_get_zone_info (sys_context, game_struct, zone_id, &zone_pos, &zone_size);
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("zone_id=%d zone_pos=%d,%d,%d zone_size=%d"), zone_id, zone_pos.x, zone_pos.y, zone_pos.z, zone_size);
  lw6ker_game_struct_find_free_slot_near (sys_context, game_struct, &there, here);

  repr = lw6ker_game_struct_repr (sys_context, game_struct);
  if (LW6SYS_TEST_ACK (repr))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("game_struct repr is \"%s\""), repr);
      LW6SYS_FREE (sys_context, repr);
    }
}

static void
_print_game_state_repr (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  char *repr = NULL;
  char *capture = NULL;
  int i;
  int32_t nb_cursors;
  int32_t nb_fighters;
  int total_fighters;
  int fighter_id;
  const lw6ker_fighter_t *fighter1;
  const lw6ker_fighter_t *fighter2;
  const lw6ker_fighter_t *fighter3;
  lw6sys_whd_t shape;
  int x, y, z;
  lw6ker_cursor_t cursor;
  int potential;
  int winner, looser;
  lw6sys_xyz_t current_pos = { 0, 0, 0 };
  lw6sys_xyz_t next_pos = { 0, 0, 0 };
  int team_color = 0;
  int weapon_id = 0;
  int per1000_left = 0;

  repr = lw6ker_game_state_repr (sys_context, game_state);
  if (LW6SYS_TEST_ACK (repr))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("game_state repr is \"%s\""), repr);
      LW6SYS_FREE (sys_context, repr);
    }

  total_fighters = lw6ker_game_state_get_nb_active_fighters (sys_context, game_state);
  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("active_fighters = %d"), total_fighters);
  if (total_fighters > 0)
    {
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	{
	  lw6ker_game_state_get_team_info (sys_context, game_state, i, &nb_cursors, &nb_fighters);

	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		      _x_ ("team %d has %d cursors, %d fighters (%2.1f%%)"),
		      i, nb_cursors, nb_fighters, ((float) nb_fighters) / ((float) total_fighters) * 100.0f);
	  /*
	   * Do NOT remove the following calls, as lw6ker_game_state_get_fighter_X
	   * functions really need to be compiled in.
	   */
	  lw6ker_game_state_get_cursor_by_index (sys_context, game_state, &cursor, 0);
	  lw6ker_game_state_get_shape (sys_context, game_state, &shape);
	  x = lw6sys_random (sys_context, lw6ker_game_state_get_w (sys_context, game_state));
	  y = lw6sys_random (sys_context, lw6ker_game_state_get_h (sys_context, game_state));
	  z = lw6sys_random (sys_context, lw6ker_game_state_get_d (sys_context, game_state));
	  potential = lw6ker_game_state_get_zone_potential (sys_context, game_state, 0, LW6MAP_TEAM_COLOR_RED);
	  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("potential of zone 0 is %d"), potential);
	  fighter_id = lw6ker_game_state_get_fighter_id (sys_context, game_state, x, y, z);
	  if (fighter_id >= 0)
	    {
	      fighter1 = lw6ker_game_state_get_fighter_ro_by_id (sys_context, game_state, fighter_id);
	      fighter2 = lw6ker_game_state_get_fighter_ro_safe (sys_context, game_state, x, y, z);
	      fighter3 = lw6ker_game_state_get_fighter_ro_unsafe (sys_context, game_state, x, y, z);
	      if (LW6SYS_TEST_ACK (fighter1 == fighter2 && fighter2 == fighter3))
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("%d,%d,%d fighter has id %d"), x, y, z, fighter_id);
		}
	      else
		{
		  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("fighter pointer mismatch in %d,%d,%d %p,%p,%p"), x, y, z, fighter1, fighter2, fighter3);
		}
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("no fighter in %d,%d,%d (%d)"), x, y, z, fighter_id);
	    }
	  winner = lw6ker_game_state_get_winner (sys_context, game_state, LW6MAP_TEAM_COLOR_INVALID);
	  looser = lw6ker_game_state_get_looser (sys_context, game_state, LW6MAP_TEAM_COLOR_INVALID);
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("winner=%d looser=%d"), winner, looser);
	  lw6ker_move_get_best_next_pos (sys_context, game_state, &next_pos, &current_pos, 0);
	  lw6ker_game_state_get_charge_per1000 (sys_context, game_state, 0);
	  lw6ker_game_state_get_weapon_per1000_left (sys_context, game_state, 0);
	  lw6ker_game_state_get_latest_weapon (sys_context, game_state, &team_color, &weapon_id, &per1000_left);
	}
    }

  capture = lw6ker_capture_str (sys_context, game_state);
  if (LW6SYS_TEST_ACK (capture))
    {
      if (lw6sys_log_get_console_state (sys_context))
	{
	  printf ("%s", capture);
	  fflush (stdout);
	}
      LW6SYS_FREE (sys_context, capture);
    }
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
	      _x_ ("nb_colors=%d nb_cursors=%d nb_nodes=%d"),
	      lw6ker_game_state_get_nb_colors (sys_context, game_state),
	      lw6ker_game_state_get_nb_cursors (sys_context, game_state), lw6ker_game_state_get_nb_nodes (sys_context, game_state));
  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
	      _x_ ("time_elapsed=%d time_left=%d"),
	      lw6ker_game_state_get_time_elapsed (sys_context, game_state), lw6ker_game_state_get_time_left (sys_context, game_state));
}

static void
_test_struct ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level;
    lw6ker_game_struct_t *game_struct;
    int checksum;

    ret = 0;
    level = lw6map_builtin_custom (sys_context, _TEST_MAP_WIDTH, _TEST_MAP_HEIGHT, _TEST_MAP_NB_LAYERS, _TEST_MAP_NOISE_PERCENT);
    if (LW6SYS_TEST_ACK (level))
      {
	game_struct = lw6ker_game_struct_new (sys_context, level, NULL);
	if (LW6SYS_TEST_ACK (game_struct))
	  {
	    _print_game_struct_repr (sys_context, game_struct);
	    checksum = lw6ker_game_struct_checksum (sys_context, game_struct);
	    if (LW6SYS_TEST_ACK (checksum == _TEST_GAME_STRUCT_CHECKSUM))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("game struct checksum is %08x, OK"), checksum);
		ret = 1;
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("game struct checksum is %08x and should be %08x"), checksum, _TEST_GAME_STRUCT_CHECKSUM);
	      }
	    lw6ker_game_struct_free (sys_context, game_struct);
	  }
	lw6map_free (sys_context, level);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_state ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level;
    lw6ker_game_struct_t *game_struct;
    lw6ker_game_state_t *game_state;
    int checksum;

    ret = 0;
    level = lw6map_builtin_custom (sys_context, _TEST_MAP_WIDTH, _TEST_MAP_HEIGHT, _TEST_MAP_NB_LAYERS, _TEST_MAP_NOISE_PERCENT);
    if (LW6SYS_TEST_ACK (level))
      {
	game_struct = lw6ker_game_struct_new (sys_context, level, NULL);
	if (LW6SYS_TEST_ACK (game_struct))
	  {
	    _print_game_struct_repr (sys_context, game_struct);
	    game_state = lw6ker_game_state_new (sys_context, game_struct, NULL);
	    if (LW6SYS_TEST_ACK (game_state))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("is_over returns %d"), lw6ker_game_state_is_over (sys_context, game_state));
		_print_game_state_repr (sys_context, game_state);
		checksum = lw6ker_game_state_checksum (sys_context, game_state);
		if (LW6SYS_TEST_ACK (checksum == _TEST_GAME_STATE_CHECKSUM))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("game state checksum is %08x, OK"), checksum);
		    ret = 1;
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("game state checksum is %08x and should be %08x"), checksum, _TEST_GAME_STATE_CHECKSUM);
		  }
		lw6ker_game_state_free (sys_context, game_state);
	      }
	    lw6ker_game_struct_free (sys_context, game_struct);
	  }
	lw6map_free (sys_context, level);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_population ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level;
    lw6ker_game_struct_t *game_struct;
    lw6ker_game_state_t *game_state;
    int checksum;

    ret = 0;

    level = lw6map_builtin_custom (sys_context, _TEST_MAP_WIDTH, _TEST_MAP_HEIGHT, _TEST_MAP_NB_LAYERS, _TEST_MAP_NOISE_PERCENT);
    if (LW6SYS_TEST_ACK (level))
      {
	game_struct = lw6ker_game_struct_new (sys_context, level, NULL);
	if (LW6SYS_TEST_ACK (game_struct))
	  {
	    _print_game_struct_repr (sys_context, game_struct);
	    game_state = lw6ker_game_state_new (sys_context, game_struct, NULL);
	    if (LW6SYS_TEST_ACK (game_state))
	      {
		_print_game_state_repr (sys_context, game_state);
		lw6ker_game_state_register_node (sys_context, game_state, _TEST_NODE_ID);
		lw6ker_game_state_add_cursor (sys_context, game_state, _TEST_NODE_ID, _TEST_CURSOR1_ID, _TEST_COLOR1);
		_print_game_state_repr (sys_context, game_state);
		lw6ker_game_state_add_cursor (sys_context, game_state, _TEST_NODE_ID, _TEST_CURSOR2_ID, _TEST_COLOR2);
		_print_game_state_repr (sys_context, game_state);
		lw6ker_game_state_add_cursor (sys_context, game_state, _TEST_NODE_ID, _TEST_CURSOR3_ID, _TEST_COLOR3);
		_print_game_state_repr (sys_context, game_state);
		lw6ker_game_state_remove_cursor (sys_context, game_state, _TEST_NODE_ID, _TEST_CURSOR2_ID);
		_print_game_state_repr (sys_context, game_state);
		checksum = lw6ker_game_state_checksum (sys_context, game_state);
		if (LW6SYS_TEST_ACK (checksum == _TEST_GAME_STATE_POPULATE_CHECKSUM))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("game state checksum is %08x, OK"), checksum);
		    ret = 1;
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				_x_ ("game state checksum is %08x and should be %08x"), checksum, _TEST_GAME_STATE_POPULATE_CHECKSUM);
		  }
		lw6ker_game_state_free (sys_context, game_state);
	      }
	    lw6ker_game_struct_free (sys_context, game_struct);
	  }
	lw6map_free (sys_context, level);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_algorithm ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level;
    lw6ker_game_struct_t *game_struct;
    lw6ker_game_state_t *game_state;
    lw6ker_score_array_t score_array;
    int checksum;
    int i;
    lw6ker_cursor_t cursor;

    ret = 0;
    level = lw6map_builtin_custom (sys_context, _TEST_MAP_WIDTH, _TEST_MAP_HEIGHT, _TEST_MAP_NB_LAYERS, _TEST_MAP_NOISE_PERCENT);
    if (LW6SYS_TEST_ACK (level))
      {
	game_struct = lw6ker_game_struct_new (sys_context, level, NULL);
	if (LW6SYS_TEST_ACK (game_struct))
	  {
	    _print_game_struct_repr (sys_context, game_struct);
	    game_state = lw6ker_game_state_new (sys_context, game_struct, NULL);
	    if (LW6SYS_TEST_ACK (game_state))
	      {
		_print_game_state_repr (sys_context, game_state);
		lw6ker_game_state_register_node (sys_context, game_state, _TEST_NODE_ID);
		lw6ker_game_state_add_cursor (sys_context, game_state, _TEST_NODE_ID, _TEST_CURSOR1_ID, _TEST_COLOR1);
		lw6ker_game_state_add_cursor (sys_context, game_state, _TEST_NODE_ID, _TEST_CURSOR2_ID, _TEST_COLOR2);
		lw6ker_game_state_add_cursor (sys_context, game_state, _TEST_NODE_ID, _TEST_CURSOR3_ID, _TEST_COLOR3);
		_print_game_state_repr (sys_context, game_state);
		lw6ker_cursor_reset (sys_context, &cursor);
		cursor.pos.x = lw6ker_game_state_get_w (sys_context, game_state) / 2;
		cursor.pos.y = lw6ker_game_state_get_h (sys_context, game_state) / 2;
		cursor.node_id = _TEST_NODE_ID;
		cursor.cursor_id = _TEST_CURSOR1_ID;
		lw6ker_game_state_set_cursor (sys_context, game_state, &cursor);
		cursor.cursor_id = _TEST_CURSOR2_ID;
		lw6ker_game_state_set_cursor (sys_context, game_state, &cursor);
		cursor.cursor_id = _TEST_CURSOR3_ID;
		lw6ker_game_state_set_cursor (sys_context, game_state, &cursor);
		lw6ker_game_state_get_cursor (sys_context, game_state, &cursor, _TEST_CURSOR1_ID);
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			    _x_ ("cursor %x letter='%c' color=%d x=%d y=%d"), _TEST_CURSOR1_ID, cursor.letter, cursor.team_color, cursor.pos.x, cursor.pos.y);
		for (i = 0; i < _TEST_NB_ROUNDS; ++i)
		  {
		    lw6ker_game_state_do_round (sys_context, game_state);
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
				_x_ ("round %d, game_state checksum=%08x"),
				lw6ker_game_state_get_rounds (sys_context, game_state), lw6ker_game_state_checksum (sys_context, game_state));
		  }
		lw6ker_score_array_update (sys_context, &score_array, game_state);
		for (i = 0; i < score_array.nb_scores; ++i)
		  {
		    if (LW6SYS_TEST_ACK (score_array.scores[i].fighters_absolute > 0))
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
				    _x_ ("order=%d team_color=%d score=%d%%"), i, score_array.scores[i].team_color, score_array.scores[i].fighters_percent);
		      }
		  }
		for (i = 0; i < lw6sys_imin (LW6KER_HISTORY_SIZE, lw6ker_game_state_get_rounds (sys_context, game_state)); ++i)
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
				_x_
				("history entry %d for team %d is global=%d latest=%d"),
				i, _TEST_HISTORY_TEAM,
				lw6ker_game_state_get_global_history
				(sys_context, game_state, i,
				 _TEST_HISTORY_TEAM), lw6ker_game_state_get_latest_history (sys_context, game_state, i, _TEST_HISTORY_TEAM));
		  }
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			    _x_ ("max history entry global=%d latest=%d"),
			    lw6ker_game_state_get_global_history_max
			    (sys_context, game_state), lw6ker_game_state_get_latest_history_max (sys_context, game_state));
		_print_game_state_repr (sys_context, game_state);
		checksum = lw6ker_game_state_checksum (sys_context, game_state);
		if (LW6SYS_TEST_ACK (checksum == _TEST_GAME_STATE_ALGORITHM_CHECKSUM))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("game state checksum is %08x, OK"), checksum);
		    ret = 1;
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				_x_ ("game state checksum is %08x and should be %08x"), checksum, _TEST_GAME_STATE_ALGORITHM_CHECKSUM);
		  }
		lw6ker_game_state_free (sys_context, game_state);
	      }
	    lw6ker_game_struct_free (sys_context, game_struct);
	  }
	lw6map_free (sys_context, level);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_dup ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level;
    lw6ker_game_struct_t *game_struct;
    lw6ker_game_state_t *game_state;
    lw6ker_game_struct_t *game_struct2;
    lw6ker_game_state_t *game_state2;

    ret = 0;
    level = lw6map_builtin_custom (sys_context, _TEST_MAP_WIDTH, _TEST_MAP_HEIGHT, _TEST_MAP_NB_LAYERS, _TEST_MAP_NOISE_PERCENT);
    if (LW6SYS_TEST_ACK (level))
      {
	game_struct = lw6ker_game_struct_new (sys_context, level, NULL);
	if (LW6SYS_TEST_ACK (game_struct))
	  {
	    _print_game_struct_repr (sys_context, game_struct);
	    game_struct2 = lw6ker_game_struct_dup (sys_context, game_struct, NULL);
	    lw6ker_game_struct_free (sys_context, game_struct);
	    game_struct = NULL;
	    _print_game_struct_repr (sys_context, game_struct2);
	    game_state = lw6ker_game_state_new (sys_context, game_struct2, NULL);
	    if (LW6SYS_TEST_ACK (game_state))
	      {
		_print_game_state_repr (sys_context, game_state);
		game_state2 = lw6ker_game_state_dup (sys_context, game_state, NULL);
		lw6ker_game_state_free (sys_context, game_state);
		game_state = NULL;
		_print_game_state_repr (sys_context, game_state2);
		lw6ker_game_state_free (sys_context, game_state2);
		game_state2 = NULL;
		ret = 1;
	      }
	    lw6ker_game_struct_free (sys_context, game_struct2);
	    game_struct2 = NULL;
	  }
	lw6map_free (sys_context, level);
	level = NULL;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_team_mask ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    u_int32_t even = 0;
    u_int32_t odd = 0;
    int i;

    lw6ker_team_mask_get (sys_context, &even, &odd, _TEST_TEAM_MASK_SEED);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("even team mask %x"), even);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("odd team mask %x"), odd);
    for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
      {
	if (lw6ker_team_mask_is_concerned (sys_context, i, even))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("team %d is even"), i);
	  }
	else if (lw6ker_team_mask_is_concerned (sys_context, i, odd))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("team %d is odd"), i);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("team mask inconsistency for %d"), i);
	    ret = 0;
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_hexa ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level = NULL;
    lw6ker_game_struct_t *game_struct = NULL;
    lw6ker_game_state_t *game_state = NULL;
    lw6ker_game_struct_t *dup_game_struct = NULL;
    lw6ker_game_state_t *dup_game_state = NULL;
    lw6ker_game_struct_t *game_struct2 = NULL;
    lw6ker_game_state_t *game_state2 = NULL;
    u_int32_t checksum_struct = 0;
    u_int32_t checksum_hexa_struct = 0;
    u_int32_t checksum_state = 0;
    u_int32_t checksum_hexa_state = 0;
    u_int32_t checksum_struct2 = 0;
    u_int32_t checksum_state2 = 0;
    char *hexa_struct = NULL;
    char *hexa_state = NULL;
    char *hexa_struct2 = NULL;
    char *hexa_state2 = NULL;
    int i = 0;

    ret = 0;
    level = lw6map_builtin_custom (sys_context, _TEST_MAP_WIDTH, _TEST_MAP_HEIGHT, _TEST_MAP_NB_LAYERS, _TEST_MAP_NOISE_PERCENT);
    if (LW6SYS_TEST_ACK (level))
      {
	game_struct = lw6ker_game_struct_new (sys_context, level, NULL);
	if (LW6SYS_TEST_ACK (game_struct))
	  {
	    _print_game_struct_repr (sys_context, game_struct);
	    checksum_struct = lw6ker_game_struct_checksum (sys_context, game_struct);
	    hexa_struct = lw6ker_game_struct_to_hexa (sys_context, game_struct);
	    lw6ker_game_struct_free (sys_context, game_struct);
	    game_struct = NULL;
	    if (LW6SYS_TEST_ACK (hexa_struct))
	      {
		checksum_hexa_struct = lw6sys_checksum_str (sys_context, hexa_struct);
		if (LW6SYS_TEST_ACK (checksum_hexa_struct == _TEST_GAME_STRUCT_HEXA_CHECKSUM))
		  {
		    game_struct2 = lw6ker_game_struct_from_hexa (sys_context, hexa_struct, level);
		    if (LW6SYS_TEST_ACK (game_struct2))
		      {
			/*
			 * Duplicate so that hexa dumps are calculated
			 * from really different objects, any id error
			 * will be tracked...
			 */
			dup_game_struct = lw6ker_game_struct_dup (sys_context, game_struct2, NULL);
			if (LW6SYS_TEST_ACK (dup_game_struct))
			  {
			    checksum_struct2 = lw6ker_game_struct_checksum (sys_context, dup_game_struct);
			    hexa_struct2 = lw6ker_game_struct_to_hexa (sys_context, dup_game_struct);
			    lw6ker_game_struct_free (sys_context, dup_game_struct);
			    dup_game_struct = NULL;
			  }
			if (LW6SYS_TEST_ACK (hexa_struct2))
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
					_x_
					("hexa_struct length=%d hexa_struct2 length=%d checksum_struct=%08x checksum_struct2=%08x"),
					(int) strlen (hexa_struct), (int) strlen (hexa_struct2), checksum_struct, checksum_struct2);
			    if (LW6SYS_TEST_ACK (checksum_struct == checksum_struct2 && lw6sys_str_is_same (sys_context, hexa_struct, hexa_struct2)))
			      {
				game_state = lw6ker_game_state_new (sys_context, game_struct2, NULL);
				if (LW6SYS_TEST_ACK (game_state))
				  {
				    lw6ker_game_state_checksum_log_set_interval (sys_context, game_state, _TEST_CHECKSUM_LOG_INTERVAL);
				    lw6ker_game_state_register_node (sys_context, game_state, _TEST_NODE_ID);
				    lw6ker_game_state_add_cursor (sys_context, game_state, _TEST_NODE_ID, _TEST_CURSOR1_ID, _TEST_COLOR1);
				    lw6ker_game_state_add_cursor (sys_context, game_state, _TEST_NODE_ID, _TEST_CURSOR2_ID, _TEST_COLOR2);
				    for (i = 0; i < _TEST_NB_ROUNDS; ++i)
				      {
					lw6ker_game_state_do_round (sys_context, game_state);
				      }
				    _print_game_state_repr (sys_context, game_state);
				    checksum_state = lw6ker_game_state_checksum (sys_context, game_state);
				    hexa_state = lw6ker_game_state_to_hexa (sys_context, game_state);
				    lw6ker_game_state_free (sys_context, game_state);

				    if (LW6SYS_TEST_ACK (hexa_state))
				      {
					checksum_hexa_state = lw6sys_checksum_str (sys_context, hexa_state);
					if (LW6SYS_TEST_ACK (checksum_hexa_state == _TEST_GAME_STATE_HEXA_CHECKSUM))
					  {
					    game_state2 = lw6ker_game_state_from_hexa (sys_context, hexa_state, game_struct2);
					    if (LW6SYS_TEST_ACK (game_state2))
					      {
						/*
						 * Duplicate so that hexa dumps are calculated
						 * from really different objects, any id error
						 * will be tracked...
						 */
						dup_game_state = lw6ker_game_state_dup (sys_context, game_state2, NULL);
						if (LW6SYS_TEST_ACK (dup_game_state))
						  {
						    checksum_state2 = lw6ker_game_state_checksum (sys_context, dup_game_state);
						    hexa_state2 = lw6ker_game_state_to_hexa (sys_context, dup_game_state);
						    lw6ker_game_state_free (sys_context, dup_game_state);
						    dup_game_state = NULL;
						  }
						if (LW6SYS_TEST_ACK (hexa_state2))
						  {
						    lw6sys_log
						      (sys_context,
						       LW6SYS_LOG_NOTICE,
						       _x_
						       ("hexa_state length=%d hexa_state2 length=%d checksum_state=%08x checksum_state2=%08x"),
						       (int) strlen (hexa_state), (int) strlen (hexa_state2), checksum_state, checksum_state2);
						    if (LW6SYS_TEST_ACK
							(checksum_state == checksum_state2 && lw6sys_str_is_same (sys_context, hexa_state, hexa_state2)))
						      {
							ret = 1;
						      }
						    else
						      {
							if (!LW6SYS_TEST_ACK (checksum_state == checksum_state2))
							  {
							    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("game_state checksums differ"));
							  }
							if (!LW6SYS_TEST_ACK (lw6sys_str_is_same (sys_context, hexa_state, hexa_state2)))
							  {
							    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("game_state hexa dumps differ"));
							  }
							ret = 0;
						      }
						    LW6SYS_FREE (sys_context, hexa_state2);
						  }
						lw6ker_game_state_free (sys_context, game_state2);
						game_state2 = NULL;
					      }
					  }
					else
					  {
					    lw6sys_log (sys_context,
							LW6SYS_LOG_WARNING,
							_x_
							("bad state hexa checksum, got %x but shoud be %x"),
							checksum_hexa_state, _TEST_GAME_STATE_HEXA_CHECKSUM);
					  }
					LW6SYS_FREE (sys_context, hexa_state);
				      }
				  }
			      }
			    else
			      {
				if (!LW6SYS_TEST_ACK (checksum_struct == checksum_struct2))
				  {
				    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("game_struct checksums differ"));
				  }
				if (!LW6SYS_TEST_ACK (lw6sys_str_is_same (sys_context, hexa_struct, hexa_struct2)))
				  {
				    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("game_struct hexa dumps differ"));
				  }
				ret = 0;
			      }
			    LW6SYS_FREE (sys_context, hexa_struct2);
			  }
			lw6ker_game_struct_free (sys_context, game_struct2);
			game_struct2 = NULL;
		      }
		  }
		else
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
				_x_ ("bad struct hexa checksum, got %x but shoud be %x"), checksum_hexa_struct, _TEST_GAME_STRUCT_HEXA_CHECKSUM);
		  }
		LW6SYS_FREE (sys_context, hexa_struct);
	      }
	  }
	lw6map_free (sys_context, level);
	level = NULL;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libker CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libker CUnit test suite"));

  return CUE_SUCCESS;
}

/**
 * lw6ker_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libker module.
 * Thoses tests Will perform deep checksums
 * and *really* check many things. If this passes, the algorithm
 * is fine. What could make it fail is a serious bug and/or some
 * weird combination of endianess, byte alignment... *
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6ker_test_register (lw6sys_context_t * sys_context, int mode)
{
  int ret = 1;
  CU_Suite *suite = NULL;

  _test_data.sys_context = sys_context;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test_register (sys_context, mode);
      lw6map_test_register (sys_context, mode);
    }

  suite = CU_add_suite ("lw6ker", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_team_mask);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_struct);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_population);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_algorithm);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_dup);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_hexa);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_state);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6ker_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @ker module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6ker_test_run (lw6sys_context_t * sys_context, int mode)
{
  int ret = 0;

  _test_data.ret = 1;
  _test_data.sys_context = sys_context;

  if (lw6sys_cunit_run_tests (sys_context, mode))
    {
      ret = _test_data.ret;
    }

  return ret;
}
