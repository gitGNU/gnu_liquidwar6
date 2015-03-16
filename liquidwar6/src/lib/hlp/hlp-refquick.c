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
_lw6hlp_populate_references_quick (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 0;

  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_HELP, _x_ ("Returns a short help for the program."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_COPYRIGHT, _x_ ("Returns the copyright notice for the program."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_VERSION, _x_ ("Returns the version of the program, as defined by the GNU Coding Standards."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_TEST,
			 _x_
			 ("Runs a (hopefully) complete test suite which will call most internal Liquid War 6 functions and check out wether they work, in a simple context, without any game interference. Usefull for troubleshooting."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_DEFAULTS,
			 _x_
			 ("Clears the config file and run the game with default settings. Use this if you suspect you have broken something by tweaking user settings, or when upgrading the game to a new version."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIST,
			 _x_
			 ("Returns the list of all keywords which can be queried for information. This includes command-line options, environment variables, and so on. This is the companion option of '--about'. Results obtained with '--list' can be passed to '--about'."));
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_ABOUT,
			_x_
			("Will allow you to get informations about a given keyword. Let's say that, for instance, you want informations about the keyword 'map-path'. Simply run 'liquidwar6 --about=map-path'. Note that this internal self-documentation system can describe command line switches as well as XML config file parameters or environment variables, and even some Guile script functions. The '--list' command line switch will give you the list of all available keywords."),
			NULL);
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_DEBUG,
			 _x_
			 ("Enables debug mode. This will turn on maximum log information, and display everything on stderr, even messages which are normally only stored in the log file."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_PEDIGREE,
			 _x_
			 ("Display all build values, these are general constants which can help debugging, tracing what binary you are running, and so on. It's a good idea to take a look at the output of 'pedigree' if you have problems running the game."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_HOST, _x_ ("Display all known system host properties, including os and cpu informations."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_AUDIT,
			 _x_
			 ("Display all path values, defaults and current settings. This output is very usefull to track down problems such as missing directories, broken installations. If you get an error message that suggests some file is missing, then give this option a try."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_MODULES,
			 _x_
			 ("Tells which modules have been enabled when the game was compiled. It's still possible to add or remove modules afterwards, but this option allows you to know how things were at first."));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_CREDITS, _x_ ("Gives hopefully extensive information on who contributed to the game."));

  ret = 1;

  return ret;
}
