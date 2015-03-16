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
 * lw6map_local_info_set_music_dir
 *
 * @sys_context: global system context
 * @local_info: the structure to modify
 * @music_dir: the new music_dir value
 *
 * Sets the music_dir value, in a 'safe' manner, freeing any previous
 * value and performing a string duplication.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6map_local_info_set_music_dir (lw6sys_context_t * sys_context, lw6map_local_info_t * local_info, const char *music_dir)
{
  int ret = 0;

  if (local_info->music_dir)
    {
      LW6SYS_FREE (sys_context, local_info->music_dir);
      local_info->music_dir = NULL;
    }
  if (music_dir)
    {
      local_info->music_dir = lw6sys_str_copy (sys_context, music_dir);
    }
  ret = (local_info->music_dir != NULL);

  return ret;
}

/**
 * lw6map_local_info_clear
 *
 * @sys_context: global system context
 * @local_info: the structure to clear
 *
 * Clears the local_info structure, before destroying a level for instance.
 *
 * Return value: none
 */
void
lw6map_local_info_clear (lw6sys_context_t * sys_context, lw6map_local_info_t * local_info)
{
  if (local_info->music_dir)
    {
      LW6SYS_FREE (sys_context, local_info->music_dir);
    }
  memset (&local_info, 0, sizeof (lw6map_local_info_t));
}
