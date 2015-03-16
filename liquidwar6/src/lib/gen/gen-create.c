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

#include "gen.h"
#include "gen-internal.h"

/**
 * lw6gen_create_from_seed
 *
 * @sys_context: global system context
 * @seed: a random seed used to generate the map
 * @map_w: width of the screen, in pixels
 * @map_h: height of the screen, in pixels
 *
 * Builds a complete, usable map from the string passed in seed. Each triplet
 * composed of seed/map_w/map_h should build the same map on any computer,
 * in any context, for a given version of the game (no backwards compatibility).
 *
 * Return value: new map on success, NULL on failure.
 */
lw6map_level_t *
lw6gen_create_from_seed (lw6sys_context_t * sys_context, const char *seed, int map_w, int map_h)
{
  lw6map_level_t *level = NULL;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("creating pseudo-random map from seed \"%s\" with size %dx%d"), seed, map_w, map_h);

  /*
   * Todo, meanwhile, there's a placeholder here creating the default map
   * Hack here!
   */

  level = lw6map_builtin_custom (sys_context, map_w, map_h, LW6MAP_BUILTIN_DEFAULT_DEPTH, LW6MAP_BUILTIN_DEFAULT_NOISE_PERCENT);

  return level;
}
