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

/*
 * Now, exceptionnally we include map.h without linking
 * on it, this is just to have default exp values.
 */
#include "../map/map.h"

#define _EXP_FILE "exp.xml"
#define _EXP_KEY "exp"
#define _CHECKSUM_KEY "checksum"

typedef struct _exp_s
{
  int exp;
  int checksum;
} _exp_t;

static char *
_get_filename (lw6sys_context_t * sys_context, const char *user_dir)
{
  return lw6sys_path_concat (sys_context, user_dir, _EXP_FILE);
}

static u_int32_t
_calc_checksum (lw6sys_context_t * sys_context, int exp)
{
  char *username = NULL;
  char *hostname = NULL;
  u_int32_t checksum = 0;

  username = lw6sys_get_username (sys_context);
  if (username)
    {
      hostname = lw6sys_get_hostname (sys_context);
      if (hostname)
	{
	  lw6sys_checksum_update_str (sys_context, &checksum, username);
	  lw6sys_checksum_update_str (sys_context, &checksum, hostname);
	  lw6sys_checksum_update_int32 (sys_context, &checksum, exp);
	  LW6SYS_FREE (sys_context, hostname);
	}
      LW6SYS_FREE (sys_context, username);
    }

  return checksum;
}

static void
_load_callback (lw6sys_context_t * sys_context, void *callback_data, const char *element, const char *key, const char *value)
{
  _exp_t *exp = (_exp_t *) callback_data;

  exp = (_exp_t *) callback_data;

  if (!strcmp (key, _EXP_KEY))
    {
      exp->exp = lw6sys_atoi (sys_context, value);
    }
  else if (!strcmp (key, _CHECKSUM_KEY))
    {
      exp->checksum = lw6sys_atoi (sys_context, value);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("item \"%s\" ignored"), key);
    }
}

/**
 * lw6cfg_load_exp
 *
 * @sys_context: global system context
 * @user_dir: the user directory
 * @exp: the exp (out param)
 *
 * Gets exp from file.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6cfg_load_exp (lw6sys_context_t * sys_context, const char *user_dir, int *exp)
{
  int ret = 0;
  char *filename = NULL;
  int checksum = 0;
  _exp_t exp_t;

  exp_t.exp = LW6MAP_RULES_MIN_EXP;
  exp_t.checksum = 0;

  filename = _get_filename (sys_context, user_dir);
  if (filename)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("loading exp from \"%s\""), filename);

      if (lw6sys_file_exists (sys_context, filename))
	{
	  ret = lw6cfg_read_key_value_xml_file (sys_context, filename, _load_callback, (void *) &exp_t);
	  checksum = _calc_checksum (sys_context, exp_t.exp);
	  if (checksum != exp_t.checksum)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("bad exp checksum"));
	      exp_t.exp = LW6MAP_RULES_MIN_EXP;
	    }
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("exp file \"%s\" doesn't exist, using defaults"), filename);
	}

      LW6SYS_FREE (sys_context, filename);
    }

  exp_t.exp = lw6sys_imin (LW6MAP_RULES_MAX_EXP, lw6sys_imax (LW6MAP_RULES_MIN_EXP, exp_t.exp));
  (*exp) = exp_t.exp;

  return ret;
}

/**
 * lw6cfg_save_exp
 *
 * @sys_context: global system context
 * @user_dir: the user directory
 * @exp: the exp
 *
 * Saves exp to file.
 *
 * Return value: 1 on success, 0 on failure
 */
int
lw6cfg_save_exp (lw6sys_context_t * sys_context, const char *user_dir, int exp)
{
  int ret = 0;
  FILE *f;
  char *filename = NULL;
  int checksum = 0;

  filename = _get_filename (sys_context, user_dir);
  if (filename)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("saving exp to \"%s\""), filename);

      f = fopen (filename, "wb");
      if (f)
	{
	  lw6sys_print_xml_header (sys_context, f,
				   _x_
				   ("This is where your exp is kept. Please do not edit, this would be assimilated to cheating, while it's not that hard to fool the game and make it believe you're super strong when you are not, such practice is not encouraged. It's believed it's more fun to wait until this number increases naturally."));

	  exp = lw6sys_imin (LW6MAP_RULES_MAX_EXP, lw6sys_imax (LW6MAP_RULES_MIN_EXP, exp));
	  checksum = _calc_checksum (sys_context, exp);
	  fprintf (f, "<int key=\"%s\" value=\"%d\" />%s", _EXP_KEY, exp, lw6sys_eol ());
	  fprintf (f, "<int key=\"%s\" value=\"%d\" />%s", _CHECKSUM_KEY, checksum, lw6sys_eol ());
	  lw6sys_print_xml_footer (sys_context, f);
	  fclose (f);
	  ret = 1;
	}
      else
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("could not open file \"%s\" in write mode"), filename);
	}
      LW6SYS_FREE (sys_context, filename);
    }

  return ret;
}
