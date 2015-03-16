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

#include "hlp.h"

/**
 * lw6hlp_match
 *
 * @sys_context: global system context
 * @keyword1: the 1st keyword
 * @keyword2: the 2nd keyword
 *
 * Checks wether a keyword matches another. Not only a string
 * comparison, will also try and guess if the error is only
 * about dash "-" replaced by underscode "_", for instance.
 *
 * Return value: 1 if matches, 0 if different.
 */
int
lw6hlp_match (lw6sys_context_t * sys_context, const char *keyword1, const char *keyword2)
{
  int ret = 0;
  char *key1 = NULL;
  char *key2 = NULL;

  key1 = lw6sys_keyword_as_key (sys_context, keyword1);
  if (key1)
    {
      key2 = lw6sys_keyword_as_key (sys_context, keyword2);
      if (key2)
	{
	  ret = !strcasecmp (key1, key2);
	  LW6SYS_FREE (sys_context, key2);
	}
      LW6SYS_FREE (sys_context, key1);
    }

  return ret;
}
