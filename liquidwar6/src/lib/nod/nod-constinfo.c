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

#include "nod.h"
#include "nod-internal.h"

int
_lw6nod_const_info_init (lw6sys_context_t * sys_context, lw6nod_const_info_t * const_info,
			 const char *program, const char *version,
			 const char *codename, int stamp, u_int64_t id,
			 const char *url, const char *title,
			 const char *description, const char *password,
			 int bench, int open_relay, int uptime, int idle_screenshot_size, void *idle_screenshot_data)
{
  int ret = 1;

  if (program && strlen (program) > 0)
    {
      const_info->program = lw6sys_str_copy (sys_context, program);
    }
  else
    {
      const_info->program = lw6sys_str_copy (sys_context, lw6sys_build_get_package_tarname ());
    }
  if (version && strlen (version) > 0)
    {
      const_info->version = lw6sys_str_copy (sys_context, version);
    }
  else
    {
      const_info->version = lw6sys_str_copy (sys_context, lw6sys_build_get_version ());
    }
  if (codename && strlen (codename) > 0)
    {
      const_info->codename = lw6sys_str_copy (sys_context, codename);
    }
  else
    {
      const_info->codename = lw6sys_str_copy (sys_context, lw6sys_build_get_codename ());
    }
  if (stamp)
    {
      const_info->stamp = stamp;
    }
  else
    {
      const_info->stamp = lw6sys_atoi (sys_context, lw6sys_build_get_stamp ());
    }

  ret = _lw6nod_ref_info_update (sys_context, &(const_info->ref_info), id, url) && ret;

  if (title && strlen (title) > 0)
    {
      const_info->title = lw6sys_str_copy (sys_context, title);
    }
  else
    {
      const_info->title = lw6sys_get_hostname (sys_context);
    }
  if (const_info->title)
    {
      lw6sys_str_cleanup (sys_context, const_info->title);
    }
  if (description)
    {
      const_info->description = lw6sys_str_copy (sys_context, description);
    }
  else
    {
      const_info->description = lw6sys_str_copy (sys_context, "");
    }
  if (const_info->description)
    {
      lw6sys_str_cleanup (sys_context, const_info->description);
    }
  if (!lw6sys_str_is_null_or_empty (sys_context, password))
    {
      const_info->has_password = 1;
      const_info->password = lw6sys_str_copy (sys_context, password);
    }
  else
    {
      const_info->password = lw6sys_str_copy (sys_context, "");
    }
  const_info->bench = bench;
  const_info->open_relay = open_relay ? 1 : 0;
  const_info->creation_timestamp = lw6sys_get_timestamp (sys_context) - (u_int64_t) (uptime * 1000);
  const_info->idle_screenshot_size = idle_screenshot_size;
  if (idle_screenshot_size > 0)
    {
      const_info->idle_screenshot_data = LW6SYS_MALLOC (sys_context, idle_screenshot_size);
      if (const_info->idle_screenshot_data)
	{
	  memcpy (const_info->idle_screenshot_data, idle_screenshot_data, idle_screenshot_size);
	}
    }
  else
    {
      const_info->idle_screenshot_data = NULL;
    }

  ret = (const_info->program && const_info->version && const_info->codename
	 && const_info->title && const_info->description && (const_info->idle_screenshot_data || (const_info->idle_screenshot_size == 0))) && ret;

  return ret;
}

void
_lw6nod_const_info_reset (lw6sys_context_t * sys_context, lw6nod_const_info_t * const_info)
{
  if (const_info->program)
    {
      LW6SYS_FREE (sys_context, const_info->program);
    }
  if (const_info->version)
    {
      LW6SYS_FREE (sys_context, const_info->version);
    }
  if (const_info->codename)
    {
      LW6SYS_FREE (sys_context, const_info->codename);
    }

  _lw6nod_ref_info_reset (sys_context, &(const_info->ref_info));

  if (const_info->title)
    {
      LW6SYS_FREE (sys_context, const_info->title);
    }
  if (const_info->description)
    {
      LW6SYS_FREE (sys_context, const_info->description);
    }
  if (const_info->password)
    {
      LW6SYS_FREE (sys_context, const_info->password);
    }
  if (const_info->idle_screenshot_data)
    {
      LW6SYS_FREE (sys_context, const_info->idle_screenshot_data);
    }
  memset (const_info, 0, sizeof (lw6nod_const_info_t));
}
