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

#include "map.h"

/**
 * lw6map_weapon_index_to_key
 *
 * @sys_context: global system context
 * @index: index of the weapon between 0 & 19
 *
 * Transforms a team weapon index into its readable string form,
 * which can be used in config files for instance.
 *
 * Return value: a string, must *not* be freed.
 */
char *
lw6map_weapon_index_to_key (lw6sys_context_t * sys_context, int index)
{
  char *ret = LW6DEF_BERZERK;

  switch (index)
    {
    case LW6MAP_WEAPON_BERZERK:
      ret = LW6DEF_BERZERK;
      break;
    case LW6MAP_WEAPON_INVINCIBLE:
      ret = LW6DEF_INVINCIBLE;
      break;
    case LW6MAP_WEAPON_ESCAPE:
      ret = LW6DEF_ESCAPE;
      break;
    case LW6MAP_WEAPON_TURBO:
      ret = LW6DEF_TURBO;
      break;
    case LW6MAP_WEAPON_TELEPORT:
      ret = LW6DEF_TELEPORT;
      break;
    case LW6MAP_WEAPON_SCATTER:
      ret = LW6DEF_SCATTER;
      break;
    case LW6MAP_WEAPON_FIX:
      ret = LW6DEF_FIX;
      break;
    case LW6MAP_WEAPON_MIX:
      ret = LW6DEF_MIX;
      break;
    case LW6MAP_WEAPON_CONTROL:
      ret = LW6DEF_CONTROL;
      break;
    case LW6MAP_WEAPON_PERMUTATION:
      ret = LW6DEF_PERMUTATION;
      break;
    case LW6MAP_WEAPON_STEAL:
      ret = LW6DEF_STEAL;
      break;
    case LW6MAP_WEAPON_ATOMIC:
      ret = LW6DEF_ATOMIC;
      break;
    case LW6MAP_WEAPON_REVERSE:
      ret = LW6DEF_REVERSE;
      break;
    case LW6MAP_WEAPON_CRAZY:
      ret = LW6DEF_CRAZY;
      break;
    case LW6MAP_WEAPON_REWIND:
      ret = LW6DEF_REWIND;
      break;
    case LW6MAP_WEAPON_ATTRACT:
      ret = LW6DEF_ATTRACT;
      break;
    case LW6MAP_WEAPON_SHRINK:
      ret = LW6DEF_SHRINK;
      break;
    case LW6MAP_WEAPON_KAMIKAZE:
      ret = LW6DEF_KAMIKAZE;
      break;
    case LW6MAP_WEAPON_DISAPPEAR:
      ret = LW6DEF_DISAPPEAR;
      break;
    case LW6MAP_WEAPON_PLAGUE:
      ret = LW6DEF_PLAGUE;
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("weapon index %d isn't valid"), index);
    }

  return ret;
}

/**
 * lw6map_weapon_key_to_index
 *
 * @sys_context: global system context
 * @key: key of the weapon, for instance "red"
 *
 * The index of the weapon, between 0 & 19
 *
 * Return value: an integer.
 */
int
lw6map_weapon_key_to_index (lw6sys_context_t * sys_context, const char *key)
{
  int ret = LW6MAP_WEAPON_NONE;

  if (!strcasecmp (key, LW6DEF_BERZERK))
    {
      ret = LW6MAP_WEAPON_BERZERK;
    }
  else if (!strcasecmp (key, LW6DEF_INVINCIBLE))
    {
      ret = LW6MAP_WEAPON_INVINCIBLE;
    }
  else if (!strcasecmp (key, LW6DEF_ESCAPE))
    {
      ret = LW6MAP_WEAPON_ESCAPE;
    }
  else if (!strcasecmp (key, LW6DEF_TURBO))
    {
      ret = LW6MAP_WEAPON_TURBO;
    }
  else if (!strcasecmp (key, LW6DEF_TELEPORT))
    {
      ret = LW6MAP_WEAPON_TELEPORT;
    }
  else if (!strcasecmp (key, LW6DEF_SCATTER))
    {
      ret = LW6MAP_WEAPON_SCATTER;
    }
  else if (!strcasecmp (key, LW6DEF_FIX))
    {
      ret = LW6MAP_WEAPON_FIX;
    }
  else if (!strcasecmp (key, LW6DEF_MIX))
    {
      ret = LW6MAP_WEAPON_MIX;
    }
  else if (!strcasecmp (key, LW6DEF_CONTROL))
    {
      ret = LW6MAP_WEAPON_CONTROL;
    }
  else if (!strcasecmp (key, LW6DEF_PERMUTATION))
    {
      ret = LW6MAP_WEAPON_PERMUTATION;
    }
  else if (!strcasecmp (key, LW6DEF_STEAL))
    {
      ret = LW6MAP_WEAPON_STEAL;
    }
  else if (!strcasecmp (key, LW6DEF_ATOMIC))
    {
      ret = LW6MAP_WEAPON_ATOMIC;
    }
  else if (!strcasecmp (key, LW6DEF_REVERSE))
    {
      ret = LW6MAP_WEAPON_REVERSE;
    }
  else if (!strcasecmp (key, LW6DEF_CRAZY))
    {
      ret = LW6MAP_WEAPON_CRAZY;
    }
  else if (!strcasecmp (key, LW6DEF_REWIND))
    {
      ret = LW6MAP_WEAPON_REWIND;
    }
  else if (!strcasecmp (key, LW6DEF_ATTRACT))
    {
      ret = LW6MAP_WEAPON_ATTRACT;
    }
  else if (!strcasecmp (key, LW6DEF_SHRINK))
    {
      ret = LW6MAP_WEAPON_SHRINK;
    }
  else if (!strcasecmp (key, LW6DEF_KAMIKAZE))
    {
      ret = LW6MAP_WEAPON_KAMIKAZE;
    }
  else if (!strcasecmp (key, LW6DEF_DISAPPEAR))
    {
      ret = LW6MAP_WEAPON_DISAPPEAR;
    }
  else if (!strcasecmp (key, LW6DEF_PLAGUE))
    {
      ret = LW6MAP_WEAPON_PLAGUE;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("weapon key %s isn't valid"), key);
    }

  return ret;
}

/**
 * lw6map_weapon_index_to_label
 *
 * @sys_context: global system context
 * @index: index of the weapon between 0 & 19
 *
 * Transforms a team weapon index into its readable string form,
 * which can be used to display information to players.
 *
 * Return value: a string, must *not* be freed.
 */
char *
lw6map_weapon_index_to_label (lw6sys_context_t * sys_context, int index)
{
  char *ret = LW6SYS_STR_EMPTY;

  switch (index)
    {
    case LW6MAP_WEAPON_BERZERK:
      ret = _("Berzerk");
      break;
    case LW6MAP_WEAPON_INVINCIBLE:
      ret = _("Invincible");
      break;
    case LW6MAP_WEAPON_ESCAPE:
      ret = _("Escape");
      break;
    case LW6MAP_WEAPON_TURBO:
      ret = _("Turbo");
      break;
    case LW6MAP_WEAPON_TELEPORT:
      ret = _("Teleport");
      break;
    case LW6MAP_WEAPON_SCATTER:
      ret = _("Scatter");
      break;
    case LW6MAP_WEAPON_FIX:
      ret = _("Fix");
      break;
    case LW6MAP_WEAPON_MIX:
      ret = _("Mix");
      break;
    case LW6MAP_WEAPON_CONTROL:
      ret = _("Control");
      break;
    case LW6MAP_WEAPON_PERMUTATION:
      ret = _("Permutation");
      break;
    case LW6MAP_WEAPON_STEAL:
      ret = _("Steal");
      break;
    case LW6MAP_WEAPON_ATOMIC:
      ret = _("Atomic");
      break;
    case LW6MAP_WEAPON_REVERSE:
      ret = _("Reverse");
      break;
    case LW6MAP_WEAPON_CRAZY:
      ret = _("Crazy");
      break;
    case LW6MAP_WEAPON_REWIND:
      ret = _("Rewind");
      break;
    case LW6MAP_WEAPON_ATTRACT:
      ret = _("Attract");
      break;
    case LW6MAP_WEAPON_SHRINK:
      ret = _("Shrink");
      break;
    case LW6MAP_WEAPON_KAMIKAZE:
      ret = _("Kamikaze");
      break;
    case LW6MAP_WEAPON_DISAPPEAR:
      ret = _("Disappear");
      break;
    case LW6MAP_WEAPON_PLAGUE:
      ret = _("Plague");
      break;
    default:
      ret = _("No weapon");
    }

  return ret;
}
