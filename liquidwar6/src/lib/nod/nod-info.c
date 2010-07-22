/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

lw6nod_info_t *
lw6nod_info_new (u_int64_t id, char *url, char *title, int bench,
		 int idle_screenshot_size, void *idle_screenshot_data)
{
  lw6nod_info_t *info = NULL;

  info = (lw6nod_info_t *) LW6SYS_CALLOC (sizeof (lw6nod_info_t));
  if (info)
    {
      info->mutex = lw6sys_mutex_create ();
      info->id = lw6sys_id_ltoa (id);
      info->url = lw6sys_str_copy (url);
      if (info->title && strlen (info->title) > 0)
	{
	  info->title = lw6sys_str_copy (title);
	}
      else
	{
	  info->title = lw6sys_get_hostname ();
	}
      info->bench = bench;
      lw6nod_info_not_playing (info);
      info->idle_screenshot_size = idle_screenshot_size;
      info->idle_screenshot_data = LW6SYS_MALLOC (idle_screenshot_size);
      if (info->idle_screenshot_data)
	{
	  memcpy (info->idle_screenshot_data, idle_screenshot_data,
		  idle_screenshot_size);
	}
      info->discovered_nodes = lw6sys_list_new (lw6sys_free_callback);
      info->verified_nodes = lw6sys_list_new (lw6sys_free_callback);

      if (info->mutex && info->id && info->url && info->title
	  && info->idle_screenshot_data && info->discovered_nodes
	  && info->verified_nodes)
	{
	  // ok
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _("unable to create nod_info object"));
	  lw6nod_info_free (info);
	  info = NULL;
	}
    }

  return info;
}

void
lw6nod_info_free (lw6nod_info_t * info)
{
  lw6nod_info_not_playing (info);
  if (info->mutex)
    {
      lw6sys_mutex_destroy (info->mutex);
    }
  if (info->id)
    {
      LW6SYS_FREE (info->id);
    }
  if (info->url)
    {
      LW6SYS_FREE (info->url);
    }
  if (info->title)
    {
      LW6SYS_FREE (info->title);
    }
  if (info->idle_screenshot_data)
    {
      LW6SYS_FREE (info->idle_screenshot_data);
    }
  if (info->discovered_nodes)
    {
      lw6sys_list_free (info->discovered_nodes);
    }
  if (info->verified_nodes)
    {
      lw6sys_list_free (info->verified_nodes);
    }
}

void
lw6nod_info_not_playing (lw6nod_info_t * info)
{
  if (info->level)
    {
      LW6SYS_FREE (info->level);
      info->level = lw6sys_str_copy ("");
    }
  info->limit = 0;
  info->colors = 0;
  info->nodes = 0;
  info->cursors = 0;
  if (info->game_screenshot_data)
    {
      LW6SYS_FREE (info->game_screenshot_data);
    }
}

int
lw6nod_info_update (lw6nod_info_t * info, char *level,
		    int limit,
		    int colors,
		    int nodes,
		    int cursors,
		    int game_screenshot_size, void *game_screenshot_data)
{
  if (info->level)
    {
      LW6SYS_FREE (info->level);
      info->level = lw6sys_str_copy (level);
    }
  info->limit = 0;
  info->colors = 0;
  info->nodes = 0;
  info->cursors = 0;
  info->game_screenshot_size = game_screenshot_size;
  if (info->game_screenshot_data)
    {
      LW6SYS_FREE (info->game_screenshot_data);
      info->game_screenshot_data = LW6SYS_MALLOC (game_screenshot_size);
      if (info->game_screenshot_data)
	{
	  memcpy (info->game_screenshot_data, game_screenshot_data,
		  game_screenshot_size);
	}
    }
}

int
lw6nod_info_add_discovered_node (lw6nod_info_t * info, char *public_url)
{
  int ret = 0;

  // todo

  return ret;
}

char *
lw6nod_info_pop_discovered_node (lw6nod_info_t * info)
{
  char *ret = NULL;

  // todo

  return ret;
}

int
lw6nod_info_set_verified_nodes (lw6nod_info_t * info, char *public_url)
{
  int ret = 0;

  // todo

  return ret;
}
