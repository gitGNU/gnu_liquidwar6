/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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

#define _BUILTIN_NAME "builtin"

/**
 * lw6map_metadata_defaults
 *
 * @sys_context: global system context
 * @metadata: struct to set to defaults
 *
 * Sets the metadata struct to defaults, this does not set
 * fields to NULL/empty values, but rather fills it with
 * data claiming, for instance, that this is a default map.
 *
 * Return value: none.
 */
void
lw6map_metadata_defaults (lw6sys_context_t * sys_context,
			  lw6map_metadata_t * metadata)
{
  lw6map_metadata_clear (sys_context, metadata);

  metadata->title = lw6sys_new_sprintf (sys_context, _x_ ("Default map"));
  metadata->author =
    lw6sys_str_copy (sys_context, lw6sys_build_get_package_tarname ());
  metadata->description =
    lw6sys_new_sprintf (sys_context,
			_x_ ("Default built-in map generated by %s"),
			lw6sys_build_get_package_string ());
  metadata->license =
    lw6sys_str_copy (sys_context, lw6sys_build_get_license ());
  metadata->vanilla_exp = LW6MAP_RULES_DEFAULT_EXP;

  if ((!metadata->title) || (!metadata->author) || (!metadata->description)
      || (!metadata->license))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_ ("unable to construct default map metadata"));
    }
}

/**
 * lw6map_metadata_clear
 *
 * @sys_context: global system context
 * @metadata: struct to clear
 *
 * Clears a metadata, will expect it to be in a consistent
 * state, that is either filled with proper values or
 * completely zeroed.
 *
 * Return value: none.
 */
void
lw6map_metadata_clear (lw6sys_context_t * sys_context,
		       lw6map_metadata_t * metadata)
{
  if (metadata)
    {
      if (metadata->title)
	{
	  LW6SYS_FREE (sys_context, metadata->title);
	}
      if (metadata->author)
	{
	  LW6SYS_FREE (sys_context, metadata->author);
	}
      if (metadata->description)
	{
	  LW6SYS_FREE (sys_context, metadata->description);
	}
      if (metadata->license)
	{
	  LW6SYS_FREE (sys_context, metadata->license);
	}
      memset (metadata, 0, sizeof (lw6map_metadata_t));

      /*
       * We don't free the metadata pointer itself, this
       * is a "clear" function, not a "free".
       */
    }
}

/**
 * lw6map_metadata_is_same
 *
 * @sys_context: global system context
 * @metadata_a: first item to compare
 * @metadata_b: second item to compare
 *
 * Tells wether both metadata items contain the same values.
 *
 * Return value: 1 if same, 0 if different.
 */
int
lw6map_metadata_is_same (lw6sys_context_t * sys_context,
			 const lw6map_metadata_t * metadata_a,
			 const lw6map_metadata_t * metadata_b)
{
  int ret = 1;

  ret = ret
    && lw6sys_str_is_same (sys_context, metadata_a->title, metadata_b->title);
  ret = ret
    && lw6sys_str_is_same (sys_context, metadata_a->author,
			   metadata_b->author);
  ret = ret
    && lw6sys_str_is_same (sys_context, metadata_a->description,
			   metadata_b->description);
  ret = ret
    && lw6sys_str_is_same (sys_context, metadata_a->license,
			   metadata_b->license);
  ret = ret && (metadata_a->vanilla_exp == metadata_b->vanilla_exp);

  return ret;
}
