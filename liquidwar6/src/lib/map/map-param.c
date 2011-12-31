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

#include <string.h>

#include "map.h"

/**
 * lw6map_param_defaults:
 *
 * @param: the param struct to modify
 *
 * Sets a param structure to its default value, note that current
 * structured must be zeroed or correctly initialized.
 *
 * Return value: none
 */
void
lw6map_param_defaults (lw6map_param_t * param)
{
  lw6map_rules_defaults (&(param->rules));
  lw6map_style_defaults (&(param->style));
  lw6map_teams_defaults (&(param->teams));
}

/**
 * lw6map_param_clear:
 *
 * @param: the param struct to modify
 *
 * Resets a param structure to nothing. Note that current
 * structured must be zeroed or correctly initialized.
 * The idea is just to free member pointers before calling free.
 *
 * Return value: none
 */
void
lw6map_param_clear (lw6map_param_t * param)
{
  if (param)
    {
      lw6map_rules_clear (&(param->rules));
      lw6map_style_clear (&(param->style));
      lw6map_teams_clear (&(param->teams));

      /*
       * We don't free the param pointer itself, this
       * is a "clear" function, not a "free".
       */
    }
}

/**
 * lw6map_param_copy:
 *
 * @src: the source param struct
 * @dst: the destination param struct
 *
 * Copies parameters. Both structures must be zeroed or
 * correctly initialized.
 *
 * Return value: none
 */
void
lw6map_param_copy (lw6map_param_t * dst, lw6map_param_t * src)
{
  lw6map_rules_copy (&(dst->rules), &(src->rules));
  lw6map_style_copy (&(dst->style), &(src->style));
  lw6map_teams_copy (&(dst->teams), &(src->teams));
}

/**
 * lw6map_param_set:
 *
 * @param: the param struct to modify
 * @key: the name of the parameter to modify
 * @value: the value of the parameter to modify
 *
 * Sets an entry in a param struct. All values must be submitted
 * as strings, internally, the function will call atoi to convert
 * to integers if needed, for instance. It will also dispatch
 * automatically between rules, style and teams.
 *
 * Return value: 1 if parameter successfully set, 0 on error.
 */
int
lw6map_param_set (lw6map_param_t * param, const char *key, const char *value)
{
  int ret = 0;

  if (lw6map_style_set (&param->style, key, value))
    {
      // ok, it's a style member
      ret = 1;
    }
  else if (lw6map_teams_set (&param->teams, key, value))
    {
      // ok, it's a teams member
      ret = 1;
    }
  else
    {
      ret = lw6map_rules_set_int (&param->rules, key, lw6sys_atoi (value));
    }

  return ret;
}

/**
 * lw6map_param_get:
 *
 * @param: the param struct to query
 * @key: the name of the parameter to get
 *
 * Gets an entry from a param struct. All values returned
 * as strings, do not use this in performance bottlenecks,
 * this is just to export values to scripts, for instance.
 *
 * Return value: dynamically allocated string, NULL on error,
 *   might return a string containing 0 on bad keys.
 */
char *
lw6map_param_get (lw6map_param_t * param, const char *key)
{
  char *ret = NULL;

  ret = lw6map_style_get (&param->style, key);
  if (!ret)
    {
      ret = lw6map_teams_get (&param->teams, key);
      if (!ret)
	{
	  ret = lw6sys_itoa (lw6map_rules_get_int (&param->rules, key));
	}
    }

  return ret;
}

/**
 * lw6map_param_is_same
 *
 * @param_a: one struct to compare
 * @param_b: another struct to compare
 *
 * Compares the contents of two param structs.
 *
 * Return value: 1 if they contain the same thing, 0 if not
 */
int
lw6map_param_is_same (lw6map_param_t * param_a, lw6map_param_t * param_b)
{
  int ret = 1;

  ret = ret && lw6map_rules_is_same (&(param_a->rules), &(param_b->rules));
  ret = ret && lw6map_style_is_same (&(param_a->style), &(param_b->style));
  ret = ret && lw6map_teams_is_same (&(param_a->teams), &(param_b->teams));

  return ret;
}
