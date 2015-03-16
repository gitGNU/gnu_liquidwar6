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
#endif // HAVE_CONFIG_H

#include "ker.h"
#include "ker-internal.h"

#define _CAPTURE_BG ' '
#define _CAPTURE_FG '*'
#define _CAPTURE_COLOR_0 '0'

static int
_capture_size (int w, int h)
{
  int size = 0;

  size = h * (w + 1);

  return size;
}

static int
_capture_pos (int w, int x, int y)
{
  int pos = 0;

  pos = y * (w + 1) + x;

  return pos;
}

static int
_capture_eol (int w, int y)
{
  int eol = 0;

  eol = y * (w + 1) + w;

  return eol;
}

char *
_lw6ker_capture_str (lw6sys_context_t * sys_context, const _lw6ker_game_state_t * game_state)
{
  char *ret = NULL;
  int w, h, x, y;
  int size, pos;
  char c = '\0';
  int fighter_id;
  lw6ker_fighter_t *fighter;

  w = game_state->map_state.shape.w;
  h = game_state->map_state.shape.h;
  size = _capture_size (w, h);
  ret = (char *) LW6SYS_CALLOC (sys_context, size + 1);
  if (ret)
    {
      for (y = 0; y < h; ++y)
	{
	  for (x = 0; x < w; ++x)
	    {
	      pos = _capture_pos (w, x, y);
	      /*
	       * No multilayer support, only display layer0, this is
	       * just a preview/debugging function
	       */
	      fighter_id = _lw6ker_map_state_get_fighter_id (&(game_state->map_state), x, y, 0);
	      if (fighter_id >= 0)
		{
		  fighter = &(game_state->map_state.armies.fighters[fighter_id]);
		  c = _CAPTURE_COLOR_0 + fighter->team_color;
		}
	      else
		{
		  c = (lw6map_body_get (&(game_state->game_struct->level->body), x, y, 0) > 0) ? _CAPTURE_BG : _CAPTURE_FG;
		}
	      ret[pos] = c;
	    }
	  ret[_capture_eol (w, y)] = '\n';
	}
      ret[size] = '\0';
    }

  return ret;
}

/**
 * lw6ker_capture_str
 *
 * @sys_context: global system context
 * @game_state: game state to represent
 *
 * Gives a string representation, an ASCII capture of the game.
 * This representation is suitable for debugging, typically print
 * it to a VT100 console.
 *
 * Return value: dynamically allocated string.
 */
char *
lw6ker_capture_str (lw6sys_context_t * sys_context, const lw6ker_game_state_t * game_state)
{
  char *ret = NULL;

  ret = _lw6ker_capture_str (sys_context, (const _lw6ker_game_state_t *) game_state);

  return ret;
}
