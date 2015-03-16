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

#include <string.h>

#include "hlp.h"
#include "hlp-internal.h"

#define _CREDITS_IDEA "Thomas Colcombet"
#define _CREDITS_CODING "Christian Mauduit"
#define _CREDITS_ARTWORK "Kasper Hviid"
#define _CREDITS_MUSIC_MENU "Tim Chadburn"
#define _CREDITS_EXTRA_MAPS1 "Joey Dodson"
#define _CREDITS_MUSIC1_TITLE "Free the sounds"
#define _CREDITS_MUSIC1_AUTHOR "Robert Radamant"
#define _CREDITS_MUSIC2_TITLE "Heav'hypnosis"
#define _CREDITS_MUSIC2_AUTHOR "LapSuS"
#define _CREDITS_MUSIC3_TITLE "Oriental Travel"
#define _CREDITS_MUSIC3_AUTHOR "Nighter313"
#define _CREDITS_NN_TRANSLATOR "Karl Ove Hufthammer"
#define _CREDITS_RU_TRANSLATOR "Yevgeny Lezhnin"
#define _CREDITS_LIBCACA "PFA Epitech 2012-2013 (france_a, clavel_r, lemonn_k, vougie_c)"
#define _CREDITS_HANDICAP "PFA Epitech 2012-2013 (Timothee Menage)"
#define _LIST_EMAIL "help-liquidwar6@gnu.org"
#define _HELP_GNU_URL "http://www.gnu.org/help/"
#define _FFII_URL "http://www.ffii.org/"

/**
 * lw6hlp_get_credits
 *
 * @sys_context: global system context
 * @id: the id of the credits line to return
 *
 * Returns a "credit line", that is a short sentence, about 30 to 50 chars,
 * saying who developped the game, created graphics, giving important URLs,
 * and so on. One can pass an arbitraty high @id, no risk, it will just loop
 * on previous lines.
 *
 * Return value: the string, must be freed.
 */
char *
lw6hlp_get_credits (lw6sys_context_t * sys_context, int id)
{
  char *ret = NULL;

  if (id < 0)
    {
      id = -id;
    }
  id = id % LW6HLP_NB_CREDITS_ENTRIES;

  switch (id)
    {
    case 0:
      ret = lw6sys_str_copy (sys_context, lw6sys_build_get_package_string ());
      break;
    case 1:
      ret = lw6sys_new_sprintf (sys_context, _("Codename \"%s\""), lw6sys_build_get_codename ());
      break;
    case 2:
      ret = lw6sys_str_copy (sys_context, lw6sys_build_get_home_url ());
      break;
    case 3:
      ret = lw6sys_new_sprintf (sys_context, _("Idea: %s"), _CREDITS_IDEA);
      break;
    case 4:
      ret = lw6sys_new_sprintf (sys_context, _("Programming: %s"), _CREDITS_CODING);
      break;
    case 5:
      ret = lw6sys_new_sprintf (sys_context, _("Artwork & design: %s"), _CREDITS_ARTWORK);
      break;
    case 6:
      ret = lw6sys_new_sprintf (sys_context, _("Menu music: %s"), _CREDITS_MUSIC_MENU);
      break;
    case 7:
      ret = lw6sys_new_sprintf (sys_context, _("Thanks to all Liquid War 5 contributors"));
      break;
    case 8:
      ret = lw6sys_new_sprintf (sys_context, _("Extra maps: %s"), _CREDITS_EXTRA_MAPS1);
      break;
    case 9:
      ret = lw6sys_new_sprintf (sys_context, _("Song \"%s\" by %s"), _CREDITS_MUSIC1_TITLE, _CREDITS_MUSIC1_AUTHOR);
      break;
    case 10:
      ret = lw6sys_new_sprintf (sys_context, _("Song \"%s\" by %s"), _CREDITS_MUSIC2_TITLE, _CREDITS_MUSIC2_AUTHOR);
      break;
    case 11:
      ret = lw6sys_new_sprintf (sys_context, _("Song \"%s\" by %s"), _CREDITS_MUSIC3_TITLE, _CREDITS_MUSIC3_AUTHOR);
      break;
    case 12:
      ret = lw6sys_new_sprintf (sys_context, _("Norwegian translation by %s"), _CREDITS_NN_TRANSLATOR);
      break;
    case 13:
      ret = lw6sys_new_sprintf (sys_context, _("Russian translation by %s"), _CREDITS_RU_TRANSLATOR);
      break;
    case 14:
      ret = lw6sys_new_sprintf (sys_context, _("Libcaca port by %s"), _CREDITS_LIBCACA);
      break;
    case 15:
      ret = lw6sys_new_sprintf (sys_context, _("Handicap patch by %s"), _CREDITS_HANDICAP);
      break;
    case 16:
      ret = lw6sys_str_copy (sys_context, _("Thanks to all other contributors"));
      break;
    case 17:
      ret = lw6sys_str_copy (sys_context, _("Help is always appreciated"));
      break;
    case 18:
      ret = lw6sys_new_sprintf (sys_context, _("Mailing list: <%s>"), _LIST_EMAIL);
      break;
    case 19:
      ret = lw6sys_str_copy (sys_context, _("Free as in \"free speech\" (not beer)"));
      break;
    case 20:
      ret = lw6sys_new_sprintf (sys_context, _("License: %s"), lw6sys_build_get_license ());
      break;
    case 21:
      ret = lw6sys_str_copy (sys_context, lw6sys_build_get_copyright ());
      break;
    case 22:
      ret = lw6sys_new_sprintf (sys_context, _("Built on %s at %s"), lw6sys_build_get_date (), lw6sys_build_get_time ());
      break;
    case 23:
      ret = lw6sys_new_sprintf (sys_context, _("Help GNU %s"), _HELP_GNU_URL);
      break;
    case 24:
      ret = lw6sys_new_sprintf (sys_context, _("Fight software patents %s"), _FFII_URL);
      break;
    case 25:
      ret = lw6sys_new_sprintf (sys_context, "%s: %s", _("Report bugs"), lw6sys_build_get_bugs_url ());
      break;
    default:
      lw6sys_log (sys_context, LW6SYS_LOG_WARNING, _("unable to handle credits id %d"), id);
    }

  return ret;
}
