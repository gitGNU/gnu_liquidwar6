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

#include <string.h>

#include "map.h"

/**
 * lw6map_color_invert
 *
 * @sys_context: global system context
 * @color: the color to invert
 *
 * Inverts a color couple, that is, replace fg by bg and vice-versa.
 *
 * Return value: none.
 */
void
lw6map_color_invert (lw6sys_context_t * sys_context, lw6map_color_couple_t * color)
{
  lw6sys_color_8_t tmp;

  tmp = color->fg;
  color->fg = color->bg;
  color->bg = tmp;
}

/**
 * lw6map_color_is_same
 *
 * @sys_context: global system context
 * @color1: 1st color to compare
 * @color2: 2nd color to compare
 *
 * Compares two colors.
 *
 * Return value: 1 if equal, 0 if not.
 */
int
lw6map_color_is_same (lw6sys_context_t * sys_context, const lw6map_color_couple_t * color1, const lw6map_color_couple_t * color2)
{
  return lw6sys_color_is_same (sys_context, color1->fg, color2->fg) && lw6sys_color_is_same (sys_context, color1->bg, color2->bg);
}

/**
 * lw6map_team_color_index_to_key
 *
 * @sys_context: global system context
 * @index: index of the color between 0 & 9
 *
 * Transforms a team color index into its readable string form,
 * which can be used in config files for instance.
 *
 * Return value: a string, must *not* be freed.
 */
char *
lw6map_team_color_index_to_key (lw6sys_context_t * sys_context, int index)
{
  char *ret = LW6DEF_RED;

  switch (index)
    {
    case LW6MAP_TEAM_COLOR_RED:
      ret = LW6DEF_RED;
      break;
    case LW6MAP_TEAM_COLOR_GREEN:
      ret = LW6DEF_GREEN;
      break;
    case LW6MAP_TEAM_COLOR_BLUE:
      ret = LW6DEF_BLUE;
      break;
    case LW6MAP_TEAM_COLOR_YELLOW:
      ret = LW6DEF_YELLOW;
      break;
    case LW6MAP_TEAM_COLOR_CYAN:
      ret = LW6DEF_CYAN;
      break;
    case LW6MAP_TEAM_COLOR_MAGENTA:
      ret = LW6DEF_MAGENTA;
      break;
    case LW6MAP_TEAM_COLOR_ORANGE:
      ret = LW6DEF_ORANGE;
      break;
    case LW6MAP_TEAM_COLOR_LIGHTBLUE:
      ret = LW6DEF_LIGHTBLUE;
      break;
    case LW6MAP_TEAM_COLOR_PURPLE:
      ret = LW6DEF_PURPLE;
      break;
    case LW6MAP_TEAM_COLOR_PINK:
      ret = LW6DEF_PINK;
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("team color index %d isn't valid"), index);
    }

  return ret;
}

/**
 * lw6map_team_color_key_to_index
 *
 * @sys_context: global system context
 * @key: key of the color, for instance "red"
 *
 * The index of the color, between 0 & 9
 *
 * Return value: an integer.
 */
int
lw6map_team_color_key_to_index (lw6sys_context_t * sys_context, const char *key)
{
  int ret = LW6MAP_TEAM_COLOR_INVALID;

  if (!strcasecmp (key, LW6DEF_RED))
    {
      ret = LW6MAP_TEAM_COLOR_RED;
    }
  else if (!strcasecmp (key, LW6DEF_GREEN))
    {
      ret = LW6MAP_TEAM_COLOR_GREEN;
    }
  else if (!strcasecmp (key, LW6DEF_BLUE))
    {
      ret = LW6MAP_TEAM_COLOR_BLUE;
    }
  else if (!strcasecmp (key, LW6DEF_YELLOW))
    {
      ret = LW6MAP_TEAM_COLOR_YELLOW;
    }
  else if (!strcasecmp (key, LW6DEF_CYAN))
    {
      ret = LW6MAP_TEAM_COLOR_CYAN;
    }
  else if (!strcasecmp (key, LW6DEF_MAGENTA))
    {
      ret = LW6MAP_TEAM_COLOR_MAGENTA;
    }
  else if (!strcasecmp (key, LW6DEF_ORANGE))
    {
      ret = LW6MAP_TEAM_COLOR_ORANGE;
    }
  else if (!strcasecmp (key, LW6DEF_LIGHTBLUE))
    {
      ret = LW6MAP_TEAM_COLOR_LIGHTBLUE;
    }
  else if (!strcasecmp (key, LW6DEF_PURPLE))
    {
      ret = LW6MAP_TEAM_COLOR_PURPLE;
    }
  else if (!strcasecmp (key, LW6DEF_PINK))
    {
      ret = LW6MAP_TEAM_COLOR_PINK;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("team color key %s isn't valid"), key);
    }

  return ret;
}

/**
 * lw6map_team_color_index_to_label
 *
 * @sys_context: global system context
 * @index: index of the color between 0 & 9
 *
 * Transforms a team color index into its readable string form,
 * which can be used to display information to players.
 *
 * Return value: a string, must *not* be freed.
 */
char *
lw6map_team_color_index_to_label (lw6sys_context_t * sys_context, int index)
{
  char *ret = LW6SYS_STR_EMPTY;

  switch (index)
    {
    case LW6MAP_TEAM_COLOR_RED:
      ret = _("Red");
      break;
    case LW6MAP_TEAM_COLOR_GREEN:
      ret = _("Green");
      break;
    case LW6MAP_TEAM_COLOR_BLUE:
      ret = _("Blue");
      break;
    case LW6MAP_TEAM_COLOR_YELLOW:
      ret = _("Yellow");;
      break;
    case LW6MAP_TEAM_COLOR_CYAN:
      ret = _("Cyan");
      break;
    case LW6MAP_TEAM_COLOR_MAGENTA:
      ret = _("Magenta");
      break;
    case LW6MAP_TEAM_COLOR_ORANGE:
      ret = _("Orange");
      break;
    case LW6MAP_TEAM_COLOR_LIGHTBLUE:
      ret = _("Light blue");
      break;
    case LW6MAP_TEAM_COLOR_PURPLE:
      ret = _("Purple");
      break;
    case LW6MAP_TEAM_COLOR_PINK:
      ret = _("Pink");
      break;
    default:
      ret = _("Invalid color");
    }

  return ret;
}
