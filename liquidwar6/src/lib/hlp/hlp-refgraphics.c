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

#include "../gui/gui.h"

#include "hlp.h"
#include "hlp-internal.h"

int
_lw6hlp_populate_references_graphics (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 0;

  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_WIDTH,
			_x_
			("Run the game with the given screen width. Note that the graphics backend might ignore this hint. Use with its companion option 'height'.A negative value will force the use of a default value."),
			-1, 0, 0);;
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_HEIGHT,
			_x_
			("Run the game with the given screen height.Note that the graphics backend might ignore this hint. Use with its companion option 'width'. A negative value will force the use of a default value."),
			-1, 0, 0);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_FULLSCREEN,
			 _x_ ("Force the game to fun fullscreen. Note that the graphics backend might ignore this hint."), LW6GUI_DEFAULT_FULLSCREEN);
  _LW6HLP_POPULATE_STR (sys_context, references, LW6DEF_GFX_BACKEND,
			_x_
			("Sets the graphics backend AKA 'gfx' to use. For now the only reasonnable choice is 'gl1' and will use an OpenGL v1  / SDL 3D-accelerated driver."),
			"gl1");
  _LW6HLP_POPULATE_INT (sys_context, references, LW6DEF_GFX_QUALITY,
			_x_
			("Sets the overall quality of the graphics backend. Depending on the backend, this can mean different things. For instance for the 'gl' backend, this can change texture filtering (nearest, linear, bilinear...). This is not the same as 'pixelize' which is a per-map option and emulates an old school appearance."),
			LW6GUI_GFX_QUALITY_STANDARD, LW6GUI_GFX_QUALITY_LOW, LW6GUI_GFX_QUALITY_HIGH);
  _LW6HLP_POPULATE_FLOAT (sys_context, references, LW6DEF_WINDOWED_MODE_LIMIT,
			  _x_
			  ("When switching back from fullscreen mode to windowed mode, if we're in maximum resolution, then this coefficient will be applied before resizing the window. The idea is that (obviously) a windowed mode is prefered when a little smaller that totally fullscreen. So set this to a value just below 1.0."),
			  0.95, 0, 0);
  _LW6HLP_POPULATE_BOOL (sys_context, references, LW6DEF_CAPTURE,
			 _x_
			 ("Enables capture mode, in which a BMP file is dumped on the disk (in your user directory, search for a 'capture' sub-directory)."),
			 0);

  ret = 1;
  return ret;
}
