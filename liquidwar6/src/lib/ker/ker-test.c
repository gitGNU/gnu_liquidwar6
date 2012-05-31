/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

#include "ker.h"

#define TEST_MAP_WIDTH 72
#define TEST_MAP_HEIGHT 24
#define TEST_MAP_NB_LAYERS 5
#define TEST_MAP_NOISE_PERCENT 20
#define TEST_NB_ROUNDS 100
#define TEST_GAME_STRUCT_CHECKSUM 0xf9c216d4
#define TEST_GAME_STATE_CHECKSUM 0x92f323b1
#define TEST_GAME_STATE_POPULATE_CHECKSUM 0x95922a8b
#define TEST_GAME_STATE_ALGORITHM_CHECKSUM 0x4fd07888
#define TEST_NODE_ID 0x1234123412341234LL
#define TEST_CURSOR1_ID 0x1234
#define TEST_CURSOR2_ID 0x2345
#define TEST_CURSOR3_ID 0x3456
#define TEST_COLOR1 LW6MAP_TEAM_COLOR_RED
#define TEST_COLOR2 LW6MAP_TEAM_COLOR_GREEN
#define TEST_COLOR3 LW6MAP_TEAM_COLOR_BLUE
#define TEST_TEAM_MASK_SEED 123
#define TEST_HISTORY_TEAM 0

static void
print_game_struct_repr (lw6ker_game_struct_t * game_struct)
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

  lw6ker_game_struct_get_shape (game_struct, &shape);
  x = lw6sys_random (lw6ker_game_struct_get_w (game_struct));
  y = lw6sys_random (lw6ker_game_struct_get_h (game_struct));
  z = lw6sys_random (lw6ker_game_struct_get_d (game_struct));
  is_fg = lw6ker_game_struct_is_fg (game_struct, x, y, z);
  is_bg = lw6ker_game_struct_is_bg (game_struct, x, y, z);
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("%d,%d,%d is_fg=%d is_bg=%d"), x,
	      y, z, is_fg, is_bg);

  lw6ker_game_struct_get_zones_info (game_struct, &nb_zones, &max_zone_size);
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("nb_zones=%d max_zone_size=%d"),
	      nb_zones, max_zone_size);
  zone_id =
    lw6ker_game_struct_get_zone_id (game_struct, shape.w / 2, shape.h / 2,
				    shape.d / 2);
  lw6ker_game_struct_get_zone_info (game_struct, zone_id, &zone_pos,
				    &zone_size);
  lw6sys_log (LW6SYS_LOG_NOTICE,
	      _x_ ("zone_id=%d zone_pos=%d,%d,%d zone_size=%d"), zone_id,
	      zone_pos.x, zone_pos.y, zone_pos.z, zone_size);
  lw6ker_game_struct_find_free_slot_near (game_struct, &there, here);

  repr = lw6ker_game_struct_repr (game_struct);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("game_struct repr is \"%s\""),
		  repr);
      LW6SYS_FREE (repr);
    }
}

static void
print_game_state_repr (lw6ker_game_state_t * game_state)
{
  char *repr = NULL;
  char *capture = NULL;
  int i;
  int32_t nb_cursors;
  int32_t nb_fighters;
  int total_fighters;
  int fighter_id;
  lw6ker_fighter_t *fighter1;
  lw6ker_fighter_t *fighter2;
  lw6ker_fighter_t *fighter3;
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

  repr = lw6ker_game_state_repr (game_state);
  if (repr)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("game_state repr is \"%s\""), repr);
      LW6SYS_FREE (repr);
    }

  total_fighters = lw6ker_game_state_get_nb_active_fighters (game_state);
  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("active_fighters = %d"), total_fighters);
  if (total_fighters > 0)
    {
      for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
	{
	  lw6ker_game_state_get_team_info (game_state, i, &nb_cursors,
					   &nb_fighters);

	  lw6sys_log (LW6SYS_LOG_DEBUG,
		      _x_ ("team %d has %d cursors, %d fighters (%2.1f%%)"),
		      i, nb_cursors, nb_fighters,
		      ((float) nb_fighters) / ((float) total_fighters) *
		      100.0f);
	  /*
	   * Do NOT remove the following calls, as lw6ker_game_state_get_fighter_X
	   * functions really need to be compiled in.
	   */
	  lw6ker_game_state_get_cursor_by_index (game_state, &cursor, 0);
	  lw6ker_game_state_get_shape (game_state, &shape);
	  x = lw6sys_random (lw6ker_game_state_get_w (game_state));
	  y = lw6sys_random (lw6ker_game_state_get_h (game_state));
	  z = lw6sys_random (lw6ker_game_state_get_d (game_state));
	  potential =
	    lw6ker_game_state_get_zone_potential (game_state, 0,
						  LW6MAP_TEAM_COLOR_RED);
	  lw6sys_log (LW6SYS_LOG_DEBUG, _x_ ("potential of zone 0 is %d"),
		      potential);
	  fighter_id = lw6ker_game_state_get_fighter_id (game_state, x, y, z);
	  if (fighter_id >= 0)
	    {
	      fighter1 =
		lw6ker_game_state_get_fighter_by_id (game_state, fighter_id);
	      fighter2 =
		lw6ker_game_state_get_fighter_safe (game_state, x, y, z);
	      fighter3 =
		lw6ker_game_state_get_fighter_unsafe (game_state, x, y, z);
	      if (fighter1 == fighter2 && fighter2 == fighter3)
		{
		  lw6sys_log (LW6SYS_LOG_NOTICE,
			      _x_ ("%d,%d,%d fighter has id %d"), x, y, z,
			      fighter_id);
		}
	      else
		{
		  lw6sys_log (LW6SYS_LOG_WARNING,
			      _x_
			      ("fighter pointer mismatch in %d,%d,%d %p,%p,%p"),
			      x, y, z, fighter1, fighter2, fighter3);
		}
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_NOTICE,
			  _x_ ("no fighter in %d,%d,%d (%d)"), x, y, z,
			  fighter_id);
	    }
	  winner =
	    lw6ker_game_state_get_winner (game_state,
					  LW6MAP_TEAM_COLOR_INVALID);
	  looser =
	    lw6ker_game_state_get_looser (game_state,
					  LW6MAP_TEAM_COLOR_INVALID);
	  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("winner=%d looser=%d"), winner,
		      looser);
	  lw6ker_move_get_best_next_pos (game_state, &next_pos, &current_pos,
					 0);
	  lw6ker_game_state_get_charge_per1000 (game_state, 0);
	  lw6ker_game_state_get_weapon_per1000_left (game_state, 0);
	  lw6ker_game_state_get_latest_weapon (game_state, &team_color,
					       &weapon_id, &per1000_left);
	}
    }

  capture = lw6ker_capture_str (game_state);
  if (capture)
    {
      printf ("%s", capture);
      fflush (stdout);
      LW6SYS_FREE (capture);
    }
  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("time_elapsed=%d time_left=%d"),
	      lw6ker_game_state_get_time_elapsed (game_state),
	      lw6ker_game_state_get_time_left (game_state));
}

static int
test_struct ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level;
    lw6ker_game_struct_t *game_struct;
    int checksum;

    ret = 0;
    level =
      lw6map_builtin_custom (TEST_MAP_WIDTH, TEST_MAP_HEIGHT,
			     TEST_MAP_NB_LAYERS, TEST_MAP_NOISE_PERCENT);
    if (level)
      {
	game_struct = lw6ker_game_struct_new (level, NULL);
	if (game_struct)
	  {
	    print_game_struct_repr (game_struct);
	    checksum = lw6ker_game_struct_checksum (game_struct);
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_
			("game struct checksum is %08x and should be %08x"),
			checksum, TEST_GAME_STRUCT_CHECKSUM);
	    lw6ker_game_struct_free (game_struct);
	    ret = (checksum == TEST_GAME_STRUCT_CHECKSUM);
	  }
	lw6map_free (level);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_state ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level;
    lw6ker_game_struct_t *game_struct;
    lw6ker_game_state_t *game_state;
    int checksum;

    ret = 0;
    level =
      lw6map_builtin_custom (TEST_MAP_WIDTH, TEST_MAP_HEIGHT,
			     TEST_MAP_NB_LAYERS, TEST_MAP_NOISE_PERCENT);
    if (level)
      {
	game_struct = lw6ker_game_struct_new (level, NULL);
	if (game_struct)
	  {
	    print_game_struct_repr (game_struct);
	    game_state = lw6ker_game_state_new (game_struct, NULL);
	    if (game_state)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("is_over returns %d"),
			    lw6ker_game_state_is_over (game_state));
		print_game_state_repr (game_state);
		checksum = lw6ker_game_state_checksum (game_state);
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("game state checksum is %08x and should be %08x"),
			    checksum, TEST_GAME_STATE_CHECKSUM);
		ret = (checksum == TEST_GAME_STATE_CHECKSUM);
		lw6ker_game_state_free (game_state);
	      }
	    lw6ker_game_struct_free (game_struct);
	  }
	lw6map_free (level);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_population ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level;
    lw6ker_game_struct_t *game_struct;
    lw6ker_game_state_t *game_state;
    int checksum;

    ret = 0;

    level =
      lw6map_builtin_custom (TEST_MAP_WIDTH, TEST_MAP_HEIGHT,
			     TEST_MAP_NB_LAYERS, TEST_MAP_NOISE_PERCENT);
    if (level)
      {
	game_struct = lw6ker_game_struct_new (level, NULL);
	if (game_struct)
	  {
	    print_game_struct_repr (game_struct);
	    game_state = lw6ker_game_state_new (game_struct, NULL);
	    if (game_state)
	      {
		print_game_state_repr (game_state);
		lw6ker_game_state_register_node (game_state, TEST_NODE_ID);
		lw6ker_game_state_add_cursor (game_state, TEST_NODE_ID,
					      TEST_CURSOR1_ID, TEST_COLOR1);
		print_game_state_repr (game_state);
		lw6ker_game_state_add_cursor (game_state, TEST_NODE_ID,
					      TEST_CURSOR2_ID, TEST_COLOR2);
		print_game_state_repr (game_state);
		lw6ker_game_state_add_cursor (game_state, TEST_NODE_ID,
					      TEST_CURSOR3_ID, TEST_COLOR3);
		print_game_state_repr (game_state);
		lw6ker_game_state_remove_cursor (game_state, TEST_NODE_ID,
						 TEST_CURSOR2_ID);
		print_game_state_repr (game_state);
		checksum = lw6ker_game_state_checksum (game_state);
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("game state checksum is %08x and should be %08x"),
			    checksum, TEST_GAME_STATE_POPULATE_CHECKSUM);
		ret = (checksum == TEST_GAME_STATE_POPULATE_CHECKSUM);
		lw6ker_game_state_free (game_state);
	      }
	    lw6ker_game_struct_free (game_struct);
	  }
	lw6map_free (level);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_algorithm ()
{
  int ret = 1;
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
    level =
      lw6map_builtin_custom (TEST_MAP_WIDTH, TEST_MAP_HEIGHT,
			     TEST_MAP_NB_LAYERS, TEST_MAP_NOISE_PERCENT);
    if (level)
      {
	game_struct = lw6ker_game_struct_new (level, NULL);
	if (game_struct)
	  {
	    print_game_struct_repr (game_struct);
	    game_state = lw6ker_game_state_new (game_struct, NULL);
	    if (game_state)
	      {
		print_game_state_repr (game_state);
		lw6ker_game_state_register_node (game_state, TEST_NODE_ID);
		lw6ker_game_state_add_cursor (game_state, TEST_NODE_ID,
					      TEST_CURSOR1_ID, TEST_COLOR1);
		lw6ker_game_state_add_cursor (game_state, TEST_NODE_ID,
					      TEST_CURSOR2_ID, TEST_COLOR2);
		lw6ker_game_state_add_cursor (game_state, TEST_NODE_ID,
					      TEST_CURSOR3_ID, TEST_COLOR3);
		print_game_state_repr (game_state);
		lw6ker_cursor_reset (&cursor);
		cursor.pos.x = lw6ker_game_state_get_w (game_state) / 2;
		cursor.pos.y = lw6ker_game_state_get_h (game_state) / 2;
		cursor.node_id = TEST_NODE_ID;
		cursor.cursor_id = TEST_CURSOR1_ID;
		lw6ker_game_state_set_cursor (game_state, &cursor);
		cursor.cursor_id = TEST_CURSOR2_ID;
		lw6ker_game_state_set_cursor (game_state, &cursor);
		cursor.cursor_id = TEST_CURSOR3_ID;
		lw6ker_game_state_set_cursor (game_state, &cursor);
		lw6ker_game_state_get_cursor (game_state,
					      &cursor, TEST_CURSOR1_ID);
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("cursor %x letter='%c' color=%d x=%d y=%d"),
			    TEST_CURSOR1_ID, cursor.letter, cursor.team_color,
			    cursor.pos.x, cursor.pos.y);
		for (i = 0; i < TEST_NB_ROUNDS; ++i)
		  {
		    lw6ker_game_state_do_round (game_state);
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_ ("round %d, game_state checksum=%08x"),
				lw6ker_game_state_get_rounds (game_state),
				lw6ker_game_state_checksum (game_state));
		  }
		lw6ker_score_array_update (&score_array, game_state);
		for (i = 0; i < score_array.nb_scores; ++i)
		  {
		    if (score_array.scores[i].fighters_absolute > 0)
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_
				    ("order=%d team_color=%d score=%d%%"),
				    i, score_array.scores[i].team_color,
				    score_array.scores[i].fighters_percent);
		      }
		  }
		for (i = 0;
		     i < lw6sys_imin (LW6KER_HISTORY_SIZE,
				      lw6ker_game_state_get_rounds
				      (game_state)); ++i)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_
				("history entry %d for team %d is global=%d latest=%d"),
				i, TEST_HISTORY_TEAM,
				lw6ker_game_state_get_global_history
				(game_state, i, TEST_HISTORY_TEAM),
				lw6ker_game_state_get_latest_history
				(game_state, i, TEST_HISTORY_TEAM));
		  }
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("max history entry global=%d latest=%d"),
			    lw6ker_game_state_get_global_history_max
			    (game_state),
			    lw6ker_game_state_get_latest_history_max
			    (game_state));
		print_game_state_repr (game_state);
		checksum = lw6ker_game_state_checksum (game_state);
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_
			    ("game state checksum is %08x and should be %08x"),
			    checksum, TEST_GAME_STATE_ALGORITHM_CHECKSUM);
		ret = (checksum == TEST_GAME_STATE_ALGORITHM_CHECKSUM);
		lw6ker_game_state_free (game_state);
	      }
	    lw6ker_game_struct_free (game_struct);
	  }
	lw6map_free (level);
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_dup ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level;
    lw6ker_game_struct_t *game_struct;
    lw6ker_game_state_t *game_state;
    lw6ker_game_struct_t *game_struct2;
    lw6ker_game_state_t *game_state2;

    ret = 0;
    level =
      lw6map_builtin_custom (TEST_MAP_WIDTH, TEST_MAP_HEIGHT,
			     TEST_MAP_NB_LAYERS, TEST_MAP_NOISE_PERCENT);
    if (level)
      {
	game_struct = lw6ker_game_struct_new (level, NULL);
	if (game_struct)
	  {
	    print_game_struct_repr (game_struct);
	    game_struct2 = lw6ker_game_struct_dup (game_struct, NULL);
	    lw6ker_game_struct_free (game_struct);
	    game_struct = NULL;
	    print_game_struct_repr (game_struct2);
	    game_state = lw6ker_game_state_new (game_struct2, NULL);
	    if (game_state)
	      {
		print_game_state_repr (game_state);
		game_state2 = lw6ker_game_state_dup (game_state, NULL);
		lw6ker_game_state_free (game_state);
		game_state = NULL;
		print_game_state_repr (game_state2);
		lw6ker_game_state_free (game_state2);
		game_state2 = NULL;
		ret = 1;
	      }
	    lw6ker_game_struct_free (game_struct2);
	    game_struct2 = NULL;
	  }
	lw6map_free (level);
	level = NULL;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_team_mask ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    u_int32_t even = 0;
    u_int32_t odd = 0;
    int i;

    lw6ker_team_mask_get (&even, &odd, TEST_TEAM_MASK_SEED);
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("even team mask %x"), even);
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("odd team mask %x"), odd);
    for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
      {
	if (lw6ker_team_mask_is_concerned (i, even))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("team %d is even"), i);
	  }
	else if (lw6ker_team_mask_is_concerned (i, odd))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("team %d is odd"), i);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("team mask inconsistency for %d"), i);
	    ret = 0;
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_hexa ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level = NULL;
    lw6ker_game_struct_t *game_struct = NULL;
    lw6ker_game_state_t *game_state = NULL;
    lw6ker_game_struct_t *game_struct2 = NULL;
    lw6ker_game_state_t *game_state2 = NULL;
    u_int32_t checksum_struct = 0;
    u_int32_t checksum_state = 0;
    u_int32_t checksum_struct2 = 0;
    u_int32_t checksum_state2 = 0;
    char *hexa_struct = NULL;
    char *hexa_state = NULL;
    char *hexa_struct2 = NULL;
    char *hexa_state2 = NULL;
    int i = 0;

    ret = 0;
    level =
      lw6map_builtin_custom (TEST_MAP_WIDTH, TEST_MAP_HEIGHT,
			     TEST_MAP_NB_LAYERS, TEST_MAP_NOISE_PERCENT);
    if (level)
      {
	game_struct = lw6ker_game_struct_new (level, NULL);
	if (game_struct)
	  {
	    print_game_struct_repr (game_struct);
	    checksum_struct = lw6ker_game_struct_checksum (game_struct);
	    hexa_struct = lw6ker_game_struct_to_hexa (game_struct);
	    lw6ker_game_struct_free (game_struct);
	    if (hexa_struct)
	      {
		game_struct2 =
		  lw6ker_game_struct_from_hexa (hexa_struct, level);
		if (game_struct2)
		  {
		    checksum_struct2 =
		      lw6ker_game_struct_checksum (game_struct2);
		    hexa_struct2 = lw6ker_game_struct_to_hexa (game_struct2);
		    if (hexa_struct2)
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_
				    ("hexa_struct length=%d hexa_struct2 length=%d checksum_struct=%08x checksum_struct2=%08x"),
				    (int) strlen (hexa_struct),
				    (int) strlen (hexa_struct2),
				    checksum_struct, checksum_struct2);
			if (checksum_struct == checksum_struct2
			    && lw6sys_str_is_same (hexa_struct, hexa_struct2))
			  {
			    game_state =
			      lw6ker_game_state_new (game_struct2, NULL);
			    if (game_state)
			      {
				lw6ker_game_state_register_node (game_state,
								 TEST_NODE_ID);
				lw6ker_game_state_add_cursor (game_state,
							      TEST_NODE_ID,
							      TEST_CURSOR1_ID,
							      TEST_COLOR1);
				lw6ker_game_state_add_cursor (game_state,
							      TEST_NODE_ID,
							      TEST_CURSOR2_ID,
							      TEST_COLOR2);
				for (i = 0; i < TEST_NB_ROUNDS; ++i)
				  {
				    lw6ker_game_state_do_round (game_state);
				  }
				print_game_state_repr (game_state);
				checksum_state =
				  lw6ker_game_state_checksum (game_state);
				hexa_state =
				  lw6ker_game_state_to_hexa (game_state);
				lw6ker_game_state_free (game_state);

				if (hexa_state)
				  {
				    game_state2 =
				      lw6ker_game_state_from_hexa (hexa_state,
								   game_struct2);
				    if (game_state2)
				      {
					checksum_state2 =
					  lw6ker_game_state_checksum
					  (game_state2);
					hexa_state2 =
					  lw6ker_game_state_to_hexa
					  (game_state2);
					if (hexa_state2)
					  {
					    lw6sys_log (LW6SYS_LOG_NOTICE,
							_x_
							("hexa_state length=%d hexa_state2 length=%d checksum_state=%08x checksum_state2=%08x"),
							(int)
							strlen (hexa_state),
							(int)
							strlen (hexa_state2),
							checksum_state,
							checksum_state2);
					    if (checksum_state ==
						checksum_state2
						&&
						lw6sys_str_is_same
						(hexa_state, hexa_state2))
					      {
						ret = 1;
					      }
					    else
					      {
						if (checksum_state !=
						    checksum_state2)
						  {
						    lw6sys_log
						      (LW6SYS_LOG_WARNING,
						       _x_
						       ("game_state checksums differ"));
						  }
						if (!lw6sys_str_is_same
						    (hexa_state, hexa_state2))
						  {
						    lw6sys_log
						      (LW6SYS_LOG_WARNING,
						       _x_
						       ("game_state hexa dumps differ"));
						  }
						ret = 0;
					      }
					    LW6SYS_FREE (hexa_state2);
					  }
					lw6ker_game_state_free (game_state2);
					game_state2 = NULL;
				      }
				    LW6SYS_FREE (hexa_state);
				  }
			      }
			  }
			else
			  {
			    if (checksum_struct != checksum_struct2)
			      {
				lw6sys_log (LW6SYS_LOG_WARNING,
					    _x_
					    ("game_struct checksums differ"));
			      }
			    if (!lw6sys_str_is_same
				(hexa_struct, hexa_struct2))
			      {
				lw6sys_log (LW6SYS_LOG_WARNING,
					    _x_
					    ("game_struct hexa dumps differ"));
			      }
			    ret = 0;
			  }
			LW6SYS_FREE (hexa_struct2);
		      }
		    lw6ker_game_struct_free (game_struct2);
		    game_struct2 = NULL;
		  }
		LW6SYS_FREE (hexa_struct);
	      }
	  }
	lw6map_free (level);
	level = NULL;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/**
 * lw6ker_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @ker module test suite. Will perform deep checksums
 * and *really* check many things. If this passes, the algorithm
 * is fine. What could make it fail is a serious bug and/or some
 * weird combination of endianess, byte alignment...
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6ker_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6map_test (mode);
    }

  ret = test_team_mask () && test_struct () && test_state ()
    && test_population () && test_algorithm () && test_dup () && test_hexa ();

  return ret;
}
