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

#include "cfg.h"
#include "cfg-internal.h"
#include "../hlp/hlp.h"

/**
 * lw6cfg_must_be_saved
 *
 * @sys_context: global system context
 * @key: key to test
 *
 * Tells wether a key should be saved in the config file.
 * Typically, some options you don't want to savem such as
 * the location of the config file itself. Most of those
 * non-savable parameters are path-related. This does not
 * mean no paths are saved in the config file.
 *
 * Return value: 1 if must be saved, 0 if not
 */
int
lw6cfg_must_be_saved (lw6sys_context_t * sys_context, const char *key)
{
  int ret = 1;

  if (lw6hlp_match (sys_context, key, LW6DEF_CONFIG_FILE) ||
      lw6hlp_match (sys_context, key, LW6DEF_PREFIX) ||
      lw6hlp_match (sys_context, key, LW6DEF_MOD_DIR) ||
      lw6hlp_match (sys_context, key, LW6DEF_DATA_DIR) ||
      lw6hlp_match (sys_context, key, LW6DEF_MUSIC_DIR) || lw6hlp_match (sys_context, key, LW6DEF_MAP_DIR)
      || lw6hlp_match (sys_context, key, LW6DEF_SCRIPT_FILE))
    {
      /*
       * These options must not be save in the config file, doing so
       * would mess things up when upgrading the game (version change ==
       * change of paths to access data & the rest).
       */
      ret = 0;
    }

  return ret;
}

int
_lw6cfg_save (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context, const char *filename)
{
  int ret = 0;
  FILE *f;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("saving config to \"%s\""), filename);

  _lw6cfg_parse_command_line (sys_context, _cfg_context);

  f = fopen (filename, "wb");
  if (f)
    {
      lw6sys_print_xml_header (sys_context, f,
			       _x_
			       ("This is the main Liquid War 6 config file. Here you'll be able to tweak many, if not all, parameters in the game. Some of these values simply reflect changes you can make through the interface, some are not even present in the menus. What is sure is that if you can modify it by clicking somewhere in the game interface, it can surely be done here too. Note that this file is overwritten every time you run the game, your own comments and personnal touch in it will simply be squashed and disappear. But of course the values you set will be kept. All entries should be documented in the file. This file should only contain entries that have values which are different from the default, so if you set a value to the default, it will automatically be removed from the file. If in doubt, documentation is online on http://www.gnu.org/software/liquidwar6/manual/"));
      if (lw6sys_spinlock_lock (sys_context, _cfg_context->spinlock))
	{
	  lw6sys_hash_sort_and_map (sys_context, _cfg_context->options, (lw6sys_assoc_callback_func_t) lw6cfg_write_xml_guess_type_skip_same, (void *) f);
	  lw6sys_spinlock_unlock (sys_context, _cfg_context->spinlock);
	}
      lw6sys_print_xml_footer (sys_context, f);
      fclose (f);
      ret = 1;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("could not open file \"%s\" in write mode"), filename);
    }

  return ret;
}

/**
 * lw6cfg_save
 *
 * @sys_context: global system context
 * @cfg_context: a context returned by @lw6cfg_init
 * @filename: a file path, absolute or relative
 *
 * Save current options into the given config file. Before saving
 * the file, all command line arguments will be read and will
 * override current values. This means the saved file will contain
 * values given as command line arguments.
 *
 * Return value: 1 if successfull, 0 if error.
 */
int
lw6cfg_save (lw6sys_context_t * sys_context, void *cfg_context, const char *filename)
{
  return _lw6cfg_save (sys_context, (_lw6cfg_context_t *) cfg_context, filename);
}
