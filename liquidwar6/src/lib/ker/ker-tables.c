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

/*
 * MOVE_? offset or used when moving fighters,
 * and calculating "what's in that direction?".
 */
_lw6ker_tables_move_offset_t _LW6KER_TABLES_MOVE_X_OFFSET = { 1, 1, 1, 0, 0, -1, -1, -1, -1, 0, 0, 1, 0, 0 };
_lw6ker_tables_move_offset_t _LW6KER_TABLES_MOVE_Y_OFFSET = { 0, 0, 1, 1, 1, 1, 0, 0, -1, -1, -1, -1, 0, 0 };
_lw6ker_tables_move_offset_t _LW6KER_TABLES_MOVE_Z_OFFSET = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1 };

/*
 * Used to try a direction or another one, when something
 * is blocking.
 */
_lw6ker_tables_move_dir_t _LW6KER_TABLES_MOVE_DIR = {
  {				// parity 0
   {
    LW6KER_DIR_ENE, LW6KER_DIR_NE, LW6KER_DIR_SE, LW6KER_DIR_NNE,
    LW6KER_DIR_SSE, LW6KER_DIR_NW, LW6KER_DIR_SW}
   ,
   {
    LW6KER_DIR_ESE, LW6KER_DIR_SE, LW6KER_DIR_NE, LW6KER_DIR_SSE,
    LW6KER_DIR_NNE, LW6KER_DIR_SW, LW6KER_DIR_NW}
   ,
   {
    LW6KER_DIR_SE, LW6KER_DIR_SSE, LW6KER_DIR_ESE, LW6KER_DIR_SW,
    LW6KER_DIR_NE, LW6KER_DIR_WSW, LW6KER_DIR_NNE}
   ,
   {
    LW6KER_DIR_SSE, LW6KER_DIR_SE, LW6KER_DIR_SW, LW6KER_DIR_ESE,
    LW6KER_DIR_WSW, LW6KER_DIR_NE, LW6KER_DIR_NW}
   ,
   {
    LW6KER_DIR_SSW, LW6KER_DIR_SW, LW6KER_DIR_SE, LW6KER_DIR_WSW,
    LW6KER_DIR_ESE, LW6KER_DIR_NW, LW6KER_DIR_NE}
   ,
   {
    LW6KER_DIR_SW, LW6KER_DIR_WSW, LW6KER_DIR_SSW, LW6KER_DIR_NW,
    LW6KER_DIR_SE, LW6KER_DIR_NNW, LW6KER_DIR_ESE}
   ,
   {
    LW6KER_DIR_WSW, LW6KER_DIR_SW, LW6KER_DIR_NW, LW6KER_DIR_SSW,
    LW6KER_DIR_NNW, LW6KER_DIR_SE, LW6KER_DIR_NE}
   ,
   {
    LW6KER_DIR_WNW, LW6KER_DIR_NW, LW6KER_DIR_SW, LW6KER_DIR_NNW,
    LW6KER_DIR_SSE, LW6KER_DIR_NE, LW6KER_DIR_SE}
   ,
   {
    LW6KER_DIR_NW, LW6KER_DIR_NNW, LW6KER_DIR_WNW, LW6KER_DIR_NE,
    LW6KER_DIR_SW, LW6KER_DIR_ENE, LW6KER_DIR_SSW}
   ,
   {
    LW6KER_DIR_NNW, LW6KER_DIR_NW, LW6KER_DIR_NE, LW6KER_DIR_WNW,
    LW6KER_DIR_ENE, LW6KER_DIR_SW, LW6KER_DIR_SE}
   ,
   {
    LW6KER_DIR_NNE, LW6KER_DIR_NE, LW6KER_DIR_NW, LW6KER_DIR_ENE,
    LW6KER_DIR_WNW, LW6KER_DIR_SE, LW6KER_DIR_SW}
   ,
   {
    LW6KER_DIR_NE, LW6KER_DIR_ENE, LW6KER_DIR_NNE, LW6KER_DIR_SE,
    LW6KER_DIR_NW, LW6KER_DIR_SSE, LW6KER_DIR_WNW}
   ,
   {
    LW6KER_DIR_DOWN, LW6KER_DIR_NNE, LW6KER_DIR_NE, LW6KER_DIR_ENE,
    LW6KER_DIR_ESE, LW6KER_DIR_SE, LW6KER_DIR_SSE}
   ,
   {
    LW6KER_DIR_UP, LW6KER_DIR_SSW, LW6KER_DIR_SW, LW6KER_DIR_WSW,
    LW6KER_DIR_WNW, LW6KER_DIR_NW, LW6KER_DIR_NNW}
   }
  ,
  {				// parity 1
   {
    LW6KER_DIR_ENE, LW6KER_DIR_NE, LW6KER_DIR_SE, LW6KER_DIR_NNE,
    LW6KER_DIR_SSE, LW6KER_DIR_NW, LW6KER_DIR_SW}
   ,
   {
    LW6KER_DIR_ESE, LW6KER_DIR_SE, LW6KER_DIR_NE, LW6KER_DIR_SSE,
    LW6KER_DIR_NNE, LW6KER_DIR_SW, LW6KER_DIR_NW}
   ,
   {
    LW6KER_DIR_SE, LW6KER_DIR_ESE, LW6KER_DIR_SSE, LW6KER_DIR_NE,
    LW6KER_DIR_SW, LW6KER_DIR_NNE, LW6KER_DIR_WSW}
   ,
   {
    LW6KER_DIR_SSE, LW6KER_DIR_SE, LW6KER_DIR_SW, LW6KER_DIR_ESE,
    LW6KER_DIR_WSW, LW6KER_DIR_NE, LW6KER_DIR_NW}
   ,
   {
    LW6KER_DIR_SSW, LW6KER_DIR_SW, LW6KER_DIR_SE, LW6KER_DIR_WSW,
    LW6KER_DIR_ESE, LW6KER_DIR_NW, LW6KER_DIR_NE}
   ,
   {
    LW6KER_DIR_SW, LW6KER_DIR_SSW, LW6KER_DIR_WSW, LW6KER_DIR_SE,
    LW6KER_DIR_NW, LW6KER_DIR_ESE, LW6KER_DIR_NNW}
   ,
   {
    LW6KER_DIR_WSW, LW6KER_DIR_SW, LW6KER_DIR_NW, LW6KER_DIR_SSW,
    LW6KER_DIR_NNW, LW6KER_DIR_SE, LW6KER_DIR_NE}
   ,
   {
    LW6KER_DIR_WNW, LW6KER_DIR_NW, LW6KER_DIR_SW, LW6KER_DIR_NNW,
    LW6KER_DIR_SSE, LW6KER_DIR_NE, LW6KER_DIR_SE}
   ,
   {
    LW6KER_DIR_NW, LW6KER_DIR_WNW, LW6KER_DIR_NNW, LW6KER_DIR_SW,
    LW6KER_DIR_NE, LW6KER_DIR_SSW, LW6KER_DIR_ENE}
   ,
   {
    LW6KER_DIR_NNW, LW6KER_DIR_NW, LW6KER_DIR_NE, LW6KER_DIR_WNW,
    LW6KER_DIR_ENE, LW6KER_DIR_SW, LW6KER_DIR_SE}
   ,
   {
    LW6KER_DIR_NNE, LW6KER_DIR_NE, LW6KER_DIR_NW, LW6KER_DIR_ENE,
    LW6KER_DIR_WNW, LW6KER_DIR_SE, LW6KER_DIR_SW}
   ,
   {
    LW6KER_DIR_NE, LW6KER_DIR_NNE, LW6KER_DIR_ENE, LW6KER_DIR_NW,
    LW6KER_DIR_SE, LW6KER_DIR_WNW, LW6KER_DIR_SSE}
   ,
   {
    LW6KER_DIR_DOWN, LW6KER_DIR_WSW, LW6KER_DIR_SW, LW6KER_DIR_SSW,
    LW6KER_DIR_SSE, LW6KER_DIR_SE, LW6KER_DIR_ESE}
   ,
   {
    LW6KER_DIR_UP, LW6KER_DIR_ENE, LW6KER_DIR_NE, LW6KER_DIR_NNE,
    LW6KER_DIR_NNW, LW6KER_DIR_NW, LW6KER_DIR_WNW}
   }
};

/*
 * Used to find the direction "a la" LW (that is, one of the 12 ones)
 * when having an information like "this is on my right". Note that
 * it can contain invalid dirs (-1) in case we are just here and
 * need not move.
 */
_lw6ker_tables_straight_dirs_t _LW6KER_TABLES_STRAIGHT_DIRS = {
  {				// parity 0
   -1,				// just here
   LW6KER_DIR_NNE,		// top
   LW6KER_DIR_ESE,		// right
   LW6KER_DIR_NE,		// top+right
   LW6KER_DIR_SSW,		// down
   -1,				// top+down
   LW6KER_DIR_SE,		// right+down
   -1,				// top+right+down
   LW6KER_DIR_WNW,		// left
   LW6KER_DIR_NW,		// top+left
   -1,				// right+left
   -1,				// top+right+left
   LW6KER_DIR_SW,		// down+left
   -1,				// top+down+left
   -1,				// right+down+left
   -1				// top+right+down+left
   }
  ,
  {				// parity 1
   -1,				// just here
   LW6KER_DIR_NNW,		// top
   LW6KER_DIR_ENE,		// right
   LW6KER_DIR_NE,		// top+right
   LW6KER_DIR_SSE,		// down
   -1,				// top+down
   LW6KER_DIR_SE,		// right+down
   -1,				// top+right+down
   LW6KER_DIR_WSW,		// left
   LW6KER_DIR_NW,		// top+left
   -1,				// right+left
   -1,				// top+right+left
   LW6KER_DIR_SW,		// down+left
   -1,				// top+down+left
   -1,				// right+down+left
   -1,				// top+right+down+left
   }
};
