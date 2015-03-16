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

_lw6cfg_context_t *
_lw6cfg_init (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  _lw6cfg_context_t *_cfg_context = NULL;

  _cfg_context = (_lw6cfg_context_t *) LW6SYS_CALLOC (sys_context, sizeof (_lw6cfg_context_t));
  if (_cfg_context)
    {
      _cfg_context->config_file = lw6sys_get_config_file (sys_context, argc, argv);
      if (_cfg_context->config_file)
	{
	  _cfg_context->options = lw6sys_hash_new (sys_context, lw6sys_free_callback, LW6HLP_APPROX_NB_ENTRIES);
	  if (_cfg_context->options)
	    {
	      _cfg_context->spinlock = lw6sys_spinlock_create (sys_context);
	      if (_cfg_context->spinlock)
		{
		  _cfg_context->argc = argc;
		  _cfg_context->argv = argv;
		  _lw6cfg_parse_command_line (sys_context, _cfg_context);
		}
	      else
		{
		  lw6sys_hash_free (sys_context, _cfg_context->options);
		  LW6SYS_FREE (sys_context, _cfg_context->config_file);
		  LW6SYS_FREE (sys_context, _cfg_context);
		  _cfg_context = NULL;
		}
	    }
	  else
	    {
	      LW6SYS_FREE (sys_context, _cfg_context->config_file);
	      LW6SYS_FREE (sys_context, _cfg_context);
	      _cfg_context = NULL;
	    }
	}
      else
	{
	  LW6SYS_FREE (sys_context, _cfg_context);
	  _cfg_context = NULL;
	}
    }

  if (!_cfg_context)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to initialize config"));
    }

  return _cfg_context;
}

/**
 * lw6cfg_init
 *
 * @sys_context: global system context
 * @argc: number of command line arguments, as given to @main
 * @argv: a list of command line arguments, as given to @main
 *
 * Initializes a config context object. This object is hidden
 * behind an opaque void * pointer to avoid direct access to
 * its elements.
 *
 * Return value: an opaque pointer, must be freed with @lw6cfg_quit.
 */
void *
lw6cfg_init (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  return (void *) _lw6cfg_init (sys_context, argc, argv);
}

void
_lw6cfg_quit (lw6sys_context_t * sys_context, _lw6cfg_context_t * _cfg_context)
{
  if (_cfg_context)
    {
      lw6sys_spinlock_destroy (sys_context, _cfg_context->spinlock);
      lw6sys_hash_free (sys_context, _cfg_context->options);
      LW6SYS_FREE (sys_context, _cfg_context->config_file);
      LW6SYS_FREE (sys_context, _cfg_context);
    }
}

/**
 * lw6cfg_quit
 *
 * @sys_context: global system context
 * @cfg_context: a context returned by @lw6cfg_init
 *
 * Frees a config cfg_context object. You must call this
 * once you're done with the context.
 *
 * Return value: none.
 */
void
lw6cfg_quit (lw6sys_context_t * sys_context, void *cfg_context)
{
  _lw6cfg_quit (sys_context, (_lw6cfg_context_t *) cfg_context);
}

/**
 * lw6cfg_reset
 *
 * @sys_context: global system context
 * @argc: number of command line arguments, as given to @main
 * @argv: a list of command line arguments, as given to @main
 *
 * Overwrites the config file with defaults.
 * Use this to get rid of old configurations.
 */
void
lw6cfg_reset (lw6sys_context_t * sys_context, int argc, const char *argv[])
{
  char *config_file = NULL;
  void *cfg_context = NULL;
  char *value = NULL;

  config_file = lw6sys_get_config_file (sys_context, argc, argv);
  if (config_file)
    {
      cfg_context = lw6cfg_init (sys_context, argc, argv);
      if (cfg_context)
	{
	  value = lw6sys_get_user_dir (sys_context, argc, argv);
	  if (value)
	    {
	      lw6cfg_set_option (sys_context, cfg_context, LW6DEF_USER_DIR, value);
	      LW6SYS_FREE (sys_context, value);
	    }
	  value = lw6sys_get_log_file (sys_context, argc, argv);
	  if (value)
	    {
	      lw6cfg_set_option (sys_context, cfg_context, LW6DEF_LOG_FILE, value);
	      LW6SYS_FREE (sys_context, value);
	    }
	  lw6cfg_set_option (sys_context, cfg_context, LW6DEF_MUSIC_PATH, "");
	  lw6cfg_set_option (sys_context, cfg_context, LW6DEF_MAP_PATH, "");
	  lw6cfg_set_option (sys_context, cfg_context, LW6DEF_BIN_ID, "0");
	  lw6cfg_save (sys_context, cfg_context, config_file);
	  lw6cfg_quit (sys_context, cfg_context);
	}
      LW6SYS_FREE (sys_context, config_file);
    }
}
