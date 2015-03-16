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

#include "ldr.h"
#include "ldr-internal.h"

/**
 * lw6ldr_param_read
 *
 * @sys_context: global system context
 * @param: the parameter struct to fill with values (read/write parameter)
 * @dirname: the directory of the map
 *
 * Read the parameters associated to a map. Pointer to param must be valid,
 * and values already initialized, either zeroed or filled in with defaults
 * or custom values.
 *
 * Return value: 1 if success, 0 if failed.
 */
int
lw6ldr_param_read (lw6sys_context_t * sys_context, lw6map_param_t * param, const char *dirname)
{
  int ret = 0;

  ret = lw6ldr_rules_read (sys_context, &(param->rules), dirname) && lw6ldr_style_read (sys_context, &(param->style), dirname);

  return ret;
}

/**
 * lw6ldr_param_update
 *
 * @sys_context: global system context
 * @param: the parameter struct to fill with values (read/write parameter)
 * @values: an assoc containing strings with the new values
 *
 * Overrides param with values. Pointer to param must be valid,
 * and values already initialized, either zeroed or filled in with defaults
 * or custom values. Not all parameters need be defined in values. It can
 * even be NULL. The idea is just that if something is defined in values,
 * it will override the existing param.
 *
 * Return value: 1 if success, 0 if failed.
 */
int
lw6ldr_param_update (lw6sys_context_t * sys_context, lw6map_param_t * param, lw6sys_assoc_t * values)
{
  int ret = 1;

  if (!lw6ldr_rules_update (sys_context, &(param->rules), values))
    {
      ret = 0;
    }

  if (!lw6ldr_style_update (sys_context, &(param->style), values))
    {
      ret = 0;
    }

  return ret;
}
