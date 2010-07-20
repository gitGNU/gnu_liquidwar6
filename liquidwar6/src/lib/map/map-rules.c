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

#include "map.h"

char *LW6MAP_RULES_LIST[] = {
  LW6DEF_TOTAL_TIME,
  LW6DEF_RESPAWN_TEAM,
  LW6DEF_RESPAWN_RANDOM_PLACE,
  LW6DEF_MOVES_PER_ROUND,
  LW6DEF_SPREADS_PER_ROUND,
  LW6DEF_ROUNDS_PER_SEC,
  LW6DEF_FIGHTER_ATTACK,
  LW6DEF_FIGHTER_DEFENSE,
  LW6DEF_FIGHTER_NEW_HEALTH,
  LW6DEF_FIGHTER_REGENERATE,
  LW6DEF_SIDE_ATTACK_FACTOR,
  LW6DEF_SIDE_DEFENSE_FACTOR,
  LW6DEF_NB_MOVE_TRIES,
  LW6DEF_NB_ATTACK_TRIES,
  LW6DEF_NB_DEFENSE_TRIES,
  LW6DEF_VERTICAL_MOVE,
  LW6DEF_SINGLE_ARMY_SIZE,
  LW6DEF_TOTAL_ARMIES_SIZE,
  LW6DEF_MAX_NB_TEAMS,
  LW6DEF_MAX_NB_CURSORS,
  LW6DEF_MAX_NB_NODES,
  LW6DEF_X_POLARITY,
  LW6DEF_Y_POLARITY,
  LW6DEF_Z_POLARITY,
  LW6DEF_MAX_ZONE_SIZE,
  LW6DEF_ROUND_DELTA,
  LW6DEF_MAX_ROUND_DELTA,
  LW6DEF_MAX_CURSOR_POT,
  LW6DEF_CURSOR_POT_INIT,
  LW6DEF_MAX_CURSOR_POT_OFFSET,
  LW6DEF_START_RED_X,
  LW6DEF_START_RED_Y,
  LW6DEF_START_GREEN_X,
  LW6DEF_START_GREEN_Y,
  LW6DEF_START_BLUE_X,
  LW6DEF_START_BLUE_Y,
  LW6DEF_START_YELLOW_X,
  LW6DEF_START_YELLOW_Y,
  LW6DEF_START_CYAN_X,
  LW6DEF_START_CYAN_Y,
  LW6DEF_START_MAGENTA_X,
  LW6DEF_START_MAGENTA_Y,
  LW6DEF_START_ORANGE_X,
  LW6DEF_START_ORANGE_Y,
  LW6DEF_START_LIGHTBLUE_X,
  LW6DEF_START_LIGHTBLUE_Y,
  LW6DEF_START_PURPLE_X,
  LW6DEF_START_PURPLE_Y,
  LW6DEF_START_PINK_X,
  LW6DEF_START_PINK_Y,
  LW6DEF_START_POSITION_MODE,
  LW6DEF_COLOR_CONFLICT_MODE,
  LW6DEF_SPREAD_THREAD,
  LW6DEF_GLUE_POWER,
  LW6DEF_BOOST_POWER,
  LW6DEF_DANGER_POWER,
  LW6DEF_MEDICINE_POWER,
  NULL				// very important that this list is NULL-terminated
};

static lw6map_rules_t default_rules = {
  LW6MAP_RULES_DEFAULT_TOTAL_TIME,
  LW6MAP_RULES_DEFAULT_RESPAWN_TEAM,
  LW6MAP_RULES_DEFAULT_RESPAWN_RANDOM_PLACE,
  LW6MAP_RULES_DEFAULT_MOVES_PER_ROUND,
  LW6MAP_RULES_DEFAULT_SPREADS_PER_ROUND,
  LW6MAP_RULES_DEFAULT_ROUNDS_PER_SEC,
  LW6MAP_RULES_DEFAULT_FIGHTER_ATTACK,
  LW6MAP_RULES_DEFAULT_FIGHTER_DEFENSE,
  LW6MAP_RULES_DEFAULT_FIGHTER_NEW_HEALTH,
  LW6MAP_RULES_DEFAULT_FIGHTER_REGENERATE,
  LW6MAP_RULES_DEFAULT_SIDE_ATTACK_FACTOR,
  LW6MAP_RULES_DEFAULT_SIDE_DEFENSE_FACTOR,
  LW6MAP_RULES_DEFAULT_NB_MOVE_TRIES,
  LW6MAP_RULES_DEFAULT_NB_ATTACK_TRIES,
  LW6MAP_RULES_DEFAULT_NB_DEFENSE_TRIES,
  LW6MAP_RULES_DEFAULT_VERTICAL_MOVE,
  LW6MAP_RULES_DEFAULT_SINGLE_ARMY_SIZE,
  LW6MAP_RULES_DEFAULT_TOTAL_ARMIES_SIZE,
  LW6MAP_RULES_DEFAULT_MAX_NB_TEAMS,
  LW6MAP_RULES_DEFAULT_MAX_NB_CURSORS,
  LW6MAP_RULES_DEFAULT_MAX_NB_NODES,
  LW6MAP_RULES_DEFAULT_X_POLARITY,
  LW6MAP_RULES_DEFAULT_Y_POLARITY,
  LW6MAP_RULES_DEFAULT_Z_POLARITY,
  LW6MAP_RULES_DEFAULT_MAX_ZONE_SIZE,
  LW6MAP_RULES_DEFAULT_ROUND_DELTA,
  LW6MAP_RULES_DEFAULT_MAX_ROUND_DELTA,
  LW6MAP_RULES_DEFAULT_MAX_CURSOR_POT,
  LW6MAP_RULES_DEFAULT_CURSOR_POT_INIT,
  LW6MAP_RULES_DEFAULT_MAX_CURSOR_POT_OFFSET,
  LW6MAP_RULES_DEFAULT_START_RED_X,
  LW6MAP_RULES_DEFAULT_START_RED_Y,
  LW6MAP_RULES_DEFAULT_START_GREEN_X,
  LW6MAP_RULES_DEFAULT_START_GREEN_Y,
  LW6MAP_RULES_DEFAULT_START_BLUE_X,
  LW6MAP_RULES_DEFAULT_START_BLUE_Y,
  LW6MAP_RULES_DEFAULT_START_YELLOW_X,
  LW6MAP_RULES_DEFAULT_START_YELLOW_Y,
  LW6MAP_RULES_DEFAULT_START_CYAN_X,
  LW6MAP_RULES_DEFAULT_START_CYAN_Y,
  LW6MAP_RULES_DEFAULT_START_MAGENTA_X,
  LW6MAP_RULES_DEFAULT_START_MAGENTA_Y,
  LW6MAP_RULES_DEFAULT_START_ORANGE_X,
  LW6MAP_RULES_DEFAULT_START_ORANGE_Y,
  LW6MAP_RULES_DEFAULT_START_LIGHTBLUE_X,
  LW6MAP_RULES_DEFAULT_START_LIGHTBLUE_Y,
  LW6MAP_RULES_DEFAULT_START_PURPLE_X,
  LW6MAP_RULES_DEFAULT_START_PURPLE_Y,
  LW6MAP_RULES_DEFAULT_START_PINK_X,
  LW6MAP_RULES_DEFAULT_START_PINK_Y,
  LW6MAP_RULES_DEFAULT_START_POSITION_MODE,
  LW6MAP_RULES_DEFAULT_COLOR_CONFLICT_MODE,
  LW6MAP_RULES_DEFAULT_SPREAD_THREAD,
  LW6MAP_RULES_DEFAULT_GLUE_POWER,
  LW6MAP_RULES_DEFAULT_BOOST_POWER,
  LW6MAP_RULES_DEFAULT_DANGER_POWER,
  LW6MAP_RULES_DEFAULT_MEDICINE_POWER
};

static lw6map_rules_t min_rules = {
  LW6MAP_RULES_MIN_TOTAL_TIME,
  LW6MAP_RULES_MIN_RESPAWN_TEAM,
  LW6MAP_RULES_MIN_RESPAWN_RANDOM_PLACE,
  LW6MAP_RULES_MIN_MOVES_PER_ROUND,
  LW6MAP_RULES_MIN_SPREADS_PER_ROUND,
  LW6MAP_RULES_MIN_ROUNDS_PER_SEC,
  LW6MAP_RULES_MIN_FIGHTER_ATTACK,
  LW6MAP_RULES_MIN_FIGHTER_DEFENSE,
  LW6MAP_RULES_MIN_FIGHTER_NEW_HEALTH,
  LW6MAP_RULES_MIN_FIGHTER_REGENERATE,
  LW6MAP_RULES_MIN_SIDE_ATTACK_FACTOR,
  LW6MAP_RULES_MIN_SIDE_DEFENSE_FACTOR,
  LW6MAP_RULES_MIN_NB_MOVE_TRIES,
  LW6MAP_RULES_MIN_NB_ATTACK_TRIES,
  LW6MAP_RULES_MIN_NB_DEFENSE_TRIES,
  LW6MAP_RULES_MIN_VERTICAL_MOVE,
  LW6MAP_RULES_MIN_SINGLE_ARMY_SIZE,
  LW6MAP_RULES_MIN_TOTAL_ARMIES_SIZE,
  LW6MAP_RULES_MIN_MAX_NB_TEAMS,
  LW6MAP_RULES_MIN_MAX_NB_CURSORS,
  LW6MAP_RULES_MIN_MAX_NB_NODES,
  LW6MAP_RULES_MIN_X_POLARITY,
  LW6MAP_RULES_MIN_Y_POLARITY,
  LW6MAP_RULES_MIN_Z_POLARITY,
  LW6MAP_RULES_MIN_MAX_ZONE_SIZE,
  LW6MAP_RULES_MIN_ROUND_DELTA,
  LW6MAP_RULES_MIN_MAX_ROUND_DELTA,
  LW6MAP_RULES_MIN_MAX_CURSOR_POT,
  LW6MAP_RULES_MIN_CURSOR_POT_INIT,
  LW6MAP_RULES_MIN_MAX_CURSOR_POT_OFFSET,
  LW6MAP_RULES_MIN_START_RED_X,
  LW6MAP_RULES_MIN_START_RED_Y,
  LW6MAP_RULES_MIN_START_GREEN_X,
  LW6MAP_RULES_MIN_START_GREEN_Y,
  LW6MAP_RULES_MIN_START_BLUE_X,
  LW6MAP_RULES_MIN_START_BLUE_Y,
  LW6MAP_RULES_MIN_START_YELLOW_X,
  LW6MAP_RULES_MIN_START_YELLOW_Y,
  LW6MAP_RULES_MIN_START_CYAN_X,
  LW6MAP_RULES_MIN_START_CYAN_Y,
  LW6MAP_RULES_MIN_START_MAGENTA_X,
  LW6MAP_RULES_MIN_START_MAGENTA_Y,
  LW6MAP_RULES_MIN_START_ORANGE_X,
  LW6MAP_RULES_MIN_START_ORANGE_Y,
  LW6MAP_RULES_MIN_START_LIGHTBLUE_X,
  LW6MAP_RULES_MIN_START_LIGHTBLUE_Y,
  LW6MAP_RULES_MIN_START_PURPLE_X,
  LW6MAP_RULES_MIN_START_PURPLE_Y,
  LW6MAP_RULES_MIN_START_PINK_X,
  LW6MAP_RULES_MIN_START_PINK_Y,
  LW6MAP_RULES_MIN_START_POSITION_MODE,
  LW6MAP_RULES_MIN_COLOR_CONFLICT_MODE,
  LW6MAP_RULES_MIN_SPREAD_THREAD,
  LW6MAP_RULES_MIN_GLUE_POWER,
  LW6MAP_RULES_MIN_BOOST_POWER,
  LW6MAP_RULES_MIN_DANGER_POWER,
  LW6MAP_RULES_MIN_MEDICINE_POWER
};

static lw6map_rules_t max_rules = {
  LW6MAP_RULES_MAX_TOTAL_TIME,
  LW6MAP_RULES_MAX_RESPAWN_TEAM,
  LW6MAP_RULES_MAX_RESPAWN_RANDOM_PLACE,
  LW6MAP_RULES_MAX_MOVES_PER_ROUND,
  LW6MAP_RULES_MAX_SPREADS_PER_ROUND,
  LW6MAP_RULES_MAX_ROUNDS_PER_SEC,
  LW6MAP_RULES_MAX_FIGHTER_ATTACK,
  LW6MAP_RULES_MAX_FIGHTER_DEFENSE,
  LW6MAP_RULES_MAX_FIGHTER_NEW_HEALTH,
  LW6MAP_RULES_MAX_FIGHTER_REGENERATE,
  LW6MAP_RULES_MAX_SIDE_ATTACK_FACTOR,
  LW6MAP_RULES_MAX_SIDE_DEFENSE_FACTOR,
  LW6MAP_RULES_MAX_NB_MOVE_TRIES,
  LW6MAP_RULES_MAX_NB_ATTACK_TRIES,
  LW6MAP_RULES_MAX_NB_DEFENSE_TRIES,
  LW6MAP_RULES_MAX_VERTICAL_MOVE,
  LW6MAP_RULES_MAX_SINGLE_ARMY_SIZE,
  LW6MAP_RULES_MAX_TOTAL_ARMIES_SIZE,
  LW6MAP_RULES_MAX_MAX_NB_TEAMS,
  LW6MAP_RULES_MAX_MAX_NB_CURSORS,
  LW6MAP_RULES_MAX_MAX_NB_NODES,
  LW6MAP_RULES_MAX_X_POLARITY,
  LW6MAP_RULES_MAX_Y_POLARITY,
  LW6MAP_RULES_MAX_Z_POLARITY,
  LW6MAP_RULES_MAX_MAX_ZONE_SIZE,
  LW6MAP_RULES_MAX_ROUND_DELTA,
  LW6MAP_RULES_MAX_MAX_ROUND_DELTA,
  LW6MAP_RULES_MAX_MAX_CURSOR_POT,
  LW6MAP_RULES_MAX_CURSOR_POT_INIT,
  LW6MAP_RULES_MAX_MAX_CURSOR_POT_OFFSET,
  LW6MAP_RULES_MAX_START_RED_X,
  LW6MAP_RULES_MAX_START_RED_Y,
  LW6MAP_RULES_MAX_START_GREEN_X,
  LW6MAP_RULES_MAX_START_GREEN_Y,
  LW6MAP_RULES_MAX_START_BLUE_X,
  LW6MAP_RULES_MAX_START_BLUE_Y,
  LW6MAP_RULES_MAX_START_YELLOW_X,
  LW6MAP_RULES_MAX_START_YELLOW_Y,
  LW6MAP_RULES_MAX_START_CYAN_X,
  LW6MAP_RULES_MAX_START_CYAN_Y,
  LW6MAP_RULES_MAX_START_MAGENTA_X,
  LW6MAP_RULES_MAX_START_MAGENTA_Y,
  LW6MAP_RULES_MAX_START_ORANGE_X,
  LW6MAP_RULES_MAX_START_ORANGE_Y,
  LW6MAP_RULES_MAX_START_LIGHTBLUE_X,
  LW6MAP_RULES_MAX_START_LIGHTBLUE_Y,
  LW6MAP_RULES_MAX_START_PURPLE_X,
  LW6MAP_RULES_MAX_START_PURPLE_Y,
  LW6MAP_RULES_MAX_START_PINK_X,
  LW6MAP_RULES_MAX_START_PINK_Y,
  LW6MAP_RULES_MAX_START_POSITION_MODE,
  LW6MAP_RULES_MAX_COLOR_CONFLICT_MODE,
  LW6MAP_RULES_MAX_SPREAD_THREAD,
  LW6MAP_RULES_MAX_GLUE_POWER,
  LW6MAP_RULES_MAX_BOOST_POWER,
  LW6MAP_RULES_MAX_DANGER_POWER,
  LW6MAP_RULES_MAX_MEDICINE_POWER
};

void
lw6map_rules_defaults (lw6map_rules_t * rules)
{
  lw6map_rules_copy (rules, &default_rules);
}

void
lw6map_rules_copy (lw6map_rules_t * dst, lw6map_rules_t * src)
{
  /*
   * Note that in this context, we'd better not use strings
   * in this structure, or code would become a bit more complex.
   */
  memcpy (dst, src, sizeof (lw6map_rules_t));
}

void
lw6map_rules_update_checksum (lw6map_rules_t * rules, u_int32_t * checksum)
{
  lw6sys_checksum_update_int32 (checksum, rules->total_time);
  lw6sys_checksum_update_int32 (checksum, rules->respawn_team);
  lw6sys_checksum_update_int32 (checksum, rules->respawn_random_place);
  lw6sys_checksum_update_int32 (checksum, rules->moves_per_round);
  lw6sys_checksum_update_int32 (checksum, rules->spreads_per_round);
  lw6sys_checksum_update_int32 (checksum, rules->rounds_per_sec);
  lw6sys_checksum_update_int32 (checksum, rules->fighter_attack);
  lw6sys_checksum_update_int32 (checksum, rules->fighter_defense);
  lw6sys_checksum_update_int32 (checksum, rules->fighter_new_health);
  lw6sys_checksum_update_int32 (checksum, rules->fighter_regenerate);
  lw6sys_checksum_update_int32 (checksum, rules->side_attack_factor);
  lw6sys_checksum_update_int32 (checksum, rules->side_defense_factor);
  lw6sys_checksum_update_int32 (checksum, rules->nb_move_tries);
  lw6sys_checksum_update_int32 (checksum, rules->nb_attack_tries);
  lw6sys_checksum_update_int32 (checksum, rules->nb_defense_tries);
  lw6sys_checksum_update_int32 (checksum, rules->vertical_move);
  lw6sys_checksum_update_int32 (checksum, rules->single_army_size);
  lw6sys_checksum_update_int32 (checksum, rules->total_armies_size);
  lw6sys_checksum_update_int32 (checksum, rules->max_nb_teams);
  lw6sys_checksum_update_int32 (checksum, rules->max_nb_cursors);
  lw6sys_checksum_update_int32 (checksum, rules->max_nb_nodes);
  lw6sys_checksum_update_int32 (checksum, rules->x_polarity);
  lw6sys_checksum_update_int32 (checksum, rules->y_polarity);
  lw6sys_checksum_update_int32 (checksum, rules->z_polarity);
  lw6sys_checksum_update_int32 (checksum, rules->max_zone_size);
  lw6sys_checksum_update_int32 (checksum, rules->round_delta);
  lw6sys_checksum_update_int32 (checksum, rules->max_round_delta);
  lw6sys_checksum_update_int32 (checksum, rules->max_cursor_pot);
  lw6sys_checksum_update_int32 (checksum, rules->cursor_pot_init);
  lw6sys_checksum_update_int32 (checksum, rules->max_cursor_pot_offset);
  lw6sys_checksum_update_int32 (checksum, rules->start_red_x);
  lw6sys_checksum_update_int32 (checksum, rules->start_red_y);
  lw6sys_checksum_update_int32 (checksum, rules->start_green_x);
  lw6sys_checksum_update_int32 (checksum, rules->start_green_y);
  lw6sys_checksum_update_int32 (checksum, rules->start_blue_x);
  lw6sys_checksum_update_int32 (checksum, rules->start_blue_y);
  lw6sys_checksum_update_int32 (checksum, rules->start_yellow_x);
  lw6sys_checksum_update_int32 (checksum, rules->start_yellow_y);
  lw6sys_checksum_update_int32 (checksum, rules->start_cyan_x);
  lw6sys_checksum_update_int32 (checksum, rules->start_cyan_y);
  lw6sys_checksum_update_int32 (checksum, rules->start_magenta_x);
  lw6sys_checksum_update_int32 (checksum, rules->start_magenta_y);
  lw6sys_checksum_update_int32 (checksum, rules->start_orange_x);
  lw6sys_checksum_update_int32 (checksum, rules->start_orange_y);
  lw6sys_checksum_update_int32 (checksum, rules->start_lightblue_x);
  lw6sys_checksum_update_int32 (checksum, rules->start_lightblue_y);
  lw6sys_checksum_update_int32 (checksum, rules->start_purple_x);
  lw6sys_checksum_update_int32 (checksum, rules->start_purple_y);
  lw6sys_checksum_update_int32 (checksum, rules->start_pink_x);
  lw6sys_checksum_update_int32 (checksum, rules->start_pink_y);
  lw6sys_checksum_update_int32 (checksum, rules->start_position_mode);
  lw6sys_checksum_update_int32 (checksum, rules->color_conflict_mode);
  lw6sys_checksum_update_int32 (checksum, rules->spread_thread);
  lw6sys_checksum_update_int32 (checksum, rules->glue_power);
  lw6sys_checksum_update_int32 (checksum, rules->boost_power);
  lw6sys_checksum_update_int32 (checksum, rules->danger_power);
  lw6sys_checksum_update_int32 (checksum, rules->medicine_power);
}

int32_t *
get_rules_int_ptr (lw6map_rules_t * rules, char *key)
{
  int32_t *ret = NULL;
  char *formatted_key = NULL;

  if (rules && key)
    {
      formatted_key = lw6sys_keyword_as_key (key);
      if (formatted_key)
	{
	  if (!strcmp (formatted_key, LW6DEF_TOTAL_TIME))
	    {
	      ret = &(rules->total_time);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_RESPAWN_TEAM))
	    {
	      ret = &(rules->respawn_team);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_RESPAWN_RANDOM_PLACE))
	    {
	      ret = &(rules->respawn_random_place);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_MOVES_PER_ROUND))
	    {
	      ret = &(rules->moves_per_round);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_SPREADS_PER_ROUND))
	    {
	      ret = &(rules->spreads_per_round);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_ROUNDS_PER_SEC))
	    {
	      ret = &(rules->rounds_per_sec);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_FIGHTER_ATTACK))
	    {
	      ret = &(rules->fighter_attack);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_FIGHTER_DEFENSE))
	    {
	      ret = &(rules->fighter_defense);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_FIGHTER_NEW_HEALTH))
	    {
	      ret = &(rules->fighter_new_health);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_FIGHTER_REGENERATE))
	    {
	      ret = &(rules->fighter_regenerate);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_SIDE_ATTACK_FACTOR))
	    {
	      ret = &(rules->side_attack_factor);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_SIDE_DEFENSE_FACTOR))
	    {
	      ret = &(rules->side_defense_factor);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_NB_MOVE_TRIES))
	    {
	      ret = &(rules->nb_move_tries);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_NB_ATTACK_TRIES))
	    {
	      ret = &(rules->nb_attack_tries);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_NB_DEFENSE_TRIES))
	    {
	      ret = &(rules->nb_defense_tries);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_VERTICAL_MOVE))
	    {
	      ret = &(rules->vertical_move);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_SINGLE_ARMY_SIZE))
	    {
	      ret = &(rules->single_army_size);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_TOTAL_ARMIES_SIZE))
	    {
	      ret = &(rules->total_armies_size);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_MAX_NB_TEAMS))
	    {
	      ret = &(rules->max_nb_teams);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_MAX_NB_CURSORS))
	    {
	      ret = &(rules->max_nb_cursors);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_MAX_NB_NODES))
	    {
	      ret = &(rules->max_nb_nodes);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_X_POLARITY))
	    {
	      ret = &(rules->x_polarity);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_Y_POLARITY))
	    {
	      ret = &(rules->y_polarity);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_Z_POLARITY))
	    {
	      ret = &(rules->z_polarity);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_MAX_ZONE_SIZE))
	    {
	      ret = &(rules->max_zone_size);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_ROUND_DELTA))
	    {
	      ret = &(rules->round_delta);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_MAX_ROUND_DELTA))
	    {
	      ret = &(rules->max_round_delta);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_MAX_CURSOR_POT))
	    {
	      ret = &(rules->max_cursor_pot);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_CURSOR_POT_INIT))
	    {
	      ret = &(rules->cursor_pot_init);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_MAX_CURSOR_POT_OFFSET))
	    {
	      ret = &(rules->max_cursor_pot_offset);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_RED_X))
	    {
	      ret = &(rules->start_red_x);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_RED_Y))
	    {
	      ret = &(rules->start_red_y);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_GREEN_X))
	    {
	      ret = &(rules->start_green_x);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_GREEN_Y))
	    {
	      ret = &(rules->start_green_y);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_BLUE_X))
	    {
	      ret = &(rules->start_blue_x);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_BLUE_Y))
	    {
	      ret = &(rules->start_blue_y);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_YELLOW_X))
	    {
	      ret = &(rules->start_yellow_x);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_YELLOW_Y))
	    {
	      ret = &(rules->start_yellow_y);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_CYAN_X))
	    {
	      ret = &(rules->start_cyan_x);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_CYAN_Y))
	    {
	      ret = &(rules->start_cyan_y);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_MAGENTA_X))
	    {
	      ret = &(rules->start_magenta_x);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_MAGENTA_Y))
	    {
	      ret = &(rules->start_magenta_y);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_ORANGE_X))
	    {
	      ret = &(rules->start_orange_x);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_ORANGE_Y))
	    {
	      ret = &(rules->start_orange_y);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_LIGHTBLUE_X))
	    {
	      ret = &(rules->start_lightblue_x);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_LIGHTBLUE_Y))
	    {
	      ret = &(rules->start_lightblue_y);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_PURPLE_X))
	    {
	      ret = &(rules->start_purple_x);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_PURPLE_Y))
	    {
	      ret = &(rules->start_purple_y);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_PINK_X))
	    {
	      ret = &(rules->start_pink_x);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_PINK_Y))
	    {
	      ret = &(rules->start_pink_y);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_START_POSITION_MODE))
	    {
	      ret = &(rules->start_position_mode);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_COLOR_CONFLICT_MODE))
	    {
	      ret = &(rules->color_conflict_mode);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_SPREAD_THREAD))
	    {
	      ret = &(rules->spread_thread);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_GLUE_POWER))
	    {
	      ret = &(rules->glue_power);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_BOOST_POWER))
	    {
	      ret = &(rules->boost_power);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_DANGER_POWER))
	    {
	      ret = &(rules->danger_power);
	    }
	  else if (!strcmp (formatted_key, LW6DEF_MEDICINE_POWER))
	    {
	      ret = &(rules->medicine_power);
	    }
	  else
	    {
	      lw6sys_log (LW6SYS_LOG_WARNING,
			  _
			  ("static rules key \"%s\" does not exist, can't get a pointer on its value"),
			  key);
	    }
	  LW6SYS_FREE (formatted_key);
	}
    }

  return ret;
}

int32_t
lw6map_rules_get_default (char *key)
{
  int32_t *ptr;
  int32_t ret = 0;

  ptr = get_rules_int_ptr (&default_rules, key);
  if (ptr)
    {
      ret = (*ptr);
    }

  return ret;
}

int32_t
lw6map_rules_get_min (char *key)
{
  int32_t *ptr;
  int32_t ret = 0;

  ptr = get_rules_int_ptr (&min_rules, key);
  if (ptr)
    {
      ret = (*ptr);
    }

  return ret;
}

int32_t
lw6map_rules_get_max (char *key)
{
  int32_t *ptr;
  int32_t ret = 0;

  ptr = get_rules_int_ptr (&max_rules, key);
  if (ptr)
    {
      ret = (*ptr);
    }

  return ret;
}

int32_t
lw6map_rules_get_int (lw6map_rules_t * rules, char *key)
{
  int32_t ret = 0;
  int32_t *ptr;
  int32_t min_value;
  int32_t max_value;

  ptr = get_rules_int_ptr (rules, key);
  if (ptr)
    {
      min_value = lw6map_rules_get_min (key);
      max_value = lw6map_rules_get_max (key);
      if ((*ptr) < min_value)
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("can't get static option \"%s\" (%d), it's too small, returning min value %d instead"),
		      key, (*ptr), min_value);
	  ret = min_value;
	}
      else if ((*ptr) > max_value)
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("can't get static option \"%s\" (%d), it's too big, returning max value %d instead"),
		      key, (*ptr), max_value);
	  ret = max_value;
	}
      else
	{
	  ret = (*ptr);
	}
    }

  return ret;
}

int
lw6map_rules_set_int (lw6map_rules_t * rules, char *key, int32_t value)
{
  int32_t *ptr;
  int32_t min_value;
  int32_t max_value;
  int ret = 0;

  ptr = get_rules_int_ptr (rules, key);
  if (ptr)
    {
      min_value = lw6map_rules_get_min (key);
      max_value = lw6map_rules_get_max (key);
      if (value < min_value)
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("can't set static option \"%s\" to value %d, it's too small, using min value %d"),
		      key, value, min_value);
	  (*ptr) = min_value;
	}
      else if (value > max_value)
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _
		      ("can't set static option \"%s\" to value %d, it's too big, using max value %d"),
		      key, value, max_value);
	  (*ptr) = max_value;
	}
      else
	{
	  (*ptr) = value;
	}
      ret = 1;
    }

  return ret;
}

int
lw6map_rules_get_bool (lw6map_rules_t * rules, char *key)
{
  return lw6map_rules_get_int (rules, key) ? 1 : 0;
}

int
lw6map_rules_set_bool (lw6map_rules_t * rules, char *key, int value)
{
  return lw6map_rules_set_int (rules, key, value ? 1 : 0);
}

void
lw6map_rules_clear (lw6map_rules_t * rules)
{
  if (rules)
    {
      memset (rules, 0, sizeof (lw6map_rules_t));
    }
}

int
lw6map_rules_is_same (lw6map_rules_t * rules_a, lw6map_rules_t * rules_b)
{
  int ret = 1;

  ret = !memcmp ((char *) rules_a, (char *) rules_b, sizeof (lw6map_rules_t));

  return ret;
}
