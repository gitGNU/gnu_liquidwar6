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

#include "scm.h"

#define _SCM_PREFIX "c-"
#define _SCM_PREFIX_LEN 2
#define _SCM_SEP '-'

#define _C_PREFIX "_scm_"
#define _C_PREFIX_LEN 5
#define _C_SEP '_'

/**
 * lw6scm_funcname_scm2c
 *
 * @sys_context: global system context
 * @funcname: function name to change
 *
 * Transforms a function name of the form c-my-func to
 * _scm_my_func.
 *
 * Return value: new allocated string.
 */
char *
lw6scm_funcname_scm2c (lw6sys_context_t * sys_context, const char *funcname)
{
  char *ret = NULL;
  char *ptr = NULL;

  if (strlen (funcname) > _SCM_PREFIX_LEN)
    {
      ret = lw6sys_new_sprintf (sys_context, "%s%s", _C_PREFIX, funcname + _SCM_PREFIX_LEN);
      if (ret)
	{
	  for (ptr = ret; *ptr; ++ptr)
	    {
	      if ((*ptr) == _SCM_SEP)
		{
		  (*ptr) = _C_SEP;
		}
	    }
	}
    }

  return ret;
}

/**
 * lw6scm_funcname_scm2c
 *
 * @sys_context: global system context
 * @funcname: function name to change
 *
 * Transforms a function name of the form _scm_my_func to
 * c-my-func.
 *
 * Return value: new allocated string.
 */
char *
lw6scm_funcname_c2scm (lw6sys_context_t * sys_context, const char *funcname)
{
  char *ret = NULL;
  char *ptr = NULL;

  if (strlen (funcname) > _C_PREFIX_LEN)
    {
      ret = lw6sys_new_sprintf (sys_context, "%s%s", _SCM_PREFIX, funcname + _C_PREFIX_LEN);
      if (ret)
	{
	  for (ptr = ret; *ptr; ++ptr)
	    {
	      if ((*ptr) == _C_SEP)
		{
		  (*ptr) = _SCM_SEP;
		}
	    }
	}
    }

  return ret;
}
