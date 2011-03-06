/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011  Christian Mauduit <ufoot@ufoot.org>

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

#include "ldr.h"
#include "ldr-internal.h"

static char *
extract_title_from_dirname (char *dirname)
{
  char *copy = NULL;
  char *title = NULL;
  char *begin = NULL;

  copy = lw6sys_path_strip_slash (dirname);
  if (copy)
    {
      begin = strrchr (copy, '/');
      if (begin)
	{
	  begin++;
	}
      else
	{
	  begin = copy;
	}

      title = lw6sys_str_copy (begin);

      LW6SYS_FREE (copy);
    }

  if (!title)
    {
      title = lw6sys_str_copy (_x_ ("no title for this map"));
    }

  if (!title)
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_ ("unable to handle map title, running out of memory?"));
    }

  return title;
}

static char *
read_readme (char *dirname)
{
  char *buf = NULL;
  char *readme = NULL;

  buf = lw6sys_path_concat (dirname, _LW6LDR_FILE_README);
  if (buf)
    {
      if (lw6sys_file_exists (buf))
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _x_ ("reading README \"%s\""), buf);
	  readme = lw6sys_read_file_content (buf);
	}
      LW6SYS_FREE (buf);
    }

  if (!readme)
    {
      buf = lw6sys_path_concat (dirname, _LW6LDR_FILE_README_TXT);
      if (buf)
	{
	  if (lw6sys_file_exists (buf))
	    {
	      lw6sys_log (LW6SYS_LOG_INFO, _x_ ("reading README \"%s\""),
			  buf);
	      readme = lw6sys_read_file_content (buf);
	    }
	  LW6SYS_FREE (buf);
	}
    }

  if (!readme)
    {
      readme = lw6sys_str_copy (_x_ ("no README for this map"));
    }

  if (!readme)
    {
      lw6sys_log (LW6SYS_LOG_WARNING,
		  _x_
		  ("unable to handle map README file, running out of memory?"));
    }

  return readme;
}

/*
 * Read the metadata associated to a map. Pointer to metadata must be valid,
 * it's modified in-place.
 */
int
lw6ldr_metadata_read (lw6map_metadata_t * metadata, char *dirname)
{
  int ret = 0;

  lw6map_metadata_clear (metadata);

  metadata->title = extract_title_from_dirname (dirname);
  metadata->readme = read_readme (dirname);

  ret = (metadata->title && metadata->readme);

  return ret;
}
