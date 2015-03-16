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
_lw6hlp_populate_references_map (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 0;

  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_CHOSEN_MAP,
			_x_
			("The last map chosen by the player, locally. This is the map which will be used for a quick-start game, a local game, or a game started as a server."),
			"subflower");
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_USE_TEXTURE,
			 _x_
			 ("Defines wether the map texture should be used. Of course if there's no map texture, the texture... won't be used. But if there is one, this parameter will force the game to ignore it and play with solid colors. This probably won't look as nice as the textured map in most cases, but some players might find it more readable and confortable to play when throwing eye candy away."),
			 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_USE_CURSOR_TEXTURE,
			 _x_
			 ("Defines wether the cursor textures should be used. If unset, then the default builtin cursor texture will be used instead of the map specific one."),
			 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_USE_RULES_XML,
			 _x_
			 ("If set, then rules will be picked up from the map defined rules.xml, if it exists. This is the default. Use force-time and force-size to override this and use user-defined values anyway."),
			 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_USE_HINTS_XML,
			 _x_ ("If set, then hints will be picked up from the map defined hints.xml, if it exists. This is the default."), 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_USE_STYLE_XML,
			 _x_
			 ("If set, then style will be picked up from the map defined style.xml, if it exists. This is the default. Use force-time and force-background to override this and use user-defined values anyway."),
			 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_USE_TEAMS_XML,
			 _x_
			 ("If set, then teams will be picked up from the map defined teams.xml, if it exists. This is the default. Use force-time and force-background to override this and use user-defined values anyway."),
			 1);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_USE_MUSIC_FILE,
			 _x_
			 ("If set, then the program will use the 'music-file' attribute to choose the music to play. If unset, then a random builtin music will be picked up, regardless of what is specified in 'music-file'."),
			 1);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_FORCE,
			_x_
			("A comma separated list of options which should be ignored when reading map XML files. For instance, if this contains 'rounds-per-sec,moves-per-round' then whatever values were defined for this in 'rules.xml', then game will ignore them and use the user's values, stored in 'config.xml', running the game at the requested speed. This ultimately allows the player to control everything despite the values set by the map designer."),
			"respawn-team,color-conflict-mode");

  ret = 1;

  return ret;
}
