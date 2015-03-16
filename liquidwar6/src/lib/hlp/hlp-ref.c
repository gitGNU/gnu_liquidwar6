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

#include "hlp.h"
#include "hlp-internal.h"

static lw6sys_hash_t *_references = NULL;

char *
_lw6hlp_true_false (int b)
{
  char *ret = _LW6HLP_DEFAULT_FALSE;

  if (b)
    {
      ret = _LW6HLP_DEFAULT_TRUE;
    }

  return ret;
}

_lw6hlp_reference_entry_t *
_lw6hlp_new_reference (lw6sys_context_t * sys_context, lw6hlp_type_t type, const char *key, const char *about, const char *default_value, int min_value,
		       int max_value)
{
  _lw6hlp_reference_entry_t *ret = NULL;

  ret = (_lw6hlp_reference_entry_t *) LW6SYS_MALLOC (sys_context, sizeof (_lw6hlp_reference_entry_t));
  if (ret)
    {
      ret->type = type;
      ret->key = key;
      ret->about = about;
      ret->default_value = default_value;
      ret->min_value = min_value;
      ret->max_value = max_value;
    }

  return ret;
}

int
_lw6hlp_populate_references (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 1;

  ret = _lw6hlp_populate_references_quick (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_doc (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_show (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_path (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_players (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_input (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_graphics (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_sound (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_network (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_map (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_map_rules (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_map_hints (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_map_style (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_map_teams (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_funcs (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_hooks (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_advanced (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_aliases (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_team_colors (sys_context, references) && ret;
  ret = _lw6hlp_populate_references_weapons (sys_context, references) && ret;

  return ret;
}

/**
 * lw6hlp_reference_init
 *
 * @sys_context: global system context
 *
 * Initializes the help reference, this must be called before
 * any call to lw6hlp_about or such help related functions.
 *
 * Return value: 1 on success, 0 if failed
 */
int
lw6hlp_reference_init (lw6sys_context_t * sys_context)
{
  int ret = 0;
  if (!_references)
    {
      _references = lw6sys_hash_new (sys_context, lw6sys_free_callback, LW6HLP_APPROX_NB_ENTRIES);
      if (_references)
	{
	  ret = _lw6hlp_populate_references (sys_context, _references);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't init, help reference already set up"));
    }

  return ret;
}

/**
 * lw6hlp_reference_quit
 *
 * @sys_context: global system context
 *
 * un-initializes the help reference, this must be called at the
 * end of the program.
 *
 * Return value: 1 on success, 0 if failed
 */
void
lw6hlp_reference_quit (lw6sys_context_t * sys_context)
{
  if (_references)
    {
      lw6sys_hash_free (sys_context, _references);
      _references = NULL;
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't quit, help reference is not set"));
    }
}

_lw6hlp_reference_entry_t *
_lw6hlp_reference_get_entry (lw6sys_context_t * sys_context, const char *key)
{
  _lw6hlp_reference_entry_t *entry = NULL;
  char *formatted_key = NULL;
  if (_references)
    {
      formatted_key = lw6sys_keyword_as_key (sys_context, key);
      if (formatted_key)
	{
	  entry = (_lw6hlp_reference_entry_t *) lw6sys_hash_get (sys_context, _references, formatted_key);
	  if (!entry)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("entry %s does not exists in help reference"), key);
	    }
	  LW6SYS_FREE (sys_context, formatted_key);
	}
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("trying to get entry before reference is set up"));
    }

  return entry;
}
