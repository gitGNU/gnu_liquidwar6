/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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
#include <math.h>

#include "gui.h"

typedef struct video_mode_sort_s
{
  lw6gui_video_mode_t video_mode;
  int distance;
} video_mode_sort_t;

static int
video_mode_sort_callback (lw6sys_list_t ** list_a, lw6sys_list_t ** list_b)
{
  int ret = 0;
  video_mode_sort_t *a, *b;

  a = (video_mode_sort_t *) ((*list_a)->data);
  b = (video_mode_sort_t *) ((*list_b)->data);

  if (a->distance < b->distance)
    {
      ret = -1;
    }
  else if (a->distance > b->distance)
    {
      ret = 1;
    }

  return ret;
}

static void
video_mode_copy_callback (void *func_data, void *data)
{
  lw6sys_list_t **target = (lw6sys_list_t **) func_data;
  lw6gui_video_mode_t *video_mode = (lw6gui_video_mode_t *) data;
  video_mode_sort_t *video_mode_sort = NULL;

  video_mode_sort =
    (video_mode_sort_t *) LW6SYS_MALLOC (sizeof (video_mode_sort_t));
  if (video_mode_sort)
    {
      video_mode_sort->video_mode = (*video_mode);
      video_mode_sort->distance = 0;
      lw6sys_lifo_push (target, video_mode_sort);
    }
}

static void
video_mode_distance_callback (void *func_data, void *data)
{
  lw6gui_video_mode_t *wished = (lw6gui_video_mode_t *) func_data;
  video_mode_sort_t *video_mode_sort = (video_mode_sort_t *) data;

  video_mode_sort->distance =
    (wished->width - video_mode_sort->video_mode.width) * (wished->width -
							   video_mode_sort->
							   video_mode.width) +
    (wished->height - video_mode_sort->video_mode.height) * (wished->height -
							     video_mode_sort->
							     video_mode.
							     height);
}

/**
 * lw6gui_video_mode_find_closest
 *
 * @closest: the closest video_mode found
 * @wished: the wished video_mode
 * @available: a list of available video_modes (list of lw6gui_video_mode_t *)
 *
 * Finds the closest video_mode available, this is just a small utility
 * to cope with different screen shapes and avoid requesting 640x480 when
 * it's just not available but there's a 640x400 instead.
 *
 * Return value: 1 if the wished video_mode exists in available list
 *   and was found, else 0 if the wished video_mode doesn't exist and
 *   an approximative match was picked.
 */
int
lw6gui_video_mode_find_closest (lw6gui_video_mode_t * closest,
				lw6gui_video_mode_t * wished,
				lw6sys_list_t * available)
{
  int ret = 0;
  lw6sys_list_t *sorted = NULL;

  *closest = *wished;

  sorted = lw6sys_list_new (lw6sys_free_callback);
  if (sorted)
    {
      if (available)
	{
	  lw6sys_list_map (available, &video_mode_copy_callback, &sorted);
	  lw6sys_list_map (sorted, &video_mode_distance_callback, wished);
	  lw6sys_sort (&sorted, video_mode_sort_callback);

	  if (sorted->data)
	    {
	      (*closest) = ((video_mode_sort_t *) (sorted->data))->video_mode;
	    }
	}
      lw6sys_list_free (sorted);
    }

  if (closest->width == wished->width && closest->height == wished->height)
    {
      ret = 1;
    }

  return ret;
}

/**
 * lw6gui_video_mode_is_same
 *
 * @mode_a: first mode to compare
 * @mode_b: second mode to compare
 *
 * Compares two video modes, to know if they're the same.
 *
 * Return value: 1 if equal, 0 if not.
 */
int
lw6gui_video_mode_is_same (lw6gui_video_mode_t * mode_a,
			   lw6gui_video_mode_t * mode_b)
{
  int ret = 0;

  ret = (mode_a->width == mode_b->width) && (mode_a->height == mode_b->height)
    && (mode_a->fullscreen == mode_b->fullscreen) ? 1 : 0;

  return ret;
}

/**
 * lw6gui_video_sync_ratio
 *
 * @dst: the target video mode
 * @src: the source video mode
 *
 * Applies the ratio of src to dst, for instance if src is 16/9, then
 * dst will be made 16/9 too, trying to keep the same surface.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6gui_video_mode_sync_ratio (lw6gui_video_mode_t * dst,
			      lw6gui_video_mode_t * src)
{
  int ret = 0;
  float coeff = 0.0f;

  if (src->width > 0 && src->height > 0)
    {
      if (dst->width > 0 && dst->height > 0)
	{
	  coeff =
	    sqrt (((float) dst->width * dst->height) /
		  ((float) src->width * src->height));
	  dst->width = src->width * coeff;
	  dst->height = src->height * coeff;
	  ret = 1;
	}
      else
	{
	  lw6sys_log (LW6SYS_LOG_WARNING,
		      _x_ ("can't handle dst resolution %dx%d"),
		      dst->width > 0 && dst->height > 0);
	}
    }
  else
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("can't handle src resolution %dx%d"), src->width > 0
		  && src->height > 0);
    }

  return ret;
}
