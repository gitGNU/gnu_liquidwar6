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

#include "../map/map.h"

#include "hlp.h"
#include "hlp-internal.h"

int
_lw6hlp_populate_references_map_rules (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 0;

  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TOTAL_TIME,
			_x_
			("Defines the maximum time of the game, in seconds. Note that in some cases, the game can end much earlier if some player has managed to win before the bell rings. Also, technically, this value will be translated into rounds and moves, and the game engine will wait until enough rounds and moves have been played. So if the computer is too slow and the desired speed is not reached, then the game will last for a longer time."),
			LW6MAP_RULES_DEFAULT_TOTAL_TIME, LW6MAP_RULES_MIN_TOTAL_TIME, LW6MAP_RULES_MAX_TOTAL_TIME);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_RESPAWN_TEAM,
			_x_
			("Defines what to do when a team dies. If set to 0, team disappears forever, if set to 1, team reappears automatically with fresh fighters. It's a deathmatch mode, where the winner is not the one who stays alive the longest time, since it makes no real sens in this case, but the one who has died less often than others."),
			LW6MAP_RULES_DEFAULT_RESPAWN_TEAM, LW6MAP_RULES_MIN_RESPAWN_TEAM, LW6MAP_RULES_MAX_RESPAWN_TEAM);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_RESPAWN_POSITION_MODE,
			_x_
			("Defines how teams are set up on the map when respawning. 0 means teams respect the pre-defined start positions. 1 means that a random position will be picked, among the existing positions. That is, red could take green's place. 2 means total randomness, teams can appear anywhere."),
			LW6MAP_RULES_DEFAULT_RESPAWN_POSITION_MODE, LW6MAP_RULES_MIN_RESPAWN_POSITION_MODE, LW6MAP_RULES_MAX_RESPAWN_POSITION_MODE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_RESPAWN_DELAY,
			_x_
			("Delay, in seconds, after which teams reappear on the battlefield, when in deathmatch mode. 0 means team right away."),
			LW6MAP_RULES_DEFAULT_RESPAWN_DELAY, LW6MAP_RULES_MIN_RESPAWN_DELAY, LW6MAP_RULES_MAX_RESPAWN_DELAY);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MOVES_PER_ROUND,
			_x_
			("Defines how many times fighters move per round. Increasing this will just make fighters move faster, but won't change anything for the rest, that is keyboard and mouse responsivity, and network traffic will stay the same. Multiplying the number of moves per round by the number of rounds per second will give the number of moves per second, which is, in fact, how fast fighters move on the screen."),
			LW6MAP_RULES_DEFAULT_MOVES_PER_ROUND, LW6MAP_RULES_MIN_MOVES_PER_ROUND, LW6MAP_RULES_MAX_MOVES_PER_ROUND);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_SPREADS_PER_ROUND,
			_x_
			("Defines how many times the gradient is spread per round. Gradient spread is a very Liquid War 6 specific feature, just remember that the more often you do it, the more accurately fighters will move. That is, you will be sure they really take the shortest path. Usually this does not have much effect, the default value should fit in most cases, but you might want to decrease it on very simple maps where the gradient is obvious, or increase it on complex maps where you want fighters to be real smart."),
			LW6MAP_RULES_DEFAULT_SPREADS_PER_ROUND, LW6MAP_RULES_MIN_SPREADS_PER_ROUND, LW6MAP_RULES_MAX_SPREADS_PER_ROUND);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_ROUNDS_PER_SEC,
			_x_
			("Defines the overall speed of the game. All other settings being equal, raising this value will cause the game to behave faster. Everything will be faster, except probably the display since your computer will calculate more game positions in a given time and spend more CPU time. It will also increase network traffic. Values between 10 and 50 really make sense."),
			LW6MAP_RULES_DEFAULT_ROUNDS_PER_SEC, LW6MAP_RULES_MIN_ROUNDS_PER_SEC, LW6MAP_RULES_MAX_ROUNDS_PER_SEC);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_FIGHTER_ATTACK,
			_x_
			("Defines how hard fighters will attack others, that is, in one attack, how many life-points the attacked fighter will loose. Increasing this will cause your opponents to melt faster when you attack them. With a low value, it will take ages to take on your opponents. Different styles of game. Can radically change the gameplay."),
			LW6MAP_RULES_DEFAULT_FIGHTER_ATTACK, LW6MAP_RULES_MIN_FIGHTER_ATTACK, LW6MAP_RULES_MAX_FIGHTER_ATTACK);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_FIGHTER_DEFENSE,
			_x_
			("Defines how fast fighters will regenerate after an attack. When this parameter is set low, an attacked fighter, which is very dark and almost dead will take a very long time to regain energy. If the parameter is set high, it can almost instantaneously regain energy."),
			LW6MAP_RULES_DEFAULT_FIGHTER_DEFENSE, LW6MAP_RULES_MIN_FIGHTER_DEFENSE, LW6MAP_RULES_MAX_FIGHTER_DEFENSE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_FIGHTER_NEW_HEALTH,
			_x_
			("Defines how healthy fighters will be when they appear on the map. This can be either at the beginning of the game of when a fighter changes team. Setting this low will allow battefields to switch from one side to another very fast, for freshly gained fighters will be feeble and very likely to return to their original camp. To calibrate this parameter, keep in mind that the absolute maximum health a fighter can have is always 10000 (ten-thousands)."),
			LW6MAP_RULES_DEFAULT_FIGHTER_NEW_HEALTH, LW6MAP_RULES_MIN_FIGHTER_NEW_HEALTH, LW6MAP_RULES_MAX_FIGHTER_NEW_HEALTH);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_FIGHTER_REGENERATE,
			_x_
			("Defines at which speed fighters will self-regenerate, without even begin packed together. This will allow lone fighters to regenerate a bit by hiding somewhere in the map. This is typically a low value, might even be 0."),
			LW6MAP_RULES_DEFAULT_FIGHTER_REGENERATE, LW6MAP_RULES_MIN_FIGHTER_REGENERATE, LW6MAP_RULES_MAX_FIGHTER_REGENERATE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_SIDE_ATTACK_FACTOR,
			_x_
			("Defines how hard fighters will attack sideways. It's an algorithm trick, fighters attack by default the opponent right in front, but if there's no fighter there, they will still try to attack someone else, maybe sideways. But doing this their attack is not as strong. This parameter enables you to tune this. This is a percentage."),
			LW6MAP_RULES_DEFAULT_SIDE_ATTACK_FACTOR, LW6MAP_RULES_MIN_SIDE_ATTACK_FACTOR, LW6MAP_RULES_MAX_SIDE_ATTACK_FACTOR);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_SIDE_DEFENSE_FACTOR,
			_x_
			("Defines how fast fighters will regenerate, when being side by side instead of being right in front of the other. This is a percentage."),
			LW6MAP_RULES_DEFAULT_SIDE_DEFENSE_FACTOR, LW6MAP_RULES_MIN_SIDE_DEFENSE_FACTOR, LW6MAP_RULES_MAX_SIDE_DEFENSE_FACTOR);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_NB_MOVE_TRIES,
			_x_
			("Defines how many tries a fighter will do before giving-up moving and choosing another behvior (attack or defense). By tries we mean: how many directions it will try. Going North? Going North-West? Setting this to a low value, your fighters will look very stubborn and always try to move in one direction, neglecting the fact that they could dodge. This can lead to queues of fighters and other strange behaviors. On the other hand, setting it too high will cause fighter to always avoid the enemy, and groups of fighters will just pass each other without any fight. Matter of taste."),
			LW6MAP_RULES_DEFAULT_NB_MOVE_TRIES, LW6MAP_RULES_MIN_NB_MOVE_TRIES, LW6MAP_RULES_MAX_NB_MOVE_TRIES);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_NB_ATTACK_TRIES,
			_x_
			("Defines how many tries a fighter will do before giving-up attacking and choosing another behvior (defense). By tries we mean: how many directions it will try. Going North? Going North-West? Setting this to a low value will make fighters somewhat less aggressive. This idea is that they'll prefer to switch to the next option, that is, defense/regeneration, if there's no opponent right in front of them."),
			LW6MAP_RULES_DEFAULT_NB_ATTACK_TRIES, LW6MAP_RULES_MIN_NB_ATTACK_TRIES, LW6MAP_RULES_MAX_NB_ATTACK_TRIES);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_NB_DEFENSE_TRIES,
			_x_
			("Defines how many tries a fighter will do before giving-up attacking and choosing another behavior (do nothing). By tries we mean: how many directions it will try. Going North? Going North-West? Setting this to a low value, you'll need a very compact pack of fighters for regeneration to operate, else fighters will hang arround unhealthy."),
			LW6MAP_RULES_DEFAULT_NB_DEFENSE_TRIES, LW6MAP_RULES_MIN_NB_DEFENSE_TRIES, LW6MAP_RULES_MAX_NB_DEFENSE_TRIES);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_VERTICAL_MOVE,
			_x_
			("Defines when to process a vertical move (along the Z 'depth' axis). If set to 0, fighters never spontaneously move along this axis. If set to 1, it will be tried just after the first move failed. If set to 2, it will be tried just after the second move failed. And so on."),
			LW6MAP_RULES_DEFAULT_VERTICAL_MOVE, LW6MAP_RULES_MIN_VERTICAL_MOVE, LW6MAP_RULES_MAX_VERTICAL_MOVE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_SPREAD_MODE,
			_x_
			("If set to 1, then gradient spread will be slower but gain in terms of homogeneity and consistency. You could consider setting this to 0 on very very big maps to save CPU cycles, else the default should work fine."),
			LW6MAP_RULES_DEFAULT_SPREAD_MODE, LW6MAP_RULES_MIN_SPREAD_MODE, LW6MAP_RULES_MAX_SPREAD_MODE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_SINGLE_ARMY_SIZE,
			_x_
			("Defines the proportion of the whole available space, which will be occupied by an army at the beginning of the game. You can either imagine playing with almost empty maps, or play very crowded with almost no space left. This is a percentage, but will be multiplied by itself to get the actual surface. That is, 50 means 50%*50%, that is, a square of 1/2 the size of a square map, so it represents 25% (1/4) of the total surface."),
			LW6MAP_RULES_DEFAULT_SINGLE_ARMY_SIZE, LW6MAP_RULES_MIN_SINGLE_ARMY_SIZE, LW6MAP_RULES_MAX_SINGLE_ARMY_SIZE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TOTAL_ARMIES_SIZE,
			_x_
			("Defines the proportion of the whole available space, which can be occupied by all the armies present together. Setting this low, whenever a new team arrives on the map, fighters might be stolen to other teams, otherwise the ame would get too crowded. This allows you to play with reasonnably enough fighters with 2 players, while still allowing interesting gameplay with many players. This is a percentage, but will be multiplied by itself to get the actual surface. That is, 50 means 50%*50%, that is, a square of 1/2 the size of a square map, so it represents 25% (1/4) of the total surface."),
			LW6MAP_RULES_DEFAULT_TOTAL_ARMIES_SIZE, LW6MAP_RULES_MIN_TOTAL_ARMIES_SIZE, LW6MAP_RULES_MAX_TOTAL_ARMIES_SIZE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MAX_NB_TEAMS,
			_x_
			("Defines the maximum number of teams who can enter the game. Really makes sense in network games. Default value is 10, the maximum."),
			LW6MAP_RULES_DEFAULT_MAX_NB_TEAMS, LW6MAP_RULES_MIN_MAX_NB_TEAMS, LW6MAP_RULES_MAX_MAX_NB_TEAMS);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MAX_NB_CURSORS,
			_x_
			("Defines the maximum number of cursors who can enter the game. Really makes sense in network games. Default value is 26, the maximum."),
			LW6MAP_RULES_DEFAULT_MAX_NB_CURSORS, LW6MAP_RULES_MIN_MAX_NB_CURSORS, LW6MAP_RULES_MAX_MAX_NB_CURSORS);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MAX_NB_NODES,
			_x_
			("Defines the maximum number of servers who can enter the game. Really makes sense in network games. Default value is 10, and should fit in most cases. Can be raised up to 26."),
			LW6MAP_RULES_DEFAULT_MAX_NB_NODES, LW6MAP_RULES_MIN_MAX_NB_NODES, LW6MAP_RULES_MAX_MAX_NB_NODES);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_EXP,
			_x_
			("Level of experience (AKA exp) required to play the current level. If this level is validated (that is, won) then player will be granted with a level of exp+1 and be able to play all the next levels. An exp of 0 means the level is playable by a pure beginner."),
			LW6MAP_RULES_DEFAULT_EXP, LW6MAP_RULES_MIN_EXP, LW6MAP_RULES_MAX_EXP);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_HIGHEST_TEAM_COLOR_ALLOWED,
			_x_
			("Id of the greatest/highest color one can use. Normally, you can leave this untouched, the program will automatically fit this according to your exp. Setting an artificially low value will just cause normally available colors to disappear, setting it to a high value does nothing, if you still don't have access to some colors, you still don't, period."),
			LW6MAP_RULES_DEFAULT_HIGHEST_TEAM_COLOR_ALLOWED, LW6MAP_RULES_MIN_HIGHEST_TEAM_COLOR_ALLOWED,
			LW6MAP_RULES_MAX_HIGHEST_TEAM_COLOR_ALLOWED);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_HIGHEST_WEAPON_ALLOWED,
			_x_
			("Id of the greatest/highest weapon one can use. Normally, you can leave this untouched, the program will automatically fit this according to your exp. Setting an artificially low value will just cause normally available weapons to disappear, setting it to a high value does nothing, if you still don't have access to some weapons, you still don't, period."),
			LW6MAP_RULES_DEFAULT_HIGHEST_WEAPON_ALLOWED, LW6MAP_RULES_MIN_HIGHEST_WEAPON_ALLOWED, LW6MAP_RULES_MAX_HIGHEST_WEAPON_ALLOWED);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_X_POLARITY,
			_x_
			("Defines how the map will be wrapped on the X (horizontal) axis. If set to 0, nothing is wrapped. If set to 1, the right and left borders are connected, any fighter can disappear on the right border and reappear on the left border, for instance. If set to -1, it will be wrapped but also inversed, that is on a 320x240 map, a fighter disappearing on the left border at position (0,60) will reapper on the right border at position (319,180). You can combine it with 'y-polarity'."),
			LW6MAP_RULES_DEFAULT_X_POLARITY, LW6MAP_RULES_MIN_X_POLARITY, LW6MAP_RULES_MAX_X_POLARITY);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_Y_POLARITY,
			_x_
			("Defines how the map will be wrapped on the Y (vertical) axis. If set to 0, nothing is wrapped. If set to 1, the top and bottom borders are connected, any fighter can disappear on the top border and reappear on the bottom border, for instance. If set to -1, it will be wrapped but also inversed, that is on a 320x240 map, a fighter disappearing on the bottom border at position (40,239) will reapper on the top border at position (280,0). You can combine it with 'x-polarity'."),
			LW6MAP_RULES_DEFAULT_Y_POLARITY, LW6MAP_RULES_MIN_Y_POLARITY, LW6MAP_RULES_MAX_Y_POLARITY);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_Z_POLARITY,
			_x_
			("Defines how the map will be wrapped on the Z (deep) axis. If set to 0, nothing is wrapped. If set to 1, when using a 4 layer map, for instance, fighters on layer 1 will be able to go directly to layer 4 even if layers 2 and 3 are filled with walls. A value of -1 is forbidden, this is not like x and y axis, it does not really make sense. Consider this an advanced setting which might save a layer in some tricky cases, the default value of 0 should fit in most cases."),
			LW6MAP_RULES_DEFAULT_Z_POLARITY, LW6MAP_RULES_MIN_Z_POLARITY, LW6MAP_RULES_MAX_Z_POLARITY);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MAX_ZONE_SIZE,
			_x_
			("Defines the maximum zone size, which is an internal and rather technical parameter. The idea is that to optimize things, Liquid War 6 divides the battlefield in squares, where it can, and tries to make these squares as big as possible, the idea being that everywhere in this square, fighters follow the same intructions. Just a technical optimization. The problem is that setting it too high will reveal the optimization and its tradeoffs to the player, who will see the fighter behave strangely, following invisible paths. Plus, it's ugly. Depending on your tastes (speed, look'n'feel) you'll prefer something nice or something fast. Note that anyways passed a certain value, this does not optimize anything anymore. In doubt, don't touch it."),
			LW6MAP_RULES_DEFAULT_MAX_ZONE_SIZE, LW6MAP_RULES_MIN_MAX_ZONE_SIZE, LW6MAP_RULES_MAX_MAX_ZONE_SIZE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_ROUND_DELTA,
			_x_
			("Conditions by how much the cursor potential will be incremented each time gradient is spreaded. Sounds cryptic? It is. The idea is that at each time you move your cursor of 1 pixel, theorically, you'll need in the worst case to move of 1 more pixel to reach any point on the map. Of course this is not true but this is the default asumption, and gradient spread will fix that. Only in Liquid War 6 this is not even the worst case, for you can control your cursor with the mouse and cross walls. Whenever you cross a wall, you might have done a great distance from the fighters' point of view, if the map is a maze. Thus this parameter, which corrects things, experience shows it does give acceptable results to increase the cursor potential by more than one at each turn. Toy arround with this if you find fighters take wrong paths on some given map. If in doubt, don't touch."),
			LW6MAP_RULES_DEFAULT_ROUND_DELTA, LW6MAP_RULES_MIN_ROUND_DELTA, LW6MAP_RULES_MAX_ROUND_DELTA);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MAX_ROUND_DELTA,
			_x_
			("This is the companion value of 'round-delta'. Will put an absolute limit to the delta, which (what did you think?) is of course incremented in some cases by the core algorithm. If in doubt, don't touch."),
			LW6MAP_RULES_DEFAULT_MAX_ROUND_DELTA, LW6MAP_RULES_MIN_MAX_ROUND_DELTA, LW6MAP_RULES_MAX_MAX_ROUND_DELTA);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MAX_CURSOR_POT,
			_x_
			("Defines the maximum cursor potential. Not really any reason to change it. Any high value should produce the same results. Low values might reveal algorithm bugs and inconsistencies."),
			LW6MAP_RULES_DEFAULT_MAX_CURSOR_POT, LW6MAP_RULES_MIN_MAX_CURSOR_POT, LW6MAP_RULES_MAX_MAX_CURSOR_POT);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_CURSOR_POT_INIT,
			_x_
			("Defines the cursor potential at startup. Not really any reason to change it. Theorically, there could be maps where the default value doesn't fit, but none has been seen yet."),
			LW6MAP_RULES_DEFAULT_CURSOR_POT_INIT, LW6MAP_RULES_MIN_CURSOR_POT_INIT, LW6MAP_RULES_MAX_CURSOR_POT_INIT);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MAX_CURSOR_POT_OFFSET,
			_x_
			("Defines the maximum cursor potential offset. The idea is that in some cases, the potential of a cursor can increase in burst mode, for instance to make this cursor more important than others, so that fighters rally to it, neglecting other cursors (talking about a multi-cursor controlled team). This parameter is here to limit this burst effect and avoid bugs."),
			LW6MAP_RULES_DEFAULT_MAX_CURSOR_POT_OFFSET, LW6MAP_RULES_MIN_MAX_CURSOR_POT_OFFSET, LW6MAP_RULES_MAX_MAX_CURSOR_POT_OFFSET);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_RED_X,
			_x_ ("X start position for the red team. This is a percentage of map width, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_RED_X, LW6MAP_RULES_MIN_START_RED_X, LW6MAP_RULES_MAX_START_RED_X);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_GREEN_X,
			_x_ ("X start position for the green team. This is a percentage of map width, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_GREEN_X, LW6MAP_RULES_MIN_START_GREEN_X, LW6MAP_RULES_MAX_START_GREEN_X);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_BLUE_X,
			_x_ ("X start position for the blue team. This is a percentage of map width, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_BLUE_X, LW6MAP_RULES_MIN_START_BLUE_X, LW6MAP_RULES_MAX_START_BLUE_X);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_YELLOW_X,
			_x_ ("X start position for the yellow team. This is a percentage of map width, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_YELLOW_X, LW6MAP_RULES_MIN_START_YELLOW_X, LW6MAP_RULES_MAX_START_YELLOW_X);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_CYAN_X,
			_x_ ("X start position for the cyan team. This is a percentage of map width, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_CYAN_X, LW6MAP_RULES_MIN_START_CYAN_X, LW6MAP_RULES_MAX_START_CYAN_X);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_MAGENTA_X,
			_x_ ("X start position for the magenta team. This is a percentage of map width, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_MAGENTA_X, LW6MAP_RULES_MIN_START_MAGENTA_X, LW6MAP_RULES_MAX_START_MAGENTA_X);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_ORANGE_X,
			_x_ ("X start position for the orange team. This is a percentage of map width, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_ORANGE_X, LW6MAP_RULES_MIN_START_ORANGE_X, LW6MAP_RULES_MAX_START_ORANGE_X);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_LIGHTBLUE_X,
			_x_ ("X start position for the lightblue team. This is a percentage of map width, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_LIGHTBLUE_X, LW6MAP_RULES_MIN_START_LIGHTBLUE_X, LW6MAP_RULES_MAX_START_LIGHTBLUE_X);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_PURPLE_X,
			_x_ ("X start position for the purple team. This is a percentage of map width, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_PURPLE_X, LW6MAP_RULES_MIN_START_PURPLE_X, LW6MAP_RULES_MAX_START_PURPLE_X);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_PINK_X,
			_x_ ("X start position for the pink team. This is a percentage of map width, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_PINK_X, LW6MAP_RULES_MIN_START_PINK_X, LW6MAP_RULES_MAX_START_PINK_X);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_RED_Y,
			_x_ ("Y start position for the red team. This is a percentage of map height, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_RED_Y, LW6MAP_RULES_MIN_START_RED_Y, LW6MAP_RULES_MAX_START_RED_Y);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_GREEN_Y,
			_x_ ("Y start position for the green team. This is a percentage of map height, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_GREEN_Y, LW6MAP_RULES_MIN_START_GREEN_Y, LW6MAP_RULES_MAX_START_GREEN_Y);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_BLUE_Y,
			_x_ ("Y start position for the blue team. This is a percentage of map height, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_BLUE_Y, LW6MAP_RULES_MIN_START_BLUE_Y, LW6MAP_RULES_MAX_START_BLUE_Y);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_YELLOW_Y,
			_x_ ("Y start position for the yellow team. This is a percentage of map height, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_YELLOW_Y, LW6MAP_RULES_MIN_START_YELLOW_Y, LW6MAP_RULES_MAX_START_YELLOW_Y);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_CYAN_Y,
			_x_ ("Y start position for the cyan team. This is a percentage of map height, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_CYAN_Y, LW6MAP_RULES_MIN_START_CYAN_Y, LW6MAP_RULES_MAX_START_CYAN_Y);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_MAGENTA_Y,
			_x_ ("Y start position for the magenta team. This is a percentage of map height, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_MAGENTA_Y, LW6MAP_RULES_MIN_START_MAGENTA_Y, LW6MAP_RULES_MAX_START_MAGENTA_Y);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_ORANGE_Y,
			_x_ ("Y start position for the orange team. This is a percentage of map height, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_ORANGE_Y, LW6MAP_RULES_MIN_START_ORANGE_Y, LW6MAP_RULES_MAX_START_ORANGE_Y);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_LIGHTBLUE_Y,
			_x_ ("Y start position for the lightblue team. This is a percentage of map height, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_LIGHTBLUE_Y, LW6MAP_RULES_MIN_START_LIGHTBLUE_Y, LW6MAP_RULES_MAX_START_LIGHTBLUE_Y);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_PURPLE_Y,
			_x_ ("Y start position for the purple team. This is a percentage of map height, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_PURPLE_Y, LW6MAP_RULES_MIN_START_PURPLE_Y, LW6MAP_RULES_MAX_START_PURPLE_Y);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_PINK_Y,
			_x_ ("Y start position for the pink team. This is a percentage of map height, value between 0 and 100."),
			LW6MAP_RULES_DEFAULT_START_PINK_Y, LW6MAP_RULES_MIN_START_PINK_Y, LW6MAP_RULES_MAX_START_PINK_Y);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_START_POSITION_MODE,
			_x_
			("Defines how teams are set up on the map at game startup. 0 means teams respect the pre-defined start positions. 1 means that a random position will be picked, among the existing positions. That is, red could take green's place. 2 means total randomness, teams can appear anywhere."),
			LW6MAP_RULES_DEFAULT_START_POSITION_MODE, LW6MAP_RULES_MIN_START_POSITION_MODE, LW6MAP_RULES_MAX_START_POSITION_MODE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_COLOR_CONFLICT_MODE,
			_x_
			("How to handle color conflicts, that is, when a player requests a color, but this color is already used, what should be done? If 0, wether a color already exists won't affect the color of a new cursor. If 1, then two players on the same computer will be allowed to share the same color/team, but if another computer is already playing with a color, any new computer will need to use another team. If 2, then it's impossible for a new cursor to use a pre-existing color, any new cursor will require a new color, if that color is already used, a new color will be picked randomly."),
			LW6MAP_RULES_DEFAULT_COLOR_CONFLICT_MODE, LW6MAP_RULES_MIN_COLOR_CONFLICT_MODE, LW6MAP_RULES_MAX_COLOR_CONFLICT_MODE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_SPREAD_THREAD,
			_x_
			("If set to 1, the core algorithm with fire a separate thread to spread the gradient. By default this is turned off (set to 0). Consider this as an experimental feature, the program is already rather heavily threaded, turning this on will probably not offer any significant performance gain, even on SMP systems. This might change in the future."),
			LW6MAP_RULES_DEFAULT_SPREAD_THREAD, LW6MAP_RULES_MIN_SPREAD_THREAD, LW6MAP_RULES_MAX_SPREAD_THREAD);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_GLUE_POWER,
			_x_
			("Defines how sticky and powerfull the glue is. That is, if on 'glue.png' it's pitch black and this parameter is set to 3, then fighters will take 3 steps to do what would normally take only one step."),
			LW6MAP_RULES_DEFAULT_GLUE_POWER, LW6MAP_RULES_MIN_GLUE_POWER, LW6MAP_RULES_MAX_GLUE_POWER);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_BOOST_POWER,
			_x_
			("Defines how fast and powerfull the boost is. That is, if on 'boost.png' it's pitch black and this parameter is set to 3, then fighters will move and act 3 times than what they would do normally."),
			LW6MAP_RULES_DEFAULT_BOOST_POWER, LW6MAP_RULES_MIN_BOOST_POWER, LW6MAP_RULES_MAX_BOOST_POWER);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_DANGER_POWER,
			_x_
			("Defines how dangerous are the black zones defined in 'danger.png'. The value is used to decrease the fighter health at each move, so you should compare its value to something like 'fighter-attack'. Being on a dangerous zone is a bit like being attacked by an invisible and unknown ennemy."),
			LW6MAP_RULES_DEFAULT_DANGER_POWER, LW6MAP_RULES_MIN_DANGER_POWER, LW6MAP_RULES_MAX_DANGER_POWER);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_MEDICINE_POWER,
			_x_
			("Defines how fast fighter will automatically regenerate on black zones defined in 'medicine.png'. The value is used to decrease the fighter health at each move, so you should compare its value to something like 'fighter-defense'. Being on a medicined zone is a bit like being defended by an invisible and unknown friend."),
			LW6MAP_RULES_DEFAULT_MEDICINE_POWER, LW6MAP_RULES_MIN_MEDICINE_POWER, LW6MAP_RULES_MAX_MEDICINE_POWER);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_FRAGS_MODE,
			_x_
			("Defines how points are calculated in deathmatch mode, 0 is old school simple mode. 1 is in a mode in which 1 point is attributed to every winner, and looser looses all the corresponding points (total is always 0). 2 isproportional mode, with a total of 0 kept constant, that is, loosers loose as many points as attributed to winners. 3 is a mode in which at each death, winners are attributed a number of points proportional to their fighters, and loosers scores remain untouched."),
			LW6MAP_RULES_DEFAULT_FRAGS_MODE, LW6MAP_RULES_MIN_FRAGS_MODE, LW6MAP_RULES_MAX_FRAGS_MODE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_FRAGS_TO_DISTRIBUTE,
			_x_
			("Defines how many points will be distributed when in deathmatch mode. When a player looses, this amont of points will be substracted to its total, and the same amount of points will be distributed to other live players, proportionnally to how many fighters they have on the battlefield."),
			LW6MAP_RULES_DEFAULT_FRAGS_TO_DISTRIBUTE, LW6MAP_RULES_MIN_FRAGS_TO_DISTRIBUTE, LW6MAP_RULES_MAX_FRAGS_TO_DISTRIBUTE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_FRAGS_FADE_OUT,
			_x_
			("When a player looses (in deathmatch mode) all player points will be multiplicated by this percentage, for instance if it's 90 and player had 50 points, then player will only have 45 points, then points corresponding to the new death will be added/substrated to its total. This is to avoid players with thousands of points in advance, and keep everyone in the race. A low value will minimize the importance of game start. This is only used in modes where frags are distributed in a proportional way."),
			LW6MAP_RULES_DEFAULT_FRAGS_FADE_OUT, LW6MAP_RULES_MIN_FRAGS_FADE_OUT, LW6MAP_RULES_MAX_FRAGS_FADE_OUT);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_USE_TEAM_PROFILES,
			_x_
			("If set, then all the team-profile-... values will be taken in account. This enables a mode in which teams behave differently according to their colors. If you disable this, then all teams will behave the same, which is more fair, but might not be as fun."),
			LW6MAP_RULES_DEFAULT_USE_TEAM_PROFILES, LW6MAP_RULES_MIN_USE_TEAM_PROFILES, LW6MAP_RULES_MAX_USE_TEAM_PROFILES);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_RED_AGGRESSIVE,
			_x_
			("Defines how aggressive the red team is. This is a percentage, if set to 200 then team will attack twice as much as any other team with the default value. Setting this to a high value clearly advantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_RED_AGGRESSIVE, LW6MAP_RULES_MIN_TEAM_PROFILE_RED_AGGRESSIVE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_RED_AGGRESSIVE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_GREEN_AGGRESSIVE,
			_x_
			("Defines how aggressive the green team is. This is a percentage, if set to 200 then team will attack twice as much as any other team with the default value. Setting this to a high value clearly advantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_GREEN_AGGRESSIVE, LW6MAP_RULES_MIN_TEAM_PROFILE_GREEN_AGGRESSIVE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_GREEN_AGGRESSIVE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_BLUE_AGGRESSIVE,
			_x_
			("Defines how aggressive the blue team is. This is a percentage, if set to 200 then team will attack twice as much as any other team with the default value. Setting this to a high value clearly advantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_BLUE_AGGRESSIVE, LW6MAP_RULES_MIN_TEAM_PROFILE_BLUE_AGGRESSIVE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_BLUE_AGGRESSIVE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_YELLOW_AGGRESSIVE,
			_x_
			("Defines how aggressive the yellow team is. This is a percentage, if set to 200 then team will attack twice as much as any other team with the default value. Setting this to a high value clearly advantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_YELLOW_AGGRESSIVE, LW6MAP_RULES_MIN_TEAM_PROFILE_YELLOW_AGGRESSIVE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_YELLOW_AGGRESSIVE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_CYAN_AGGRESSIVE,
			_x_
			("Defines how aggressive the cyan team is. This is a percentage, if set to 200 then team will attack twice as much as any other team with the default value. Setting this to a high value clearly advantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_CYAN_AGGRESSIVE, LW6MAP_RULES_MIN_TEAM_PROFILE_CYAN_AGGRESSIVE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_CYAN_AGGRESSIVE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_MAGENTA_AGGRESSIVE,
			_x_
			("Defines how aggressive the magenta team is. This is a percentage, if set to 200 then team will attack twice as much as any other team with the default value. Setting this to a high value clearly advantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_MAGENTA_AGGRESSIVE, LW6MAP_RULES_MIN_TEAM_PROFILE_MAGENTA_AGGRESSIVE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_MAGENTA_AGGRESSIVE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_ORANGE_AGGRESSIVE,
			_x_
			("Defines how aggressive the orange team is. This is a percentage, if set to 200 then team will attack twice as much as any other team with the default value. Setting this to a high value clearly advantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_ORANGE_AGGRESSIVE, LW6MAP_RULES_MIN_TEAM_PROFILE_ORANGE_AGGRESSIVE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_ORANGE_AGGRESSIVE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_LIGHTBLUE_AGGRESSIVE,
			_x_
			("Defines how aggressive the lightblue team is. This is a percentage, if set to 200 then team will attack twice as much as any other team with the default value. Setting this to a high value clearly advantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_LIGHTBLUE_AGGRESSIVE, LW6MAP_RULES_MIN_TEAM_PROFILE_LIGHTBLUE_AGGRESSIVE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_LIGHTBLUE_AGGRESSIVE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_PURPLE_AGGRESSIVE,
			_x_
			("Defines how aggressive the purple team is. This is a percentage, if set to 200 then team will attack twice as much as any other team with the default value. Setting this to a high value clearly advantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_PURPLE_AGGRESSIVE, LW6MAP_RULES_MIN_TEAM_PROFILE_PURPLE_AGGRESSIVE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_PURPLE_AGGRESSIVE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_PINK_AGGRESSIVE,
			_x_
			("Defines how aggressive the pink team is. This is a percentage, if set to 200 then team will attack twice as much as any other team with the default value. Setting this to a high value clearly advantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_PINK_AGGRESSIVE, LW6MAP_RULES_MIN_TEAM_PROFILE_PINK_AGGRESSIVE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_PINK_AGGRESSIVE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_RED_VULNERABLE,
			_x_
			("Defines how vulnerable the red team is. This is a percentage, if set to 200 then team will be attacked twice as much as any other team with the default value. Setting this to a high value clearly disadvantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_RED_VULNERABLE, LW6MAP_RULES_MIN_TEAM_PROFILE_RED_VULNERABLE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_RED_VULNERABLE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_GREEN_VULNERABLE,
			_x_
			("Defines how vulnerable the green team is. This is a percentage, if set to 200 then team will be attacked twice as much as any other team with the default value. Setting this to a high value clearly disadvantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_GREEN_VULNERABLE, LW6MAP_RULES_MIN_TEAM_PROFILE_GREEN_VULNERABLE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_GREEN_VULNERABLE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_BLUE_VULNERABLE,
			_x_
			("Defines how vulnerable the blue team is. This is a percentage, if set to 200 then team will be attacked twice as much as any other team with the default value. Setting this to a high value clearly disadvantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_BLUE_VULNERABLE, LW6MAP_RULES_MIN_TEAM_PROFILE_BLUE_VULNERABLE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_BLUE_VULNERABLE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_YELLOW_VULNERABLE,
			_x_
			("Defines how vulnerable the yellow team is. This is a percentage, if set to 200 then team will be attacked twice as much as any other team with the default value. Setting this to a high value clearly disadvantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_YELLOW_VULNERABLE, LW6MAP_RULES_MIN_TEAM_PROFILE_YELLOW_VULNERABLE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_YELLOW_VULNERABLE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_CYAN_VULNERABLE,
			_x_
			("Defines how vulnerable the cyan team is. This is a percentage, if set to 200 then team will be attacked twice as much as any other team with the default value. Setting this to a high value clearly disadvantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_CYAN_VULNERABLE, LW6MAP_RULES_MIN_TEAM_PROFILE_CYAN_VULNERABLE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_CYAN_VULNERABLE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_MAGENTA_VULNERABLE,
			_x_
			("Defines how vulnerable the magenta team is. This is a percentage, if set to 200 then team will be attacked twice as much as any other team with the default value. Setting this to a high value clearly disadvantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_MAGENTA_VULNERABLE, LW6MAP_RULES_MIN_TEAM_PROFILE_MAGENTA_VULNERABLE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_MAGENTA_VULNERABLE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_ORANGE_VULNERABLE,
			_x_
			("Defines how vulnerable the orange team is. This is a percentage, if set to 200 then team will be attacked twice as much as any other team with the default value. Setting this to a high value clearly disadvantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_ORANGE_VULNERABLE, LW6MAP_RULES_MIN_TEAM_PROFILE_ORANGE_VULNERABLE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_ORANGE_VULNERABLE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_LIGHTBLUE_VULNERABLE,
			_x_
			("Defines how vulnerable the lightblue team is. This is a percentage, if set to 200 then team will be attacked twice as much as any other team with the default value. Setting this to a high value clearly disadvantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_LIGHTBLUE_VULNERABLE, LW6MAP_RULES_MIN_TEAM_PROFILE_LIGHTBLUE_VULNERABLE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_LIGHTBLUE_VULNERABLE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_PURPLE_VULNERABLE,
			_x_
			("Defines how vulnerable the purple team is. This is a percentage, if set to 200 then team will be attacked twice as much as any other team with the default value. Setting this to a high value clearly disadvantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_PURPLE_VULNERABLE, LW6MAP_RULES_MIN_TEAM_PROFILE_PURPLE_VULNERABLE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_PURPLE_VULNERABLE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_PINK_VULNERABLE,
			_x_
			("Defines how vulnerable the pink team is. This is a percentage, if set to 200 then team will be attacked twice as much as any other team with the default value. Setting this to a high value clearly disadvantages this team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_PINK_VULNERABLE, LW6MAP_RULES_MIN_TEAM_PROFILE_PINK_VULNERABLE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_PINK_VULNERABLE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_RED_MOBILE,
			_x_
			("Increases (or decreases if negative) the number of move/attack/defense tries for the red team. If set to a high value team will appear more mobile and do more things, but it won't change its cruising speed. It's not obvious to tell wether this is an advantage or not, but it clearly changes the behavior."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_RED_MOBILE, LW6MAP_RULES_MIN_TEAM_PROFILE_RED_MOBILE, LW6MAP_RULES_MAX_TEAM_PROFILE_RED_MOBILE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_GREEN_MOBILE,
			_x_
			("Increases (or decreases if negative) the number of move/attack/defense tries for the green team. If set to a high value team will appear more mobile and do more things, but it won't change its cruising speed. It's not obvious to tell wether this is an advantage or not, but it clearly changes the behavior."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_GREEN_MOBILE, LW6MAP_RULES_MIN_TEAM_PROFILE_GREEN_MOBILE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_GREEN_MOBILE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_BLUE_MOBILE,
			_x_
			("Increases (or decreases if negative) the number of move/attack/defense tries for the blue team. If set to a high value team will appear more mobile and do more things, but it won't change its cruising speed. It's not obvious to tell wether this is an advantage or not, but it clearly changes the behavior."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_BLUE_MOBILE, LW6MAP_RULES_MIN_TEAM_PROFILE_BLUE_MOBILE, LW6MAP_RULES_MAX_TEAM_PROFILE_BLUE_MOBILE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_YELLOW_MOBILE,
			_x_
			("Increases (or decreases if negative) the number of move/attack/defense tries for the yellow team. If set to a high value team will appear more mobile and do more things, but it won't change its cruising speed. It's not obvious to tell wether this is an advantage or not, but it clearly changes the behavior."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_YELLOW_MOBILE, LW6MAP_RULES_MIN_TEAM_PROFILE_YELLOW_MOBILE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_YELLOW_MOBILE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_CYAN_MOBILE,
			_x_
			("Increases (or decreases if negative) the number of move/attack/defense tries for the cyan team. If set to a high value team will appear more mobile and do more things, but it won't change its cruising speed. It's not obvious to tell wether this is an advantage or not, but it clearly changes the behavior."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_CYAN_MOBILE, LW6MAP_RULES_MIN_TEAM_PROFILE_CYAN_MOBILE, LW6MAP_RULES_MAX_TEAM_PROFILE_CYAN_MOBILE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_MAGENTA_MOBILE,
			_x_
			("Increases (or decreases if negative) the number of move/attack/defense tries for the magenta team. If set to a high value team will appear more mobile and do more things, but it won't change its cruising speed. It's not obvious to tell wether this is an advantage or not, but it clearly changes the behavior."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_MAGENTA_MOBILE, LW6MAP_RULES_MIN_TEAM_PROFILE_MAGENTA_MOBILE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_MAGENTA_MOBILE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_ORANGE_MOBILE,
			_x_
			("Increases (or decreases if negative) the number of move/attack/defense tries for the orange team. If set to a high value team will appear more mobile and do more things, but it won't change its cruising speed. It's not obvious to tell wether this is an advantage or not, but it clearly changes the behavior."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_ORANGE_MOBILE, LW6MAP_RULES_MIN_TEAM_PROFILE_ORANGE_MOBILE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_ORANGE_MOBILE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_LIGHTBLUE_MOBILE,
			_x_
			("Increases (or decreases if negative) the number of move/attack/defense tries for the lightblue team. If set to a high value team will appear more mobile and do more things, but it won't change its cruising speed. It's not obvious to tell wether this is an advantage or not, but it clearly changes the behavior."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_LIGHTBLUE_MOBILE, LW6MAP_RULES_MIN_TEAM_PROFILE_LIGHTBLUE_MOBILE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_LIGHTBLUE_MOBILE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_PURPLE_MOBILE,
			_x_
			("Increases (or decreases if negative) the number of move/attack/defense tries for the purple team. If set to a high value team will appear more mobile and do more things, but it won't change its cruising speed. It's not obvious to tell wether this is an advantage or not, but it clearly changes the behavior."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_PURPLE_MOBILE, LW6MAP_RULES_MIN_TEAM_PROFILE_PURPLE_MOBILE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_PURPLE_MOBILE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_PINK_MOBILE,
			_x_
			("Increases (or decreases if negative) the number of move/attack/defense tries for the pink team. If set to a high value team will appear more mobile and do more things, but it won't change its cruising speed. It's not obvious to tell wether this is an advantage or not, but it clearly changes the behavior."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_PINK_MOBILE, LW6MAP_RULES_MIN_TEAM_PROFILE_PINK_MOBILE, LW6MAP_RULES_MAX_TEAM_PROFILE_PINK_MOBILE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_RED_FAST,
			_x_
			("Changes the speed of the red team. This is a percentage, if set to 50, then team will move twice slower than other teams with the default parameter. Setting this high is very likely to advantage the team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_RED_FAST, LW6MAP_RULES_MIN_TEAM_PROFILE_RED_FAST, LW6MAP_RULES_MAX_TEAM_PROFILE_RED_FAST);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_GREEN_FAST,
			_x_
			("Changes the speed of the green team. This is a percentage, if set to 50, then team will move twice slower than other teams with the default parameter. Setting this high is very likely to advantage the team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_GREEN_FAST, LW6MAP_RULES_MIN_TEAM_PROFILE_GREEN_FAST, LW6MAP_RULES_MAX_TEAM_PROFILE_GREEN_FAST);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_BLUE_FAST,
			_x_
			("Changes the speed of the blue team. This is a percentage, if set to 50, then team will move twice slower than other teams with the default parameter. Setting this high is very likely to advantage the team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_BLUE_FAST, LW6MAP_RULES_MIN_TEAM_PROFILE_BLUE_FAST, LW6MAP_RULES_MAX_TEAM_PROFILE_BLUE_FAST);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_YELLOW_FAST,
			_x_
			("Changes the speed of the yellow team. This is a percentage, if set to 50, then team will move twice slower than other teams with the default parameter. Setting this high is very likely to advantage the team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_YELLOW_FAST, LW6MAP_RULES_MIN_TEAM_PROFILE_YELLOW_FAST, LW6MAP_RULES_MAX_TEAM_PROFILE_YELLOW_FAST);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_CYAN_FAST,
			_x_
			("Changes the speed of the cyan team. This is a percentage, if set to 50, then team will move twice slower than other teams with the default parameter. Setting this high is very likely to advantage the team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_CYAN_FAST, LW6MAP_RULES_MIN_TEAM_PROFILE_CYAN_FAST, LW6MAP_RULES_MAX_TEAM_PROFILE_CYAN_FAST);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_MAGENTA_FAST,
			_x_
			("Changes the speed of the magenta team. This is a percentage, if set to 50, then team will move twice slower than other teams with the default parameter. Setting this high is very likely to advantage the team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_MAGENTA_FAST, LW6MAP_RULES_MIN_TEAM_PROFILE_MAGENTA_FAST,
			LW6MAP_RULES_MAX_TEAM_PROFILE_MAGENTA_FAST);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_ORANGE_FAST,
			_x_
			("Changes the speed of the orange team. This is a percentage, if set to 50, then team will move twice slower than other teams with the default parameter. Setting this high is very likely to advantage the team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_ORANGE_FAST, LW6MAP_RULES_MIN_TEAM_PROFILE_ORANGE_FAST, LW6MAP_RULES_MAX_TEAM_PROFILE_ORANGE_FAST);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_LIGHTBLUE_FAST,
			_x_
			("Changes the speed of the lightblue team. This is a percentage, if set to 50, then team will move twice slower than other teams with the default parameter. Setting this high is very likely to advantage the team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_LIGHTBLUE_FAST, LW6MAP_RULES_MIN_TEAM_PROFILE_LIGHTBLUE_FAST,
			LW6MAP_RULES_MAX_TEAM_PROFILE_LIGHTBLUE_FAST);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_PURPLE_FAST,
			_x_
			("Changes the speed of the purple team. This is a percentage, if set to 50, then team will move twice slower than other teams with the default parameter. Setting this high is very likely to advantage the team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_PURPLE_FAST, LW6MAP_RULES_MIN_TEAM_PROFILE_PURPLE_FAST, LW6MAP_RULES_MAX_TEAM_PROFILE_PURPLE_FAST);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_PINK_FAST,
			_x_
			("Changes the speed of the pink team. This is a percentage, if set to 50, then team will move twice slower than other teams with the default parameter. Setting this high is very likely to advantage the team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_PINK_FAST, LW6MAP_RULES_MIN_TEAM_PROFILE_PINK_FAST, LW6MAP_RULES_MAX_TEAM_PROFILE_PINK_FAST);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_RED_HANDICAP, _x_ ("Defines the handicap for the red team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_RED_HANDICAP, LW6MAP_RULES_MIN_TEAM_PROFILE_RED_HANDICAP,
			LW6MAP_RULES_MAX_TEAM_PROFILE_RED_HANDICAP);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_GREEN_HANDICAP, _x_ ("Defines the handicap for the green team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_GREEN_HANDICAP, LW6MAP_RULES_MIN_TEAM_PROFILE_GREEN_HANDICAP,
			LW6MAP_RULES_MAX_TEAM_PROFILE_GREEN_HANDICAP);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_BLUE_HANDICAP, _x_ ("Defines the handicap for the blue team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_BLUE_HANDICAP, LW6MAP_RULES_MIN_TEAM_PROFILE_BLUE_HANDICAP,
			LW6MAP_RULES_MAX_TEAM_PROFILE_BLUE_HANDICAP);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_YELLOW_HANDICAP, _x_ ("Defines the handicap for the yellow team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_YELLOW_HANDICAP, LW6MAP_RULES_MIN_TEAM_PROFILE_YELLOW_HANDICAP,
			LW6MAP_RULES_MAX_TEAM_PROFILE_YELLOW_HANDICAP);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_CYAN_HANDICAP, _x_ ("Defines the handicap for the cyan team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_CYAN_HANDICAP, LW6MAP_RULES_MIN_TEAM_PROFILE_CYAN_HANDICAP,
			LW6MAP_RULES_MAX_TEAM_PROFILE_CYAN_HANDICAP);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_MAGENTA_HANDICAP, _x_ ("Defines the handicap for the magenta team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_MAGENTA_HANDICAP, LW6MAP_RULES_MIN_TEAM_PROFILE_MAGENTA_HANDICAP,
			LW6MAP_RULES_MAX_TEAM_PROFILE_MAGENTA_HANDICAP);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_ORANGE_HANDICAP, _x_ ("Defines the handicap for the orange team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_ORANGE_HANDICAP, LW6MAP_RULES_MIN_TEAM_PROFILE_ORANGE_HANDICAP,
			LW6MAP_RULES_MAX_TEAM_PROFILE_ORANGE_HANDICAP);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_LIGHTBLUE_HANDICAP, _x_ ("Defines the handicap for the lightblue team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_LIGHTBLUE_HANDICAP, LW6MAP_RULES_MIN_TEAM_PROFILE_LIGHTBLUE_HANDICAP,
			LW6MAP_RULES_MAX_TEAM_PROFILE_LIGHTBLUE_HANDICAP);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_PURPLE_HANDICAP, _x_ ("Defines the handicap for the purple team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_PURPLE_HANDICAP, LW6MAP_RULES_MIN_TEAM_PROFILE_PURPLE_HANDICAP,
			LW6MAP_RULES_MAX_TEAM_PROFILE_PURPLE_HANDICAP);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_PINK_HANDICAP, _x_ ("Defines the handicap for the pink team."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_PINK_HANDICAP, LW6MAP_RULES_MIN_TEAM_PROFILE_PINK_HANDICAP,
			LW6MAP_RULES_MAX_TEAM_PROFILE_PINK_HANDICAP);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_RED_WEAPON_ID,
			_x_ ("Id of the default weapon for the red team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_RED_WEAPON_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_RED_WEAPON_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_RED_WEAPON_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_GREEN_WEAPON_ID,
			_x_ ("Id of the default weapon for the green team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_GREEN_WEAPON_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_GREEN_WEAPON_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_GREEN_WEAPON_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_BLUE_WEAPON_ID,
			_x_ ("Id of the default weapon for the blue team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_BLUE_WEAPON_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_BLUE_WEAPON_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_BLUE_WEAPON_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_YELLOW_WEAPON_ID,
			_x_ ("Id of the default weapon for the yellow team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_YELLOW_WEAPON_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_YELLOW_WEAPON_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_YELLOW_WEAPON_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_CYAN_WEAPON_ID,
			_x_ ("Id of the default weapon for the cyan team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_CYAN_WEAPON_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_CYAN_WEAPON_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_CYAN_WEAPON_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_MAGENTA_WEAPON_ID,
			_x_ ("Id of the default weapon for the magenta team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_MAGENTA_WEAPON_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_MAGENTA_WEAPON_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_MAGENTA_WEAPON_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_ORANGE_WEAPON_ID,
			_x_ ("Id of the default weapon for the orange team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_ORANGE_WEAPON_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_ORANGE_WEAPON_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_ORANGE_WEAPON_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_LIGHTBLUE_WEAPON_ID,
			_x_ ("Id of the default weapon for the lightblue team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_LIGHTBLUE_WEAPON_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_LIGHTBLUE_WEAPON_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_LIGHTBLUE_WEAPON_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_PURPLE_WEAPON_ID,
			_x_ ("Id of the default weapon for the purple team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_PURPLE_WEAPON_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_PURPLE_WEAPON_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_PURPLE_WEAPON_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_PINK_WEAPON_ID,
			_x_ ("Id of the default weapon for the pink team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_PINK_WEAPON_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_PINK_WEAPON_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_PINK_WEAPON_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_RED_WEAPON_ALTERNATE_ID,
			_x_ ("Id of the default alternate weapon for the red team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_RED_WEAPON_ALTERNATE_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_RED_WEAPON_ALTERNATE_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_RED_WEAPON_ALTERNATE_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_GREEN_WEAPON_ALTERNATE_ID,
			_x_ ("Id of the default alternate weapon for the green team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_GREEN_WEAPON_ALTERNATE_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_GREEN_WEAPON_ALTERNATE_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_GREEN_WEAPON_ALTERNATE_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_BLUE_WEAPON_ALTERNATE_ID,
			_x_ ("Id of the default alternate weapon for the blue team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_BLUE_WEAPON_ALTERNATE_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_BLUE_WEAPON_ALTERNATE_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_BLUE_WEAPON_ALTERNATE_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_YELLOW_WEAPON_ALTERNATE_ID,
			_x_ ("Id of the default alternate weapon for the yellow team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_YELLOW_WEAPON_ALTERNATE_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_YELLOW_WEAPON_ALTERNATE_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_YELLOW_WEAPON_ALTERNATE_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_CYAN_WEAPON_ALTERNATE_ID,
			_x_ ("Id of the default alternate weapon for the cyan team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_CYAN_WEAPON_ALTERNATE_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_CYAN_WEAPON_ALTERNATE_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_CYAN_WEAPON_ALTERNATE_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_MAGENTA_WEAPON_ALTERNATE_ID,
			_x_ ("Id of the default alternate weapon for the magenta team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_MAGENTA_WEAPON_ALTERNATE_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_MAGENTA_WEAPON_ALTERNATE_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_MAGENTA_WEAPON_ALTERNATE_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_ORANGE_WEAPON_ALTERNATE_ID,
			_x_ ("Id of the default alternate weapon for the orange team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_ORANGE_WEAPON_ALTERNATE_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_ORANGE_WEAPON_ALTERNATE_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_ORANGE_WEAPON_ALTERNATE_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_LIGHTBLUE_WEAPON_ALTERNATE_ID,
			_x_ ("Id of the default alternate weapon for the lightblue team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_LIGHTBLUE_WEAPON_ALTERNATE_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_LIGHTBLUE_WEAPON_ALTERNATE_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_LIGHTBLUE_WEAPON_ALTERNATE_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_PURPLE_WEAPON_ALTERNATE_ID,
			_x_ ("Id of the default alternate weapon for the purple team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_PURPLE_WEAPON_ALTERNATE_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_PURPLE_WEAPON_ALTERNATE_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_PURPLE_WEAPON_ALTERNATE_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_PINK_WEAPON_ALTERNATE_ID,
			_x_ ("Id of the default alternate weapon for the pink team, see the documentation about weapons to know what these ids mean."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_PINK_WEAPON_ALTERNATE_ID, LW6MAP_RULES_MIN_TEAM_PROFILE_PINK_WEAPON_ALTERNATE_ID,
			LW6MAP_RULES_MAX_TEAM_PROFILE_PINK_WEAPON_ALTERNATE_ID);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_RED_WEAPON_MODE,
			_x_
			("Weapon mode for red team. 0 means there's no weapon, 1 means one weapon per team, defined by the weapon-id parameter, 2 means random weapon."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_RED_WEAPON_MODE, LW6MAP_RULES_MIN_TEAM_PROFILE_RED_WEAPON_MODE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_RED_WEAPON_MODE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_GREEN_WEAPON_MODE,
			_x_
			("Weapon mode for green team. 0 means there's no weapon, 1 means one weapon per team, defined by the weapon-id parameter, 2 means random weapon."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_GREEN_WEAPON_MODE, LW6MAP_RULES_MIN_TEAM_PROFILE_GREEN_WEAPON_MODE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_GREEN_WEAPON_MODE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_BLUE_WEAPON_MODE,
			_x_
			("Weapon mode for blue team. 0 means there's no weapon, 1 means one weapon per team, defined by the weapon-id parameter, 2 means random weapon."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_BLUE_WEAPON_MODE, LW6MAP_RULES_MIN_TEAM_PROFILE_BLUE_WEAPON_MODE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_BLUE_WEAPON_MODE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_YELLOW_WEAPON_MODE,
			_x_
			("Weapon mode for yellow team. 0 means there's no weapon, 1 means one weapon per team, defined by the weapon-id parameter, 2 means random weapon."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_YELLOW_WEAPON_MODE, LW6MAP_RULES_MIN_TEAM_PROFILE_YELLOW_WEAPON_MODE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_YELLOW_WEAPON_MODE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_CYAN_WEAPON_MODE,
			_x_
			("Weapon mode for cyan team. 0 means there's no weapon, 1 means one weapon per team, defined by the weapon-id parameter, 2 means random weapon."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_CYAN_WEAPON_MODE, LW6MAP_RULES_MIN_TEAM_PROFILE_CYAN_WEAPON_MODE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_CYAN_WEAPON_MODE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_MAGENTA_WEAPON_MODE,
			_x_
			("Weapon mode for magenta team. 0 means there's no weapon, 1 means one weapon per team, defined by the weapon-id parameter, 2 means random weapon."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_MAGENTA_WEAPON_MODE, LW6MAP_RULES_MIN_TEAM_PROFILE_MAGENTA_WEAPON_MODE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_MAGENTA_WEAPON_MODE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_ORANGE_WEAPON_MODE,
			_x_
			("Weapon mode for orange team. 0 means there's no weapon, 1 means one weapon per team, defined by the weapon-id parameter, 2 means random weapon."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_ORANGE_WEAPON_MODE, LW6MAP_RULES_MIN_TEAM_PROFILE_ORANGE_WEAPON_MODE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_ORANGE_WEAPON_MODE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_LIGHTBLUE_WEAPON_MODE,
			_x_
			("Weapon mode for lightblue team. 0 means there's no weapon, 1 means one weapon per team, defined by the weapon-id parameter, 2 means random weapon."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_LIGHTBLUE_WEAPON_MODE, LW6MAP_RULES_MIN_TEAM_PROFILE_LIGHTBLUE_WEAPON_MODE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_LIGHTBLUE_WEAPON_MODE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_PURPLE_WEAPON_MODE,
			_x_
			("Weapon mode for purple team. 0 means there's no weapon, 1 means one weapon per team, defined by the weapon-id parameter, 2 means random weapon."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_PURPLE_WEAPON_MODE, LW6MAP_RULES_MIN_TEAM_PROFILE_PURPLE_WEAPON_MODE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_PURPLE_WEAPON_MODE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_TEAM_PROFILE_PINK_WEAPON_MODE,
			_x_
			("Weapon mode for pink team. 0 means there's no weapon, 1 means one weapon per team, defined by the weapon-id parameter, 2 means random weapon."),
			LW6MAP_RULES_DEFAULT_TEAM_PROFILE_PINK_WEAPON_MODE, LW6MAP_RULES_MIN_TEAM_PROFILE_PINK_WEAPON_MODE,
			LW6MAP_RULES_MAX_TEAM_PROFILE_PINK_WEAPON_MODE);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_WEAPON_DURATION,
			_x_ ("How long all weapons (for which duration makes sense) will last. Unit is seconds."), LW6MAP_RULES_DEFAULT_WEAPON_DURATION,
			LW6MAP_RULES_MIN_WEAPON_DURATION, LW6MAP_RULES_MAX_WEAPON_DURATION);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_WEAPON_CHARGE_DELAY,
			_x_ ("How long it will take for weapons to charge and be usable, by default. Unit is seconds."),
			LW6MAP_RULES_DEFAULT_WEAPON_CHARGE_DELAY, LW6MAP_RULES_MIN_WEAPON_CHARGE_DELAY, LW6MAP_RULES_MAX_WEAPON_CHARGE_DELAY);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_WEAPON_CHARGE_MAX,
			_x_
			("Maximum (percentage) of charge intensity that one have. For instance, if this is 400, then if you wait four times more than required before firing, then you weapon will have four times its default power, but if you wait five times more it will still be four times more powerfull, it's just the limit after which it's useless to charge."),
			LW6MAP_RULES_DEFAULT_WEAPON_CHARGE_MAX, LW6MAP_RULES_MIN_WEAPON_CHARGE_MAX, LW6MAP_RULES_MAX_WEAPON_CHARGE_MAX);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_WEAPON_TUNE_BERZERK_POWER,
			_x_ ("Use to specifiy how strong berzerk mode is, if set to 3, then attacks will be 3 times as efficient in berzerk mode."),
			LW6MAP_RULES_DEFAULT_WEAPON_TUNE_BERZERK_POWER, LW6MAP_RULES_MIN_WEAPON_TUNE_BERZERK_POWER,
			LW6MAP_RULES_MAX_WEAPON_TUNE_BERZERK_POWER);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_WEAPON_TUNE_TURBO_POWER,
			_x_ ("Defines how fast fighters move in turbo mode, if set to 3, then fighters move and act 3 times as fast."),
			LW6MAP_RULES_DEFAULT_WEAPON_TUNE_TURBO_POWER, LW6MAP_RULES_MIN_WEAPON_TUNE_TURBO_POWER, LW6MAP_RULES_MAX_WEAPON_TUNE_TURBO_POWER);

  ret = 1;
  return ret;
}
