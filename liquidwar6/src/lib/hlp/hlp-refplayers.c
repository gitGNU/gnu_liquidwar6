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

#include "hlp.h"
#include "hlp-internal.h"

int
_lw6hlp_populate_references_players (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 0;

  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_PLAYER1_NAME,
			_x_
			("Name of the first player, the player used by default. A default value is provided, you can of course, change it at will."),
			"<username>");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_PLAYER2_NAME,
			_x_ ("Name of the second player. A default value is provided, you'll certainly want to change it."), "player2-<hostname>");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_PLAYER3_NAME,
			_x_ ("Name of the third player. A default value is provided, you'll certainly want to change it."), "player3-<hostname>");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_PLAYER4_NAME,
			_x_ ("Name of the fourth player. A default value is provided, you'll certainly want to change it."), "player4-<hostname>");
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_PLAYER1_STATUS, _x_ ("Status of the first player, true if player is activated, false if idle."), 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_PLAYER2_STATUS, _x_ ("Status of the second player, true if player is activated, false if idle."), 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_PLAYER3_STATUS, _x_ ("Status of the third player, true if player is activated, false if idle."), 0);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_PLAYER4_STATUS, _x_ ("Status of the fourth player, true if player is activated, false if idle."), 0);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_PLAYER1_CONTROL,
			_x_ ("Control for the first player, must be mouse, keyboard, joystick1, joystick2 or custom."), "mouse");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_PLAYER2_CONTROL,
			_x_ ("Control for the second player, must be mouse, keyboard, joystick1, joystick2 or custom."), "keyboard");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_PLAYER3_CONTROL,
			_x_ ("Control for the third player, must be mouse, keyboard, joystick1, joystick2 or custom."), "joystick1");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_PLAYER4_CONTROL,
			_x_ ("Control for the fourth player, must be mouse, keyboard, joystick1, joystick2 or custom."), "joystick2");

  ret = 1;

  return ret;
}
