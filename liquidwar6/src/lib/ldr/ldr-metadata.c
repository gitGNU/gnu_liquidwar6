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

#include "ldr.h"
#include "ldr-internal.h"

static char *
_extract_title_from_dirname (lw6sys_context_t * sys_context, const char *dirname)
{
  char *copy = NULL;
  char *title = NULL;
  char *begin = NULL;

  copy = lw6sys_path_strip_slash (sys_context, dirname);
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

      title = lw6sys_str_copy (sys_context, begin);

      LW6SYS_FREE (sys_context, copy);
    }

  if (!title)
    {
      title = lw6sys_str_copy (sys_context, _x_ ("no title for this map"));
    }

  if (!title)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to handle map title, running out of memory?"));
    }

  return title;
}

static char *
_read_readme (lw6sys_context_t * sys_context, const char *dirname)
{
  char *buf = NULL;
  char *readme = NULL;

  buf = lw6sys_path_concat (sys_context, dirname, LW6SYS_FILE_README);
  if (buf)
    {
      if (lw6sys_file_exists (sys_context, buf))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading README \"%s\""), buf);
	  readme = lw6sys_read_file_content (sys_context, buf);
	}
      LW6SYS_FREE (sys_context, buf);
    }

  if (!readme)
    {
      buf = lw6sys_path_concat (sys_context, dirname, LW6SYS_FILE_README_TXT);
      if (buf)
	{
	  if (lw6sys_file_exists (sys_context, buf))
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading README \"%s\""), buf);
	      readme = lw6sys_read_file_content (sys_context, buf);
	    }
	  LW6SYS_FREE (sys_context, buf);
	}
    }

  if (!readme)
    {
      readme = lw6sys_str_copy (sys_context, _x_ ("no README for this map"));
    }

  if (!readme)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to handle map README file, running out of memory?"));
    }

  return readme;
}

static void
_read_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  lw6map_metadata_t *metadata_data;

  metadata_data = (lw6map_metadata_t *) callback_data;

  if (!strcmp (element, "string"))
    {
      lw6cfg_read_xml_string (sys_context, key, value, "title", &(metadata_data->title));
      lw6cfg_read_xml_string (sys_context, key, value, "author", &(metadata_data->author));
      lw6cfg_read_xml_string (sys_context, key, value, "description", &(metadata_data->description));
      lw6cfg_read_xml_string (sys_context, key, value, "license", &(metadata_data->license));
    }
}

/**
 * lw6ldr_metadata_read
 *
 * @sys_context: global system context
 * @metadata: structure containting read data (out param)
 * @dirname: map dirname (absolute path)
 *
 * Reads the metadata, will first parse metadata.xml, and if not available
 * read README and guess a title from map path. When function returns,
 * all fields in structure are non-NULL.
 *
 * Return value: 1 on success, 0 on failure.
 */
int
lw6ldr_metadata_read (lw6sys_context_t * sys_context, lw6map_metadata_t * metadata, const char *dirname)
{
  int ret = 0;
  char *buf = NULL;

  lw6map_metadata_clear (sys_context, metadata);

  buf = lw6sys_path_concat (sys_context, dirname, _LW6LDR_FILE_METADATA_XML);
  if (buf)
    {
      if (lw6sys_file_exists (sys_context, buf))
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading metadata \"%s\""), buf);
	  ret = lw6cfg_read_key_value_xml_file (sys_context, buf, _read_callback, (void *) metadata);
	}
      LW6SYS_FREE (sys_context, buf);
    }

  if (!(metadata->title))
    {
      metadata->title = _extract_title_from_dirname (sys_context, dirname);
    }
  if (!(metadata->author))
    {
      metadata->author = lw6sys_str_copy (sys_context, _x_ ("unknown"));
    }
  if (!(metadata->description))
    {
      metadata->description = _read_readme (sys_context, dirname);
    }
  if (!(metadata->license))
    {
      metadata->license = lw6sys_str_copy (sys_context, _x_ ("unspecified"));
    }

  ret = (metadata->title && metadata->author && metadata->description && metadata->license);

  return ret;
}
