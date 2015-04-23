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
#endif // HAVE_CONFIG_H

#include "sys.h"
#include "sys-internal.h"

#ifdef LW6_X86
/**
 * lw6sys_test_and_set
 *
 * @test_and_set: pointer to the value used to test and set
 *
 * Low level function which performs an atomic exchange to
 * implement a spinlock. This one is just a wrapper to help
 * debugging asm calls.
 *
 * Return value: 1 when lock is acquired.
 */
int32_t
lw6sys_test_and_set (volatile int32_t * test_and_set)
{
  if (test_and_set && (*test_and_set == 0 || *test_and_set == 1))
    {
      return _lw6sys_test_and_set_asm (test_and_set);
    }

  // theorically unreachable code
  return 0;
}
#endif // LW6_X86

#ifdef LW6_AMD64
/**
 * lw6sys_test_and_set
 *
 * @test_and_set: pointer to the value used to test and set
 *
 * Low level function which performs an atomic exchange to
 * implement a spinlock. This one is just a wrapper to help
 * debugging asm calls.
 *
 * Return value: 1 when lock is acquired.
 */
int64_t
lw6sys_test_and_set (volatile int64_t * test_and_set)
{
  if (test_and_set && (*test_and_set == 0 || *test_and_set == 1))
    {
      return _lw6sys_test_and_set_asm (test_and_set);
    }

  // theorically unreachable code
  return 0;
}
#endif // LW6_AMD64
