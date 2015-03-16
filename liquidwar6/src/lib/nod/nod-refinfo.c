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

#include "nod.h"
#include "nod-internal.h"

void
_lw6nod_ref_info_reset (lw6sys_context_t * sys_context, lw6nod_ref_info_t * ref_info)
{
  if (ref_info->id_str)
    {
      LW6SYS_FREE (sys_context, ref_info->id_str);
    }
  if (ref_info->url)
    {
      LW6SYS_FREE (sys_context, ref_info->url);
    }

  memset (ref_info, 0, sizeof (lw6nod_ref_info_t));
}

int
_lw6nod_ref_info_update (lw6sys_context_t * sys_context, lw6nod_ref_info_t * ref_info, u_int64_t id, const char *url)
{
  int ret = 1;

  if (ref_info->id_int != id)
    {
      ref_info->id_int = id;
      if (ref_info->id_str)
	{
	  LW6SYS_FREE (sys_context, ref_info->id_str);
	  ref_info->id_str = NULL;
	}
      ref_info->id_str = lw6sys_id_ltoa (sys_context, id);
    }

  /*
   * This test about pointers being different is both an optimization
   * (will run faster if pointer are equal as it does nothing) and a
   * protection, else we could free the pointer before duplicating it.
   */
  if (url != ref_info->url)
    {
      if (ref_info->url)
	{
	  LW6SYS_FREE (sys_context, ref_info->url);
	  ref_info->url = NULL;
	}
      if (url)
	{
	  ref_info->url = lw6sys_url_canonize (sys_context, url);
	}
      else
	{
	  /*
	   * url could technically be NULL if we just had a node id
	   * and/or do not know for sure the node url yet
	   */
	  ref_info->url = NULL;
	}
    }

  if (id != 0LL)
    {
      ret = (ref_info->id_str != NULL) && ret;
    }
  if (url != NULL)
    {
      ret = (ref_info->url != NULL) && ret;
    }

  return ret;
}
