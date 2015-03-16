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

#include "../cfg/cfg.h"
#include "net-internal.h"

#define CONST_FILE "net/net-const.xml"

static void
_read_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  _lw6net_const_data_t *const_data;

  const_data = (_lw6net_const_data_t *) callback_data;

  if (!strcmp (element, "int"))
    {
      lw6cfg_read_xml_int (sys_context, key, value, "listen-backlog", &const_data->listen_backlog);
      lw6cfg_read_xml_int (sys_context, key, value, "chunk-size", &const_data->chunk_size);
      lw6cfg_read_xml_int (sys_context, key, value, "line-size", &const_data->line_size);
      lw6cfg_read_xml_int (sys_context, key, value, "line-delay-msec", &const_data->line_delay_msec);
      lw6cfg_read_xml_int (sys_context, key, value, "dns-cache-hash-size", &const_data->dns_cache_hash_size);
      lw6cfg_read_xml_int (sys_context, key, value, "dns-cache-delay-sec", &const_data->dns_cache_delay_sec);
      lw6cfg_read_xml_int (sys_context, key, value, "connectable-cache-hash-size", &const_data->connectable_cache_hash_size);
      lw6cfg_read_xml_int (sys_context, key, value, "connectable-cache-delay-sec", &const_data->connectable_cache_delay_sec);
    }
}

/*
 * Loads constants.
 */
int
_lw6net_const_init (lw6sys_context_t * sys_context, int argc, const char *argv[], _lw6net_const_data_t * const_data)
{
  int ret = 0;
  char *const_file = NULL;
  char *data_root_dir = NULL;

  data_root_dir = lw6sys_get_data_dir (sys_context, argc, argv);

  if (data_root_dir)
    {
      const_file = lw6sys_path_concat (sys_context, data_root_dir, CONST_FILE);

      if (const_file)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("reading \"%s\""), const_file);

	  ret = lw6cfg_read_key_value_xml_file (sys_context, const_file, _read_callback, (void *) const_data);
	  LW6SYS_FREE (sys_context, const_file);
	}
      LW6SYS_FREE (sys_context, data_root_dir);
    }

  /*
   * Check for max value since some code relies on fixed length buffers.
   * Anyway with MTUs being arround 1500, the limit is purely theorical,
   * kernel network stack will stop us before that.
   */
  const_data->chunk_size = lw6sys_imin (LW6NET_MAX_CHUNK_SIZE, const_data->chunk_size);
  const_data->line_size = lw6sys_imin (LW6NET_MAX_LINE_SIZE, const_data->line_size);

  return ret;
}

/*
 * Unload constants, free memory
 */
void
_lw6net_const_quit (lw6sys_context_t * sys_context, _lw6net_const_data_t * const_data)
{
  memset (const_data, 0, sizeof (_lw6net_const_data_t));
}
