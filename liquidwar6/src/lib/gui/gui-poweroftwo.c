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

#include "gui.h"

/**
 * lw6gui_power_of_two_ge
 *
 * @sys_context: global system context
 * @input: the value to approach
 *
 * Finds the closest power of two, which is at least greater
 * or equal to input. Typically used to size textures.
 *
 * Return value: a power of two.
 */
int
lw6gui_power_of_two_ge (lw6sys_context_t * sys_context, int input)
{
  int value = 1;

  while (value < input)
    {
      value <<= 1;
    }
  return value;
}

/**
 * lw6gui_power_of_two_le
 *
 * @sys_context: global system context
 * @input: the value to approach
 *
 * Finds the closest power of two, which is equal of inferior
 * to input. Typically used to size textures.
 *
 * Return value: a power of two.
 */
int
lw6gui_power_of_two_le (lw6sys_context_t * sys_context, int input)
{
  int value = 1;

  while ((value << 1) <= input)
    {
      value <<= 1;
    }

  return value;
}
