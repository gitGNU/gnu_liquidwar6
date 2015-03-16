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

#include <CUnit/CUnit.h>

#include "map.h"

/*
 * The checksums below will change any time a field is added
 * or its default is changed in the map structure.
 */
#define _TEST_RULES_CHECKSUM 0xe45216e7

/*
 * At some point a checksum on hexa dump was implemented but
 * this is not a very good idea, a map can have (int its dump)
 * some floats which are possibly not absolutely totally exact
 * and could behave differently on different hardware, so it's
 * been disabled and it's probably a good idea to keep it
 * disabled.
 */
//#define _TEST_LEVEL_HEXA_CHECKSUM 0x285faeeb

#define _TEST_MAP_NB_LAYERS 2
#define _TEST_MAP_WIDTH 20
#define _TEST_MAP_HEIGHT 15
#define _TEST_MAP_NOISE_PERCENT 30
#define _TEST_RULES_KEY LW6DEF_TOTAL_TIME
#define _TEST_RULES_VALUE 3600
#define _TEST_STYLE_KEY LW6DEF_ZOOM
#define _TEST_STYLE_VALUE "1.0"
#define _TEST_TEAMS_KEY LW6DEF_BOT_IQ
#define _TEST_TEAMS_VALUE "99"
#define _TEST_PARAM_KEY LW6DEF_MOVES_PER_ROUND
#define _TEST_PARAM_VALUE "3"
#define _TEST_COORDS_NB 5
#define _TEST_COORDS_W 100
#define _TEST_COORDS_H 50
#define _TEST_COORDS_D 1
#define _TEST_COORDS_X1 66
#define _TEST_COORDS_Y1 33
#define _TEST_COORDS_X2 130
#define _TEST_COORDS_Y2 20
#define _TEST_COORDS_X3 -30
#define _TEST_COORDS_Y3 20
#define _TEST_COORDS_X4 -10
#define _TEST_COORDS_Y4 -5
#define _TEST_COORDS_X5 1000
#define _TEST_COORDS_Y5 51
#define _TEST_MUSIC_DIR1 "../"
#define _TEST_MUSIC_DIR2 "/foo/bar"
#define _TEST_BUILTIN_WIDTH 100
#define _TEST_BUILTIN_HEIGHT 50
#define _TEST_BUILTIN_DEPTH 4
#define _TEST_BUILTIN_NOISE_PERCENT 5
#define _TEST_BUILTIN_SCALE 250

typedef struct _lw6map_test_data_s
{
  int ret;
  lw6sys_context_t *sys_context;
} _lw6map_test_data_t;

static _lw6map_test_data_t _test_data = { 0, NULL };

/*
 * Testing basic new & free functions
 */
static void
_test_new ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level = NULL;
    char *repr = NULL;

    level = lw6map_new (sys_context);
    if (LW6SYS_TEST_ACK (level))
      {
	repr = lw6map_repr (sys_context, level);
	if (LW6SYS_TEST_ACK (repr))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("empty map \"%s\" constructed"), repr);
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			_x_
			("max_nb_colors=%d max_nb_cursors=%d max_nb_nodes=%d"),
			lw6map_get_max_nb_colors (sys_context, level),
			lw6map_get_max_nb_cursors (sys_context, level), lw6map_get_max_nb_nodes (sys_context, level));
	    LW6SYS_FREE (sys_context, repr);
	  }
	else
	  {
	    ret = 0;
	  }
	lw6map_free (sys_context, level);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_color ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_color_couple_t color_couple;
    int i, j;
    char *key;
    char *label;

    color_couple.bg = LW6SYS_COLOR_8_WHITE;
    color_couple.fg = LW6SYS_COLOR_8_BLACK;

    lw6map_color_invert (sys_context, &color_couple);
    if (!LW6SYS_TEST_ACK (lw6map_color_is_same (sys_context, &color_couple, &color_couple)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("color_is_same pretending colors are different, while they should be equal"));
	ret = 0;
      }

    for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
      {
	key = lw6map_team_color_index_to_key (sys_context, i);
	if (LW6SYS_TEST_ACK (key))
	  {
	    label = lw6map_team_color_index_to_label (sys_context, i);
	    if (LW6SYS_TEST_ACK (label))
	      {
		j = lw6map_team_color_key_to_index (sys_context, key);
		if (LW6SYS_TEST_ACK (i == j))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("team_color key for %d is \"%s\" label is \"%s\""), i, key, label);
		  }
		else
		  {
		    ret = 0;
		  }
	      }
	    else
	      {
		ret = 0;
	      }
	  }
	else
	  {
	    ret = 0;
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_coords ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_rules_t rules;
    lw6sys_whd_t shape;
    int _test_x[_TEST_COORDS_NB] = { _TEST_COORDS_X1, _TEST_COORDS_X2, _TEST_COORDS_X3, _TEST_COORDS_X4,
      _TEST_COORDS_X5
    };
    int _test_y[_TEST_COORDS_NB] = { _TEST_COORDS_Y1, _TEST_COORDS_Y2, _TEST_COORDS_Y3, _TEST_COORDS_Y4,
      _TEST_COORDS_Y5
    };
    int i, x, y, px, py;

    lw6map_rules_defaults (sys_context, &rules);
    shape.w = _TEST_COORDS_W;
    shape.h = _TEST_COORDS_H;
    shape.d = _TEST_COORDS_D;

    for (py = 1; py >= -1; --py)
      {
	for (px = 1; px >= -1; --px)
	  {
	    rules.x_polarity = px;
	    rules.y_polarity = py;
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("polarity set to %d,%d"), px, py);
	    for (i = 0; i < _TEST_COORDS_NB; ++i)
	      {
		x = _test_x[i];
		y = _test_y[i];
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("coords before fix %d,%d"), x, y);
		lw6map_coords_fix_xy (&rules, &shape, &x, &y);
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("coords after fix %d,%d"), x, y);
	      }
	  }
      }

    ret = 1;
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_print_meta_layer (lw6sys_context_t * sys_context, lw6map_meta_layer_t * meta_layer, char *label)
{
  int x, y, w, h;
  int value;

  if (lw6sys_log_get_console_state (sys_context))
    {
      w = meta_layer->shape.w;
      h = meta_layer->shape.h;

      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("meta-layer \"%s\""), label);
      for (y = 0; y < h; ++y)
	{
	  for (x = 0; x < w; ++x)
	    {
	      value = lw6map_meta_layer_get (sys_context, meta_layer, x, y);
	      printf ("%03d ", value);
	    }
	  printf ("\n");
	}
      fflush (stdout);
    }
}

static void
_print_body (lw6sys_context_t * sys_context, lw6map_body_t * body)
{
  int x, y, z, w, h, d;

  if (lw6sys_log_get_console_state (sys_context))
    {
      w = body->shape.w;
      h = body->shape.h;
      d = body->shape.d;

      for (z = 0; z < d; ++z)
	{
	  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("layer%d (z=%d)"), z + 1, z);
	  for (y = 0; y < h; ++y)
	    {
	      for (x = 0; x < w; ++x)
		{
		  if (lw6map_body_get (body, x, y, z))
		    {
		      printf (".");
		    }
		  else
		    {
		      printf ("#");
		    }
		}
	      printf ("\n");
	    }
	}
      fflush (stdout);
    }
}

static void
_print_cursor_texture (lw6sys_context_t * sys_context, lw6map_cursor_texture_t * cursor_texture)
{
  int x, y;
  lw6sys_color_8_t fg_bg;
  lw6sys_color_8_t color;

  if (lw6sys_log_get_console_state (sys_context))
    {
      lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("cursor_texture:"));
      for (y = 0; y < LW6MAP_CURSOR_TEXTURE_SIZE; ++y)
	{
	  for (x = 0; x < LW6MAP_CURSOR_TEXTURE_SIZE; ++x)
	    {
	      fg_bg = lw6map_cursor_texture_layer_get (sys_context, &(cursor_texture->fg_bg_layer), x, y);
	      color = lw6map_cursor_texture_layer_get (sys_context, &(cursor_texture->color_layer), x, y);
	      if (color.a < 128)
		{
		  if (fg_bg.a < 128)
		    {
		      printf (".");
		    }
		  else
		    {
		      printf ("#");
		    }
		}
	      else
		{
		  printf ("C");
		}
	    }
	  printf ("\n");
	}

      fflush (stdout);
    }
}

/*
 * Testing builtin
 */
static void
_test_builtin ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level = NULL;
    char *repr = NULL;
    int has_alpha = 0;

    level = lw6map_builtin_defaults (sys_context);
    if (LW6SYS_TEST_ACK (level))
      {
	repr = lw6map_repr (sys_context, level);
	if (LW6SYS_TEST_ACK (repr))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("default map \"%s\" constructed"), repr);
	    has_alpha = lw6map_texture_has_alpha (sys_context, &(level->texture));
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("has_alpha=%d"), has_alpha);
	    _print_body (sys_context, &level->body);
	    _print_cursor_texture (sys_context, &level->cursor_texture);
	    LW6SYS_FREE (sys_context, repr);
	  }
	else
	  {
	    ret = 0;
	  }
	lw6map_free (sys_context, level);
      }
    else
      {
	ret = 0;
      }

    level = lw6map_builtin_custom (sys_context, _TEST_BUILTIN_WIDTH, _TEST_BUILTIN_HEIGHT, _TEST_BUILTIN_DEPTH, _TEST_BUILTIN_NOISE_PERCENT);
    if (LW6SYS_TEST_ACK (level))
      {
	repr = lw6map_repr (sys_context, level);
	if (LW6SYS_TEST_ACK (repr))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("default map \"%s\" constructed"), repr);
	    has_alpha = lw6map_texture_has_alpha (sys_context, &(level->texture));
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("has_alpha=%d"), has_alpha);
	    _print_body (sys_context, &level->body);
	    _print_cursor_texture (sys_context, &level->cursor_texture);
	    LW6SYS_FREE (sys_context, repr);
	  }
	else
	  {
	    ret = 0;
	  }
	lw6map_free (sys_context, level);
      }
    else
      {
	ret = 0;
      }

    level = lw6map_builtin_scale (sys_context, _TEST_BUILTIN_SCALE);
    if (LW6SYS_TEST_ACK (level))
      {
	repr = lw6map_repr (sys_context, level);
	if (LW6SYS_TEST_ACK (repr))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("default map \"%s\" constructed"), repr);
	    has_alpha = lw6map_texture_has_alpha (sys_context, &(level->texture));
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("has_alpha=%d"), has_alpha);
	    _print_body (sys_context, &level->body);
	    _print_cursor_texture (sys_context, &level->cursor_texture);
	    LW6SYS_FREE (sys_context, repr);
	  }
	else
	  {
	    ret = 0;
	  }
	lw6map_free (sys_context, level);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing dup
 */
static void
_test_dup ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level1 = NULL;
    lw6map_level_t *level2 = NULL;
    char *repr1 = NULL;
    char *repr2 = NULL;

    level1 = lw6map_builtin_custom (sys_context, _TEST_MAP_WIDTH, _TEST_MAP_HEIGHT, _TEST_MAP_NB_LAYERS, _TEST_MAP_NOISE_PERCENT);
    if (LW6SYS_TEST_ACK (level1))
      {
	repr1 = lw6map_repr (sys_context, level1);
	if (LW6SYS_TEST_ACK (repr1))
	  {
	    level2 = lw6map_dup (sys_context, level1, NULL);
	    if (LW6SYS_TEST_ACK (level2))
	      {
		repr2 = lw6map_repr (sys_context, level2);
		if (LW6SYS_TEST_ACK (repr2))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("map \"%s\" copied to \"%s\""), repr1, repr2);
		    if (LW6SYS_TEST_ACK (lw6map_is_same (sys_context, level1, level2)))
		      {
			lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("map \"%s\" and \"%s\" are the same"), repr1, repr2);
		      }
		    else
		      {
			ret = 0;
		      }
		    LW6SYS_FREE (sys_context, repr2);
		  }
		else
		  {
		    ret = 0;
		  }
		lw6map_free (sys_context, level2);
	      }
	    else
	      {
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, repr1);
	  }
	else
	  {
	    ret = 0;
	  }
	lw6map_free (sys_context, level1);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing exp
 */
static void
_test_exp ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i = 0;
    lw6map_rules_t rules;

    lw6map_rules_defaults (sys_context, &rules);
    for (i = LW6MAP_RULES_MIN_EXP; i <= LW6MAP_RULES_MAX_EXP; ++i)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
		    _x_
		    ("exp=%d highest_color=%d highest_weapon=%d unlocked_team_color=%d unlocked_weapon=%d"),
		    i, lw6map_exp_get_highest_team_color_allowed (sys_context,
								  i),
		    lw6map_exp_get_highest_weapon_allowed (sys_context, i),
		    lw6map_exp_get_unlocked_team_color (sys_context, i), lw6map_exp_get_unlocked_weapon (sys_context, i));
      }
    for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("team_color=%d allowed=%d"), i, lw6map_exp_is_team_color_allowed (sys_context, &rules, i));
      }
    for (i = 0; i <= LW6MAP_MAX_WEAPON_ID; ++i)
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("weapon=%d allowed=%d"), i, lw6map_exp_is_weapon_allowed (sys_context, &rules, i));
      }
  }
  LW6SYS_TEST_FUNCTION_END;
}

/*
 * Testing hexa
 */
static void
_test_hexa ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level = NULL;
    lw6map_level_t *dup_level = NULL;
    char *repr = NULL;
    char *hexa = NULL;
    char *hexa_check = NULL;

    level = lw6map_builtin_custom (sys_context, _TEST_MAP_WIDTH, _TEST_MAP_HEIGHT, _TEST_MAP_NB_LAYERS, _TEST_MAP_NOISE_PERCENT);
    if (LW6SYS_TEST_ACK (level))
      {
	repr = lw6map_repr (sys_context, level);
	if (LW6SYS_TEST_ACK (repr))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("map \"%s\" constructed"), repr);
	    LW6SYS_FREE (sys_context, repr);
	  }
	else
	  {
	    ret = 0;
	  }
	hexa = lw6map_to_hexa (sys_context, level);
	lw6map_free (sys_context, level);
	level = NULL;
	if (LW6SYS_TEST_ACK (hexa))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("hexa is \"%s\""), hexa);

	    level = lw6map_from_hexa (sys_context, hexa);
	    if (LW6SYS_TEST_ACK (level))
	      {
		repr = lw6map_repr (sys_context, level);
		if (LW6SYS_TEST_ACK (repr))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("map \"%s\" re-constructed from hexa"), repr);
		    LW6SYS_FREE (sys_context, repr);
		  }
		else
		  {
		    ret = 0;
		  }
		/*
		 * Duplicate so that hexa dumps are calculated
		 * from really different objects, any id error
		 * will be tracked...
		 */
		dup_level = lw6map_dup (sys_context, level, NULL);
		if (LW6SYS_TEST_ACK (dup_level))
		  {
		    hexa_check = lw6map_to_hexa (sys_context, dup_level);
		    lw6map_free (sys_context, dup_level);
		    dup_level = NULL;
		    if (LW6SYS_TEST_ACK (hexa_check))
		      {
			if (LW6SYS_TEST_ACK (!strcmp (hexa, hexa_check)))
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("copy and original look the same"));
			  }
			else
			  {
			    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("copy and original are different"));
			    ret = 0;
			  }
			LW6SYS_FREE (sys_context, hexa_check);
			hexa_check = NULL;
		      }
		    else
		      {
			ret = 0;
		      }
		  }
		else
		  {
		    ret = 0;
		  }

		lw6map_free (sys_context, level);
		level = NULL;
	      }
	    else
	      {
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, hexa);
	    hexa = NULL;
	  }
	else
	  {
	    ret = 0;
	  }
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_param ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_param_t *param = NULL;
    lw6map_param_t param2;
    char *value = NULL;

    param = (lw6map_param_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6map_param_t));
    if (LW6SYS_TEST_ACK (param))
      {
	lw6map_param_zero (sys_context, param);
	lw6map_param_defaults (sys_context, param);

	lw6map_param_set (sys_context, param, _TEST_PARAM_KEY, _TEST_PARAM_VALUE);
	value = lw6map_param_get (sys_context, param, _TEST_PARAM_KEY);
	if (LW6SYS_TEST_ACK (value))
	  {
	    if (LW6SYS_TEST_ACK (lw6sys_str_is_same (sys_context, value, _TEST_PARAM_VALUE)))
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_NOTICE,
			    _x_ ("param value getter and setter seem to work value for \"%s\" is \"%s\""), _TEST_PARAM_KEY, value);
	      }
	    else
	      {
		lw6sys_log (sys_context, LW6SYS_LOG_WARNING,
			    _x_ ("bad param value returned for \"%s\", was \"%s\" should have been \"%s\""), _TEST_PARAM_KEY, value, _TEST_PARAM_VALUE);
		ret = 0;
	      }
	    LW6SYS_FREE (sys_context, value);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("no param value for \"%s\""), _TEST_PARAM_KEY);
	    ret = 0;
	  }
	lw6map_param_zero (sys_context, &param2);
	if (!LW6SYS_TEST_ACK (!lw6map_param_is_same (sys_context, param, &param2)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("param and param2 reported as same, they should be different"));
	    ret = 0;
	  }
	lw6map_param_copy (sys_context, &param2, param);
	if (!LW6SYS_TEST_ACK (lw6map_param_is_same (sys_context, param, &param2)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("param and param2 reported as different, they should be the same"));
	    ret = 0;
	  }

	lw6map_param_clear (sys_context, &param2);
	lw6map_param_clear (sys_context, param);
	LW6SYS_FREE (sys_context, param);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_rules ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_rules_t *rules;
    int value;
    unsigned int checksum = 0;

    rules = (lw6map_rules_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6map_rules_t));
    if (LW6SYS_TEST_ACK (rules))
      {
	lw6map_rules_zero (sys_context, rules);
	lw6map_rules_set_int (sys_context, rules, _TEST_RULES_KEY, _TEST_RULES_VALUE);
	value = lw6map_rules_get_int (sys_context, rules, _TEST_RULES_KEY);
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("map rules for key \"%s\" is %d"), _TEST_RULES_KEY, value);
	lw6map_rules_clear (sys_context, rules);

	lw6map_rules_defaults (sys_context, rules);
	checksum = 0;
	lw6map_rules_update_checksum (sys_context, rules, &checksum);

	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("rules checksum is %08x and should be %08x"), checksum, _TEST_RULES_CHECKSUM);
	ret = ret && (checksum == _TEST_RULES_CHECKSUM);
	if (LW6SYS_TEST_ACK (lw6map_rules_sanity_check (sys_context, rules)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("rules are valid, as sanity_check says"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("sanity_check failed"));
	    ret = 0;
	  }
	rules->total_time = LW6MAP_RULES_MAX_TOTAL_TIME + 1;
	rules->moves_per_round = LW6MAP_RULES_MIN_MOVES_PER_ROUND - 1;
	if (LW6SYS_TEST_ACK (!lw6map_rules_sanity_check (sys_context, rules)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("rules are not valid, as sanity_check says, this is fine"));
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("sanity_check return true, this is wrong"));
	    ret = 0;
	  }
	LW6SYS_FREE (sys_context, rules);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_style ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_style_t *style;
    char *value;

    style = (lw6map_style_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6map_style_t));
    if (LW6SYS_TEST_ACK (style))
      {
	lw6map_style_zero (sys_context, style);
	lw6map_style_set (sys_context, style, _TEST_STYLE_KEY, _TEST_STYLE_VALUE);
	value = lw6map_style_get (sys_context, style, _TEST_STYLE_KEY);
	if (LW6SYS_TEST_ACK (value))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("map style for key \"%s\" is \"%s\""), _TEST_STYLE_KEY, value);
	    LW6SYS_FREE (sys_context, value);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't find style key \"%s\""), _TEST_STYLE_KEY);
	    ret = 0;
	  }
	if (!LW6SYS_TEST_ACK (lw6map_style_is_same (sys_context, style, style)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("map style comparison failed"));
	    ret = 0;
	  }
	lw6map_style_clear (sys_context, style);
	LW6SYS_FREE (sys_context, style);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_teams ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_teams_t *teams;
    char *value;

    teams = (lw6map_teams_t *) LW6SYS_CALLOC (sys_context, sizeof (lw6map_teams_t));
    if (LW6SYS_TEST_ACK (teams))
      {
	lw6map_teams_zero (sys_context, teams);
	lw6map_teams_set (sys_context, teams, _TEST_TEAMS_KEY, _TEST_TEAMS_VALUE);
	value = lw6map_teams_get (sys_context, teams, _TEST_TEAMS_KEY);
	if (LW6SYS_TEST_ACK (value))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("map teams for key \"%s\" is \"%s\""), _TEST_TEAMS_KEY, value);
	    LW6SYS_FREE (sys_context, value);
	  }
	else
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("can't find teams key \"%s\""), _TEST_TEAMS_KEY);
	    ret = 0;
	  }
	if (!LW6SYS_TEST_ACK (lw6map_teams_is_same (sys_context, teams, teams)))
	  {
	    lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("map teams comparison failed"));
	    ret = 0;
	  }
	lw6map_teams_clear (sys_context, teams);
	LW6SYS_FREE (sys_context, teams);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_local_info ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_local_info_t local_info;

    memset (&local_info, 0, sizeof (lw6map_local_info_t));
    lw6map_local_info_set_music_dir (sys_context, &local_info, _TEST_MUSIC_DIR1);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("local info music dir is \"%s\""), local_info.music_dir);
    lw6map_local_info_set_music_dir (sys_context, &local_info, _TEST_MUSIC_DIR2);
    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("local info music dir is \"%s\""), local_info.music_dir);
    lw6map_local_info_clear (sys_context, &local_info);
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_meta_layer ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_meta_layer_t meta_layer;

    memset (&meta_layer, 0, sizeof (lw6map_meta_layer_t));
    lw6map_meta_layer_clear (sys_context, &meta_layer);
    if (LW6SYS_TEST_ACK (lw6map_meta_layer_builtin_custom (sys_context, &meta_layer, _TEST_MAP_WIDTH, _TEST_MAP_WIDTH, 0, _TEST_MAP_NOISE_PERCENT, 0)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("custom boolean meta-layer initialized"));
	_print_meta_layer (sys_context, &meta_layer, "boolean");
      }
    else
      {
	ret = 0;
      }
    if (LW6SYS_TEST_ACK (lw6map_meta_layer_builtin_custom (sys_context, &meta_layer, _TEST_MAP_WIDTH, _TEST_MAP_WIDTH, 1, _TEST_MAP_NOISE_PERCENT, 0)))
      {
	lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("custom boolean meta-layer initialized"));
	_print_meta_layer (sys_context, &meta_layer, "analog");
      }
    else
      {
	ret = 0;
      }
    lw6map_meta_layer_clear (sys_context, &meta_layer);
  }

  LW6SYS_TEST_FUNCTION_END;
}

static void
_test_weapon ()
{
  int ret = 1;
  lw6sys_context_t *sys_context = NULL;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i, j;
    char *key;
    char *label;

    for (i = 0; i <= LW6MAP_MAX_WEAPON_ID; ++i)
      {
	key = lw6map_weapon_index_to_key (sys_context, i);
	if (LW6SYS_TEST_ACK (key))
	  {
	    label = lw6map_weapon_index_to_label (sys_context, i);
	    if (LW6SYS_TEST_ACK (label))
	      {
		j = lw6map_weapon_key_to_index (sys_context, key);
		if (LW6SYS_TEST_ACK (i == j))
		  {
		    lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("weapon key for %d is \"%s\" label is \"%s\""), i, key, label);
		  }
		else
		  {
		    ret = 0;
		  }
	      }
	    else
	      {
		ret = 0;
	      }
	  }
	else
	  {
	    ret = 0;
	  }
      }
  }

  LW6SYS_TEST_FUNCTION_END;
}

static int
_setup_init ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("init libmap CUnit test suite"));

  return CUE_SUCCESS;
}

static int
_setup_quit ()
{
  lw6sys_context_t *sys_context = _test_data.sys_context;

  lw6sys_log (sys_context, LW6SYS_LOG_NOTICE, _x_ ("quit libmap CUnit test suite"));

  return CUE_SUCCESS;
}

/**
 * lw6map_test_register
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Registers all tests for the libmap module.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6map_test_register (lw6sys_context_t * sys_context, int mode)
{
  int ret = 1;
  CU_Suite *suite = NULL;

  _test_data.sys_context = sys_context;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test_register (sys_context, mode);
    }

  suite = CU_add_suite ("lw6map", _setup_init, _setup_quit);
  if (suite)
    {
      LW6SYS_CUNIT_ADD_TEST (suite, _test_new);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_color);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_coords);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_builtin);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_dup);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_exp);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_hexa);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_local_info);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_meta_layer);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_param);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_rules);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_style);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_teams);
      LW6SYS_CUNIT_ADD_TEST (suite, _test_weapon);
    }
  else
    {
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _x_ ("unable to add CUnit test suite, error msg is \"%s\""), CU_get_error_msg ());
      ret = 0;
    }

  return ret;
}

/**
 * lw6map_test_run
 *
 * @sys_context: global system context
 * @mode: test mode (bitmask)
 *
 * Runs the @map module test suite, testing most (if not all...)
 * functions.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6map_test_run (lw6sys_context_t * sys_context, int mode)
{
  int ret = 0;

  _test_data.ret = 1;
  _test_data.sys_context = sys_context;

  if (lw6sys_cunit_run_tests (sys_context, mode))
    {
      ret = _test_data.ret;
    }

  return ret;
}
