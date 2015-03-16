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

#include "gui.h"

/*
 * This is used to stamp menuitems as they are created.
 * Theorically, there could be a race condition if several threads used
 * this, in practice, very rare, and, moreover, few consequences.
 */
static volatile u_int32_t seq_id = 0;

/**
 * lw6gui_menuitem_new
 *
 * @sys_context: global system context
 * @label: the string to be displayed, what the user sees. Can be freed after
 *   the call is done, function will make a copy internally.
 * @tooltip: the string to be displayed as a tooltip, describing the menu
 *   item in detail. Can be NULL if you don't want to use this feature.
 * @value: the value. No GUI function uses this, this is the "real" value
 *   associated to the item.
 * @enabled: wether the menu item can be selected, used, and so on
 * @selected: wether the menu item is the item selected among all menu items.
 * @colored: wetherr the menu item must, when drawn, be colored according
 *   to its value.
 *
 * Constructs a new menuitem object. Note that you can always call other
 * functions to modify these values afterwards, this might change rendering
 * since @lw6gui_menuitem_set_value or @lw6gui_menuitem_set_label will,
 * for instance, modify the "when was that item last modified" information.
 *
 * Return value: a pointer to the newly allocated object.
 */
lw6gui_menuitem_t *
lw6gui_menuitem_new (lw6sys_context_t * sys_context, const char *label, const char *tooltip, int value, int enabled, int selected, int colored)
{
  lw6gui_menuitem_t *menuitem = NULL;

  menuitem = (lw6gui_menuitem_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6gui_menuitem_t));
  if (menuitem)
    {
      menuitem->id = 0;
      while (!menuitem->id)
	{
	  menuitem->id = ++seq_id;
	}

      menuitem->label = lw6sys_str_copy (sys_context, label);
      if (menuitem->label)
	{
	  menuitem->tooltip = lw6sys_str_copy (sys_context, lw6sys_str_empty_if_null (sys_context, tooltip));
	  if (menuitem->tooltip)
	    {
	      menuitem->value = value;
	      menuitem->enabled = enabled;
	      menuitem->selected = selected;
	      menuitem->colored = colored;
	    }
	}
      if ((!menuitem->label) || (!menuitem->tooltip))
	{
	  lw6gui_menuitem_free (sys_context, menuitem);
	  menuitem = NULL;
	}
    }

  return menuitem;
}

/**
 * lw6gui_menuitem_free
 *
 * @sys_context: global system context
 * @menuitem: a pointer to the menuitem.
 *
 * Frees the menuitem, checking if things are OK before doing so.
 *
 * Return value: none.
 */
void
lw6gui_menuitem_free (lw6sys_context_t * sys_context, lw6gui_menuitem_t * menuitem)
{
  if (menuitem)
    {
      if (menuitem->label)
	{
	  LW6SYS_FREE (sys_context, menuitem->label);
	  menuitem->label = NULL;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("menuitem with NULL label"));
	}
      if (menuitem->tooltip)
	{
	  LW6SYS_FREE (sys_context, menuitem->tooltip);
	  menuitem->tooltip = NULL;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("menuitem with NULL tooltip"));
	}
      LW6SYS_FREE (sys_context, menuitem);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL menuitem"));
    }
}

/**
 * lw6gui_menuitem_memory_footprint
 *
 * @sys_context: global system context
 * @menuitem: a pointer to the menuitem.
 *
 * Gets the memory occupied by the menuitem. Could be usefull to help
 * a garbage collector taking decisions or reporting erros, for instance.
 *
 * Return value: the number of bytes used.
 */
int
lw6gui_menuitem_memory_footprint (lw6sys_context_t * sys_context, lw6gui_menuitem_t * menuitem)
{
  int memory_footprint = 0;

  if (menuitem)
    {
      memory_footprint += sizeof (lw6gui_menuitem_t);
      memory_footprint += strlen (menuitem->label) + 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling memory_footprint on NULL menuitem"));
    }

  return memory_footprint;
}

/**
 * lw6gui_menuitem_repr
 *
 * @sys_context: global system context
 * @menuitem: a pointer to the menuitem.
 *
 * Constructs a readable description of the object. Usefull for
 * debugging, or to introspect things using scripts, at run-time.
 * Does not necessarly describe all the informations about the
 * object, but helps knowing what it is.
 *
 * Return value: a string describing the object, must be freed.
 */
char *
lw6gui_menuitem_repr (lw6sys_context_t * sys_context, const lw6gui_menuitem_t * menuitem)
{
  char *repr;

  repr = lw6sys_new_sprintf (sys_context, _x_ ("%u \"%s\" (value=%d)"), menuitem->id, menuitem->label, menuitem->value);

  return repr;
}

/**
 * lw6gui_menuitem_set_label
 *
 * @sys_context: global system context
 * @menuitem: a pointer to the menuitem.
 * @label: the new label, you can free it after calling the function,
 *   an internal copy will be made.
 * @now: the current time, as a timestamp.
 *
 * Change the label of the menu item. That is to say, what the user sees.
 * Use this function to change the menuitem
 * value, don't try to access the struct directly. The idea is 1) to have safe
 * memory management and 2) to keep the @last_change member up to date.
 * It can be later used for eye-candy effects.
 *
 * Return value: none
 */
void
lw6gui_menuitem_set_label (lw6sys_context_t * sys_context, lw6gui_menuitem_t * menuitem, const char *label, int64_t now)
{
  if (strcmp (label, menuitem->label))
    {
      menuitem->last_change = now;
    }
  LW6SYS_FREE (sys_context, menuitem->label);
  menuitem->label = lw6sys_str_copy (sys_context, lw6sys_str_empty_if_null (sys_context, label));
  if (!(menuitem->label))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't set menu item label \"%s\""), label);
    }
}

/**
 * lw6gui_menuitem_set_tooltip
 *
 * @sys_context: global system context
 * @menuitem: a pointer to the menuitem.
 * @tooltip: the new tooltip, you can free it after calling the function,
 *   an internal copy will be made.
 * @now: the current time, as a timestamp.
 *
 * Change the tooltip of the menu item (the explanation of what the item is about)
 * Use this function to change the menuitem
 * value, don't try to access the struct directly. The idea is 1) to have safe
 * memory management and 2) to keep the @last_change member up to date.
 * It can be later used for eye-candy effects.
 *
 * Return value: none
 */
void
lw6gui_menuitem_set_tooltip (lw6sys_context_t * sys_context, lw6gui_menuitem_t * menuitem, const char *tooltip, int64_t now)
{
  if (strcmp (tooltip, menuitem->tooltip))
    {
      menuitem->last_change = now;
    }
  LW6SYS_FREE (sys_context, menuitem->tooltip);
  menuitem->tooltip = lw6sys_str_copy (sys_context, lw6sys_str_empty_if_null (sys_context, tooltip));
  if (!(menuitem->tooltip))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't set menu item tooltip \"%s\""), tooltip);
    }
}

/**
 * lw6gui_menuitem_set_value
 *
 * @sys_context: global system context
 * @menuitem: a pointer to the menuitem.
 * @value: the new value.
 * @now: the current time, as a timestamp.
 *
 * Changes the value of a menuitem. This is the internal value, not what
 * the user sees. Use this function to change the menuitem
 * value, don't try to access the struct directly. The idea is to keep
 * the @last_change member up to date.
 * It can be later used for eye-candy effects.
 *
 * Return value: none
 */
void
lw6gui_menuitem_set_value (lw6sys_context_t * sys_context, lw6gui_menuitem_t * menuitem, int value, int64_t now)
{
  if (value != menuitem->value)
    {
      menuitem->last_change = now;
    }
  menuitem->value = value;
}

/**
 * lw6gui_menuitem_select
 *
 * @sys_context: global system context
 * @menuitem: a pointer to the menuitem.
 * @state: 1 to select, 0 to unselect
 * @now: the current time, as a timestamp.
 *
 * Switches the menuitem to (un)selected state. Use this function, don't try
 * to modify the struct members directly. The idea is to have
 * the @last_select parameter up to date.
 * It can be later used for eye-candy effects.
 *
 * Return value: none
 */
void
lw6gui_menuitem_select (lw6sys_context_t * sys_context, lw6gui_menuitem_t * menuitem, int state, int64_t now)
{
  if (state && !(menuitem->selected))
    {
      menuitem->selected = 1;
      menuitem->last_select = now;
    }
  if ((!state) && menuitem->selected)
    {
      menuitem->selected = 0;
      menuitem->last_unselect = now;
    }
}

/**
 * lw6gui_menuitem_enable
 *
 * @sys_context: global system context
 * @menuitem: a pointer to the menuitem.
 * @state: 1 to enable, 0 to disable
 * @now: the current time, as a timestamp.
 *
 * Switches the menuitem to enabled/disabled state. Use this function, don't try
 * to modify the struct members directly. The idea is to have
 * the @last_select parameter up to date.
 * It can be later used for eye-candy effects.
 *
 * Return value: none
 */
void
lw6gui_menuitem_enable (lw6sys_context_t * sys_context, lw6gui_menuitem_t * menuitem, int state, int64_t now)
{
  if (state && (!menuitem->enabled))
    {
      menuitem->enabled = 1;
      if (menuitem->selected)
	{
	  menuitem->last_select = now;
	}
      else
	{
	  menuitem->last_unselect = now;
	}
    }
  if ((!state) && menuitem->enabled)
    {
      menuitem->enabled = 0;
      if (menuitem->selected)
	{
	  menuitem->last_select = now;
	}
      else
	{
	  menuitem->last_unselect = now;
	}
    }
}

/**
 * lw6gui_menuitem_checksum
 *
 * @sys_context: global system context
 * @menuitem: the menuitem we want to identify
 *
 * Returns a checksum which can be used to know, for
 * instance, wether the menuitem has changed or not, and if we should
 * redraw it.
 *
 * Return value: a checksum.
 */
u_int32_t
lw6gui_menuitem_checksum (lw6sys_context_t * sys_context, lw6gui_menuitem_t * menuitem, lw6gui_look_t * look)
{
  u_int32_t ret = 0;


  lw6sys_checksum_update (sys_context, &ret, (unsigned char *) menuitem, sizeof (lw6gui_menuitem_t));
  lw6sys_checksum_update (sys_context, &ret, (unsigned char *) look, sizeof (lw6gui_look_t));

  return ret;
}

/**
 * lw6gui_menuitem_is_same
 *
 * @sys_context: global system context
 * @menuitem_a: first item to compare
 * @menuitem_b: second item to compare
 *
 * Compares two menuitems.
 *
 * Return value: 1 if they are the same, 0 if not
 */
int
lw6gui_menuitem_is_same (lw6sys_context_t * sys_context, const lw6gui_menuitem_t * menuitem_a, const lw6gui_menuitem_t * menuitem_b)
{
  int ret = 1;

  if (((!menuitem_a) && menuitem_b) || (menuitem_a && (!menuitem_b)))
    {
      ret = 0;
    }
  if (menuitem_a && menuitem_b)
    {
      ret = ret && !strcmp (menuitem_a->label, menuitem_b->label);
      ret = ret && menuitem_a->value == menuitem_b->value;
      ret = ret && menuitem_a->enabled == menuitem_b->enabled;
      ret = ret && menuitem_a->selected == menuitem_b->selected;
      ret = ret && menuitem_a->colored == menuitem_b->colored;
      ret = ret && menuitem_a->last_change == menuitem_b->last_change;
      ret = ret && menuitem_a->last_select == menuitem_b->last_select;
      ret = ret && menuitem_a->last_unselect == menuitem_b->last_unselect;
    }

  return ret;
}

/**
 * lw6gui_menuitem_dup
 *
 * @sys_context: global system context
 * @menuitem: the menuitem to duplicate
 *
 * The menuitem to duplicate.
 *
 * Return value: a pointer to the duplicted menuitem.
 */
lw6gui_menuitem_t *
lw6gui_menuitem_dup (lw6sys_context_t * sys_context, const lw6gui_menuitem_t * menuitem)
{
  lw6gui_menuitem_t *ret = NULL;

  if (menuitem)
    {
      ret = lw6gui_menuitem_new (sys_context, menuitem->label, menuitem->tooltip, menuitem->value, menuitem->enabled, menuitem->selected, menuitem->colored);
      if (ret)
	{
	  ret->last_change = menuitem->last_change;
	  ret->last_select = menuitem->last_select;
	  ret->last_unselect = menuitem->last_unselect;
	}
    }

  return ret;
}

/**
 * lw6gui_menuitem_sync
 *
 * @sys_context: global system context
 * @dst: the target menuitem
 * @src: the source menuitem
 *
 * Synchronizes two menuitems, this supposes that they represent the same item, but
 * simply in a different state.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6gui_menuitem_sync (lw6sys_context_t * sys_context, lw6gui_menuitem_t * dst, lw6gui_menuitem_t * src)
{
  int ret = 0;

  if (dst && src)
    {
      if (!lw6sys_str_is_same (sys_context, dst->label, src->label))
	{
	  lw6gui_menuitem_set_label (sys_context, dst, src->label, src->last_change);
	}
      if (!lw6sys_str_is_same (sys_context, dst->tooltip, src->tooltip))
	{
	  lw6gui_menuitem_set_tooltip (sys_context, dst, src->tooltip, src->last_change);
	}
      dst->value = src->value;
      dst->enabled = src->enabled;
      dst->selected = src->selected;
      dst->colored = src->colored;
      dst->last_change = src->last_change;
      dst->last_select = src->last_select;
      dst->last_unselect = src->last_unselect;
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("menuitems are not syncable"));
    }

  return ret;
}
