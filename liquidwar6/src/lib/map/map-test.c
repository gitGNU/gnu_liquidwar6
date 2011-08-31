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

#include <string.h>

#include "map.h"

#define TEST_MAP_NB_LAYERS 2
#define TEST_MAP_WIDTH 8
#define TEST_MAP_HEIGHT 6
#define TEST_MAP_NOISE_PERCENT 30
#define TEST_RULES_KEY LW6DEF_TOTAL_TIME
#define TEST_RULES_VALUE 3600
#define TEST_RULES_CHECKSUM 0x0f97e577
#define TEST_STYLE_KEY LW6DEF_ZOOM
#define TEST_STYLE_VALUE "1.0"
#define TEST_TEAMS_KEY LW6DEF_BOT_IQ
#define TEST_TEAMS_VALUE "99"
#define TEST_COORDS_NB 5
#define TEST_COORDS_W 100
#define TEST_COORDS_H 50
#define TEST_COORDS_D 1
#define TEST_COORDS_X1 66
#define TEST_COORDS_Y1 33
#define TEST_COORDS_X2 130
#define TEST_COORDS_Y2 20
#define TEST_COORDS_X3 -30
#define TEST_COORDS_Y3 20
#define TEST_COORDS_X4 -10
#define TEST_COORDS_Y4 -5
#define TEST_COORDS_X5 1000
#define TEST_COORDS_Y5 51
#define _TEST_MUSIC_DIR1 "../"
#define _TEST_MUSIC_DIR2 "/foo/bar"

/*
 * Testing basic new & free functions
 */
static int
test_new ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level = NULL;
    char *repr = NULL;

    level = lw6map_new ();
    if (level)
      {
	repr = lw6map_repr (level);
	if (repr)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("empty map \"%s\" constructed"), repr);
	    LW6SYS_FREE (repr);
	  }
	else
	  {
	    ret = 0;
	  }
	lw6map_free (level);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_color ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_color_couple_t color_couple;
    int i, j;
    char *key;

    color_couple.bg = LW6SYS_COLOR_8_WHITE;
    color_couple.fg = LW6SYS_COLOR_8_BLACK;

    lw6map_color_invert (&color_couple);
    if (!lw6map_color_is_same (&color_couple, &color_couple))
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("color_is_same pretending colors are different, while they should be equal"));
	ret = 0;
      }

    for (i = 0; i < LW6MAP_MAX_NB_TEAMS; ++i)
      {
	key = lw6map_team_color_index_to_key (i);
	if (key)
	  {
	    j = lw6map_team_color_key_to_index (key);
	    if (i == j)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("team_color key for %d is \"%s\""), i, key);
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
  return ret;
}

static int
test_coords ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_rules_t rules;
    lw6sys_whd_t shape;
    int test_x[TEST_COORDS_NB] =
      { TEST_COORDS_X1, TEST_COORDS_X2, TEST_COORDS_X3, TEST_COORDS_X4,
      TEST_COORDS_X5
    };
    int test_y[TEST_COORDS_NB] =
      { TEST_COORDS_Y1, TEST_COORDS_Y2, TEST_COORDS_Y3, TEST_COORDS_Y4,
      TEST_COORDS_Y5
    };
    int i, x, y, px, py;

    lw6map_rules_defaults (&rules);
    shape.w = TEST_COORDS_W;
    shape.h = TEST_COORDS_H;
    shape.d = TEST_COORDS_D;

    for (py = 1; py >= -1; --py)
      {
	for (px = 1; px >= -1; --px)
	  {
	    rules.x_polarity = px;
	    rules.y_polarity = py;
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("polarity set to %d,%d"), px,
			py);
	    for (i = 0; i < TEST_COORDS_NB; ++i)
	      {
		x = test_x[i];
		y = test_y[i];
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("coords before fix %d,%d"), x, y);
		lw6map_coords_fix_xy (&rules, &shape, &x, &y);
		lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("coords after fix %d,%d"),
			    x, y);
	      }
	  }
      }

    ret = 1;
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static void
_print_meta_layer (lw6map_meta_layer_t * meta_layer, char *label)
{
  int x, y, w, h;
  int value;

  w = meta_layer->shape.w;
  h = meta_layer->shape.h;

  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("meta-layer \"%s\""), label);
  for (y = 0; y < h; ++y)
    {
      for (x = 0; x < w; ++x)
	{
	  value = lw6map_meta_layer_get (meta_layer, x, y);
	  printf ("%03d ", value);
	}
      printf ("\n");
    }
  fflush (stdout);
}

static void
_print_body (lw6map_body_t * body)
{
  int x, y, z, w, h, d;

  w = body->shape.w;
  h = body->shape.h;
  d = body->shape.d;

  for (z = 0; z < d; ++z)
    {
      lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("layer%d (z=%d)"), z + 1, z);
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

static void
_print_cursor_texture (lw6map_cursor_texture_t * cursor_texture)
{
  int x, y;
  lw6sys_color_8_t fg_bg;
  lw6sys_color_8_t color;

  lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("cursor_texture:"));
  for (y = 0; y < LW6MAP_CURSOR_TEXTURE_SIZE; ++y)
    {
      for (x = 0; x < LW6MAP_CURSOR_TEXTURE_SIZE; ++x)
	{
	  fg_bg =
	    lw6map_cursor_texture_layer_get (&(cursor_texture->fg_bg_layer),
					     x, y);
	  color =
	    lw6map_cursor_texture_layer_get (&(cursor_texture->color_layer),
					     x, y);
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

/*
 * Testing defaults
 */
static int
test_defaults ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level = NULL;
    char *repr = NULL;
    int has_alpha = 0;

    level = lw6map_builtin_defaults ();
    if (level)
      {
	repr = lw6map_repr (level);
	if (repr)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("default map \"%s\" constructed"), repr);
	    has_alpha = lw6map_texture_has_alpha (&(level->texture));
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("has_alpha=%d"), has_alpha);
	    _print_body (&level->body);
	    _print_cursor_texture (&level->cursor_texture);
	    LW6SYS_FREE (repr);
	  }
	else
	  {
	    ret = 0;
	  }
	lw6map_free (level);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing dup
 */
static int
test_dup ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level1 = NULL;
    lw6map_level_t *level2 = NULL;
    char *repr1 = NULL;
    char *repr2 = NULL;

    level1 =
      lw6map_builtin_custom (TEST_MAP_WIDTH, TEST_MAP_HEIGHT,
			     TEST_MAP_NB_LAYERS, TEST_MAP_NOISE_PERCENT);
    if (level1)
      {
	repr1 = lw6map_repr (level1);
	if (repr1)
	  {
	    level2 = lw6map_dup (level1, NULL);
	    if (level2)
	      {
		repr2 = lw6map_repr (level2);
		if (repr2)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_ ("map \"%s\" copied to \"%s\""), repr1,
				repr2);
		    if (lw6map_is_same (level1, level2))
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_
				    ("map \"%s\" and \"%s\" are the same"),
				    repr1, repr2);
		      }
		    else
		      {
			ret = 0;
		      }
		    LW6SYS_FREE (repr2);
		  }
		else
		  {
		    ret = 0;
		  }
		lw6map_free (level2);
	      }
	    else
	      {
		ret = 0;
	      }
	    LW6SYS_FREE (repr1);
	  }
	else
	  {
	    ret = 0;
	  }
	lw6map_free (level1);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing exp
 */
static int
test_exp ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i = 0;

    for (i = LW6MAP_RULES_MIN_EXP; i <= LW6MAP_RULES_MAX_EXP; ++i)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _("exp=%d highest_color=%d highest_weapon=%d"), i,
		    lw6map_exp_get_highest_team_color_allowed (i),
		    lw6map_exp_get_highest_weapon_allowed (i));
      }
  }
  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

/*
 * Testing hexa
 */
static int
test_hexa ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level = NULL;
    char *repr = NULL;
    char *hexa = NULL;
    char *hexa_check = NULL;

    level =
      lw6map_builtin_custom (TEST_MAP_WIDTH, TEST_MAP_HEIGHT,
			     TEST_MAP_NB_LAYERS, TEST_MAP_NOISE_PERCENT);
    if (level)
      {
	repr = lw6map_repr (level);
	if (repr)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("map \"%s\" constructed"),
			repr);
	    LW6SYS_FREE (repr);
	  }
	else
	  {
	    ret = 0;
	  }
	hexa = lw6map_to_hexa (level);
	lw6map_free (level);
	if (hexa)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("hexa is \"%s\""), hexa);
	    level = lw6map_from_hexa (hexa);
	    if (level)
	      {
		repr = lw6map_repr (level);
		if (repr)
		  {
		    lw6sys_log (LW6SYS_LOG_NOTICE,
				_x_ ("map \"%s\" re-constructed from hexa"),
				repr);
		    LW6SYS_FREE (repr);
		  }
		else
		  {
		    ret = 0;
		  }

		hexa_check = lw6map_to_hexa (level);
		if (hexa_check)
		  {
		    if (!strcmp (hexa, hexa_check))
		      {
			lw6sys_log (LW6SYS_LOG_NOTICE,
				    _x_ ("copy and original look the same"));
		      }
		    else
		      {
			lw6sys_log (LW6SYS_LOG_WARNING,
				    _x_ ("copy and original are different"));
			ret = 0;
		      }
		    LW6SYS_FREE (hexa_check);
		  }
		else
		  {
		    ret = 0;
		  }
		lw6map_free (level);
	      }
	    else
	      {
		ret = 0;
	      }
	    LW6SYS_FREE (hexa);
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
  return ret;
}

static int
test_rules ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_rules_t *rules;
    int value;
    unsigned int checksum = 0;

    rules = (lw6map_rules_t *) LW6SYS_CALLOC (sizeof (lw6map_rules_t));
    if (rules)
      {
	lw6map_rules_set_int (rules, TEST_RULES_KEY, TEST_RULES_VALUE);
	value = lw6map_rules_get_int (rules, TEST_RULES_KEY);
	lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("map rules for key \"%s\" is %d"),
		    TEST_RULES_KEY, value);
	lw6map_rules_clear (rules);

	lw6map_rules_defaults (rules);
	checksum = 0;
	lw6map_rules_update_checksum (rules, &checksum);

	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_
		    ("rules checksum is %08x and should be %08x"),
		    checksum, TEST_RULES_CHECKSUM);
	ret = ret && (checksum == TEST_RULES_CHECKSUM);
	LW6SYS_FREE (rules);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_style ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_style_t *style;
    char *value;

    style = (lw6map_style_t *) LW6SYS_CALLOC (sizeof (lw6map_style_t));
    if (style)
      {
	lw6map_style_set (style, TEST_STYLE_KEY, TEST_STYLE_VALUE);
	value = lw6map_style_get (style, TEST_STYLE_KEY);
	if (value)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("map style for key \"%s\" is \"%s\""),
			TEST_STYLE_KEY, value);
	    LW6SYS_FREE (value);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("can't find style key \"%s\""), TEST_STYLE_KEY);
	    ret = 0;
	  }
	if (!lw6map_style_is_same (style, style))
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("map style comparison failed"));
	    ret = 0;
	  }
	lw6map_style_clear (style);
	LW6SYS_FREE (style);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_teams ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_teams_t *teams;
    char *value;

    teams = (lw6map_teams_t *) LW6SYS_CALLOC (sizeof (lw6map_teams_t));
    if (teams)
      {
	lw6map_teams_set (teams, TEST_TEAMS_KEY, TEST_TEAMS_VALUE);
	value = lw6map_teams_get (teams, TEST_TEAMS_KEY);
	if (value)
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE,
			_x_ ("map teams for key \"%s\" is \"%s\""),
			TEST_TEAMS_KEY, value);
	    LW6SYS_FREE (value);
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("can't find teams key \"%s\""), TEST_TEAMS_KEY);
	    ret = 0;
	  }
	if (!lw6map_teams_is_same (teams, teams))
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_ ("map teams comparison failed"));
	    ret = 0;
	  }
	lw6map_teams_clear (teams);
	LW6SYS_FREE (teams);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_local_info ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_local_info_t local_info;

    memset (&local_info, 0, sizeof (lw6map_local_info_t));
    lw6map_local_info_set_music_dir (&local_info, _TEST_MUSIC_DIR1);
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("local info music dir is \"%s\""),
		local_info.music_dir);
    lw6map_local_info_set_music_dir (&local_info, _TEST_MUSIC_DIR2);
    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("local info music dir is \"%s\""),
		local_info.music_dir);
    lw6map_local_info_clear (&local_info);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_meta_layer ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_meta_layer_t meta_layer;

    memset (&meta_layer, 0, sizeof (lw6map_meta_layer_t));
    lw6map_meta_layer_clear (&meta_layer);
    if (lw6map_meta_layer_builtin_custom
	(&meta_layer, TEST_MAP_WIDTH, TEST_MAP_WIDTH, 0,
	 TEST_MAP_NOISE_PERCENT, 0))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("custom boolean meta-layer initialized"));
	_print_meta_layer (&meta_layer, "boolean");
      }
    else
      {
	ret = 0;
      }
    if (lw6map_meta_layer_builtin_custom
	(&meta_layer, TEST_MAP_WIDTH, TEST_MAP_WIDTH, 1,
	 TEST_MAP_NOISE_PERCENT, 0))
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("custom boolean meta-layer initialized"));
	_print_meta_layer (&meta_layer, "analog");
      }
    else
      {
	ret = 0;
      }
    lw6map_meta_layer_clear (&meta_layer);
  }

  LW6SYS_TEST_FUNCTION_END;
  return ret;
}

static int
test_weapon ()
{
  int ret = 1;
  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    int i, j;
    char *key;

    for (i = 0; i <= LW6MAP_MAX_WEAPON_ID; ++i)
      {
	key = lw6map_weapon_index_to_key (i);
	if (key)
	  {
	    j = lw6map_weapon_key_to_index (key);
	    if (i == j)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("weapon key for %d is \"%s\""), i, key);
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
  return ret;
}

/**
 * lw6map_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @map module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6map_test (int mode)
{
  int ret = 0;

  if (lw6sys_false ())
    {
      /*
       * Just to make sure most functions are stuffed in the binary
       */
      lw6sys_test (mode);
      lw6map_test (mode);
    }

  ret = test_new () && test_color () && test_coords ()
    && test_defaults () && test_dup () && test_exp ()
    && test_hexa () && test_local_info () && test_meta_layer ()
    && test_rules () && test_style () && test_teams () && test_weapon ();

  return ret;
}
