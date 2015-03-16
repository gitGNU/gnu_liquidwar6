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
 * This is used to stamp menus as they are created.
 * Theorically, there could be a race condition if several threads used
 * this, in practice, very rare, and, moreover, few consequences.
 */
static volatile u_int32_t seq_id = 0;

/**
 * lw6gui_menu_new
 *
 * @sys_context: global system context
 * @title: the string to be displayed, what the user sees. Can be freed after
 *   the call is done, function will make a copy internally.
 * @help: a string introducing the menu, describing what it does,
 *   giving hints on how to use it.
 * @popup: a string to be displayed in popup mode when menu is displayed
 *   for the first time.
 * @esc: the label to be displayed in the ESC button
 * @enable_esc: wether to enable the escape button.
 *
 * Constructs a new menu object. Note that you can always call other
 * functions to modify it afterwards.
 *
 * Return value: a pointer to the newly allocated object.
 */
lw6gui_menu_t *
lw6gui_menu_new (lw6sys_context_t * sys_context, const char *title, const char *help, const char *popup, const char *esc, int enable_esc)
{
  lw6gui_menu_t *menu = NULL;

  menu = (lw6gui_menu_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6gui_menu_t));
  if (menu)
    {
      menu->id = 0;
      while (!menu->id)
	{
	  menu->id = ++seq_id;
	}

      menu->title = lw6sys_str_copy (sys_context, lw6sys_str_empty_if_null (sys_context, title));
      if (menu->title)
	{
	  menu->help = lw6sys_str_copy (sys_context, lw6sys_str_empty_if_null (sys_context, help));
	  if (menu->help)
	    {
	      menu->popup = lw6sys_str_copy (sys_context, lw6sys_str_empty_if_null (sys_context, popup));
	      if (menu->popup)
		{
		  menu->esc_item = lw6gui_menuitem_new (sys_context, esc, NULL, 0, enable_esc, 0, 0);
		  if (menu->esc_item)
		    {
		      // OK
		    }
		}
	    }
	}
      if ((!menu->title) || (!menu->help) || (!menu->popup) || (!menu->esc_item))
	{
	  lw6gui_menu_free (sys_context, menu);
	  menu = NULL;
	}
    }

  return menu;
}

/**
 * lw6gui_menu_free
 *
 * @sys_context: global system context
 * @menu: a pointer to the menu.
 *
 * Frees the menu, checking if things are OK before doing so.
 *
 * Return value: none.
 */
void
lw6gui_menu_free (lw6sys_context_t * sys_context, lw6gui_menu_t * menu)
{
  int i;

  if (menu)
    {
      if (menu->title)
	{
	  LW6SYS_FREE (sys_context, menu->title);
	  menu->title = NULL;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("menu with NULL title"));
	}

      if (menu->help)
	{
	  LW6SYS_FREE (sys_context, menu->help);
	  menu->help = NULL;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("menu with NULL help"));
	}

      if (menu->popup)
	{
	  LW6SYS_FREE (sys_context, menu->popup);
	  menu->popup = NULL;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("menu with NULL popup"));
	}

      if (menu->esc_item)
	{
	  lw6gui_menuitem_free (sys_context, menu->esc_item);
	}
      if (menu->items)
	{
	  for (i = 0; i < menu->nb_items; ++i)
	    {
	      lw6gui_menuitem_free (sys_context, menu->items[i]);
	    }
	  LW6SYS_FREE (sys_context, menu->items);
	}
      if (menu->breadcrumbs)
	{
	  lw6sys_list_free (sys_context, menu->breadcrumbs);
	}
      LW6SYS_FREE (sys_context, menu);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to free NULL menu"));
    }
}

/**
 * lw6gui_menu_memory_footprint
 *
 * @sys_context: global system context
 * @menu: a pointer to the menu.
 *
 * Gets the memory occupied by the menu. Could be usefull to help
 * a garbage collector taking decisions or reporting erros, for instance.
 *
 * Return value: the number of bytes used.
 */
int
lw6gui_menu_memory_footprint (lw6sys_context_t * sys_context, lw6gui_menu_t * menu)
{
  int memory_footprint = 0;
  int i;

  if (menu)
    {
      memory_footprint += sizeof (lw6gui_menu_t);
      memory_footprint += strlen (menu->title) + 1;
      memory_footprint += strlen (menu->help) + 1;
      memory_footprint += strlen (menu->popup) + 1;
      for (i = 0; i < menu->nb_items; ++i)
	{
	  memory_footprint += lw6gui_menuitem_memory_footprint (sys_context, menu->items[i]);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("calling memory_footprint on NULL menu"));
    }

  return memory_footprint;
}

/**
 * lw6gui_menu_repr
 *
 * @sys_context: global system context
 * @menu: a pointer to the menu.
 *
 * Constructs a readable description of the object. Usefull for
 * debugging, or to introspect things using scripts, at run-time.
 * Does not necessarly describe all the informations about the
 * object, but helps knowing what it is.
 *
 * Return value: a string describing the object, must be freed.
 */
char *
lw6gui_menu_repr (lw6sys_context_t * sys_context, const lw6gui_menu_t * menu)
{
  char *repr;

  repr = lw6sys_new_sprintf (sys_context, _x_ ("%u \"%s\" (nb_items=%d)"), menu->id, menu->title, menu->nb_items);

  return repr;
}

/**
 * lw6gui_menu_set_title
 *
 * @sys_context: global system context
 * @menu: a pointer to the menu.
 * @title: the new title, you can free it after calling the function,
 *   an internal copy will be made.
 *
 * Change the title of the menu.
 * Use this function to change the title, don't try to access the
 * struct directly. The idea is to have safe memory management.
 *
 * Return value: none
 */
void
lw6gui_menu_set_title (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, const char *title)
{
  LW6SYS_FREE (sys_context, menu->title);
  menu->title = lw6sys_str_copy (sys_context, lw6sys_str_empty_if_null (sys_context, title));
  if (!(menu->title))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't set menu title \"%s\""), title);
    }
}

/**
 * lw6gui_menu_set_help
 *
 * @sys_context: global system context
 * @menu: a pointer to the menu.
 * @help: the new help, you can free it after calling the function,
 *   an internal copy will be made.
 *
 * Change the help of the menu.
 * Use this function to change the help, don't try to access the
 * struct directly. The idea is to have safe memory management.
 *
 * Return value: none
 */
void
lw6gui_menu_set_help (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, const char *help)
{
  LW6SYS_FREE (sys_context, menu->help);
  menu->help = lw6sys_str_copy (sys_context, lw6sys_str_empty_if_null (sys_context, help));
  if (!(menu->help))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't set menu help \"%s\""), help);
    }
}

/**
 * lw6gui_menu_set_popup
 *
 * @sys_context: global system context
 * @menu: a pointer to the menu.
 * @popup: the new popup, you can free it after calling the function,
 *   an internal copy will be made.
 *
 * Change the popup of the menu. That is to say, its popup.
 * Use this function to change the popup, don't try to access the
 * struct directly. The idea is to have safe memory management.
 *
 * Return value: none
 */
void
lw6gui_menu_set_popup (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, const char *popup)
{
  LW6SYS_FREE (sys_context, menu->popup);
  menu->popup = lw6sys_str_copy (sys_context, lw6sys_str_empty_if_null (sys_context, popup));
  if (!(menu->popup))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("couldn't set menu popup \"%s\""), popup);
    }
}

/**
 * lw6gui_menu_close_popup
 *
 * @sys_context: global system context
 * @menu: a pointer to the menu.
 *
 * Closes the popup, in practice, this is equivalent to
 * setting the popup string to "" or NULL.
 *
 * Return value: none
 */
void
lw6gui_menu_close_popup (lw6sys_context_t * sys_context, lw6gui_menu_t * menu)
{
  lw6gui_menu_set_popup (sys_context, menu, NULL);
}

/**
 * lw6gui_menu_has_popup
 *
 * @sys_context: global system context
 * @menu: a pointer to the menu.
 *
 * Tells wether a popup is defined. Behavior is simplistic,
 * at creation (when a non-NULL non-empty popup string has
 * been set) then the popup is displayed. In this state,
 * popup is considered to be defined. Then it can be close,
 * and after this action the popup ain't here anymore,
 * program continues the way it started.
 *
 * Return value: 1 if has popup, 0 if does not
 */
int
lw6gui_menu_has_popup (lw6sys_context_t * sys_context, lw6gui_menu_t * menu)
{
  return !lw6sys_str_is_null_or_empty (sys_context, menu->popup);
}

/**
 * lw6gui_menu_get_item
 *
 * @sys_context: global system context
 * @menu: the menu we want to query
 * @position: the order of the item we want
 *
 * Gets the menu item at the given position. First item is 0, last is N-1.
 * Returns a pointer on the real object, not a copy.
 *
 * Return value: a pointer to a menu item, NULL if out of range.
 */
lw6gui_menuitem_t *
lw6gui_menu_get_item (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int position)
{
  lw6gui_menuitem_t *ret = NULL;

  if (position >= 0 && position < menu->nb_items)
    {
      ret = menu->items[position];
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("requesting menuitem %d but there are only %d in \"%s\""), position, menu->nb_items, menu->title);
    }

  return ret;
}

/**
 * lw6gui_menu_select
 *
 * @sys_context: global system context
 * @menu: the menu we want to modify
 * @position: the position of the item we want to select
 * @allow_scroll: wether scrolling should be allowed when displaying it
 * @now: the current time, as a timestamp.
 *
 * Selects the item at the given position. Use this function to be sure
 * that only one item is selected, and all other states are consistent.
 * Timestamp is needed for the sake of eye-candy.
 *
 * Return value: 1 if success, 0 if failure (out of range).
 */
int
lw6gui_menu_select (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int position, int allow_scroll, int64_t now)
{
  int ret = 0;
  int i = 0;

  if (menu->nb_items > 0)
    {
      if (position >= 0 && position < menu->nb_items)
	{
	  menu->selected_item = position;
	  for (i = 0; i < menu->nb_items; ++i)
	    {
	      lw6gui_menuitem_select (sys_context, menu->items[i], (i == position), now);
	    }
	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("menu select out of range (%d/%d)"), position, menu->nb_items);
	}
    }
  menu->allow_scroll = allow_scroll;

  return ret;
}

/**
 * lw6gui_menu_select_esc
 *
 * @sys_context: global system context
 * @menu: the menu we want to modify
 * @state: 1 to select, 0 to unselect
 * @now: the current time, as a timestamp.
 *
 * Selects the escape item, this does not affect other items, it's mostly.
 * to handle eye candy.
 *
 * Return value: none.
 */
void
lw6gui_menu_select_esc (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int state, int64_t now)
{
  lw6gui_menuitem_select (sys_context, menu->esc_item, state, now);
}

/**
 * lw6gui_menu_enable_esc
 *
 * @sys_context: global system context
 * @menu: the menu we want to modify
 * @state: 1 to enable, 0 to disable
 * @now: the current time, as a timestamp.
 *
 * Enables the escape item, this does not affect other items, it's mostly.
 * to handle eye candy.
 *
 * Return value: none.
 */
void
lw6gui_menu_enable_esc (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int state, int64_t now)
{
  lw6gui_menuitem_enable (sys_context, menu->esc_item, state, now);
}

/**
 * lw6gui_menu_scroll_up
 *
 * @sys_context: global system context
 * @menu: the menu to scroll
 *
 * Scrolls a menu up, used as a callback for mouse wheel up for instance.
 * The idea is just to decrement the first displayed item index.
 *
 * Return value: 1 if OK, 0 if failed (out of range).
 */
int
lw6gui_menu_scroll_up (lw6sys_context_t * sys_context, lw6gui_menu_t * menu)
{
  int ret = 0;

  if (menu->selected_item > 0 && menu->first_item_displayed > 0)
    {
      menu->first_item_displayed--;
      ret = 1;
    }

  return ret;
}

/**
 * lw6gui_menu_scroll_down
 *
 * @sys_context: global system context
 * @menu: the menu to scroll
 *
 * Scrolls a menu down, used as a callback for mouse wheel down for instance.
 * The idea is just to increment the first displayed item index.
 *
 * Return value: 1 if OK, 0 if failed (out of range).
 */
int
lw6gui_menu_scroll_down (lw6sys_context_t * sys_context, lw6gui_menu_t * menu)
{
  int ret = 0;

  if (menu->selected_item < menu->nb_items - 1 && menu->first_item_displayed < menu->nb_items - 1)
    {
      menu->first_item_displayed++;
      ret = 1;
    }

  return ret;
}

/**
 * lw6gui_menu_set_breadcrumbs
 *
 * @sys_context: global system context
 * @menu: the menu to scroll
 * @breadcrumbs: list of strings containing breadcrumbs
 *
 * Set the breadcrumbs, that's to say the readable, logical
 * path to get to a given menu. This is just eye candy, not
 * linked to any logic at this level.
 *
 * Return value: 1 if OK, 0 if failed.
 */
int
lw6gui_menu_set_breadcrumbs (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, lw6sys_list_t * breadcrumbs)
{
  int ret = 0;

  if (menu->breadcrumbs)
    {
      lw6sys_list_free (sys_context, menu->breadcrumbs);
    }
  menu->breadcrumbs = lw6sys_list_dup (sys_context, breadcrumbs, (lw6sys_dup_func_t) lw6sys_str_copy);
  ret = (menu->breadcrumbs != NULL);

  return ret;
}

/**
 * lw6gui_menu_center
 *
 * @sys_context: global system context
 * @menu: the menu to center
 * @position: the position of the menuitem to be put in the center
 * @max_displayed_items: the maximum number of items displayed
 *
 * Centers the menu on a given menuitem. Typically used when pushing
 * a menu with a menuitem selected 'anywhere' in the list.
 *
 * Return value: none.
 */
void
lw6gui_menu_center (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int position, int max_displayed_items)
{
  menu->first_item_displayed = position - (max_displayed_items / 2);
  menu->first_item_displayed = lw6sys_imin (menu->first_item_displayed, menu->nb_items - 1);
  menu->first_item_displayed = lw6sys_imax (menu->first_item_displayed, 0);
}

/**
 * lw6gui_menu_insert
 *
 * @sys_context: global system context
 * @menu: the menu we want to modify
 * @menuitem: the item to insert
 * @position: the position the new item will occupy ("insert before" mode)
 * @now: the current time, as a timestamp.
 *
 * Inserts the given item in the menu. All items starting at the insert
 * position will be "pushed" (that is, their position incremented by 1).
 * Once the menuitem is inserted, the menu object will take care of
 * memory management and automatically free it when needed.
 *
 * Return value: 1 if success, 0 if failure (memory problem, out of range).
 */
int
lw6gui_menu_insert (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, lw6gui_menuitem_t * menuitem, int position, int64_t now)
{
  int ret = 0;
  int i;

  if (position >= 0 && position <= menu->nb_items)
    {
      if (menu->items > 0)
	{
	  menu->items = (lw6gui_menuitem_t **) LW6SYS_REALLOC (sys_context, (void *) (menu->items), (menu->nb_items + 1) * sizeof (lw6gui_menuitem_t *));
	  if (menu->items)
	    {
	      ++(menu->nb_items);
	      for (i = menu->nb_items - 1; i > position; --i)
		{
		  menu->items[i] = menu->items[i - 1];
		}
	      menu->items[position] = menuitem;
	      if (menu->selected_item >= position)
		{
		  lw6gui_menu_select (sys_context, menu, menu->selected_item + 1, 0, now);
		}
	      ret = 1;
	    }
	}
      else
	{
	  menu->items = (lw6gui_menuitem_t **) LW6SYS_MALLOC (sys_context, sizeof (lw6gui_menuitem_t *));
	  if (menu->items)
	    {
	      menu->nb_items = 1;
	      menu->items[0] = menuitem;
	      lw6gui_menu_select (sys_context, menu, 0, 0, now);
	      ret = 1;
	    }
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_
		  ("trying to insert menuitem \"%s\" at position %d but menu \"%s\" has only %d items"), menu->title, position, menu->title, menu->nb_items);
    }

  return ret;
}

/**
 * lw6gui_menu_insert
 *
 * @sys_context: global system context
 * @menu: the menu we want to modify
 * @menuitem: the item to insert
 * @now: the current time, as a timestamp.
 *
 * Appends the given item to the menu.
 * Once the menuitem is appended, the menu object will take care of
 * memory management and automatically free it when needed.
 *
 * Return value: 1 if success, 0 if failure (memory problem).
 */
int
lw6gui_menu_append (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, lw6gui_menuitem_t * menuitem, int64_t now)
{
  int ret = 0;

  ret = lw6gui_menu_insert (sys_context, menu, menuitem, menu->nb_items, now);

  return ret;
}

/**
 * lw6gui_menu_remove
 *
 * @sys_context: global system context
 * @menu: the menu we want to modify
 * @position: the item to insert
 * @now: the current time, as a timestamp.
 *
 * Removes an item from the menu. It will automatically be freed.
 *
 * Return value: 1 if success, 0 if failure (out of range).
 */
int
lw6gui_menu_remove (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int position, int64_t now)
{
  int ret = 0;
  int i;

  if (position >= 0 && position < menu->nb_items)
    {
      lw6gui_menuitem_free (sys_context, menu->items[position]);
      --(menu->nb_items);
      for (i = position; i < menu->nb_items; ++i)
	{
	  menu->items[i] = menu->items[i + 1];
	}
      if (menu->nb_items > 0)
	{
	  if (menu->selected_item > position)
	    {
	      if (menu->selected_item > 0)
		{
		  lw6gui_menu_select (sys_context, menu, menu->selected_item - 1, 0, now);
		}
	      else
		{
		  lw6gui_menu_select (sys_context, menu, menu->selected_item, 0, now);
		}
	    }
	  else
	    {
	      if (position < menu->nb_items)
		{
		  lw6gui_menu_select (sys_context, menu, position, 0, now);
		}
	      else
		{
		  lw6gui_menu_select (sys_context, menu, menu->nb_items - 1, 0, now);
		}
	    }
	}
      else
	{
	  menu->nb_items = 0;
	  menu->selected_item = 0;
	  LW6SYS_FREE (sys_context, menu->items);
	  menu->items = NULL;
	}
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_
		  ("trying to remove menuitem \"%s\" at position %d but menu \"%s\" has only %d items"), menu->title, position, menu->title, menu->nb_items);
    }

  return ret;
}

/**
 * lw6gui_menu_remove_all
 *
 * @sys_context: global system context
 * @menu: the menu we want to modify
 * @now: the current time, as a timestamp.
 *
 * Removes all items from the menu, usefull when one wants
 * to repopulate the items completely, from scratch.
 *
 * Return value: 1 if success, 0 if failure.
 */
int
lw6gui_menu_remove_all (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int64_t now)
{
  int ret = 1;

  while (menu->nb_items > 0)
    {
      ret = lw6gui_menu_remove (sys_context, menu, 0, now) && ret;
    }

  return ret;
}

/**
 * lw6gui_menu_update_display_range
 *
 * @sys_context: global system context
 * @menu: the menu concerned
 * @max_displayed_items: the maximum number of items to display at once
 *
 * Updates the display range. The reason for having this is that the
 * first item, that is, how far we scroll in a very long menu, depends
 * on the previous position. Plus you have to handle limit cases (begin/end).
 * Thus, this function, which will automatically pick-up a suitable
 * position. Of course, @first_item_displayed is not necessarly
 * equal to @selected_item.
 *
 * Return value: none.
 */
void
lw6gui_menu_update_display_range (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int max_displayed_items)
{
  if (menu->nb_items > max_displayed_items)
    {
      if (menu->allow_scroll)
	{
	  menu->first_item_displayed = menu->selected_item - menu->order_of_selected_on_display;
	}

      /*
       * Here there are more items in the menu than the max
       * displayable number.
       */
      menu->nb_items_displayed = max_displayed_items;

      while (menu->first_item_displayed > menu->selected_item)
	{
	  menu->first_item_displayed--;
	}
      while (menu->first_item_displayed + menu->nb_items_displayed - 1 < menu->selected_item)
	{
	  menu->first_item_displayed++;
	}

      // Checking for absolute limits.
      if (menu->first_item_displayed < 0)
	{
	  menu->first_item_displayed = 0;
	}
      if (menu->first_item_displayed + menu->nb_items_displayed > menu->nb_items)
	{
	  menu->first_item_displayed = menu->nb_items - menu->nb_items_displayed;
	}
    }
  else
    {
      menu->first_item_displayed = 0;
      menu->nb_items_displayed = menu->nb_items;
    }
  menu->order_of_selected_on_display = menu->selected_item - menu->first_item_displayed;
}

/**
 * lw6gui_menu_insert_for_id_use
 *
 * @sys_context: global system context
 * @menu: the menu to work on
 * @label: the label of the menuitem to append
 * @tooltip: the tooltip of the menuitem to append
 * @value: the value of the menuitem to append
 * @enabled: wether the inserted menuitem should be enabled
 * @selected: wether the inserted menuitem should be selected
 * @colored: wether the inserted menuitem should use value as its color
 * @postion: the position the inserted menuitem will occupy
 * @now: current time (timestamp)
 *
 * Inserts a menu item at the given position. The idea is that the
 * menu item object is automatically constructed on the fly, and an
 * id is returned, which can be passed to '_using_id' menu-related
 * functions. This is typically for using in scripts. The idea is
 * that the script just keeps a copy of the id returned, and can
 * this way operate directly on the menuitem without keeping a
 * pointer, a smob or anything internally. From the C point of view,
 * having a real C structure enables persistent data from one display
 * to the other, and this is nice and conveninent. I acknowledge
 * the prototype is scary.
 *
 * Return value: 0 if error, or else an id which will later be
 *   used with '_using_id' functions.
 */
int
lw6gui_menu_insert_for_id_use (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, char *label, char *tooltip, int value, int enabled, int selected,
			       int colored, int position, int64_t now)
{
  int ret = 0;
  lw6gui_menuitem_t *menuitem;

  menuitem = lw6gui_menuitem_new (sys_context, label, tooltip, value, enabled, selected, colored);
  if (menuitem)
    {
      if (lw6gui_menu_insert (sys_context, menu, menuitem, position, now))
	{
	  if (selected)
	    {
	      lw6gui_menu_select (sys_context, menu, position, 0, now);
	    }
	  ret = menuitem->id;
	}
    }

  return ret;
}

/**
 * lw6gui_menu_append_for_id_use
 *
 * @sys_context: global system context
 * @menu: the menu to work on
 * @label: the label of the menuitem to append
 * @tooltip: the tooltip of the menuitem to append
 * @value: the value of the menuitem to append
 * @enabled: wether the appended menuitem should be enabled
 * @selected: wether the appended menuitem should be selected
 * @colored: wether the appended menuitem should use value as its color
 * @now: current time (timestamp)
 *
 * Appends a menuitem using the same logic as @lw6gui_menu_insert_for_id_use
 * that is to say a parameter is returned which can later be used to
 * directly operate on a given menuitem, without having its pointer,
 * and even if its position changes.
 *
 * Return value: 0 if error, or else an id which will later be
 *   used with '_using_id' functions.
 */
int
lw6gui_menu_append_for_id_use (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, char *label, char *tooltip, int value, int enabled, int selected,
			       int colored, int64_t now)
{
  int ret = 0;

  ret = lw6gui_menu_insert_for_id_use (sys_context, menu, label, tooltip, value, enabled, selected, colored, menu->nb_items, now);

  return ret;
}

static int
_get_position_using_id (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int id)
{
  int ret = -1;
  int i;

  for (i = 0; i < menu->nb_items; ++i)
    {
      if (menu->items[i]->id == id)
	{
	  ret = i;
	  break;
	}
    }

  if (ret < 0)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("error, no menuitem with id %d in menu %d \"%s\""), id, menu->id, menu->title);
    }

  return ret;
}

/**
 * lw6gui_menu_remove_using_id
 *
 * @sys_context: global system context
 * @menu: the menu to work on
 * @menuitem_id: the id of the menuitem to remove
 * @now: current time (timestamp)
 *
 * Deletes the menuitem with the given id. Very important: the id is not
 * the position. Id are arbitrary numbers that stick to menuitems, but
 * they are not directly linked with the position. This function is practical
 * to use if, for some reason, you don't have the pointer on the menuitem.
 *
 * Return value: 1 if success, 0 if failure (out of range).
 */
int
lw6gui_menu_remove_using_id (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int menuitem_id, int64_t now)
{
  int ret = 0;
  int position;

  position = _get_position_using_id (sys_context, menu, menuitem_id);
  if (position >= 0 && position < menu->nb_items)
    {
      ret = lw6gui_menu_remove (sys_context, menu, position, now);
    }

  return ret;
}

/**
 * lw6gui_menu_sync_using_id
 *
 * @sys_context: global system context
 * @menu: the menu to work on
 * @menuitem_id: the id of the menuitem to synchronize
 * @label: menu label
 * @tooltip: menu tooltip
 * @value: value
 * @enabled: wether it's usable or not
 * @selected: 1 if the menuite is current item
 * @colored: wether to use color
 * @now: current time (timestamp)
 *
 * Updates the menuitem with the given id. Very important: the id is not
 * the position. Id are arbitrary numbers that stick to menuitems, but
 * they are not directly linked with the position. This function is practical
 * to use if, for some reason, you don't have the pointer on the menuitem.
 * In practice, it's heavily used in the game to transmit informations from
 * the scripts to the core C engine. Additionnaly, this function will
 * automatically synchronize the @selected_item field of the menu struct.
 *
 * Return value: 1 if success, 0 if failure (out of range).
 */
void
lw6gui_menu_sync_using_id (lw6sys_context_t * sys_context, lw6gui_menu_t * menu, int menuitem_id, char *label, char *tooltip, int value, int enabled,
			   int selected, int colored, int64_t now)
{
  int position;
  lw6gui_menuitem_t *menuitem;

  position = _get_position_using_id (sys_context, menu, menuitem_id);
  if (position >= 0 && position < menu->nb_items)
    {
      menuitem = lw6gui_menu_get_item (sys_context, menu, position);
      if (menuitem)
	{
	  lw6gui_menuitem_set_label (sys_context, menuitem, label, now);
	  lw6gui_menuitem_set_tooltip (sys_context, menuitem, tooltip, now);
	  lw6gui_menuitem_set_value (sys_context, menuitem, value, now);
	  menuitem->enabled = enabled;
	  if (selected)
	    {
	      lw6gui_menu_select (sys_context, menu, position, 0, now);
	    }
	  menuitem->colored = colored;
	}
    }
}

/**
 * lw6gui_menu_is_same
 *
 * @sys_context: global system context
 * @menu_a: first item to compare
 * @menu_b: second item to compare
 *
 * Compares two menus.
 *
 * Return value: 1 if they are the same, 0 if not
 */
int
lw6gui_menu_is_same (lw6sys_context_t * sys_context, const lw6gui_menu_t * menu_a, const lw6gui_menu_t * menu_b)
{
  int ret = 1;
  int i;

  if (((!menu_a) && menu_b) || (menu_a && (!menu_b)))
    {
      ret = 0;
    }
  if (menu_a && menu_b)
    {
      ret = ret && lw6sys_str_is_same (sys_context, menu_a->title, menu_b->title);
      ret = ret && lw6sys_str_is_same (sys_context, menu_a->help, menu_b->help);
      ret = ret && lw6sys_str_is_same (sys_context, menu_a->popup, menu_b->popup);
      ret = ret && menu_a->nb_items == menu_b->nb_items;
      ret = ret && lw6gui_menuitem_is_same (sys_context, menu_a->esc_item, menu_b->esc_item);
      for (i = 0; ret && i < lw6sys_imin (menu_a->nb_items, menu_b->nb_items); ++i)
	{
	  if (i < menu_a->nb_items && i < menu_b->nb_items && !lw6gui_menuitem_is_same (sys_context, menu_a->items[i], menu_b->items[i]))
	    {
	      ret = 0;
	    }
	}
      ret = ret && menu_a->selected_item == menu_b->selected_item;
      ret = ret && menu_a->first_item_displayed == menu_b->first_item_displayed;
      ret = ret && menu_a->nb_items_displayed == menu_b->nb_items_displayed;
      ret = ret && menu_a->order_of_selected_on_display == menu_b->order_of_selected_on_display;
      ret = ret && menu_a->allow_scroll == menu_b->allow_scroll;
      if (menu_a->breadcrumbs && menu_b->breadcrumbs)
	{
	  /*
	   * OK, here, in theory, we should check each item
	   * one by one. In practice, we just rely on size,
	   * period.
	   */
	  ret = ret && lw6sys_list_length (sys_context, menu_a->breadcrumbs) == lw6sys_list_length (sys_context, menu_b->breadcrumbs);
	}
      else
	{
	  ret = ret && (menu_a->breadcrumbs == menu_b->breadcrumbs);
	}
    }

  return ret;
}

/**
 * lw6gui_menu_dup
 *
 * @sys_context: global system context
 * @menu: the menu to duplicate
 *
 * Duplicates a menu structure.
 *
 * Return value: a pointer to the new menu.
 */
lw6gui_menu_t *
lw6gui_menu_dup (lw6sys_context_t * sys_context, const lw6gui_menu_t * menu)
{
  lw6gui_menu_t *ret = NULL;
  lw6gui_menuitem_t *item = NULL;
  int i;

  if (menu)
    {
      ret = lw6gui_menu_new (sys_context, menu->title, menu->help, menu->popup, menu->esc_item->label, menu->esc_item->enabled);
      if (ret)
	{
	  lw6gui_menuitem_sync (sys_context, ret->esc_item, menu->esc_item);
	  for (i = 0; i < menu->nb_items; ++i)
	    {
	      item = lw6gui_menuitem_dup (sys_context, menu->items[i]);
	      if (item)
		{
		  lw6gui_menu_append (sys_context, ret, item, item->last_change);
		}
	    }
	  if (ret->nb_items == menu->nb_items)
	    {
	      for (i = 0; i < menu->nb_items; ++i)
		{
		  ret->items[i]->selected = menu->items[i]->selected;
		}
	    }
	  else
	    {
	      lw6gui_menu_free (sys_context, ret);
	      ret = NULL;
	    }
	}
    }

  if (ret)
    {
      ret->selected_item = menu->selected_item;
      ret->first_item_displayed = menu->first_item_displayed;
      ret->nb_items_displayed = menu->nb_items_displayed;
      ret->order_of_selected_on_display = menu->order_of_selected_on_display;
      ret->allow_scroll = menu->allow_scroll;

      if (menu->breadcrumbs)
	{
	  ret->breadcrumbs = lw6sys_list_dup (sys_context, menu->breadcrumbs, (lw6sys_dup_func_t) lw6sys_str_copy);
	  if (!(ret->breadcrumbs))
	    {
	      lw6gui_menu_free (sys_context, ret);
	      ret = NULL;
	    }
	}
    }

  return ret;
}

/**
 * lw6gui_menu_sync
 *
 * @sys_context: global system context
 * @dst: the target menu
 * @src: the source menu
 *
 * Synchronizes two menus, this supposes that they represent the same menu, but
 * simply in a different state. This function does not really copy src to dst, it
 * has a special behavior, indeed everything is copied from src to dst, except
 * the @first_item_displayed and @nb_items_displayed which are taken from dst
 * and copied to src. This is because in practise, those values are updated
 * in the display loop/thread, which is the one which uses the target. This is
 * not very orthodox, but should work.
 *
 * Return value: 1 if success, 0 if failure
 */
int
lw6gui_menu_sync (lw6sys_context_t * sys_context, lw6gui_menu_t * dst, lw6gui_menu_t * src)
{
  int ret = 0;
  int i, d;
  lw6gui_menuitem_t *item = NULL;

  if (dst && src)
    {
      if (!lw6sys_str_is_same (sys_context, dst->title, src->title))
	{
	  lw6gui_menu_set_title (sys_context, dst, src->title);
	}
      if (!lw6sys_str_is_same (sys_context, dst->help, src->help))
	{
	  lw6gui_menu_set_help (sys_context, dst, src->help);
	}
      if (!lw6sys_str_is_same (sys_context, dst->popup, src->popup))
	{
	  lw6gui_menu_set_popup (sys_context, dst, src->popup);
	}
      d = dst->nb_items - src->nb_items;
      if (d > 0)
	{
	  for (i = 0; i < d; ++i)
	    {
	      /*
	       * We remove the first item, it might not be perfect but
	       * we don't care, items will be synchronized later
	       */
	      lw6gui_menu_remove (sys_context, dst, 0, 0);
	    }
	}
      if (d < 0)
	{
	  for (i = 0; i < -d; ++i)
	    {
	      /*
	       * We add any item, we just don't care, synchronisation
	       * of items will fix things.
	       */
	      item = lw6gui_menuitem_dup (sys_context, src->items[i]);
	      if (item)
		{
		  lw6gui_menu_append (sys_context, dst, item, 0);
		}
	    }
	}

      lw6gui_menuitem_sync (sys_context, dst->esc_item, src->esc_item);
      if (dst->nb_items == src->nb_items)
	{
	  for (i = 0; i < src->nb_items; ++i)
	    {
	      lw6gui_menuitem_sync (sys_context, dst->items[i], src->items[i]);
	    }
	  dst->selected_item = src->selected_item;
	  /*
	   * Note the src <-> dst inversion for the following values
	   */
	  src->first_item_displayed = dst->first_item_displayed;
	  src->nb_items_displayed = dst->nb_items_displayed;
	  src->order_of_selected_on_display = dst->order_of_selected_on_display;
	  /*
	   * The rest is "normal", copied from src to dst
	   */
	  dst->allow_scroll = src->allow_scroll;

	  if (src->breadcrumbs)
	    {
	      if (dst->breadcrumbs)
		{
		  /*
		   * Lazy test on size only
		   */
		  if (lw6sys_list_length (sys_context, dst->breadcrumbs) != lw6sys_list_length (sys_context, src->breadcrumbs))
		    {
		      lw6sys_list_free (sys_context, dst->breadcrumbs);
		      dst->breadcrumbs = lw6sys_list_dup (sys_context, src->breadcrumbs, (lw6sys_dup_func_t) lw6sys_str_copy);
		    }
		}
	      else
		{
		  dst->breadcrumbs = lw6sys_list_dup (sys_context, src->breadcrumbs, (lw6sys_dup_func_t) lw6sys_str_copy);
		}
	    }
	  else
	    {
	      if (dst->breadcrumbs)
		{
		  lw6sys_list_free (sys_context, dst->breadcrumbs);
		  dst->breadcrumbs = NULL;
		}
	    }

	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to sync menus, dst has %d items while src has %d items"), dst->nb_items, src->nb_items);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("menus are not syncable"));
    }

  return ret;
}
