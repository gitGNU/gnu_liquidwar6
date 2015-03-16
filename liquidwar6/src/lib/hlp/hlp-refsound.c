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
_lw6hlp_populate_references_sound (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 0;

  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_FX_VOLUME,
			  _x_ ("Set the sound effects volume. This is a floating point value. 0 is mute. Maximum value is 1."), 0.3, 0, 1);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_WATER_VOLUME,
			  _x_ ("Set the volume for water sounds. This is a floating point value. 0 is mute. Maximum value is 1."), 0.2, 0, 1);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_MUSIC_VOLUME,
			  _x_ (" Set the music volume.This is a floating point value. 0 is mute. Maximum value is 1."), 0.6, 0, 1);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_SND_BACKEND,
			_x_ ("Sets the sound backend AKA 'snd' to use. Can be 'ogg' or 'csound' but only 'ogg' will produce sound in the current release."),
			"ogg");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_AMBIANCE_FILE,
			_x_ ("A music file which will be used to be played during the menus. If not found, game will fallback on random files."), "");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_AMBIANCE_FILTER,
			_x_
			("A music filter, used to select the files which are played while navigating in the menus. It works like 'music-filter' except this one is not related to a peculiar map. This is not a complex regex-enabled filter, just a plain string search. Even the '*' wildcard won't work."),
			"Chadburn");
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_AMBIANCE_EXCLUDE,
			_x_ ("If this string is present in a music file name, this file won't be played during the menus, it will be excluded from the list."),
			"");


  ret = 1;

  return ret;
}
