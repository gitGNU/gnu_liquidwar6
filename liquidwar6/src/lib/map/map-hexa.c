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

static int
_push_metadata (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const lw6map_metadata_t * metadata)
{
  int ret = 1;

  ret = ret && lw6sys_hexa_serializer_push_str (sys_context, hexa_serializer, metadata->title);
  ret = ret && lw6sys_hexa_serializer_push_str (sys_context, hexa_serializer, metadata->author);
  ret = ret && lw6sys_hexa_serializer_push_str (sys_context, hexa_serializer, metadata->description);
  ret = ret && lw6sys_hexa_serializer_push_str (sys_context, hexa_serializer, metadata->license);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, metadata->vanilla_exp);

  return ret;
}

static int
_push_layer (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const lw6map_layer_t * layer)
{
  int ret = 1;
  int x, y;

  ret = ret && lw6sys_hexa_serializer_push_whd (sys_context, hexa_serializer, layer->shape);
  for (y = 0; y < layer->shape.h && ret; ++y)
    {
      for (x = 0; x < layer->shape.w && ret; ++x)
	{
	  ret = ret && lw6sys_hexa_serializer_push_int8 (sys_context, hexa_serializer, lw6map_layer_get (layer, x, y));
	}
    }

  return ret;
}

static int
_push_meta_layer (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const lw6map_meta_layer_t * meta_layer)
{
  int ret = 1;
  int x, y;

  ret = ret && lw6sys_hexa_serializer_push_whd (sys_context, hexa_serializer, meta_layer->shape);
  for (y = 0; y < meta_layer->shape.h && ret; ++y)
    {
      for (x = 0; x < meta_layer->shape.w && ret; ++x)
	{
	  ret = ret && lw6sys_hexa_serializer_push_int8 (sys_context, hexa_serializer, lw6map_meta_layer_get (sys_context, meta_layer, x, y));
	}
    }

  return ret;
}

static int
_push_body (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const lw6map_body_t * body)
{
  int ret = 1;
  int layer;

  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, body->checksum);
  ret = ret && lw6sys_hexa_serializer_push_whd (sys_context, hexa_serializer, body->shape);
  for (layer = 0; layer < body->shape.d && ret; ++layer)
    {
      ret = ret && _push_layer (sys_context, hexa_serializer, &(body->layers[layer]));
    }
  ret = ret && _push_meta_layer (sys_context, hexa_serializer, &(body->glue));
  ret = ret && _push_meta_layer (sys_context, hexa_serializer, &(body->boost));
  ret = ret && _push_meta_layer (sys_context, hexa_serializer, &(body->danger));
  ret = ret && _push_meta_layer (sys_context, hexa_serializer, &(body->medicine));
  ret = ret && _push_meta_layer (sys_context, hexa_serializer, &(body->one_way_north));
  ret = ret && _push_meta_layer (sys_context, hexa_serializer, &(body->one_way_east));
  ret = ret && _push_meta_layer (sys_context, hexa_serializer, &(body->one_way_south));
  ret = ret && _push_meta_layer (sys_context, hexa_serializer, &(body->one_way_west));

  return ret;
}

static int
_push_texture (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const lw6map_texture_t * texture)
{
  int ret = 1;
  int x, y;

  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, texture->w);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, texture->h);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, texture->has_alpha);
  for (y = 0; y < texture->h && ret; ++y)
    {
      for (x = 0; x < texture->w && ret; ++x)
	{
	  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, lw6map_texture_get (texture, x, y));
	}
    }
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, texture->guessed_color_base.bg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, texture->guessed_color_base.fg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, texture->guessed_color_alternate.bg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, texture->guessed_color_alternate.fg);

  return ret;
}

static int
_push_cursor_texture (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const lw6map_cursor_texture_t * cursor_texture)
{
  int ret = 1;
  int x, y;

  for (y = 0; y < LW6MAP_CURSOR_TEXTURE_SIZE && ret; ++y)
    {
      for (x = 0; x < LW6MAP_CURSOR_TEXTURE_SIZE && ret; ++x)
	{
	  ret = ret
	    && lw6sys_hexa_serializer_push_color (sys_context,
						  hexa_serializer, lw6map_cursor_texture_layer_get (sys_context, &(cursor_texture->fg_bg_layer), x, y));
	}
    }
  for (y = 0; y < LW6MAP_CURSOR_TEXTURE_SIZE && ret; ++y)
    {
      for (x = 0; x < LW6MAP_CURSOR_TEXTURE_SIZE && ret; ++x)
	{
	  ret = ret
	    && lw6sys_hexa_serializer_push_color (sys_context,
						  hexa_serializer, lw6map_cursor_texture_layer_get (sys_context, &(cursor_texture->color_layer), x, y));
	}
    }

  return ret;
}

static int
_push_param (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, const lw6map_param_t * param)
{
  int ret = 1;
  int i, value;
  const char *key;

  for (i = 0; (key = LW6MAP_RULES_LIST[i]) != NULL && ret; ++i)
    {
      value = lw6map_rules_get_int (sys_context, &(param->rules), key);
      ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, value);
    }

  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, param->style.keep_ratio);
  ret = ret && lw6sys_hexa_serializer_push_float (sys_context, hexa_serializer, param->style.zoom);
  ret = ret && lw6sys_hexa_serializer_push_float (sys_context, hexa_serializer, param->style.zoom_min);
  ret = ret && lw6sys_hexa_serializer_push_float (sys_context, hexa_serializer, param->style.zoom_max);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, param->style.x_wrap);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, param->style.y_wrap);
  ret = ret && lw6sys_hexa_serializer_push_str (sys_context, hexa_serializer, param->style.background_style);
  ret = ret && lw6sys_hexa_serializer_push_str (sys_context, hexa_serializer, param->style.hud_style);
  ret = ret && lw6sys_hexa_serializer_push_str (sys_context, hexa_serializer, param->style.menu_style);
  ret = ret && lw6sys_hexa_serializer_push_str (sys_context, hexa_serializer, param->style.view_style);
  ret = ret && lw6sys_hexa_serializer_push_float (sys_context, hexa_serializer, param->style.animation_density);
  ret = ret && lw6sys_hexa_serializer_push_float (sys_context, hexa_serializer, param->style.animation_speed);
  ret = ret && lw6sys_hexa_serializer_push_float (sys_context, hexa_serializer, param->style.cursor_size);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, param->style.colorize_cursor);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, param->style.blink_cursor);
  ret = ret && lw6sys_hexa_serializer_push_float (sys_context, hexa_serializer, param->style.hidden_layer_alpha);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, param->style.colorize);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, param->style.pixelize);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.color_base.bg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.color_base.fg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.color_alternate.bg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.color_alternate.fg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.background_color_root.bg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.background_color_root.fg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.background_color_stuff.bg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.background_color_stuff.fg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.hud_color_frame.bg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.hud_color_frame.fg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.hud_color_text.bg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.hud_color_text.fg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.menu_color_default.bg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.menu_color_default.fg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.menu_color_selected.bg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.menu_color_selected.fg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.menu_color_disabled.bg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.menu_color_disabled.fg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.view_color_cursor.bg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.view_color_cursor.fg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.view_color_map.bg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.view_color_map.fg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.system_color.bg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.system_color.fg);
  ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.team_color_dead);
  for (i = 0; i < LW6MAP_NB_TEAM_COLORS && ret; ++i)
    {
      ret = ret && lw6sys_hexa_serializer_push_color (sys_context, hexa_serializer, param->style.color_set.team_colors[i]);
    }
  ret = ret && lw6sys_hexa_serializer_push_str (sys_context, hexa_serializer, param->style.music_file);
  ret = ret && lw6sys_hexa_serializer_push_str (sys_context, hexa_serializer, param->style.music_filter);
  ret = ret && lw6sys_hexa_serializer_push_str (sys_context, hexa_serializer, param->style.music_exclude);
  ret = ret && lw6sys_hexa_serializer_push_int32 (sys_context, hexa_serializer, param->style.waves);

  return ret;
}

/**
 * lw6map_to_hexa
 *
 * @sys_context: global system context
 * @map: the map to convert
 *
 * Converts a map to something that is later readable by @lw6map_from_hexa
 * to reproduce the exact same map. Just a serializer.
 *
 * Return value: a newly allocated pointer, NULL if conversion failed.
 */
char *
lw6map_to_hexa (lw6sys_context_t * sys_context, const lw6map_level_t * level)
{
  char *ret = NULL;
  lw6sys_hexa_serializer_t *hexa_serializer = NULL;
  int ok = 1;

  hexa_serializer = lw6sys_hexa_serializer_new (sys_context, NULL);
  if (hexa_serializer)
    {
      ok = ok && _push_metadata (sys_context, hexa_serializer, &(level->metadata));
      ok = ok && _push_body (sys_context, hexa_serializer, &(level->body));
      ok = ok && _push_texture (sys_context, hexa_serializer, &(level->texture));
      ok = ok && _push_cursor_texture (sys_context, hexa_serializer, &(level->cursor_texture));
      ok = ok && _push_param (sys_context, hexa_serializer, &(level->param));
      if (ok)
	{
	  ret = lw6sys_hexa_serializer_as_string (sys_context, hexa_serializer);
	}
      lw6sys_hexa_serializer_free (sys_context, hexa_serializer);
    }

  return ret;
}

static int
_pop_metadata (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, lw6map_metadata_t * metadata)
{
  int ret = 1;

  ret = ret && lw6sys_hexa_serializer_pop_str (sys_context, hexa_serializer, &(metadata->title));
  ret = ret && lw6sys_hexa_serializer_pop_str (sys_context, hexa_serializer, &(metadata->author));
  ret = ret && lw6sys_hexa_serializer_pop_str (sys_context, hexa_serializer, &(metadata->description));
  ret = ret && lw6sys_hexa_serializer_pop_str (sys_context, hexa_serializer, &(metadata->license));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(metadata->vanilla_exp));

  return ret;
}

static int
_pop_layer (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, lw6map_layer_t * layer, lw6sys_whd_t * shape)
{
  int ret = 1;
  int x, y;
  int8_t c = 0;

  ret = ret && lw6sys_hexa_serializer_pop_whd (sys_context, hexa_serializer, &(layer->shape));
  if (layer->shape.w == shape->w && layer->shape.h == shape->h)
    {
      layer->data = (u_int8_t *) LW6SYS_CALLOC (sys_context, layer->shape.w * layer->shape.h);
      if (layer->data)
	{
	  for (y = 0; y < layer->shape.h && ret; ++y)
	    {
	      for (x = 0; x < layer->shape.w && ret; ++x)
		{
		  ret = ret && lw6sys_hexa_serializer_pop_int8 (sys_context, hexa_serializer, &c);
		  lw6map_layer_set (layer, x, y, c);
		}
	    }
	}
      else
	{
	  ret = 0;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("bad layer shape (%dx%d != %dx%d)"), layer->shape.w, layer->shape.h, shape->w, shape->h);
      ret = 0;
    }

  return ret;
}

static int
_pop_meta_layer (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, lw6map_meta_layer_t * meta_layer, lw6sys_whd_t * shape)
{
  int ret = 1;
  int x, y;
  int8_t c = 0;

  ret = ret && lw6sys_hexa_serializer_pop_whd (sys_context, hexa_serializer, &(meta_layer->shape));
  if (meta_layer->shape.w == shape->w && meta_layer->shape.h == shape->h)
    {
      meta_layer->data = (u_int8_t *) LW6SYS_CALLOC (sys_context, meta_layer->shape.w * meta_layer->shape.h);
      if (meta_layer->data)
	{
	  for (y = 0; y < meta_layer->shape.h && ret; ++y)
	    {
	      for (x = 0; x < meta_layer->shape.w && ret; ++x)
		{
		  ret = ret && lw6sys_hexa_serializer_pop_int8 (sys_context, hexa_serializer, &c);
		  lw6map_meta_layer_set (sys_context, meta_layer, x, y, c);
		}
	    }
	}
      else
	{
	  ret = 0;
	}
    }
  else
    {
      if (meta_layer->shape.w == 0 && meta_layer->shape.h == 0)
	{
	  meta_layer->data = NULL;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		      _x_ ("bad meta_layer shape (%dx%d != %dx%d)"), meta_layer->shape.w, meta_layer->shape.h, shape->w, shape->h);
	  ret = 0;
	}
    }

  return ret;
}

static int
_pop_body (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, lw6map_body_t * body)
{
  int ret = 1;
  int layer;
  lw6sys_whd_t shape_min = { LW6MAP_MIN_BODY_WIDTH, LW6MAP_MIN_BODY_HEIGHT, LW6MAP_MIN_BODY_DEPTH };
  lw6sys_whd_t shape_max = { LW6MAP_MAX_BODY_WIDTH, LW6MAP_MAX_BODY_HEIGHT, LW6MAP_MAX_BODY_DEPTH };

  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(body->checksum));
  ret = ret && lw6sys_hexa_serializer_pop_whd (sys_context, hexa_serializer, &(body->shape));
  if (lw6sys_shape_check_min_max_whd (sys_context, &body->shape, &shape_min, &shape_max))
    {
      for (layer = 0; layer < body->shape.d; ++layer)
	{
	  ret = ret && _pop_layer (sys_context, hexa_serializer, &(body->layers[layer]), &(body->shape));
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("map shape out of range (%dx%dx%d)"), body->shape.w, body->shape.h, body->shape.d);
      ret = 0;
    }
  ret = ret && _pop_meta_layer (sys_context, hexa_serializer, &(body->glue), &(body->shape));
  ret = ret && _pop_meta_layer (sys_context, hexa_serializer, &(body->boost), &(body->shape));
  ret = ret && _pop_meta_layer (sys_context, hexa_serializer, &(body->danger), &(body->shape));
  ret = ret && _pop_meta_layer (sys_context, hexa_serializer, &(body->medicine), &(body->shape));
  ret = ret && _pop_meta_layer (sys_context, hexa_serializer, &(body->one_way_north), &(body->shape));
  ret = ret && _pop_meta_layer (sys_context, hexa_serializer, &(body->one_way_east), &(body->shape));
  ret = ret && _pop_meta_layer (sys_context, hexa_serializer, &(body->one_way_south), &(body->shape));
  ret = ret && _pop_meta_layer (sys_context, hexa_serializer, &(body->one_way_west), &(body->shape));

  return ret;
}

static int
_pop_texture (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, lw6map_texture_t * texture)
{
  int ret = 1;
  int x, y;
  lw6sys_color_8_t color = LW6SYS_COLOR_8_BLACK;

  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(texture->w));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(texture->h));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(texture->has_alpha));
  if (texture->w >= LW6MAP_MIN_TEXTURE_WIDTH
      && texture->w <= LW6MAP_MAX_TEXTURE_WIDTH && texture->h >= LW6MAP_MIN_TEXTURE_HEIGHT && texture->h <= LW6MAP_MAX_TEXTURE_HEIGHT)
    {
      texture->data = (lw6sys_color_8_t *) LW6SYS_CALLOC (sys_context, texture->w * texture->h * sizeof (lw6sys_color_8_t));
      if (texture->data)
	{
	  for (y = 0; y < texture->h && ret; ++y)
	    {
	      for (x = 0; x < texture->w && ret; ++x)
		{
		  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &color);
		  lw6map_texture_set (texture, x, y, color);
		}
	    }
	}
      else
	{
	  ret = 0;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("texture shape out of range (%dx%d)"), texture->w, texture->h);
      ret = 0;
    }
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(texture->guessed_color_base.bg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(texture->guessed_color_base.fg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(texture->guessed_color_alternate.bg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(texture->guessed_color_alternate.fg));

  return ret;
}

static int
_pop_cursor_texture (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, lw6map_cursor_texture_t * cursor_texture)
{
  int ret = 1;
  int x, y;
  lw6sys_color_8_t color = { 0, 0, 0, 0 };

  for (y = 0; y < LW6MAP_CURSOR_TEXTURE_SIZE && ret; ++y)
    {
      for (x = 0; x < LW6MAP_CURSOR_TEXTURE_SIZE && ret; ++x)
	{
	  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &color);
	  lw6map_cursor_texture_layer_set (sys_context, &(cursor_texture->fg_bg_layer), x, y, color);
	}
    }
  for (y = 0; y < LW6MAP_CURSOR_TEXTURE_SIZE && ret; ++y)
    {
      for (x = 0; x < LW6MAP_CURSOR_TEXTURE_SIZE && ret; ++x)
	{
	  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &color);
	  lw6map_cursor_texture_layer_set (sys_context, &(cursor_texture->color_layer), x, y, color);
	}
    }

  return ret;
}

static int
_pop_param (lw6sys_context_t * sys_context, lw6sys_hexa_serializer_t * hexa_serializer, lw6map_param_t * param)
{
  int ret = 1;
  int32_t value;
  int i;
  const char *key;

  for (i = 0; (key = LW6MAP_RULES_LIST[i]) != NULL && ret; ++i)
    {
      ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &value);
      if (ret)
	{
	  lw6map_rules_set_int (sys_context, &(param->rules), key, value);
	}
    }
  ret = ret && lw6map_rules_sanity_check (sys_context, &(param->rules));

  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(param->style.keep_ratio));
  ret = ret && lw6sys_hexa_serializer_pop_float (sys_context, hexa_serializer, &(param->style.zoom));
  ret = ret && lw6sys_hexa_serializer_pop_float (sys_context, hexa_serializer, &(param->style.zoom_min));
  ret = ret && lw6sys_hexa_serializer_pop_float (sys_context, hexa_serializer, &(param->style.zoom_max));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(param->style.x_wrap));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(param->style.y_wrap));
  ret = ret && lw6sys_hexa_serializer_pop_str (sys_context, hexa_serializer, &(param->style.background_style));
  ret = ret && lw6sys_hexa_serializer_pop_str (sys_context, hexa_serializer, &(param->style.hud_style));
  ret = ret && lw6sys_hexa_serializer_pop_str (sys_context, hexa_serializer, &(param->style.menu_style));
  ret = ret && lw6sys_hexa_serializer_pop_str (sys_context, hexa_serializer, &(param->style.view_style));
  ret = ret && lw6sys_hexa_serializer_pop_float (sys_context, hexa_serializer, &(param->style.animation_density));
  ret = ret && lw6sys_hexa_serializer_pop_float (sys_context, hexa_serializer, &(param->style.animation_speed));
  ret = ret && lw6sys_hexa_serializer_pop_float (sys_context, hexa_serializer, &(param->style.cursor_size));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(param->style.colorize_cursor));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(param->style.blink_cursor));
  ret = ret && lw6sys_hexa_serializer_pop_float (sys_context, hexa_serializer, &(param->style.hidden_layer_alpha));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(param->style.colorize));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(param->style.pixelize));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.color_base.bg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.color_base.fg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.color_alternate.bg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.color_alternate.fg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.background_color_root.bg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.background_color_root.fg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.background_color_stuff.bg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.background_color_stuff.fg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.hud_color_frame.bg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.hud_color_frame.fg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.hud_color_text.bg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.hud_color_text.fg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.menu_color_default.bg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.menu_color_default.fg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.menu_color_selected.bg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.menu_color_selected.fg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.menu_color_disabled.bg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.menu_color_disabled.fg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.view_color_cursor.bg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.view_color_cursor.fg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.view_color_map.bg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.view_color_map.fg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.system_color.bg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.system_color.fg));
  ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.team_color_dead));
  for (i = 0; i < LW6MAP_NB_TEAM_COLORS && ret; ++i)
    {
      ret = ret && lw6sys_hexa_serializer_pop_color (sys_context, hexa_serializer, &(param->style.color_set.team_colors[i]));
    }
  ret = ret && lw6sys_hexa_serializer_pop_str (sys_context, hexa_serializer, &(param->style.music_file));
  ret = ret && lw6sys_hexa_serializer_pop_str (sys_context, hexa_serializer, &(param->style.music_filter));
  ret = ret && lw6sys_hexa_serializer_pop_str (sys_context, hexa_serializer, &(param->style.music_exclude));
  ret = ret && lw6sys_hexa_serializer_pop_int32 (sys_context, hexa_serializer, &(param->style.waves));

  return ret;
}

/**
 * lw6map_from_hexa
 *
 * @sys_context: global system context
 * @hexa: an hexadecimal ASCII string, created by @lw6map_to_hexa
 *
 * Constructs a map from an hexadecimal string generated
 * by @lw6map_to_hexa. Just an un-serializer.
 *
 * Return value: a new map, might be NULL if string isn't correct.
 */
lw6map_level_t *
lw6map_from_hexa (lw6sys_context_t * sys_context, const char *hexa)
{
  lw6map_level_t *level = NULL;
  lw6sys_hexa_serializer_t *hexa_serializer;
  int ok = 1;

  hexa_serializer = lw6sys_hexa_serializer_new (sys_context, hexa);
  if (hexa_serializer)
    {
      lw6sys_hexa_serializer_rewind (sys_context, hexa_serializer);
      level = lw6map_new (sys_context);
      if (level)
	{
	  ok = ok && _pop_metadata (sys_context, hexa_serializer, &(level->metadata));
	  ok = ok && _pop_body (sys_context, hexa_serializer, &(level->body));
	  ok = ok && _pop_texture (sys_context, hexa_serializer, &(level->texture));
	  ok = ok && _pop_cursor_texture (sys_context, hexa_serializer, &(level->cursor_texture));
	  ok = ok && _pop_param (sys_context, hexa_serializer, &(level->param));
	  if (!lw6sys_hexa_serializer_eof (sys_context, hexa_serializer))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("expected EOF in serialized level"));
	      ok = 0;
	    }
	  if (!ok)
	    {
	      lw6map_free (sys_context, level);
	      level = NULL;
	    }
	}
      lw6sys_hexa_serializer_free (sys_context, hexa_serializer);
    }

  return level;
}
