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
_lw6hlp_populate_references_doc (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 0;

  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_QUICK,
			 _x_ ("List quick help entries, this includes the GNU standard options and a few troubleshooting tools."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_DOC,
			 _x_
			 ("List documentation-related command line options. These commands allow you to list all the keywords related to a given domain."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_SHOW,
			 _x_
			 ("List command-line options which begin with '--show-...'. These will display on the console many internal parameters. Usefull when debugging."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_PATH,
			 _x_
			 ("List parameters which allow you to override the defaults of the game, and force the game your own file paths and directories."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_PLAYERS, _x_ ("List player-related entries, that is to say 'who plays'."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_INPUT,
			 _x_ ("List input (AKA controls) related options. Use these to change keyboard, joystick and mouse settingds."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_GRAPHICS, _x_ ("List graphics options (resolution, fullscreen...)."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_SOUND, _x_ ("List sound options (volume...)."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_NETWORK, _x_ ("List network options."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_MAP, _x_ ("List map-related entries, excluding rules.xml, hints.xml and style.xml entries."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_MAP_RULES,
			 _x_ ("List 'options.xml' entries. These parameters enable you to modify the gameplay."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_MAP_HINTS,
			 _x_ ("List 'hints.xml' entries. These parameters enable you to modify the behavior of the map loader."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_MAP_STYLE,
			 _x_ ("List 'style.xml' entries. These parameters enable you to modify the aspect of the game."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_MAP_TEAMS,
			 _x_ ("List 'teams.xml' entries. These parameters enable you to specify which teams should play on the map."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_FUNCS, _x_ ("List the C-functions which are exported to Guile, thus usable in scripts."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_HOOKS, _x_ ("List user-modifiable hooks."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_ADVANCED, _x_ ("List advanced options which can be used for fine-tuning the game."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_ALIASES, _x_ ("List the keyword aliases. These are here for convenience."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_TEAM_COLORS, _x_ ("List the team colors, there should be 10 of them"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST_WEAPONS, _x_ ("List the available weapons."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_EXAMPLE_RULES_XML,
			 _x_
			 ("Dumps on stdout an example options.xml file. Such a file is normally shipped with the game. It is indeed generated using this command."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_EXAMPLE_HINTS_XML,
			 _x_
			 ("Dumps on stdout an example hints.xml file. Such a file is normally shipped with the game. It is indeed generated using this command."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_EXAMPLE_STYLE_XML,
			 _x_
			 ("Dumps on stdout an example style.xml file. Such a file is normally shipped with the game. It is indeed generated using this command."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_EXAMPLE_TEAMS_XML,
			 _x_
			 ("Dumps on stdout an example teams.xml file. Such a file is normally shipped with the game. It is indeed generated using this command."));

  ret = 1;

  return ret;
}
