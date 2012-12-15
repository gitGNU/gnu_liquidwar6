/*
  Liquid War 6 is a unique multiplayer wargame.
  Copyright (C)  2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012  Christian Mauduit <ufoot@ufoot.org>

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

#include "gen-internal.h"

#define _TEST_SEED "abcdef0123456789"
#define _TEST_WIDTH 320
#define _TEST_HEIGHT 180

#define _TEST_SEED_SHORT_SRC "abc{}"
#define _TEST_SEED_SHORT_DST "abcfhabcfhabcfha"
#define _TEST_SEED_LONG_SRC "FOO' BAR' 1234567890 you'll never read this!"
#define _TEST_SEED_LONG_DST "foo3wbar3w123456"

/*
 * test create
 */
static int
test_create ()
{
  int ret = 0;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    lw6map_level_t *level = NULL;
    char *repr = NULL;
    lw6sys_log (LW6SYS_LOG_NOTICE,
		_x_ ("creating map from seed \"%s\" with size %dx%d"),
		_TEST_SEED, _TEST_WIDTH, _TEST_HEIGHT);
    level = lw6gen_create_from_seed (_TEST_SEED, _TEST_WIDTH, _TEST_HEIGHT);
    if (level)
      {
	repr = lw6map_repr (level);
	if (repr)
	  {
	    if (level->body.shape.w == _TEST_WIDTH
		&& level->body.shape.h == _TEST_HEIGHT)
	      {
		lw6sys_log (LW6SYS_LOG_NOTICE,
			    _x_ ("built map \"%s\" from seed \"%s\""), repr,
			    _TEST_SEED);
		LW6SYS_FREE (repr);

		ret = 1;
	      }
	    else
	      {
		lw6sys_log (LW6SYS_LOG_WARNING,
			    _x_
			    ("map created from seed \"%s\" with bad size %dx%d, should have been %dx%d"),
			    _TEST_SEED, level->body.shape.w,
			    level->body.shape.h, _TEST_WIDTH, _TEST_HEIGHT);
	      }
	  }
	lw6map_free (level);
      }
    else
      {
	lw6sys_log (LW6SYS_LOG_WARNING,
		    _x_
		    ("unable to create map from seed \"%s\" with size %dx%d"),
		    _TEST_SEED, _TEST_WIDTH, _TEST_HEIGHT);
      }
  }

  LW6SYS_TEST_FUNCTION_END;

  return ret;
}

/*
 * test seed
 */
static int
test_seed ()
{
  int ret = 1;

  LW6SYS_TEST_FUNCTION_BEGIN;

  {
    char *seed = NULL;

    seed = lw6gen_seed_new ();
    if (seed)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("generated new random seed \"%s\""), seed);
	LW6SYS_FREE (seed);
      }
    else
      {
	ret = 0;
      }

    seed = lw6gen_seed_normalize (_TEST_SEED_SHORT_SRC);
    if (seed)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("normalizing short seed \"%s\" -> \"%s\""),
		    _TEST_SEED_SHORT_SRC, seed);
	if (lw6sys_str_is_same (seed, _TEST_SEED_SHORT_DST))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("ouput is correct"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_
			("bad ouput, should have been \"%s\" but is \"%s\""),
			_TEST_SEED_SHORT_DST, seed);
	    ret = 0;
	  }
	LW6SYS_FREE (seed);
      }
    else
      {
	ret = 0;
      }

    seed = lw6gen_seed_normalize (_TEST_SEED_LONG_SRC);
    if (seed)
      {
	lw6sys_log (LW6SYS_LOG_NOTICE,
		    _x_ ("normalizing long seed \"%s\" -> \"%s\""),
		    _TEST_SEED_LONG_SRC, seed);
	if (lw6sys_str_is_same (seed, _TEST_SEED_LONG_DST))
	  {
	    lw6sys_log (LW6SYS_LOG_NOTICE, _x_ ("ouput is correct"));
	  }
	else
	  {
	    lw6sys_log (LW6SYS_LOG_WARNING,
			_x_
			("bad ouput, should have been \"%s\" but is \"%s\""),
			_TEST_SEED_LONG_DST, seed);
	    ret = 0;
	  }
	LW6SYS_FREE (seed);
      }
    else
      {
	ret = 0;
      }
  }

  LW6SYS_TEST_FUNCTION_END;

  return ret;
}

/**
 * lw6gen_test
 *
 * @mode: 0 for check only, 1 for full test
 *
 * Runs the @gen module test suite.
 *
 * Return value: 1 if test is successfull, 0 on error.
 */
int
lw6gen_test (int mode)
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

  ret = test_create () && test_seed ();

  return ret;
}
