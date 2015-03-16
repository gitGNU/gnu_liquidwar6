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

#include "ker.h"
#include "ker-internal.h"

void
_lw6ker_zone_struct_update_checksum (lw6sys_context_t * sys_context, const _lw6ker_zone_struct_t * zone_struct, u_int32_t * checksum)
{
  int i;

  lw6sys_checksum_update_xyz (sys_context, checksum, &(zone_struct->pos));
  lw6sys_checksum_update_int32 (sys_context, checksum, zone_struct->size);
  for (i = 0; i < LW6KER_NB_DIRS; ++i)
    {
      lw6sys_checksum_update_int32 (sys_context, checksum, zone_struct->link[i]);
    }
  lw6sys_checksum_update_int32 (sys_context, checksum, zone_struct->one_way_dir_even);
  lw6sys_checksum_update_int32 (sys_context, checksum, zone_struct->one_way_dir_odd);
}
