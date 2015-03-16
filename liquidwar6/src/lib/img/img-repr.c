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

#include "img.h"

/**
 * lw6img_repr
 *
 * @sys_context: global system context
 * @jpeg: the jpeg to describe
 *
 * Returns a string describing the jepg. This is a very short description,
 * use it for logs, and to debug stuff. By no means it's a complete exhaustive
 * description. Still, the string returned should be unique.
 *
 * Return value: a dynamically allocated string.
 */
char *
lw6img_repr (lw6sys_context_t * sys_context, const lw6img_jpeg_t * jpeg)
{
  char *ret = NULL;

  if (jpeg)
    {
      ret = lw6sys_new_sprintf (sys_context, "%u jpeg %dx%d, %d bytes", jpeg->id, jpeg->shape.w, jpeg->shape.h, jpeg->jpeg_size);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't generate string id for NULL jpeg"));
    }

  return ret;
}
