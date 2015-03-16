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

#include "ldr-png.h"

#include "ldr.h"
#include "ldr-internal.h"

/**
 * lw6ldr_read
 *
 * @sys_context: global system context
 * @dirname: the directory containing the map
 * @default_param: default parameters, as strings
 * @forced_param: forced parameters, as strings
 * @display_w: the width of the display output (resolution)
 * @display_h: the height of the display output (resolution)
 * @bench_value: the bench value (depends on computer capacity)
 * @magic_number: arbitrary constant
 * @user_dir: the user directory
 * @progress: information used to handle the progress bar
 *
 * Loads a map from dist. The default_param and forced_param can contain
 * values corresponding to rules.xml and style.xml entries. Parameters
 * are read in 4 steps. 1st, a default value is picked by the program.
 * 2nd, any value in @default_param replaces previous values. 3rd, any
 * value in rules.xml or style.xml replaces previous values.
 * 4th, any value in @forced_param replaces previous values. In practice,
 * the @default_param allows the user to set defaults which can still
 * be overwritten by the map, while @forced_param is a definitive 'ignore
 * what is is defined in the map' way of doing things.
 * See also @lw6ldr_read_relative.
 *
 * Return value: 1 if success, 0 if failed.
 */
lw6map_level_t *
lw6ldr_read (lw6sys_context_t * sys_context, const char *dirname, lw6sys_assoc_t * default_param,
	     lw6sys_assoc_t * forced_param, int display_w, int display_h,
	     int bench_value, int magic_number, const char *user_dir, lw6sys_progress_t * progress)
{
  lw6map_level_t *level = NULL;
  int ok = 0;
  float ratio = 0.0f;		// important, in case texture isn't found/used
  int texture_exists = 0;
  lw6sys_progress_t progress_texture;
  lw6sys_progress_t progress_body;
  lw6sys_progress_t progress_color;
  lw6ldr_hints_t hints;
  lw6map_rules_t rules_tmp;
  lw6ldr_use_t use;
  int player_exp = LW6MAP_RULES_DEFAULT_EXP;
  int map_exp = LW6MAP_RULES_DEFAULT_EXP;

  lw6sys_log (sys_context, LW6SYS_LOG_INFO, _x_ ("loading map \"%s\" display=%dx%d bench=%d"), dirname, display_w, display_h, bench_value);

  lw6sys_progress_split3 (sys_context, &progress_texture, &progress_body, &progress_color, progress);

  level = lw6map_new (sys_context);
  if (level)
    {
      ok = 1;

      lw6cfg_load_exp (sys_context, user_dir, &player_exp);
      lw6map_param_defaults (sys_context, &(level->param));
      lw6ldr_hints_defaults (sys_context, &hints);
      lw6ldr_use_defaults (sys_context, &use);

      ok = ok && lw6ldr_metadata_read (sys_context, &level->metadata, dirname);
      ok = ok && lw6map_local_info_set_music_dir (sys_context, &level->local_info, dirname);
      lw6ldr_rules_update (sys_context, &(level->param.rules), default_param);
      level->param.rules.exp = LW6MAP_RULES_DEFAULT_EXP;
      lw6ldr_hints_update (sys_context, &hints, default_param);
      lw6ldr_style_update (sys_context, &(level->param.style), default_param);
      lw6ldr_teams_update (sys_context, &(level->param.teams), default_param);
      lw6ldr_use_update (sys_context, &use, default_param);
      if (use.use_rules_xml)
	{
	  ok = ok && lw6ldr_rules_read (sys_context, &level->param.rules, dirname);
	  map_exp = level->param.rules.exp;
	  lw6ldr_rules_update (sys_context, &(level->param.rules), forced_param);
	  level->param.rules.exp = map_exp;
	}
      else
	{
	  /*
	   * OK we don't use rules, but still we need to know
	   * the "exp" associated to the level...
	   */
	  lw6map_rules_defaults (sys_context, &rules_tmp);
	  lw6ldr_rules_read (sys_context, &rules_tmp, dirname);
	  map_exp = rules_tmp.exp;
	}
      // following is important else exp increase won't work
      level->metadata.vanilla_exp = map_exp;

      level->param.rules.highest_team_color_allowed =
	lw6sys_imin (LW6MAP_MAX_NB_TEAMS - 1,
		     lw6sys_imin (level->param.rules.highest_team_color_allowed, lw6map_exp_get_highest_team_color_allowed (sys_context, player_exp)));
      level->param.rules.highest_weapon_allowed =
	lw6sys_imin (LW6MAP_MAX_WEAPON_ID,
		     lw6sys_imin (level->param.rules.highest_weapon_allowed, lw6map_exp_get_highest_weapon_allowed (sys_context, player_exp)));

      if (use.use_hints_xml)
	{
	  ok = ok && lw6ldr_hints_read (sys_context, &hints, dirname);
	  lw6ldr_hints_update (sys_context, (&hints), forced_param);
	}
      if (use.use_style_xml)
	{
	  ok = ok && lw6ldr_style_read (sys_context, &(level->param.style), dirname);
	  lw6ldr_style_update (sys_context, &(level->param.style), forced_param);
	}
      if (use.use_teams_xml)
	{
	  ok = ok && lw6ldr_teams_read (sys_context, &(level->param.teams), dirname);
	  lw6ldr_teams_update (sys_context, &(level->param.teams), forced_param);
	}
      if (use.use_texture)
	{
	  lw6ldr_auto_colors (sys_context, &level->param.style, &hints);
	  /*
	   * Note that this will return 0 only on errors, if there's just
	   * no texture (texture.jpg) file then it will return true but
	   * texture_exists will contain 0.
	   */
	  ok = ok && lw6ldr_texture_read (sys_context, &level->texture, dirname,
					  &(level->param), &hints, use.use_texture, display_w, display_h, &ratio, &texture_exists, &progress_texture);
	}
      if (use.use_cursor_texture)
	{
	  ok = ok && lw6ldr_cursor_texture_read (sys_context, &level->cursor_texture, dirname);
	}
      else
	{
	  lw6map_cursor_texture_builtin (sys_context, &(level->cursor_texture));
	}
      ok = ok
	&& lw6ldr_body_read (sys_context, &level->body, dirname, &(level->param), &hints, display_w, display_h, ratio, bench_value, magic_number,
			     &progress_body);
      if (!level->texture.data)
	{
	  ok = ok && lw6map_texture_from_body (sys_context, &level->texture, &level->body, &(level->param.style.color_set.view_color_map));
	}
      if (ok && texture_exists)
	{
	  if (hints.guess_colors)
	    {
	      _lw6ldr_guess_colors (sys_context, level, &progress_color);
	      _lw6ldr_apply_guessed_colors (sys_context, level);
	    }
	}
      lw6ldr_auto_colors (sys_context, &level->param.style, &hints);
      level->texture.has_alpha = lw6map_texture_has_alpha (sys_context, &(level->texture));
      if (ok)
	{
	  if (player_exp >= map_exp)
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_DEBUG, _x_ ("OK, can load \"%s\" with exp=%d, requires only %d"), dirname, player_exp, map_exp);
	      _lw6ldr_exp_fix (sys_context, &(level->param.rules), player_exp);
	    }
	  else
	    {
	      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't load \"%s\" with exp=%d, requires %d"), dirname, player_exp, map_exp);
	      ok = 0;
	    }
	}
    }

  if (!ok)
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to load map \"%s\""), dirname);
      lw6map_free (sys_context, level);
      level = NULL;
    }

  return level;
}

/**
 * lw6ldr_read_relative
 *
 * @sys_context: global system context
 * @map_path: a collection of paths where to find maps
 * @relative_path: something which will be appended to a @map_path member
 * @default_param: default parameters, as strings
 * @forced_param: forced parameters, as strings
 * @display_w: the width of the display output (resolution)
 * @display_h: the height of the display output (resolution)
 * @bench_value: the bench value (depends on computer capacity)
 * @magic_number: arbitrary constant
 * @user_dir: the user directory
 * @progress: information used to handle the progress bar
 *
 * Reads a map from disk, using the map-path value, which is a collection
 * of paths defined by the command-line, the environment variables, and
 * the config file. @default_param and @forced_param work as in the
 * function @lw6ldr_read.
 *
 * Return value: 1 if success, 0 if failure.
 */
lw6map_level_t *
lw6ldr_read_relative (lw6sys_context_t * sys_context, const char *map_path, const char *relative_path,
		      lw6sys_assoc_t * default_param,
		      lw6sys_assoc_t * forced_param,
		      int display_w, int display_h, int bench_value, int magic_number, const char *user_dir, lw6sys_progress_t * progress)
{
  lw6map_level_t *ret = NULL;
  lw6sys_list_t *dirs = NULL;
  char *dir = NULL;
  char *full_dir = NULL;

  dirs = lw6sys_env_split (sys_context, map_path);
  if (dirs)
    {
      while (dirs && (dir = (char *) lw6sys_lifo_pop (sys_context, &dirs)) != NULL)
	{
	  full_dir = lw6sys_path_concat (sys_context, dir, relative_path);
	  if (full_dir)
	    {
	      if (lw6sys_dir_exists (sys_context, full_dir))
		{
		  if (!ret)
		    {
		      ret =
			lw6ldr_read (sys_context, full_dir, default_param, forced_param, display_w, display_h, bench_value, magic_number, user_dir, progress);
		    }
		}
	      LW6SYS_FREE (sys_context, full_dir);
	    }
	  LW6SYS_FREE (sys_context, dir);
	}
    }

  return ret;
}
