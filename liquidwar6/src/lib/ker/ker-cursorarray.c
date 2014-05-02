/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014  Christian Mauduit <ufoot@ufoot.org>

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

#include <string.h>

#include "ker.h"
#include "ker-internal.h"

void
_lw6ker_cursor_array_init (_lw6ker_cursor_array_t * cursor_array)
{
  int i = 0;

  cursor_array->nb_cursors = 0;
  for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
    {
      _lw6ker_cursor_init (&(cursor_array->cursors[i]), 'a' + i);
    }
}

void
_lw6ker_cursor_array_update_checksum (const _lw6ker_cursor_array_t *
				      cursor_array, u_int32_t * checksum)
{
  int i = 0;

  lw6sys_checksum_update_int32 (sys_context, checksum,
				cursor_array->nb_cursors);
  for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
    {
      _lw6ker_cursor_update_checksum (&(cursor_array->cursors[i]), checksum);
    }
}

lw6ker_cursor_t *
_lw6ker_cursor_array_find_free (_lw6ker_cursor_array_t * cursor_array)
{
  lw6ker_cursor_t *ret = NULL;
  int i;

  for (i = 0; i < LW6MAP_MAX_NB_CURSORS && !ret; ++i)
    {
      if (!cursor_array->cursors[i].enabled)
	{
	  ret = (lw6ker_cursor_t *) & (cursor_array->cursors[i]);
	}
    }

  if (!ret)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("unable to find free cursor"));
    }

  return ret;
}

int
_lw6ker_cursor_array_is_color_owned_by (const _lw6ker_cursor_array_t *
					cursor_array, u_int64_t node_id,
					int team_color)
{
  int ret = 1;
  int i;

  for (i = 0; i < LW6MAP_MAX_NB_CURSORS && ret; ++i)
    {
      if (cursor_array->cursors[i].enabled
	  && cursor_array->cursors[i].team_color == team_color
	  && cursor_array->cursors[i].node_id != node_id)
	{
	  // this team_color is owned at least one by another node
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		      _x_ ("color %d owned by %" LW6SYS_PRINTF_LL "x"),
		      team_color, (long long) node_id);
	  ret = 0;
	}
    }

  return ret;
}

void
_lw6ker_cursor_array_reset (_lw6ker_cursor_array_t * cursor_array)
{
  int i;

  cursor_array->nb_cursors = 0;
  for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
    {
      lw6ker_cursor_reset (&(cursor_array->cursors[i]));
    }
}

lw6ker_cursor_t *
_lw6ker_cursor_array_get_rw (_lw6ker_cursor_array_t *
			     cursor_array, u_int16_t cursor_id)
{
  lw6ker_cursor_t *ret = NULL;
  int i;

  for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
    {
      if (cursor_array->cursors[i].cursor_id == cursor_id)
	{
	  ret = &(cursor_array->cursors[i]);
	}
    }

  return ret;
}

const lw6ker_cursor_t *
_lw6ker_cursor_array_get_ro (const _lw6ker_cursor_array_t *
			     cursor_array, u_int16_t cursor_id)
{
  const lw6ker_cursor_t *ret = NULL;
  int i;

  for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
    {
      if (cursor_array->cursors[i].cursor_id == cursor_id)
	{
	  ret = &(cursor_array->cursors[i]);
	}
    }

  return ret;
}

int
_lw6ker_cursor_array_enable (_lw6ker_cursor_array_t * cursor_array,
			     u_int64_t node_id,
			     u_int16_t cursor_id, int team_color,
			     int32_t x, int32_t y)
{
  int ret = 0;
  lw6ker_cursor_t *cursor;

  cursor = _lw6ker_cursor_array_get_rw (cursor_array, cursor_id);
  if (!cursor)
    {
      cursor = _lw6ker_cursor_array_find_free (cursor_array);
      if (cursor)
	{
	  _lw6ker_cursor_enable (cursor, node_id, cursor_id, team_color, x,
				 y);
	  cursor_array->nb_cursors++;
	  ret = 1;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("cursor %x already exists, can't enable it twice"),
		  cursor_id);
    }

  return ret;
}

int
_lw6ker_cursor_array_disable (_lw6ker_cursor_array_t * cursor_array,
			      u_int64_t node_id, u_int16_t cursor_id)
{
  int ret = 0;
  lw6ker_cursor_t *cursor;

  cursor = _lw6ker_cursor_array_get_rw (cursor_array, cursor_id);
  if (cursor)
    {
      if (_lw6ker_cursor_check_node_id (cursor, node_id))
	{
	  _lw6ker_cursor_disable (cursor);
	  cursor_array->nb_cursors--;
	  ret = 1;
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("cursor %x already does not exist, can't disable it"),
		  cursor_id);
    }

  return ret;
}

int
_lw6ker_cursor_array_update (_lw6ker_cursor_array_t * cursor_array,
			     u_int64_t node_id,
			     u_int16_t cursor_id, int32_t x,
			     int32_t y, int fire, int fire2,
			     int32_t pot_offset, const lw6sys_whd_t * shape,
			     const lw6map_rules_t * rules)
{
  int ret = 0;
  lw6ker_cursor_t *cursor;

  lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
	      _x_ ("cursor array update %" LW6SYS_PRINTF_LL "x %x %d %d %d"),
	      (long long) node_id, cursor_id, x, y, pot_offset);
  cursor = _lw6ker_cursor_array_get_rw (cursor_array, cursor_id);
  if (cursor)
    {
      if (_lw6ker_cursor_check_node_id (cursor, node_id))
	{
	  ret =
	    _lw6ker_cursor_update (cursor, x, y, fire, fire2, pot_offset,
				   shape, rules);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG,
		  _x_ ("cursor %x already does not exist, can't update it"),
		  cursor_id);
    }

  return ret;
}

int
_lw6ker_cursor_array_sanity_check (const _lw6ker_cursor_array_t *
				   cursor_array, const lw6sys_whd_t * shape,
				   const lw6map_rules_t * rules)
{
  int ret = 1;
  int i;
  int found = 0;

  for (i = 0; i < LW6MAP_MAX_NB_CURSORS; ++i)
    {
      ret = ret
	&& _lw6ker_cursor_sanity_check (&(cursor_array->cursors[i]), shape,
					rules);
      if (cursor_array->cursors[i].enabled)
	{
	  found++;
	}
    }
  if (found != cursor_array->nb_cursors)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
		  _x_
		  ("inconsistency in cursor_array, found %d cursors but array reports %d"),
		  found, cursor_array->nb_cursors);
      ret = 0;
    }

  return ret;
}
