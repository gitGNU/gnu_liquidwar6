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

#include <errno.h>

#include "mod-caca-internal.h"

int
_mod_caca_display_menu (lw6sys_context_t * sys_context, _mod_caca_context_t * caca_context, const lw6gui_look_t * look, lw6gui_menu_t * menu)
{
  int ret = 1;
  int i, j, wc;
  lw6gui_menuitem_t *menuitem;

  wc = caca_get_canvas_width (caca_context->canvas);
  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("menu:nb_item %d"), menu->nb_items);
  for (i = 0; i < menu->nb_items; ++i)
    {
      j = i + menu->first_item_displayed;
      menuitem = menu->items[j];
      caca_set_color_ansi (caca_context->canvas, CACA_WHITE, CACA_DEFAULT);
      if (menuitem->selected)
	{
	  caca_set_color_ansi (caca_context->canvas, CACA_BLACK, CACA_WHITE);
	}
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("menu:nb_item %s"), menuitem->label);
      caca_put_str (caca_context->canvas, (wc - strlen (menuitem->label)) / 2, 4 + i, menuitem->label);
    }

  return ret;
}
