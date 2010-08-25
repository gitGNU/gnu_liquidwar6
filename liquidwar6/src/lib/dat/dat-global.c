/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010  Christian Mauduit <ufoot@ufoot.org>

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

#include "dat-internal.h"

#define CONST_FILE "dat/dat-const.xml"

static void
read_callback (void *callback_data, char *element, char *key, char *value)
{
  _lw6dat_const_data_t *const_data;

  const_data = (_lw6dat_const_data_t *) callback_data;

  if (!strcmp (element, "int"))
    {
      lw6cfg_read_xml_int (key, value, "max-nb-blocks",
			   &const_data->max_nb_blocks);
      lw6cfg_read_xml_int (key, value, "nb-atoms-per-block",
			   &const_data->nb_atoms_per_block);
    }
}

int
_lw6dat_const_load (int argc, char *argv[], _lw6dat_const_t * const_data)
{
  int ret = 0;
  char *const_file = NULL;
  char *data_root_dir = NULL;

  data_root_dir = lw6sys_get_data_dir (argc, argv);

  if (data_root_dir)
    {
      const_file = lw6sys_path_concat (data_root_dir, CONST_FILE);

      if (const_file)
	{
	  lw6sys_log (LW6SYS_LOG_INFO, _("reading \"%s\""), const_file);

	  ret =
	    lw6cfg_read_key_value_xml_file (const_file, read_callback,
					    (void *) const_data);
	  LW6SYS_FREE (const_file);
	}
      LW6SYS_FREE (data_root_dir);
    }

  return ret;
}

void
_lw6dat_const_unload (_lw6dat_const_t * const_data)
{
  memset (const_data, 0, sizeof (_lw6dat_const_data_t));
}
