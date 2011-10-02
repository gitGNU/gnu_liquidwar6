/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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

#include "map.h"

/**
 * lw6map_weapon_index_to_key
 *
 * @index: index of the weapon between 0 & 19
 *
 * Transforms a team weapon index into its readable string form,
 * which can be used in config files for instance.
 *
 * Return value: a string, must *not* be freed.
 */
char *
lw6map_weapon_index_to_key (int index)
{
  char *ret = LW6MAP_WEAPON_KEY_BERZERK;

  switch (index)
    {
    case LW6MAP_WEAPON_BERZERK:
      ret = LW6MAP_WEAPON_KEY_BERZERK;
      break;
    case LW6MAP_WEAPON_INVINCIBLE:
      ret = LW6MAP_WEAPON_KEY_INVINCIBLE;
      break;
    case LW6MAP_WEAPON_ESCAPE:
      ret = LW6MAP_WEAPON_KEY_ESCAPE;
      break;
    case LW6MAP_WEAPON_TURBO:
      ret = LW6MAP_WEAPON_KEY_TURBO;
      break;
    case LW6MAP_WEAPON_TELEPORT:
      ret = LW6MAP_WEAPON_KEY_TELEPORT;
      break;
    case LW6MAP_WEAPON_SCATTER:
      ret = LW6MAP_WEAPON_KEY_SCATTER;
      break;
    case LW6MAP_WEAPON_FIX:
      ret = LW6MAP_WEAPON_KEY_FIX;
      break;
    case LW6MAP_WEAPON_MIX:
      ret = LW6MAP_WEAPON_KEY_MIX;
      break;
    case LW6MAP_WEAPON_CONTROL:
      ret = LW6MAP_WEAPON_KEY_CONTROL;
      break;
    case LW6MAP_WEAPON_PERMUTATION:
      ret = LW6MAP_WEAPON_KEY_PERMUTATION;
      break;
    case LW6MAP_WEAPON_STEAL:
      ret = LW6MAP_WEAPON_KEY_STEAL;
      break;
    case LW6MAP_WEAPON_ATOMIC:
      ret = LW6MAP_WEAPON_KEY_ATOMIC;
      break;
    case LW6MAP_WEAPON_REVERSE:
      ret = LW6MAP_WEAPON_KEY_REVERSE;
      break;
    case LW6MAP_WEAPON_CRAZY:
      ret = LW6MAP_WEAPON_KEY_CRAZY;
      break;
    case LW6MAP_WEAPON_REWIND:
      ret = LW6MAP_WEAPON_KEY_REWIND;
      break;
    case LW6MAP_WEAPON_ATTRACT:
      ret = LW6MAP_WEAPON_KEY_ATTRACT;
      break;
    case LW6MAP_WEAPON_SHRINK:
      ret = LW6MAP_WEAPON_KEY_SHRINK;
      break;
    case LW6MAP_WEAPON_KAMIKAZE:
      ret = LW6MAP_WEAPON_KEY_KAMIKAZE;
      break;
    case LW6MAP_WEAPON_DISAPPEAR:
      ret = LW6MAP_WEAPON_KEY_DISAPPEAR;
      break;
    case LW6MAP_WEAPON_PLAGUE:
      ret = LW6MAP_WEAPON_KEY_PLAGUE;
      break;
    default:
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("team weapon index %d isn't valid"), index);
    }

  return ret;
}

/**
 * lw6map_weapon_key_to_index
 *
 * @key: key of the weapon, for instance "red"
 *
 * The index of the weapon, between 0 & 19
 *
 * Return value: an integer.
 */
int
lw6map_weapon_key_to_index (char *key)
{
  int ret = LW6MAP_WEAPON_NONE;

  if (!strcasecmp (key, LW6MAP_WEAPON_KEY_BERZERK))
    {
      ret = LW6MAP_WEAPON_BERZERK;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_INVINCIBLE))
    {
      ret = LW6MAP_WEAPON_INVINCIBLE;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_ESCAPE))
    {
      ret = LW6MAP_WEAPON_ESCAPE;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_TURBO))
    {
      ret = LW6MAP_WEAPON_TURBO;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_TELEPORT))
    {
      ret = LW6MAP_WEAPON_TELEPORT;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_SCATTER))
    {
      ret = LW6MAP_WEAPON_SCATTER;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_FIX))
    {
      ret = LW6MAP_WEAPON_FIX;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_MIX))
    {
      ret = LW6MAP_WEAPON_MIX;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_CONTROL))
    {
      ret = LW6MAP_WEAPON_CONTROL;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_PERMUTATION))
    {
      ret = LW6MAP_WEAPON_PERMUTATION;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_STEAL))
    {
      ret = LW6MAP_WEAPON_STEAL;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_ATOMIC))
    {
      ret = LW6MAP_WEAPON_ATOMIC;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_REVERSE))
    {
      ret = LW6MAP_WEAPON_REVERSE;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_CRAZY))
    {
      ret = LW6MAP_WEAPON_CRAZY;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_REWIND))
    {
      ret = LW6MAP_WEAPON_REWIND;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_ATTRACT))
    {
      ret = LW6MAP_WEAPON_ATTRACT;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_SHRINK))
    {
      ret = LW6MAP_WEAPON_SHRINK;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_KAMIKAZE))
    {
      ret = LW6MAP_WEAPON_KAMIKAZE;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_DISAPPEAR))
    {
      ret = LW6MAP_WEAPON_DISAPPEAR;
    }
  else if (!strcasecmp (key, LW6MAP_WEAPON_KEY_PLAGUE))
    {
      ret = LW6MAP_WEAPON_PLAGUE;
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING, _x_ ("weapon key %s isn't valid"), key);
    }

  return ret;
}
