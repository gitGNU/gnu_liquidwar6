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

#include <ctype.h>

#include "gen.h"
#include "gen-internal.h"

/**
 * lw6gen_seed_new
 *
 * @sys_context: global system context
 *
 * Generate a new random seed. The seed is composed of letters and numbers.
 *
 * Return value: newly allocated string.
 */
char *
lw6gen_seed_new (lw6sys_context_t * sys_context)
{
  char *ret = NULL;
  int i = 0;

  ret = LW6SYS_CALLOC (sys_context, LW6GEN_SEED_LENGTH + 1);
  if (ret)
    {
      for (i = 0; i < LW6GEN_SEED_LENGTH; ++i)
	{
	  ret[i] = lw6gen_seed_char (sys_context);
	}
    }

  return ret;
}

/**
 * lw6gen_seed_normalize
 *
 * @sys_context: global system context
 * @seed: the seed to normalize
 *
 * Builds a normalized seed from an arbitrary string. The idea is to
 * avoid sending strange stuff on the network and/or storing strange
 * stuff in config files, so we process correct strings only
 *
 * Return value: newly allocated string.
 */
char *
lw6gen_seed_normalize (lw6sys_context_t * sys_context, const char *seed)
{
  char *ret = NULL;
  int i = 0;
  unsigned char c = ' ';
  int len_chars = 0;
  int len_seed = 0;

  ret = LW6SYS_CALLOC (sys_context, LW6GEN_SEED_LENGTH + 1);
  if (ret)
    {
      len_chars = strlen (LW6GEN_SEED_CHARS);
      len_seed = strlen (seed);
      for (i = 0; i < LW6GEN_SEED_LENGTH; ++i)
	{
	  c = tolower (seed[i % len_seed]);
	  if (strchr (LW6GEN_SEED_CHARS, c))
	    {
	      ret[i] = c;
	    }
	  else
	    {
	      ret[i] = LW6GEN_SEED_CHARS[c % len_chars];
	    }
	}
    }

  return ret;
}

/**
 * lw6gen_seed_char
 *
 * @sys_context: global system context
 *
 * Returns a random char suitable for seed (letter or digit).
 *
 * Return value: a single char
 */
char
lw6gen_seed_char (lw6sys_context_t * sys_context)
{
  char ret = ' ';
  int len = 0;

  /*
   * OK, it's not efficient to run strlen each time we keep up
   * a char, but this code is not used very often is not
   * a real issue and it avoids messing up with a hardcoded
   * array size and simplifies code in lw6gen_seed_new.
   */
  len = strlen (LW6GEN_SEED_CHARS);
  ret = LW6GEN_SEED_CHARS[lw6sys_random (sys_context, len)];

  return ret;
}
