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

#include "sys.h"

#define _VERSION_SEP '.'

static void
_cut_at_2nd_version_sep (lw6sys_context_t * sys_context, char *version)
{
  char *end = NULL;

  end = strchr (version, _VERSION_SEP);
  if (end)
    {
      end = strchr (end + 1, _VERSION_SEP);
      if (end)
	{
	  (*end) = '\0';
	}
    }
}

/**
 * lw6sys_version_is_compatible
 *
 * @sys_context: global system context
 * @version_a: 1st version to compare
 * @version_b: 2nd version to compare
 *
 * Compares two versions and tells wether they are compatible or not.
 * Actually, it only checks that MAJOR.MINOR is the same in both cases.
 * As a side not, it's not case sensitive. In most LW6 relevant cases,
 * it's a moot issue since MAJOR.MINOR is a number, but well, just in case.
 *
 * Return value: 1 if compatible, 0 if not
 */
int
lw6sys_version_is_compatible (lw6sys_context_t * sys_context, const char *version_a, const char *version_b)
{
  int ret = 0;
  char *tmp_a = NULL;
  char *tmp_b = NULL;

  tmp_a = lw6sys_str_copy (sys_context, version_a);
  if (tmp_a)
    {
      tmp_b = lw6sys_str_copy (sys_context, version_b);
      if (tmp_b)
	{
	  _cut_at_2nd_version_sep (sys_context, tmp_a);
	  _cut_at_2nd_version_sep (sys_context, tmp_b);

	  ret = lw6sys_str_is_same_no_case (sys_context, tmp_a, tmp_b);

	  LW6SYS_FREE (sys_context, tmp_b);
	}
      LW6SYS_FREE (sys_context, tmp_a);
    }

  return ret;
}
