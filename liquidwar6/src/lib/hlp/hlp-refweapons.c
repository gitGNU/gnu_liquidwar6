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
_lw6hlp_populate_references_weapons (lw6sys_context_t * sys_context, lw6sys_hash_t * references)
{
  int ret = 0;

  // note: weapons need to be localized
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_BERZERK, _("super-strong attack for a limited time, crush your enemies"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_INVINCIBLE, _("no damage for a limited time, move untouched"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_ESCAPE, _("fighters placed as far as possible from cursor, magically escape from any grip"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_TURBO, _("move faster for a limited time"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_TELEPORT, _("fighters placed as close as possible to cursor"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SCATTER, _("every fighters of every team scattered in random places"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_FIX, _("all other teams are freezed, you can move but not attack them"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_MIX, _("fighters exchange position, their properties being preserved"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_CONTROL, _("you take the control of all other teams while your cursor stays in place"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_PERMUTATION, _("will exchange colors, randomly, requires at least 3 teams (double edged weapon)"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_STEAL, _("steals some fighters to other teams"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_ATOMIC, _("nuclear explosion, all fighters arround your cursor are about to die"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_REVERSE, _("fighters continue to move normally, but attacks are done in reverse mode, backwards"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_CRAZY, _("all your opponents go crazy for some time, acting with no logic"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_REWIND, _("make the battlefield be like it was a few seconds ago"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_ATTRACT, _("all fighters from all teams are packed near your cursor"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_SHRINK, _("reduces the number of fighters on the map"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_KAMIKAZE, _("you die along with the strongest team on the battlefield, requires at least 3 teams"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_DISAPPEAR, _("you disappear for some time from the battlefield, to reappear later, somewhere else"));
  _LW6HLP_POPULATE_VOID (sys_context, references, LW6DEF_PLAGUE, _("general disease, all fighters mysteriously loose health"));

  ret = 1;

  return ret;
}
