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

#ifndef LIQUIDWAR6HLP_INTERNAL_H
#define LIQUIDWAR6HLP_INTERNAL_H

#include "hlp.h"

#define _LW6HLP_DEFAULT_TRUE "true"
#define _LW6HLP_DEFAULT_FALSE "false"

#define _LW6HLP_POPULATE_VOID(SYS_CONTEXT,REFERENCES,KEY,ABOUT) lw6sys_hash_set((SYS_CONTEXT),(REFERENCES),(KEY),_lw6hlp_new_reference((SYS_CONTEXT),LW6HLP_TYPE_VOID,(KEY),(ABOUT),NULL,0,0))
#define _LW6HLP_POPULATE_STR(SYS_CONTEXT,REFERENCES,KEY,ABOUT,DEFAULT) lw6sys_hash_set((SYS_CONTEXT),(REFERENCES),(KEY),_lw6hlp_new_reference((SYS_CONTEXT),LW6HLP_TYPE_STR,(KEY),(ABOUT),(DEFAULT),0,0))
#define _LW6HLP_POPULATE_INT(SYS_CONTEXT,REFERENCES,KEY,ABOUT,DEFAULT,MIN,MAX) lw6sys_hash_set((SYS_CONTEXT),(REFERENCES),(KEY),_lw6hlp_new_reference((SYS_CONTEXT),LW6HLP_TYPE_INT,(KEY),(ABOUT),LW6SYS_QUOTE(DEFAULT),(MIN),(MAX)))
#define _LW6HLP_POPULATE_FLOAT(SYS_CONTEXT,REFERENCES,KEY,ABOUT,DEFAULT,MIN,MAX) lw6sys_hash_set((SYS_CONTEXT),(REFERENCES),(KEY),_lw6hlp_new_reference((SYS_CONTEXT),LW6HLP_TYPE_FLOAT,(KEY),(ABOUT),LW6SYS_QUOTE(DEFAULT),(MIN),(MAX)))
#define _LW6HLP_POPULATE_BOOL(SYS_CONTEXT,REFERENCES,KEY,ABOUT,DEFAULT) lw6sys_hash_set((SYS_CONTEXT),(REFERENCES),(KEY),_lw6hlp_new_reference((SYS_CONTEXT),LW6HLP_TYPE_BOOL,(KEY),(ABOUT),_lw6hlp_true_false(DEFAULT),0,0))
#define _LW6HLP_POPULATE_COLOR(SYS_CONTEXT,REFERENCES,KEY,ABOUT,DEFAULT) lw6sys_hash_set((SYS_CONTEXT),(REFERENCES),(KEY),_lw6hlp_new_reference((SYS_CONTEXT),LW6HLP_TYPE_COLOR,(KEY),(ABOUT),(DEFAULT),0,0))

typedef struct _lw6hlp_reference_entry_s
{
  lw6hlp_type_t type;
  const char *key;
  const char *about;
  const char *default_value;
  int min_value;
  int max_value;
} _lw6hlp_reference_entry_t;

/* hlp-ref.c */
extern char *_lw6hlp_true_false (int b);
extern _lw6hlp_reference_entry_t *_lw6hlp_new_reference (lw6sys_context_t * sys_context, lw6hlp_type_t type, const char *key, const char *about,
							 const char *default_value, int min_value, int max_value);
extern _lw6hlp_reference_entry_t *_lw6hlp_reference_get_entry (lw6sys_context_t * sys_context, const char *key);
extern int _lw6hlp_populate_references (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refquick.c */
extern int _lw6hlp_populate_references_quick (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refdoc.c */
extern int _lw6hlp_populate_references_doc (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refshow.c */
extern int _lw6hlp_populate_references_show (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refpath.c */
extern int _lw6hlp_populate_references_path (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refplayers.c */
extern int _lw6hlp_populate_references_players (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refinput.c */
extern int _lw6hlp_populate_references_input (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refgraphics.c */
extern int _lw6hlp_populate_references_graphics (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refsound.c */
extern int _lw6hlp_populate_references_sound (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refnetwork.c */
extern int _lw6hlp_populate_references_network (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refmap.c */
extern int _lw6hlp_populate_references_map (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refmaprules.c */
extern int _lw6hlp_populate_references_map_rules (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refmaphints.c */
extern int _lw6hlp_populate_references_map_hints (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refmapstyle.c */
extern int _lw6hlp_populate_references_map_style (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refmapteams.c */
extern int _lw6hlp_populate_references_map_teams (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-reffuncs.c */
extern int _lw6hlp_populate_references_funcs (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refhooks.c */
extern int _lw6hlp_populate_references_hooks (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refadvanced.c */
extern int _lw6hlp_populate_references_advanced (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refaliases.c */
extern int _lw6hlp_populate_references_aliases (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refteamcolors.c */
extern int _lw6hlp_populate_references_team_colors (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

/* hlp-refweapons.c */
extern int _lw6hlp_populate_references_weapons (lw6sys_context_t * sys_context, lw6sys_hash_t * references);

#endif
