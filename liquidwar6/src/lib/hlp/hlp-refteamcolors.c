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

int
_lw6hlp_populate_references_team_colors (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 0;

  // note: team colors need to be localized
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_RED, _("moves faster than the average"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_GREEN, _("has a better defense than the average"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_BLUE, _("has a strong attack but is slow"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_YELLOW, _("has a strong attack"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_CYAN, _("has an extremely good defense but is slow"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_MAGENTA, _("is extremely fast but also very vulnerable"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_ORANGE, _("is fast, but has a very weak attack"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_LIGHTBLUE, _("has an extremely strong attack but is very slow"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_PURPLE, _("has a very good defense but a weak attack"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_PINK, _("has a very strong attack, but is also very vulnerable"));

  ret = 1;

  return ret;
}
