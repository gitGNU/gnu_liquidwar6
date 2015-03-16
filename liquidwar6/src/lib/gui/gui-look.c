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

#include "gui.h"

#define DEFAULT_DYNAMIC_ZOOM 1.0f

static void
_fix_zoom (lw6sys_context_t * sys_context, lw6gui_look_t * look)
{
  if (look->style.zoom <= 0.0f)
    {
      look->style.zoom = LW6MAP_STYLE_DEFAULT_ZOOM;
    }
  if (look->dynamic_zoom * look->style.zoom > look->style.zoom_max)
    {
      look->dynamic_zoom = look->style.zoom_max / look->style.zoom;
    }
  if (look->dynamic_zoom * look->style.zoom < look->style.zoom_min)
    {
      look->dynamic_zoom = look->style.zoom_min / look->style.zoom;
    }
}

static void
_fix_gfx_quality (lw6sys_context_t * sys_context, lw6gui_look_t * look)
{
  if (look->gfx_quality < LW6GUI_GFX_QUALITY_LOW)
    {
      look->gfx_quality = LW6GUI_GFX_QUALITY_LOW;
    }
  if (look->gfx_quality > LW6GUI_GFX_QUALITY_HIGH)
    {
      look->gfx_quality = LW6GUI_GFX_QUALITY_HIGH;
    }
}

/*
 * This is used to stamp maps as they are created.
 * Note that there's no race condition due to the fact that this
 * is global here, in fact even when 2 processes would share
 * this sequence id, it would not matter for they would then
 * try and identify the objects in their on per-process lists,
 * structures, Guile object, whatever they use.
 */
static volatile u_int32_t seq_id = 0;

/**
 * lw6gui_look_new
 *
 * @sys_context: global system context
 * @map_style: map_style to use as a base
 *
 * Create a new look object, a look is basically a style plus
 * some dynamic parameters.
 *
 * Return value: newly created object.
 */
lw6gui_look_t *
lw6gui_look_new (lw6sys_context_t * sys_context, const lw6map_style_t * map_style)
{
  lw6gui_look_t *look = NULL;

  look = (lw6gui_look_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6gui_look_t));

  if (look)
    {
      look->id = 0;
      while (!look->id)
	{
	  look->id = ++seq_id;
	}

      look->dynamic_zoom = DEFAULT_DYNAMIC_ZOOM;
      look->gfx_quality = LW6GUI_GFX_QUALITY_STANDARD;
      if (map_style)
	{
	  lw6map_style_copy (sys_context, &look->style, map_style);
	}
      else
	{
	  lw6map_style_defaults (sys_context, &look->style);
	}
    }

  return look;
}

/**
 * lw6gui_look_free
 *
 * @sys_context: global system context
 * @look: look object to free
 *
 * Frees a look and all its members.
 *
 * Return value: none.
 */
void
lw6gui_look_free (lw6sys_context_t * sys_context, lw6gui_look_t * look)
{
  if (look)
    {
      lw6map_style_clear (sys_context, &look->style);
      LW6SYS_FREE (sys_context, look);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL look"));
    }
}


/**
 * lw6gui_look_memory_footprint
 *
 * @sys_context: global system context
 * @look: look object to query
 *
 * Gives the memory taken by this object in memory.
 *
 * Return value: number of bytes.
 */
int
lw6gui_look_memory_footprint (lw6sys_context_t * sys_context, const lw6gui_look_t * look)
{
  //todo
  return sizeof (lw6gui_look_t);
}

/**
 * lw6gui_look_repr
 *
 * @sys_context: global system context
 * @look: look object to describe
 *
 * Returns a readable description of the object.
 *
 * Return value: newly allocated string.
 */
char *
lw6gui_look_repr (lw6sys_context_t * sys_context, const lw6gui_look_t * look)
{
  char *ret = NULL;

  if (look)
    {
      ret = lw6sys_new_sprintf (sys_context, "id=%u", look->id);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't generate string id for NULL look"));
    }

  return ret;
}

/**
 * lw6gui_look_set
 *
 * @sys_context: global system context
 * @look: look object to modify
 * @key: the key to change
 * @value: the new value for the key
 *
 * Sets a new value for a given key in the look. The value is
 * always a string, it will be converted/casted to the right
 * type if needed.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6gui_look_set (lw6sys_context_t * sys_context, lw6gui_look_t * look, char *key, char *value)
{
  int ret = 0;

  if (!strcmp (key, "dynamic-zoom"))
    {
      look->dynamic_zoom = lw6sys_atof (sys_context, value);
      _fix_zoom (sys_context, look);
      ret = 1;
    }
  else if (!strcmp (key, LW6DEF_GFX_QUALITY))
    {
      look->gfx_quality = lw6sys_atoi (sys_context, value);
      _fix_gfx_quality (sys_context, look);
    }
  else
    {
      ret = lw6map_style_set (sys_context, &(look->style), key, value);
    }

  return ret;
}

/**
 * lw6gui_look_get
 *
 * @sys_context: global system context
 * @look: look object to query
 * @key: the key to get
 *
 * Gets a new value for a given key in the look. The value is
 * always a string, it will be converted/casted from the right
 * type if needed.
 *
 * Return value: dynamically allocated string.
 */
char *
lw6gui_look_get (lw6sys_context_t * sys_context, const lw6gui_look_t * look, char *key)
{
  char *ret = NULL;

  if (!strcmp (key, "dynamic-zoom"))
    {
      ret = lw6sys_ftoa (sys_context, look->dynamic_zoom);
    }
  else if (!strcmp (key, LW6DEF_GFX_QUALITY))
    {
      ret = lw6sys_itoa (sys_context, look->gfx_quality);
    }
  else
    {
      ret = lw6map_style_get (sys_context, &look->style, key);
    }

  return ret;
}

/**
 * lw6gui_look_is_same
 *
 * @sys_context: global system context
 * @look_a: first object to compare
 * @look_b: second object to compare
 *
 * Compares two look objects, doing recursive
 * comparisons. This can be very usefull if, for
 * instance, a graphics renderer is using some contextual
 * objects that depend on the look (colors?) and need
 * to be updated/regenerated on a look change
 *
 * Return value: 1 if they are the same, 0 if not.
 */
int
lw6gui_look_is_same (lw6sys_context_t * sys_context, const lw6gui_look_t * look_a, const lw6gui_look_t * look_b)
{
  int ret = 1;

  if (((!look_a) && look_b) || (look_a && (!look_b)))
    {
      ret = 0;
    }
  if (look_a && look_b)
    {
      ret = ret && look_a->dynamic_zoom == look_b->dynamic_zoom;
      ret = ret && look_a->gfx_quality == look_b->gfx_quality;
      ret = ret && lw6map_style_is_same (sys_context, &look_a->style, &look_b->style);
    }

  return ret;
}

/**
 * lw6gui_look_dup
 *
 * @sys_context: global system context
 * @look: object to duplicate
 *
 * Duplicates a look object, performing recursive copies.
 *
 * Return value: newly allocated object.
 */
lw6gui_look_t *
lw6gui_look_dup (lw6sys_context_t * sys_context, const lw6gui_look_t * look)
{
  lw6gui_look_t *ret = NULL;

  if (look)
    {
      ret = lw6gui_look_new (sys_context, &(look->style));
    }
  else
    {
      ret = lw6gui_look_new (sys_context, NULL);
    }
  if (ret)
    {
      ret->gfx_quality = look->gfx_quality;
      ret->dynamic_zoom = look->dynamic_zoom;
    }

  return ret;
}

/**
 * lw6gui_look_fix
 *
 * @sys_context: global system context
 * @look: look object to modify
 *
 * Peforms sanity checks and modifies the look if needed,
 * to make all values fit within acceptable ranges, among
 * other things.
 *
 * Return value: none.
 */
void
lw6gui_look_fix (lw6sys_context_t * sys_context, lw6gui_look_t * look)
{
  _fix_zoom (sys_context, look);
}

/**
 * lw6gui_look_zoom_in
 *
 * @sys_context: global system context
 * @look: look object to act upon
 * @zoom_step: how much we should zoom, 2.0 means 2 times bigger
 *
 * Zooms in, the function does not only multiplicates the current
 * zoom, it also performs sanity checks.
 *
 * Return value: 1 if zoom was changed, 0 if not.
 */
int
lw6gui_look_zoom_in (lw6sys_context_t * sys_context, lw6gui_look_t * look, float zoom_step)
{
  int ret = 0;
  float old_zoom = look->dynamic_zoom;

  if (zoom_step > 1.0f)
    {
      look->dynamic_zoom *= zoom_step;
      if (look->dynamic_zoom >= 1.0 / zoom_step && look->dynamic_zoom < 1.0f)
	{
	  /*
	   * Sticky behavior
	   */
	  look->dynamic_zoom = 1.0f;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("zoom_step=%f, should be >1.0"), zoom_step);
    }
  _fix_zoom (sys_context, look);

  ret = (old_zoom != look->dynamic_zoom);

  return ret;
}

/**
 * lw6gui_look_zoom_out
 *
 * @sys_context: global system context
 * @look: look object to act upon
 * @zoom_step: how much we should zoom, 2.0 means 2 times smaller
 *
 * Zooms out, the function does not only divides the current
 * zoom, it also performs sanity checks.
 *
 * Return value: 1 if zoom was changed, 0 if not.
 */
int
lw6gui_look_zoom_out (lw6sys_context_t * sys_context, lw6gui_look_t * look, float zoom_step)
{
  int ret = 0;
  float old_zoom = look->dynamic_zoom;

  if (zoom_step > 1.0f)
    {
      look->dynamic_zoom /= zoom_step;
      /*
       * Sticky behavior
       */
      if (look->dynamic_zoom <= zoom_step && look->dynamic_zoom > 1.0f)
	{
	  look->dynamic_zoom = 1.0f;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("zoom_step=%f, should be >1.0"), zoom_step);
    }
  _fix_zoom (sys_context, look);

  ret = (old_zoom != look->dynamic_zoom);

  return ret;
}
