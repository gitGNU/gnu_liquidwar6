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

#include "map.h"

/**
 * lw6map_dup:
 *
 * @sys_context: global system context
 * @source: the map to copy
 * @progress: to show advancement
 *
 * Performs a deep copy of the map, all elements are newly allocated
 * and source can safely be destroyed after it's been duplicated.
 *
 * Return value: a newly allocated map, may be NULL.
 */
lw6map_level_t *
lw6map_dup (lw6sys_context_t * sys_context, lw6map_level_t * source, lw6sys_progress_t * progress)
{
  lw6map_level_t *ret = NULL;
  int size = 0;
  u_int32_t id;
  int layer = 0;
  int progress_i = 0;
  int progress_n = 1;

  lw6sys_progress_begin (sys_context, progress);
  progress_n = source->body.shape.d + 3;

  ret = lw6map_new (sys_context);
  if (ret)
    {
      id = ret->id;
      memcpy (ret, source, sizeof (lw6map_level_t));
      ret->id = id;

      /*
       * We replace those pointers with NULLs, normally this is not
       * needed but in case the implementation below fails *or*
       * a pointer is not correctly allocated we end up with a
       * NULL and avoid the much more complex to debug "double free".
       */
      ret->metadata.title = NULL;
      ret->metadata.author = NULL;
      ret->metadata.description = NULL;
      ret->metadata.license = NULL;
      ret->local_info.music_dir = NULL;
      for (layer = 0; layer < LW6MAP_MAX_BODY_DEPTH; ++layer)
	{
	  ret->body.layers[layer].data = NULL;
	}
      ret->body.glue.data = NULL;
      ret->body.boost.data = NULL;
      ret->body.danger.data = NULL;
      ret->body.medicine.data = NULL;
      ret->body.one_way_north.data = NULL;
      ret->body.one_way_east.data = NULL;
      ret->body.one_way_south.data = NULL;
      ret->body.one_way_west.data = NULL;
      ret->texture.data = NULL;

      memset (&(ret->param), 0, sizeof (lw6map_param_t));
      lw6map_param_copy (sys_context, &(ret->param), &(source->param));
      if (source->metadata.title)
	{
	  ret->metadata.title = lw6sys_str_copy (sys_context, source->metadata.title);
	}
      if (source->metadata.author)
	{
	  ret->metadata.author = lw6sys_str_copy (sys_context, source->metadata.author);
	}
      if (source->metadata.description)
	{
	  ret->metadata.description = lw6sys_str_copy (sys_context, source->metadata.description);
	}
      if (source->metadata.license)
	{
	  ret->metadata.license = lw6sys_str_copy (sys_context, source->metadata.license);
	}
      if (source->local_info.music_dir)
	{
	  ret->local_info.music_dir = lw6sys_str_copy (sys_context, source->local_info.music_dir);
	}
      lw6sys_progress_update (sys_context, progress, 0, progress_n, ++progress_i);
      for (layer = 0; layer < ret->body.shape.d; ++layer)
	{
	  size = source->body.layers[layer].shape.w * source->body.layers[layer].shape.h * sizeof (u_int8_t);
	  ret->body.layers[layer].data = LW6SYS_MALLOC (sys_context, size);
	  if (ret->body.layers[layer].data)
	    {
	      memcpy (ret->body.layers[layer].data, source->body.layers[layer].data, size);
	      lw6sys_progress_update (sys_context, progress, 0, progress_n, ++progress_i);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to duplicate layer %d data"), layer);

	    }
	}
      if (source->body.glue.data)
	{
	  size = source->body.glue.shape.w * source->body.glue.shape.h * sizeof (u_int8_t);
	  ret->body.glue.data = LW6SYS_MALLOC (sys_context, size);
	  if (ret->body.glue.data)
	    {
	      memcpy (ret->body.glue.data, source->body.glue.data, size);
	      lw6sys_progress_update (sys_context, progress, 0, progress_n, ++progress_i);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to duplicate meta-layer"));

	    }
	}
      if (source->body.boost.data)
	{
	  size = source->body.boost.shape.w * source->body.boost.shape.h * sizeof (u_int8_t);
	  ret->body.boost.data = LW6SYS_MALLOC (sys_context, size);
	  if (ret->body.boost.data)
	    {
	      memcpy (ret->body.boost.data, source->body.boost.data, size);
	      lw6sys_progress_update (sys_context, progress, 0, progress_n, ++progress_i);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to duplicate meta-layer"));

	    }
	}
      if (source->body.danger.data)
	{
	  size = source->body.danger.shape.w * source->body.danger.shape.h * sizeof (u_int8_t);
	  ret->body.danger.data = LW6SYS_MALLOC (sys_context, size);
	  if (ret->body.danger.data)
	    {
	      memcpy (ret->body.danger.data, source->body.danger.data, size);
	      lw6sys_progress_update (sys_context, progress, 0, progress_n, ++progress_i);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to duplicate meta-layer"));

	    }
	}
      if (source->body.medicine.data)
	{
	  size = source->body.medicine.shape.w * source->body.medicine.shape.h * sizeof (u_int8_t);
	  ret->body.medicine.data = LW6SYS_MALLOC (sys_context, size);
	  if (ret->body.medicine.data)
	    {
	      memcpy (ret->body.medicine.data, source->body.medicine.data, size);
	      lw6sys_progress_update (sys_context, progress, 0, progress_n, ++progress_i);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to duplicate meta-layer"));

	    }
	}
      if (source->body.one_way_north.data)
	{
	  size = source->body.one_way_north.shape.w * source->body.one_way_north.shape.h * sizeof (u_int8_t);
	  ret->body.one_way_north.data = LW6SYS_MALLOC (sys_context, size);
	  if (ret->body.one_way_north.data)
	    {
	      memcpy (ret->body.one_way_north.data, source->body.one_way_north.data, size);
	      lw6sys_progress_update (sys_context, progress, 0, progress_n, ++progress_i);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to duplicate meta-layer"));

	    }
	}
      if (source->body.one_way_east.data)
	{
	  size = source->body.one_way_east.shape.w * source->body.one_way_east.shape.h * sizeof (u_int8_t);
	  ret->body.one_way_east.data = LW6SYS_MALLOC (sys_context, size);
	  if (ret->body.one_way_east.data)
	    {
	      memcpy (ret->body.one_way_east.data, source->body.one_way_east.data, size);
	      lw6sys_progress_update (sys_context, progress, 0, progress_n, ++progress_i);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to duplicate meta-layer"));

	    }
	}
      if (source->body.one_way_south.data)
	{
	  size = source->body.one_way_south.shape.w * source->body.one_way_south.shape.h * sizeof (u_int8_t);
	  ret->body.one_way_south.data = LW6SYS_MALLOC (sys_context, size);
	  if (ret->body.one_way_south.data)
	    {
	      memcpy (ret->body.one_way_south.data, source->body.one_way_south.data, size);
	      lw6sys_progress_update (sys_context, progress, 0, progress_n, ++progress_i);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to duplicate meta-layer"));

	    }
	}
      if (source->body.one_way_west.data)
	{
	  size = source->body.one_way_west.shape.w * source->body.one_way_west.shape.h * sizeof (u_int8_t);
	  ret->body.one_way_west.data = LW6SYS_MALLOC (sys_context, size);
	  if (ret->body.one_way_west.data)
	    {
	      memcpy (ret->body.one_way_west.data, source->body.one_way_west.data, size);
	      lw6sys_progress_update (sys_context, progress, 0, progress_n, ++progress_i);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to duplicate meta-layer"));

	    }
	}
      if (source->texture.data)
	{
	  size = source->texture.w * source->texture.h * sizeof (lw6sys_color_8_t);
	  ret->texture.data = LW6SYS_MALLOC (sys_context, size);
	  if (ret->texture.data)
	    {
	      memcpy (ret->texture.data, source->texture.data, size);
	      lw6sys_progress_update (sys_context, progress, 0, progress_n, ++progress_i);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to duplicate texture data"));
	    }
	}
    }
  lw6sys_progress_end (sys_context, progress);

  return ret;
}
