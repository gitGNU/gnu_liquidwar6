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
_lw6hlp_populate_references_map_teams (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 0;

  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_PLAYER1_COLOR,
			_x_
			("Color of the first player, must be red, green, blue, yellow, cyan, magenta, orange, lightblue, purple or pink"),
			LW6MAP_TEAMS_DEFAULT_PLAYER1_COLOR);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_PLAYER2_COLOR,
			_x_
			("Color of the second player, must be red, green, blue, yellow, cyan, magenta, orange, lightblue, purple or pink"),
			LW6MAP_TEAMS_DEFAULT_PLAYER2_COLOR);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_PLAYER3_COLOR,
			_x_
			("Color of the third player, must be red, green, blue, yellow, cyan, magenta, orange, lightblue, purple or pink"),
			LW6MAP_TEAMS_DEFAULT_PLAYER3_COLOR);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_PLAYER4_COLOR,
			_x_
			("Color of the fourth player, must be red, green, blue, yellow, cyan, magenta, orange, lightblue, purple or pink"),
			LW6MAP_TEAMS_DEFAULT_PLAYER4_COLOR);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_NB_BOTS,
			_x_
			("Number of bots on the map. 0 means no bots, if set to 1 the the bot1-... settings will be used, if set to 2 then bot1-... and bot2-... will be used, and so on."),
			LW6MAP_TEAMS_DEFAULT_NB_BOTS, LW6MAP_TEAMS_MIN_NB_BOTS, LW6MAP_TEAMS_MAX_NB_BOTS);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_BOT_SPEED,
			  _x_
			  ("The speed of bots, 1 means normal speed, higher value will speed it up, lower will slow it down. Note that this only has an impact on bot engines, not on the game speed itself."),
			  LW6MAP_TEAMS_DEFAULT_BOT_SPEED, 0, 0);
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_BOT_IQ,
			_x_
			("The IQ (intelligence quotient) of bots. Typically, a value of 100 will make the bot behave normally, performing at its best. A value of 0 will just make it act the worst way it can. Values over 100 probably won't change anything compared to 100, but this truely depends on which bot backend you're running."),
			LW6MAP_TEAMS_DEFAULT_BOT_IQ, LW6MAP_TEAMS_MIN_BOT_IQ, LW6MAP_TEAMS_MAX_BOT_IQ);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT1_COLOR, _x_ ("Color for bot number 1."), LW6MAP_TEAMS_DEFAULT_BOT1_COLOR);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT2_COLOR, _x_ ("Color for bot number 2."), LW6MAP_TEAMS_DEFAULT_BOT2_COLOR);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT3_COLOR, _x_ ("Color for bot number 3."), LW6MAP_TEAMS_DEFAULT_BOT3_COLOR);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT4_COLOR, _x_ ("Color for bot number 4."), LW6MAP_TEAMS_DEFAULT_BOT4_COLOR);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT5_COLOR, _x_ ("Color for bot number 5."), LW6MAP_TEAMS_DEFAULT_BOT5_COLOR);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT6_COLOR, _x_ ("Color for bot number 6."), LW6MAP_TEAMS_DEFAULT_BOT6_COLOR);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT7_COLOR, _x_ ("Color for bot number 7."), LW6MAP_TEAMS_DEFAULT_BOT7_COLOR);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT8_COLOR, _x_ ("Color for bot number 8."), LW6MAP_TEAMS_DEFAULT_BOT8_COLOR);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT9_COLOR, _x_ ("Color for bot number 9."), LW6MAP_TEAMS_DEFAULT_BOT9_COLOR);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT1_AI, _x_ ("AI engine for bot number 1."), LW6MAP_TEAMS_DEFAULT_BOT1_AI);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT2_AI, _x_ ("AI engine for bot number 2."), LW6MAP_TEAMS_DEFAULT_BOT2_AI);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT3_AI, _x_ ("AI engine for bot number 3."), LW6MAP_TEAMS_DEFAULT_BOT3_AI);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT4_AI, _x_ ("AI engine for bot number 4."), LW6MAP_TEAMS_DEFAULT_BOT4_AI);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT5_AI, _x_ ("AI engine for bot number 5."), LW6MAP_TEAMS_DEFAULT_BOT5_AI);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT6_AI, _x_ ("AI engine for bot number 6."), LW6MAP_TEAMS_DEFAULT_BOT6_AI);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT7_AI, _x_ ("AI engine for bot number 7."), LW6MAP_TEAMS_DEFAULT_BOT7_AI);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT8_AI, _x_ ("AI engine for bot number 8."), LW6MAP_TEAMS_DEFAULT_BOT8_AI);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_BOT9_AI, _x_ ("AI engine for bot number 9."), LW6MAP_TEAMS_DEFAULT_BOT9_AI);

  ret = 1;

  return ret;
}
